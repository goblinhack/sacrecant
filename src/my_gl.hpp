//
// Copyright goblinhack@gmail.com
//

#ifndef MY_GLAPI_HPP
#define MY_GLAPI_HPP

#include "my_enum.hpp"
#include "my_game_defs.hpp"
#include "my_size.hpp"
#include "my_spoint.hpp"

#include <array>

#ifdef GLES_BUILD
//
// OpenGL ES 3.0 is used on all platforms. The legacy desktop OpenGL fixed function
// pipeline (matrix stacks, client arrays, glColor etc...) does not exist in ES, so
// see gl.cpp for the small shader based replacements: gl_matrix_*, gl_color_*.
//
// On Linux this comes from libgles-dev (-lGLESv2). On Windows and macOS an ES 3.0
// implementation such as ANGLE is needed.
//
#include <GLES3/gl3.h>
#include <SDL_opengles2.h>
#else
//
// Desktop OpenGL
//
#define GL_GLEXT_PROTOTYPES

#include "my_enum.hpp"
#include "my_game_defs.hpp"
#include "my_spoint.hpp"

/* Defined before OpenGL and GLUT includes to avoid deprecation messages */
#define GL_SILENCE_DEPRECATION

#ifdef _WIN32
#include "SDL_opengl.h"
#endif

#define glOrthof glOrtho

#ifdef _WIN32
//    #include "GL/glew.h"
#elif __APPLE__
#include "OpenGL/gl.h"
#include "OpenGL/glext.h"
#include "TargetConditionals.h"
#else
// linux
#include "GL/gl.h"
#include "GL/glext.h"
#endif

#endif

#include "my_color.hpp"

//
// Push elements onto the array buffer.
//
#define GL_PUSH_TEXCOORD(p, x, y)                                                                                                               \
  {                                                                                                                                             \
    *(p)++ = x;                                                                                                                                 \
    *(p)++ = y;                                                                                                                                 \
  }

//
// Push elements onto the array buffer.
//
#define GL_PUSH_VERTEX(p, x, y)                                                                                                                 \
  {                                                                                                                                             \
    auto c = (GLshort *) (p);                                                                                                                   \
    *c++   = x;                                                                                                                                 \
    *c++   = y;                                                                                                                                 \
    (p)    = (GLfloat *) c;                                                                                                                     \
  }

//
// Push elements onto the array buffer.
//
#define GL_PUSH_RGBA(p, r, g, b, a)                                                                                                             \
  {                                                                                                                                             \
    auto c = (GLubyte *) (p);                                                                                                                   \
    *c++   = r;                                                                                                                                 \
    *c++   = g;                                                                                                                                 \
    *c++   = b;                                                                                                                                 \
    *c++   = a;                                                                                                                                 \
    (p)    = (GLfloat *) c;                                                                                                                     \
  }

#define GL_PUSH_VERTEX_SIMPLE(x, y)                                                                                                             \
  *xyp++ = x;                                                                                                                                   \
  *xyp++ = y;

#define GL_PUSH_POINT(p, p_end, x1, y1)                                                                                                         \
  {                                                                                                                                             \
    GL_PUSH_VERTEX(p, x1, y1);                                                                                                                  \
  }

#define GL_PUSH_TEX_POINT(p, p_end, tx, ty, x1, y1, r1, g1, b1, a1)                                                                             \
  {                                                                                                                                             \
    GL_PUSH_TEXCOORD(p, tx, ty);                                                                                                                \
    GL_PUSH_VERTEX(p, x1, y1);                                                                                                                  \
    GL_PUSH_RGBA(p, r1, g1, b1, a1);                                                                                                            \
  }

#define PUSH_POINT(x1, y1)                                                                                                                      \
  {                                                                                                                                             \
    GL_PUSH_POINT(bufp, bufp_end, x1, y1);                                                                                                      \
  }

//
// Frame buffer objects
//
#define FBO_ENUM(list_macro)                                                                                                                    \
  list_macro(FBO_MAP_BG_PREVIOUSLY_SEEN_TILES, "FBO_MAP_BG_PREVIOUSLY_SEEN_TILES"),               /* newline */                                 \
      list_macro(FBO_MAP_BG_FLOOR_WATER_LAVA, "FBO_MAP_BG_FLOOR_WATER_LAVA"),                     /* newline */                                 \
      list_macro(FBO_MAP_LAVA_OVERLAY, "FBO_MAP_LAVA_OVERLAY"),                                   /* newline */                                 \
      list_macro(FBO_MAP_FG, "FBO_MAP_FG"),                                                       /* newline */                                 \
      list_macro(FBO_MAP_FG_OVERLAY, "FBO_MAP_FG_OVERLAY"),                                       /* newline */                                 \
      list_macro(FBO_MAP_LIGHT, "FBO_MAP_LIGHT"),                                                 /* newline */                                 \
      list_macro(FBO_FULL_SCREEN_PREVIOUSLY_SEEN_TILES, "FBO_FULL_SCREEN_PREVIOUSLY_SEEN_TILES"), /* newline */                                 \
      list_macro(FBO_FULL_SCREEN_VISIBLE_TILES, "FBO_FULL_SCREEN_VISIBLE_TILES"),                 /* newline */                                 \
      list_macro(FBO_FULL_SCREEN_LEVEL_CURR, "FBO_FULL_SCREEN_LEVEL_CURR"),                       /* newline */                                 \
      list_macro(FBO_FULL_SCREEN_LEVEL_BELOW, "FBO_FULL_SCREEN_LEVEL_BELOW"),                     /* newline */                                 \
      list_macro(FBO_FULL_SCREEN_LEVEL_MERGED, "FBO_FULL_SCREEN_LEVEL_MERGED"),                   /* newline */                                 \
      list_macro(FBO_MINIMAP_LEVEL, "FBO_MINIMAP_LEVEL"),                                         /* newline */                                 \
      list_macro(FBO_FULL_SCREEN_WID, "FBO_FULL_SCREEN_WID"),                                     /* newline */                                 \
      list_macro(FBO_SPRITE1, "FBO_SPRITE1"),                                                     /* newline */                                 \
      list_macro(FBO_FLAMES, "FBO_FLAMES"),                                                       /* newline */                                 \
      list_macro(FBO_SKULLS, "FBO_SKULLS"),                                                       /* newline */                                 \
      list_macro(FBO_FULL_SCREEN_FINAL, "FBO_FULL_SCREEN_FINAL"),                                 /* newline */

ENUM_DEF_H(FBO_ENUM, FboEnum);

#define FBO_ENUM_FIRST ((FboEnum) 0)
#define FBO_NONE       ((FboEnum) (FBO_ENUM_MAX))

#define FOR_ALL_FBO(_iter_)                                                                                                                     \
  for (FboEnum _iter_ = FBO_ENUM_FIRST; /* newline */                                                                                           \
       (_iter_) < FBO_ENUM_MAX;         /* newline */                                                                                           \
       (_iter_) = static_cast< FboEnum >(static_cast< int >(_iter_) + 1))

extern GLuint  g_fbo_id[ FBO_ENUM_MAX ];
extern GLuint  g_fbo_tex_id[ FBO_ENUM_MAX ];
extern GLuint  g_render_buf_id[ FBO_ENUM_MAX ];
extern isize   g_fbo_size[ FBO_ENUM_MAX ];
extern FboEnum fbo_locked;
extern FboEnum fbo_last;

extern std::array< GLubyte, 4 > gl_current_color;

struct LightPixels;

auto blit(int tex, GLshort left, GLshort top, GLshort right, GLshort bottom, const color &c) -> void;
void blit(int tex, float texMinX, float texMinY, float texMaxX, float texMaxY, GLshort left, GLshort top, GLshort right, GLshort bottom,
          const color &c);
void blit(int tex, float texMinX, float texMinY, float texMaxX, float texMaxY, GLshort left, GLshort top, GLshort right, GLshort bottom,
          const color &color_bl, const color &color_br, const color &color_tl, const color &color_tr);
void blit(int tex, float texMinX, float texMinY, float texMaxX, float texMaxY, GLshort pixMinX, GLshort pixMinY, GLshort pixMaxX,
          GLshort pixMaxY, const color &c, struct LightPixels * /*light_pixels*/, bool is_blit_flush_per_line);
void blit(int tex, float texMinX, float texMinY, float texMaxX, float texMaxY, GLshort pixMinX, GLshort pixMinY, GLshort pixMaxX,
          GLshort pixMaxY, struct LightPixels * /*light_pixels*/, bool is_blit_flush_per_line);
auto blit_fbo(Gamep g, FboEnum fbo, color c) -> void;
auto blit_fbo(Gamep g, FboEnum fbo, int tl_x, int tl_y, int br_x, int br_y, color c) -> void;
auto blit_fbo_push(FboEnum fbo) -> void;
auto blit_fbo_pop() -> void;
auto blit_fbo_bind(FboEnum fbo) -> void;
auto blit_fbo_bind_locked(FboEnum fbo) -> void;
auto blit_fbo_unbind() -> void;
auto blit_fbo_unbind_locked() -> void;
auto blit_fini() -> void;
auto blit_flush() -> void;
auto blit_flush_colored_triangle_fan(float *begin, const float *end) -> void;
auto blit_flush_colored_triangle_fan() -> void;
auto blit_flush_triangle_fan(float *begin, const float *end) -> void;
auto blit_flush_triangle_fan() -> void;
auto blit_init() -> void;
auto fbo_get_size(Gamep g, FboEnum fbo, int &w, int &h) -> void;
auto fbo_get_curr_size(Gamep g, int &w, int &h) -> void;

auto gl_enter_2d_mode(Gamep g, int w, int h) -> void;
auto gl_enter_2d_mode(Gamep g) -> void;
auto gl_error(GLenum err) -> void;
auto gl_ext_init() -> void;
auto gl_fini_2d_mode(Gamep g) -> void;
auto gl_fini_fbo(Gamep g) -> void;
auto gl_init_2d_mode(Gamep g) -> void;
void gl_init_fbo_(FboEnum fbo, GLuint *render_buf_id, GLuint *fbo_id, GLuint *fbo_tex_id, GLuint tex_width, GLuint tex_height);
void gl_fini_fbo_(GLuint *render_buf_id, GLuint *fbo_id, GLuint *fbo_tex_id, GLuint tex_width, GLuint tex_height);
auto gl_init_fbo(Gamep g, FboEnum fbo = FBO_NONE) -> void;
auto gl_leave_2d_mode(Gamep g) -> void;
auto gl_clear() -> void;
void gl_clear_fbo_all();

#ifdef GLES_BUILD
//
// Replacements for the legacy fixed function state that no longer exists in OpenGL ES.
//
enum {
  GL_MATRIX_PROJECTION,
  GL_MATRIX_MODELVIEW,
};
#else
enum {
  GL_MATRIX_PROJECTION = GL_PROJECTION,
  GL_MATRIX_MODELVIEW  = GL_MODELVIEW,
};
#endif

auto gl_matrix_mode(int mode) -> void;
auto gl_matrix_push() -> void;
auto gl_matrix_pop() -> void;
auto gl_matrix_identity() -> void;
auto gl_matrix_ortho(float left, float right, float bottom, float top, float near_val, float far_val) -> void;
auto gl_matrix_translate(float x, float y, float z) -> void;
auto gl_matrix_rotate(float angle_degrees, float x, float y, float z) -> void;
auto gl_color_set(GLubyte r, GLubyte g, GLubyte b, GLubyte a) -> void;
auto gl_shader_init() -> void;
auto gl_shader_fini() -> void;

#define GL_ERROR_CHECK()                                                                                                                        \
  {                                                                                                                                             \
    auto errCode = glGetError();                                                                                                                \
    [[likely]] if ((errCode == GL_NO_ERROR)) { /* log("GL: ok at %s:%s line %u", SRC_FILE_NAME, SRC_FUNC_NAME, SRC_LINE_NUM); */                \
    } else {                                                                                                                                    \
      log("GL: error at %s:%s line %u", SRC_FILE_NAME, SRC_FUNC_NAME, SRC_LINE_NUM);                                                            \
      gl_error(errCode);                                                                                                                        \
    }                                                                                                                                           \
  }

#ifdef _WIN32
extern PFNGLCREATEPROGRAMPROC           glCreateProgram_EXT;
extern PFNGLDELETEPROGRAMPROC           glDeleteProgram_EXT;
extern PFNGLISPROGRAMPROC               glIsProgram_EXT;
extern PFNGLCREATESHADERPROC            glCreateShader_EXT;
extern PFNGLDELETESHADERPROC            glDeleteShader_EXT;
extern PFNGLSHADERSOURCEPROC            glShaderSource_EXT;
extern PFNGLCOMPILESHADERPROC           glCompileShader_EXT;
extern PFNGLATTACHSHADERPROC            glAttachShader_EXT;
extern PFNGLDETACHSHADERPROC            glDetachShader_EXT;
extern PFNGLGETATTACHEDSHADERSPROC      glGetAttachedShaders_EXT;
extern PFNGLLINKPROGRAMPROC             glLinkProgram_EXT;
extern PFNGLUSEPROGRAMPROC              glUseProgram_EXT;
extern PFNGLGETSHADERINFOLOGPROC        glGetShaderInfoLog_EXT;
extern PFNGLGETPROGRAMINFOLOGPROC       glGetProgramInfoLog_EXT;
extern PFNGLGETUNIFORMLOCATIONPROC      glGetUniformLocation_EXT;
extern PFNGLUNIFORM1FPROC               glUniform1f_EXT;
extern PFNGLUNIFORM1IPROC               glUniform1i_EXT;
extern PFNGLUNIFORM2FVPROC              glUniform2fv_EXT;
extern PFNGLUNIFORM3FVPROC              glUniform3fv_EXT;
extern PFNGLGENERATEMIPMAPPROC          glGenerateMipmap_EXT;
extern PFNGLGENFRAMEBUFFERSPROC         glGenFramebuffers_EXT;
extern PFNGLDELETEFRAMEBUFFERSPROC      glDeleteFramebuffers_EXT;
extern PFNGLBINDFRAMEBUFFERPROC         glBindFramebuffer_EXT;
extern PFNGLGENRENDERBUFFERSPROC        glGenRenderbuffers_EXT;
extern PFNGLDELETERENDERBUFFERSPROC     glDeleteRenderbuffers_EXT;
extern PFNGLBINDRENDERBUFFERPROC        glBindRenderbuffer_EXT;
extern PFNGLRENDERBUFFERSTORAGEPROC     glRenderbufferStorage_EXT;
extern PFNGLFRAMEBUFFERRENDERBUFFERPROC glFramebufferRenderbuffer_EXT;
extern PFNGLFRAMEBUFFERTEXTURE2DPROC    glFramebufferTexture2D_EXT;
extern PFNGLCHECKFRAMEBUFFERSTATUSPROC  glCheckFramebufferStatus_EXT;
extern PFNGLGENBUFFERSARBPROC           glGenBuffersARB_EXT;
extern PFNGLBINDBUFFERARBPROC           glBindBufferARB_EXT;
extern PFNGLBUFFERDATAARBPROC           glBufferDataARB_EXT;
extern PFNGLDELETEBUFFERSARBPROC        glDeleteBuffersARB_EXT;
extern PFNGLDEBUGMESSAGECALLBACKPROC    glDebugMessageCallback_EXT;
#elif GLES_BUILD
#define glDebugMessageCallback_EXT    glDebugMessageCallback
#define glCreateProgram_EXT           glCreateProgram
#define glDeleteProgram_EXT           glDeleteProgram
#define glIsProgram_EXT               glIsProgram
#define glCreateShader_EXT            glCreateShader
#define glDeleteShader_EXT            glDeleteShader
#define glShaderSource_EXT            glShaderSource
#define glCompileShader_EXT           glCompileShader
#define glAttachShader_EXT            glAttachShader
#define glDetachShader_EXT            glDetachShader
#define glGetAttachedShaders_EXT      glGetAttachedShaders
#define glLinkProgram_EXT             glLinkProgram
#define glUseProgram_EXT              glUseProgram
#define glGetShaderInfoLog_EXT        glGetShaderInfoLog
#define glGetProgramInfoLog_EXT       glGetProgramInfoLog
#define glGetUniformLocation_EXT      glGetUniformLocation
#define glUniform1f_EXT               glUniform1f
#define glUniform1i_EXT               glUniform1i
#define glUniform2fv_EXT              glUniform2fv
#define glUniform3fv_EXT              glUniform3fv
#define glGenerateMipmap_EXT          glGenerateMipmap
#define glGenFramebuffers_EXT         glGenFramebuffers
#define glDeleteFramebuffers_EXT      glDeleteFramebuffers
#define glBindFramebuffer_EXT         glBindFramebuffer
#define glGenRenderbuffers_EXT        glGenRenderbuffers
#define glDeleteRenderbuffers_EXT     glDeleteRenderbuffers
#define glBindRenderbuffer_EXT        glBindRenderbuffer
#define glRenderbufferStorage_EXT     glRenderbufferStorage
#define glFramebufferRenderbuffer_EXT glFramebufferRenderbuffer
#define glFramebufferTexture2D_EXT    glFramebufferTexture2D
#define glCheckFramebufferStatus_EXT  glCheckFramebufferStatus
#define glGenBuffersARB_EXT           glGenBuffersARB
#define glBindBufferARB_EXT           glBindBufferARB
#define glBufferDataARB_EXT           glBufferDataARB
#define glDeleteBuffersARB_EXT        glDeleteBuffersARB
#else
#define glDebugMessageCallback_EXT    glDebugMessageCallbackEXT
#define glCreateProgram_EXT           glCreateProgramEXT
#define glDeleteProgram_EXT           glDeleteProgramEXT
#define glIsProgram_EXT               glIsProgramEXT
#define glCreateShader_EXT            glCreateShaderEXT
#define glDeleteShader_EXT            glDeleteShaderEXT
#define glShaderSource_EXT            glShaderSourceEXT
#define glCompileShader_EXT           glCompileShaderEXT
#define glAttachShader_EXT            glAttachShaderEXT
#define glDetachShader_EXT            glDetachShaderEXT
#define glGetAttachedShaders_EXT      glGetAttachedShadersEXT
#define glLinkProgram_EXT             glLinkProgramEXT
#define glUseProgram_EXT              glUseProgramEXT
#define glGetShaderInfoLog_EXT        glGetShaderInfoLogEXT
#define glGetProgramInfoLog_EXT       glGetProgramInfoLogEXT
#define glGetUniformLocation_EXT      glGetUniformLocationEXT
#define glUniform1f_EXT               glUniform1fEXT
#define glUniform1i_EXT               glUniform1iEXT
#define glUniform2fv_EXT              glUniform2fvEXT
#define glUniform3fv_EXT              glUniform3fvEXT
#define glGenerateMipmap_EXT          glGenerateMipmapEXT
#define glGenFramebuffers_EXT         glGenFramebuffersEXT
#define glDeleteFramebuffers_EXT      glDeleteFramebuffersEXT
#define glBindFramebuffer_EXT         glBindFramebufferEXT
#define glGenRenderbuffers_EXT        glGenRenderbuffersEXT
#define glDeleteRenderbuffers_EXT     glDeleteRenderbuffersEXT
#define glBindRenderbuffer_EXT        glBindRenderbufferEXT
#define glRenderbufferStorage_EXT     glRenderbufferStorageEXT
#define glFramebufferRenderbuffer_EXT glFramebufferRenderbufferEXT
#define glFramebufferTexture2D_EXT    glFramebufferTexture2DEXT
#define glCheckFramebufferStatus_EXT  glCheckFramebufferStatusEXT
#define glGenBuffersARB_EXT           glGenBuffersARBEXT
#define glBindBufferARB_EXT           glBindBufferARBEXT
#define glBufferDataARB_EXT           glBufferDataARBEXT
#define glDeleteBuffersARB_EXT        glDeleteBuffersARBEXT
#endif

extern uint32_t NUMBER_BYTES_PER_VERTEX_2D;

extern GLfloat *bufp;
extern GLfloat *bufp_end;
extern GLshort  glapi_last_bottom;
extern GLshort  glapi_last_right;

extern float *gl_array_buf;
extern float *gl_array_buf_end;
extern float  glapi_last_tex_bottom;
extern float  glapi_last_tex_right;

extern int buf_tex;

//
// x and y per element.
//
enum { NUMBER_DIMENSIONS_PER_COORD_2D = 2 };

//
// r,g,b,a per element
//
enum { NUMBER_COMPONENTS_PER_COLOR = 4 };

#endif
