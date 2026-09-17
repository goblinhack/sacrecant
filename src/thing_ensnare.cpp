//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_level.hpp"
#include "my_level_inlines.hpp" // NOLINT
#include "my_main.hpp"
#include "my_thing.hpp"
#include "my_thing_inlines.hpp"
#include "my_tp.hpp"
#include "my_types.hpp"

//
// Who is ensnaring me?
//
[[nodiscard]] auto thing_ensnarer(Gamep g, Levelsp v, Levelp l, Thingp me) -> Thingp
{
  TRACE();

  if (me == nullptr) {
    ERR("no thing pointer");
    return nullptr;
  }

  if (! thing_is_ensnared(me)) {
    return nullptr;
  }

  auto at = thing_at(g, v, l, me);

  FOR_ALL_THINGS_AT(g, v, l, it, at)
  {
    if (it == me) {
      continue;
    }

    if (thing_is_able_to_ensnare(it)) {
      return it;
    }
  }

  return nullptr;
}

//
// Returns true/false on success/fail
//
[[nodiscard]] static auto thing_is_ensnared_try_set(Gamep g, Levelsp v, Levelp l, Thingp me, Thingp ensnarer, bool val = true) -> bool
{
  if (me == nullptr) {
    ERR("no thing pointer");
    return false;
  }

  if (me->_is_ensnared == static_cast< int >(val)) {
    return true;
  }
  me->_is_ensnared = val;

  if (val) {
    THING_DBG(g, v, l, ensnarer, "ensnarer");
    TRACE_INDENT();
    THING_DBG(g, v, l, me, "ensnared");
    TRACE_INDENT();
  } else {
    THING_DBG(g, v, l, me, "un-ensnared");
  }

  //
  // Reset animation
  //
  thing_anim_init(g, v, l, me, THING_ANIM_IDLE);

  return true;
}

[[nodiscard]] auto thing_is_ensnared_try_unset(Gamep g, Levelsp v, Levelp l, Thingp me) -> bool
{
  TRACE_DEBUG();

  return thing_is_ensnared_try_set(g, v, l, me, nullptr, false);
}

//
// Ensnare things
//
[[nodiscard]] auto thing_ensnare(Gamep g, Levelsp v, Levelp l, Thingp me, Thingp ensnarer) -> bool
{
  THING_DBG(g, v, l, ensnarer, "wants to ensnare");
  TRACE_INDENT();
  THING_DBG(g, v, l, me, "me");

  if (thing_is_ensnared(me)) {
    THING_DBG(g, v, l, me, "already ensnared");
    return false;
  }

  if (! thing_is_player(ensnarer) && ! thing_is_monst(ensnarer) && ! thing_is_spiderweb(ensnarer)) {
    thing_err(g, v, l, ensnarer, "unexpected thing for %s", __FUNCTION__);
    return false;
  }

  if (! thing_is_able_to_ensnare(ensnarer)) {
    THING_DBG(g, v, l, ensnarer, "cannot ensnare");
    return false;
  }

  if (! thing_is_able_to_be_ensnared(g, v, l, me)) {
    THING_DBG(g, v, l, me, "cannot be ensnared");
    return false;
  }

  bool const success = thing_is_ensnared_try_set(g, v, l, me, ensnarer);
  if (success) {
    if (thing_is_player(ensnarer)) {
      (void) level_tick_begin_requested(g, v, l, "player ensnared something");
      THING_DBG(g, v, l, ensnarer, "ensnared %s", to_string(g, v, l, me).c_str());
    }
  }

  return success;
}

//
// Unensnare
//
[[nodiscard]] auto thing_unensnare(Gamep g, Levelsp v, Levelp l, Thingp me, Thingp ensnarer) -> bool
{
  THING_DBG(g, v, l, me, "%s", __FUNCTION__);
  TRACE_INDENT();

  if (! thing_is_player(ensnarer) && ! thing_is_monst(ensnarer)) {
    thing_err(g, v, l, ensnarer, "unexpected thing for %s", __FUNCTION__);
    return false;
  }

  if (! thing_is_able_to_be_ensnared(g, v, l, me)) {
    return false;
  }

  if (! thing_is_able_to_ensnare(ensnarer)) {
    return false;
  }

  if (! thing_is_ensnared(me)) {
    return false;
  }

  bool const success = thing_is_ensnared_try_unset(g, v, l, me);
  if (success) {
    if (thing_is_player(ensnarer)) {
      (void) level_tick_begin_requested(g, v, l, "player closed something");
      THING_DBG(g, v, l, ensnarer, "closed %s", to_string(g, v, l, me).c_str());
    }
  }

  return success;
}

[[nodiscard]] auto thing_is_able_to_ensnare(Thingp me) -> bool
{
  TRACE_DEBUG();

  if (me == nullptr) {
    ERR("no thing pointer");
    return false;
  }

  if (thing_is_dead(me)) {
    return false;
  }

  return tp_flag(thing_tp(me), is_able_to_ensnare) != 0;
}

[[nodiscard]] auto thing_is_able_to_be_ensnared(Gamep g, Levelsp v, Levelp l, Thingp me) -> bool
{
  TRACE_DEBUG();

  if (me == nullptr) {
    ERR("no thing pointer");
    return false;
  }

  if (thing_is_ethereal(g, v, l, me)) {
    return false;
  }

  if (thing_is_levitating(g, v, l, me)) {
    return false;
  }

  FOR_ALL_HOOKS(g, v, l, me, buff)
  {
    if (tp_flag(thing_tp(buff), is_able_to_be_ensnared_blocked) != 0) {
      return false;
    }
  }

  //
  // More specifically for spider boots
  //
  FOR_ALL_ACTIVE_ITEMS(g, v, l, me, item)
  {
    if (tp_flag(thing_tp(item), is_able_to_be_ensnared_blocked) != 0) {
      return false;
    }
  }

  return tp_flag(thing_tp(me), is_able_to_be_ensnared) != 0;
}
