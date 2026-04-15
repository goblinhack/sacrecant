//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_game.hpp"
#include "my_globals.hpp"
#include "my_level_inlines.hpp"
#include "my_line.hpp"
#include "my_main.hpp"
#include "my_thing_callbacks.hpp"
#include "my_thing_inlines.hpp"
#include "my_wid_warning.hpp"

#include <ranges>

void thing_player_init(Gamep g)
{
  TRACE();

  auto *v = game_levels_get(g);
  if (v == nullptr) {
    ERR("no levels");
    return;
  }

  if (v->player_id == 0U) {
    ERR("no player");
    return;
  }
  auto *l = game_level_get(g, v);
  if (l == nullptr) {
    return;
  }

  player_state_change(g, v, l, PLAYER_STATE_NORMAL);
}

auto thing_player(Gamep g) -> Thingp
{
  TRACE();

  auto *v = game_levels_get(g);
  if (v == nullptr) {
    return nullptr;
  }

  if (v->player_id == 0U) {
    return nullptr;
  }

  return thing_find(g, v, v->player_id);
}

auto thing_player_level(Gamep g) -> Levelp
{
  TRACE();

  auto *me = thing_player(g);
  if (me == nullptr) {
    return nullptr;
  }

  auto *v = game_levels_get(g);
  if (v == nullptr) {
    return nullptr;
  }

  return game_level_get(g, v, me->level_num);
}

//
// Replace the mouse path upon mouse down events
//
[[nodiscard]] static auto thing_player_replace_current_mouse_path(Gamep g, Levelsp v, Levelp l) -> bool
{
  TRACE();

  //
  // Need to recreate the path first, as the me may have moved since the last mouse move
  //
  level_cursor_path_recreate(g, v, l);

  player_state_change(g, v, l, PLAYER_STATE_PATH_REQUESTED);

  //
  // Apply the new path
  //
  level_cursor_copy_mouse_path_to_player(g, v, l);

  return player_check_if_target_needs_move_confirm(g, v, l, v->cursor_at);
}

//
// Pass the event to things for consumption e.g. doors
//
[[nodiscard]] static auto thing_player_pass_event_to_other_things(Gamep g, Levelsp v, Levelp l, int x, int y, uint32_t button) -> bool
{
  TRACE();

  FOR_ALL_THINGS_AT(g, v, l, it, v->cursor_at)
  {
    if (thing_mouse_down(g, v, l, it, x, y, button)) {
      //
      // Processed an event, like door closing
      //
      return true;
    }
  }

  return false;
}

//
// Return true on the event being consumed
//
auto thing_player_mouse_down(Gamep g, Levelsp v, Levelp l, int x, int y, uint32_t button) -> bool
{
  log("thing mouse down");
  TRACE_INDENT();

  switch (player_state(g, v)) {
    case PLAYER_STATE_INIT :
      //
      // Player not initialized yet
      //
      break;
    case PLAYER_STATE_DEAD :
      //
      // No player mouse events when dead
      //
      break;
    case PLAYER_STATE_NORMAL :
      //
      // Give a chance to open/close doors first.
      //
      if (thing_player_pass_event_to_other_things(g, v, l, x, y, button)) {
        break;
      }

      //
      // Replace the mouse path
      //
      (void) thing_player_replace_current_mouse_path(g, v, l);
      break;
    case PLAYER_STATE_PATH_REQUESTED :
      //
      // Player wants to start following or replace the current path.
      //
      break;
    case PLAYER_STATE_MOVE_CONFIRM_REQUESTED :
      //
      // Wait for confirmation.
      //
      break;
    case PLAYER_STATE_FOLLOWING_PATH :
      //
      // Already following a path. Allow the me to change the path.
      //
      (void) thing_player_replace_current_mouse_path(g, v, l);
      break;
    case PLAYER_STATE_ENUM_MAX : break;
  }

  //
  // Suppress mouse click
  //
  return false;
}

//
// Called per display loop
//
static void thing_player_cursor_loop(Gamep g, Levelsp v, Levelp l)
{
  TRACE();

  //
  // If the cursor moved, update what we see
  //
  if (game_request_to_update_cursor_get(g)) {
    level_cursor_path_recreate(g, v, l);
    level_cursor_describe(g, v, l);
    game_request_to_update_cursor_unset(g);
  }
}

//
// Called per display loop
//
void thing_player_event_loop(Gamep g, Levelsp v, Levelp l)
{
  TRACE();

  switch (game_state(g)) {
    case STATE_INIT :      break;
    case STATE_MAIN_MENU : break;
    case STATE_QUITTING :  break;
    case STATE_PLAYING :
      //
      // If the me pressed the mouse, we need to apply the current cursor path and start moving.
      //
      switch (player_state(g, v)) {
        case PLAYER_STATE_INIT :
          //
          // Player not initialized yet
          //
          if (g_opt_level_select_menu) {
            thing_player_cursor_loop(g, v, l);
          }
          break;
        case PLAYER_STATE_DEAD :
          //
          // No moving when dead
          //
          break;
        case PLAYER_STATE_NORMAL :
          //
          // If the cursor moved, update what we see
          //
          thing_player_cursor_loop(g, v, l);
          break;
        case PLAYER_STATE_PATH_REQUESTED :
          //
          // Player wants to start following or replace the current path.
          //
          level_cursor_copy_mouse_path_to_player(g, v, l);
          break;
        case PLAYER_STATE_MOVE_CONFIRM_REQUESTED :
          //
          // Wait for confirmation.
          //
          break;
        case PLAYER_STATE_FOLLOWING_PATH :
          //
          // Already following a path. Allow the me to mouse around looking for
          // a better path while moving.
          //
          thing_player_cursor_loop(g, v, l);
          break;
        case PLAYER_STATE_ENUM_MAX : break;
      }
      break;
    case STATE_DEAD_MENU :
      //
      // If the cursor moved, update what we see
      //
      thing_player_cursor_loop(g, v, l);
      break;
    case STATE_MOVE_WARNING_MENU : break;
    case STATE_KEYBOARD_MENU :     break;
    case STATE_LOAD_MENU :         break;
    case STATE_LOADED :            break;
    case STATE_SAVE_MENU :         break;
    case STATE_QUIT_MENU :         break;
    case STATE_INVENTORY_MENU :    break;
    case STATE_ITEM_MENU :         break;
    case STATE_GENERATING :        break;
    case STATE_GENERATED :         break;
    case GAME_STATE_ENUM_MAX :     break;
  }
}

auto player_state_to_string(PlayerState state) -> std::string
{
  TRACE();
  return PlayerState_to_string(state);
}

auto player_state(Gamep g, Levelsp v) -> PlayerState
{
  TRACE();
  return v->_player_state;
}

void player_state_change(Gamep g, Levelsp v, Levelp l, PlayerState new_state)
{
  TRACE();

  auto *me = thing_player(g);
  if (me == nullptr) {
    return;
  }

  auto old_state = v->_player_state;

  if (v->_player_state == new_state) {
    return;
  }

  switch (old_state) {
    case PLAYER_STATE_INIT :
      //
      // Player not initialized yet
      //
      player_move_requests_reset(g, v);
      break;
    case PLAYER_STATE_DEAD :
      //
      // Player is dead.
      //
      return;
    case PLAYER_STATE_NORMAL :
      //
      // Replace the mouse path
      //
      break;
    case PLAYER_STATE_PATH_REQUESTED :
      //
      // Player wants to start following or replace the current path.
      //
      break;
    case PLAYER_STATE_MOVE_CONFIRM_REQUESTED :
      //
      // Wait for confirmation.
      //
      break;
    case PLAYER_STATE_FOLLOWING_PATH :
      //
      // Already following a path, stick to it until completion.
      //
      break;
    case PLAYER_STATE_ENUM_MAX : break;
  }

  //
  // Set here to stop recursion.
  //
  v->_player_state = new_state;

  //
  // Why oh why change state
  //
  THING_DBG(me, "player state change: %s -> %s", player_state_to_string(old_state).c_str(), player_state_to_string(new_state).c_str());
  TRACE_INDENT();

  switch (new_state) {
    case PLAYER_STATE_INIT :
      //
      // Player not initialized yet
      //
      break;
    case PLAYER_STATE_DEAD :
      //
      // No need of a mouse path when dead.
      //
      level_cursor_path_reset(g, v, l);

      player_move_requests_reset(g, v);
      break;
    case PLAYER_STATE_NORMAL :
      //
      // If we're hit while moving, we return to this state and so need to clear the mouse path
      //
      level_cursor_path_reset(g, v, l);

      if (old_state == PLAYER_STATE_FOLLOWING_PATH) {
        //
        // Only auto scroll at the start and end of moving
        //
        v->requested_auto_scroll = true;
      }
      break;
    case PLAYER_STATE_PATH_REQUESTED :
      //
      // Player wants to start following or replace the current path.
      //
      break;
    case PLAYER_STATE_MOVE_CONFIRM_REQUESTED :
      //
      // Wait for confirmation.
      //
      break;
    case PLAYER_STATE_FOLLOWING_PATH :
      //
      // Already following a path, stick to it until completion.
      //
      if (old_state != PLAYER_STATE_FOLLOWING_PATH) {
        //
        // Only auto scroll at the start and end of moving
        //
        v->requested_auto_scroll = true;
      }
      break;
    case PLAYER_STATE_ENUM_MAX : break;
  }
}

//
// Return true on the event being consumed
//
static void player_check_if_target_needs_move_confirm_callback(Gamep g, bool val)
{
  if (val) {
    game_state_change(g, STATE_PLAYING, "callback: got 'Yes' warning confirmation");
  } else {
    game_state_change(g, STATE_PLAYING, "callback: got 'No' warning confirmation");
  }

  auto *v = game_levels_get(g);
  if (v == nullptr) {
    return;
  }

  auto *l = game_level_get(g, v);
  if (l == nullptr) {
    return;
  }

  auto *me = thing_player(g);
  if (me == nullptr) {
    return;
  }

  if (val) {
    THING_DBG(me, "callback: got 'Yes' warning confirmation");
  } else {
    THING_DBG(me, "callback: got 'No' warning confirmation");
  }
  TRACE_INDENT();

  switch (player_state(g, v)) {
    case PLAYER_STATE_INIT :
      //
      // Player not initialized yet
      //
      break;
    case PLAYER_STATE_DEAD :
      //
      // Player is dead
      //
      return;
    case PLAYER_STATE_NORMAL :
      //
      // Replace the mouse path
      //
      break;
    case PLAYER_STATE_PATH_REQUESTED :
      //
      // Player wants to start following or replace the current path.
      //
      break;
    case PLAYER_STATE_MOVE_CONFIRM_REQUESTED :
      //
      // Wait for confirmation.
      //
      if (val) {
        THING_DBG(me, "player confirmed move, path size %d", thing_move_path_size(g, v, l, me));
        TRACE_INDENT();
        thing_move_path_confirm(g, v, l, me);
        player_state_change(g, v, l, PLAYER_STATE_FOLLOWING_PATH);
      } else {
        THING_DBG(me, "player declined move");
        TRACE_INDENT();
        player_state_change(g, v, l, PLAYER_STATE_NORMAL);
      }
      break;
    case PLAYER_STATE_FOLLOWING_PATH :
      //
      // Already following a path, stick to it until completion.
      //
      break;
    case PLAYER_STATE_ENUM_MAX : break;
  }

  if (! val) {
    level_cursor_path_reset(g, v, l);
    thing_move_path_reset(g, v, l, me);
  }
}

//
// Return true on the event being consumed
//
auto player_check_if_target_needs_move_confirm(Gamep g, Levelsp v, Levelp l, const bpoint &to) -> bool
{
  auto *me = thing_player(g);
  if (me == nullptr) {
    return false;
  }

  THING_DBG(me, "player move: move path size %d", thing_move_path_size(g, v, l, me));
  TRACE_INDENT();

  if (! adjacent(thing_at(me), to)) {
    return false;
  }

  if (thing_move_path_size(g, v, l, me) == 0) {
    player_state_change(g, v, l, PLAYER_STATE_NORMAL);

    if (level_is_cursor_path_hazard(g, v, l, to) != nullptr) {
      THING_DBG(me, "player move: cursor path is a hazard and have no move path");
      TRACE_INDENT();

      std::vector< bpoint > move_path;
      move_path.push_back(to);
      level_cursor_copy_path_to_player(g, v, l, move_path);
      player_state_change(g, v, l, PLAYER_STATE_FOLLOWING_PATH);
      THING_DBG(me, "player move: move path size %d", thing_move_path_size(g, v, l, me));
    }
  }

  //
  // Double check before jumping in chasms or lava
  //
  if (level_is_needs_move_confirm(g, v, l, to) != nullptr) {
    if (! thing_is_ethereal(me) && ! thing_is_floating(me) && ! thing_is_flying(me)) {
      if (level_is_chasm(g, v, l, to) != nullptr) {
        std::string const msg = "Do you really want to leap into a chasm?";
        player_state_change(g, v, l, PLAYER_STATE_MOVE_CONFIRM_REQUESTED);
        game_state_change(g, STATE_MOVE_WARNING_MENU, "need warning confirmation");
        if (g_opt_tests) {
          player_check_if_target_needs_move_confirm_callback(g, true);
        } else {
          wid_warning(g, msg, player_check_if_target_needs_move_confirm_callback);
        }
        return false;
      }

      if (level_alive_is_brazier(g, v, l, to) != nullptr) {
        std::string const msg = "Do you really want to kick over the brazier?";
        player_state_change(g, v, l, PLAYER_STATE_MOVE_CONFIRM_REQUESTED);
        game_state_change(g, STATE_MOVE_WARNING_MENU, "need warning confirmation");
        if (g_opt_tests) {
          player_check_if_target_needs_move_confirm_callback(g, true);
        } else {
          wid_warning(g, msg, player_check_if_target_needs_move_confirm_callback);
        }
        return false;
      }

      //
      // If not already in lava, warn about moving into it
      //
      if (level_is_lava(g, v, l, thing_at(me)) == nullptr) {
        if (level_is_lava(g, v, l, to) != nullptr) {
          if (! thing_is_immune_to(me, THING_EVENT_HEAT_DAMAGE) && ! thing_is_immune_to(me, THING_EVENT_FIRE_DAMAGE)) {
            std::string const msg = "Do you really want to leap into lava?";
            player_state_change(g, v, l, PLAYER_STATE_MOVE_CONFIRM_REQUESTED);
            game_state_change(g, STATE_MOVE_WARNING_MENU, "need warning confirmation");
            if (g_opt_tests) {
              player_check_if_target_needs_move_confirm_callback(g, true);
            } else {
              wid_warning(g, msg, player_check_if_target_needs_move_confirm_callback);
            }
            return false;
          }
        }
      }
    }
  }

  return true;
}

//
// Return true on a successful move (or a popup asking more info)
//
[[nodiscard]] static auto player_move_try(Gamep g, Levelsp v, Levelp l, Thingp me, bpoint to, bool move_confirmed, bool need_path) -> bool
{
  if (move_confirmed) {
    THING_DBG(me, "move try (confirmed move)");
  } else {
    THING_DBG(me, "move try");
  }
  TRACE_INDENT();

  if (! move_confirmed) {
    if (! player_check_if_target_needs_move_confirm(g, v, l, to)) {
      //
      // A popup is present
      //
      if (thing_move_path_size(g, v, l, me) == 0) {
        std::vector< bpoint > pending_path;
        pending_path.push_back(to);
        (void) thing_move_path_confirmed(g, v, l, me, pending_path);
      }

      return false;
    }
  }

  if (thing_can_move_to_attempt(g, v, l, me, to)) {
    //
    // Fake a mouse path for movement
    //
    THING_DBG(me, "move try: can move to");

    if (need_path) {
      std::vector< bpoint > move_path;
      move_path.push_back(to);
      player_state_change(g, v, l, PLAYER_STATE_PATH_REQUESTED);
      level_cursor_copy_path_to_player(g, v, l, move_path);
      if (! player_check_if_target_needs_move_confirm(g, v, l, to)) {
        //
        // A popup is present
        //
      }
    }
    return true;
  }

  if (thing_can_move_to_attempt_by_shoving(g, v, l, me, to)) {
    //
    // Can we shove it out of the way to move?
    //
    THING_DBG(me, "move try: can move to by shoving");

    if (need_path) {
      std::vector< bpoint > move_path;
      move_path.push_back(to);
      player_state_change(g, v, l, PLAYER_STATE_PATH_REQUESTED);
      level_cursor_copy_path_to_player(g, v, l, move_path);

      //
      // If this needs confirmation, then do not continue onto shoving.
      //
      if (! player_check_if_target_needs_move_confirm(g, v, l, to)) {
        //
        // A popup is present
        //
        return true;
      }
    }

    if (thing_shove_to(g, v, l, me, to)) {
      (void) level_tick_begin_requested(g, v, l, "player shoved");

      //
      // Do not step onto the thing we just shoved.
      //
      return level_is_dead_on_shoving(g, v, l, to) == nullptr;
    }
    (void) level_tick_begin_requested(g, v, l, "player failed to shove");

  } else if (thing_can_move_to_attempt_by_opening(g, v, l, me, to)) {
    //
    // Can we open it allow movement?
    //
    THING_DBG(me, "move try: can move to by opening");

    if (thing_move_to(g, v, l, me, to)) {
      (void) level_tick_begin_requested(g, v, l, "player opened a door to move");
      return true;
    }
    (void) level_tick_begin_requested(g, v, l, "player failed to open something in the way");

  } else if (thing_can_move_to_diagonal_is_blocked(g, v, l, me, to)) {
    //
    // Can we jump diagonally?
    //
    THING_DBG(me, "diagonal move blocked; try to jump");

    if (thing_jump_to(g, v, l, me, to, false)) {
      (void) level_tick_begin_requested(g, v, l, "player jumped");
    } else {
      (void) level_tick_begin_requested(g, v, l, "player could not pass diagonal obstacle");
    }
  } else {
    (void) level_tick_begin_requested(g, v, l, "player bumped into obstacle");
  }

  //
  // If could not move, then abort the path walk
  //
  player_state_change(g, v, l, PLAYER_STATE_NORMAL);

  return false;
}

static void player_move_delta(Gamep g, Levelsp v, Levelp l, int dx, int dy)
{
  TRACE();

  if (game_state(g) != STATE_PLAYING) {
    player_move_requests_reset(g, v);
    return;
  }

  auto *me = thing_player(g);
  if (me == nullptr) {
    return;
  }

  //
  // Override any mouse request with the key move.
  //
  level_cursor_path_reset(g, v, l);

  //
  // Wait until the end of the tick
  //
  if (level_tick_is_in_progress(g, v, l)) {
    return;
  }

  auto         at = thing_at(me);
  bpoint const to(at.x + dx, at.y + dy);

  const bool need_path      = true;
  const bool move_confirmed = false;

  (void) player_move_try(g, v, l, me, to, move_confirmed, need_path);

  player_move_requests_reset(g, v);
}

auto player_fire(Gamep g, Levelsp v, Levelp l, int dx, int dy, Tpp fire_what) -> bool
{
  TRACE();

  auto *me = thing_player(g);
  if (me == nullptr) {
    ERR("no thing pointer");
    return false;
  }

  //
  // TODO(nmcgill):
  //
  static Tpp tp_fireball;
  if (tp_fireball == nullptr) {
    tp_fireball = tp_find_mand("fireball");
  }

  if (fire_what == nullptr) {
    fire_what = tp_fireball;
  }

  auto *ext_struct = thing_ext_struct(g, me);
  if (ext_struct == nullptr) {
    ERR("no ext struct found");
    return false;
  }

  if (fire_what == nullptr) {
    ERR("nothing to fire");
    return false;
  }

  if (game_state(g) != STATE_PLAYING) {
    player_move_requests_reset(g, v);
    return false;
  }

  //
  // Wait until the end of the tick
  //
  if (level_tick_is_in_progress(g, v, l)) {
    return false;
  }

  thing_set_dir_from_delta(me, dx, dy);

  player_move_requests_reset(g, v);

  //
  // No firing in deep water
  //
  if (level_is_deep_water(g, v, l, thing_at(me)) != nullptr) {
    if (thing_is_player(me)) {
      topcon("The deep water is preventing you from firing a volley!");
      return false;
    }
  }

  bpoint target;

  if (v->cursor_visible) {
    target = v->cursor_at;
  } else {
    fpoint const delta = thing_get_direction(g, v, l, me);
    target             = make_bpoint(thing_real_at(me) + delta);
  }

  if (! thing_projectile_fire_at(g, v, l, me, fire_what, target)) {
    return false;
  }

  return level_tick_begin_requested(g, v, l, "player fired");
}

//
// All keys have been released, forget any accumulation of events
//
void player_move_requests_reset(Gamep g, Levelsp v)
{
  TRACE();

  v->requested_fire       = false;
  v->requested_move_up    = false;
  v->requested_move_down  = false;
  v->requested_move_left  = false;
  v->requested_move_right = false;
}

//
// Allow moves to accumulate so we can do diagonal moves.
//
void player_move_accum(Gamep g, Levelsp v, Levelp l, bool up, bool down, bool left, bool right, bool fire)
{
  TRACE();

  if (fire) {
    v->requested_fire = fire;
  }

  if (up) {
    v->requested_move_up = up;
  }

  if (down) {
    v->requested_move_down = down;
  }

  if (left) {
    v->requested_move_left = left;
  }

  if (right) {
    v->requested_move_right = right;
  }
}

//
// Attempt to move
//
auto player_move_request(Gamep g, bool up, bool down, bool left, bool right, bool fire) -> bool
{
  TRACE();

  auto *v = game_levels_get(g);
  if (v == nullptr) {
    return false;
  }

  auto *l = game_level_get(g, v);
  if (l == nullptr) {
    return false;
  }

  if (game_state(g) != STATE_PLAYING) {
    player_move_requests_reset(g, v);
    return false;
  }

  player_move_accum(g, v, l, up, down, left, right, fire);

  //
  // If a move is in progress, do nothing
  //
  if (level_tick_is_in_progress(g, v, l)) {
    return false;
  }

  if (v->requested_fire) {
    if (v->requested_move_up) {
      if (v->requested_move_left) {
        (void) player_fire(g, v, l, -1, -1);
      } else if (v->requested_move_right) {
        (void) player_fire(g, v, l, 1, -1);
      } else {
        (void) player_fire(g, v, l, 0, -1);
      }
    } else if (v->requested_move_down) {
      if (v->requested_move_left) {
        (void) player_fire(g, v, l, -1, 1);
      } else if (v->requested_move_right) {
        (void) player_fire(g, v, l, 1, 1);
      } else {
        (void) player_fire(g, v, l, 0, 1);
      }
    } else if (v->requested_move_left) {
      (void) player_fire(g, v, l, -1, 0);
    } else if (v->requested_move_right) {
      (void) player_fire(g, v, l, 1, 0);
    } else {
      (void) player_fire(g, v, l, 0, 0);
    }
  }

  if (v->requested_move_up) {
    if (v->requested_move_left) {
      player_move_delta(g, v, l, -1, -1);
    } else if (v->requested_move_right) {
      player_move_delta(g, v, l, 1, -1);
    } else {
      player_move_delta(g, v, l, 0, -1);
    }
  } else if (v->requested_move_down) {
    if (v->requested_move_left) {
      player_move_delta(g, v, l, -1, 1);
    } else if (v->requested_move_right) {
      player_move_delta(g, v, l, 1, 1);
    } else {
      player_move_delta(g, v, l, 0, 1);
    }
  } else if (v->requested_move_left) {
    player_move_delta(g, v, l, -1, 0);
  } else if (v->requested_move_right) {
    player_move_delta(g, v, l, 1, 0);
  }

  return true;
}

//
// Handle common level exit interactions
//
static void player_leave_current_level_and_change_to_level_num(Gamep g, Levelsp v, LevelNum level_num = LEVEL_SELECT_ID)
{
  TRACE();

  level_select_update_grid_tiles(g, v);
  level_cursor_path_reset(g, v, game_level_get(g, v));
  (void) level_change(g, v, level_num);
  game_request_to_remake_ui_set(g);
}

//
// Force move a me to a specific level
//
void player_warp_to_specific_level(Gamep g, Levelsp v, LevelNum level_num)
{
  TRACE();

  player_leave_current_level_and_change_to_level_num(g, v, level_num);

  auto *me = thing_player(g);
  if (me == nullptr) {
    ERR("no thing pointer");
    return;
  }

  auto *new_level = level_change(g, v, level_num);
  if (new_level == nullptr) {
    thing_err(me, "failed to move me to level %u", level_num);
    return;
  }

  thing_level_warp_to_entrance(g, v, new_level, me);
}

//
// Handle level exit interactions
//
void player_reached_exit(Gamep g, Levelsp v, Levelp l)
{
  TRACE();

  level_is_completed_by_player_exiting(g, v, l);
  player_leave_current_level_and_change_to_level_num(g, v, LEVEL_SELECT_ID);
}

//
// Handle level entrance interactions
//
void player_reached_entrance(Gamep g, Levelsp v, Levelp l)
{
  TRACE();

  player_leave_current_level_and_change_to_level_num(g, v, LEVEL_SELECT_ID);
}

//
// Handle the me falling out of the level
//
void player_fell(Gamep g, Levelsp v, Levelp l, Levelp next_level, Thingp me)
{
  TRACE();

  level_cursor_path_reset(g, v, l);
  level_is_completed_by_player_falling(g, v, l);

  if (next_level != level_change(g, v, next_level->level_num)) {
    ERR("unexpected level found");
    return;
  }
}

//
// Handle interactions for a thing at its location
//
void player_collision_handle(Gamep g, Levelsp v, Levelp l, Thingp me)
{
  TRACE();

  auto *ext_struct = thing_ext_struct(g, me);
  if (ext_struct == nullptr) {
    return;
  }

  auto at = thing_at(me);

  FOR_ALL_THINGS_AT(g, v, l, it, at)
  {
    //
    // Open secret doors automatically
    //
    if (thing_is_openable(it)) {
      if (thing_open(g, v, l, it, me /* opener */)) {
        if (thing_is_dead(it)) {
          continue;
        }
      }
    }

    //
    // At the end of the popped path or not?
    //
    if (thing_move_path_size(g, v, l, me) != 0) {
      //
      // If still more tiles to pop, do not descend automatically
      //
      if (thing_is_exit(it)) {
        //
        // To enabled the descend shortcut
        //
        game_request_to_remake_ui_set(g);
        return;
      }

      if (thing_is_entrance(it)) {
        //
        // To enabled the ascent shortcut
        //
        game_request_to_remake_ui_set(g);
        return;
      }
    } else {
      //
      // If at the end of the move path then we can enter or leave when we get to that final tile.
      //
      if (thing_is_exit(it)) {
        //
        // Descend
        //
        player_reached_exit(g, v, l);
        return;
      }

      if (thing_is_entrance(it)) {
        //
        // Ascend
        //
        player_reached_entrance(g, v, l);
        return;
      }
    }
  }
}

//
// Handle me jumping
//
auto player_jump(Gamep g, Levelsp v, Levelp l, Thingp me, bpoint to) -> bool
{
  TRACE();
  THING_DBG(me, "player jump");

  auto *player_struct = thing_player_struct(g);
  if (player_struct == nullptr) {
    ERR("no player struct found");
    return false;
  }

  //
  // If jumping too far, truncate the jump
  //
  auto how_far_i_can_jump = thing_distance_jump(me);
  if (! how_far_i_can_jump) {
    if (thing_is_player(me)) {
      topcon("You are too tired to jump.");
    }
    return false;
  }

  auto at        = thing_at(me);
  auto jump_path = draw_line(at, to, how_far_i_can_jump + 1);
  bool warn      = true;

  for (auto intermediate : std::ranges::reverse_view(jump_path)) {
    if (thing_jump_to(g, v, l, me, intermediate, warn)) {
      (void) level_tick_begin_requested(g, v, l, "player jumped");
      player_state_change(g, v, l, PLAYER_STATE_FOLLOWING_PATH);
      return true;
    }
    warn = false;
  }

  return false;
}

//
// Move to the next path on the popped path if it exits.
//
auto player_move_to_next(Gamep g, Levelsp v, Levelp l, Thingp me) -> bool
{
  //
  // If already moving, do not pop the next path tile
  //
  if (thing_is_moving(me)) {
    THING_DBG(me, "player move to next: already moving, delay");
    return false;
  }

  //
  // If not following a path, then nothing to pop
  //
  switch (player_state(g, v)) {
    case PLAYER_STATE_INIT :
      //
      // Player not initialized yet
      //
      break;
    case PLAYER_STATE_DEAD :
      //
      // No moving when dead
      //
      return false;
    case PLAYER_STATE_NORMAL :
      //
      // Replace the mouse path
      //
      return false;
    case PLAYER_STATE_PATH_REQUESTED :
      //
      // Player wants to start following or replace the current path.
      //
      return false;
    case PLAYER_STATE_MOVE_CONFIRM_REQUESTED :
      //
      // Wait for confirmation.
      //
      return false;
    case PLAYER_STATE_FOLLOWING_PATH :
      //
      // Already following a path, stick to it until completion.
      //
      break;
    case PLAYER_STATE_ENUM_MAX : break;
  }

  THING_DBG(me, "player move to next");
  TRACE_INDENT();

  //
  // Get the next tile to move to
  //
  bpoint move_next      = {};
  bool   move_confirmed = {};

  THING_DBG(me, "player pop next move");
  TRACE_INDENT();

  if (! thing_move_path_pop(g, v, l, me, move_confirmed, move_next)) {
    //
    // If could not pop, then no path is left
    //
    THING_DBG(me, "player pop next move; no path left");
    TRACE_INDENT();

    player_state_change(g, v, l, PLAYER_STATE_NORMAL);
    return false;
  }

  bpoint move_destination = {};
  if (thing_move_path_target(g, v, l, me, move_destination)) {
    THING_DBG(me, "player has a path to target");
    TRACE_INDENT();

    if (level_is_cursor_path_hazard(g, v, l, move_next) != nullptr) {
      THING_DBG(me, "player has a path to target, but it is a hazard");
      TRACE_INDENT();

      if (thing_jump_to(g, v, l, me, move_destination)) {
        THING_DBG(me, "player has a path to target, but can jump to it");
        TRACE_INDENT();

        //
        // If we're jumping onto a chasm still, maybe a few tiles away, we want
        // the player to have a chance to say no
        //
        bool const need_path = false;
        if (level_is_cursor_path_hazard(g, v, l, move_destination) != nullptr) {
          if (! player_move_try(g, v, l, me, move_next, move_confirmed, need_path)) {
            return false;
          }
        }

        //
        // If could jump, then abort the path walk
        //
        (void) level_tick_begin_requested(g, v, l, "player jumped to avoid a hazard");
        player_state_change(g, v, l, PLAYER_STATE_NORMAL);
        return false;
      }

      //
      // Something was in the way of jumping. Best to stop rather than accidentally
      // walk into a chasm.
      //
      (void) level_tick_begin_requested(g, v, l, "player fail to jump to avoid a hazard");
      player_state_change(g, v, l, PLAYER_STATE_NORMAL);
      return false;
    }
  }

  bool const need_path = false;
  if (! player_move_try(g, v, l, me, move_next, move_confirmed, need_path)) {
    return false;
  }

  if (thing_move_to(g, v, l, me, move_next)) {
    if (thing_is_player(me)) {
      (void) level_tick_begin_requested(g, v, l, "player moved to next");
    }
  } else {
    if (thing_is_player(me)) {
      (void) level_tick_begin_requested(g, v, l, "player faled moved to next location");
    }
  }

  return true;
}
