//
// Copyright goblinhack@gmail.com
//

#ifdef GLES_BUILD
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
#include <array>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <numbers>

static bool in_2d_mode;

void gl_init_2d_mode(Gamep g)
{
  DBG("SDL: Init 2d mode");
  TRACE_INDENT();

  GL_ERROR_CHECK();
  gl_leave_2d_mode(g);

  //
  // The shader that replaces the fixed function pipeline. No need to enable textures; that
  // is done in the shader.
  //
  gl_shader_init();

  //
  // Enable alpha blending for sprites
  //
  DBG("GLES: OpenGL enable blending");
  glEnable(GL_BLEND);
  GL_ERROR_CHECK();
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  GL_ERROR_CHECK();

  //
  // Setup our viewport
  //
  DBG("GLES: OpenGL enable viewport");
  glViewport(0, 0, game_window_pix_width_get(g), game_window_pix_height_get(g));
  GL_ERROR_CHECK();

  //
  // Reset the view
  //
  gl_matrix_identity();
  GL_ERROR_CHECK();

  gl_init_fbo(g);

  glLineWidth(1.0F);
  GL_ERROR_CHECK();

  log("GLES: OpenGL initialized");
}

void gl_fini_2d_mode(Gamep g)
{
  DBG("SDL: fini 2d mode");
  TRACE_INDENT();

  GL_ERROR_CHECK();

  gl_leave_2d_mode(g);
  gl_fini_fbo(g);
  gl_shader_fini();
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
  gl_matrix_mode(GL_MATRIX_PROJECTION);
  GL_ERROR_CHECK();
  gl_matrix_push();
  GL_ERROR_CHECK();

  //
  // Reset the view
  //
  gl_matrix_identity();
  GL_ERROR_CHECK();

  //
  // 2D projection
  //
  if ((game_map_fbo_width_get(g) == 0) || (game_map_fbo_height_get(g) == 0)) {
    log("cannot set ortho (%d,%d)", game_map_fbo_width_get(g), game_map_fbo_height_get(g));
    return;
  }

  gl_matrix_ortho(0,                                                // left
                  static_cast< float >(game_map_fbo_width_get(g)),  // right
                  static_cast< float >(game_map_fbo_height_get(g)), // bottom
                  0,                                                // top
                  -1200.0F, 1200.0F);
  GL_ERROR_CHECK();

  //
  // Make sure we're changing the model view and not the projection
  //
  gl_matrix_mode(GL_MATRIX_MODELVIEW);
  GL_ERROR_CHECK();
  gl_matrix_push();
  GL_ERROR_CHECK();

  //
  // Reset the view
  //
  gl_matrix_identity();
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
  gl_matrix_mode(GL_MATRIX_PROJECTION);
  GL_ERROR_CHECK();
  gl_matrix_push();
  GL_ERROR_CHECK();

  //
  // Reset the view
  //
  gl_matrix_identity();
  GL_ERROR_CHECK();

  //
  // 2D projection
  //
  gl_matrix_ortho(0,                       // left
                  static_cast< float >(w), // right
                  static_cast< float >(h), // bottom
                  0,                       // top
                  -1200.0F, 1200.0F);
  GL_ERROR_CHECK();

  glViewport(0, 0, w, h);
  GL_ERROR_CHECK();

  //
  // Make sure we're changing the model view and not the projection
  //
  gl_matrix_mode(GL_MATRIX_MODELVIEW);
  GL_ERROR_CHECK();
  gl_matrix_push();
  GL_ERROR_CHECK();

  //
  // Reset the view
  //
  gl_matrix_identity();
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

  gl_matrix_mode(GL_MATRIX_MODELVIEW);
  GL_ERROR_CHECK();
  gl_matrix_pop();
  GL_ERROR_CHECK();

  gl_matrix_mode(GL_MATRIX_PROJECTION);
  GL_ERROR_CHECK();
  gl_matrix_pop();
  GL_ERROR_CHECK();
}

void gl_init_fbo_(FboEnum fbo, GLuint *render_buf_id, GLuint *fbo_id, GLuint *fbo_tex_id, GLuint tex_width, GLuint tex_height)
{
  TRACE();
  DBG("GLES: create FBO, size %dx%d", tex_width, tex_height);

  GL_ERROR_CHECK();

  DBG2("GLES: - glGenTextures");
  if (static_cast< bool >(*fbo_tex_id)) {
    DBG2("GLES: - glDeleteTextures");
    glDeleteTextures(1, fbo_tex_id);
    GL_ERROR_CHECK();
    *fbo_tex_id = 0;
  }

  if (static_cast< bool >(*fbo_id)) {
    DBG2("GLES: - glDeleteFramebuffers");
    glDeleteFramebuffers(1, fbo_id);
    GL_ERROR_CHECK();
    *fbo_id = 0;
  }

  if (static_cast< bool >(*render_buf_id)) {
    DBG2("GLES: - glDeleteRenderbuffers");
    glDeleteRenderbuffers(1, render_buf_id);
    GL_ERROR_CHECK();
    *render_buf_id = 0;
  }

  glGenTextures(1, fbo_tex_id);
  GL_ERROR_CHECK();

  DBG2("GLES: - glBindTexture");
  glBindTexture(GL_TEXTURE_2D, *fbo_tex_id);
  GL_ERROR_CHECK();

  DBG2("GLES: - glTexParameterf (clamping)");
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

  DBG2("GLES: - glTexImage2D");

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, static_cast< GLsizei >(tex_width), static_cast< GLsizei >(tex_height), 0, GL_RGBA, GL_UNSIGNED_BYTE,
               nullptr);
  GL_ERROR_CHECK();
  glBindTexture(GL_TEXTURE_2D, 0);
  GL_ERROR_CHECK();

  //
  // Create a render buffer object.
  //
  DBG2("GLES: - glGenRenderbuffers");
  glGenRenderbuffers(1, render_buf_id);
  GL_ERROR_CHECK();

  DBG2("GLES: - glBindRenderbuffer");
  glBindRenderbuffer(GL_RENDERBUFFER, *render_buf_id);
  GL_ERROR_CHECK();

  DBG2("GLES: - glRenderbufferStorage");
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, static_cast< GLsizei >(tex_width), static_cast< GLsizei >(tex_height));
  GL_ERROR_CHECK();

  DBG2("GLES: - glBindRenderbuffer");
  glBindRenderbuffer(GL_RENDERBUFFER, 0);
  GL_ERROR_CHECK();

  //
  // Create a frame buffer object.
  //
  DBG2("GLES: - glGenFramebuffers");
  glGenFramebuffers(1, fbo_id);
  GL_ERROR_CHECK();

  DBG2("GLES: - glBindFramebuffer");
  glBindFramebuffer(GL_FRAMEBUFFER, *fbo_id);
  GL_ERROR_CHECK();

  //
  // Attach the texture to FBO color attachment point
  //
  DBG2("GLES: - glFramebufferTexture2D");
  glFramebufferTexture2D(GL_FRAMEBUFFER,       // 1. fbo target: GL_FRAMEBUFFER
                         GL_COLOR_ATTACHMENT0, // 2. attachment point
                         GL_TEXTURE_2D,        // 3. tex target: GL_TEXTURE_2D
                         *fbo_tex_id,          // 4. tex ID
                         0);                   // 5. mipmap level: 0(base)
  GL_ERROR_CHECK();

  //
  // Attach the renderbuffer to depth attachment point
  //
  DBG2("GLES: - glFramebufferRenderbuffer");
  glFramebufferRenderbuffer(GL_FRAMEBUFFER,      // 1. fbo target: GL_FRAMEBUFFER
                            GL_DEPTH_ATTACHMENT, // 2. attachment point
                            GL_RENDERBUFFER,     // 3. rbo target: GL_RENDERBUFFER
                            *render_buf_id);     // 4. rbo ID
  GL_ERROR_CHECK();

  //
  // Check FBO status
  //
  DBG2("GLES: - glCheckFramebufferStatus");
  auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  if ((static_cast< bool >(status)) && (status != GL_FRAMEBUFFER_COMPLETE)) {
    log("GLES: failed to create framebuffer, error: %d/0x%x", status, status);

#ifdef GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT
    if (status == GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT) {
      log("GLES: - GLES: GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT Not all framebuffer attachment points "
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
      log("GLES: - GLES: GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS Not all attached images have the same "
          "width and height.");
    }
#endif
#ifdef GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT
    if (status == GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT) {
      log("GLES: - GLES: GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT No images are attached to the framebuffer.");
    }
#endif
#ifdef GL_FRAMEBUFFER_UNSUPPORTED
    if (status == GL_FRAMEBUFFER_UNSUPPORTED) {
      log("GLES: - GLES: GL_FRAMEBUFFER_UNSUPPORTED The combination of internal formats of the attached images "
          "violates an implementation-dependent set of restrictions.");
    }
#endif
#ifdef GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER
    if (status == GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER) {
      log("GLES: - GLES: GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER");
    }
#endif
#ifdef GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER
    if (status == GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER) {
      log("GLES: - GLES: GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER");
    }
#endif
  }
  if (! static_cast< bool >(status)) {
    glGetError();
  }
  GL_ERROR_CHECK();

  // switch back to window-system-provided framebuffer
  DBG2("GLES: - glBindFramebuffer");
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  GL_ERROR_CHECK();
}

void gl_fini_fbo_(GLuint *render_buf_id, GLuint *fbo_id, GLuint *fbo_tex_id, GLuint tex_width, GLuint tex_height)
{
  TRACE();
  DBG2("GLES: destroy FBO, size %dx%d", tex_width, tex_height);

  GL_ERROR_CHECK();

  DBG2("GLES: - glGenTextures");
  if (static_cast< bool >(*fbo_tex_id)) {
    DBG2("GLES: - glDeleteTextures");
    glDeleteTextures(1, fbo_tex_id);
    GL_ERROR_CHECK();
    *fbo_tex_id = 0;
  }

  if (static_cast< bool >(*fbo_id)) {
    DBG2("GLES: - glDeleteFramebuffers");
    glDeleteFramebuffers(1, fbo_id);
    GL_ERROR_CHECK();
    *fbo_id = 0;
  }

  if (static_cast< bool >(*render_buf_id)) {
    DBG2("GLES: - glDeleteRenderbuffers");
    glDeleteRenderbuffers(1, render_buf_id);
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
  glBindFramebuffer(GL_FRAMEBUFFER, g_fbo_id[ fbo ]);
}

void blit_fbo_pop()
{
  TRACE_DEBUG();
  glBindFramebuffer(GL_FRAMEBUFFER, g_fbo_id[ fbo_last ]);
}

void blit_fbo_bind(FboEnum fbo)
{
  TRACE_DEBUG();

  fbo_last = fbo;
  if (fbo_locked != FBO_NONE) {
    CROAK("Attempt to bind to another FBO %d when locked", fbo);
  }
  glBindFramebuffer(GL_FRAMEBUFFER, g_fbo_id[ fbo ]);
}

void blit_fbo_unbind()
{
  TRACE_DEBUG();

  if (fbo_locked != FBO_NONE) {
    CROAK("Attempt to unbind when locked");
  }
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void blit_fbo_bind_locked(FboEnum fbo)
{
  TRACE_DEBUG();

  fbo_last   = fbo;
  fbo_locked = fbo;
  glBindFramebuffer(GL_FRAMEBUFFER, g_fbo_id[ fbo ]);
}

void blit_fbo_unbind_locked()
{
  TRACE_DEBUG();

  fbo_locked = FBO_NONE;
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

//
// OpenGL ES 3.0 has no fixed function pipeline. There is no glMatrixMode, glColor4ub, glVertexPointer etc...
//
// What follows is a small replacement; a matrix stack, a "current colour" and a single shader that draws
// the three vertex layouts used by this game, via a streaming vertex buffer.
//
using Mat4 = std::array< float, 16 >; // column major, as OpenGL expects

static constexpr Mat4 MAT4_IDENTITY = {1.0F, 0.0F, 0.0F, 0.0F, 0.0F, 1.0F, 0.0F, 0.0F, 0.0F, 0.0F, 1.0F, 0.0F, 0.0F, 0.0F, 0.0F, 1.0F};

static constexpr int MATRIX_STACK_MAX = 32;

static Mat4 matrix_curr[ 2 ]                      = {MAT4_IDENTITY, MAT4_IDENTITY};
static Mat4 matrix_stack[ 2 ][ MATRIX_STACK_MAX ] = {};
static int  matrix_sp[ 2 ]                        = {0, 0};
static int  matrix_mode                           = GL_MATRIX_MODELVIEW;

//
// a * b
//
static auto mat4_mul(const Mat4 &a, const Mat4 &b) -> Mat4
{
  Mat4 r {};
  for (int col = 0; col < 4; col++) {
    for (int row = 0; row < 4; row++) {
      float sum = 0.0F;
      for (int k = 0; k < 4; k++) {
        sum += a[ (k * 4) + row ] * b[ (col * 4) + k ];
      }
      r[ (col * 4) + row ] = sum;
    }
  }
  return r;
}

void gl_matrix_mode(int mode) { matrix_mode = mode; }

void gl_matrix_push()
{
  if (matrix_sp[ matrix_mode ] >= MATRIX_STACK_MAX) {
    ERR("GLES: matrix stack overflow");
    return;
  }
  matrix_stack[ matrix_mode ][ matrix_sp[ matrix_mode ]++ ] = matrix_curr[ matrix_mode ];
}

void gl_matrix_pop()
{
  if (matrix_sp[ matrix_mode ] <= 0) {
    ERR("GLES: matrix stack underflow");
    return;
  }
  matrix_curr[ matrix_mode ] = matrix_stack[ matrix_mode ][ --matrix_sp[ matrix_mode ] ];
}

void gl_matrix_identity() { matrix_curr[ matrix_mode ] = MAT4_IDENTITY; }

void gl_matrix_ortho(float left, float right, float bottom, float top, float near_val, float far_val)
{
  Mat4 m = MAT4_IDENTITY;

  m[ 0 ]  = 2.0F / (right - left);
  m[ 5 ]  = 2.0F / (top - bottom);
  m[ 10 ] = -2.0F / (far_val - near_val);
  m[ 12 ] = -(right + left) / (right - left);
  m[ 13 ] = -(top + bottom) / (top - bottom);
  m[ 14 ] = -(far_val + near_val) / (far_val - near_val);

  matrix_curr[ matrix_mode ] = mat4_mul(matrix_curr[ matrix_mode ], m);
}

void gl_matrix_translate(float x, float y, float z)
{
  Mat4 m = MAT4_IDENTITY;

  m[ 12 ] = x;
  m[ 13 ] = y;
  m[ 14 ] = z;

  matrix_curr[ matrix_mode ] = mat4_mul(matrix_curr[ matrix_mode ], m);
}

void gl_matrix_rotate(float angle_degrees, float x, float y, float z)
{
  float const len = std::sqrt((x * x) + (y * y) + (z * z));
  if (len == 0.0F) {
    return;
  }
  x /= len;
  y /= len;
  z /= len;

  float const rad = angle_degrees * (std::numbers::pi_v< float > / 180.0F);
  float const c   = std::cos(rad);
  float const s   = std::sin(rad);
  float const t   = 1.0F - c;

  Mat4 m = MAT4_IDENTITY;

  m[ 0 ]  = (t * x * x) + c;
  m[ 1 ]  = (t * x * y) + (s * z);
  m[ 2 ]  = (t * x * z) - (s * y);
  m[ 4 ]  = (t * x * y) - (s * z);
  m[ 5 ]  = (t * y * y) + c;
  m[ 6 ]  = (t * y * z) + (s * x);
  m[ 8 ]  = (t * x * z) + (s * y);
  m[ 9 ]  = (t * y * z) - (s * x);
  m[ 10 ] = (t * z * z) + c;

  matrix_curr[ matrix_mode ] = mat4_mul(matrix_curr[ matrix_mode ], m);
}

//
// The one and only shader.
//
static const char *const GL_VERTEX_SHADER_SRC = R"(#version 300 es
layout(location = 0) in vec2 a_uv;
layout(location = 1) in vec2 a_xy;
layout(location = 2) in vec4 a_color;

uniform mat4 u_mvp;

out vec2 v_uv;
out vec4 v_color;

void main()
{
  gl_Position = u_mvp * vec4(a_xy, 0.0, 1.0);
  v_uv        = a_uv;
  v_color     = a_color;
}
)";

//
// u_flags bit 0 : modulate with the texture
// u_flags bit 1 : use the per vertex colour, else the uniform colour
//
static const char *const GL_FRAGMENT_SHADER_SRC = R"(#version 300 es
precision mediump float;

in vec2 v_uv;
in vec4 v_color;

uniform sampler2D u_tex;
uniform vec4      u_color;
uniform int       u_flags;

out vec4 o_color;

void main()
{
  vec4 c = ((u_flags & 2) != 0) ? v_color : u_color;
  if ((u_flags & 1) != 0) {
    c *= texture(u_tex, v_uv);
  }
  o_color = c;
}
)";

static constexpr GLuint ATTR_UV    = 0;
static constexpr GLuint ATTR_XY    = 1;
static constexpr GLuint ATTR_COLOR = 2;

static constexpr GLint FLAG_TEXTURE      = 1;
static constexpr GLint FLAG_VERTEX_COLOR = 2;

static GLuint gl_program;
static GLuint gl_vao;
static GLuint gl_vbo;
static GLint  gl_loc_mvp   = -1;
static GLint  gl_loc_tex   = -1;
static GLint  gl_loc_color = -1;
static GLint  gl_loc_flags = -1;

static auto gl_shader_compile(GLenum type, const char *src) -> GLuint
{
  GLuint shader = glCreateShader(type);
  glShaderSource(shader, 1, &src, nullptr);
  glCompileShader(shader);

  GLint ok = 0;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
  if (ok == 0) {
    int                                    chars = 0;
    std::array< char, GL_INFO_LOG_LENGTH > info {};
    glGetShaderInfoLog(shader, GL_INFO_LOG_LENGTH, &chars, info.data());
    CROAK("GLES: shader compile failed: [%s], %d chars", info.data(), chars);
  }
  return shader;
}

void gl_shader_init()
{
  TRACE();

  if (gl_program != 0) {
    return;
  }

  DBG("GLES: create shader");

  GLuint vs = gl_shader_compile(GL_VERTEX_SHADER, GL_VERTEX_SHADER_SRC);
  GLuint fs = gl_shader_compile(GL_FRAGMENT_SHADER, GL_FRAGMENT_SHADER_SRC);

  gl_program = glCreateProgram();
  glAttachShader(gl_program, vs);
  glAttachShader(gl_program, fs);
  glLinkProgram(gl_program);

  GLint ok = 0;
  glGetProgramiv(gl_program, GL_LINK_STATUS, &ok);
  if (ok == 0) {
    std::array< char, 1024 > info {};
    glGetProgramInfoLog(gl_program, static_cast< GLsizei >(info.size()), nullptr, info.data());
    CROAK("GLES: shader link failed: %s", info.data());
  }

  glDetachShader(gl_program, vs);
  glDetachShader(gl_program, fs);
  glDeleteShader(vs);
  glDeleteShader(fs);

  gl_loc_mvp   = glGetUniformLocation(gl_program, "u_mvp");
  gl_loc_tex   = glGetUniformLocation(gl_program, "u_tex");
  gl_loc_color = glGetUniformLocation(gl_program, "u_color");
  gl_loc_flags = glGetUniformLocation(gl_program, "u_flags");

  glGenVertexArrays(1, &gl_vao);
  glGenBuffers(1, &gl_vbo);

  GL_ERROR_CHECK();
}

void gl_shader_fini()
{
  TRACE();

  if (gl_vbo != 0) {
    glDeleteBuffers(1, &gl_vbo);
    gl_vbo = 0;
  }
  if (gl_vao != 0) {
    glDeleteVertexArrays(1, &gl_vao);
    gl_vao = 0;
  }
  if (gl_program != 0) {
    glDeleteProgram(gl_program);
    gl_program = 0;
  }
}

//
// The vertex layouts we draw with. In all cases the position is a pair of shorts.
//
enum {
  VERTEX_XY,         // x,y
  VERTEX_XY_RGBA,    // x,y,r,g,b,a
  VERTEX_UV_XY_RGBA, // u,v,x,y,r,g,b,a
};

static void gl_draw(GLenum mode, int layout, const void *data, GLsizei stride, GLsizei nvertices, GLuint tex)
{
  if (nvertices <= 0) {
    return;
  }

  gl_shader_init();

  glUseProgram(gl_program);

  Mat4 const mvp = mat4_mul(matrix_curr[ GL_MATRIX_PROJECTION ], matrix_curr[ GL_MATRIX_MODELVIEW ]);
  glUniformMatrix4fv(gl_loc_mvp, 1, GL_FALSE, mvp.data());

  constexpr float inv255 = 1.0F / 255.0F;
  glUniform4f(gl_loc_color, static_cast< float >(gl_current_color[ 0 ]) * inv255, static_cast< float >(gl_current_color[ 1 ]) * inv255,
              static_cast< float >(gl_current_color[ 2 ]) * inv255, static_cast< float >(gl_current_color[ 3 ]) * inv255);

  GLint flags = 0;
  if (layout != VERTEX_XY) {
    flags |= FLAG_VERTEX_COLOR;
  }
  if (tex != 0) {
    flags |= FLAG_TEXTURE;
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex);
    glUniform1i(gl_loc_tex, 0);
  }
  glUniform1i(gl_loc_flags, flags);

  glBindVertexArray(gl_vao);
  glBindBuffer(GL_ARRAY_BUFFER, gl_vbo);
  glBufferData(GL_ARRAY_BUFFER, static_cast< GLsizeiptr >(stride) * nvertices, data, GL_STREAM_DRAW);

  size_t offset = 0;

  if (layout == VERTEX_UV_XY_RGBA) {
    glEnableVertexAttribArray(ATTR_UV);
    glVertexAttribPointer(ATTR_UV, 2, GL_FLOAT, GL_FALSE, stride, reinterpret_cast< const void * >(offset));
    offset += SIZEOF(GLfloat) * 2;
  } else {
    glDisableVertexAttribArray(ATTR_UV);
  }

  glEnableVertexAttribArray(ATTR_XY);
  glVertexAttribPointer(ATTR_XY, 2, GL_SHORT, GL_FALSE, stride, reinterpret_cast< const void * >(offset));
  offset += SIZEOF(GLshort) * 2;

  if (layout != VERTEX_XY) {
    glEnableVertexAttribArray(ATTR_COLOR);
    glVertexAttribPointer(ATTR_COLOR, 4, GL_UNSIGNED_BYTE, GL_TRUE, stride, reinterpret_cast< const void * >(offset));
  } else {
    glDisableVertexAttribArray(ATTR_COLOR);
  }

#ifdef DEBUG_BUILD
  GL_ERROR_CHECK();
#endif

  glDrawArrays(mode, 0, nvertices);

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  if (tex != 0) {
    glBindTexture(GL_TEXTURE_2D, 0);
  }
}

void blit_flush()
{
  TRACE_DEBUG();

  if (gl_array_buf == bufp) [[unlikely]] {
    return;
  }

  auto const nvertices
      = static_cast< GLsizei >((reinterpret_cast< char * >(bufp) - reinterpret_cast< char * >(gl_array_buf)) / NUMBER_BYTES_PER_VERTEX_2D);

  gl_draw(GL_TRIANGLE_STRIP, VERTEX_UV_XY_RGBA, gl_array_buf, static_cast< GLsizei >(NUMBER_BYTES_PER_VERTEX_2D), nvertices,
          static_cast< GLuint >(buf_tex));

  blit_init();
}

void blit_flush_colored_triangle_fan(float *b, const float *e)
{
  TRACE_DEBUG();

  static const GLsizei stride = (SIZEOF(GLshort) * NUMBER_DIMENSIONS_PER_COORD_2D) + (SIZEOF(GLubyte) * NUMBER_COMPONENTS_PER_COLOR);

  auto const nvertices = static_cast< GLsizei >((reinterpret_cast< const char * >(e) - reinterpret_cast< char * >(b)) / stride);

  gl_draw(GL_TRIANGLE_FAN, VERTEX_XY_RGBA, b, stride, nvertices, 0);

  blit_init();
}

void blit_flush_triangle_fan(float *b, const float *e)
{
  TRACE_DEBUG();

  static const GLsizei stride = SIZEOF(GLshort) * NUMBER_DIMENSIONS_PER_COORD_2D;

  auto const nvertices = static_cast< GLsizei >((reinterpret_cast< const char * >(e) - reinterpret_cast< char * >(b)) / stride);

  gl_draw(GL_TRIANGLE_FAN, VERTEX_XY, b, stride, nvertices, 0);

  blit_init();
}

//
// OpenGL ES 3.0 entry points are all exported directly by libGLESv2 so there is nothing to load.
//
void gl_ext_init() { TRACE(); }

#endif
