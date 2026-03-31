//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_level_inlines.hpp"
#include "my_thing_callbacks.hpp"
#include "my_thing_inlines.hpp"

//
// Get thing direction
//
auto thing_is_dir_br(Thingp me) -> bool { return (me->dir == THING_DIR_BR); }
auto thing_is_dir_tr(Thingp me) -> bool { return (me->dir == THING_DIR_TR); }
auto thing_is_dir_bl(Thingp me) -> bool { return (me->dir == THING_DIR_BL); }
auto thing_is_dir_tl(Thingp me) -> bool { return (me->dir == THING_DIR_TL); }
auto thing_is_dir_right(Thingp me) -> bool { return (me->dir == THING_DIR_RIGHT); }
auto thing_is_dir_down(Thingp me) -> bool { return (me->dir == THING_DIR_DOWN); }
auto thing_is_dir_up(Thingp me) -> bool { return (me->dir == THING_DIR_UP); }
auto thing_is_dir_left(Thingp me) -> bool { return (me->dir == THING_DIR_LEFT); }

void thing_is_moving_set(Gamep g, Levelsp v, Levelp l, Thingp t, bool val)
{
  TRACE_DEBUG();

  if (t == nullptr) {
    err("no thing pointer");
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

  if (val) {
    thing_on_moved(g, v, l, t);
  }
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
void thing_set_dir_from_delta(Thingp me, int dx, int dy)
{
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

//
// Get direction; need to also account for projectiles that move at an angle.
//
auto thing_get_direction(Gamep g, Levelsp v, Levelp l, Thingp me) -> fpoint
{
  TRACE();

  if (thing_is_projectile(me)) {
    return thing_projectile_get_direction(g, v, l, me);
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
auto thing_get_direction_grid(Gamep g, Levelsp v, Levelp l, Thingp me) -> bpoint
{
  TRACE();

  if (thing_is_projectile(me)) {
    thing_err(me, "cannot return grid direction");
    return bpoint(0, 0);
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

//
// Handles manual and mouse follow moves
//
auto thing_move_to(Gamep g, Levelsp v, Levelp l, Thingp me, bpoint to) -> bool
{
  TRACE();

  if (is_oob_or_border(to)) [[unlikely]] {
    return false;
  }

  auto at = thing_at(me);
  if (to == at) {
    return false;
  }

  (void) thing_pop(g, v, me);

  spoint pix_at;
  pix_at.x = at.x * TILE_WIDTH;
  pix_at.y = at.y * TILE_HEIGHT;

  thing_pix_at_set(g, v, l, me, pix_at);
  thing_moving_from_set(me, at);
  thing_at_set(g, v, l, me, to);
  (void) thing_push(g, v, l, me);

  thing_is_moving_set(g, v, l, me);
  THING_DBG(me, "is moving to set");

  return true;
}

//
// Handles shoving to a location. We can'me move there yet.
//
auto thing_shove_to(Gamep g, Levelsp v, Levelp l, Thingp me, bpoint to) -> bool
{
  TRACE();

  if (is_oob_or_border(to)) [[unlikely]] {
    return false;
  }

  auto at = thing_at(me);
  if (to == at) {
    return false;
  }

  auto ret = thing_shove_handle(g, v, l, me, to);
  if (ret) {
    THING_DBG(me, "shoved");
  } else {
    THING_DBG(me, "failed to shove");
  }

  return ret;
}

//
// Handles immediate moves even across levels.
//
// This is used for teleporting, falling etc...
//
auto thing_warp_to(Gamep g, Levelsp v, Levelp new_level, Thingp me, bpoint to) -> bool
{
  TRACE();

  if (is_oob_or_border(to)) [[unlikely]] {
    return false;
  }

  bool  level_changed = false;
  auto *old_level     = thing_level(g, v, me);

  //
  // Need to reset vision when leaving a level
  //
  if (old_level != new_level) {
    level_changed = true;
  }

  if (level_changed) {
    thing_on_level_leave(g, v, old_level, me);
  }

  //
  // Check if already present at the destination.
  //
  auto at = thing_at(me);
  if ((new_level == old_level) && (to == at)) {
    //
    // No need to pop. If might be an inventory item though, so make sure and push
    // it onto the map after this check.
    //
  } else {
    //
    // If the player is following a path and we teleport warp, then we want to stop
    // following that path.
    //
    if (thing_is_player(me)) {
      player_state_change(g, v, old_level, PLAYER_STATE_NORMAL);
    }

    //
    // Complete the current move
    //
    thing_move_or_jump_finish(g, v, old_level, me);

    //
    // Remove from the current level or position
    //
    (void) thing_pop(g, v, me);
  }

  spoint pix_at;
  pix_at.x = at.x * TILE_WIDTH;
  pix_at.y = at.y * TILE_HEIGHT;

  thing_pix_at_set(g, v, new_level, me, pix_at);
  thing_at_set(g, v, new_level, me, to);

  //
  // Join the level.
  //
  (void) thing_push(g, v, new_level, me);

  //
  // Need to update with the new pixel position
  //
  thing_update_pos(g, v, new_level, me);

  //
  // For a time period e.g. post teleport, we want to ignore mouse moves until the player is
  // centered once more.
  //
  if (thing_is_player(me)) {
    level_forced_auto_scroll(g, v, new_level);
  }

  //
  // Enter the new level callback. Do this after updating position so that light update works.
  //
  if (level_changed) {
    thing_on_level_enter(g, v, new_level, me);
  }

  //
  // In case of level change or teleport, need to update things
  //
  if (thing_is_player(me)) {
    //
    // Update minimaps and lighting
    //
    level_update_visibility(g, v, new_level);
  }

  new_level->is_tick_requested = true;

  THING_DBG(me, "moved to new location");

  return true;
}

//
// Move to the next tile is completed. Need to stop interpolating.
// There could be more tiles to pop.
//
void thing_move_or_jump_finish(Gamep g, Levelsp v, Levelp l, Thingp me)
{
  TRACE();

  auto *player_struct = thing_player_struct(g);
  if (player_struct == nullptr) {
    return;
  }

  if (thing_is_dead(me)) {
    THING_DBG(me, "finish moving when dead");
  }

  //
  // Need to complete the interpolation or the thing will appear to be on
  // the previous tile
  //
  if (thing_is_moving(me)) {
    thing_interpolate(g, v, l, me, 1.0);
  }
  me->thing_dt = 0.0;

  auto at = thing_at(me);
  thing_moving_from_set(me, at);

  thing_is_lunging_set(g, v, l, me, false);
  thing_is_teleporting_unset(g, v, l, me);
  thing_is_moving_unset(g, v, l, me);
  thing_is_jumping_unset(g, v, l, me);
  thing_dmap(g, v, l, me);
  thing_update_pos(g, v, l, me);
}

//
// Post init/warp, we need to update the position so we can determine the level draw bounds
//
void thing_update_pos(Gamep g, Levelsp v, Levelp l, Thingp me)
{
  TRACE();

  auto   real_at = thing_real_at(me);
  spoint pix_at;
  pix_at.x = static_cast< int >(real_at.x * static_cast< float >(TILE_WIDTH));
  pix_at.y = static_cast< int >(real_at.y * static_cast< float >(TILE_HEIGHT));
  thing_pix_at_set(g, v, l, me, pix_at);

  //
  // Update submerged status
  //
  thing_submerged_update(g, v, l, me);

  //
  // Update hidden status
  //
  if ((thing_is_blit_when_obscured_faded(me) || thing_is_blit_when_obscured_outline(me))
      && level_alive_is_blit_obscures(g, v, l, thing_at(me)) != nullptr) {
    thing_is_hidden_set(g, v, l, me, true);
  } else {
    thing_is_hidden_set(g, v, l, me, false);
  }
}
