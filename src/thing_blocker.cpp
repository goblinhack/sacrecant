//
// Copyright goblinhack@gmail.com
//

#include "my_bpoint.hpp"
#include "my_callstack.hpp"
#include "my_fpoint.hpp"
#include "my_level.hpp"
#include "my_level_inlines.hpp"
#include "my_main.hpp"
#include "my_spoint.hpp"
#include "my_thing.hpp"
#include "my_thing_callbacks.hpp"
#include "my_thing_inlines.hpp"
#include "my_tile.hpp"
#include "my_tp.hpp"
#include "my_types.hpp"

//
// Prevent movement?
//
[[nodiscard]] auto thing_is_blocker_movement(Gamep g, Levelsp v, Levelp l, Thingp me) -> bool
{
  TRACE_DEBUG();

  if (me == nullptr) {
    no_thing_pointer();
    return false;
  }

  FOR_ALL_HOOKS(g, v, l, me, hook)
  {
    if (thing_is_blocker_movement(g, v, l, hook)) {
      return true;
    }
  }

  FOR_ALL_ACTIVE_ITEMS(g, v, l, me, item)
  {
    if (thing_is_blocker_movement(g, v, l, item)) {
      return true;
    }
  }

  return tp_flag(thing_tp(me), is_blocker_movement) != 0;
}

//
// Prevent falling?
//
[[nodiscard]] auto thing_is_blocker_falling(Gamep g, Levelsp v, Levelp l, Thingp me) -> bool
{
  TRACE_DEBUG();

  if (me == nullptr) {
    no_thing_pointer();
    return false;
  }

  FOR_ALL_HOOKS(g, v, l, me, hook)
  {
    if (thing_is_blocker_falling(g, v, l, hook)) {
      return true;
    }
  }

  FOR_ALL_ACTIVE_ITEMS(g, v, l, me, item)
  {
    if (thing_is_blocker_falling(g, v, l, item)) {
      return true;
    }
  }

  return tp_flag(thing_tp(me), is_blocker_falling) != 0;
}

//
// Prevent teleport?
//
[[nodiscard]] auto thing_is_blocker_teleport(Gamep g, Levelsp v, Levelp l, Thingp me) -> bool
{
  TRACE_DEBUG();

  if (me == nullptr) {
    no_thing_pointer();
    return false;
  }

  FOR_ALL_HOOKS(g, v, l, me, hook)
  {
    if (thing_is_blocker_teleport(g, v, l, hook)) {
      return true;
    }
  }

  FOR_ALL_ACTIVE_ITEMS(g, v, l, me, item)
  {
    if (thing_is_blocker_teleport(g, v, l, item)) {
      return true;
    }
  }

  return tp_flag(thing_tp(me), is_blocker_teleport) != 0;
}

//
// Prevent being ensnared?
//
[[nodiscard]] auto thing_is_blocker_ensnared(Gamep g, Levelsp v, Levelp l, Thingp me) -> bool
{
  TRACE_DEBUG();

  if (me == nullptr) {
    no_thing_pointer();
    return false;
  }

  FOR_ALL_HOOKS(g, v, l, me, hook)
  {
    if (thing_is_blocker_ensnared(g, v, l, hook)) {
      return true;
    }
  }

  FOR_ALL_ACTIVE_ITEMS(g, v, l, me, item)
  {
    if (thing_is_blocker_ensnared(g, v, l, item)) {
      return true;
    }
  }

  return tp_flag(thing_tp(me), is_blocker_ensnared) != 0;
}

//
// Prevent being engulfed?
//
[[nodiscard]] auto thing_is_blocker_engulfed(Gamep g, Levelsp v, Levelp l, Thingp me) -> bool
{
  TRACE_DEBUG();

  if (me == nullptr) {
    no_thing_pointer();
    return false;
  }

  FOR_ALL_HOOKS(g, v, l, me, hook)
  {
    if (thing_is_blocker_engulfed(g, v, l, hook)) {
      return true;
    }
  }

  FOR_ALL_ACTIVE_ITEMS(g, v, l, me, item)
  {
    if (thing_is_blocker_engulfed(g, v, l, item)) {
      return true;
    }
  }

  return tp_flag(thing_tp(me), is_blocker_engulfed) != 0;
}
