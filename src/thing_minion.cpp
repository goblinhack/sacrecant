//
// Copyright goblinhack@gmail.com
//

#include "my_bpoint.hpp"
#include "my_callstack.hpp"
#include "my_dmap.hpp"
#include "my_level_inlines.hpp"
#include "my_random.hpp"
#include "my_thing.hpp"
#include "my_thing_inlines.hpp"
#include "my_tp.hpp"
#include "my_types.hpp"

#include <cstring>
#include <utility>

//
// Is this minion attached to a mob?
//
[[nodiscard]] auto thing_minion_mob_get(Gamep g, Levelsp v, Levelp l, Thingp me) -> Thingp
{
  if (me == nullptr) {
    return nullptr;
  }

  if (! thing_is_minion(me)) {
    return nullptr;
  }

  auto *ext = thing_ext_struct(g, v, me);
  if (ext == nullptr) [[unlikely]] {
    return nullptr;
  }

  if (ext->mob_id == 0U) {
    return nullptr;
  }

  return thing_find(g, v, ext->mob_id);
}

//
// Get the dmap associated with the mob
//
[[nodiscard]] auto thing_minion_get_mob_dmap(Gamep g, Levelsp v, Levelp l, Thingp me) -> Dmap *
{
  TRACE();

  auto *mob = thing_minion_mob_get(g, v, l, me);
  if (mob == nullptr) {
    return nullptr; // can be normal if detached
  }

  auto *mob_ext = thing_dmap_struct(g, v, mob);
  if (mob_ext == nullptr) {
    return nullptr;
  }

  return &mob_ext->dmap;
}

//
// Detach a minion from its mob
//
[[nodiscard]] auto thing_minion_detach_me_from_mob(Gamep g, Levelsp v, Levelp l, Thingp me) -> bool
{
  TRACE();

  if (me == nullptr) {
    return false;
  }

  if (! thing_is_minion(me)) {
    thing_err(g, v, l, me, "non minion trying to detach itself");
    return false;
  }

  auto *mob = thing_minion_mob_get(g, v, l, me);
  if (mob == nullptr) {
    return false; // can be normal if detached
  }

  THING_DBG(g, v, l, me, "detach me from mob");
  TRACE_INDENT();

  return thing_mob_detach_minion(g, v, l, mob, me);
}

//
// Collect all possible points we could wander to in the given dmap.
// The points are arranged in a radius around the dmap goal, which was
// what was presumably used to create the dmap.
//
[[nodiscard]] static auto thing_minion_get_mob_dmap_target_cand(Gamep g, Levelsp v, Levelp l, Thingp me, bpoint dmap_goal, Dmap *dmap,
                                                                int radius, bpoint &target) -> bool
{
  TRACE();

  //
  // Look at tiles surrounding the mob for somewhere to wander to
  //
  target.x = dmap_goal.x - radius + PCG_RANDOM_RANGE(0, radius * 2);
  target.y = dmap_goal.y - radius + PCG_RANDOM_RANGE(0, radius * 2);

  if (is_oob_or_border(target)) [[unlikely]] {
    return false;
  }

  if (target == dmap_goal) {
    return false;
  }

  if (target == thing_at(g, v, l, me)) {
    return false;
  }

  if (std::cmp_greater_equal(dmap->val[ target.x ][ target.y ], radius)) {
    return false;
  }

  if (! thing_can_move_to_ai(g, v, l, me, target)) {
    return false;
  }

  return true;
}

//
// Given a mob, choose somewhere to wander, near the mob.
//
[[nodiscard]] auto thing_minion_choose_target_near_mob(Gamep g, Levelsp v, Levelp l, Thingp me) -> bool
{
  THING_DBG(g, v, l, me, "choose target: near mob");
  TRACE_INDENT();

  auto *mob = thing_minion_mob_get(g, v, l, me);
  if (mob == nullptr) {
    THING_DBG(g, v, l, me, "choose target: no mob");
    return false; // can be normal if detached
  }

  auto *dmap = thing_minion_get_mob_dmap(g, v, l, me);
  if (dmap == nullptr) {
    THING_DBG(g, v, l, me, "choose target: no mob dmap");
    return false;
  }

  auto minion_at = thing_at(g, v, l, me);
  auto mob_at    = thing_at(g, v, l, mob);

  //
  // How far to look for a target?
  //
  auto radius = thing_distance_minion_from_mob_max(g, v, l, me);
  if (radius == 0) {
    thing_err(g, v, l, me, "unexpected value for radius");
    return false;
  }

  //
  // Keep trying to find a target
  //
  bpoint target;
  int    tries = 1;

  while (tries-- > 0) {
    if (! thing_minion_get_mob_dmap_target_cand(g, v, l, me, mob_at, dmap, radius, target)) {
      continue;
    }

    THING_DBG(g, v, l, me, "astar thing_minion_choose_target_near_mob");
    auto p = astar_solve(g, v, l, me, minion_at, target);
    if (p.empty()) {
      continue;
    }

    if (thing_move_path_apply(g, v, l, me, p)) {
      thing_monst_target_set(g, v, l, me, target);
      THING_DBG(g, v, l, me, "choose target: wander around mob");
      return true;
    }
  }

  return false;
}

//
// Returns true if it is POSSIBLE for this thing to move to this location.
// e.g. it is possible for a chasm or monst to move into a chasm. Does not
// mean the monst wants to do that.
//
// We do not change direction upon this attempt; purely a check only
//
[[nodiscard]] auto thing_minion_can_move_to_possible(Gamep g, Levelsp v, Levelp l, Thingp me, const bpoint &to) -> bool
{
  TRACE();

  if (! thing_is_minion(me)) {
    return true;
  }

  //
  // If a minion, check we're not moving too far from the mob
  //
  auto *mob = thing_minion_mob_get(g, v, l, me);
  if (mob == nullptr) {
    //
    // If unleashed, ignore
    //
    return true;
  }

  if (distance(thing_at(g, v, l, mob), to) > thing_distance_minion_from_mob_max(g, v, l, me)) {
    return false;
  }

  return true;
}

[[nodiscard]] auto thing_is_minion(Thingp me) -> bool
{
  TRACE_DEBUG();

  if (me == nullptr) {
    no_thing_pointer();
    return false;
  }
  return tp_flag(thing_tp(me), is_minion) != 0;
}

[[nodiscard]] auto thing_minion_max(Gamep g, Levelsp v, Levelp l, Thingp me) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    no_thing_pointer();
    return 0;
  }

  auto *ext = thing_ext_struct(g, v, me);
  if (ext == nullptr) [[unlikely]] {
    return 0;
  }

  return ext->_minion_max;
}

[[nodiscard]] auto thing_minion_max_set(Gamep g, Levelsp v, Levelp l, Thingp me, int val) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    no_thing_pointer();
    return 0;
  }

  auto *ext = thing_ext_struct(g, v, me);
  if (ext == nullptr) [[unlikely]] {
    return 0;
  }

  if (val > THING_MINION_MAX) {
    thing_croak(g, v, l, me, "trying to set minion max too high");
    val = THING_MINION_MAX;
  }

  return ext->_minion_max = val;
}

[[nodiscard]] auto thing_minion_max_incr(Gamep g, Levelsp v, Levelp l, Thingp me, int val) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    no_thing_pointer();
    return 0;
  }

  auto *ext = thing_ext_struct(g, v, me);
  if (ext == nullptr) [[unlikely]] {
    return 0;
  }

  if (ext->_minion_max + val >= THING_MINION_MAX) {
    return ext->_minion_max = THING_MINION_MAX;
  }

  return ext->_minion_max += val;
}

[[nodiscard]] auto thing_minion_max_decr(Gamep g, Levelsp v, Levelp l, Thingp me, int val) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    no_thing_pointer();
    return 0;
  }

  auto *ext = thing_ext_struct(g, v, me);
  if (ext == nullptr) [[unlikely]] {
    return 0;
  }

  if (static_cast< int >(ext->_minion_max) - val <= 0) {
    return ext->_minion_max = 0;
  }
  return ext->_minion_max -= val;
}

[[nodiscard]] auto thing_distance_minion_from_mob_max(Gamep g, Levelsp v, Levelp l, Thingp me) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    no_thing_pointer();
    return 0;
  }

  auto *ext = thing_ext_struct(g, v, me);
  if (ext == nullptr) {
    return 0;
  }

  return ext->_distance_minion_from_mob_max;
}

[[nodiscard]] auto thing_distance_minion_from_mob_max_set(Gamep g, Levelsp v, Levelp l, Thingp me, int val) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    no_thing_pointer();
    return 0;
  }

  auto *ext = thing_ext_struct(g, v, me);
  if (ext == nullptr) {
    return 0;
  }

  return ext->_distance_minion_from_mob_max = val;
}
