//
// Copyright goblinhack@gmail.com
//

#ifndef WEB_BUILD
#include "my_callstack.hpp"
#include "my_color.hpp"
#include "my_color_defs.hpp"
#include "my_enum.hpp"
#include "my_game.hpp"
#include "my_game_defs.hpp"
#include "my_gl.hpp"
#include "my_level.hpp"
#include "my_main.hpp"
#include "my_ptrcheck.hpp"
#include "my_size.hpp"
#include "my_spoint.hpp"
#include "my_tile.hpp"
#include "my_types.hpp"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstring>

static bool in_2d_mode;

// NOLINTBEGIN
void MessageCallback(GLenum /*source*/, GLenum type, GLuint id, GLenum severity, GLsizei /*length*/, const GLchar *message,
                     const void * /*userParam*/)
{
  TRACE();
  con("GL CALLBACK: type = 0x%x, severity = 0x%x, message = %s\n", type, severity, message);
}
// NOLINTEND

void gl_matrix_mode(int mode) { glMatrixMode(mode); }

void gl_matrix_push() { glPushMatrix(); }

void gl_matrix_pop() { glPopMatrix(); }

void gl_matrix_identity() { glLoadIdentity(); }

void gl_matrix_ortho(float left, float right, float bottom, float top, float near_val, float far_val)
{
  glOrtho(left, right, bottom, top, near_val, far_val);
}

void gl_matrix_translate(float x, float y, float z) { glTranslatef(x, y, z); }

void gl_matrix_rotate(float angle_degrees, float x, float y, float z) { glRotatef(angle_degrees, x, y, z); }

void gl_init_2d_mode(Gamep g)
{
  DBG("SDL: Init 2d mode");
  TRACE_INDENT();

  GL_ERROR_CHECK();
  gl_leave_2d_mode(g);

  //
  // Enable Textures
  //
  DBG("DesktopOpenGL: OpenGL enable textures");
  glEnable(GL_TEXTURE_2D);
  GL_ERROR_CHECK();

  //
  // Enable alpha blending for sprites
  //
  DBG("DesktopOpenGL: OpenGL enable blending");
  glEnable(GL_BLEND);
  GL_ERROR_CHECK();
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  GL_ERROR_CHECK();

  //
  // Setup our viewport
  //
  DBG("DesktopOpenGL: OpenGL enable viewport");
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

  log("DesktopOpenGL: OpenGL initialized");
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

void gl_init_fbo_(FboEnum fbo, GLuint *render_buf_id, GLuint *fbo_id, GLuint *fbo_tex_id, GLuint tex_width, GLuint tex_height)
{
  TRACE();
  DBG("DesktopOpenGL: create FBO, size %dx%d", tex_width, tex_height);

  GL_ERROR_CHECK();

  DBG2("DesktopOpenGL: - glGenTextures");
  if (static_cast< bool >(*fbo_tex_id)) {
    DBG2("DesktopOpenGL: - glDeleteTextures");
    glDeleteTextures(1, fbo_tex_id);
    GL_ERROR_CHECK();
    *fbo_tex_id = 0;
  }

  if (static_cast< bool >(*fbo_id)) {
    DBG2("DesktopOpenGL: - glDeleteRenderbuffers");
    glDeleteRenderbuffers_EXT(1, fbo_id);
    GL_ERROR_CHECK();
    *fbo_id = 0;
  }

  glGenTextures(1, fbo_tex_id);
  GL_ERROR_CHECK();

  DBG2("DesktopOpenGL: - glBindTexture");
  glBindTexture(GL_TEXTURE_2D, *fbo_tex_id);
  GL_ERROR_CHECK();

  DBG2("DesktopOpenGL: - glTexParameterf (clamping)");
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  GL_ERROR_CHECK();
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  GL_ERROR_CHECK();

  switch (fbo) {
    case FBO_MAP_BG_PREVIOUSLY_SEEN_TILES :
    case FBO_MAP_BG_FLOOR_WATER_LAVA :
    case FBO_MAP_FG :
    case FBO_MAP_FG_OVERLAY :
    case FBO_MAP_LIGHT :
    case FBO_FULL_SCREEN_PREVIOUSLY_SEEN_TILES :
    case FBO_FULL_SCREEN_VISIBLE_TILES :
    case FBO_FULL_SCREEN_LEVEL_CURR :
    case FBO_FULL_SCREEN_LEVEL_BELOW :
    case FBO_FULL_SCREEN_LEVEL_MERGED :
    case FBO_FULL_SCREEN_WID :
    case FBO_FULL_SCREEN_FINAL :
    case FBO_FLAMES :
    case FBO_SKULLS :
    case FBO_MINIMAP_LEVEL :
    case FBO_SPRITE1 :
    default :
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      GL_ERROR_CHECK();
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      GL_ERROR_CHECK();
      break;
    case FBO_MAP_LAVA_OVERLAY :
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      GL_ERROR_CHECK();
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      GL_ERROR_CHECK();
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      GL_ERROR_CHECK();
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      GL_ERROR_CHECK();
      break;
  }

  DBG2("DesktopOpenGL: - glTexImage2D");

#ifdef WEB_BUILD
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, tex_width, tex_height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, nullptr);
#else
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, tex_width, tex_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
#endif
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

  DBG2("DesktopOpenGL: - glGenRenderbuffers_EXT");
  glGenRenderbuffers_EXT(1, render_buf_id);
  GL_ERROR_CHECK();

  DBG2("DesktopOpenGL: - glBindRenderbuffer_EXT");
  glBindRenderbuffer_EXT(GL_RENDERBUFFER, *render_buf_id);
  GL_ERROR_CHECK();

  DBG2("DesktopOpenGL: - glRenderbufferStorage_EXT");
  glRenderbufferStorage_EXT(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, tex_width, tex_height);
  GL_ERROR_CHECK();

  DBG2("DesktopOpenGL: - glBindRenderbuffer_EXT");
  glBindRenderbuffer_EXT(GL_RENDERBUFFER, 0);
  GL_ERROR_CHECK();

  //
  // Create a frame buffer object.
  //
  DBG2("DesktopOpenGL: - glGenFramebuffers_EXT");
  glGenFramebuffers_EXT(1, fbo_id);
  GL_ERROR_CHECK();

  DBG2("DesktopOpenGL: - glBindFramebuffer_EXT");
  glBindFramebuffer_EXT(GL_FRAMEBUFFER, *fbo_id);
  GL_ERROR_CHECK();

  //
  // Attach the texture to FBO color attachment point
  //
  DBG2("DesktopOpenGL: - glFramebufferTexture2D_EXT");
  glFramebufferTexture2D_EXT(GL_FRAMEBUFFER,       // 1. fbo target: GL_FRAMEBUFFER
                             GL_COLOR_ATTACHMENT0, // 2. attachment point
                             GL_TEXTURE_2D,        // 3. tex target: GL_TEXTURE_2D
                             *fbo_tex_id,          // 4. tex ID
                             0);                   // 5. mipmap level: 0(base)
  GL_ERROR_CHECK();

  //
  // Attach the renderbuffer to depth attachment point
  //
  DBG2("DesktopOpenGL: - glFramebufferRenderbuffer_EXT");
  glFramebufferRenderbuffer_EXT(GL_FRAMEBUFFER,      // 1. fbo target: GL_FRAMEBUFFER
                                GL_DEPTH_ATTACHMENT, // 2. attachment point
                                GL_RENDERBUFFER,     // 3. rbo target: GL_RENDERBUFFER
                                *render_buf_id);     // 4. rbo ID
  GL_ERROR_CHECK();

  //
  // Check FBO status
  //
  DBG2("DesktopOpenGL: - glCheckFramebufferStatus_EXT");
  auto status = glCheckFramebufferStatus_EXT(GL_FRAMEBUFFER);
  if ((static_cast< bool >(status)) && (status != GL_FRAMEBUFFER_COMPLETE)) {
    log("DesktopOpenGL: failed to create framebuffer, error: %d/0x%x", status, status);

#ifdef GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT
    if (status == GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT) {
      log("DesktopOpenGL: - DesktopOpenGL: GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT Not all framebuffer attachment points "
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
      log("DesktopOpenGL: - DesktopOpenGL: GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS Not all attached images have the same "
          "width and height.");
    }
#endif
#ifdef GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT
    if (status == GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT) {
      log("DesktopOpenGL: - DesktopOpenGL: GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT No images are attached to the framebuffer.");
    }
#endif
#ifdef GL_FRAMEBUFFER_UNSUPPORTED
    if (status == GL_FRAMEBUFFER_UNSUPPORTED) {
      log("DesktopOpenGL: - DesktopOpenGL: GL_FRAMEBUFFER_UNSUPPORTED The combination of internal formats of the attached images "
          "violates an implementation-dependent set of restrictions.");
    }
#endif
#ifdef GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER
    if (status == GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER) {
      log("DesktopOpenGL: - DesktopOpenGL: GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER");
    }
#endif
#ifdef GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER
    if (status == GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER) {
      log("DesktopOpenGL: - DesktopOpenGL: GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER");
    }
#endif
  }
  if (! static_cast< bool >(status)) {
    glGetError();
  }
  GL_ERROR_CHECK();

  // switch back to window-system-provided framebuffer
  DBG2("DesktopOpenGL: - glBindFramebuffer_EXT");
  glBindFramebuffer_EXT(GL_FRAMEBUFFER, 0);
  GL_ERROR_CHECK();
}

void gl_fini_fbo_(GLuint *render_buf_id, GLuint *fbo_id, GLuint *fbo_tex_id, GLuint tex_width, GLuint tex_height)
{
  TRACE();
  DBG2("DesktopOpenGL: destroy FBO, size %dx%d", tex_width, tex_height);

  GL_ERROR_CHECK();

  DBG2("DesktopOpenGL: - glGenTextures");
  if (static_cast< bool >(*fbo_tex_id)) {
    DBG2("DesktopOpenGL: - glDeleteTextures");
    glDeleteTextures(1, fbo_tex_id);
    GL_ERROR_CHECK();
    *fbo_tex_id = 0;
  }

  if (static_cast< bool >(*fbo_id)) {
    DBG2("GLES: - glDeleteFramebuffers");
    glDeleteFramebuffers_EXT(1, fbo_id);
    GL_ERROR_CHECK();
    *fbo_id = 0;
  }

  if (static_cast< bool >(*render_buf_id)) {
    DBG2("GLES: - glDeleteRenderbuffers");
    glDeleteRenderbuffers_EXT(1, render_buf_id);
    GL_ERROR_CHECK();
    *render_buf_id = 0;
  }

  memset(g_fbo_id, 0, SIZEOF(g_fbo_id));
  memset(g_fbo_tex_id, 0, SIZEOF(g_fbo_tex_id));
  memset(g_render_buf_id, 0, SIZEOF(g_render_buf_id));

  for (auto &i : g_fbo_size) {
    i = isize(0, 0);
  }
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

  nvertices = (reinterpret_cast< char * >(bufp) - reinterpret_cast< char * >(gl_array_buf)) / NUMBER_BYTES_PER_VERTEX_2D;

  glBindTexture(GL_TEXTURE_2D, buf_tex);

  glTexCoordPointer(NUMBER_DIMENSIONS_PER_COORD_2D, // (u,v)
                    GL_FLOAT, NUMBER_BYTES_PER_VERTEX_2D, gl_array_buf);

  glVertexPointer(NUMBER_DIMENSIONS_PER_COORD_2D, // (x,y)
                  GL_SHORT, NUMBER_BYTES_PER_VERTEX_2D,
                  (reinterpret_cast< char * >(gl_array_buf))
                      + (static_cast< ptrdiff_t >(SIZEOF(GLfloat) * // skip (u,v)
                                                  NUMBER_DIMENSIONS_PER_COORD_2D)));

  glColorPointer(NUMBER_COMPONENTS_PER_COLOR, // (r,g,b,a)
                 GL_UNSIGNED_BYTE, NUMBER_BYTES_PER_VERTEX_2D,
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

  GL_PUSH_VERTEX_SIMPLE(left, top);
  GL_PUSH_VERTEX_SIMPLE(right, top);
  GL_PUSH_VERTEX_SIMPLE(left, bottom);
  GL_PUSH_VERTEX_SIMPLE(right, bottom);

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

  GL_PUSH_VERTEX_SIMPLE(tl.x, tl.y);
  GL_PUSH_VERTEX_SIMPLE(tr.x, tr.y);
  GL_PUSH_VERTEX_SIMPLE(bl.x, bl.y);
  GL_PUSH_VERTEX_SIMPLE(br.x, br.y);

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

  GL_PUSH_VERTEX_SIMPLE(left, top);
  GL_PUSH_VERTEX_SIMPLE(right, top);
  GL_PUSH_VERTEX_SIMPLE(right, bottom);
  GL_PUSH_VERTEX_SIMPLE(left, bottom);

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

  GL_PUSH_VERTEX_SIMPLE(left, top);
  GL_PUSH_VERTEX_SIMPLE(right, bottom);

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
    log("DesktopOpenGL: - glDebugMessageCallback_EXT - NOT present");
  } else {
    log("DesktopOpenGL: - glDebugMessageCallback_EXT - present");
  }

  if (glDebugMessageCallback_EXT) {
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback_EXT(MessageCallback, 0);
  }

  TRACE();
  glCreateProgram_EXT = (__typeof__(glCreateProgram_EXT)) (void *) wglGetProcAddress("glCreateProgram");
  if (! glCreateProgram_EXT) {
    log("DesktopOpenGL: - glCreateProgram_EXT - NOT present");
  } else {
    log("DesktopOpenGL: - glCreateProgram_EXT - present");
  }

  glDeleteProgram_EXT = (__typeof__(glDeleteProgram_EXT)) (void *) wglGetProcAddress("glDeleteProgram");
  if (! glDeleteProgram_EXT) {
    log("DesktopOpenGL: - glDeleteProgram_EXT - NOT present");
  } else {
    log("DesktopOpenGL: - glDeleteProgram_EXT - present");
  }

  glIsProgram_EXT = (__typeof__(glIsProgram_EXT)) (void *) wglGetProcAddress("glIsProgram");
  if (! glIsProgram_EXT) {
    log("DesktopOpenGL: - glIsProgram_EXT - NOT present");
  } else {
    log("DesktopOpenGL: - glIsProgram_EXT - present");
  }

  glCreateShader_EXT = (__typeof__(glCreateShader_EXT)) (void *) wglGetProcAddress("glCreateShader");
  if (! glCreateShader_EXT) {
    log("DesktopOpenGL: - glCreateShader_EXT - NOT present");
  } else {
    log("DesktopOpenGL: - glCreateShader_EXT - present");
  }

  glDeleteShader_EXT = (__typeof__(glDeleteShader_EXT)) (void *) wglGetProcAddress("glDeleteShader");
  if (! glDeleteShader_EXT) {
    log("DesktopOpenGL: - glDeleteShader_EXT - NOT present");
  } else {
    log("DesktopOpenGL: - glDeleteShader_EXT - present");
  }

  glShaderSource_EXT = (__typeof__(glShaderSource_EXT)) (void *) wglGetProcAddress("glShaderSource");
  if (! glShaderSource_EXT) {
    log("DesktopOpenGL: - glShaderSource_EXT - NOT present");
  } else {
    log("DesktopOpenGL: - glShaderSource_EXT - present");
  }

  glCompileShader_EXT = (__typeof__(glCompileShader_EXT)) (void *) wglGetProcAddress("glCompileShader");
  if (! glCompileShader_EXT) {
    log("DesktopOpenGL: - glCompileShader_EXT - NOT present");
  } else {
    log("DesktopOpenGL: - glCompileShader_EXT - present");
  }

  glAttachShader_EXT = (__typeof__(glAttachShader_EXT)) (void *) wglGetProcAddress("glAttachShader");
  if (! glAttachShader_EXT) {
    log("DesktopOpenGL: - glAttachShader_EXT - NOT present");
  } else {
    log("DesktopOpenGL: - glAttachShader_EXT - present");
  }

  glDetachShader_EXT = (__typeof__(glDetachShader_EXT)) (void *) wglGetProcAddress("glDetachShader");
  if (! glDetachShader_EXT) {
    log("DesktopOpenGL: - glDetachShader_EXT - NOT present");
  } else {
    log("DesktopOpenGL: - glDetachShader_EXT - present");
  }

  glGetAttachedShaders_EXT = (__typeof__(glGetAttachedShaders_EXT)) (void *) wglGetProcAddress("glGetAttachedShaders");
  if (! glGetAttachedShaders_EXT) {
    log("DesktopOpenGL: - glGetAttachedShaders_EXT - NOT present");
  } else {
    log("DesktopOpenGL: - glGetAttachedShaders_EXT - present");
  }

  glLinkProgram_EXT = (__typeof__(glLinkProgram_EXT)) (void *) wglGetProcAddress("glLinkProgram");
  if (! glLinkProgram_EXT) {
    log("DesktopOpenGL: - glLinkProgram_EXT - NOT present");
  } else {
    log("DesktopOpenGL: - glLinkProgram_EXT - present");
  }

  glUseProgram_EXT = (__typeof__(glUseProgram_EXT)) (void *) wglGetProcAddress("glUseProgram");
  if (! glUseProgram_EXT) {
    log("DesktopOpenGL: - glUseProgram_EXT - NOT present");
  } else {
    log("DesktopOpenGL: - glUseProgram_EXT - present");
  }

  glGetShaderInfoLog_EXT = (__typeof__(glGetShaderInfoLog_EXT)) (void *) wglGetProcAddress("glGetShaderInfoLog");
  if (! glGetShaderInfoLog_EXT) {
    log("DesktopOpenGL: - glGetShaderInfoLog_EXT - NOT present");
  } else {
    log("DesktopOpenGL: - glGetShaderInfoLog_EXT - present");
  }

  glGetProgramInfoLog_EXT = (__typeof__(glGetProgramInfoLog_EXT)) (void *) wglGetProcAddress("glGetProgramInfoLog");
  if (! glGetProgramInfoLog_EXT) {
    log("DesktopOpenGL: - glGetProgramInfoLog_EXT - NOT present");
  } else {
    log("DesktopOpenGL: - glGetProgramInfoLog_EXT - present");
  }

  glGetUniformLocation_EXT = (__typeof__(glGetUniformLocation_EXT)) (void *) wglGetProcAddress("glGetUniformLocation");
  if (! glGetUniformLocation_EXT) {
    log("DesktopOpenGL: - glGetUniformLocation_EXT - NOT present");
  } else {
    log("DesktopOpenGL: - glGetUniformLocation_EXT - present");
  }

  glUniform1f_EXT = (__typeof__(glUniform1f_EXT)) (void *) wglGetProcAddress("glUniform1f");
  if (! glUniform1f_EXT) {
    log("DesktopOpenGL: - glUniform1f_EXT - NOT present");
  } else {
    log("DesktopOpenGL: - glUniform1f_EXT - present");
  }

  glUniform1i_EXT = (__typeof__(glUniform1i_EXT)) (void *) wglGetProcAddress("glUniform1i");
  if (! glUniform1i_EXT) {
    log("DesktopOpenGL: - glUniform1i_EXT - NOT present");
  } else {
    log("DesktopOpenGL: - glUniform1i_EXT - present");
  }

  glUniform2fv_EXT = (__typeof__(glUniform2fv_EXT)) (void *) wglGetProcAddress("glUniform2fv");
  if (! glUniform2fv_EXT) {
    log("DesktopOpenGL: - glUniform2fv_EXT - NOT present");
  } else {
    log("DesktopOpenGL: - glUniform2fv_EXT - present");
  }

  glUniform3fv_EXT = (__typeof__(glUniform3fv_EXT)) (void *) wglGetProcAddress("glUniform3fv");
  if (! glUniform3fv_EXT) {
    log("DesktopOpenGL: - glUniform3fv_EXT - NOT present");
  } else {
    log("DesktopOpenGL: - glUniform3fv_EXT - present");
  }

  glGenerateMipmap_EXT = (__typeof__(glGenerateMipmap_EXT)) (void *) wglGetProcAddress("glGenerateMipmap");
  if (! glGenerateMipmap_EXT) {
    log("DesktopOpenGL: - glGenerateMipmap_EXT - NOT present");
  } else {
    log("DesktopOpenGL: - glGenerateMipmap_EXT - present");
  }

  glGenFramebuffers_EXT = (__typeof__(glGenFramebuffers_EXT)) (void *) wglGetProcAddress("glGenFramebuffers");
  if (! glGenFramebuffers_EXT) {
    log("DesktopOpenGL: - glGenFramebuffers_EXT - NOT present");
  } else {
    log("DesktopOpenGL: - glGenFramebuffers_EXT - present");
  }

  glDeleteFramebuffers_EXT = (__typeof__(glDeleteFramebuffers_EXT)) (void *) wglGetProcAddress("glDeleteFramebuffers");
  if (! glDeleteFramebuffers_EXT) {
    log("DesktopOpenGL: - glDeleteFramebuffers_EXT - NOT present");
  } else {
    log("DesktopOpenGL: - glDeleteFramebuffers_EXT - present");
  }

  glBindFramebuffer_EXT = (__typeof__(glBindFramebuffer_EXT)) (void *) wglGetProcAddress("glBindFramebuffer");
  if (! glBindFramebuffer_EXT) {
    log("DesktopOpenGL: - glBindFramebuffer_EXT - NOT present");
  } else {
    log("DesktopOpenGL: - glBindFramebuffer_EXT - present");
  }

  glGenRenderbuffers_EXT = (__typeof__(glGenRenderbuffers_EXT)) (void *) wglGetProcAddress("glGenRenderbuffers");
  if (! glGenRenderbuffers_EXT) {
    log("DesktopOpenGL: - glGenRenderbuffers_EXT - NOT present");
  } else {
    log("DesktopOpenGL: - glGenRenderbuffers_EXT - present");
  }

  glDeleteRenderbuffers_EXT = (__typeof__(glDeleteRenderbuffers_EXT)) (void *) wglGetProcAddress("glDeleteRenderbuffers");
  if (! glDeleteRenderbuffers_EXT) {
    log("DesktopOpenGL: - glDeleteRenderbuffers_EXT - NOT present");
  } else {
    log("DesktopOpenGL: - glDeleteRenderbuffers_EXT - present");
  }

  glBindRenderbuffer_EXT = (__typeof__(glBindRenderbuffer_EXT)) (void *) wglGetProcAddress("glBindRenderbuffer");
  if (! glBindRenderbuffer_EXT) {
    log("DesktopOpenGL: - glBindRenderbuffer_EXT - NOT present");
  } else {
    log("DesktopOpenGL: - glBindRenderbuffer_EXT - present");
  }

  glRenderbufferStorage_EXT = (__typeof__(glRenderbufferStorage_EXT)) (void *) wglGetProcAddress("glRenderbufferStorage");
  if (! glRenderbufferStorage_EXT) {
    log("DesktopOpenGL: - glRenderbufferStorage_EXT - NOT present");
  } else {
    log("DesktopOpenGL: - glRenderbufferStorage_EXT - present");
  }

  glFramebufferRenderbuffer_EXT = (__typeof__(glFramebufferRenderbuffer_EXT)) (void *) wglGetProcAddress("glFramebufferRenderbuffer");
  if (! glFramebufferRenderbuffer_EXT) {
    log("DesktopOpenGL: - glFramebufferRenderbuffer_EXT - NOT present");
  } else {
    log("DesktopOpenGL: - glFramebufferRenderbuffer_EXT - present");
  }

  glCheckFramebufferStatus_EXT = (__typeof__(glCheckFramebufferStatus_EXT)) (void *) wglGetProcAddress("glCheckFramebufferStatus");
  if (! glCheckFramebufferStatus_EXT) {
    log("DesktopOpenGL: - glCheckFramebufferStatus_EXT - NOT present");
  } else {
    log("DesktopOpenGL: - glCheckFramebufferStatus_EXT - present");
  }

  glFramebufferTexture2D_EXT = (__typeof__(glFramebufferTexture2D_EXT)) (void *) wglGetProcAddress("glFramebufferTexture2D");
  if (! glFramebufferTexture2D_EXT) {
    log("DesktopOpenGL: - glFramebufferTexture2D_EXT - NOT present");
  } else {
    log("DesktopOpenGL: - glFramebufferTexture2D_EXT - present");
  }

  glGenBuffersARB_EXT = (__typeof__(glGenBuffersARB_EXT)) (void *) wglGetProcAddress("glGenBuffersARB");
  if (! glGenBuffersARB_EXT) {
    log("DesktopOpenGL: - glGenBuffersARB_EXT - NOT present");
  } else {
    log("DesktopOpenGL: - glGenBuffersARB_EXT - present");
  }

  glBindBufferARB_EXT = (__typeof__(glBindBufferARB_EXT)) (void *) wglGetProcAddress("glBindBufferARB");
  if (! glBindBufferARB_EXT) {
    log("DesktopOpenGL: - glBindBufferARB_EXT - NOT present");
  } else {
    log("DesktopOpenGL: - glBindBufferARB_EXT - present");
  }

  glDeleteBuffersARB_EXT = (__typeof__(glDeleteBuffersARB_EXT)) (void *) wglGetProcAddress("glDeleteBuffersARB");
  if (! glDeleteBuffersARB_EXT) {
    log("DesktopOpenGL: - glDeleteBuffersARB_EXT - NOT present");
  } else {
    log("DesktopOpenGL: - glDeleteBuffersARB_EXT - present");
  }

  glDeleteBuffersARB_EXT = (__typeof__(glDeleteBuffersARB_EXT)) (void *) wglGetProcAddress("glDeleteBuffersARB");
  if (! glDeleteBuffersARB_EXT) {
    log("DesktopOpenGL: - glDeleteBuffersARB_EXT - NOT present");
  } else {
    log("DesktopOpenGL: - glDeleteBuffersARB_EXT - present");
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

  log("DesktopOpenGL: extensions");

  DBG("DesktopOpenGL: - GetModuleHandle");
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

  DBG("DesktopOpenGL: - RegisterClassEx");
  if (! RegisterClassEx(&wc)) {
    MessageBox(nullptr, "Window Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
    return;
  }

  DBG("DesktopOpenGL: - CreateWindowEx");
  hwnd = CreateWindowEx(WS_EX_CLIENTEDGE, g_szClassName, "sacrecant startup", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 240, 120,
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

#endif
