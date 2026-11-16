//
// Changed by goblinhack@gmail.com
//

#include "my_bpoint.hpp"
#include "my_callstack.hpp"
#include "my_color.hpp"
#include "my_color_defs.hpp"
#include "my_fov_map_inlines.hpp"
#include "my_game_defs.hpp"
#include "my_game_inlines.hpp"
#include "my_gl.hpp"
#include "my_globals.hpp"
#include "my_level.hpp"
#include "my_level_inlines.hpp"
#include "my_main.hpp"
#include "my_sdl_proto.hpp" // NOLINT
#include "my_tex.hpp"
#include "my_thing.hpp"
#include "my_thing_inlines.hpp"
#include "my_tp.hpp"
#include "my_types.hpp"
#include <cstdint>

static Texp solid_tex;
static int  solid_tex_id;

//
// What is on a tile, as far as the minimap is concerned.
//
enum : uint32_t {
  MINIMAP_EMPTY         = 1U << 0,
  MINIMAP_BORDER        = 1U << 1,
  MINIMAP_DIRT          = 1U << 2,
  MINIMAP_FLOOR         = 1U << 3,
  MINIMAP_CORRIDOR      = 1U << 4,
  MINIMAP_WALL          = 1U << 5,
  MINIMAP_BRIDGE        = 1U << 6,
  MINIMAP_CHASM         = 1U << 7,
  MINIMAP_WATER_SHALLOW = 1U << 8,
  MINIMAP_WATER_DEEP    = 1U << 9,
  MINIMAP_DOOR_LOCKED   = 1U << 10,
  MINIMAP_DOOR_SECRET   = 1U << 11,
  MINIMAP_LAVA          = 1U << 12,
  MINIMAP_TELEPORT      = 1U << 13,
  MINIMAP_ENTRANCE      = 1U << 14,
  MINIMAP_EXIT          = 1U << 15,
  MINIMAP_FIRE          = 1U << 16,
  MINIMAP_PLAYER        = 1U << 17,
  MINIMAP_MONST         = 1U << 18,
  MINIMAP_MOB           = 1U << 19,
  MINIMAP_PROJECTILE    = 1U << 20,
  MINIMAP_BEAM_WEAPON   = 1U << 21,
  MINIMAP_OVERLAY       = 1U << 22,
};

//
// Work out everything we want to know about a tile in a single pass over the things on it. This is what
// level_is_empty_bool() and level_is_*_bool() each work out, but they each go over all the things on the tile, which
// is a lot of passes for every tile of the level.
//
[[nodiscard]] static auto level_minimap_tile_flags(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t
{
  uint32_t mask         = 0;
  bool     non_internal = false;

  FOR_ALL_THINGS_AT_UNSAFE(g, v, l, it, p)
  {
    auto *tp = thing_tp(it);

    if (! thing_is_internal(it)) {
      non_internal = true;
    }

    if (tp_flag(tp, is_border) != 0) {
      mask |= MINIMAP_BORDER;
    }
    if (tp_flag(tp, is_dirt) != 0) {
      mask |= MINIMAP_DIRT;
    }
    if (tp_flag(tp, is_floor) != 0) {
      mask |= MINIMAP_FLOOR;
    }
    if (tp_flag(tp, is_corridor) != 0) {
      mask |= MINIMAP_CORRIDOR;
    }
    if (tp_flag(tp, is_wall) != 0) {
      mask |= MINIMAP_WALL;
    }
    if (tp_flag(tp, is_bridge) != 0) {
      mask |= MINIMAP_BRIDGE;
    }
    if (tp_flag(tp, is_chasm) != 0) {
      mask |= MINIMAP_CHASM;
    }
    if (tp_flag(tp, is_water_shallow) != 0) {
      mask |= MINIMAP_WATER_SHALLOW;
    }
    if (tp_flag(tp, is_water_deep) != 0) {
      mask |= MINIMAP_WATER_DEEP;
    }
    if (tp_flag(tp, is_door_locked) != 0) {
      mask |= MINIMAP_DOOR_LOCKED;
    }
    if (tp_flag(tp, is_door_secret) != 0) {
      mask |= MINIMAP_DOOR_SECRET;
    }
    if (tp_flag(tp, is_lava) != 0) {
      mask |= MINIMAP_LAVA;
    }
    if (tp_flag(tp, is_teleport) != 0) {
      mask |= MINIMAP_TELEPORT;
    }
    if (tp_flag(tp, is_entrance) != 0) {
      mask |= MINIMAP_ENTRANCE;
    }
    if (tp_flag(tp, is_exit) != 0) {
      mask |= MINIMAP_EXIT;
    }
    if (tp_flag(tp, is_fire) != 0) {
      mask |= MINIMAP_FIRE;
    }
    if (tp_flag(tp, is_player) != 0) {
      mask |= MINIMAP_PLAYER;
    }
    if (tp_flag(tp, is_monst) != 0) {
      mask |= MINIMAP_MONST;
    }
    if (tp_flag(tp, is_mob) != 0) {
      mask |= MINIMAP_MOB;
    }
    if (tp_flag(tp, is_projectile) != 0) {
      mask |= MINIMAP_PROJECTILE;
    }
    if (tp_flag(tp, is_beam_weapon) != 0) {
      mask |= MINIMAP_BEAM_WEAPON;
    }
    if (tp_flag(tp, is_blit_shown_in_overlay) != 0) {
      mask |= MINIMAP_OVERLAY;
    }
  }

  if (! non_internal) {
    mask |= MINIMAP_EMPTY;
  }

  return mask;
}

//
// What color to show for a tile. Returns false if there is nothing to show. Later rules take priority.
//
[[nodiscard]] static auto level_minimap_tile_color(Gamep g, Levelsp v, Levelp l, const bpoint &p, const uint32_t mask, color &c) -> bool
{
  bool c_set {};

  if ((mask & MINIMAP_EMPTY) != 0U) {
    c     = BLUE4;
    c_set = true;
  }
  if ((mask & MINIMAP_BORDER) != 0U) {
    c     = GRAY10;
    c_set = true;
  }
  if ((mask & MINIMAP_DIRT) != 0U) {
    c     = GRAY20;
    c_set = true;
  }
  if ((mask & MINIMAP_FLOOR) != 0U) {
    c     = GRAY20;
    c_set = true;
  }
  if ((mask & MINIMAP_CORRIDOR) != 0U) {
    c     = GRAY20;
    c_set = true;
  }
  if ((mask & MINIMAP_WALL) != 0U) {
    c     = GRAY60;
    c_set = true;
  }
  if ((mask & MINIMAP_BRIDGE) != 0U) {
    c     = BROWN;
    c_set = true;
  }
  if ((mask & MINIMAP_CHASM) != 0U) {
    c     = BLACK;
    c_set = true;
  }
  if ((mask & MINIMAP_WATER_SHALLOW) != 0U) {
    c     = BLUE;
    c_set = true;
  }
  if ((mask & MINIMAP_WATER_DEEP) != 0U) {
    c     = BLUE2;
    c_set = true;
  }
  if ((mask & MINIMAP_DOOR_LOCKED) != 0U) {
    if (level_open_is_door_locked(g, v, l, p) != nullptr) {
      // ignore
    } else {
      c     = GRAY50;
      c_set = true;
    }
  }
  if ((mask & MINIMAP_DOOR_SECRET) != 0U) {
    if (level_open_is_door_secret(g, v, l, p) != nullptr) {
      // ignore
    } else {
      c     = GRAY50;
      c_set = true;
    }
  }
  if ((mask & MINIMAP_LAVA) != 0U) {
    c     = ORANGE;
    c_set = true;
  }
  if ((mask & MINIMAP_TELEPORT) != 0U) {
    c     = PURPLE;
    c_set = true;
  }
  if ((mask & MINIMAP_ENTRANCE) != 0U) {
    c     = WHITE;
    c_set = true;
  }
  if ((mask & MINIMAP_EXIT) != 0U) {
    c     = YELLOW;
    c_set = true;
  }
  if ((mask & MINIMAP_FIRE) != 0U) {
    c     = ORANGE2;
    c_set = true;
  }
  if ((mask & MINIMAP_PLAYER) != 0U) {
    c     = PINK;
    c_set = true;
  }
  if ((mask & MINIMAP_MONST) != 0U) {
    c     = RED;
    c_set = true;
  }
  if ((mask & MINIMAP_MOB) != 0U) {
    c     = RED;
    c_set = true;
  }
  if ((mask & MINIMAP_PROJECTILE) != 0U) {
    c     = ORANGE;
    c_set = true;
  }
  if ((mask & MINIMAP_BEAM_WEAPON) != 0U) {
    c     = ORANGE;
    c_set = true;
  }

  return c_set;
}

//
// thing_vision_can_see_tile() for the same thing and level over and over. Everything but the final lookup is the same
// for every tile, so only do that once.
//
class MinimapVision
{
public:
  MinimapVision(Gamep g, Levelsp v, Levelp l, Thingp player) : ext(thing_ext_struct(g, v, player))
  {

    if (ext == nullptr) [[unlikely]] {
      return;
    }

    //
    // Check the thing is on the same level.
    //
    auto *t_level = game_level_get(g, v, player->level_arr_idx);
    if ((t_level != nullptr) && (t_level != l)) {
      ext = nullptr;
    }
  }

  [[nodiscard]] auto can_see(int x, int y) const -> bool
  {
    if (ext == nullptr) [[unlikely]] {
      return false;
    }

    return static_cast< uint32_t >(fov_map_get(&ext->can_see, static_cast< uint8_t >(x), static_cast< uint8_t >(y))) != 0U;
  }

private:
  ThingExtp ext {};
};

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

  //
  // Only show what has been seen, unless in level select, or debugging.
  //
  const bool only_seen = ! level_select && ! g_opt_debug1;

  const MinimapVision vision(g, v, l, player);

  blit_fbo_bind(fbo);
  {
    glBlendFunc(GL_ONE, GL_ZERO);
    gl_clear();

    blit_init();

    for (auto y = 0; y < MAP_HEIGHT; y++) {
      for (auto x = 0; x < MAP_WIDTH; x++) {
        bpoint const p(x, y);

        if (only_seen) {
          if (! level_has_seen_cached(g, v, l, p)) {
            continue;
          }
        }

        auto const mask = level_minimap_tile_flags(g, v, l, p);

        color c = BLACK;
        if (! level_minimap_tile_color(g, v, l, p, mask, c)) {
          continue;
        }

        if ((mask & MINIMAP_OVERLAY) != 0U) {
          //
          // Keep bright colors
          //
        } else if (level_select) {
          //
          // No vision in level selection
          //
        } else if (! vision.can_see(x, y)) {
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
