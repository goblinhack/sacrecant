//
// Copyright goblinhack@gmail.com
//

#include "my_bpoint.hpp"
#include "my_callstack.hpp"
#include "my_level.hpp"
#include "my_level_inlines.hpp"
#include "my_main.hpp"
#include "my_thing.hpp"
#include "my_thing_callbacks.hpp"
#include "my_thing_inlines.hpp"
#include "my_tp.hpp"
#include "my_types.hpp"

//
// Who is engulfing me?
//
[[nodiscard]] auto thing_engulfer(Gamep g, Levelsp v, Levelp l, Thingp me) -> Thingp
{
  TRACE();

  if (me == nullptr) {
    ERR("no thing pointer");
    return nullptr;
  }

  if (! thing_is_engulfed(me)) {
    return nullptr;
  }

  auto at = thing_at(g, v, l, me);

  FOR_ALL_THINGS_AT(g, v, l, it, at)
  {
    if (it == me) {
      continue;
    }

    if (thing_is_able_to_engulf(it)) {
      return it;
    }
  }

  return nullptr;
}

//
// Returns true/false on success/fail
//
[[nodiscard]] static auto thing_is_engulfed_try_set(Gamep g, Levelsp v, Levelp l, Thingp me, Thingp engulfer, bool val = true) -> bool
{
  if (me == nullptr) {
    ERR("no thing pointer");
    return false;
  }

  if (me->_is_engulfed == static_cast< int >(val)) {
    return true;
  }
  me->_is_engulfed = val;

  //
  // Attempt the engulf. It can fail.
  //
  if (val) {
    //
    // Try to engulf
    //
    THING_DBG(g, v, l, engulfer, "engulfer");
    TRACE_INDENT();
    THING_DBG(g, v, l, me, "to be engulfed");
    TRACE_INDENT();

    if (! thing_on_engulf_request(g, v, l, me, engulfer)) {
      //
      // Engulf failed
      //
      THING_DBG(g, v, l, me, "engulf failed");
      me->_is_engulfed = false;
      return false;
    }

    //
    // Reset animation
    //
    THING_DBG(g, v, l, me, "engulf success");
    thing_anim_init(g, v, l, me, THING_ANIM_IDLE);
  } else {
    //
    // Un-engulfed
    //
    THING_DBG(g, v, l, me, "un-engulf success");
    thing_anim_init(g, v, l, me, THING_ANIM_IDLE);
  }

  return true;
}

[[nodiscard]] auto thing_is_engulfed_try_unset(Gamep g, Levelsp v, Levelp l, Thingp me) -> bool
{
  TRACE_DEBUG();

  return thing_is_engulfed_try_set(g, v, l, me, nullptr, false);
}

//
// Engulf things
//
[[nodiscard]] static auto thing_engulf(Gamep g, Levelsp v, Levelp l, Thingp me, Thingp engulfer) -> bool
{
  THING_DBG(g, v, l, engulfer, "wants to engulf");
  TRACE_INDENT();
  THING_DBG(g, v, l, me, "me");

  if (thing_is_engulfed(me)) {
    THING_DBG(g, v, l, me, "already engulfed");
    return false;
  }

  if (! thing_is_player(engulfer) && ! thing_is_monst(engulfer)) {
    thing_err(g, v, l, engulfer, "unexpected thing for %s", __FUNCTION__);
    return false;
  }

  if (! thing_is_able_to_engulf(engulfer)) {
    THING_DBG(g, v, l, engulfer, "cannot engulf");
    return false;
  }

  if (! thing_is_able_to_be_engulfed(g, v, l, me)) {
    THING_DBG(g, v, l, me, "cannot be engulfed");
    return false;
  }

  if (thing_is_collectable(me)) {
    ThingEvent e {
        .reason     = "auto collected",    //
        .event_type = THING_EVENT_CARRIED, //
        .source     = me,                  //
    };

    if (thing_carry(g, v, l, engulfer, me, e)) {
      THING_DBG(g, v, l, me, "collect instead of engulfing");
      return true;
    }

    THING_DBG(g, v, l, me, "could not collect");
    return false;
  }

  bool const success = thing_is_engulfed_try_set(g, v, l, me, engulfer);
  if (success) {
    if (thing_is_player(engulfer)) {
      (void) level_tick_begin_requested(g, v, l, "player engulfed something");
      THING_DBG(g, v, l, engulfer, "engulfed %s", to_string(g, v, l, me).c_str());
    }
  }

  return success;
}

//
// Unengulf
//
[[nodiscard]] auto thing_unengulf(Gamep g, Levelsp v, Levelp l, Thingp me, Thingp engulfer) -> bool
{
  THING_DBG(g, v, l, me, "%s", __FUNCTION__);
  TRACE_INDENT();

  if (! thing_is_player(engulfer) && ! thing_is_monst(engulfer)) {
    thing_err(g, v, l, engulfer, "unexpected thing for %s", __FUNCTION__);
    return false;
  }

  if (! thing_is_able_to_be_engulfed(g, v, l, me)) {
    return false;
  }

  if (! thing_is_able_to_engulf(engulfer)) {
    return false;
  }

  if (! thing_is_engulfed(me)) {
    return false;
  }

  bool const success = thing_is_engulfed_try_unset(g, v, l, me);
  if (success) {
    if (thing_is_player(engulfer)) {
      (void) level_tick_begin_requested(g, v, l, "player closed something");
      THING_DBG(g, v, l, engulfer, "closed %s", to_string(g, v, l, me).c_str());
    }
  }

  return success;
}

//
// Returns true if we can move to this location by engulfing
//
[[nodiscard]] auto thing_can_move_to_attempt_by_engulfing(Gamep g, Levelsp v, Levelp l, Thingp me, bpoint to) -> bool
{
  TRACE();

  if (is_oob_or_border(to)) [[unlikely]] {
    return false;
  }

  if (! thing_is_able_to_engulf(me)) {
    return false;
  }

  if (to == thing_at(g, v, l, me)) {
    return true;
  }

  auto at = thing_at(g, v, l, me);
  auto dx = to.x - at.x;
  auto dy = to.y - at.y;
  thing_set_dir_from_delta(g, v, l, me, dx, dy);

  THING_DBG(g, v, l, me, "try to engulf");
  TRACE_INDENT();

  //
  // If there is an engulfer there already, don't pile on
  //
  if (level_is_able_to_engulf(g, v, l, to) != nullptr) {
    THING_DBG(g, v, l, me, "try to engulf: engulfer exists there already");
    return false;
  }

  //
  // Needs to be something we can engulf
  //
  if (level_is_treasure_bool(g, v, l, to) && thing_is_able_to_eat_treasure(me)) {
    //
    // Can be eaten
    //
    THING_DBG(g, v, l, me, "try to engulf: yes, can be eaten");
  } else if (level_is_item_bool(g, v, l, to) && thing_is_able_to_eat_items(me)) {
    //
    // Can be eaten
    //
    THING_DBG(g, v, l, me, "try to engulf: yes, can be eaten");
  } else if (level_is_able_to_be_engulfed_bool(g, v, l, to)) {
    //
    // Can be engulfed
    //
    THING_DBG(g, v, l, me, "try to engulf: yes, can be engulfed");
  } else {
    //
    // Cannot engulf/eat
    //
    THING_DBG(g, v, l, me, "try to engulf: not possible");
    return false;
  }

  FOR_ALL_THINGS_AT(g, v, l, it, to)
  {
    if (thing_is_treasure(it)) {
      if (thing_is_able_to_eat_treasure(me)) {
        if (thing_engulf(g, v, l, it, me)) {
          return true;
        }
      }
    }

    if (thing_is_item(it)) {
      if (thing_is_able_to_eat_items(me)) {
        if (thing_engulf(g, v, l, it, me)) {
          return true;
        }
      }
    }

    //
    // A wall or pillar or somesuch?
    //
    if (thing_is_obs_to_movement(it)) {
      if (thing_is_able_to_be_engulfed(g, v, l, it)) {
        if (thing_engulf(g, v, l, it, me)) {
          return true;
        }
      }
    }
  }

  return false;
}

[[nodiscard]] auto thing_is_able_to_engulf(Thingp me) -> bool
{
  TRACE_DEBUG();

  if (me == nullptr) {
    ERR("no thing pointer");
    return false;
  }

  if (thing_is_dead(me)) {
    return false;
  }

  return tp_flag(thing_tp(me), is_able_to_engulf) != 0;
}

[[nodiscard]] auto thing_is_able_to_be_engulfed(Gamep g, Levelsp v, Levelp l, Thingp me) -> bool
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
    if (tp_flag(thing_tp(buff), is_able_to_be_engulfed_blocked) != 0) {
      return false;
    }
  }

  FOR_ALL_ACTIVE_ITEMS(g, v, l, me, item)
  {
    if (tp_flag(thing_tp(item), is_able_to_be_engulfed_blocked) != 0) {
      return false;
    }
  }

  return tp_flag(thing_tp(me), is_able_to_be_engulfed) != 0;
}
