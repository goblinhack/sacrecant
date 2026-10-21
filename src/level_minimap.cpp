//
// Changed by goblinhack@gmail.com
//

#include "my_bpoint.hpp"
#include "my_callstack.hpp"
#include "my_color.hpp"
#include "my_color_defs.hpp"
#include "my_game_defs.hpp"
#include "my_game_inlines.hpp"
#include "my_gl.hpp"
#include "my_globals.hpp"
#include "my_level.hpp"
#include "my_level_inlines.hpp"
#include "my_sdl_proto.hpp" // NOLINT
#include "my_tex.hpp"
#include "my_thing.hpp"
#include "my_types.hpp"

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

        if (level_select) {
          if (! l->is_treasure_level) {
            continue;
          }
        } else {
          if (! g_opt_debug1) {
            if (! level_has_seen_cached(g, v, l, p)) {
              continue;
            }
          }
        }

        if (level_is_dirt_bool(g, v, l, p)) {
          c = GRAY10;
        }
        if (level_is_floor_bool(g, v, l, p)) {
          c = GRAY30;
        }
        if (level_is_corridor_bool(g, v, l, p)) {
          c = GRAY30;
        }
        if (level_is_wall_bool(g, v, l, p)) {
          c = GRAY90;
        }
        if (level_is_rock_bool(g, v, l, p)) {
          c = BROWN4;
        }
        if (level_is_bridge_bool(g, v, l, p)) {
          c = BROWN;
        }
        if (level_is_chasm_bool(g, v, l, p)) {
          c = BLACK;
        }
        if (level_is_water_shallow_bool(g, v, l, p)) {
          c = BLUE;
        }
        if (level_is_water_deep_bool(g, v, l, p)) {
          c = BLUE4;
        }
        if (level_is_door_locked_bool(g, v, l, p)) {
          if (level_open_is_door_locked(g, v, l, p) != nullptr) {
            // ignore
          } else {
            c = BROWN;
          }
        }
        if (level_is_door_secret_bool(g, v, l, p)) {
          if (level_open_is_door_secret(g, v, l, p) != nullptr) {
            // ignore
          } else {
            c = GRAY40;
          }
        }
        if (level_is_lava_bool(g, v, l, p)) {
          c = ORANGE;
        }
        if (level_is_teleport_bool(g, v, l, p)) {
          // ignore
          c = RED;
        }
        if (level_is_entrance_bool(g, v, l, p)) {
          c = PINK;
        }
        if (level_is_exit_bool(g, v, l, p)) {
          c = YELLOW;
        }
        if (level_is_fire_bool(g, v, l, p)) {
          c = ORANGE2;
        }
        if (level_is_player_bool(g, v, l, p)) {
          c = CYAN;
        }
        if (level_is_monst_bool(g, v, l, p)) {
          c = RED;
        }
        if (level_is_proj_bool(g, v, l, p)) {
          c = ORANGE;
        }
        if (level_is_beam_weapon_bool(g, v, l, p)) {
          c = ORANGE;
        }

        if (level_is_blit_shown_in_overlay_bool(g, v, l, p)) {
          //
          // Keep bright colors
          //
        } else if (level_select) {
          //
          // No vision in level selection
          //
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

  if (g_thread_id != MAIN_THREAD) {
    return;
  }

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

  if (compiler_unused) {
    sdl_fbo_dump(g, FBO_MINIMAP_LEVEL, "level");
  }
}
