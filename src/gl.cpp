//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_color_defs.hpp"
#include "my_game.hpp"
#include "my_gl.hpp"
#include "my_globals.hpp"
#include "my_level.hpp"
#include "my_main.hpp"
#include "my_ptrcheck.hpp"
#include "my_size.hpp"
#include "my_tile.hpp"

ENUM_DEF_C(FBO_ENUM, FboEnum)

static bool in_2d_mode;

float   glapi_last_tex_right;
float   glapi_last_tex_bottom;
GLshort glapi_last_right;
GLshort glapi_last_bottom;

GLuint       g_fbo_id[ FBO_ENUM_MAX ];
GLuint       g_fbo_tex_id[ FBO_ENUM_MAX ];
GLuint       g_render_buf_id[ FBO_ENUM_MAX ];
static isize g_fbo_size[ FBO_ENUM_MAX ];

static FboEnum fbo_locked = FBO_NONE;
static FboEnum fbo_last   = FBO_NONE;

// NOLINTBEGIN
void MessageCallback(GLenum /*source*/, GLenum type, GLuint id, GLenum severity, GLsizei /*length*/, const GLchar *message,
                     const void * /*userParam*/)
{
  TRACE();
  con("GL CALLBACK: type = 0x%x, severity = 0x%x, message = %s\n", type, severity, message);
}
// NOLINTEND

void gl_init_2d_mode(Gamep g)
{
  DBG("SDL: Init 2d mode");
  TRACE_INDENT();

  GL_ERROR_CHECK();
  gl_leave_2d_mode(g);

  //
  // Enable Textures
  //
  DBG("GFX: OpenGL enable textures");
  glEnable(GL_TEXTURE_2D);
  GL_ERROR_CHECK();

  //
  // Enable alpha blending for sprites
  //
  DBG("GFX: OpenGL enable blending");
  glEnable(GL_BLEND);
  GL_ERROR_CHECK();
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  GL_ERROR_CHECK();

  //
  // Setup our viewport
  //
  DBG("GFX: OpenGL enable viewport");
  glViewport(0, 0, game_window_pix_width_get(g), game_window_pix_height_get(g));
  GL_ERROR_CHECK();

  //
  // Reset the view
  //
  glLoadIdentity();
  GL_ERROR_CHECK();

  gl_init_fbo(g);

  glLineWidth(1.0);
  GL_ERROR_CHECK();
  glEnable(GL_LINE_SMOOTH);
  GL_ERROR_CHECK();

  log("GFX: OpenGL initialized");
}

void gl_fini_2d_mode(Gamep g)
{
  DBG("SDL: fini 2d mode");
  TRACE_INDENT();

  GL_ERROR_CHECK();

  gl_leave_2d_mode(g);
  gl_fini_fbo(g);
  GL_ERROR_CHECK();
}

void gl_enter_2d_mode(Gamep g)
{
  if (compiler_unused) {
    DBG("SDL: enter 2d mode");
  }
  TRACE();

  GL_ERROR_CHECK();

  gl_leave_2d_mode(g);

  //
  // Change to the projection matrix and set our viewing volume.
  //
  glMatrixMode(GL_PROJECTION);
  GL_ERROR_CHECK();
  glPushMatrix();
  GL_ERROR_CHECK();

  //
  // Reset the view
  //
  glLoadIdentity();
  GL_ERROR_CHECK();

  //
  // 2D projection
  //
  if ((game_map_fbo_width_get(g) == 0) || (game_map_fbo_height_get(g) == 0)) {
    log("cannot call glOrtho(%d,%d)", game_map_fbo_width_get(g), game_map_fbo_height_get(g));
    return;
  }

  glOrtho(0,                          // left
          game_map_fbo_width_get(g),  // right
          game_map_fbo_height_get(g), // bottom
          0,                          // top
          -1200.0, 1200.0);
  GL_ERROR_CHECK();

  //
  // Make sure we're changing the model view and not the projection
  //
  glMatrixMode(GL_MODELVIEW);
  GL_ERROR_CHECK();
  glPushMatrix();
  GL_ERROR_CHECK();

  //
  // Reset the view
  //
  glLoadIdentity();
  GL_ERROR_CHECK();
  in_2d_mode = true;
}

void gl_enter_2d_mode(Gamep g, int w, int h)
{
  if (compiler_unused) {
    DBG("SDL: enter 2d mode %ux%u", w, h);
  }
  TRACE();

  GL_ERROR_CHECK();

  gl_leave_2d_mode(g);

  //
  // Change to the projection matrix and set our viewing volume.
  //
  glMatrixMode(GL_PROJECTION);
  GL_ERROR_CHECK();
  glPushMatrix();
  GL_ERROR_CHECK();

  //
  // Reset the view
  //
  glLoadIdentity();
  GL_ERROR_CHECK();

  //
  // 2D projection
  //
  glOrtho(0, // left
          w, // right
          h, // bottom
          0, // top
          -1200.0, 1200.0);
  GL_ERROR_CHECK();

  glViewport(0, 0, w, h);
  GL_ERROR_CHECK();

  //
  // Make sure we're changing the model view and not the projection
  //
  glMatrixMode(GL_MODELVIEW);
  GL_ERROR_CHECK();
  glPushMatrix();
  GL_ERROR_CHECK();

  //
  // Reset the view
  //
  glLoadIdentity();
  GL_ERROR_CHECK();
  in_2d_mode = true;
}

void gl_leave_2d_mode(Gamep g)
{
  if (! in_2d_mode) {
    return;
  }

  if (compiler_unused) {
    DBG("SDL: leave 2d mode");
  }
  TRACE();

  in_2d_mode = false;

  glMatrixMode(GL_MODELVIEW);
  GL_ERROR_CHECK();
  glPopMatrix();
  GL_ERROR_CHECK();

  glMatrixMode(GL_PROJECTION);
  GL_ERROR_CHECK();
  glPopMatrix();
  GL_ERROR_CHECK();
}

void gl_clear()
{
  TRACE();

  GLCOLOR(WHITE);
  glClearColor(0, 0, 0, 0);
  glClear(GL_COLOR_BUFFER_BIT);
}

static void gl_init_fbo_(FboEnum fbo, GLuint *render_buf_id, GLuint *fbo_id, GLuint *fbo_tex_id, GLuint tex_width, GLuint tex_height)
{
  TRACE();
  DBG2("GFX: create FBO, size %dx%d", tex_width, tex_height);

  GL_ERROR_CHECK();

  DBG2("OpenGl: - glGenTextures");
  if (static_cast< bool >(*fbo_tex_id)) {
    DBG2("OpenGl: - glDeleteTextures");
    glDeleteTextures(1, fbo_tex_id);
    GL_ERROR_CHECK();
    *fbo_tex_id = 0;
  }

  if (static_cast< bool >(*fbo_id)) {
    DBG2("OpenGl: - glDeleteRenderbuffers");
    glDeleteRenderbuffers_EXT(1, fbo_id);
    GL_ERROR_CHECK();
    *fbo_id = 0;
  }

  glGenTextures(1, fbo_tex_id);
  GL_ERROR_CHECK();

  DBG2("OpenGl: - glBindTexture");
  glBindTexture(GL_TEXTURE_2D, *fbo_tex_id);
  GL_ERROR_CHECK();

  DBG2("OpenGl: - glTexParameterf (clamping)");
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  GL_ERROR_CHECK();
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  GL_ERROR_CHECK();

  switch (fbo) {
    case FBO_MAP_BG :
    case FBO_MAP_FG :
    case FBO_MAP_FG_OVERLAY :
    case FBO_MAP_LIGHT :
    case FBO_MAP_BG_MERGED :
    case FBO_MAP_FG_MERGED :
    case FBO_WID :
    case FBO_FINAL :
    case FBO_MINIMAP_LEVEL :
    case FBO_SPRITE1 :
    default :
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      break;
  }

  DBG2("OpenGl: - glTexImage2D");
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, tex_width, tex_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
  GL_ERROR_CHECK();
  glBindTexture(GL_TEXTURE_2D, 0);
  GL_ERROR_CHECK();

  //
  // Create a render buffer object.
  //
#ifdef _WIN32
  if (! glFramebufferTexture2D_EXT) {
    CROAK("GlGenRenderbuffers_EXT is not present; fatal");
    return;
  }
#endif

  DBG2("OpenGl: - glGenRenderbuffers_EXT");
  glGenRenderbuffers_EXT(1, render_buf_id);
  GL_ERROR_CHECK();

  DBG2("OpenGl: - glBindRenderbuffer_EXT");
  glBindRenderbuffer_EXT(GL_RENDERBUFFER, *render_buf_id);
  GL_ERROR_CHECK();

  DBG2("OpenGl: - glRenderbufferStorage_EXT");
  glRenderbufferStorage_EXT(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, tex_width, tex_height);
  GL_ERROR_CHECK();

  DBG2("OpenGl: - glBindRenderbuffer_EXT");
  glBindRenderbuffer_EXT(GL_RENDERBUFFER, 0);
  GL_ERROR_CHECK();

  //
  // Create a frame buffer object.
  //
  DBG2("OpenGl: - glGenFramebuffers_EXT");
  glGenFramebuffers_EXT(1, fbo_id);
  GL_ERROR_CHECK();

  DBG2("OpenGl: - glBindFramebuffer_EXT");
  glBindFramebuffer_EXT(GL_FRAMEBUFFER, *fbo_id);
  GL_ERROR_CHECK();

  //
  // Attach the texture to FBO color attachment point
  //
  DBG2("OpenGl: - glFramebufferTexture2D_EXT");
  glFramebufferTexture2D_EXT(GL_FRAMEBUFFER,       // 1. fbo target: GL_FRAMEBUFFER
                             GL_COLOR_ATTACHMENT0, // 2. attachment point
                             GL_TEXTURE_2D,        // 3. tex target: GL_TEXTURE_2D
                             *fbo_tex_id,          // 4. tex ID
                             0);                   // 5. mipmap level: 0(base)
  GL_ERROR_CHECK();

  //
  // Attach the renderbuffer to depth attachment point
  //
  DBG2("OpenGl: - glFramebufferRenderbuffer_EXT");
  glFramebufferRenderbuffer_EXT(GL_FRAMEBUFFER,      // 1. fbo target: GL_FRAMEBUFFER
                                GL_DEPTH_ATTACHMENT, // 2. attachment point
                                GL_RENDERBUFFER,     // 3. rbo target: GL_RENDERBUFFER
                                *render_buf_id);     // 4. rbo ID
  GL_ERROR_CHECK();

  //
  // Check FBO status
  //
  DBG2("OpenGl: - glCheckFramebufferStatus_EXT");
  auto status = glCheckFramebufferStatus_EXT(GL_FRAMEBUFFER);
  if ((static_cast< bool >(status)) && (status != GL_FRAMEBUFFER_COMPLETE)) {
    ERR("failed to create framebuffer, error: %d/0x%x", status, status);

#ifdef GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT
    if (status == GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT) {
      log("openGl: - GFX: GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT Not all framebuffer attachment points "
          "are framebuffer attachment complete. This means that at least one attachment point with a "
          "renderbuffer or texture attached has its attached object no longer in existence or has an "
          "attached image with a width or height of zero, or the color attachment point has a "
          "non-color-renderable image attached, or the depth attachment point has a non-depth-renderable "
          "image attached, or the stencil attachment point has a non-stencil-renderable image attached. "
          "Color-renderable formats include GL_RGBA4, GL_RGB5_A1, and GL_RGB565. GL_DEPTH_COMPONENT16 is "
          "the only depth-renderable format. GL_STENCIL_INDEX8 is the only stencil-renderable format.");
    }
#endif
#ifdef GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS
    if (status == GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS) {
      log("openGl: - GFX: GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS Not all attached images have the same "
          "width and height.");
    }
#endif
#ifdef GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT
    if (status == GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT) {
      log("openGl: - GFX: GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT No images are attached to the framebuffer.");
    }
#endif
#ifdef GL_FRAMEBUFFER_UNSUPPORTED
    if (status == GL_FRAMEBUFFER_UNSUPPORTED) {
      log("openGl: - GFX: GL_FRAMEBUFFER_UNSUPPORTED The combination of internal formats of the attached images "
          "violates an implementation-dependent set of restrictions.");
    }
#endif
#ifdef GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER
    if (status == GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER) {
      log("openGl: - GFX: GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER");
    }
#endif
#ifdef GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER
    if (status == GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER) {
      log("openGl: - GFX: GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER");
    }
#endif
  }
  if (! static_cast< bool >(status)) {
    glGetError();
  }
  GL_ERROR_CHECK();

  // switch back to window-system-provided framebuffer
  DBG2("OpenGl: - glBindFramebuffer_EXT");
  glBindFramebuffer_EXT(GL_FRAMEBUFFER, 0);
  GL_ERROR_CHECK();
}

static void gl_fini_fbo_(GLuint *fbo_id, GLuint *fbo_tex_id, GLuint tex_width, GLuint tex_height)
{
  TRACE();
  DBG2("GFX: destroy FBO, size %dx%d", tex_width, tex_height);

  GL_ERROR_CHECK();

  DBG2("OpenGl: - glGenTextures");
  if (static_cast< bool >(*fbo_tex_id)) {
    DBG2("OpenGl: - glDeleteTextures");
    glDeleteTextures(1, fbo_tex_id);
    GL_ERROR_CHECK();
    *fbo_tex_id = 0;
  }

  if (static_cast< bool >(*fbo_id)) {
    DBG2("OpenGl: - glDeleteRenderbuffers");
    glDeleteRenderbuffers_EXT(1, fbo_id);
    GL_ERROR_CHECK();
    *fbo_id = 0;
  }

  memset(g_fbo_id, 0, SIZEOF(g_fbo_id));
  memset(g_fbo_tex_id, 0, SIZEOF(g_fbo_tex_id));
  memset(g_render_buf_id, 0, SIZEOF(g_render_buf_id));

  for (auto &i : g_fbo_size) {
    i = isize(0, 0);
  }
}

void gl_init_fbo(Gamep g, FboEnum fbo)
{
  log("GFX: OpenGL create FBO %s", FboEnum_to_string(fbo).c_str());
  TRACE_INDENT();

  GL_ERROR_CHECK();

  static bool first = true;

  FOR_ALL_FBO(i)
  {
    int tex_width  = 0;
    int tex_height = 0;

    //
    // Filter to a specific fbo?
    //
    if (fbo != FBO_NONE) {
      if (i != fbo) {
        continue;
      }
    }

    // old size check
    fbo_get_size(g, i, tex_width, tex_height);

    //
    // If no change in size then do not reset the FBO
    //
    if (g_fbo_size[ i ] == isize(tex_width, tex_height)) {
      DBG("no change in size for FBO %u, %ux%u", i, tex_width, tex_height);
      // continue;
    }
    if (g_fbo_size[ i ].w != 0) {
      DBG("Change in size for FBO %u, %ux%u -> %ux%u", i, g_fbo_size[ i ].w, g_fbo_size[ i ].h, tex_width, tex_height);
    } else {
      DBG("Init FBO %u, %ux%u", i, tex_width, tex_height);
    }

    gl_init_fbo_(i, &g_render_buf_id[ i ], &g_fbo_id[ i ], &g_fbo_tex_id[ i ], tex_width, tex_height);
    g_fbo_size[ i ] = isize(tex_width, tex_height);

    if (compiler_unused) {
      gl_enter_2d_mode(g, tex_width, tex_height);
      blit_fbo_bind(i);
      {
        gl_clear();
      }
      blit_fbo_unbind();
    }

    if (fbo == FBO_NONE) {
      if (first) {
        tile_from_fbo(g, i);
      }
    }
  }

  DBG("GFX: OpenGL created FBOs");
  GL_ERROR_CHECK();

  if (fbo == FBO_NONE) {
    first = false;
  }
}

void gl_fini_fbo(Gamep g)
{
  log("GFX: OpenGL destroy FBOs");
  TRACE_INDENT();

  GL_ERROR_CHECK();

  FOR_ALL_FBO(i)
  {
    int tex_width  = 0;
    int tex_height = 0;

    fbo_get_size(g, i, tex_width, tex_height);

    gl_fini_fbo_(&g_fbo_id[ i ], &g_fbo_tex_id[ i ], tex_width, tex_height);
    g_fbo_size[ i ] = isize(0, 0);
  }

  DBG("GFX: OpenGL destroyed FBOs");
  GL_ERROR_CHECK();
}

void fbo_get_size(Gamep g, FboEnum fbo, int &w, int &h)
{
  TRACE();

  switch (fbo) {
    case FBO_MAP_BG :
    case FBO_MAP_FG :
    case FBO_MAP_FG_OVERLAY :
      w = game_map_fbo_width_get(g);
      h = game_map_fbo_height_get(g);
      break;
    case FBO_MAP_LIGHT :
      w = static_cast< int >(MAP_WIDTH) * static_cast< int >(TILE_WIDTH);
      h = static_cast< int >(MAP_HEIGHT) * static_cast< int >(TILE_HEIGHT);
      break;
    case FBO_MAP_BG_MERGED :
    case FBO_MAP_FG_MERGED :
    case FBO_WID :
    case FBO_FINAL :
      w = game_window_pix_width_get(g);
      h = game_window_pix_height_get(g);
      break;
    case FBO_MINIMAP_LEVEL :
      w = MAP_WIDTH;
      h = MAP_HEIGHT;
      break;
    case FBO_SPRITE1 :
      w = game_window_pix_width_get(g);
      h = game_window_pix_height_get(g);
      break;
    default : //
      CROAK("Need size for fbo %d", fbo);
      break;
  }
}

void fbo_get_curr_size(Gamep g, int &w, int &h) { fbo_get_size(g, fbo_last, w, h); }

void blit_fbo(Gamep g, FboEnum fbo)
{
  TRACE_DEBUG();

  int tex_width  = 0;
  int tex_height = 0;
  fbo_get_size(g, fbo, tex_width, tex_height);
  blit_init();
  blit(g_fbo_tex_id[ fbo ], 0.0, 1.0, 1.0, 0.0, 0, 0, tex_width, tex_height, WHITE);
  blit_flush();
}

void blit_fbo(Gamep g, FboEnum fbo, int tl_x, int tl_y, int br_x, int br_y)
{
  TRACE_DEBUG();

  blit_init();
  blit(g_fbo_tex_id[ fbo ], 0.0, 1.0, 1.0, 0.0, tl_x, tl_y, br_x, br_y, WHITE);
  blit_flush();
}

void blit_fbo_push(FboEnum fbo)
{
  TRACE_DEBUG();
  glBindFramebuffer_EXT(GL_FRAMEBUFFER, g_fbo_id[ fbo ]);
}

void blit_fbo_pop()
{
  TRACE_DEBUG();
  glBindFramebuffer_EXT(GL_FRAMEBUFFER, g_fbo_id[ fbo_last ]);
}

void blit_fbo_bind(FboEnum fbo)
{
  TRACE_DEBUG();

  fbo_last = fbo;
  if (fbo_locked != FBO_NONE) {
    CROAK("Attempt to bind to another FBO %d when locked", fbo);
  }
  glBindFramebuffer_EXT(GL_FRAMEBUFFER, g_fbo_id[ fbo ]);
}

void blit_fbo_unbind()
{
  TRACE_DEBUG();

  if (fbo_locked != FBO_NONE) {
    CROAK("Attempt to unbind when locked");
  }
  glBindFramebuffer_EXT(GL_FRAMEBUFFER, 0);
}

void blit_fbo_bind_locked(FboEnum fbo)
{
  TRACE_DEBUG();

  fbo_last   = fbo;
  fbo_locked = fbo;
  glBindFramebuffer_EXT(GL_FRAMEBUFFER, g_fbo_id[ fbo ]);
}

void blit_fbo_unbind_locked()
{
  TRACE_DEBUG();

  fbo_locked = FBO_NONE;
  glBindFramebuffer_EXT(GL_FRAMEBUFFER, 0);
}

//
// x and y per element.
//
#define NUMBER_DIMENSIONS_PER_COORD_2D 2

//
// r,g,b,a per element
//
#define NUMBER_COMPONENTS_PER_COLOR 4

uint32_t NUMBER_BYTES_PER_VERTICE_2D = (SIZEOF(GLfloat) * NUMBER_DIMENSIONS_PER_COORD_2D)
                                     + (SIZEOF(GLshort) * NUMBER_DIMENSIONS_PER_COORD_2D) + (SIZEOF(GLubyte) * NUMBER_COMPONENTS_PER_COLOR);
//
// Two arrays, xy and uv.
//
float *gl_array_buf;
float *gl_array_buf_end;

//
// Where we are currently up to in writing to these buffers.
//
GLfloat *bufp;
GLfloat *bufp_end;
int      buf_tex;

void blit_init()
{
  TRACE_DEBUG();

  buf_tex = 0;

  if (gl_array_buf != nullptr) {
    bufp = gl_array_buf;
    return;
  }

  //
  // Our array size requirements.
  //
  uint32_t gl_array_size_required = 0;

  //
  // If the screen size has changed or this is the first run, allocate our
  // buffer if our size requirements have changed.
  //
  gl_array_size_required = 32 * 1024 * 1024;

  gl_array_buf = static_cast< __typeof__(gl_array_buf) >(MYZALLOC(gl_array_size_required, "GL xy buffer"));

  //
  // Make the end a bit smaller so we have plenty of headroom.
  //
  gl_array_buf_end
      = reinterpret_cast< __typeof__(gl_array_buf_end) >(reinterpret_cast< char * >(gl_array_buf)) + ((gl_array_size_required * 2) / 3);

  bufp     = gl_array_buf;
  bufp_end = gl_array_buf_end;
}

void blit_fini()
{
  TRACE_DEBUG();

  if (gl_array_buf != nullptr) {
    MYFREE(gl_array_buf);
    gl_array_buf = nullptr;
  }
}

void blit_flush()
{
  TRACE_DEBUG();

  if (gl_array_buf == bufp) [[unlikely]] {
    return;
  }

  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  glEnableClientState(GL_COLOR_ARRAY);

  static long nvertices;

  nvertices = (reinterpret_cast< char * >(bufp) - reinterpret_cast< char * >(gl_array_buf)) / NUMBER_BYTES_PER_VERTICE_2D;

  glBindTexture(GL_TEXTURE_2D, buf_tex);

  glTexCoordPointer(NUMBER_DIMENSIONS_PER_COORD_2D, // (u,v)
                    GL_FLOAT, NUMBER_BYTES_PER_VERTICE_2D, gl_array_buf);

  glVertexPointer(NUMBER_DIMENSIONS_PER_COORD_2D, // (x,y)
                  GL_SHORT, NUMBER_BYTES_PER_VERTICE_2D,
                  (reinterpret_cast< char * >(gl_array_buf))
                      + (static_cast< ptrdiff_t >(SIZEOF(GLfloat) * // skip (u,v)
                                                  NUMBER_DIMENSIONS_PER_COORD_2D)));

  glColorPointer(NUMBER_COMPONENTS_PER_COLOR, // (r,g,b,a)
                 GL_UNSIGNED_BYTE, NUMBER_BYTES_PER_VERTICE_2D,
                 (reinterpret_cast< char * >(gl_array_buf))
                     + (static_cast< ptrdiff_t >(SIZEOF(GLshort) * // skip (x,y)
                                                 NUMBER_DIMENSIONS_PER_COORD_2D))
                     + (static_cast< ptrdiff_t >(SIZEOF(GLfloat) * // skip (u,v)
                                                 NUMBER_DIMENSIONS_PER_COORD_2D)));

#ifdef DEBUG_BUILD
  GL_ERROR_CHECK();
#endif

  glDrawArrays(GL_TRIANGLE_STRIP, 0, static_cast< GLsizei >(nvertices));
  glBindTexture(GL_TEXTURE_2D, 0);

  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_COLOR_ARRAY);

  blit_init();
}

void blit_flush_triangle_fan() { blit_flush_triangle_fan(gl_array_buf, bufp); }

void blit_flush_colored_triangle_fan()
{
  TRACE_DEBUG();

  blit_flush_colored_triangle_fan(gl_array_buf, bufp);
}

void blit_flush_colored_triangle_fan(float *b, const float *e)
{
  TRACE_DEBUG();

  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_COLOR_ARRAY);

  static long nvertices;

  static const GLsizei stride = (SIZEOF(GLshort) * NUMBER_DIMENSIONS_PER_COORD_2D) + (SIZEOF(GLubyte) * NUMBER_COMPONENTS_PER_COLOR);

  nvertices = ((char *) e - reinterpret_cast< char * >(b)) / stride;

  glVertexPointer(NUMBER_DIMENSIONS_PER_COORD_2D, // (x,y)
                  GL_SHORT, stride, b);

  glColorPointer(NUMBER_COMPONENTS_PER_COLOR, // (r,g,b,a)
                 GL_UNSIGNED_BYTE, stride,
                 (reinterpret_cast< char * >(b))
                     + (static_cast< ptrdiff_t >(SIZEOF(GLshort) * // skip (x,y)
                                                 NUMBER_DIMENSIONS_PER_COORD_2D)));

  GL_ERROR_CHECK();
  glDrawArrays(GL_TRIANGLE_FAN, 0, static_cast< GLsizei >(nvertices));

  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_COLOR_ARRAY);

  blit_init();
}

void blit_flush_triangle_fan(float *b, const float *e)
{
  TRACE_DEBUG();

  glEnableClientState(GL_VERTEX_ARRAY);

  static long nvertices;

  static const GLsizei stride = SIZEOF(GLshort) * NUMBER_DIMENSIONS_PER_COORD_2D;

  nvertices = ((char *) e - reinterpret_cast< char * >(b)) / stride;

  glVertexPointer(NUMBER_DIMENSIONS_PER_COORD_2D, // (x,y)
                  GL_SHORT, stride, b);

  GL_ERROR_CHECK();
  glDrawArrays(GL_TRIANGLE_FAN, 0, static_cast< GLsizei >(nvertices));

  glDisableClientState(GL_VERTEX_ARRAY);

  blit_init();
}

void gl_blitquad(GLshort left, GLshort top, GLshort right, GLshort bottom)
{
  TRACE_DEBUG();

  GLshort  xy[ 4 * 2 ];
  GLshort *xyp = xy; // NOLINT

  Vertex2(left, top);
  Vertex2(right, top);
  Vertex2(left, bottom);
  Vertex2(right, bottom);

  glEnableClientState(GL_VERTEX_ARRAY);

  glVertexPointer(2, GL_SHORT, 0, xy);
  GL_ERROR_CHECK();
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  glDisableClientState(GL_VERTEX_ARRAY);
}

void gl_blitquad(const spoint tl, const spoint tr, const spoint bl, const spoint br)
{
  TRACE_DEBUG();

  GLshort  xy[ 4 * 2 ];
  GLshort *xyp = xy; // NOLINT

  Vertex2(tl.x, tl.y);
  Vertex2(tr.x, tr.y);
  Vertex2(bl.x, bl.y);
  Vertex2(br.x, br.y);

  glEnableClientState(GL_VERTEX_ARRAY);

  glVertexPointer(2, GL_SHORT, 0, xy);
  GL_ERROR_CHECK();
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  glDisableClientState(GL_VERTEX_ARRAY);
}

void gl_blitsquare(GLshort left, GLshort top, GLshort right, GLshort bottom)
{
  TRACE_DEBUG();

  GLshort  xy[ 4 * 2 ];
  GLshort *xyp = xy; // NOLINT

  Vertex2(left, top);
  Vertex2(right, top);
  Vertex2(right, bottom);
  Vertex2(left, bottom);

  glEnableClientState(GL_VERTEX_ARRAY);

  glVertexPointer(2, GL_SHORT, 0, xy);
  GL_ERROR_CHECK();
  glDrawArrays(GL_LINE_LOOP, 0, 4);

  glDisableClientState(GL_VERTEX_ARRAY);
}

void gl_blitline(GLshort left, GLshort top, GLshort right, GLshort bottom)
{
  TRACE_DEBUG();

  GLshort  xy[ 2 * 2 ];
  GLshort *xyp = xy; // NOLINT

  Vertex2(left, top);
  Vertex2(right, bottom);

  glEnableClientState(GL_VERTEX_ARRAY);

  glVertexPointer(2, GL_SHORT, 0, xy);
  GL_ERROR_CHECK();
  glDrawArrays(GL_LINES, 0, 2);

  glDisableClientState(GL_VERTEX_ARRAY);
}

#ifdef _WIN32

HDC      hDC;
HGLRC    hGLRC;
HPALETTE hPalette;

const char g_szClassName[] = "myWindowClass";

PFNGLCREATEPROGRAMPROC           glCreateProgram_EXT;
PFNGLDEBUGMESSAGECALLBACKPROC    glDebugMessageCallback_EXT;
PFNGLDELETEPROGRAMPROC           glDeleteProgram_EXT;
PFNGLISPROGRAMPROC               glIsProgram_EXT;
PFNGLCREATESHADERPROC            glCreateShader_EXT;
PFNGLDELETESHADERPROC            glDeleteShader_EXT;
PFNGLSHADERSOURCEPROC            glShaderSource_EXT;
PFNGLCOMPILESHADERPROC           glCompileShader_EXT;
PFNGLATTACHSHADERPROC            glAttachShader_EXT;
PFNGLDETACHSHADERPROC            glDetachShader_EXT;
PFNGLGETATTACHEDSHADERSPROC      glGetAttachedShaders_EXT;
PFNGLLINKPROGRAMPROC             glLinkProgram_EXT;
PFNGLUSEPROGRAMPROC              glUseProgram_EXT;
PFNGLGETSHADERINFOLOGPROC        glGetShaderInfoLog_EXT;
PFNGLGETPROGRAMINFOLOGPROC       glGetProgramInfoLog_EXT;
PFNGLGETUNIFORMLOCATIONPROC      glGetUniformLocation_EXT;
PFNGLUNIFORM1FPROC               glUniform1f_EXT;
PFNGLUNIFORM1IPROC               glUniform1i_EXT;
PFNGLUNIFORM2FVPROC              glUniform2fv_EXT;
PFNGLUNIFORM3FVPROC              glUniform3fv_EXT;
PFNGLGENERATEMIPMAPPROC          glGenerateMipmap_EXT;
PFNGLGENFRAMEBUFFERSPROC         glGenFramebuffers_EXT;
PFNGLDELETEFRAMEBUFFERSPROC      glDeleteFramebuffers_EXT;
PFNGLBINDFRAMEBUFFERPROC         glBindFramebuffer_EXT;
PFNGLGENRENDERBUFFERSPROC        glGenRenderbuffers_EXT;
PFNGLDELETERENDERBUFFERSPROC     glDeleteRenderbuffers_EXT;
PFNGLBINDRENDERBUFFERPROC        glBindRenderbuffer_EXT;
PFNGLRENDERBUFFERSTORAGEPROC     glRenderbufferStorage_EXT;
PFNGLFRAMEBUFFERRENDERBUFFERPROC glFramebufferRenderbuffer_EXT;
PFNGLFRAMEBUFFERTEXTURE2DPROC    glFramebufferTexture2D_EXT;
PFNGLCHECKFRAMEBUFFERSTATUSPROC  glCheckFramebufferStatus_EXT;
PFNGLGENBUFFERSARBPROC           glGenBuffersARB_EXT;
PFNGLBINDBUFFERARBPROC           glBindBufferARB_EXT;
PFNGLBUFFERDATAARBPROC           glBufferDataARB_EXT;
PFNGLDELETEBUFFERSARBPROC        glDeleteBuffersARB_EXT;

static void gl_ext_load(void)
{
  TRACE();
  glDebugMessageCallback_EXT = (__typeof__(glDebugMessageCallback_EXT)) (void *) wglGetProcAddress("glDebugMessageCallback");
  if (! glDebugMessageCallback_EXT) {
    log("openGl: - glDebugMessageCallback_EXT - NOT present");
  } else {
    log("openGl: - glDebugMessageCallback_EXT - present");
  }

  if (glDebugMessageCallback_EXT) {
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback_EXT(MessageCallback, 0);
  }

  TRACE();
  glCreateProgram_EXT = (__typeof__(glCreateProgram_EXT)) (void *) wglGetProcAddress("glCreateProgram");
  if (! glCreateProgram_EXT) {
    log("openGl: - glCreateProgram_EXT - NOT present");
  } else {
    log("openGl: - glCreateProgram_EXT - present");
  }

  glDeleteProgram_EXT = (__typeof__(glDeleteProgram_EXT)) (void *) wglGetProcAddress("glDeleteProgram");
  if (! glDeleteProgram_EXT) {
    log("openGl: - glDeleteProgram_EXT - NOT present");
  } else {
    log("openGl: - glDeleteProgram_EXT - present");
  }

  glIsProgram_EXT = (__typeof__(glIsProgram_EXT)) (void *) wglGetProcAddress("glIsProgram");
  if (! glIsProgram_EXT) {
    log("openGl: - glIsProgram_EXT - NOT present");
  } else {
    log("openGl: - glIsProgram_EXT - present");
  }

  glCreateShader_EXT = (__typeof__(glCreateShader_EXT)) (void *) wglGetProcAddress("glCreateShader");
  if (! glCreateShader_EXT) {
    log("openGl: - glCreateShader_EXT - NOT present");
  } else {
    log("openGl: - glCreateShader_EXT - present");
  }

  glDeleteShader_EXT = (__typeof__(glDeleteShader_EXT)) (void *) wglGetProcAddress("glDeleteShader");
  if (! glDeleteShader_EXT) {
    log("openGl: - glDeleteShader_EXT - NOT present");
  } else {
    log("openGl: - glDeleteShader_EXT - present");
  }

  glShaderSource_EXT = (__typeof__(glShaderSource_EXT)) (void *) wglGetProcAddress("glShaderSource");
  if (! glShaderSource_EXT) {
    log("openGl: - glShaderSource_EXT - NOT present");
  } else {
    log("openGl: - glShaderSource_EXT - present");
  }

  glCompileShader_EXT = (__typeof__(glCompileShader_EXT)) (void *) wglGetProcAddress("glCompileShader");
  if (! glCompileShader_EXT) {
    log("openGl: - glCompileShader_EXT - NOT present");
  } else {
    log("openGl: - glCompileShader_EXT - present");
  }

  glAttachShader_EXT = (__typeof__(glAttachShader_EXT)) (void *) wglGetProcAddress("glAttachShader");
  if (! glAttachShader_EXT) {
    log("openGl: - glAttachShader_EXT - NOT present");
  } else {
    log("openGl: - glAttachShader_EXT - present");
  }

  glDetachShader_EXT = (__typeof__(glDetachShader_EXT)) (void *) wglGetProcAddress("glDetachShader");
  if (! glDetachShader_EXT) {
    log("openGl: - glDetachShader_EXT - NOT present");
  } else {
    log("openGl: - glDetachShader_EXT - present");
  }

  glGetAttachedShaders_EXT = (__typeof__(glGetAttachedShaders_EXT)) (void *) wglGetProcAddress("glGetAttachedShaders");
  if (! glGetAttachedShaders_EXT) {
    log("openGl: - glGetAttachedShaders_EXT - NOT present");
  } else {
    log("openGl: - glGetAttachedShaders_EXT - present");
  }

  glLinkProgram_EXT = (__typeof__(glLinkProgram_EXT)) (void *) wglGetProcAddress("glLinkProgram");
  if (! glLinkProgram_EXT) {
    log("openGl: - glLinkProgram_EXT - NOT present");
  } else {
    log("openGl: - glLinkProgram_EXT - present");
  }

  glUseProgram_EXT = (__typeof__(glUseProgram_EXT)) (void *) wglGetProcAddress("glUseProgram");
  if (! glUseProgram_EXT) {
    log("openGl: - glUseProgram_EXT - NOT present");
  } else {
    log("openGl: - glUseProgram_EXT - present");
  }

  glGetShaderInfoLog_EXT = (__typeof__(glGetShaderInfoLog_EXT)) (void *) wglGetProcAddress("glGetShaderInfoLog");
  if (! glGetShaderInfoLog_EXT) {
    log("openGl: - glGetShaderInfoLog_EXT - NOT present");
  } else {
    log("openGl: - glGetShaderInfoLog_EXT - present");
  }

  glGetProgramInfoLog_EXT = (__typeof__(glGetProgramInfoLog_EXT)) (void *) wglGetProcAddress("glGetProgramInfoLog");
  if (! glGetProgramInfoLog_EXT) {
    log("openGl: - glGetProgramInfoLog_EXT - NOT present");
  } else {
    log("openGl: - glGetProgramInfoLog_EXT - present");
  }

  glGetUniformLocation_EXT = (__typeof__(glGetUniformLocation_EXT)) (void *) wglGetProcAddress("glGetUniformLocation");
  if (! glGetUniformLocation_EXT) {
    log("openGl: - glGetUniformLocation_EXT - NOT present");
  } else {
    log("openGl: - glGetUniformLocation_EXT - present");
  }

  glUniform1f_EXT = (__typeof__(glUniform1f_EXT)) (void *) wglGetProcAddress("glUniform1f");
  if (! glUniform1f_EXT) {
    log("openGl: - glUniform1f_EXT - NOT present");
  } else {
    log("openGl: - glUniform1f_EXT - present");
  }

  glUniform1i_EXT = (__typeof__(glUniform1i_EXT)) (void *) wglGetProcAddress("glUniform1i");
  if (! glUniform1i_EXT) {
    log("openGl: - glUniform1i_EXT - NOT present");
  } else {
    log("openGl: - glUniform1i_EXT - present");
  }

  glUniform2fv_EXT = (__typeof__(glUniform2fv_EXT)) (void *) wglGetProcAddress("glUniform2fv");
  if (! glUniform2fv_EXT) {
    log("openGl: - glUniform2fv_EXT - NOT present");
  } else {
    log("openGl: - glUniform2fv_EXT - present");
  }

  glUniform3fv_EXT = (__typeof__(glUniform3fv_EXT)) (void *) wglGetProcAddress("glUniform3fv");
  if (! glUniform3fv_EXT) {
    log("openGl: - glUniform3fv_EXT - NOT present");
  } else {
    log("openGl: - glUniform3fv_EXT - present");
  }

  glGenerateMipmap_EXT = (__typeof__(glGenerateMipmap_EXT)) (void *) wglGetProcAddress("glGenerateMipmap");
  if (! glGenerateMipmap_EXT) {
    log("openGl: - glGenerateMipmap_EXT - NOT present");
  } else {
    log("openGl: - glGenerateMipmap_EXT - present");
  }

  glGenFramebuffers_EXT = (__typeof__(glGenFramebuffers_EXT)) (void *) wglGetProcAddress("glGenFramebuffers");
  if (! glGenFramebuffers_EXT) {
    log("openGl: - glGenFramebuffers_EXT - NOT present");
  } else {
    log("openGl: - glGenFramebuffers_EXT - present");
  }

  glDeleteFramebuffers_EXT = (__typeof__(glDeleteFramebuffers_EXT)) (void *) wglGetProcAddress("glDeleteFramebuffers");
  if (! glDeleteFramebuffers_EXT) {
    log("openGl: - glDeleteFramebuffers_EXT - NOT present");
  } else {
    log("openGl: - glDeleteFramebuffers_EXT - present");
  }

  glBindFramebuffer_EXT = (__typeof__(glBindFramebuffer_EXT)) (void *) wglGetProcAddress("glBindFramebuffer");
  if (! glBindFramebuffer_EXT) {
    log("openGl: - glBindFramebuffer_EXT - NOT present");
  } else {
    log("openGl: - glBindFramebuffer_EXT - present");
  }

  glGenRenderbuffers_EXT = (__typeof__(glGenRenderbuffers_EXT)) (void *) wglGetProcAddress("glGenRenderbuffers");
  if (! glGenRenderbuffers_EXT) {
    log("openGl: - glGenRenderbuffers_EXT - NOT present");
  } else {
    log("openGl: - glGenRenderbuffers_EXT - present");
  }

  glDeleteRenderbuffers_EXT = (__typeof__(glDeleteRenderbuffers_EXT)) (void *) wglGetProcAddress("glDeleteRenderbuffers");
  if (! glDeleteRenderbuffers_EXT) {
    log("openGl: - glDeleteRenderbuffers_EXT - NOT present");
  } else {
    log("openGl: - glDeleteRenderbuffers_EXT - present");
  }

  glBindRenderbuffer_EXT = (__typeof__(glBindRenderbuffer_EXT)) (void *) wglGetProcAddress("glBindRenderbuffer");
  if (! glBindRenderbuffer_EXT) {
    log("openGl: - glBindRenderbuffer_EXT - NOT present");
  } else {
    log("openGl: - glBindRenderbuffer_EXT - present");
  }

  glRenderbufferStorage_EXT = (__typeof__(glRenderbufferStorage_EXT)) (void *) wglGetProcAddress("glRenderbufferStorage");
  if (! glRenderbufferStorage_EXT) {
    log("openGl: - glRenderbufferStorage_EXT - NOT present");
  } else {
    log("openGl: - glRenderbufferStorage_EXT - present");
  }

  glFramebufferRenderbuffer_EXT = (__typeof__(glFramebufferRenderbuffer_EXT)) (void *) wglGetProcAddress("glFramebufferRenderbuffer");
  if (! glFramebufferRenderbuffer_EXT) {
    log("openGl: - glFramebufferRenderbuffer_EXT - NOT present");
  } else {
    log("openGl: - glFramebufferRenderbuffer_EXT - present");
  }

  glCheckFramebufferStatus_EXT = (__typeof__(glCheckFramebufferStatus_EXT)) (void *) wglGetProcAddress("glCheckFramebufferStatus");
  if (! glCheckFramebufferStatus_EXT) {
    log("openGl: - glCheckFramebufferStatus_EXT - NOT present");
  } else {
    log("openGl: - glCheckFramebufferStatus_EXT - present");
  }

  glFramebufferTexture2D_EXT = (__typeof__(glFramebufferTexture2D_EXT)) (void *) wglGetProcAddress("glFramebufferTexture2D");
  if (! glFramebufferTexture2D_EXT) {
    log("openGl: - glFramebufferTexture2D_EXT - NOT present");
  } else {
    log("openGl: - glFramebufferTexture2D_EXT - present");
  }

  glGenBuffersARB_EXT = (__typeof__(glGenBuffersARB_EXT)) (void *) wglGetProcAddress("glGenBuffersARB");
  if (! glGenBuffersARB_EXT) {
    log("openGl: - glGenBuffersARB_EXT - NOT present");
  } else {
    log("openGl: - glGenBuffersARB_EXT - present");
  }

  glBindBufferARB_EXT = (__typeof__(glBindBufferARB_EXT)) (void *) wglGetProcAddress("glBindBufferARB");
  if (! glBindBufferARB_EXT) {
    log("openGl: - glBindBufferARB_EXT - NOT present");
  } else {
    log("openGl: - glBindBufferARB_EXT - present");
  }

  glDeleteBuffersARB_EXT = (__typeof__(glDeleteBuffersARB_EXT)) (void *) wglGetProcAddress("glDeleteBuffersARB");
  if (! glDeleteBuffersARB_EXT) {
    log("openGl: - glDeleteBuffersARB_EXT - NOT present");
  } else {
    log("openGl: - glDeleteBuffersARB_EXT - present");
  }

  glDeleteBuffersARB_EXT = (__typeof__(glDeleteBuffersARB_EXT)) (void *) wglGetProcAddress("glDeleteBuffersARB");
  if (! glDeleteBuffersARB_EXT) {
    log("openGl: - glDeleteBuffersARB_EXT - NOT present");
  } else {
    log("openGl: - glDeleteBuffersARB_EXT - present");
  }
}

static void setupPixelFormat(HDC this_hdc)
{
  TRACE();
  PIXELFORMATDESCRIPTOR pfd = {
      SIZEOF(PIXELFORMATDESCRIPTOR),                              // size
      1,                                                          // version
      PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER, // support double-buffering
      PFD_TYPE_RGBA,                                              // color type
      16,                                                         // preferred color depth
      0,
      0,
      0,
      0,
      0,
      0, // color bits (ignored)
      0, // no alpha buffer
      0, // alpha bits (ignored)
      0, // no accumulation buffer
      0,
      0,
      0,
      0,              // accum bits (ignored)
      16,             // depth buffer
      0,              // no stencil buffer
      0,              // no auxiliary buffers
      PFD_MAIN_PLANE, // main layer
      0,              // reserved
      0,
      0,
      0, // no layer, visible, damage masks
  };
  int pixelFormat;

  pixelFormat = ChoosePixelFormat(this_hdc, &pfd);
  if (pixelFormat == 0) {
    MessageBox(WindowFromDC(this_hdc), "ChoosePixelFormat failed.", "Error", MB_ICONERROR | MB_OK);
    exit(1);
  }

  if (SetPixelFormat(this_hdc, pixelFormat, &pfd) != TRUE) {
    MessageBox(WindowFromDC(this_hdc), "SetPixelFormat failed.", "Error", MB_ICONERROR | MB_OK);
    exit(1);
  }
}

static void setupPalette(HDC this_hdc)
{
  TRACE();
  int                   pixelFormat = GetPixelFormat(this_hdc);
  PIXELFORMATDESCRIPTOR pfd;
  LOGPALETTE           *pPal;
  int                   paletteSize;

  DescribePixelFormat(this_hdc, pixelFormat, SIZEOF(PIXELFORMATDESCRIPTOR), &pfd);

  if (pfd.dwFlags & PFD_NEED_PALETTE) {
    paletteSize = 1 << pfd.cColorBits;
  } else {
    return;
  }

  pPal                = (LOGPALETTE *) malloc(SIZEOF(LOGPALETTE) + paletteSize * SIZEOF(PALETTEENTRY));
  pPal->palVersion    = 0x300;
  pPal->palNumEntries = paletteSize;

  // build a simple RGB color palette
  {
    int redMask   = (1 << pfd.cRedBits) - 1;
    int greenMask = (1 << pfd.cGreenBits) - 1;
    int blueMask  = (1 << pfd.cBlueBits) - 1;
    int i;

    for (i = 0; i < paletteSize; ++i) {
      pPal->palPalEntry[ i ].peRed   = (((i >> pfd.cRedShift) & redMask) * 255) / redMask;
      pPal->palPalEntry[ i ].peGreen = (((i >> pfd.cGreenShift) & greenMask) * 255) / greenMask;
      pPal->palPalEntry[ i ].peBlue  = (((i >> pfd.cBlueShift) & blueMask) * 255) / blueMask;
      pPal->palPalEntry[ i ].peFlags = 0;
    }
  }

  hPalette = CreatePalette(pPal);
  free(pPal);

  if (hPalette) {
    SelectPalette(this_hdc, hPalette, FALSE);
    RealizePalette(this_hdc);
  }
}

static LRESULT APIENTRY WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  TRACE();
  return DefWindowProc(hWnd, message, wParam, lParam);
}

void gl_ext_init(void)
{
  TRACE();
  WNDCLASSEX wc;
  HWND       hwnd;

  log("GFX: extensions");

  DBG("OpenGl: - GetModuleHandle");
  HINSTANCE hInstance = GetModuleHandle(0);

  wc.cbSize        = SIZEOF(WNDCLASSEX);
  wc.style         = 0;
  wc.lpfnWndProc   = WndProc;
  wc.cbClsExtra    = 0;
  wc.cbWndExtra    = 0;
  wc.hInstance     = hInstance;
  wc.hIcon         = LoadIcon(nullptr, IDI_APPLICATION);
  wc.hCursor       = LoadCursor(nullptr, IDC_ARROW);
  wc.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
  wc.lpszMenuName  = nullptr;
  wc.lpszClassName = g_szClassName;
  wc.hIconSm       = LoadIcon(nullptr, IDI_APPLICATION);

  DBG("OpenGl: - RegisterClassEx");
  if (! RegisterClassEx(&wc)) {
    MessageBox(nullptr, "Window Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
    return;
  }

  DBG("OpenGl: - CreateWindowEx");
  hwnd = CreateWindowEx(WS_EX_CLIENTEDGE, g_szClassName, "gorget startup", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 240, 120,
                        nullptr, nullptr, hInstance, nullptr);

  if (hwnd == nullptr) {
    MessageBox(nullptr, "Window Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
    return;
  }

  ShowWindow(hwnd, 0);
  UpdateWindow(hwnd);

  // initialize OpenGL rendering

  hDC = GetDC(hwnd);
  setupPixelFormat(hDC);
  setupPalette(hDC);
  hGLRC = wglCreateContext(hDC);
  wglMakeCurrent(hDC, hGLRC);

  gl_ext_load();

  // finish OpenGL rendering
  if (hGLRC) {
    wglMakeCurrent(nullptr, nullptr);
    wglDeleteContext(hGLRC);
  }

  if (hPalette) {
    DeleteObject(hPalette);
  }

  ReleaseDC(hwnd, hDC);
}
#else
void gl_ext_init() { TRACE(); }
#endif

void gl_error(GLenum errCode)
{
  TRACE();

  if (errCode == GL_INVALID_ENUM) {
    log("GFX: error, GL_INVALID_ENUM");
  } else if (errCode == GL_INVALID_VALUE) {
    log("GFX: error, GL_INVALID_VALUE");
  } else if (errCode == GL_INVALID_OPERATION) {
    log("GFX: error, GL_INVALID_OPERATION");
  } else if (errCode == GL_STACK_OVERFLOW) {
    log("GFX: error, GL_STACK_OVERFLOW");
  } else if (errCode == GL_STACK_UNDERFLOW) {
    log("GFX: error, GL_STACK_UNDERFLOW");
  } else if (errCode == GL_OUT_OF_MEMORY) {
    log("GFX: error, GL_OUT_OF_MEMORY");
  } else if (errCode == GL_INVALID_FRAMEBUFFER_OPERATION) {
    log("GFX: error, GL_INVALID_FRAMEBUFFER_OPERATION");
  } else {
    log("GFX: unknown error, %d", errCode);
  }
}

static void gl_push(float **P, const float *p_end, bool first_vertex, float tex_left, float tex_top, float tex_right, float tex_bottom,
                    spoint tl, spoint tr, spoint bl, spoint br, uint8_t r1, uint8_t g1, uint8_t b1, uint8_t a1, uint8_t r2, uint8_t g2,
                    uint8_t b2, uint8_t a2, uint8_t r3, uint8_t g3, uint8_t b3, uint8_t a3, uint8_t r4, uint8_t g4, uint8_t b4, uint8_t a4)
{
  TRACE_DEBUG();

  float *p = *P;

  if ((p >= p_end)) [[unlikely]] {
    log("overflow on gl bug %s", __FUNCTION__);
    return;
  }

  [[likely]] if ((! first_vertex)) {
    //
    // If there is a break in the triangle strip then make a degenerate triangle.
    //
    if ((glapi_last_right != bl.x) || (glapi_last_bottom != bl.y)) {
      GL_PUSH_TEXCOORD(p, glapi_last_tex_right, glapi_last_tex_bottom);
      GL_PUSH_VERTEX(p, glapi_last_right, glapi_last_bottom);
      GL_PUSH_RGBA(p, r4, g4, b4, a4);

      GL_PUSH_TEXCOORD(p, tex_left, tex_top);
      GL_PUSH_VERTEX(p, tl.x, tl.y);
      GL_PUSH_RGBA(p, r1, g1, b1, a1);
    }
  }

  GL_PUSH_TEXCOORD(p, tex_left, tex_top);
  GL_PUSH_VERTEX(p, tl.x, tl.y);
  GL_PUSH_RGBA(p, r1, g1, b1, a1);

  GL_PUSH_TEXCOORD(p, tex_left, tex_bottom);
  GL_PUSH_VERTEX(p, bl.x, bl.y);
  GL_PUSH_RGBA(p, r2, g2, b2, a2);

  GL_PUSH_TEXCOORD(p, tex_right, tex_top);
  GL_PUSH_VERTEX(p, tr.x, tr.y);
  GL_PUSH_RGBA(p, r3, g3, b3, a3);

  GL_PUSH_TEXCOORD(p, tex_right, tex_bottom);
  GL_PUSH_VERTEX(p, br.x, br.y);
  GL_PUSH_RGBA(p, r4, g4, b4, a4);

  glapi_last_tex_right  = tex_right;
  glapi_last_tex_bottom = tex_bottom;
  glapi_last_right      = br.x;
  glapi_last_bottom     = br.y;
  *P                    = p;
}

static void gl_push(float **P, float *p_end, bool first_vertex, float tex_left, float tex_top, float tex_right, float tex_bottom,
                    GLshort left, GLshort top, GLshort right, GLshort bottom, uint8_t r1, uint8_t g1, uint8_t b1, uint8_t a1, uint8_t r2,
                    uint8_t g2, uint8_t b2, uint8_t a2, uint8_t r3, uint8_t g3, uint8_t b3, uint8_t a3, uint8_t r4, uint8_t g4, uint8_t b4,
                    uint8_t a4)
{
  TRACE_DEBUG();

  spoint const tl(left, top);
  spoint const tr(right, top);
  spoint const bl(left, bottom);
  spoint const br(right, bottom);

  gl_push(P, p_end, first_vertex, tex_left, tex_top, tex_right, tex_bottom, tl, tr, bl, br, r1, g1, b1, a1, r2, g2, b2, a2, r3, g3, b3, a3,
          r4, g4, b4, a4);
}

static bool first_vertex;

void blit(int tex, float texMinX, float texMinY, float texMaxX, float texMaxY, GLshort left, GLshort top, GLshort right, GLshort bottom,
          const color &c)
{
  TRACE_DEBUG();

  if (buf_tex == 0) [[unlikely]] {
    blit_init();
    first_vertex = true;
  } else if (buf_tex != tex) [[unlikely]] {
    blit_flush();
    first_vertex = true;
  } else {
    first_vertex = false;
  }

  buf_tex = tex;

  uint8_t const r = c.r;
  uint8_t const g = c.g;
  uint8_t const b = c.b;
  uint8_t const a = c.a;

  gl_push(&bufp, bufp_end, first_vertex, texMinX, texMinY, texMaxX, texMaxY, left, top, right, bottom, r, g, b, a, r, g, b, a, r, g, b, a,
          r, g, b, a);
}

void blit(int tex, float texMinX, float texMinY, float texMaxX, float texMaxY, GLshort pixMinX, GLshort pixMinY, GLshort pixMaxX,
          GLshort pixMaxY, const color &c, LightPixels *light_pixels, bool blit_flush_per_line)
{
  TRACE_DEBUG();

  if (buf_tex == 0) [[unlikely]] {
    blit_init();
    first_vertex = true;
  } else if (buf_tex != tex) [[unlikely]] {
    blit_flush();
    first_vertex = true;
  } else {
    first_vertex = false;
  }

  buf_tex = tex;

  const float texDiffX = (texMaxX - texMinX) / static_cast< float > LIGHT_PIXEL;
  const float texDiffY = (texMinY - texMaxY) / static_cast< float > LIGHT_PIXEL;
  const float pixDiffX = (static_cast< float >(pixMaxX) - static_cast< float >(pixMinX)) / static_cast< float > LIGHT_PIXEL;
  const float pixDiffY = (static_cast< float >(pixMinY) - static_cast< float >(pixMaxY)) / static_cast< float > LIGHT_PIXEL;
  const uint8_t                                                                                                 a = 255;

  //
  // Fewer triangle breaks doing y rows first
  //
  for (auto y = 0; y < LIGHT_PIXEL; y++) {

    const float texMaxY2 = texMaxY + (y * texDiffY);
    const float texMinY2 = texMaxY + ((y + 1) * texDiffY);
    const auto  pixMaxY2 = static_cast< GLshort >(static_cast< float >(pixMaxY) + (static_cast< float >(y) * pixDiffY));
    const auto  pixMinY2 = static_cast< GLshort >(static_cast< float >(pixMaxY) + (static_cast< float >(y + 1) * pixDiffY));

    for (auto x = 0; x < LIGHT_PIXEL; x++) {

      auto *const   pixel = &light_pixels->pixel[ x ][ y ];
      uint8_t const r     = std::max(pixel->r, pixel->player_r);
      uint8_t const g     = std::max(pixel->g, pixel->player_g);
      uint8_t const b     = std::max(pixel->b, pixel->player_b);

      float const texMinX2 = texMinX + (x * texDiffX);
      float const texMaxX2 = texMinX + ((x + 1) * texDiffX);
      auto        pixMinX2 = static_cast< GLshort >(static_cast< float >(pixMinX) + (static_cast< float >(x) * pixDiffX));
      auto        pixMaxX2 = static_cast< GLshort >(static_cast< float >(pixMinX) + (static_cast< float >(x + 1) * pixDiffX));

      gl_push(&bufp, bufp_end, first_vertex, texMinX2, texMinY2, texMaxX2, texMaxY2, pixMinX2, pixMinY2, pixMaxX2, pixMaxY2, r, g, b, a, r,
              g, b, a, r, g, b, a, r, g, b, a);
    }

    //
    // If we have alpha values in the texture, the end of one triangle line and the start of another creates
    // a visible strip
    //
    if (blit_flush_per_line) {
      buf_tex = tex;
      blit_flush();
    }
  }
}

void blit(int tex, float texMinX, float texMinY, float texMaxX, float texMaxY, GLshort left, GLshort top, GLshort right, GLshort bottom,
          const color &color_bl, const color &color_br, const color &color_tl, const color &color_tr)
{
  TRACE_DEBUG();

  if (buf_tex == 0) [[unlikely]] {
    blit_init();
    first_vertex = true;
  } else if (buf_tex != tex) [[unlikely]] {
    blit_flush();
    first_vertex = true;
  } else {
    first_vertex = false;
  }

  buf_tex = tex;

  gl_push(&bufp, bufp_end, first_vertex, texMinX, texMinY, texMaxX, texMaxY, left, top, right, bottom, color_tl.r, color_tl.g, color_tl.b,
          color_tl.a, color_bl.r, color_bl.g, color_bl.b, color_bl.a, color_tr.r, color_tr.g, color_tr.b, color_tr.a, color_br.r,
          color_br.g, color_br.b, color_br.a);
}

void blit(int tex, GLshort left, GLshort top, GLshort right, GLshort bottom, const color &c)
{
  TRACE_DEBUG();

  blit(tex, 0.0, 1.0, 1.0, 0.0, left, top, right, bottom, c);
}
