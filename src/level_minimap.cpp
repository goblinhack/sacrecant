//
// Changed by goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_color_defs.hpp"
#include "my_game.hpp"
#include "my_gl.hpp"
#include "my_globals.hpp"
#include "my_level.hpp"
#include "my_level_inlines.hpp"
#include "my_main.hpp"
#include "my_sdl_proto.hpp"
#include "my_tests.hpp"
#include "my_tex.hpp"

static Texp solid_tex;
static int  solid_tex_id;

static void level_minimap_levels_update(Gamep g, Levelsp v, Levelp l, const bool level_select)
{
  TRACE();

  auto *player = thing_player(g);
  if (player == nullptr) [[unlikely]] {
    return;
  }

  const FboEnum fbo = FBO_MINIMAP_LEVEL;
  const auto    dx  = 1;
  const auto    dy  = 1;

  int w = 0;
  int h = 0;
  fbo_get_size(g, fbo, w, h);
  gl_enter_2d_mode(g, w, h);

  blit_fbo_bind(fbo);
  {
    glBlendFunc(GL_ONE, GL_ZERO);
    gl_clear();

    blit_init();

    for (auto y = 0; y < MAP_HEIGHT; y++) {
      for (auto x = 0; x < MAP_WIDTH; x++) {
        color        c = BLACK;
        bpoint const p(x, y);

        if (! g_opt_debug1) {
          if (! level_has_seen_cached(g, v, l, p)) {
            continue;
          }
        }

        if (level_is_dirt(g, v, l, p) != nullptr) {
          c = GRAY10;
        }
        if (level_is_floor(g, v, l, p) != nullptr) {
          c = GRAY30;
        }
        if (level_is_corridor(g, v, l, p) != nullptr) {
          c = GRAY30;
        }
        if (level_is_wall(g, v, l, p) != nullptr) {
          c = GRAY50;
        }
        if (level_is_rock(g, v, l, p) != nullptr) {
          c = BROWN4;
        }
        if (level_is_bridge(g, v, l, p) != nullptr) {
          c = BROWN;
        }
        if (level_is_chasm(g, v, l, p) != nullptr) {
          c = BLACK;
        }
        if (level_is_water(g, v, l, p) != nullptr) {
          c = BLUE;
        }
        if (level_is_deep_water(g, v, l, p) != nullptr) {
          c = BLUE4;
        }
        if (level_is_door_locked(g, v, l, p) != nullptr) {
          if (level_open_is_door_locked(g, v, l, p) != nullptr) {
            // ignore
          } else {
            c = BROWN;
          }
        }
        if (level_is_door_secret(g, v, l, p) != nullptr) {
          if (level_open_is_door_secret(g, v, l, p) != nullptr) {
            // ignore
          } else {
            c = GRAY40;
          }
        }
        if (level_is_lava(g, v, l, p) != nullptr) {
          c = ORANGE;
        }
        if (level_is_teleport(g, v, l, p) != nullptr) {
          // ignore
          c = RED;
        }
        if (level_is_entrance(g, v, l, p) != nullptr) {
          c = PINK;
        }
        if (level_is_exit(g, v, l, p) != nullptr) {
          c = YELLOW;
        }
        if (level_is_fire(g, v, l, p) != nullptr) {
          c = ORANGE2;
        }
        if (level_is_player(g, v, l, p) != nullptr) {
          c = CYAN;
        }

        if (level_is_blit_shown_in_overlay(g, v, l, p) != nullptr) {
          //
          // Keep bright colors
          //
        } else if (level_select) {
          //
          // No vision in level selection
          //
          c.r /= 2;
          c.g /= 2;
          c.b /= 2;
        } else if (! thing_vision_can_see_tile(g, v, l, player, p)) {
          //
          // Dim
          //
          c.r /= 2;
          c.g /= 2;
          c.b /= 2;
        }

        auto X   = x;
        auto Y   = MAP_HEIGHT - y;
        auto tlx = X * dx;
        auto tly = Y * dy;
        auto brx = tlx + dx;
        auto bry = tly + dy;
        tly--;
        bry--;

        blit(solid_tex_id, tlx, tly, brx, bry, c);
      }
    }

    blit_flush();
  }
  blit_fbo_unbind();
}

void level_minimaps_update(Gamep g, Levelsp v, Levelp l)
{
  if (g_opt_tests) {
    return;
  }

  IF_NODEBUG { return; }

  TRACE();

  //
  // If in level select mode, avoid certain things, like vision effects
  //
  bool const level_select = level_is_level_select(g, v, game_level_get(g, v));

  if (solid_tex == nullptr) {
    solid_tex    = tex_load("", "solid", GL_LINEAR);
    solid_tex_id = tex_get_gl_binding(solid_tex);
  }

  level_minimap_levels_update(g, v, l, level_select);
  //  sdl_fbo_dump(g, FBO_MINIMAP_LEVEL, "level");
}
