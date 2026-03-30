//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_level_inlines.hpp"
#include "my_main.hpp"
#include "my_thing_inlines.hpp"

#include <algorithm>
#include <cmath>

using ThingCand  = std::pair< float, Thingp >;
using ThingCands = std::vector< ThingCand >;

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
    if (! thing_is_teleport_blocked(me)) {
      THING_DBG(me, "over teleport");
      TRACE_INDENT();
      if (thing_warp_to(g, v, l, me, thing_at(obstacle))) {
        (void) thing_teleport_handle(g, v, l, me);
        stop = true;
        return;
      }
    }
  }

  //
  // Handle walking onto a chasm
  //
  if (thing_is_chasm(obstacle)) {
    if (thing_is_able_to_fall(me)) {
      THING_DBG(me, "over chasm");
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
  TRACE();

  thing_collision_handle_common(g, v, l, obstacle, me, stop);

  if (! thing_is_hit_when_dead(obstacle)) {
    return;
  }

  if (thing_is_projectile(me)) {
    auto *source     = me;
    auto  event_type = THING_EVENT_FIRE_DAMAGE;
    auto  damage     = tp_damage(thing_tp(source), event_type);

    ThingEvent e {
        .reason     = "by projectile fire damage", //
        .event_type = event_type,                  //
        .damage     = damage,                      //
        .source     = source,                      //
    };

    THING_DBG(me, "collision with");
    THING_DBG(obstacle, "me");

    thing_damage(g, v, l, obstacle, e);
  }

  if (thing_is_dead_on_collision(me)) {
    ThingEvent e {
        .reason     = "collided",                   //
        .event_type = THING_EVENT_LIFESPAN_EXPIRED, //
    };

    thing_dead(g, v, l, me, e);
    stop = true;
  }
}

//
// Handle interactions for a thing at its location with an alive thing
//
static void thing_collision_handle_alive_thing(Gamep g, Levelsp v, Levelp l, Thingp obstacle, Thingp me, bool &stop)
{
  TRACE();

  thing_collision_handle_common(g, v, l, obstacle, me, stop);
  if (stop) {
    return;
  }

  //
  // Crush obstacle
  //
  if (thing_crush(g, v, l, obstacle, me)) {
    stop = true;
    return;
  }

  //
  // Collect keys?
  //
  if (thing_collect_key(g, v, l, obstacle, me)) {
    return;
  }

  //
  // Collect items?
  //
  if (thing_carry_item(g, v, l, obstacle, me)) {
    return;
  }

  if (thing_is_projectile(me)) {
    auto *source     = me;
    auto  event_type = THING_EVENT_FIRE_DAMAGE;
    auto  damage     = tp_damage(thing_tp(source), event_type);

    ThingEvent e {
        .reason     = "by projectile fire damage", //
        .event_type = event_type,                  //
        .damage     = damage,                      //
        .source     = source,                      //
    };

    THING_DBG(me, "collision with");
    THING_DBG(obstacle, "me");

    thing_damage(g, v, l, obstacle, e);
  }

  if (thing_is_dead_on_collision(me)) {
    ThingEvent e {
        .reason     = "collided",                   //
        .event_type = THING_EVENT_LIFESPAN_EXPIRED, //
    };

    thing_dead(g, v, l, me, e);
    stop = true;
  }
}

//
// Handle interactions for a thing at its location
//
// Return true to continue with more collisions, or false to stop.
// e.g. if we teleport, we want to stop
//
static void thing_collision_handle(Gamep g, Levelsp v, Levelp l, Thingp obstacle, Thingp me, bool &stop)
{
  TRACE();

  if (obstacle == me) {
    return;
  }

  if (thing_is_ethereal(me)) {
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
  TRACE();

  if (compiler_unused) {
    THING_DBG(me, "thing_collision_handle");
  }

  //
  // Projectiles handled seperately.
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

  //
  // Common collision handling for player and anything else
  //
  auto at = thing_at(me);
  FOR_ALL_THINGS_AT(g, v, l, obstacle, at)
  {
    bool stop = {};
    thing_collision_handle(g, v, l, obstacle, me, stop);
    if (stop) {
      return;
    }
  }
}

static auto thing_collision_check_circle_circle(Gamep g, Levelsp v, Levelp l, Thingp A, fpoint A_at, Thingp B, fpoint B_at) -> bool
{
  float const A_radius = thing_collision_radius(A);
  float const B_radius = thing_collision_radius(B);

  float const touching_dist = A_radius + B_radius;
  float const dist          = distance(A_at, B_at);

  if (compiler_unused) {
    THING_DBG(A, "A %f,%f touching_dist %f dist %f ", A_at.x, A_at.y, touching_dist, dist);
    THING_DBG(B, "B %f,%f", B_at.x, B_at.y);
  }

  if (dist >= touching_dist) {
    return false;
  }

  return true;
}

[[nodiscard]] static auto thing_collision_check_circle_square(Thingp C, fpoint C_at, Thingp B, fpoint B_at) -> bool
{
  float const radius = thing_collision_radius(C);

  //
  // Need to perform calculations from the center of the circle as the
  // circle is really inside a tile with the top left co-ords being the
  // blit point for the tile. The radius then is used from this cental
  // point for collision.s
  //
  C_at.x += 0.5F;
  C_at.y += 0.5F;

  fpoint const tl(B_at.x, B_at.y);
  fpoint const br(B_at.x + 1, B_at.y + 1);

  if (compiler_unused) {
    THING_DBG(C, "circle %f,%f", C_at.x, C_at.y);
    THING_DBG(B, "box %f,%f -> %f,%f", tl.x, tl.y, br.x, br.y);
  }

  fpoint const B0(B_at.x - 0, B_at.y - 0);
  fpoint const B1(B_at.x + 1, B_at.y - 0);
  fpoint const B2(B_at.x + 1, B_at.y + 1);
  fpoint const B3(B_at.x + 0, B_at.y + 1);

  //
  // Circle inside box
  //
  if ((C_at.x >= B0.x) && (C_at.y >= B0.y) && (C_at.x <= B2.x) && (C_at.y <= B2.y)) {
    return true;
  }

  //
  // Corner collisions.
  //
  if (distance(C_at, B0) < radius) {
    return true;
  }

  if (distance(C_at, B1) < radius) {
    return true;
  }

  if (distance(C_at, B2) < radius) {
    return true;
  }

  if (distance(C_at, B3) < radius) {
    return true;
  }

  fpoint *intersect_out = nullptr;
  float   dist          = 0;

  if (distance_to_line(C_at, B0, B1, &dist, intersect_out) != 0) {
    if (dist < radius) {
      return true;
    }
  }

  if (distance_to_line(C_at, B1, B2, &dist, intersect_out) != 0) {
    if (dist < radius) {
      return true;
    }
  }

  if (distance_to_line(C_at, B2, B3, &dist, intersect_out) != 0) {
    if (dist < radius) {
      return true;
    }
  }

  if (distance_to_line(C_at, B3, B0, &dist, intersect_out) != 0) {
    if (dist < radius) {
      return true;
    }
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

[[nodiscard]] static auto thing_collision_check_circle_small_circle_small(Gamep g, Levelsp v, Levelp l, Thingp me, fpoint me_at, Thingp o,
                                                                          fpoint o_at) -> bool
{
  TRACE();
  return thing_collision_check_circle_circle(g, v, l, me, me_at, o, o_at);
}

[[nodiscard]] static auto thing_collision_check_circle_small_circle_large(Gamep g, Levelsp v, Levelp l, Thingp me, fpoint me_at, Thingp o,
                                                                          fpoint o_at) -> bool
{
  TRACE();
  return thing_collision_check_circle_circle(g, v, l, me, me_at, o, o_at);
}

[[nodiscard]] static auto thing_collision_check_circle_small_square(Gamep g, Levelsp v, Levelp l, Thingp me, fpoint me_at, Thingp o,
                                                                    fpoint o_at) -> bool
{
  TRACE();
  return thing_collision_check_circle_square(me, me_at, o, o_at);
}

[[nodiscard]] static auto thing_collision_check_circle_large_circle_large(Gamep g, Levelsp v, Levelp l, Thingp me, fpoint me_at, Thingp o,
                                                                          fpoint o_at) -> bool
{
  TRACE();
  return thing_collision_check_circle_circle(g, v, l, me, me_at, o, o_at);
}

[[nodiscard]] static auto thing_collision_check_circle_large_square(Gamep g, Levelsp v, Levelp l, Thingp me, fpoint me_at, Thingp o,
                                                                    fpoint o_at) -> bool
{
  TRACE();
  return thing_collision_check_circle_square(me, me_at, o, o_at);
}

[[nodiscard]] static auto thing_collision_check_square_square(Gamep g, Levelsp v, Levelp l, Thingp me, fpoint me_at, Thingp o, fpoint o_at)
    -> bool
{
  TRACE();
  return thing_collision_check_squares(me_at, o_at);
}

//
// Sort the candidates by distance / potentially add more cands if we can hit all
// things on the same tile
//
static void thing_collision_interpolated_expand_candidates(Gamep g, Levelsp v, Levelp l, Thingp me, const bpoint &collision_at,
                                                           ThingCands &cands)
{
  TRACE();

  //
  // If this is a fireball hitting a wall, then we want to hit the ghost that is
  // also hiding inside the wall
  //
  if (cands.empty()) {
    return;
  }

  if (! thing_is_collision_hit_all_on_tile(me) && ! thing_is_collision_hit_first_on_tile(me)) {
    return;
  }

  auto at = thing_real_at(me);

  auto *owner = top_owner(g, v, l, me);

  //
  // For all other things on the same tile as the collision
  //
  FOR_ALL_THINGS_AT(g, v, l, o, collision_at)
  {
    //
    // Filter to only things that can be hit
    //
    if (! thing_is_collision_detection_enabled(o)) {
      continue;
    }

    if (o == me) {
      continue;
    }

    if (o == owner) {
      continue;
    }

    //
    // Check this thing is not on the cand list already
    //
    bool already_cand = false;
    for (auto a_cand : cands) {
      if (a_cand.second == o) {
        already_cand = true;
      }
      break;
    }

    if (! already_cand) {
      //
      // Sort by center of the tile distance. This allows walls and ghost in walls to have
      // the same distance
      //
      auto            o_tiled_at = make_fpoint(thing_at(o)) + fpoint(0.5, 0.5);
      float const     o_dist     = distance(at, o_tiled_at);
      ThingCand const p          = std::make_pair(o_dist, o);
      cands.push_back(p);
    }
  }
}

//
// Sort the candidates by distance / potentially add more cands if we can hit all
// things on the same tile
//
static void thing_collision_interpolated_sort_candidates(Gamep g, Levelsp v, Levelp l, Thingp me, ThingCands &cands)
{
  TRACE();

  if (compiler_unused) {
    THING_DBG(me, "final cands: (pre sort)");
    for (auto a_cand : cands) {
      auto  o_dist = a_cand.first;
      auto *o      = a_cand.second;

      THING_DBG(o, "- sort_distance %f prio %u", o_dist, thing_priority(o));
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
    if (d1 < d2) {
      return true;
    } else {
      return false;
    }
  });

  //
  // Dump the final cands
  //
  if (compiler_unused) {
    THING_DBG(me, "final cands:");
    for (auto a_cand : cands) {
      auto  o_dist = a_cand.first;
      auto *o      = a_cand.second;

      THING_DBG(o, "- sort_distance %f prio %u", o_dist, thing_priority(o));
    }
  }
}

//
// Process the collision candidate list
//
static auto thing_collision_interplolated_process_candidates(Gamep g, Levelsp v, Levelp l, Thingp me, const ThingCands &cands) -> bool
{
  TRACE();

  bool hit_something = {};

  for (auto cand : cands) {
    auto *o = cand.second;

    //
    // Skip things that are dead; unless we can hit their corpse
    //
    if (thing_is_dead(o)) {
      if (! thing_is_hit_when_dead(o)) {
        continue;
      }
    }

    //
    // Flash the thing red
    //
    thing_is_hit_set(g, v, l, o, THING_HIT_FLASH_TIME_MS);

    //
    // Handle the actual collision
    //
    bool stop = {};
    thing_collision_handle(g, v, l, o, me, stop);
    if (stop) {
      if (compiler_unused) {
        THING_DBG(me, "stop");
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
// Do accurate hit box collision detection for this interpolated postion
//
static void thing_collision_handle_interpolated_delta(Gamep g, Levelsp v, Levelp l, Thingp me, const bpoint &collision_at,
                                                      const fpoint &interp_at_f, ThingCands &cands)
{
  TRACE();

  FOR_ALL_THINGS_AT(g, v, l, o, collision_at)
  {
    if (o == me) {
      continue;
    }

    auto o_at      = thing_real_at(o);
    auto collision = false;

    if (thing_is_collision_circle_small(me)) {
      if (thing_is_collision_circle_small(o)) {
        collision = thing_collision_check_circle_small_circle_small(g, v, l, me, interp_at_f, o, o_at);
        if (compiler_unused) {
          THING_DBG(o, "cand coll %d collision=%d", __LINE__, static_cast< int >(collision));
        }
      } else if (thing_is_collision_circle_large(o)) {
        collision = thing_collision_check_circle_small_circle_large(g, v, l, me, interp_at_f, o, o_at);
        if (compiler_unused) {
          THING_DBG(o, "cand coll %d collision=%d", __LINE__, static_cast< int >(collision));
        }
      } else if (thing_is_collision_square(o)) {
        collision = thing_collision_check_circle_small_square(g, v, l, me, interp_at_f, o, o_at);
        if (compiler_unused) {
          THING_DBG(o, "cand coll %d collision=%d", __LINE__, static_cast< int >(collision));
        }
      } else {
        continue;
      }
    } else if (thing_is_collision_circle_large(me)) {
      if (thing_is_collision_circle_small(o)) {
        collision = thing_collision_check_circle_small_circle_large(g, v, l, me, interp_at_f, o, o_at);
        if (compiler_unused) {
          THING_DBG(o, "cand coll %d collision=%d", __LINE__, static_cast< int >(collision));
        }
      } else if (thing_is_collision_circle_large(o)) {
        collision = thing_collision_check_circle_large_circle_large(g, v, l, me, interp_at_f, o, o_at);
        if (compiler_unused) {
          THING_DBG(o, "cand coll %d collision=%d", __LINE__, static_cast< int >(collision));
        }
      } else if (thing_is_collision_square(o)) {
        collision = thing_collision_check_circle_large_square(g, v, l, me, interp_at_f, o, o_at);
        if (compiler_unused) {
          THING_DBG(o, "cand coll %d collision=%d", __LINE__, static_cast< int >(collision));
        }
      } else {
        continue;
      }
    } else if (thing_is_collision_square(me)) {
      if (thing_is_collision_circle_small(o)) {
        collision = thing_collision_check_circle_small_square(g, v, l, o, o_at, me, interp_at_f);
        if (compiler_unused) {
          THING_DBG(o, "cand coll %d collision=%d", __LINE__, static_cast< int >(collision));
        }
      } else if (thing_is_collision_circle_large(o)) {
        collision = thing_collision_check_circle_large_square(g, v, l, o, o_at, me, interp_at_f);
        if (compiler_unused) {
          THING_DBG(o, "cand coll %d collision=%d", __LINE__, static_cast< int >(collision));
        }
      } else if (thing_is_collision_square(o)) {
        collision = thing_collision_check_square_square(g, v, l, o, o_at, me, interp_at_f);
        if (compiler_unused) {
          THING_DBG(o, "cand coll %d collision=%d", __LINE__, static_cast< int >(collision));
        }
      } else {
        continue;
      }
    } else {
      thing_err(me, "no collision type set");
      continue;
    }

    if (collision) {
      auto at = thing_real_at(me);
      //
      // Sort by center of the tile distance. This allows walls and ghost in walls to have
      // the same distance
      //
      auto            o_tiled_at = make_fpoint(thing_at(o)) + fpoint(0.5, 0.5);
      float const     o_dist     = distance(at, o_tiled_at);
      ThingCand const p          = std::make_pair(o_dist, o);
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
  TRACE();

  if (compiler_unused) {
    THING_DBG(me, "thing_collision_handle_interpolated");
  }

  auto        at    = thing_real_at(me);
  float const dist  = distance(at, old_at);
  float const steps = ceil(dist) * 10;
  auto        diff  = at - old_at;
  float const stepx = diff.x / steps;
  float const stepy = diff.y / steps;

  for (auto step = 0; step < steps; step++) {
    fpoint const interp_at_f(old_at.x + (stepx * step), old_at.y + (stepy * step));
    bpoint const interp_at = make_bpoint(static_cast< int >(interp_at_f.x), static_cast< int >(interp_at_f.y));

    if (compiler_unused) {
      THING_DBG(me, "interp collision at %f,%f step %d", interp_at_f.x, interp_at_f.y, step);
      for (auto dx = -1; dx <= 1; dx++) {
        for (auto dy = -1; dy <= 1; dy++) {
          bpoint collision_at(interp_at.x + dx, interp_at.y + dy);
          THING_DBG(me, "- at %d,%d", collision_at.x, collision_at.y);
          FOR_ALL_THINGS_AT(g, v, l, o, collision_at)
          {
            if (o != me) {
              THING_DBG(o, "   - check for collision");
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
        // Do accurate hit box collision detection for this interpolated postion
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
    thing_collision_interpolated_sort_candidates(g, v, l, me, cands);

    //
    // Process the collision candidate list
    //
    if (thing_collision_interplolated_process_candidates(g, v, l, me, cands)) {
      return;
    }
  }

  if (compiler_unused) {
    THING_DBG(me, "-");
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
