//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_game_defs.hpp"
#include "my_level.hpp"
#include "my_level_inlines.hpp"
#include "my_main.hpp"
#include "my_random.hpp"
#include "my_thing.hpp"
#include "my_thing_inlines.hpp"
#include "my_tp.hpp"
#include "my_types.hpp"
#include <vector>

//
// Push an additional tile for the thing
//
[[nodiscard]] auto thing_push_additional(Gamep g, Levelsp v, Levelp l, Thingp t, const bpoint at) -> bool
{
  TRACE();

  if (is_oob(at)) [[unlikely]] {
    return false;
  }

  //
  // Already at this location?
  //
  for (auto slot = 0; slot < MAP_SLOTS; slot++) {
    auto o_id = l->thing_id[ at.x ][ at.y ][ slot ];
    if (o_id == t->id) {
      return true;
    }
  }

  //
  // Need to push to the new location.
  //
  for (auto slot = 0; slot < MAP_SLOTS; slot++) {
    auto o_id = l->thing_id[ at.x ][ at.y ][ slot ];
    if (o_id == 0U) {
      l->thing_id[ at.x ][ at.y ][ slot ] = t->id;

      if (compiler_unused) {
        THING_DBG(g, v, l, t, "pushed to %u,%u slot %u", at.x, at.y, slot);
      }

      return true;
    }
  }

  return false;
}

//
// Push the thing onto the level
//
[[nodiscard]] static auto thing_push_internal(Gamep g, Levelsp v, Levelp l, Thingp t) -> bool
{
  TRACE();

  auto at = thing_at_no_owner(g, v, l, t);
  if (is_oob(at)) [[unlikely]] {
    return false;
  }

  //
  // Already at this location?
  //
  auto slot_count = 0;
  for (auto slot = 0; slot < MAP_SLOTS; slot++) {
    auto o_id = l->thing_id[ at.x ][ at.y ][ slot ];
    if (o_id == t->id) {
      return true;
    }
    if (o_id != 0U) {
      slot_count++;
    }
  }

  //
  // Try and keep some space for critical things like the player
  //
  if (slot_count > MAP_SLOTS - 2) {
    if (! thing_is_critical_to_gameplay(t)) {
      return false;
    }
  }

  //
  // Detach from the old location
  //
  (void) thing_pop(g, v, t);

  //
  // Need to push to the new location.
  //
  for (auto tries = 0; tries < MAP_SLOTS; tries++) {
    for (auto slot = 0; slot < MAP_SLOTS; slot++) {
      auto o_id = l->thing_id[ at.x ][ at.y ][ slot ];
      if (o_id == 0U) {
        auto *tp = thing_tp(t);

        //
        // Keep track of tiles the player has been on.
        //
        if (tp_is_player(tp)) {
          l->player_has_walked_tile[ at.x ][ at.y ] = 1U;
        }

        //
        // Save where we were pushed so we can pop the same location
        //
        thing_is_on_map_set(g, v, l, t);
        t->level_num                        = l->level_num;
        t->last_pushed_at                   = at;
        l->thing_id[ at.x ][ at.y ][ slot ] = t->id;

        if (compiler_unused) {
          THING_DBG(g, v, l, t, "pushed to %u,%u slot %u", at.x, at.y, slot);
        }

        return true;
      }
    }

    //
    // We failed to push this thing. Try to remove something and try again.
    // Try the lowest priority stuff first.
    //
    bool removed_one = false;

    for (auto slot = 0; slot < MAP_SLOTS; slot++) {
      auto o_id = l->thing_id[ at.x ][ at.y ][ slot ];
      if (o_id != 0U) {
        auto *it = thing_find(g, v, o_id);
        if ((it != nullptr) && thing_is_removable_on_err(it)) {
          if (thing_pop(g, v, it)) {
            THING_DBG(g, v, l, t, "removed from the map due to lack of slots");
            removed_one = true;
            break;
          }
        }
      }
    }

    if (removed_one) {
      continue;
    }

    //
    // We failed to pop this thing. Try to remove something and try again.
    // Try to remove any higher priority things if we're still stuck.
    //
    for (auto slot = 0; slot < MAP_SLOTS; slot++) {
      auto o_id = l->thing_id[ at.x ][ at.y ][ slot ];
      if (o_id != 0U) {
        auto *it = thing_find(g, v, o_id);
        if ((it != nullptr) && thing_is_dead(it) && thing_is_removable_when_dead_on_err(it)) {
          if (thing_pop(g, v, it)) {
            THING_DBG(g, v, l, t, "removed from the map due to lack of slots");
            removed_one = true;
            break;
          }
        }
      }
    }

    if (removed_one) {
      continue;
    }

    //
    // Give up
    //
    break;
  }

  return false;
}

[[nodiscard]] auto thing_push(Gamep g, Levelsp v, Levelp l, Thingp t) -> bool
{
  TRACE();

  auto at = thing_at_no_owner(g, v, l, t);
  if (is_oob(at)) [[unlikely]] {
    return false;
  }

  if (thing_push_internal(g, v, l, t)) {
    thing_on_pushed(g, v, l, t);
    return true;
  }

  //
  // Failed. Try pushing on an adjacent tile
  //
  static const std::vector< bpoint > all_deltas = {
      bpoint(0, -1),
      bpoint(-1, 0),
      bpoint(1, 0),
      bpoint(0, 1),
      bpoint(-1, -1),
      bpoint(-1, 1),
      bpoint(1, -1),
      bpoint(1, 1),

      //
      bpoint(-2, -2),
      bpoint(-2, -1),
      bpoint(-2, 0),
      bpoint(-2, 1),
      bpoint(-2, 2),
      //
      bpoint(-1, -2),
      bpoint(-1, -1),
      bpoint(-1, 0),
      bpoint(-1, 1),
      bpoint(-1, 2),
      //
      bpoint(0, -2),
      bpoint(0, -1),
      bpoint(0, 1),
      bpoint(0, 2),
      //
      bpoint(1, -2),
      bpoint(1, -1),
      bpoint(1, 0),
      bpoint(1, 1),
      bpoint(1, 2),
      //
      bpoint(2, -2),
      bpoint(2, -1),
      bpoint(2, 0),
      bpoint(2, 1),
      bpoint(2, 2),
  };

  for (const auto &d : all_deltas) {
    auto new_at = at + d;

    if (is_oob(new_at)) [[unlikely]] {
      continue;
    }

    if (level_is_obs_to_movement(g, v, l, new_at) != nullptr) {
      continue;
    }

    thing_at_set(g, v, l, t, new_at); // INTENTIONAL
    thing_at_set(g, v, l, t, new_at); // INTENTIONAL Doing it twice sets old_at too
    thing_moving_from_set(t, new_at);

    if (thing_push_internal(g, v, l, t)) {
      thing_on_pushed(g, v, l, t);
      return true;
    }
  }

  //
  // Last resort. Try anywhere.
  //
  auto tries  = 100;
  auto border = 1;
  while (tries-- > 0) {
    auto x = PCG_RANDOM_RANGE(border, MAP_WIDTH - border);
    auto y = PCG_RANDOM_RANGE(border, MAP_HEIGHT - border);

    auto new_at = bpoint(x, y);

    if (is_oob(new_at)) [[unlikely]] {
      continue;
    }

    if (level_is_obs_to_movement(g, v, l, new_at) != nullptr) {
      continue;
    }

    thing_at_set(g, v, l, t, new_at); // INTENTIONAL
    thing_at_set(g, v, l, t, new_at); // INTENTIONAL Doing it twice sets old_at too
    thing_moving_from_set(t, new_at);

    if (thing_push_internal(g, v, l, t)) {
      thing_on_pushed(g, v, l, t);
      return true;
    }
  }

  thing_err(g, v, l, t, "out of thing slots");

  //
  // Dump the contents of slots if we were unable to push
  //
  for (auto slot = 0; slot < MAP_SLOTS; slot++) {
    auto dump_id = l->thing_id[ at.x ][ at.y ][ slot ];
    if (dump_id != 0U) {
      auto *it = thing_find(g, v, dump_id);
      thing_con(g, v, l, it, "DUMP: is using slot %u", slot);
    }
  }

  return false;
}

//
// Pop an additional tile for the thing
//
[[nodiscard]] auto thing_pop_additional(Gamep g, Levelsp v, Levelp l, Thingp t, const bpoint at) -> bool
{
  TRACE();

  if (is_oob(at)) [[unlikely]] {
    return false;
  }

  for (auto slot = 0; slot < MAP_SLOTS; slot++) {
    auto o_id = l->thing_id[ at.x ][ at.y ][ slot ];
    if (o_id == t->id) {
      l->thing_id[ at.x ][ at.y ][ slot ] = 0;
      if (compiler_unused) {
        THING_DBG(g, v, l, t, "popped from slot %u", slot);
      }
      return true;
    }
  }

  return false;
}

//
// Pop the thing off the level
//
[[nodiscard]] auto thing_pop(Gamep g, Levelsp v, Thingp t) -> bool
{
  TRACE();

  auto *l = thing_level(g, v, t);
  if (l == nullptr) [[unlikely]] {
    return false;
  }

  //
  // Pop from where we were pushed
  //
  if (! thing_is_on_map(t)) {
    return false;
  }

  bpoint const at = t->last_pushed_at;

  if (compiler_unused) {
    THING_DBG(g, v, l, t, "is on the map, last pushed %u,%u", at.x, at.y);
  }

  if (is_oob(at)) [[unlikely]] {
    return false;
  }

  for (auto slot = 0; slot < MAP_SLOTS; slot++) {
    auto o_id = l->thing_id[ at.x ][ at.y ][ slot ];
    if (o_id == t->id) {
      l->thing_id[ at.x ][ at.y ][ slot ] = 0;
      if (compiler_unused) {
        THING_DBG(g, v, l, t, "popped from slot %u", slot);
      }
      thing_is_on_map_unset(g, v, l, t);
      thing_on_popped(g, v, l, t);
      return true;
    }
  }

  //
  // Dump the contents if we failed to pop
  //
  for (auto slot = 0; slot < MAP_SLOTS; slot++) {
    auto dump_id = l->thing_id[ at.x ][ at.y ][ slot ];
    if (dump_id != 0U) {
      auto *it = thing_find(g, v, dump_id);
      thing_con(g, v, l, it, "DUMP: is using slot %u", slot);
    }
  }

  thing_err(g, v, l, t, "could not pop thing that is on the map");
  return false;
}
