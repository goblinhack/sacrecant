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
// Is this follower attached to a leader?
//
[[nodiscard]] auto thing_follower_leader_get(Gamep g, Levelsp v, Levelp l, Thingp me) -> Thingp
{
  if (me == nullptr) {
    return nullptr;
  }

  auto *ext = thing_ext_struct(g, v, me);
  if (ext == nullptr) [[unlikely]] {
    return nullptr;
  }

  if (ext->leader_id == 0U) {
    return nullptr;
  }

  return thing_find(g, v, ext->leader_id);
}

//
// Get the dmap associated with the leader
//
[[nodiscard]] auto thing_follower_get_leader_dmap(Gamep g, Levelsp v, Levelp l, Thingp me) -> Dmap *
{
  TRACE();

  auto *leader = thing_follower_leader_get(g, v, l, me);
  if (leader == nullptr) {
    return nullptr; // can be normal if detached
  }

  auto *leader_ext = thing_dmap_struct(g, v, leader);
  if (leader_ext == nullptr) {
    return nullptr;
  }

  return &leader_ext->dmap;
}

//
// Detach a follower from its leader
//
[[nodiscard]] auto thing_follower_detach_me_from_leader(Gamep g, Levelsp v, Levelp l, Thingp me) -> bool
{
  TRACE();

  if (me == nullptr) {
    return false;
  }

  auto *leader = thing_follower_leader_get(g, v, l, me);
  if (leader == nullptr) {
    return false; // can be normal if detached
  }

  THING_DBG(g, v, l, me, "detach me from leader");
  TRACE_INDENT();

  return thing_leader_detach_follower(g, v, l, leader, me);
}

//
// Collect all possible points we could wander to in the given dmap.
// The points are arranged in a radius around the dmap goal, which was
// what was presumably used to create the dmap.
//
[[nodiscard]] static auto thing_follower_get_leader_dmap_target_cand(Gamep g, Levelsp v, Levelp l, Thingp me, bpoint dmap_goal, Dmap *dmap,
                                                                     int radius, bpoint &target) -> bool
{
  TRACE();

  //
  // Look at tiles surrounding the leader for somewhere to wander to
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
// Given a leader, choose somewhere to wander, near the leader.
//
[[nodiscard]] auto thing_follower_choose_target_near_leader(Gamep g, Levelsp v, Levelp l, Thingp me) -> bool
{
  THING_DBG(g, v, l, me, "choose target: near leader");
  TRACE_INDENT();

  auto *leader = thing_follower_leader_get(g, v, l, me);
  if (leader == nullptr) {
    THING_DBG(g, v, l, me, "choose target: no leader");
    return false; // can be normal if detached
  }

  auto *dmap = thing_follower_get_leader_dmap(g, v, l, me);
  if (dmap == nullptr) {
    THING_DBG(g, v, l, me, "choose target: no leader dmap");
    return false;
  }

  auto follower_at = thing_at(g, v, l, me);
  auto leader_at   = thing_at(g, v, l, leader);

  //
  // How far to look for a target?
  //
  auto radius = thing_distance_follower_from_leader_max(g, v, l, me);
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
    if (! thing_follower_get_leader_dmap_target_cand(g, v, l, me, leader_at, dmap, radius, target)) {
      continue;
    }

    THING_DBG(g, v, l, me, "astar thing_follower_choose_target_near_leader");
    auto p = astar_solve(g, v, l, me, follower_at, target);
    if (p.empty()) {
      continue;
    }

    if (thing_move_path_apply(g, v, l, me, p)) {
      thing_monst_target_set(g, v, l, me, target);
      THING_DBG(g, v, l, me, "choose target: wander around leader");
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
[[nodiscard]] auto thing_follower_can_move_to_possible(Gamep g, Levelsp v, Levelp l, Thingp me, const bpoint &to) -> bool
{
  TRACE();

  //
  // If a follower, check we're not moving too far from the leader
  //
  auto *leader = thing_follower_leader_get(g, v, l, me);
  if (leader == nullptr) {
    //
    // If unleashed, ignore
    //
    return true;
  }

  if (distance(thing_at(g, v, l, leader), to) > thing_distance_follower_from_leader_max(g, v, l, me)) {
    return false;
  }

  return true;
}

[[nodiscard]] auto thing_is_follower(Thingp me) -> bool
{
  TRACE_DEBUG();

  if (me == nullptr) {
    no_thing_pointer();
    return false;
  }

  if (me->_is_follower) {
    return true;
  }

  return tp_flag(thing_tp(me), is_follower) != 0;
}

void thing_is_follower_set(Thingp me)
{
  TRACE_DEBUG();

  if (me == nullptr) {
    no_thing_pointer();
    return;
  }

  me->_is_follower = true;
}

[[nodiscard]] auto thing_follower_max(Gamep g, Levelsp v, Levelp l, Thingp me) -> int
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

  return ext->_follower_max;
}

[[nodiscard]] auto thing_follower_max_set(Gamep g, Levelsp v, Levelp l, Thingp me, int val) -> int
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

  if (val > THING_FOLLOWER_MAX) {
    thing_croak(g, v, l, me, "trying to set follower max too high");
    val = THING_FOLLOWER_MAX;
  }

  return ext->_follower_max = val;
}

[[nodiscard]] auto thing_follower_max_incr(Gamep g, Levelsp v, Levelp l, Thingp me, int val) -> int
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

  if (ext->_follower_max + val >= THING_FOLLOWER_MAX) {
    return ext->_follower_max = THING_FOLLOWER_MAX;
  }

  return ext->_follower_max += val;
}

[[nodiscard]] auto thing_follower_max_decr(Gamep g, Levelsp v, Levelp l, Thingp me, int val) -> int
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

  if (static_cast< int >(ext->_follower_max) - val <= 0) {
    return ext->_follower_max = 0;
  }
  return ext->_follower_max -= val;
}

[[nodiscard]] auto thing_distance_follower_from_leader_max(Gamep g, Levelsp v, Levelp l, Thingp me) -> int
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

  return ext->_distance_follower_from_leader_max;
}

[[nodiscard]] auto thing_distance_follower_from_leader_max_set(Gamep g, Levelsp v, Levelp l, Thingp me, int val) -> int
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

  return ext->_distance_follower_from_leader_max = val;
}
