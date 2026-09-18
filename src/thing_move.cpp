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
// Handles manual and mouse follow moves
//
[[nodiscard]] auto thing_move_to(Gamep g, Levelsp v, Levelp l, Thingp me, bpoint to) -> bool
{
  TRACE();

  if (is_oob_or_border(to)) [[unlikely]] {
    return false;
  }

  auto at = thing_at(g, v, l, me);
  if (to == at) {
    return false;
  }

  if (thing_is_wooden_leg(g, v, l, me)) {
    if (! thing_is_jumping(me)) {
      return thing_jump_to(g, v, l, me, to);
    }
  }

  //
  // We call moving-to when the move cannot fail
  //
  thing_on_moving_to(g, v, l, me, to);
  (void) thing_pop(g, v, me);

  spoint pix_at;
  pix_at.x = at.x * TILE_WIDTH;
  pix_at.y = at.y * TILE_HEIGHT;

  thing_pix_at_set(g, v, l, me, pix_at);
  thing_moving_from_set(me, at);
  thing_at_set(g, v, l, me, to);

  //
  // The move is complete now
  //
  (void) thing_push(g, v, l, me);
  thing_on_moved(g, v, l, me);

  //
  // Inerpolate the move
  //
  thing_is_moving_set(g, v, l, me);

  //
  // If we're an engulfer, move the unlucky engulfed things inside us along with us as we move
  //
  if (thing_is_able_to_engulf(me)) {
    FOR_ALL_THINGS_AT(g, v, l, it, at)
    {
      if (thing_is_engulfed(it)) {
        THING_DBG(g, v, l, it, "is engulfed and needs to follow the engulfer");

        if (! thing_move_to(g, v, l, it, to)) {
          THING_DBG(g, v, l, it, "is engulfed but could not be moved");
          (void) thing_is_engulfed_try_unset(g, v, l, it);
        }
      }
    }
  }

  return true;
}

//
// Handles shoving to a location. We can't move there yet.
//
[[nodiscard]] auto thing_shove_to(Gamep g, Levelsp v, Levelp l, Thingp me, bpoint to) -> bool
{
  TRACE();

  if (is_oob_or_border(to)) [[unlikely]] {
    return false;
  }

  auto at = thing_at(g, v, l, me);
  if (to == at) {
    return false;
  }

  auto ret = thing_shove_handle(g, v, l, me, to);
  if (ret) {
    THING_DBG(g, v, l, me, "shoved");
  } else {
    THING_DBG(g, v, l, me, "failed to shove");
  }

  return ret;
}

//
// Handles immediate moves even across levels.
//
// This is used for teleporting, falling etc...
//
[[nodiscard]] auto thing_warp_to(Gamep g, Levelsp v, Levelp new_level, Thingp me, bpoint to) -> bool
{
  TRACE();

  if (is_oob_or_border(to)) [[unlikely]] {
    thing_err(g, v, new_level, me, "warp to oob %d,%d", to.x, to.y);
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
  auto at = thing_at(g, v, thing_level(g, v, me), me);
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
    thing_move_finish(g, v, old_level, me);

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
  // For newly spawned things, like a beam_weapon weapon, we do not want to see interpolation else
  // the thing will glide over the level to the new location
  //
  thing_moving_from_set(me, to);

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

  THING_DBG(g, v, new_level, me, "moved to new location");

  return true;
}

//
// Move to the next tile is completed. Need to stop interpolating.
// There could be more tiles to pop.
//
void thing_move_finish(Gamep g, Levelsp v, Levelp l, Thingp me)
{
  TRACE();

  auto *player_struct = thing_player_struct(g);
  if (player_struct == nullptr) {
    return;
  }

  //
  // Need to complete the interpolation or the thing will appear to be on
  // the previous tile
  //
  if (thing_is_moving(me)) {
    thing_interpolate(g, v, l, me, 1.0);
  }
  me->thing_dt = 0.0;

  auto at = thing_at(g, v, l, me);
  thing_moving_from_set(me, at);

  thing_is_lunging_set(g, v, l, me, false);
  thing_is_teleporting_unset(g, v, l, me);
  thing_is_moving_unset(g, v, l, me);
  thing_is_jumping_unset(g, v, l, me);
  thing_is_thrown_unset(g, v, l, me, thing_owner(g, v, l, me));

  //
  // If we've gotten away from an engulfer, unset the state
  //
  if (! level_is_able_to_engulf_bool(g, v, l, at)) {
    (void) thing_is_engulfed_try_unset(g, v, l, me);
  }

  //
  // If we've gotten away from an ensarer, unset the state
  //
  if (! level_is_able_to_ensnare_bool(g, v, l, at)) {
    (void) thing_is_ensnared_try_unset(g, v, l, me);
  }

  thing_dmap(g, v, l, me);
  thing_update_pos(g, v, l, me);
}

//
// Post init/warp, we need to update the position so we can determine the level draw bounds
//
void thing_update_pos(Gamep g, Levelsp v, Levelp l, Thingp me)
{
  TRACE();

  auto   real_at = thing_real_at(g, v, l, me);
  spoint pix_at;
  pix_at.x = static_cast< int >(real_at.x * static_cast< float >(TILE_WIDTH));
  pix_at.y = static_cast< int >(real_at.y * static_cast< float >(TILE_HEIGHT));
  thing_pix_at_set(g, v, l, me, pix_at);

  //
  // Update hidden status
  //
  if ((thing_is_blit_when_obscured_faded(me) || thing_is_blit_when_obscured_outline(me))
      && level_alive_is_blit_obscures(g, v, l, thing_at(g, v, l, me)) != nullptr) {
    thing_is_hidden_set(g, v, l, me, true);
  } else {
    thing_is_hidden_set(g, v, l, me, false);
  }
}

[[nodiscard]] auto thing_move_remaining(Thingp t) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_move_remaining;
}

[[nodiscard]] auto thing_move_remaining_set(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_move_remaining = val;
}

[[nodiscard]] auto thing_move_remaining_incr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_move_remaining += val;
}

[[nodiscard]] auto thing_move_remaining_decr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  if (t->_move_remaining - val <= 0) {
    return t->_move_remaining = 0;
  }
  return t->_move_remaining -= val;
}

[[nodiscard]] auto thing_moving_from(Thingp t) -> bpoint
{
  TRACE_DEBUG();

  if (t == nullptr) {
    CROAK("no thing pointer");
  }
  return t->_moving_from;
}

void thing_moving_from_set(Thingp t, const bpoint &val)
{
  TRACE_DEBUG();

  if (t == nullptr) {
    CROAK("no thing pointer");
  }
  t->_moving_from = val;
}

[[nodiscard]] auto thing_is_obs_to_movement(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }

  //
  // Unless open
  //
  if (thing_is_openable(t)) {
    if (thing_is_open(t)) {
      return false;
    }
  }

  return tp_flag(thing_tp(t), is_obs_to_movement) != 0;
}

[[nodiscard]] auto thing_is_wall_walker(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_wall_walker) != 0;
}

[[nodiscard]] auto thing_is_able_to_move_diagonally(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_able_to_move_diagonally) != 0;
}

[[nodiscard]] auto thing_is_needs_move_confirm(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_needs_move_confirm) != 0;
}
