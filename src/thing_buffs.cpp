//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_main.hpp"
#include "my_thing.hpp"
#include "my_thing_inlines.hpp"
#include "my_tp.hpp"
#include "my_types.hpp"

#include <cstring>

//
// Dump all buffs
//
static void thing_dump_buffs(Gamep g, Levelsp v, Levelp l, Thingp me)
{
  TRACE();

  if (me == nullptr) {
    return;
  }

  if (! thing_is_able_to_be_buffed(me)) {
    thing_log(g, v, l, me, "non owner trying to detach buff");
    return;
  }

  auto *ext_struct = thing_ext_struct(g, v, me);
  if (ext_struct == nullptr) {
    return;
  }

  FOR_ALL_HOOKS_SLOTS(g, v, l, me, slot, a_buff)
  {
    if (a_buff == nullptr) {
      THING_DBG(g, v, l, me, "slot %d: -", _n_);
      continue;
    }

    auto s = to_string(g, v, l, a_buff);
    THING_DBG(g, v, l, me, "slot %d: %s", _n_, s.c_str());
  }
}

//
// How many projectils?
//
[[nodiscard]] static auto thing_buff_count_get(Gamep g, Levelsp v, Thingp me) -> int
{
  if (me == nullptr) {
    return 0;
  }

  if (! thing_is_able_to_be_buffed(me)) {
    return 0;
  }

  auto *ext_struct = thing_ext_struct(g, v, me);
  if (ext_struct == nullptr) {
    return 0;
  }

  return ext_struct->buffs.count;
}

//
// Does the buff exist already?
//
[[nodiscard]] static auto thing_buff_find(Gamep g, Levelsp v, Levelp l, Thingp me, Tpp what) -> Thingp
{
  TRACE();

  if (me == nullptr) {
    return nullptr;
  }

  if (! thing_is_able_to_be_buffed(me)) {
    return nullptr;
  }

  if (what == nullptr) {
    return nullptr;
  }

  auto *ext_struct = thing_ext_struct(g, v, me);
  if (ext_struct == nullptr) {
    return nullptr;
  }

  FOR_ALL_HOOKS(g, v, l, me, a_buff)
  {
    if (thing_tp(a_buff) == what) {
      return a_buff;
    }
  }

  return nullptr;
}

//
// Add a buff if possible
//
[[nodiscard]] auto thing_buff_add(Gamep g, Levelsp v, Levelp l, Thingp me, Tpp what) -> Thingp
{
  TRACE();

  if (me == nullptr) {
    return nullptr;
  }

  if (! thing_is_able_to_be_buffed(me)) {
    thing_log(g, v, l, me, "thing trying to add buffs when it cannot");
    return nullptr;
  }

  if (what == nullptr) {
    thing_err(g, v, l, me, "no buff to add");
    return nullptr;
  }

  auto *ext_struct = thing_ext_struct(g, v, me);
  if (ext_struct == nullptr) {
    thing_err(g, v, l, me, "missing ext struct");
    return nullptr;
  }

  //
  // One of this type exists already?
  //
  auto *existing_buff = thing_buff_find(g, v, l, me, what);
  if (existing_buff != nullptr) {
    //
    // Merge a new tp buff into an existing thing buff instead of creating a new buff
    //
    THING_DBG(g, v, l, existing_buff, "enhance existing buff");
    thing_enhance(g, v, l, existing_buff, what);
    return existing_buff;
  }

  //
  // Too many buffs
  //
  if (thing_buff_count_get(g, v, me) >= THING_BUFF_MAX) {
    THING_DBG(g, v, l, me, "trying to apply too many buffs");
    thing_dump_buffs(g, v, l, me);

    if (thing_is_player(me)) {
      topcon("Trying to apply too many buffs!");
    }

    return nullptr;
  }

  //
  // Look for a free slot
  //
  FOR_ALL_HOOKS_SLOTS(g, v, l, me, slot, a_buff)
  {
    if (a_buff != nullptr) {
      continue;
    }

    //
    // Create the buff. Should be no chance to fail now.
    //
    auto *new_buff = thing_spawn(g, v, l, what, thing_at(g, v, l, me));
    if (new_buff == nullptr) {
      return nullptr;
    }

    memset(slot, 0, sizeof(*slot));
    slot->buff_id           = new_buff->id;
    new_buff->buff_owner_id = me->id;
    ext_struct->buffs.count++;

    THING_DBG(g, v, l, me, "added buff %s", to_string(g, v, l, new_buff).c_str());
    THING_DBG(g, v, l, new_buff, "new born buff");

    return new_buff;
  }

  //
  // Out of slots; but we checked above
  //
  thing_err(g, v, l, me, "unexpectedly out of buff slots");

  return nullptr;
}

//
// Is this buff attached to a thing?
//
[[nodiscard]] auto thing_buff_owner_get(Gamep g, Levelsp v, Levelp l, Thingp me) -> Thingp
{
  if (me == nullptr) {
    return nullptr;
  }

  if (me->buff_owner_id == 0U) {
    return nullptr;
  }

  return thing_find(g, v, me->buff_owner_id);
}

//
// Detach or kill all buffs (or a specific one)
//
[[nodiscard]] static auto thing_buff_process_all(Gamep g, Levelsp v, Levelp l, Thingp me, Thingp specific_buff, ThingEvent &e) -> bool
{
  TRACE();

  if (me == nullptr) {
    return false;
  }

  if (! thing_is_able_to_be_buffed(me)) {
    thing_log(g, v, l, me, "non owner trying to detach buffs");
    return false;
  }

  auto *ext_struct = thing_ext_struct(g, v, me);
  if (ext_struct == nullptr) {
    return false;
  }

  bool got_one = false;

  FOR_ALL_HOOKS_SLOTS(g, v, l, me, slot, buff)
  {
    if (buff == nullptr) {
      continue;
    }

    if (specific_buff != nullptr) {
      if (buff != specific_buff) {
        continue;
      }
    }

    if (! static_cast< bool >(buff->buff_owner_id)) {
      thing_err(g, v, l, me, "found detached buff: %s", to_string(g, v, l, buff).c_str());
      return false;
    }

    if (ext_struct->buffs.count <= 0) {
      thing_err(g, v, l, me, "has unexpected buff count when detaching: %s", to_string(g, v, l, buff).c_str());
      return false;
    }

    ext_struct->buffs.count--;
    memset(slot, 0, sizeof(*slot));
    buff->buff_owner_id = 0;

    if (e.event_type != THING_EVENT_NONE) {
      THING_DBG(g, v, l, me, "kill buff %s", to_string(g, v, l, buff).c_str());
      TRACE_INDENT();
      thing_dead(g, v, l, buff, e);
      got_one = true;
    } else {
      THING_DBG(g, v, l, me, "detach buff %s", to_string(g, v, l, buff).c_str());
      got_one = true;
    }
  }

  if (! got_one) {
    THING_DBG(g, v, l, me, "could not detach");
  }

  return got_one;
}

//
// Detach all buffs from their owner
//
[[nodiscard]] auto thing_buff_detach_all(Gamep g, Levelsp v, Levelp l, Thingp me) -> bool
{
  TRACE();

  ThingEvent e = {};
  return thing_buff_process_all(g, v, l, me, nullptr, e);
}

//
// Detach a buff from its owner
//
static auto thing_buff_detach_from_owner(Gamep g, Levelsp v, Levelp l, Thingp me, Thingp buff) -> bool
{
  TRACE();

  ThingEvent e = {};
  return thing_buff_process_all(g, v, l, me, buff, e);
}

//
// Detach a buff from its owner
//
[[nodiscard]] auto thing_buff_detach_me_from_owner(Gamep g, Levelsp v, Levelp l, Thingp me) -> bool
{
  TRACE();

  if (me == nullptr) {
    return false;
  }

  if (! thing_is_buff(me)) {
    thing_err(g, v, l, me, "non buff trying to detach itself");
    return false;
  }

  auto *buff_owner = thing_buff_owner_get(g, v, l, me);
  if (buff_owner == nullptr) {
    return false; // can be normal if detached
  }

  THING_DBG(g, v, l, me, "detach me from owner");
  TRACE_INDENT();

  return thing_buff_detach_from_owner(g, v, l, buff_owner, me);
}
