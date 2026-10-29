//
// Copyright goblinhack@gmail.com
//

#include "my_bpoint.hpp"
#include "my_callstack.hpp"
#include "my_level.hpp"
#include "my_level_inlines.hpp" // NOLINT
#include "my_main.hpp"
#include "my_thing.hpp"
#include "my_thing_inlines.hpp" // NOLINT
#include "my_tp.hpp"
#include "my_types.hpp"
#include <cstring>

void thing_leader_dead(Gamep g, Levelsp v, Levelp l, Thingp leader, ThingEvent &e)
{
  TRACE();

  if (leader == nullptr) {
    return;
  }

  if (! thing_is_leader(leader)) {
    return;
  }

  if (thing_is_leader_kill_followers_on_death(leader)) {
    (void) thing_leader_kill_all_followers(g, v, l, leader, e);
  } else {
    (void) thing_leader_detach_all_followers(g, v, l, leader);
  }
}

//
// How many followers?
//
[[nodiscard]] auto thing_leader_follower_count_get(Gamep g, Levelsp v, Levelp l, Thingp leader) -> int
{
  TRACE();

  if (leader == nullptr) {
    return 0;
  }

  if (! thing_is_leader(leader)) {
    return 0;
  }

  auto *ext = thing_ext_struct(g, v, leader);
  if (ext == nullptr) {
    return 0;
  }

  return ext->followers.count;
}

//
// Add a follower to the leader if possible
//
[[nodiscard]] auto thing_leader_spawn_a_follower_at(Gamep g, Levelsp v, Levelp l, Thingp leader, Tpp tp_follower, const bpoint &at) -> Thingp
{
  TRACE();

  if (leader == nullptr) {
    return nullptr;
  }

  if (! thing_is_leader(leader)) {
    thing_err(g, v, l, leader, "non leader trying to spawn followers");
    return nullptr;
  }

  if (tp_follower == nullptr) {
    thing_err(g, v, l, leader, "no follower to spawn");
    return nullptr;
  }

  //
  // Can't spawn over certain things
  //
  if (level_is_obs_to_spawning_bool(g, v, l, at)) {
    return nullptr;
  }

  auto *ext = thing_ext_struct(g, v, leader);
  if (ext == nullptr) {
    return nullptr;
  }

  //
  // Too many followers
  //
  if (thing_leader_follower_count_get(g, v, l, leader) >= thing_follower_max(g, v, l, leader)) {
    return nullptr;
  }

  //
  // Look for a free slot
  //
  FOR_ALL_FOLLOWER_SLOTS_NO_BREAK(g, v, l, leader, slot, existing_follower)
  {
    if (existing_follower != nullptr) {
      continue;
    }

    //
    // Create the follower. Should be no chance to fail now.
    //
    auto *new_follower = thing_spawn(g, v, l, tp_follower, at);
    if (new_follower == nullptr) {
      return nullptr;
    }

    auto *new_follower_ext = thing_ext_struct(g, v, new_follower);
    if (new_follower_ext == nullptr) [[unlikely]] {
      thing_err(g, v, l, new_follower, "no thing ext pointer");
      no_thing_ext_pointer();
      return nullptr;
    }

    memset(slot, 0, sizeof(*slot));
    slot->follower_id           = new_follower->id;
    new_follower_ext->leader_id = leader->id;
    ext->followers.count++;

    THING_DBG(g, v, l, leader, "spawned follower %s (max %d)", to_string(g, v, l, new_follower).c_str(), thing_follower_max(g, v, l, leader));
    THING_DBG(g, v, l, new_follower, "new born follower");

    thing_is_follower_set(new_follower);

    return new_follower;
  }

  //
  // Out of slots; but we checked above
  //
  thing_err(g, v, l, leader, "unexpectedly out of follower slots");

  return nullptr;
}

//
// Add a follower to the leader if possible
//
[[nodiscard]] auto thing_leader_spawn_a_follower(Gamep g, Levelsp v, Levelp l, Thingp leader, Tpp tp_follower) -> Thingp
{
  TRACE();

  return thing_leader_spawn_a_follower_at(g, v, l, leader, tp_follower, thing_at(g, v, l, leader));
}

//
// Detach or kill all followers (or a specific one)
//
[[nodiscard]] static auto thing_leader_process_followers(Gamep g, Levelsp v, Levelp l, Thingp leader, Thingp specific_follower, ThingEvent &e)
    -> bool
{
  TRACE();

  if (leader == nullptr) {
    return false;
  }

  if (! thing_is_leader(leader)) {
    thing_err(g, v, l, leader, "non leader trying to detach followers");
    return false;
  }

  auto *ext = thing_ext_struct(g, v, leader);
  if (ext == nullptr) {
    return false;
  }

  THING_DBG(g, v, l, leader, "process followers");
  TRACE_INDENT();

  IF_DEBUG2 { thing_leader_dump_followers(g, v, l, leader); }

  FOR_ALL_FOLLOWER_SLOTS_NO_BREAK(g, v, l, leader, slot, follower)
  {
    TRACE();

    if (follower == nullptr) {
      continue;
    }

    if (specific_follower != nullptr) {
      if (follower != specific_follower) {
        continue;
      }
    }

    THING_DBG(g, v, l, follower, "process follower");
    TRACE_INDENT();

    auto *follower_ext = thing_ext_struct(g, v, follower);
    if (follower_ext == nullptr) [[unlikely]] {
      thing_err(g, v, l, follower, "no thing ext pointer");
      no_thing_ext_pointer();
      continue;
    }

    if (! static_cast< bool >(follower_ext->leader_id)) {
      thing_err(g, v, l, leader, "leader found detached follower: %s", to_string(g, v, l, follower).c_str());
      return false;
    }

    if (ext->followers.count <= 0) {
      thing_err(g, v, l, leader, "leader has unexpected follower count when detaching: %s", to_string(g, v, l, follower).c_str());
      return false;
    }

    ext->followers.count--;
    memset(slot, 0, sizeof(*slot));
    follower_ext->leader_id = 0;

    if (e.event_type != THING_EVENT_NONE) {
      THING_DBG(g, v, l, leader, "kill follower %s", to_string(g, v, l, follower).c_str());
      TRACE_INDENT();
      thing_dead(g, v, l, follower, e);
    } else {
      THING_DBG(g, v, l, leader, "detach follower %s", to_string(g, v, l, follower).c_str());
    }
  }

  return true;
}

//
// Detach all followers from their leader
//
[[nodiscard]] auto thing_leader_detach_all_followers(Gamep g, Levelsp v, Levelp l, Thingp leader) -> bool
{
  TRACE();

  ThingEvent e = {};
  e.reason     = "detach all followers";
  return thing_leader_process_followers(g, v, l, leader, nullptr, e);
}

//
// Kill all followers
//
[[nodiscard]] auto thing_leader_kill_all_followers(Gamep g, Levelsp v, Levelp l, Thingp leader, ThingEvent &e) -> bool
{
  TRACE();

  return thing_leader_process_followers(g, v, l, leader, nullptr, e);
}

//
// Detach a follower from its leader
//
[[nodiscard]] auto thing_leader_detach_follower(Gamep g, Levelsp v, Levelp l, Thingp leader, Thingp follower) -> bool
{
  TRACE();

  ThingEvent e = {};
  e.reason     = "detach followers";
  return thing_leader_process_followers(g, v, l, leader, follower, e);
}

//
// Dump all followers
//
void thing_leader_dump_followers(Gamep g, Levelsp v, Levelp l, Thingp leader)
{
  TRACE();

  if (leader == nullptr) {
    return;
  }

  if (! thing_is_leader(leader)) {
    thing_err(g, v, l, leader, "non leader trying to detach follower");
    return;
  }

  auto *ext = thing_ext_struct(g, v, leader);
  if (ext == nullptr) {
    return;
  }

  FOR_ALL_FOLLOWER_SLOTS_NO_BREAK(g, v, l, leader, slot, existing_follower)
  {
    if (existing_follower == nullptr) {
      THING_DBG(g, v, l, leader, "slot %d: -", _follower_n_);
      continue;
    }

    auto s = to_string(g, v, l, existing_follower);
    THING_DBG(g, v, l, leader, "slot %d: %s", _follower_n_, s.c_str());
  }
}

[[nodiscard]] auto thing_is_leader(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    no_thing_pointer();
    return false;
  }
  return tp_flag(thing_tp(t), is_leader) != 0;
}

//
// If any followers have strayed too far, kill them
//
void thing_leader_followers_kill_if_too_far(Gamep g, Levelsp v, Levelp l, Thingp me)
{
  TRACE();

  auto leader_at = thing_at(g, v, l, me);

  FOR_ALL_FOLLOWERS(g, v, l, me, follower)
  {
    TRACE();

    auto follower_at = thing_at(g, v, l, follower);

    if (distance(leader_at, follower_at) > thing_distance_follower_from_leader_max(g, v, l, follower)) {
      //
      // Try to hold onto the tentacle, but if not, then it's done
      //
      if (! thing_warp_to(g, v, l, follower, leader_at)) {
        ThingEvent e {
            .reason     = "by moving",                  //
            .event_type = THING_EVENT_LIFESPAN_EXPIRED, //
        };

        THING_DBG(g, v, l, follower, "dead due to off the leash");
        TRACE_INDENT();
        thing_dead(g, v, l, follower, e);
      }
    }
  }
}

//
// Move all followers
//
// This is used for teleporting, falling etc...
//
void thing_leader_warp_followers_to(Gamep g, Levelsp v, Levelp old_level, Levelp new_level, Thingp me, const bpoint from, const bpoint to)
{
  TRACE();

  //
  // Move all followers
  //
  if (! thing_is_leader(me)) {
    return;
  }

  FOR_ALL_FOLLOWERS(g, v, old_level, me, follower)
  {
    TRACE();

    auto offset  = thing_at(g, v, old_level, follower) - from;
    auto warp_to = to + offset;

    if (is_oob(warp_to) || ! thing_warp_to(g, v, new_level, follower, to)) {
      ThingEvent e {
          .reason     = "by warping",                 //
          .event_type = THING_EVENT_LIFESPAN_EXPIRED, //
      };

      THING_DBG(g, v, new_level, follower, "dead due to warping");
      TRACE_INDENT();
      thing_dead(g, v, new_level, follower, e);
    }
  }

  thing_leader_followers_kill_if_too_far(g, v, new_level, me);
}

//
// Move all followers
//
void thing_leader_move_followers_offset(Gamep g, Levelsp v, Levelp l, Thingp me, const bpoint offset)
{
  TRACE();

  FOR_ALL_FOLLOWERS(g, v, l, me, follower)
  {
    TRACE();

    auto move_to = thing_at(g, v, l, follower) + offset;

    if (! thing_can_move_to_possible(g, v, l, me, move_to)) {
      continue;
    }

    if (! thing_move_to(g, v, l, follower, move_to)) {
      ThingEvent e {
          .reason     = "by moving",                  //
          .event_type = THING_EVENT_LIFESPAN_EXPIRED, //
      };

      THING_DBG(g, v, l, follower, "dead due to move fail");
      TRACE_INDENT();
      thing_dead(g, v, l, follower, e);
    }
  }

  thing_leader_followers_kill_if_too_far(g, v, l, me);
}
