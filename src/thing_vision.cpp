//
// Copyright goblinhack@gmail.com
//

#include "my_age_map_inlines.hpp"
#include "my_bpoint.hpp"
#include "my_callstack.hpp"
#include "my_fov_map.hpp"
#include "my_fov_map_inlines.hpp"
#include "my_game_defs.hpp"
#include "my_game_inlines.hpp"
#include "my_globals.hpp"
#include "my_level.hpp"
#include "my_level_inlines.hpp"
#include "my_main.hpp"
#include "my_thing.hpp"
#include "my_thing_inlines.hpp"
#include "my_tp.hpp"
#include "my_types.hpp"

#include <cstdint>
#include <string>

//
// Something blocking the fov?
//
[[nodiscard]] auto thing_vision_blocker(Gamep g, Levelsp v, Levelp l, Thingp me, Thingp it) -> bool
{
  //
  // Some monsters can see through walls
  //
  if (thing_is_able_to_see_through_walls(me)) {
    if (thing_is_wall(it)) {
      return false;
    }
  }

  //
  // Monsters can see through secret doors
  //
  if (thing_is_monst(me)) {
    if (thing_is_door_secret(it)) {
      return false;
    }
  }

  //
  // Dead foliage should not block
  //
  if (thing_is_dead(it)) {
    return false;
  }

  //
  // Open doors should not block
  //
  if (thing_is_open(it)) {
    return false;
  }

  //
  // See invisible?
  //
  if (thing_is_invisible(g, v, l, it)) {
    if (! thing_is_able_to_see_invisible(me)) {
      return false;
    }
  }

  if (thing_is_obs_to_vision(it)) {
    //
    // Things that are submerged should not block you, if you are less submerged
    //
    auto sub_it = thing_submerged_pct(it);
    auto sub_me = thing_submerged_pct(me);

    if ((sub_it != 0) && (sub_me != 0)) {
      if (sub_it >= sub_me) {
        return false;
      }
    }
    return true;
  }

  return false;
}

void thing_vision_reset(Gamep g, Levelsp v, Levelp l, Thingp t)
{
  TRACE();

  auto *ext = thing_ext_struct(g, v, t);
  if (ext != nullptr) {
    ext->has_seen = {{{0}}};
    ext->can_see  = {{0}};
  }

  auto *light = thing_light_struct(g, t);
  if (light != nullptr) {
    light->is_lit = {{0}};
  }
}

[[nodiscard]] auto thing_vision_can_see_tile(Gamep g, Levelsp v, Levelp l, Thingp t, bpoint p) -> bool
{
  TRACE_DEBUG();

  auto *ext = thing_ext_struct(g, v, t);
  if (ext == nullptr) [[unlikely]] {
    return false;
  }

  if (is_oob(p)) [[unlikely]] {
    return false;
  }

  //
  // Check the thing is on the same level.
  //
  auto *t_level = game_level_get(g, v, t->level_num);
  if ((t_level != nullptr) && (t_level != l)) {
    return false;
  }

  return static_cast< uint32_t >(fov_map_get(&ext->can_see, p.x, p.y)) != 0U;
}

void thing_can_see_dump(Gamep g, Levelsp v, Levelp l, Thingp t)
{
  auto *ext = thing_ext_struct(g, v, t);
  if (ext == nullptr) [[unlikely]] {
    return;
  }

  THING_DBG(g, v, l, t, "can see:");
  TRACE_INDENT();

  for (auto y = 0; y < MAP_HEIGHT; y++) {
    std::string debug;
    for (auto x = 0; x < MAP_WIDTH; x++) {
      bpoint const p(x, y);

      if (p == thing_monst_target(t)) {
        debug += "G";
        continue;
      }

      if (thing_is_player(t)) {
        debug += "@";
        continue;
      }

      if (p == thing_at(g, v, l, t)) {
        debug += "m";
        continue;
      }

      if (level_is_wall_bool(g, v, l, p)) {
        debug += "#";
        continue;
      }

      if (static_cast< uint32_t >(fov_map_get(&ext->can_see, p.x, p.y)) != 0U) {
        debug += "*";
        continue;
      }

      if (static_cast< uint32_t >(age_map_get(&ext->has_seen, p.x, p.y)) != 0U) {
        debug += "o";
        continue;
      }

      debug += ".";
    }

    THING_DBG(g, v, l, t, "can see: %s", debug.c_str());
  }
  THING_DBG(g, v, l, t, "-");
}

void thing_has_seen_dump(Gamep g, Levelsp v, Levelp l, Thingp t)
{
  auto *ext = thing_ext_struct(g, v, t);
  if (ext == nullptr) [[unlikely]] {
    return;
  }

  THING_DBG(g, v, l, t, "has seen:");
  TRACE_INDENT();

  for (auto y = 0; y < MAP_HEIGHT; y++) {
    std::string debug;
    for (auto x = 0; x < MAP_WIDTH; x++) {
      bpoint const p(x, y);

      if (p == thing_at(g, v, l, t)) {
        debug += "@";
        continue;
      }

      if (level_is_wall_bool(g, v, l, p)) {
        debug += "#";
        continue;
      }

      if (static_cast< uint32_t >(age_map_get(&ext->has_seen, p.x, p.y)) != 0U) {
        debug += "*";
        continue;
      }

      debug += ".";
    }

    THING_DBG(g, v, l, t, "has seen: %s", debug.c_str());
  }
}

void thing_vision_calculate(Gamep g, Levelsp v, Levelp l, Thingp me)
{
  TRACE();

  auto max_radius = thing_distance_vision(g, v, l, me);
  if (max_radius == 0) {
    return;
  }

  auto *ext = thing_ext_struct(g, v, me);
  if (ext == nullptr) [[unlikely]] {
    return;
  }

  //
  // Per thread stdout name
  //
  IF_DEBUG
  {
    g_thread_id = VISION_THREAD;
    (void) redirect_stdout();
    (void) redirect_stderr();
  }

  FovContext ctx;

  ctx.g                  = g;
  ctx.v                  = v;
  ctx.l                  = l;
  ctx.me                 = me;
  ctx.pov                = thing_at(g, v, l, me);
  ctx.thing_at_in_pixels = thing_pix_at(me);
  ctx.max_radius         = max_radius;
  ctx.can_see_tile       = &ext->can_see;
  ctx.has_seen_tile      = &ext->has_seen;
  ctx.light_walls        = false; // If this is set, we will be able to see through foliage at the edge of vision

  level_fov(ctx);

  IF_DEBUG
  {
    if (compiler_unused) {
      THING_DBG(g, v, l, me, "dir %s", ThingDirType_to_string(me->dir).c_str());
      thing_can_see_dump(g, v, l, me);
    }
  }

  IF_DEBUG
  {
    close_stdout();
    close_stderr();
  }
}

[[nodiscard]] auto thing_distance_vision(Gamep g, Levelsp v, Levelp l, Thingp t) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }

  auto d = t->_distance_vision;
  if (d == 0) {
    return d;
  }

  //
  // Reduce vision in bogland
  //
  if (thing_is_player(t)) {
    switch (level_to_biome(g, v, l)) {
      case BIOME_DUNGEON :    break;
      case BIOME_BOGLAND :    d /= 4; break;
      case BIOME_NETHERVOID : break;
      case BIOME_GRAVEYARD :  break;
      case BIOME_UNDERHELL :  d /= 2; break;
      case BIOME_NONE :       [[fallthrough]];
      case BIOME_ENUM_MAX :   break;
    }
  }

  if (thing_is_myopic(g, v, l, t)) {
    d /= 8;
  }

  if (d == 0) {
    d = 1;
  }

  return d;
}

[[nodiscard]] auto thing_distance_vision_set(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_distance_vision = val;
}

[[nodiscard]] auto thing_distance_vision_incr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_distance_vision += val;
}

[[nodiscard]] auto thing_distance_vision_decr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  if (static_cast< int >(t->_distance_vision) - val <= 0) {
    return t->_distance_vision = 0;
  }
  return t->_distance_vision -= val;
}

[[nodiscard]] auto thing_is_vision_360_degrees(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_vision_360_degrees) != 0;
}

[[nodiscard]] auto thing_is_vision_180_degrees(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_vision_180_degrees) != 0;
}
