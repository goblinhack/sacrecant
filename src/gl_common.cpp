//
// Copyright goblinhack@gmail.com
//

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

ENUM_DEF_C(FBO_ENUM, FboEnum)

GLuint g_fbo_id[ FBO_ENUM_MAX ];
GLuint g_fbo_tex_id[ FBO_ENUM_MAX ];
GLuint g_render_buf_id[ FBO_ENUM_MAX ];
isize  g_fbo_size[ FBO_ENUM_MAX ];

FboEnum fbo_locked = FBO_NONE;
FboEnum fbo_last   = FBO_NONE;

void gl_error(GLenum errCode)
{
  TRACE();

  if (errCode == GL_INVALID_ENUM) {
    log("GL: error, GL_INVALID_ENUM");
  } else if (errCode == GL_INVALID_VALUE) {
    log("GL: error, GL_INVALID_VALUE");
  } else if (errCode == GL_INVALID_OPERATION) {
    log("GL: error, GL_INVALID_OPERATION");
#ifndef WEB_BUILD
  } else if (errCode == GL_STACK_OVERFLOW) {
    log("GL: error, GL_STACK_OVERFLOW");
  } else if (errCode == GL_STACK_UNDERFLOW) {
    log("GL: error, GL_STACK_UNDERFLOW");
#endif
  } else if (errCode == GL_OUT_OF_MEMORY) {
    log("GL: error, GL_OUT_OF_MEMORY");
  } else if (errCode == GL_INVALID_FRAMEBUFFER_OPERATION) {
    log("GL: error, GL_INVALID_FRAMEBUFFER_OPERATION");
  } else {
    log("GL: unknown error, %d", errCode);
  }
}

//
// The equivalent of glColor4ub. Only used for draws that do not have a per vertex colour.
//
std::array< GLubyte, 4 > gl_current_color = {255, 255, 255, 255};

void gl_color_set(GLubyte r, GLubyte g, GLubyte b, GLubyte a) { gl_current_color = {r, g, b, a}; }

void gl_clear()
{
  TRACE();

  GLCOLOR(WHITE);
  glClearColor(0, 0, 0, 0);
  glClear(GL_COLOR_BUFFER_BIT);
}

void gl_clear_fbo_all()
{
  TRACE();

  FOR_ALL_FBO(i)
  {
    blit_fbo_bind(i);
    gl_clear();
    blit_fbo_unbind();
  }
}

void gl_init_fbo(Gamep g, FboEnum fbo)
{
  log("GL: OpenGL create FBO %s", FboEnum_to_string(fbo).c_str());
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

  DBG("GL: OpenGL created FBOs");
  GL_ERROR_CHECK();

  if (fbo == FBO_NONE) {
    first = false;
  }
}

void gl_fini_fbo(Gamep g)
{
  log("GL: OpenGL destroy FBOs");
  TRACE_INDENT();

  GL_ERROR_CHECK();

  FOR_ALL_FBO(i)
  {
    int tex_width  = 0;
    int tex_height = 0;

    fbo_get_size(g, i, tex_width, tex_height);

    gl_fini_fbo_(&g_render_buf_id[ i ], &g_fbo_id[ i ], &g_fbo_tex_id[ i ], tex_width, tex_height);

    g_fbo_size[ i ] = isize(0, 0);
  }

  DBG("GL: OpenGL destroyed FBOs");
  GL_ERROR_CHECK();
}

void fbo_get_size(Gamep g, FboEnum fbo, int &w, int &h)
{
  TRACE();

  switch (fbo) {
    case FBO_MAP_BG_PREVIOUSLY_SEEN_TILES :
    case FBO_MAP_BG_FLOOR_WATER_LAVA :
    case FBO_MAP_FG :
    case FBO_MAP_FG_OVERLAY :
    case FBO_MAP_LAVA_OVERLAY :
      w = game_map_fbo_width_get(g);
      h = game_map_fbo_height_get(g);
      break;
    case FBO_MAP_LIGHT :
      w = static_cast< int >(MAP_WIDTH) * static_cast< int >(TILE_WIDTH);
      h = static_cast< int >(MAP_HEIGHT) * static_cast< int >(TILE_HEIGHT);
      break;
    case FBO_FULL_SCREEN_PREVIOUSLY_SEEN_TILES :
    case FBO_FULL_SCREEN_VISIBLE_TILES :
    case FBO_FULL_SCREEN_LEVEL_CURR :
    case FBO_FULL_SCREEN_LEVEL_BELOW :
    case FBO_FULL_SCREEN_LEVEL_MERGED :
    case FBO_FULL_SCREEN_WID :
    case FBO_FULL_SCREEN_FINAL :
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
    case FBO_FLAMES :
    case FBO_SKULLS :
      w = 256;
      h = 192;
      break;
    default : //
      CROAK("Need size for fbo %d", fbo);
      break;
  }
}

void fbo_get_curr_size(Gamep g, int &w, int &h) { fbo_get_size(g, fbo_last, w, h); }
