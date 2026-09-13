//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_main.hpp"
#include "my_thing.hpp"
#include "my_thing_callbacks.hpp"
#include "my_thing_inlines.hpp"
#include "my_tp.hpp"
#include "my_types.hpp"

#include <cstring>

//
// Dump all hooks
//
static void thing_dump_buffs(Gamep g, Levelsp v, Levelp l, Thingp me)
{
  TRACE();

  if (me == nullptr) {
    return;
  }

  if (! thing_is_able_to_be_buffed(me)) {
    thing_log(g, v, l, me, "non owner trying to detach hook");
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
[[nodiscard]] static auto thing_hook_count_get(Gamep g, Levelsp v, Thingp me) -> int
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

  return ext_struct->hooks.count;
}

//
// Does the hook exist already?
//
[[nodiscard]] static auto thing_hook_find(Gamep g, Levelsp v, Levelp l, Thingp me, Tpp what) -> Thingp
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
// Prioritize hooks.
//
static void thing_hook_sort(Gamep g, Levelsp v, Levelp l, Thingp me)
{
  TRACE();

  if (me == nullptr) {
    return;
  }

  auto *ext_struct = thing_ext_struct(g, v, me);
  if (ext_struct == nullptr) {
    thing_err(g, v, l, me, "missing ext struct");
    return;
  }

  ThingHooks new_buffs = {};
  int        count {};

  FOR_ALL_HOOKS_SLOTS(g, v, l, me, slot, hook)
  {
    if (hook != nullptr) {
      if (thing_is_sacrifice(hook)) {
        new_buffs.hook[ count++ ] = *slot;
        *slot                     = {};
      }
    }
  }

  FOR_ALL_HOOKS_SLOTS(g, v, l, me, slot, hook)
  {
    if (hook != nullptr) {
      if (thing_is_boost(hook)) {
        new_buffs.hook[ count++ ] = *slot;
        *slot                     = {};
      }
    }
  }

  FOR_ALL_HOOKS_SLOTS(g, v, l, me, slot, hook)
  {
    if (hook != nullptr) {
      if (thing_is_buff(hook)) {
        new_buffs.hook[ count++ ] = *slot;
        *slot                     = {};
      }
    }
  }

  FOR_ALL_HOOKS_SLOTS(g, v, l, me, slot, hook)
  {
    if (hook != nullptr) {
      if (thing_is_debuff(hook)) {
        new_buffs.hook[ count++ ] = *slot;
        *slot                     = {};
      }
    }
  }

  new_buffs.count = count;

  if (ext_struct->hooks.count != count) {
    thing_dump_buffs(g, v, l, me);
    thing_err(g, v, l, me, "sorting hooks failed");
  }

  ext_struct->hooks = new_buffs;
}

//
// Add a hook if possible
//
[[nodiscard]] auto thing_hook_add(Gamep g, Levelsp v, Levelp l, Thingp me, Tpp what) -> Thingp
{
  TRACE();

  if (me == nullptr) {
    return nullptr;
  }

  if (! thing_is_able_to_be_buffed(me)) {
    thing_log(g, v, l, me, "thing trying to add hooks when it cannot");
    return nullptr;
  }

  if (what == nullptr) {
    thing_err(g, v, l, me, "no hook to add");
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
  auto *existing_buff = thing_hook_find(g, v, l, me, what);
  if (existing_buff != nullptr) {
    //
    // Merge a new tp hook into an existing thing hook instead of creating a new hook
    //
    THING_DBG(g, v, l, existing_buff, "enhance existing hook");
    thing_enhance(g, v, l, existing_buff, what);
    return existing_buff;
  }

  //
  // Too many hooks
  //
  if (thing_hook_count_get(g, v, me) >= THING_HOOK_MAX) {
    THING_DBG(g, v, l, me, "trying to apply too many hooks");
    thing_dump_buffs(g, v, l, me);

    if (thing_is_player(me)) {
      topcon("Trying to apply too many hooks!");
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
    // Create the hook. Should be no chance to fail now.
    //
    auto *new_hook = thing_spawn(g, v, l, what, thing_at(g, v, l, me));
    if (new_hook == nullptr) {
      return nullptr;
    }

    memset(slot, 0, sizeof(*slot));
    slot->hook_id           = new_hook->id;
    new_hook->hook_owner_id = me->id;
    ext_struct->hooks.count++;

    THING_DBG(g, v, l, me, "added hook %s", to_string(g, v, l, new_hook).c_str());
    THING_DBG(g, v, l, new_hook, "new born hook");

    thing_hook_sort(g, v, l, me);

    thing_hook_on_attached(g, v, l, new_hook);

    return new_hook;
  }

  //
  // Out of slots; but we checked above
  //
  thing_err(g, v, l, me, "unexpectedly out of hook slots");

  return nullptr;
}

//
// Is this hook attached to a thing?
//
[[nodiscard]] auto thing_hook_owner_get(Gamep g, Levelsp v, Levelp l, Thingp me) -> Thingp
{
  if (me == nullptr) {
    return nullptr;
  }

  if (me->hook_owner_id == 0U) {
    return nullptr;
  }

  return thing_find(g, v, me->hook_owner_id);
}

//
// Detach or kill all hooks (or a specific one)
//
[[nodiscard]] static auto thing_hook_process_all(Gamep g, Levelsp v, Levelp l, Thingp me, Thingp specific_hook, ThingEvent &e) -> bool
{
  TRACE();

  if (me == nullptr) {
    return false;
  }

  if (! thing_is_able_to_be_buffed(me)) {
    thing_log(g, v, l, me, "non owner trying to detach hooks");
    return false;
  }

  auto *ext_struct = thing_ext_struct(g, v, me);
  if (ext_struct == nullptr) {
    return false;
  }

  bool got_one = false;

  FOR_ALL_HOOKS_SLOTS(g, v, l, me, slot, hook)
  {
    if (hook == nullptr) {
      continue;
    }

    if (specific_hook != nullptr) {
      if (hook != specific_hook) {
        continue;
      }
    }

    if (! static_cast< bool >(hook->hook_owner_id)) {
      thing_err(g, v, l, me, "found detached hook: %s", to_string(g, v, l, hook).c_str());
      return false;
    }

    if (ext_struct->hooks.count <= 0) {
      thing_err(g, v, l, me, "has unexpected hook count when detaching: %s", to_string(g, v, l, hook).c_str());
      return false;
    }

    ext_struct->hooks.count--;
    memset(slot, 0, sizeof(*slot));
    hook->hook_owner_id = 0;

    if (e.event_type != THING_EVENT_NONE) {
      THING_DBG(g, v, l, me, "kill hook %s", to_string(g, v, l, hook).c_str());
      TRACE_INDENT();
      thing_dead(g, v, l, hook, e);
      got_one = true;
    } else {
      THING_DBG(g, v, l, me, "detach hook %s", to_string(g, v, l, hook).c_str());
      got_one = true;
    }
  }

  if (! got_one) {
    if (specific_hook != nullptr) {
      THING_DBG(g, v, l, specific_hook, "could not detach hook");
      TRACE_INDENT();
      THING_DBG(g, v, l, me, "from me");
    } else if (ext_struct->hooks.count != 0) {
      THING_DBG(g, v, l, me, "could not detach hook");
    }
  }

  return got_one;
}

//
// Detach all hooks from their owner
//
[[nodiscard]] auto thing_hook_detach_all(Gamep g, Levelsp v, Levelp l, Thingp me) -> bool
{
  TRACE();

  ThingEvent e = {};
  return thing_hook_process_all(g, v, l, me, nullptr, e);
}

//
// Detach a hook from its owner
//
static auto thing_hook_detach_from_owner(Gamep g, Levelsp v, Levelp l, Thingp me, Thingp hook) -> bool
{
  TRACE();

  ThingEvent e = {};
  return thing_hook_process_all(g, v, l, me, hook, e);
}

//
// Detach a hook from its owner
//
[[nodiscard]] auto thing_hook_detach_me_from_owner(Gamep g, Levelsp v, Levelp l, Thingp me) -> bool
{
  TRACE();

  if (me == nullptr) {
    return false;
  }

  if (! thing_is_buff(me)) {
    thing_err(g, v, l, me, "non hook trying to detach itself");
    return false;
  }

  auto *hook_owner = thing_hook_owner_get(g, v, l, me);
  if (hook_owner == nullptr) {
    return false; // can be normal if detached
  }

  THING_DBG(g, v, l, me, "detach me from owner");
  TRACE_INDENT();

  return thing_hook_detach_from_owner(g, v, l, hook_owner, me);
}

void thing_hook_on_attached_set(Tpp tp, thing_hook_on_attached_t callback)
{
  TRACE();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return;
  }
  tp->hook_on_attached = callback;
}

void thing_hook_on_attached(Gamep g, Levelsp v, Levelp l, Thingp me)
{
  TRACE();
  auto *tp = thing_tp(me);
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return;
  }
  if (tp->hook_on_attached == nullptr) {
    return;
  }
  tp->hook_on_attached(g, v, l, me);
}
