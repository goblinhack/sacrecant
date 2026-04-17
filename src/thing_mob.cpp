//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_thing_inlines.hpp"

#include <cstring>

//
// Howm many minions?
//
auto thing_mob_minion_count_get(Gamep g, Levelsp v, Levelp l, Thingp mob) -> int
{
  TRACE();

  if (mob == nullptr) {
    return 0;
  }

  if (! thing_is_mob(mob)) {
    return 0;
  }

  auto *ext_struct = thing_ext_struct(g, mob);
  if (ext_struct == nullptr) {
    return 0;
  }

  return ext_struct->minions.count;
}

//
// Add a minion to the mob if possible
//
auto thing_mob_spawn_a_minion(Gamep g, Levelsp v, Levelp l, Thingp mob, Tpp tp_minion) -> Thingp
{
  TRACE();

  if (mob == nullptr) {
    return nullptr;
  }

  if (! thing_is_mob(mob)) {
    thing_err(mob, "non mob trying to spawn minions");
    return nullptr;
  }

  if (tp_minion == nullptr) {
    thing_err(mob, "no minion to spawn");
    return nullptr;
  }

  //
  // Can't spawn over certain things
  //
  if (level_is_obs_to_spawning(g, v, l, thing_at(mob)) != nullptr) {
    return nullptr;
  }

  if (! tp_is_minion(tp_minion)) {
    thing_err(mob, "mob trying to spawn non minion %s", tp_name(tp_minion).c_str());
    return nullptr;
  }

  auto *ext_struct = thing_ext_struct(g, mob);
  if (ext_struct == nullptr) {
    thing_err(mob, "mob is missing ext struct");
    return nullptr;
  }

  //
  // Too many minions
  //
  if (thing_mob_minion_count_get(g, v, l, mob) >= thing_minion_max(mob)) {
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
    auto *new_minion = thing_spawn(g, v, l, tp_minion, thing_at(mob));
    if (new_minion == nullptr) {
      return nullptr;
    }

    memset(slot, 0, sizeof(*slot));
    slot->minion_id    = new_minion->id;
    new_minion->mob_id = mob->id;
    ext_struct->minions.count++;

    THING_DBG(mob, "spawned minion %s", to_string(g, v, l, new_minion).c_str());
    THING_DBG(new_minion, "new born minion");

    return new_minion;
  }

  //
  // Out of slots; but we checked above
  //
  thing_err(mob, "unexpectedly out of minion slots");

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
    thing_err(mob, "non mob trying to detach minions");
    return false;
  }

  auto *ext_struct = thing_ext_struct(g, mob);
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
      thing_err(mob, "mob found detached minion: %s", to_string(g, v, l, minion).c_str());
      return false;
    }

    if (ext_struct->minions.count <= 0) {
      thing_err(mob, "mob has unexpected minion count when detaching: %s", to_string(g, v, l, minion).c_str());
      return false;
    }

    ext_struct->minions.count--;
    memset(slot, 0, sizeof(*slot));
    minion->mob_id = 0;

    if (e.event_type != THING_EVENT_NONE) {
      THING_DBG(mob, "kill minion %s", to_string(g, v, l, minion).c_str());
      TRACE_INDENT();
      thing_dead(g, v, l, minion, e);
    } else {
      THING_DBG(mob, "detach minion %s", to_string(g, v, l, minion).c_str());
    }
  }

  return true;
}

//
// Detach all minions from their mob
//
auto thing_mob_detach_all_minions(Gamep g, Levelsp v, Levelp l, Thingp mob) -> bool
{
  TRACE();

  ThingEvent e = {};
  return thing_mob_process_minions(g, v, l, mob, nullptr, e);
}

//
// Kill all minions
//
auto thing_mob_kill_all_minions(Gamep g, Levelsp v, Levelp l, Thingp mob, ThingEvent &e) -> bool
{
  TRACE();

  return thing_mob_process_minions(g, v, l, mob, nullptr, e);
}

//
// Detach a minion from its mob
//
auto thing_mob_detach_minion(Gamep g, Levelsp v, Levelp l, Thingp mob, Thingp minion) -> bool
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
    thing_err(mob, "non mob trying to detach minion");
    return;
  }

  auto *ext_struct = thing_ext_struct(g, mob);
  if (ext_struct == nullptr) {
    return;
  }

  FOR_ALL_MINION_SLOTS(g, v, l, mob, slot, existing_minion)
  {
    if (existing_minion == nullptr) {
      THING_DBG(mob, "slot %d: -", _n_);
      continue;
    }

    auto s = to_string(g, v, l, existing_minion);
    THING_DBG(mob, "slot %d: %s", _n_, s.c_str());
  }
}
