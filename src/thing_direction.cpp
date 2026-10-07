//
// Copyright goblinhack@gmail.com
//

#include "my_bpoint.hpp"
#include "my_callstack.hpp"
#include "my_fpoint.hpp"
#include "my_main.hpp"
#include "my_thing.hpp"
#include "my_thing_inlines.hpp"
#include "my_tp.hpp"
#include "my_types.hpp"

#include <cmath>

//
// Get thing direction
//
[[nodiscard]] auto thing_is_dir_br(Thingp me) -> bool { return (me->dir == THING_DIR_BR); }
[[nodiscard]] auto thing_is_dir_tr(Thingp me) -> bool { return (me->dir == THING_DIR_TR); }
[[nodiscard]] auto thing_is_dir_bl(Thingp me) -> bool { return (me->dir == THING_DIR_BL); }
[[nodiscard]] auto thing_is_dir_tl(Thingp me) -> bool { return (me->dir == THING_DIR_TL); }
[[nodiscard]] auto thing_is_dir_right(Thingp me) -> bool { return (me->dir == THING_DIR_RIGHT); }
[[nodiscard]] auto thing_is_dir_down(Thingp me) -> bool { return (me->dir == THING_DIR_DOWN); }
[[nodiscard]] auto thing_is_dir_up(Thingp me) -> bool { return (me->dir == THING_DIR_UP); }
[[nodiscard]] auto thing_is_dir_left(Thingp me) -> bool { return (me->dir == THING_DIR_LEFT); }

void thing_is_moving_set(Gamep g, Levelsp v, Levelp l, Thingp t, bool val)
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return;
  }

  if (t->_is_moving == static_cast< int >(val)) {
    return;
  }
  t->_is_moving = val;

  //
  // Just in case we are still mid lunge, stop it.
  //
  thing_is_lunging_set(g, v, l, t, false);
}

void thing_is_moving_unset(Gamep g, Levelsp v, Levelp l, Thingp t)
{
  TRACE_DEBUG();

  thing_is_moving_set(g, v, l, t, false);
}

//
// Set thing direction
//
static void thing_dir_set_down(Thingp me)
{
  TRACE();

  if (tp_is_animated_no_dir(thing_tp(me))) {
    return;
  }

  if (me->dir != THING_DIR_DOWN) {
    me->dir = THING_DIR_DOWN;
    // move_carried_items();
  }
}

//
// Set thing direction
//
static void thing_dir_set_up(Thingp me)
{
  TRACE();

  if (tp_is_animated_no_dir(thing_tp(me))) {
    return;
  }

  if (me->dir != THING_DIR_UP) {
    me->dir = THING_DIR_UP;
    // move_carried_items();
  }
}

//
// Set thing direction
//
static void thing_dir_set_left(Thingp me)
{
  TRACE();

  if (tp_is_animated_no_dir(thing_tp(me))) {
    return;
  }

  if (me->dir != THING_DIR_LEFT) {
    me->dir = THING_DIR_LEFT;
    // move_carried_items();
  }
}

//
// Set thing direction
//
static void thing_dir_set_right(Thingp me)
{
  TRACE();

  if (tp_is_animated_no_dir(thing_tp(me))) {
    return;
  }

  if (me->dir != THING_DIR_RIGHT) {
    me->dir = THING_DIR_RIGHT;
    // move_carried_items();
  }
}

//
// Set thing direction
//
static void thing_dir_set_tl(Thingp me)
{
  TRACE();

  if (tp_is_animated_no_dir(thing_tp(me))) {
    return;
  }

  if (me->dir != THING_DIR_TL) {
    me->dir = THING_DIR_TL;
    // move_carried_items();
  }
}

//
// Set thing direction
//
static void thing_dir_set_bl(Thingp me)
{
  TRACE();

  if (tp_is_animated_no_dir(thing_tp(me))) {
    return;
  }

  if (me->dir != THING_DIR_BL) {
    me->dir = THING_DIR_BL;
    // move_carried_items();
  }
}

//
// Set thing direction
//
static void thing_dir_set_tr(Thingp me)
{
  TRACE();

  if (tp_is_animated_no_dir(thing_tp(me))) {
    return;
  }

  if (me->dir != THING_DIR_TR) {
    me->dir = THING_DIR_TR;
    // move_carried_items();
  }
}

//
// Set thing direction
//
static void thing_dir_set_br(Thingp me)
{
  TRACE();

  if (tp_is_animated_no_dir(thing_tp(me))) {
    return;
  }

  if (me->dir != THING_DIR_BR) {
    me->dir = THING_DIR_BR;
    // move_carried_items();
  }
}

//
// Set tile direction from delta
//
void thing_set_dir_from_delta(Gamep g, Levelsp v, Levelp l, Thingp me, int dx, int dy)
{
  if (compiler_unused) {
    THING_DBG(g, v, l, me, "set dir from delta %d,%d", dx, dy);
  }
  TRACE();

  if (dx < 0) {
    if (dy > 0) {
      thing_dir_set_bl(me);
    } else if (dy < 0) {
      thing_dir_set_tl(me);
    } else {
      thing_dir_set_left(me);
    }
    return;
  }

  if (dx > 0) {
    if (dy > 0) {
      thing_dir_set_br(me);
    } else if (dy < 0) {
      thing_dir_set_tr(me);
    } else {
      thing_dir_set_right(me);
    }
    return;
  }

  if (dy > 0) {
    if (dx > 0) {
      thing_dir_set_br(me);
    } else if (dx < 0) {
      thing_dir_set_bl(me);
    } else {
      thing_dir_set_down(me);
    }
    return;
  }

  if (dy < 0) {
    if (dx > 0) {
      thing_dir_set_tr(me);
    } else if (dx < 0) {
      thing_dir_set_tl(me);
    } else {
      thing_dir_set_up(me);
    }
    return;
  }
}

void thing_set_dir_from_delta(Gamep g, Levelsp v, Levelp l, Thingp me, const bpoint &p)
{
  TRACE();
  thing_set_dir_from_delta(g, v, l, me, p.x, p.y);
}

void thing_set_dir_from_target(Gamep g, Levelsp v, Levelp l, Thingp me, const bpoint &p)
{
  TRACE();
  auto at = thing_at(g, v, l, me);
  thing_set_dir_from_delta(g, v, l, me, p.x - at.x, p.y - at.y);
}

//
// Get direction; need to also account for projectiles that move at an angle.
//
[[nodiscard]] auto thing_get_direction(Gamep g, Levelsp v, Levelp l, Thingp me) -> fpoint
{
  TRACE();

  if (thing_is_projectile(me) || thing_is_beam_weapon(me)) {
    return thing_missile_get_direction(g, v, l, me);
  }

  switch (me->dir) {
    case THING_DIR_BR :    return fpoint(1, 1);
    case THING_DIR_TR :    return fpoint(1, -1);
    case THING_DIR_BL :    return fpoint(-1, 1);
    case THING_DIR_TL :    return fpoint(-1, -1);
    case THING_DIR_RIGHT : return fpoint(1, 0);
    case THING_DIR_NONE :  return fpoint(0, 0);
    case THING_DIR_DOWN :  return fpoint(0, 1);
    case THING_DIR_UP :    return fpoint(0, -1);
    case THING_DIR_LEFT :  return fpoint(-1, 0);
    default :              return fpoint(0, 0);
  }
}

//
// Get direction; need to also account for projectiles that move at an angle.
//
[[nodiscard]] auto thing_get_direction_grid(Gamep g, Levelsp v, Levelp l, Thingp me) -> bpoint
{
  TRACE();

  if (thing_is_projectile(me) || thing_is_beam_weapon(me)) {
    //
    // Convert to a grid direction. This is very rough.
    //
    fpoint dir = thing_missile_get_direction(g, v, l, me);
    bpoint dir_out;

    dir.x = static_cast< int >(std::round(dir.x));
    dir.y = static_cast< int >(std::round(dir.y));

    if (dir.x > 0) {
      dir_out.x = 1;
    } else if (dir.x < 0) {
      dir_out.x = -1;
    }
    if (dir.y > 0) {
      dir_out.y = 1;
    } else if (dir.y < 0) {
      dir_out.y = -1;
    }

    return dir_out;
  }

  switch (me->dir) {
    case THING_DIR_BR :    return bpoint(1, 1);
    case THING_DIR_TR :    return bpoint(1, -1);
    case THING_DIR_BL :    return bpoint(-1, 1);
    case THING_DIR_TL :    return bpoint(-1, -1);
    case THING_DIR_RIGHT : return bpoint(1, 0);
    case THING_DIR_NONE :  return bpoint(0, 0);
    case THING_DIR_DOWN :  return bpoint(0, 1);
    case THING_DIR_UP :    return bpoint(0, -1);
    case THING_DIR_LEFT :  return bpoint(-1, 0);
    default :              return bpoint(0, 0);
  }
}
