//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_level_inlines.hpp"
#include "my_thing_inlines.hpp"

//
// Returns true/false on success/fail
//
auto thing_is_open_try_set(Gamep g, Levelsp v, Levelp l, Thingp t, Thingp opener, bool val) -> bool
{
  TRACE_DEBUG();

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
      t->_is_open = false;
      return false;
    }

    //
    // Reset animation
    //
    thing_anim_init(g, v, l, t, THING_ANIM_OPEN);
  } else {
    //
    // Try to close
    //
    if (! thing_on_close_request(g, v, l, t, opener)) {
      //
      // Close failed
      //
      t->_is_open = true;
      return false;
    }

    //
    // Reset animation
    //
    thing_anim_init(g, v, l, t, THING_ANIM_IDLE);
  }

  return true;
}

auto thing_is_open_try_unset(Gamep g, Levelsp v, Levelp l, Thingp t, Thingp closer) -> bool
{
  TRACE_DEBUG();

  return thing_is_open_try_set(g, v, l, t, closer, false);
}

auto thing_is_unlocked(Thingp t) -> bool
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
auto thing_open(Gamep g, Levelsp v, Levelp l, Thingp me, Thingp opener) -> bool
{
  TRACE();

  if (! thing_is_player(opener) && ! thing_is_monst(opener)) {
    thing_err(opener, "unexpected thing for %s", __FUNCTION__);
    return false;
  }

  if (! thing_is_openable(me)) {
    return false;
  }

  bool const success = thing_is_open_try_set(g, v, l, me, opener);
  if (success) {
    if (thing_is_player(opener)) {
      (void) level_tick_begin_requested(g, v, l, "player opened something");
      THING_DBG(opener, "opened %s", to_string(g, v, l, me).c_str());
    }
  }

  return success;
}

//
// Close doors
//
auto thing_close(Gamep g, Levelsp v, Levelp l, Thingp me, Thingp closer) -> bool
{
  TRACE();

  if (! thing_is_player(closer) && ! thing_is_monst(closer)) {
    thing_err(closer, "unexpected thing for %s", __FUNCTION__);
    return false;
  }

  if (! thing_is_openable(me)) {
    return false;
  }

  bool const success = thing_is_open_try_unset(g, v, l, me, closer);
  if (success) {
    if (thing_is_player(closer)) {
      (void) level_tick_begin_requested(g, v, l, "player closed something");
      THING_DBG(closer, "closed %s", to_string(g, v, l, me).c_str());
    }
  }

  return success;
}

//
// Returns true if we can move to this location by opening a door
//
auto thing_can_move_to_attempt_by_opening(Gamep g, Levelsp v, Levelp l, Thingp me, bpoint to) -> bool
{
  TRACE();

  if (is_oob_or_border(to)) [[unlikely]] {
    return false;
  }

  if (to == thing_at(me)) {
    return true;
  }

  auto at = thing_at(me);
  auto dx = to.x - at.x;
  auto dy = to.y - at.y;
  thing_set_dir_from_delta(me, dx, dy);

  if (! thing_is_able_to_open(me)) {
    (void) thing_lunge(g, v, l, me, to);
    return false;
  }

  //
  // Needs to be something we can shove
  //
  if (! level_is_openable(g, v, l, to)) {
    return false;
  }

  FOR_ALL_THINGS_AT(g, v, l, it, to)
  {
    //
    // A wall or pillar or somesuch?
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
