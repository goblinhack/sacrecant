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
// Returns true/false on success/fail
//
[[nodiscard]] auto thing_is_open_try_set(Gamep g, Levelsp v, Levelp l, Thingp t, Thingp opener, bool val) -> bool
{
  THING_DBG(g, v, l, t, "%s", __FUNCTION__);
  TRACE_INDENT();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }

  if (t->_is_open == static_cast< int >(val)) {
    return true;
  }
  t->_is_open = val;

  //
  // Attempt the open/close. It can fail.
  //
  if (val) {
    //
    // Try to open
    //
    if (! thing_on_open_request(g, v, l, t, opener)) {
      //
      // Open failed
      //
      THING_DBG(g, v, l, t, "open failed");
      t->_is_open = false;
      return false;
    }

    //
    // Reset animation
    //
    THING_DBG(g, v, l, t, "open success");
    thing_anim_init(g, v, l, t, THING_ANIM_OPEN);
  } else {
    //
    // Try to close
    //
    if (! thing_on_close_request(g, v, l, t, opener)) {
      //
      // Close failed
      //
      THING_DBG(g, v, l, t, "close failed");
      t->_is_open = true;
      return false;
    }

    //
    // Reset animation
    //
    THING_DBG(g, v, l, t, "close success");
    thing_anim_init(g, v, l, t, THING_ANIM_IDLE);
  }

  return true;
}

[[nodiscard]] auto thing_is_open_try_unset(Gamep g, Levelsp v, Levelp l, Thingp t, Thingp closer) -> bool
{
  TRACE_DEBUG();

  return thing_is_open_try_set(g, v, l, t, closer, false);
}

[[nodiscard]] auto thing_is_unlocked(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return t->_is_unlocked;
}

void thing_is_unlocked_set(Gamep g, Levelsp v, Levelp l, Thingp t, bool val)
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return;
  }
  t->_is_unlocked = val;
}

void thing_is_unlocked_unset(Gamep g, Levelsp v, Levelp l, Thingp t)
{
  TRACE_DEBUG();

  thing_is_unlocked_set(g, v, l, t, false);
}

//
// Open doors
//
[[nodiscard]] auto thing_open(Gamep g, Levelsp v, Levelp l, Thingp me, Thingp opener) -> bool
{
  THING_DBG(g, v, l, me, "%s", __FUNCTION__);
  TRACE_INDENT();

  if (! thing_is_player(opener) && ! thing_is_monst(opener)) {
    thing_err(g, v, l, opener, "unexpected thing for %s", __FUNCTION__);
    return false;
  }

  if (! thing_is_openable(me)) {
    return false;
  }

  bool const success = thing_is_open_try_set(g, v, l, me, opener);
  if (success) {
    if (thing_is_player(opener)) {
      (void) level_tick_begin_requested(g, v, l, "player opened something");
      THING_DBG(g, v, l, opener, "opened %s", to_string(g, v, l, me).c_str());
    }
  }

  return success;
}

//
// Close doors
//
[[nodiscard]] auto thing_close(Gamep g, Levelsp v, Levelp l, Thingp me, Thingp closer) -> bool
{
  THING_DBG(g, v, l, me, "%s", __FUNCTION__);
  TRACE_INDENT();

  if (! thing_is_player(closer) && ! thing_is_monst(closer)) {
    thing_err(g, v, l, closer, "unexpected thing for %s", __FUNCTION__);
    return false;
  }

  if (! thing_is_openable(me)) {
    return false;
  }

  bool const success = thing_is_open_try_unset(g, v, l, me, closer);
  if (success) {
    if (thing_is_player(closer)) {
      (void) level_tick_begin_requested(g, v, l, "player closed something");
      THING_DBG(g, v, l, closer, "closed %s", to_string(g, v, l, me).c_str());
    }
  }

  return success;
}

//
// Returns true if we can move to this location by opening a door
//
[[nodiscard]] auto thing_can_move_to_attempt_by_opening(Gamep g, Levelsp v, Levelp l, Thingp me, bpoint to) -> bool
{
  TRACE();

  if (is_oob_or_border(to)) [[unlikely]] {
    return false;
  }

  if (to == thing_at(g, v, l, me)) {
    return true;
  }

  auto at = thing_at(g, v, l, me);
  auto dx = to.x - at.x;
  auto dy = to.y - at.y;
  thing_set_dir_from_delta(g, v, l, me, dx, dy);

  if (! thing_is_able_to_open_things(me)) {
    return false;
  }

  //
  // Needs to be something we can open
  //
  if (! level_is_openable_bool(g, v, l, to)) {
    return false;
  }

  FOR_ALL_THINGS_AT(g, v, l, it, to)
  {
    //
    // A wall or pillar or some-such?
    //
    if (thing_is_obs_to_movement(it)) {
      //
      // But make exceptions for things like doors
      //
      if (thing_is_openable(it)) {
        if (thing_open(g, v, l, it, me)) {
          return true;
        }
      }

      return false;
    }
  }

  return true;
}
