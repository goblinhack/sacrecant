//
// Copyright goblinhack@gmail.com
//

#ifndef MY_GLAPI_HPP
#define MY_GLAPI_HPP

#define GL_GLEXT_PROTOTYPES

#include "my_enum.hpp"
#include "my_game_defs.hpp"
#include "my_spoint.hpp"

/* Defined before OpenGL and GLUT includes to avoid deprecation messages */
#define GL_SILENCE_DEPRECATION

#ifdef __IPHONE_OS_VERSION_MIN_REQUIRED
#define __IPHONEOS__
#include "SDL_opengles.h"

#define CreateVertexBuffers(nobuf, bufptr) glGenBuffers(nobuf, bufptr)
#define DeleteVertexBuffers(nobuf, bufptr) glDeleteBuffers(nobuf, bufptr)
#else

#ifdef _WIN32
#include "SDL_opengl.h"
#endif

#define glOrthof glOrtho
#endif

#ifdef _WIN32
//    #include "GL/glew.h"
#elif __APPLE__
#include "OpenGL/gl.h"
#include "OpenGL/glext.h"
#include "TargetConditionals.h"

#if TARGET_IPHONE_SIMULATOR
// iOS Simulator
#elif TARGET_OS_IPHONE
// iOS device
#elif TARGET_OS_MAC
// Other kinds of Mac OS
#else
// Unsupported platform
#endif
#else
// linux
#include "GL/gl.h"
#include "GL/glext.h"
#endif

#include "my_color.hpp"

//
// Push elements onto the array buffer.
//
#define GL_PUSH_TEXCOORD(p, x, y)                                                                                                          \
  {                                                                                                                                        \
    *(p)++ = x;                                                                                                                            \
    *(p)++ = y;                                                                                                                            \
  }

//
// Push elements onto the array buffer.
//
#define GL_PUSH_VERTEX(p, x, y)                                                                                                            \
  {                                                                                                                                        \
    auto c = (GLshort *) (p);                                                                                                              \
    *c++   = x;                                                                                                                            \
    *c++   = y;                                                                                                                            \
    (p)    = (GLfloat *) c;                                                                                                                \
  }

//
// Push elements onto the array buffer.
//
#define GL_PUSH_RGBA(p, r, g, b, a)                                                                                                        \
  {                                                                                                                                        \
    auto c = (GLubyte *) (p);                                                                                                              \
    *c++   = r;                                                                                                                            \
    *c++   = g;                                                                                                                            \
    *c++   = b;                                                                                                                            \
    *c++   = a;                                                                                                                            \
    (p)    = (GLfloat *) c;                                                                                                                \
  }

#define Vertex2(x, y)                                                                                                                      \
  *xyp++ = x;                                                                                                                              \
  *xyp++ = y;

//
// GL_PUSH_TRIANGLE
//
#define GL_PUSH_TRIANGLE_COLORED(p, p_end, x1, y1, x2, y2, x3, y3, r1, g1, b1, a1, r2, g2, b2, a2, r3, g3, b3, a3)                         \
  {                                                                                                                                        \
    GL_PUSH_VERTEX(p, x1, y1);                                                                                                             \
    GL_PUSH_RGBA(p, r1, g1, b1, a1);                                                                                                       \
    GL_PUSH_VERTEX(p, x2, y2);                                                                                                             \
    GL_PUSH_RGBA(p, r2, g2, b2, a2);                                                                                                       \
    GL_PUSH_VERTEX(p, x3, y3);                                                                                                             \
    GL_PUSH_RGBA(p, r2, g3, b3, a3);                                                                                                       \
  }

#define GL_PUSH_COLORED_POINT(p, p_end, x1, y1, r1, g1, b1, a1)                                                                            \
  {                                                                                                                                        \
    GL_PUSH_VERTEX(p, x1, y1);                                                                                                             \
    GL_PUSH_RGBA(p, r1, g1, b1, a1);                                                                                                       \
  }

#define GL_PUSH_POINT(p, p_end, x1, y1)                                                                                                    \
  {                                                                                                                                        \
    GL_PUSH_VERTEX(p, x1, y1);                                                                                                             \
  }

//
// GL_PUSH_TEX_POINT
//
#define GL_PUSH_TEX_POINT(p, p_end, tx, ty, x1, y1, r1, g1, b1, a1)                                                                        \
  {                                                                                                                                        \
    GL_PUSH_TEXCOORD(p, tx, ty);                                                                                                           \
    GL_PUSH_VERTEX(p, x1, y1);                                                                                                             \
    GL_PUSH_RGBA(p, r1, g1, b1, a1);                                                                                                       \
  }

#define TRIANGLE_COLORED(x1, y1, x2, y2, x3, y3, r1, g1, b1, a1, r2, g2, b2, a2, r3, g3, b3, a3)                                           \
  {                                                                                                                                        \
    GL_PUSH_TRIANGLE_COLORED(bufp, bufp_end, x1, y1, x2, y2, x3, y3, r1, g1, b1, a1, r2, g2, b2, a2, r3, g3, b3, a3);                      \
  }

#define PUSH_COLORED_POINT(x1, y1, r1, g1, b1, a1)                                                                                         \
  {                                                                                                                                        \
    GL_PUSH_COLORED_POINT(bufp, bufp_end, x1, y1, r1, g1, b1, a1);                                                                         \
  }

#define PUSH_POINT(x1, y1)                                                                                                                 \
  {                                                                                                                                        \
    GL_PUSH_POINT(bufp, bufp_end, x1, y1);                                                                                                 \
  }

#define PUSH_TEX_POINT(tx, ty, x1, y1, r1, g1, b1, a1)                                                                                     \
  {                                                                                                                                        \
    GL_PUSH_TEX_POINT(bufp, bufp_end, tx, ty, x1, y1, r1, g1, b1, a1);                                                                     \
  }

//
// GL_PUSH_TRIANGLE
//
#define GL_PUSH_TRIANGLE(p, p_end, x1, y1, x2, y2, x3, y3)                                                                                 \
  {                                                                                                                                        \
    GL_PUSH_VERTEX(p, x1, y1);                                                                                                             \
    GL_PUSH_VERTEX(p, x2, y2);                                                                                                             \
    GL_PUSH_VERTEX(p, x3, y3);                                                                                                             \
  }

#define triangle(x1, y1, x2, y2, x3, y3)                                                                                                   \
  {                                                                                                                                        \
    GL_PUSH_TRIANGLE(bufp, bufp_end, x1, y1, x2, y2, x3, y3);                                                                              \
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

//
// Frame buffer objects
//
#define FBO_ENUM(list_macro)                                                                                                               \
  list_macro(FBO_MAP_BG, "FBO_MAP_BG"),                     /* newline */                                                                  \
      list_macro(FBO_MAP_FG, "FBO_MAP_FG"),                 /* newline */                                                                  \
      list_macro(FBO_MAP_BG_MERGED, "FBO_MAP_BG_MERGED"),   /* newline */                                                                  \
      list_macro(FBO_MAP_FG_MERGED, "FBO_MAP_FG_MERGED"),   /* newline */                                                                  \
      list_macro(FBO_MAP_FG_OVERLAY, "FBO_MAP_FG_OVERLAY"), /* newline */                                                                  \
      list_macro(FBO_MAP_LIGHT, "FBO_MAP_LIGHT"),           /* newline */                                                                  \
      list_macro(FBO_MINIMAP_LEVEL, "FBO_MINIMAP_LEVEL"),   /* newline */                                                                  \
      list_macro(FBO_WID, "FBO_WID"),                       /* newline */                                                                  \
      list_macro(FBO_SPRITE1, "FBO_SPRITE1"),               /* newline */                                                                  \
      list_macro(FBO_FINAL, "FBO_FINAL"),                   /* newline */

ENUM_DEF_H(FBO_ENUM, FboEnum);

#define FBO_ENUM_FIRST ((FboEnum) 0)
#define FBO_NONE       ((FboEnum) (FBO_ENUM_MAX))

#define FOR_ALL_FBO(_iter_)                                                                                                                \
  for (FboEnum _iter_ = FBO_ENUM_FIRST; /* newline */                                                                                      \
       (_iter_) < FBO_ENUM_MAX;         /* newline */                                                                                      \
       (_iter_) = static_cast< FboEnum >(static_cast< int >(_iter_) + 1))

extern uint32_t NUMBER_BYTES_PER_VERTICE_2D;

extern GLfloat *bufp;
extern GLfloat *bufp_end;
extern GLshort  glapi_last_bottom;
extern GLshort  glapi_last_right;

extern float *gl_array_buf;
extern float *gl_array_buf_end;
extern float  glapi_last_tex_bottom;
extern float  glapi_last_tex_right;

extern int buf_tex;

extern GLuint g_fbo_id[ FBO_ENUM_MAX ];
extern GLuint g_fbo_tex_id[ FBO_ENUM_MAX ];
extern GLuint g_render_buf_id[ FBO_ENUM_MAX ];

struct LightPixels;

void blit(int tex, GLshort left, GLshort top, GLshort right, GLshort bottom, const color &c);
void blit(int tex, float texMinX, float texMinY, float texMaxX, float texMaxY, GLshort left, GLshort top, GLshort right, GLshort bottom,
          const color &c);
void blit(int tex, float texMinX, float texMinY, float texMaxX, float texMaxY, GLshort left, GLshort top, GLshort right, GLshort bottom,
          const color &color_bl, const color &color_br, const color &color_tl, const color &color_tr);
void blit(int tex, float texMinX, float texMinY, float texMaxX, float texMaxY, GLshort pixMinX, GLshort pixMinY, GLshort pixMaxX,
          GLshort pixMaxY, const color &c, struct LightPixels * /*light_pixels*/, bool is_blit_flush_per_line);
void blit_fbo(Gamep g, FboEnum fbo);
void blit_fbo(Gamep g, FboEnum fbo, int tl_x, int tl_y, int br_x, int br_y);
void blit_fbo_push(FboEnum fbo);
void blit_fbo_pop();
void blit_fbo_bind(FboEnum fbo);
void blit_fbo_bind_locked(FboEnum fbo);
void blit_fbo_unbind();
void blit_fbo_unbind_locked();
void blit_fini();
void blit_flush();
void blit_flush_colored_triangle_fan(float *begin, const float *end);
void blit_flush_colored_triangle_fan();
void blit_flush_triangle_fan(float *begin, const float *end);
void blit_flush_triangle_fan();
void blit_init();
void fbo_get_size(Gamep g, FboEnum fbo, int &w, int &h);
void fbo_get_curr_size(Gamep g, int &w, int &h);

void gl_blitline(GLshort left, GLshort top, GLshort right, GLshort bottom);
void gl_blitquad(spoint tl, spoint tr, spoint bl, spoint br);
void gl_blitquad(GLshort left, GLshort top, GLshort right, GLshort bottom);
void gl_blitsquare(GLshort left, GLshort top, GLshort right, GLshort bottom);
void gl_enter_2_5d_mode(Gamep);
void gl_enter_2d_mode(Gamep g, int w, int h);
void gl_enter_2d_mode(Gamep g);
void gl_error(GLenum err);
void gl_ext_init();
void gl_fini_2d_mode(Gamep g);
void gl_fini_fbo(Gamep g);
void gl_init_2d_mode(Gamep g);
void gl_init_fbo(Gamep g, FboEnum fbo = FBO_NONE);
void gl_leave_2d_mode(Gamep g);
void gl_clear();

#define GL_ERROR_CHECK()                                                                                                                   \
  {                                                                                                                                        \
    auto errCode = glGetError();                                                                                                           \
    [[likely]] if ((errCode == GL_NO_ERROR)) { /* log("GFX: ok at %s:%s line %u", SRC_FILE_NAME, SRC_FUNC_NAME, SRC_LINE_NUM); */          \
    } else {                                                                                                                               \
      log("GFX: error at %s:%s line %u", SRC_FILE_NAME, SRC_FUNC_NAME, SRC_LINE_NUM);                                                      \
      gl_error(errCode);                                                                                                                   \
    }                                                                                                                                      \
  }

#endif
