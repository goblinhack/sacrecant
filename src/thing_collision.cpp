//
// Copyright goblinhack@gmail.com
//

#include "my_bpoint.hpp"
#include "my_callstack.hpp"
#include "my_fpoint.hpp"
#include "my_game_defs.hpp"
#include "my_level.hpp"
#include "my_level_inlines.hpp" // NOLINT
#include "my_main.hpp"          // NOLINT
#include "my_thing.hpp"
#include "my_thing_inlines.hpp" // NOLINT
#include "my_tp.hpp"
#include "my_types.hpp"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <unordered_set>
#include <utility>
#include <vector>

using ThingCand  = std::pair< float, Thingp >;
using ThingCands = std::vector< ThingCand >;

//
// Sort the candidates by distance / potentially add more cands if we can hit all
// things on the same tile
//
static void thing_collision_sort_cands(Gamep g, Levelsp v, Levelp l, Thingp me, ThingCands &cands)
{
  TRACE();

  if (compiler_unused) {
    THING_DBG(g, v, l, me, "final cands: (pre sort)");
    for (auto a_cand : cands) {
      auto  o_dist   = a_cand.first;
      auto *obstacle = a_cand.second;

      THING_DBG(g, v, l, obstacle, "- sort_distance %f prio %u", o_dist, thing_priority(obstacle));
    }
  }

  //
  // Sort by distance and priority
  //
  std::ranges::sort(cands, [](const ThingCand &a, const ThingCand &b) -> bool {
    auto  d1 = a.first;
    auto  d2 = b.first;
    auto *t1 = a.second;
    auto *t2 = b.second;

    if (d1 == d2) {
      return thing_priority(t1) < thing_priority(t2);
    }
    return d1 < d2;
  });

  //
  // Dump the final cands
  //
  if (compiler_unused) {
    THING_DBG(g, v, l, me, "final cands:");
    for (auto a_cand : cands) {
      auto  o_dist   = a_cand.first;
      auto *obstacle = a_cand.second;

      THING_DBG(g, v, l, obstacle, "- sort_distance %f prio %u", o_dist, thing_priority(obstacle));
    }
  }
}

[[nodiscard]] static auto thing_collision_check_circle_circle(Gamep g, Levelsp v, Levelp l, Thingp A, fpoint A_at, Thingp B, fpoint B_at) -> bool
{
  float const A_radius = thing_collision_radius(A);
  float const B_radius = thing_collision_radius(B);

  float const touching_dist = A_radius + B_radius;
  float const dist          = distance(A_at, B_at);

  if (compiler_unused) {
    THING_DBG(g, v, l, A, "A %f,%f touching_dist %f dist %f ", A_at.x, A_at.y, touching_dist, dist);
    THING_DBG(g, v, l, B, "B %f,%f", B_at.x, B_at.y);
  }

  if (dist >= touching_dist) {
    return false;
  }

  return true;
}

[[nodiscard]] static auto thing_collision_check_circle_square(Gamep g, Levelsp v, Levelp l, Thingp C, fpoint C_at, Thingp B, fpoint B_at) -> bool
{
  float const radius = thing_collision_radius(C);

  //
  // Need to perform calculations from the center of the circle as the
  // circle is really inside a tile with the top left coordinates being the
  // blit point for the tile. The radius then is used from this cental
  // point for collision.s
  //
  C_at.x += 0.5F;
  C_at.y += 0.5F;

  fpoint const tl(B_at.x, B_at.y);
  fpoint const br(B_at.x + 1, B_at.y + 1);

  if (compiler_unused) {
    THING_DBG(g, v, l, C, "circle %f,%f", C_at.x, C_at.y);
    THING_DBG(g, v, l, B, "box %f,%f -> %f,%f", tl.x, tl.y, br.x, br.y);
  }

  fpoint const B0(B_at.x - 0, B_at.y - 0);
  fpoint const B1(B_at.x + 1, B_at.y - 0);
  fpoint const B2(B_at.x + 1, B_at.y + 1);
  fpoint const B3(B_at.x + 0, B_at.y + 1);

  //
  // Circle inside box
  //
  if ((C_at.x >= B0.x) && (C_at.y >= B0.y) && (C_at.x <= B2.x) && (C_at.y <= B2.y)) {
    if (compiler_unused) {
      THING_DBG(g, v, l, B, "collision line %d", __LINE__);
    }
    return true;
  }

  //
  // Corner collisions.
  //
  if (distance(C_at, B0) < radius) {
    if (compiler_unused) {
      THING_DBG(g, v, l, B, "collision line %d", __LINE__);
    }
    return true;
  }

  if (distance(C_at, B1) < radius) {
    if (compiler_unused) {
      THING_DBG(g, v, l, B, "collision line %d", __LINE__);
    }
    return true;
  }

  if (distance(C_at, B2) < radius) {
    if (compiler_unused) {
      THING_DBG(g, v, l, B, "collision line %d", __LINE__);
    }
    return true;
  }

  if (distance(C_at, B3) < radius) {
    if (compiler_unused) {
      THING_DBG(g, v, l, B, "collision line %d", __LINE__);
    }
    return true;
  }

  fpoint *intersect_out = nullptr;
  float   dist          = 0;

  if (distance_to_line(C_at, B0, B1, &dist, intersect_out) != 0) {
    if (dist < radius) {
      if (compiler_unused) {
        THING_DBG(g, v, l, B, "collision line %d", __LINE__);
      }
      return true;
    }
  }

  if (distance_to_line(C_at, B1, B2, &dist, intersect_out) != 0) {
    if (dist < radius) {
      if (compiler_unused) {
        THING_DBG(g, v, l, B, "collision line %d", __LINE__);
      }
      return true;
    }
  }

  if (distance_to_line(C_at, B2, B3, &dist, intersect_out) != 0) {
    if (dist < radius) {
      if (compiler_unused) {
        THING_DBG(g, v, l, B, "collision line %d", __LINE__);
      }
      return true;
    }
  }

  if (distance_to_line(C_at, B3, B0, &dist, intersect_out) != 0) {
    if (dist < radius) {
      if (compiler_unused) {
        THING_DBG(g, v, l, B, "collision line %d", __LINE__);
      }
      return true;
    }
  }

  if (compiler_unused) {
    THING_DBG(g, v, l, B, "no collision line %d", __LINE__);
  }

  return false;
}

[[nodiscard]] static auto thing_collision_check_squares(fpoint A_at, fpoint B_at) -> bool
{
  fpoint const A0(A_at.x - 0, A_at.y - 0);
  fpoint const A1(A_at.x + 1, A_at.y - 0);
  fpoint const A2(A_at.x - 0, A_at.y + 1);
  fpoint const A3(A_at.x + 1, A_at.y + 1);

  fpoint const tl(B_at.x - 0, B_at.y - 0);
  fpoint const br(B_at.x + 1, B_at.y + 1);

  if ((A0.x >= tl.x) && (A0.x <= br.x) && (A0.y >= tl.y) && (A0.y <= br.y)) {
    return true;
  }
  if ((A1.x >= tl.x) && (A1.x <= br.x) && (A1.y >= tl.y) && (A1.y <= br.y)) {
    return true;
  }
  if ((A2.x >= tl.x) && (A2.x <= br.x) && (A2.y >= tl.y) && (A2.y <= br.y)) {
    return true;
  }
  if ((A3.x >= tl.x) && (A3.x <= br.x) && (A3.y >= tl.y) && (A3.y <= br.y)) {
    return true;
  }

  return false;
}

[[nodiscard]] static auto thing_collision_check_circle_small_circle_small(Gamep g, Levelsp v, Levelp l, Thingp me, fpoint me_at, Thingp obstacle,
                                                                          fpoint o_at) -> bool
{
  TRACE();
  return thing_collision_check_circle_circle(g, v, l, me, me_at, obstacle, o_at);
}

[[nodiscard]] static auto thing_collision_check_circle_small_circle_large(Gamep g, Levelsp v, Levelp l, Thingp me, fpoint me_at, Thingp obstacle,
                                                                          fpoint o_at) -> bool
{
  TRACE();
  return thing_collision_check_circle_circle(g, v, l, me, me_at, obstacle, o_at);
}

[[nodiscard]] static auto thing_collision_check_circle_small_square(Gamep g, Levelsp v, Levelp l, Thingp me, fpoint me_at, Thingp obstacle,
                                                                    fpoint o_at) -> bool
{
  TRACE();
  return thing_collision_check_circle_square(g, v, l, me, me_at, obstacle, o_at);
}

[[nodiscard]] static auto thing_collision_check_circle_large_circle_large(Gamep g, Levelsp v, Levelp l, Thingp me, fpoint me_at, Thingp obstacle,
                                                                          fpoint o_at) -> bool
{
  TRACE();
  return thing_collision_check_circle_circle(g, v, l, me, me_at, obstacle, o_at);
}

[[nodiscard]] static auto thing_collision_check_circle_large_square(Gamep g, Levelsp v, Levelp l, Thingp me, fpoint me_at, Thingp obstacle,
                                                                    fpoint o_at) -> bool
{
  TRACE();
  return thing_collision_check_circle_square(g, v, l, me, me_at, obstacle, o_at);
}

[[nodiscard]] static auto thing_collision_check_square_square(fpoint me_at, fpoint o_at) -> bool
{
  TRACE();
  return thing_collision_check_squares(me_at, o_at);
}

//
// Do accurate hit box collision detection for this interpolated position
//
static auto thing_collision_check(Gamep g, Levelsp v, Levelp l, Thingp me, const fpoint &interp_at_f, Thingp obstacle) -> bool
{
  TRACE();

  auto o_at      = thing_real_at(g, v, l, obstacle);
  auto collision = false;

  if (thing_is_collision_circle_small(me)) {
    if (thing_is_collision_circle_small(obstacle)) {
      collision = thing_collision_check_circle_small_circle_small(g, v, l, me, interp_at_f, obstacle, o_at);
      if (compiler_unused) {
        THING_DBG(g, v, l, obstacle, "cand coll %d collision=%d", __LINE__, static_cast< int >(collision));
      }
      return collision;
    }
    if (thing_is_collision_circle_large(obstacle)) {
      collision = thing_collision_check_circle_small_circle_large(g, v, l, me, interp_at_f, obstacle, o_at);
      if (compiler_unused) {
        THING_DBG(g, v, l, obstacle, "cand coll %d collision=%d", __LINE__, static_cast< int >(collision));
      }
      return collision;
    }
    if (thing_is_collision_square(obstacle)) {
      collision = thing_collision_check_circle_small_square(g, v, l, me, interp_at_f, obstacle, o_at);
      if (compiler_unused) {
        THING_DBG(g, v, l, obstacle, "cand coll %d collision=%d", __LINE__, static_cast< int >(collision));
      }
      return collision;
    }
    return false;
  }
  if (thing_is_collision_circle_large(me)) {
    if (thing_is_collision_circle_small(obstacle)) {
      collision = thing_collision_check_circle_small_circle_large(g, v, l, me, interp_at_f, obstacle, o_at);
      if (compiler_unused) {
        THING_DBG(g, v, l, obstacle, "cand coll %d collision=%d", __LINE__, static_cast< int >(collision));
      }
      return collision;
    }
    if (thing_is_collision_circle_large(obstacle)) {
      collision = thing_collision_check_circle_large_circle_large(g, v, l, me, interp_at_f, obstacle, o_at);
      if (compiler_unused) {
        THING_DBG(g, v, l, obstacle, "cand coll %d collision=%d", __LINE__, static_cast< int >(collision));
      }
      return collision;
    }
    if (thing_is_collision_square(obstacle)) {
      collision = thing_collision_check_circle_large_square(g, v, l, me, interp_at_f, obstacle, o_at);
      if (compiler_unused) {
        THING_DBG(g, v, l, obstacle, "cand coll %d collision=%d", __LINE__, static_cast< int >(collision));
      }
      return collision;
    }
    return false;
  }
  if (thing_is_collision_square(me)) {
    if (thing_is_collision_circle_small(obstacle)) {
      collision = thing_collision_check_circle_small_square(g, v, l, obstacle, o_at, me, interp_at_f);
      if (compiler_unused) {
        THING_DBG(g, v, l, obstacle, "cand coll %d collision=%d", __LINE__, static_cast< int >(collision));
      }
      return collision;
    }
    if (thing_is_collision_circle_large(obstacle)) {
      collision = thing_collision_check_circle_large_square(g, v, l, obstacle, o_at, me, interp_at_f);
      if (compiler_unused) {
        THING_DBG(g, v, l, obstacle, "cand coll %d collision=%d", __LINE__, static_cast< int >(collision));
      }
      return collision;
    }
    if (thing_is_collision_square(obstacle)) {
      collision = thing_collision_check_square_square(o_at, interp_at_f);
      if (compiler_unused) {
        THING_DBG(g, v, l, obstacle, "cand coll %d collision=%d", __LINE__, static_cast< int >(collision));
      }
      return collision;
    }
    return false;
  }
  return false;
}

//
// Handle common interactions for a thing at its location with a thing
//
static void thing_collision_handle_common(Gamep g, Levelsp v, Levelp l, Thingp obstacle, Thingp me, bool &stop)
{
  TRACE();

  //
  // Handle walking into a teleport
  //
  if (thing_is_teleport(obstacle)) {
    if (thing_is_able_to_be_teleported(me)) {
      THING_DBG(g, v, l, me, "over teleport");
      TRACE_INDENT();
      if (thing_warp_to(g, v, l, me, thing_at(g, v, l, obstacle))) {
        (void) thing_teleport_handle(g, v, l, me);
        stop = true;
        return;
      }
    }
  }

  //
  // Handle walking into a spiderweb
  //
  if (thing_is_able_to_ensnare(obstacle)) {
    THING_DBG(g, v, l, me, "ensnare check");
    TRACE_INDENT();
    if (thing_ensnare(g, v, l, me, obstacle)) {
      stop = true;
      return;
    }
  }

  //
  // Handle walking onto a chasm
  //
  if (thing_is_chasm(obstacle)) {
    if (thing_is_able_to_fall(g, v, l, me)) {
      THING_DBG(g, v, l, me, "over chasm");
      TRACE_INDENT();
      thing_chasm_handle(g, v, l, me);
      stop = true;
      return;
    }
  }
}

//
// Handle interactions for a thing at its location with a dead thing
//
static void thing_collision_handle_dead_thing(Gamep g, Levelsp v, Levelp l, Thingp obstacle, Thingp me, bool &stop)
{
  if (compiler_unused) {
    THING_DBG(g, v, l, obstacle, "%s", __FUNCTION__);
  }

  thing_collision_handle_common(g, v, l, obstacle, me, stop);

  //
  // Wait until complete
  //
  if (thing_is_thrown(obstacle) || (thing_is_falling(obstacle) != 0)) {
    return;
  }

  if (! thing_is_hit_when_dead(obstacle)) {
    return;
  }

  if (thing_is_projectile(me) || thing_is_beam_weapon(me)) {
    auto *source     = me;
    auto *source_tp  = thing_tp(source);
    auto  event_type = tp_damage_random_type_get(source_tp);
    auto  damage     = thing_damage(g, v, l, source, event_type);

    ThingEvent e {
        .reason     = "by weapon damage", //
        .event_type = event_type,         //
        .damage     = damage,             //
        .source     = source,             //
    };

    THING_DBG(g, v, l, me, "weapon collision with dead thing");
    THING_DBG(g, v, l, obstacle, "me");
    TRACE_INDENT();

    thing_damage_apply(g, v, l, obstacle, e);
  }

  if (thing_is_dead_on_collision(me)) {
    ThingEvent e {
        .reason     = "collided",                   //
        .event_type = THING_EVENT_LIFESPAN_EXPIRED, //
    };

    THING_DBG(g, v, l, me, "dead due to lifespan");
    TRACE_INDENT();

    thing_dead(g, v, l, me, e);
    stop = true;
  }
}

//
// Handle interactions for a thing at its location with an alive thing
//
static void thing_collision_handle_alive_thing(Gamep g, Levelsp v, Levelp l, Thingp obstacle, Thingp me, bool &stop)
{
  if (compiler_unused) {
    THING_DBG(g, v, l, me, "me: %s", __FUNCTION__);
    THING_DBG(g, v, l, obstacle, "obstacle: %s", __FUNCTION__);
  }

  thing_collision_handle_common(g, v, l, obstacle, me, stop);
  if (stop) {
    return;
  }

  //
  // Wait until complete
  //
  if (thing_is_thrown(obstacle) || (thing_is_falling(obstacle) != 0)) {
    return;
  }

  //
  // Collect keys?
  //
  if (thing_collect_key(g, v, l, me, obstacle)) {
    return;
  }

  //
  // Auto carry items? As long as not dropped during this tick
  //
  {
    ThingEvent e {
        .reason     = "auto collected",    //
        .event_type = THING_EVENT_CARRIED, //
        .source     = me,                  //
    };

    if (thing_carry(g, v, l, me, obstacle, e)) {
      return;
    }
  }

  //
  // Crush obstacle
  //
  if (thing_crush(g, v, l, obstacle, me)) {
    stop = true;
    return;
  }

  if (thing_is_projectile(me) || thing_is_beam_weapon(me)) {
    auto *source     = me;
    auto *source_tp  = thing_tp(source);
    auto  event_type = tp_damage_random_type_get(source_tp);
    auto  damage     = thing_damage(g, v, l, source, event_type);

    ThingEvent e {
        .reason     = "by weapon damage", //
        .event_type = event_type,         //
        .damage     = damage,             //
        .source     = source,             //
    };

    THING_DBG(g, v, l, me, "weapon collision with");
    THING_DBG(g, v, l, obstacle, "me");
    TRACE_INDENT();

    thing_damage_apply(g, v, l, obstacle, e);
  }

  if (thing_is_dead_on_collision(me)) {
    ThingEvent e {
        .reason     = "collided",                   //
        .event_type = THING_EVENT_LIFESPAN_EXPIRED, //
    };

    THING_DBG(g, v, l, me, "dead due to collision");
    TRACE_INDENT();

    thing_dead(g, v, l, me, e);
    stop = true;
  }
}

//
// Check we only collide once between objects per tick
//
[[nodiscard]] auto thing_collision_handle_done_already(Levelsp v, Thingp obstacle, Thingp me) -> bool
{
  static std::unordered_set< uint64_t > collided;

  //
  // Reset each new tick
  //
  static uint32_t collided_tick;
  if (v->tick != collided_tick) {
    collided.clear();
    collided_tick = v->tick;
  }

  uint64_t const p = (static_cast< uint64_t >(obstacle->id) << 32) | me->id;
  if (collided.contains(p)) {
    return true;
  }

  collided.insert(p);
  return false;
}

//
// Handle interactions for a thing at its location
//
// Return true to continue with more collisions, or false to stop.
// e.g. if we teleport, we want to stop
//
static void thing_collision_handle_do(Gamep g, Levelsp v, Levelp l, Thingp obstacle, Thingp me, bool &stop)
{
  if (compiler_unused) {
    THING_DBG(g, v, l, obstacle, "%s", __FUNCTION__);
  }

  if (obstacle == me) {
    return;
  }

  if (thing_collision_handle_done_already(v, obstacle, me)) {
    return;
  }

  if (! thing_is_collision_detection_enabled(obstacle)) {
    return;
  }

  if (thing_is_ethereal(g, v, l, me)) {
    return;
  }

  //
  // Projectiles do not hit grass
  //
  if (thing_is_flat(obstacle)) {
    if (thing_is_projectile(me) || thing_is_beam_weapon(me)) {
      THING_DBG(g, v, l, obstacle, "ignore as flat");
      return;
    }
  }

  auto *owner = thing_owner(g, v, l, me);
  if (owner != nullptr) {
    if (obstacle == owner) {
      THING_DBG(g, v, l, obstacle, "ignore for owner");
      return;
    }
  }

  //
  // Don't get hit by your own weapon
  //
  auto *fired_by = thing_missile_fired_by_get(g, v, l, me);
  if (obstacle == fired_by) {
    THING_DBG(g, v, l, obstacle, "ignore for firer");
    return;
  }

  //
  // No overlapping weapon blasts from the same player
  //
  if ((fired_by != nullptr) && (thing_missile_fired_by_get(g, v, l, obstacle) == fired_by)) {
    return;
  }

  if (thing_is_dead(obstacle)) {
    //
    // Dead things
    //
    thing_collision_handle_dead_thing(g, v, l, obstacle, me, stop);
    return;
  }

  //
  // Alive things
  //
  thing_collision_handle_alive_thing(g, v, l, obstacle, me, stop);
}

//
// Handle interactions for a thing at its location
//
void thing_collision_handle(Gamep g, Levelsp v, Levelp l, Thingp me)
{
  //
  // Common collision handling for player and anything else
  //
  auto at = thing_at(g, v, l, me);

  if (compiler_unused) {
    THING_DBG(g, v, l, me, "%s at (%d,%d)", __FUNCTION__, at.x, at.y);
  }

  //
  // Weapons handled separately.
  //
  if (thing_is_projectile(me)) {
    return;
  }

  //
  // Handle player specific actions first, like leaving levels
  //
  if (thing_is_player(me)) {
    player_collision_handle(g, v, l, me);
  }

  ThingCands cands;
  FOR_ALL_THINGS_AT(g, v, l, obstacle, at)
  {
    //
    // Filter to only things that can be hit
    //
    if (! thing_is_collision_detection_enabled(obstacle)) {
      continue;
    }

    ThingCand const p = std::make_pair(0 /* dist */, obstacle);
    cands.push_back(p);
  }

  thing_collision_sort_cands(g, v, l, me, cands);
  TRACE_INDENT();

  for (auto cand : cands) {
    auto *obstacle = cand.second;
    bool  stop     = {};

    if (compiler_unused) {
      THING_DBG(g, v, l, obstacle, "cand");
    }

    thing_collision_handle_do(g, v, l, obstacle, me, stop);
    if (stop) {
      return;
    }
  }
}

//
// Sort the candidates by distance / potentially add more cands if we can hit all
// things on the same tile
//
static void thing_collision_interpolated_expand_candidates(Gamep g, Levelsp v, Levelp l, Thingp me, const bpoint &collision_at,
                                                           ThingCands &cands)
{
  //
  // If this is a proj_fire hitting a wall, then we want to hit the ghost that is
  // also hiding inside the wall
  //
  if (cands.empty()) {
    return;
  }

  if (! thing_is_collision_hit_all_on_tile(me) && ! thing_is_collision_hit_first_on_tile(me)) {
    return;
  }

  THING_DBG(g, v, l, me, "expand candidates on (%d,%d)", collision_at.x, collision_at.y);
  TRACE_INDENT();

  auto at = thing_real_at(g, v, l, me);

  auto *owner    = thing_owner(g, v, l, me);
  auto *fired_by = thing_missile_fired_by_get(g, v, l, me);

  //
  // For all other things on the same tile as the collision
  //
  FOR_ALL_THINGS_AT(g, v, l, obstacle, collision_at)
  {
    //
    // Filter to only things that can be hit
    //
    if (! thing_is_collision_detection_enabled(obstacle)) {
      continue;
    }

    if (obstacle == me) {
      continue;
    }

    if (obstacle == owner) {
      continue;
    }

    //
    // Don't get hit by your own weapon
    //
    if (fired_by == me) {
      continue;
    }

    //
    // No overlapping weapon blasts from the same player
    //
    if ((fired_by != nullptr) && (thing_missile_fired_by_get(g, v, l, obstacle) == fired_by)) {
      return;
    }

    //
    // Check this thing is not on the cand list already
    //
    bool already_cand = false;
    for (auto a_cand : cands) {
      if (a_cand.second == obstacle) {
        already_cand = true;
      }
      break;
    }

    if (! already_cand) {
      //
      // Sort by center of the tile distance. This allows walls and ghost in walls to have
      // the same distance
      //
      auto            o_tiled_at = make_fpoint(thing_at(g, v, l, obstacle)) + fpoint(0.5, 0.5);
      float const     o_dist     = distance(at, o_tiled_at);
      ThingCand const p          = std::make_pair(o_dist, obstacle);
      cands.push_back(p);

      THING_DBG(g, v, l, obstacle, "add candidate as on same tile");
    }
  }
}

//
// Process the collision candidate list
//
static auto thing_collision_interplolated_process_candidates(Gamep g, Levelsp v, Levelp l, Thingp me, const fpoint &interp_at_f,
                                                             const ThingCands &cands) -> bool
{
  TRACE();

  bool hit_something = {};

  for (auto cand : cands) {
    auto *obstacle = cand.second;

    //
    // Skip things that are dead; unless we can hit their corpse
    //
    if (thing_is_dead(obstacle)) {
      if (! thing_is_hit_when_dead(obstacle)) {
        continue;
      }
    }

    auto collision = thing_collision_check(g, v, l, me, interp_at_f, obstacle);
    if (! collision) {
      continue;
    }

    //
    // Handle the actual collision
    //
    bool stop = {};
    thing_collision_handle_do(g, v, l, obstacle, me, stop);
    if (stop) {
      if (compiler_unused) {
        THING_DBG(g, v, l, me, "stop");
      }

      return true;
    }

    if (thing_is_collision_hit_first_on_tile(me)) {
      return true;
    }

    hit_something = true;
  }

  return hit_something;
}

//
// Do accurate hit box collision detection for this interpolated position
//
static void thing_collision_handle_interpolated_delta(Gamep g, Levelsp v, Levelp l, Thingp me, const bpoint &collision_at,
                                                      const fpoint &interp_at_f, ThingCands &cands)
{
  TRACE();

  FOR_ALL_THINGS_AT(g, v, l, obstacle, collision_at)
  {
    if (obstacle == me) {
      continue;
    }

    auto collision = thing_collision_check(g, v, l, me, interp_at_f, obstacle);
    if (! collision) {
      continue;
    }

    if (collision) {
      auto at = thing_real_at(g, v, l, me);
      //
      // Sort by center of the tile distance. This allows walls and ghost in walls to have
      // the same distance
      //
      auto            o_tiled_at = make_fpoint(thing_at(g, v, l, obstacle)) + fpoint(0.5, 0.5);
      float const     o_dist     = distance(at, o_tiled_at);
      ThingCand const p          = std::make_pair(o_dist, obstacle);
      cands.push_back(p);
    }
  }
}

//
// For fast moving objects, they might jump 1 or 2 tiles, so we need to collision detect
// for enough points in between to make sense.
//
void thing_collision_handle_interpolated(Gamep g, Levelsp v, Levelp l, Thingp me, fpoint old_at)
{
  THING_DBG(g, v, l, me, "thing_collision_handle_interpolated");
  TRACE_INDENT();

  auto        at    = thing_real_at(g, v, l, me);
  float const dist  = distance(at, old_at);
  float const steps = ceil(dist) * 10;
  auto        diff  = at - old_at;
  float const stepx = diff.x / steps;
  float const stepy = diff.y / steps;

  for (auto step = 0; step < steps; step++) {
    fpoint const interp_at_f(old_at.x + (stepx * step), old_at.y + (stepy * step));
    bpoint const interp_at = make_bpoint(static_cast< int >(interp_at_f.x), static_cast< int >(interp_at_f.y));

    if (compiler_unused) {
      THING_DBG(g, v, l, me, "interp collision at %f,%f step %d", interp_at_f.x, interp_at_f.y, step);
      for (auto dx = -1; dx <= 1; dx++) {
        for (auto dy = -1; dy <= 1; dy++) {
          bpoint collision_at(interp_at.x + dx, interp_at.y + dy);
          THING_DBG(g, v, l, me, "- at (%d,%d)", collision_at.x, collision_at.y);
          FOR_ALL_THINGS_AT(g, v, l, obstacle, collision_at)
          {
            if (obstacle != me) {
              THING_DBG(g, v, l, obstacle, "   - check for collision");
            }
          }
        }
      }
    }

    ThingCands cands;

    for (auto dx = -1; dx <= 1; dx++) {
      for (auto dy = -1; dy <= 1; dy++) {
        bpoint const collision_at(interp_at.x + dx, interp_at.y + dy);

        //
        // Do accurate hit box collision detection for this interpolated position
        //
        thing_collision_handle_interpolated_delta(g, v, l, me, collision_at, interp_at_f, cands);

        //
        // Sort the candidates by distance / potentially add more cands if we can hit all
        // things on the same tile
        //
        thing_collision_interpolated_expand_candidates(g, v, l, me, collision_at, cands);
      }
    }

    //
    // Sort the candidates by distance / potentially add more cands if we can hit all
    // things on the same tile
    //
    thing_collision_sort_cands(g, v, l, me, cands);

    //
    // Process the collision candidate list
    //
    if (thing_collision_interplolated_process_candidates(g, v, l, me, interp_at_f, cands)) {
      return;
    }
  }

  if (compiler_unused) {
    THING_DBG(g, v, l, me, "-");
  }
}

void tp_collision_init(Tpp tp)
{
  TRACE();

  if (tp_is_collision_circle_small(tp) || //
      tp_is_collision_circle_large(tp) || //
      tp_is_collision_square(tp)) {
    tp_flag_set(tp, is_collision_detection_enabled);

    if (tp_priority_get(tp) == THING_PRIORITY_NONE) {
      tp_err(tp, "need to set priority");
    }
  }
}

[[nodiscard]] auto thing_is_collision_hit_first_on_tile(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_collision_hit_first_on_tile) != 0;
}

[[nodiscard]] auto thing_is_collision_hit_all_on_tile(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_collision_hit_all_on_tile) != 0;
}

[[nodiscard]] auto thing_is_collision_detection_enabled(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_collision_detection_enabled) != 0;
}

[[nodiscard]] auto thing_is_collision_circle_large(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_collision_circle_large) != 0;
}

[[nodiscard]] auto thing_is_collision_circle_small(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_collision_circle_small) != 0;
}

[[nodiscard]] auto thing_collision_radius(Thingp t) -> float
{
  TRACE_DEBUG();

  return thing_is_collision_circle_small(t) ? THING_COLLISION_CIRCLE_SMALL_RADIUS : THING_COLLISION_CIRCLE_LARGE_RADIUS;
}

[[nodiscard]] auto thing_is_collision_square(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_collision_square) != 0;
}
