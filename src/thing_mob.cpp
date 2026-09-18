//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_level.hpp"
#include "my_level_inlines.hpp" // NOLINT
#include "my_main.hpp"
#include "my_thing.hpp"
#include "my_thing_inlines.hpp" // NOLINT
#include "my_tp.hpp"
#include "my_types.hpp"
#include <cstring>

//
// How many minions?
//
[[nodiscard]] auto thing_mob_minion_count_get(Gamep g, Levelsp v, Levelp l, Thingp mob) -> int
{
  TRACE();

  if (mob == nullptr) {
    return 0;
  }

  if (! thing_is_mob(mob)) {
    return 0;
  }

  auto *ext_struct = thing_ext_struct(g, v, mob);
  if (ext_struct == nullptr) {
    return 0;
  }

  return ext_struct->minions.count;
}

//
// Add a minion to the mob if possible
//
[[nodiscard]] auto thing_mob_spawn_a_minion(Gamep g, Levelsp v, Levelp l, Thingp mob, Tpp tp_minion) -> Thingp
{
  TRACE();

  if (mob == nullptr) {
    return nullptr;
  }

  if (! thing_is_mob(mob)) {
    thing_err(g, v, l, mob, "non mob trying to spawn minions");
    return nullptr;
  }

  if (tp_minion == nullptr) {
    thing_err(g, v, l, mob, "no minion to spawn");
    return nullptr;
  }

  //
  // Can't spawn over certain things
  //
  if (level_is_obs_to_spawning(g, v, l, thing_at(g, v, l, mob)) != nullptr) {
    return nullptr;
  }

  if (! tp_is_minion(tp_minion)) {
    thing_err(g, v, l, mob, "mob trying to spawn non minion %s", tp_name(tp_minion).c_str());
    return nullptr;
  }

  auto *ext_struct = thing_ext_struct(g, v, mob);
  if (ext_struct == nullptr) {
    thing_err(g, v, l, mob, "mob is missing ext struct");
    return nullptr;
  }

  //
  // Too many minions
  //
  if (thing_mob_minion_count_get(g, v, l, mob) >= thing_minion_max(g, v, l, mob)) {
    auto dormant_for = thing_dormant_max(mob);
    THING_DBG(g, v, l, mob, "already spawned max minions => dormant for %d", dormant_for);
    (void) thing_dormant_set(g, v, l, mob, dormant_for);
    return nullptr;
  }

  //
  // Look for a free slot
  //
  FOR_ALL_MINION_SLOTS(g, v, l, mob, slot, existing_minion)
  {
    if (existing_minion != nullptr) {
      continue;
    }

    //
    // Create the minion. Should be no chance to fail now.
    //
    auto *new_minion = thing_spawn(g, v, l, tp_minion, thing_at(g, v, l, mob));
    if (new_minion == nullptr) {
      return nullptr;
    }

    memset(slot, 0, sizeof(*slot));
    slot->minion_id    = new_minion->id;
    new_minion->mob_id = mob->id;
    ext_struct->minions.count++;

    THING_DBG(g, v, l, mob, "spawned minion %s (max %d)", to_string(g, v, l, new_minion).c_str(), thing_minion_max(g, v, l, mob));
    THING_DBG(g, v, l, new_minion, "new born minion");

    if (thing_mob_minion_count_get(g, v, l, mob) >= thing_minion_max(g, v, l, mob)) {
      auto dormant_for = thing_dormant_max(mob);
      THING_DBG(g, v, l, mob, "spawned max minions => dormant for %d", dormant_for);
      (void) thing_dormant_set(g, v, l, mob, dormant_for);
    }

    return new_minion;
  }

  //
  // Out of slots; but we checked above
  //
  thing_err(g, v, l, mob, "unexpectedly out of minion slots");

  return nullptr;
}

//
// Detach or kill all minions (or a specific one)
//
[[nodiscard]] static auto thing_mob_process_minions(Gamep g, Levelsp v, Levelp l, Thingp mob, Thingp specific_minion, ThingEvent &e) -> bool
{
  TRACE();

  if (mob == nullptr) {
    return false;
  }

  if (! thing_is_mob(mob)) {
    thing_err(g, v, l, mob, "non mob trying to detach minions");
    return false;
  }

  auto *ext_struct = thing_ext_struct(g, v, mob);
  if (ext_struct == nullptr) {
    return false;
  }

  FOR_ALL_MINION_SLOTS(g, v, l, mob, slot, minion)
  {
    if (minion == nullptr) {
      continue;
    }

    if (specific_minion != nullptr) {
      if (minion != specific_minion) {
        continue;
      }
    }

    if (! static_cast< bool >(minion->mob_id)) {
      thing_err(g, v, l, mob, "mob found detached minion: %s", to_string(g, v, l, minion).c_str());
      return false;
    }

    if (ext_struct->minions.count <= 0) {
      thing_err(g, v, l, mob, "mob has unexpected minion count when detaching: %s", to_string(g, v, l, minion).c_str());
      return false;
    }

    ext_struct->minions.count--;
    memset(slot, 0, sizeof(*slot));
    minion->mob_id = 0;

    if (e.event_type != THING_EVENT_NONE) {
      THING_DBG(g, v, l, mob, "kill minion %s", to_string(g, v, l, minion).c_str());
      TRACE_INDENT();
      thing_dead(g, v, l, minion, e);
    } else {
      THING_DBG(g, v, l, mob, "detach minion %s", to_string(g, v, l, minion).c_str());
    }
  }

  return true;
}

//
// Detach all minions from their mob
//
[[nodiscard]] auto thing_mob_detach_all_minions(Gamep g, Levelsp v, Levelp l, Thingp mob) -> bool
{
  TRACE();

  ThingEvent e = {};
  return thing_mob_process_minions(g, v, l, mob, nullptr, e);
}

//
// Kill all minions
//
[[nodiscard]] auto thing_mob_kill_all_minions(Gamep g, Levelsp v, Levelp l, Thingp mob, ThingEvent &e) -> bool
{
  TRACE();

  return thing_mob_process_minions(g, v, l, mob, nullptr, e);
}

//
// Detach a minion from its mob
//
[[nodiscard]] auto thing_mob_detach_minion(Gamep g, Levelsp v, Levelp l, Thingp mob, Thingp minion) -> bool
{
  TRACE();

  ThingEvent e = {};
  return thing_mob_process_minions(g, v, l, mob, minion, e);
}

//
// Dump all minions
//
void thing_mob_dump_minions(Gamep g, Levelsp v, Levelp l, Thingp mob)
{
  TRACE();

  if (mob == nullptr) {
    return;
  }

  if (! thing_is_mob(mob)) {
    thing_err(g, v, l, mob, "non mob trying to detach minion");
    return;
  }

  auto *ext_struct = thing_ext_struct(g, v, mob);
  if (ext_struct == nullptr) {
    return;
  }

  FOR_ALL_MINION_SLOTS(g, v, l, mob, slot, existing_minion)
  {
    if (existing_minion == nullptr) {
      THING_DBG(g, v, l, mob, "slot %d: -", _n_);
      continue;
    }

    auto s = to_string(g, v, l, existing_minion);
    THING_DBG(g, v, l, mob, "slot %d: %s", _n_, s.c_str());
  }
}

[[nodiscard]] auto thing_is_mob(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_mob) != 0;
}

[[nodiscard]] auto thing_is_mob1(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_mob1) != 0;
}

[[nodiscard]] auto thing_is_mob2(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_mob2) != 0;
}
