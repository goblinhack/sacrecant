//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_dice_rolls.hpp"
#include "my_game.hpp"
#include "my_game_popups.hpp"
#include "my_globals.hpp"
#include "my_level.hpp"
#include "my_main.hpp"
#include "my_pcg_basic.hpp"
#include "my_thing_inlines.hpp"
#include "my_wids.hpp"

static void level_tick_begin(Gamep g, Levelsp v, Levelp l);
static void level_tick_body(Gamep g, Levelsp v, Levelp l, float dt, bool tick_is_about_to_end = false);
static void level_tick_end(Gamep g, Levelsp v, Levelp l);
static void level_tick_idle(Gamep g, Levelsp v, Levelp l);
static void level_tick_check_running_time(Gamep g, Levelsp v, Levelp l);
static void level_tick_in_progress(Gamep g, Levelsp v, Levelp l);

static std::vector< ThingId > worklist;

//
// Called at the end of the tick and then whenever needed, like at the end of an animation.
//
static void level_cleanup_things(Gamep g, Levelsp v, Levelp l)
{
  TRACE();

  if (! level_request_to_cleanup_things(g, v, l)) {
    return;
  }
  level_request_to_cleanup_things_unset(g, v, l);

  //
  // This can pop the next player move
  //
  FOR_ALL_THINGS_ON_LEVEL(g, v, l, t)
  {
    if (thing_is_scheduled_for_cleanup(t)) {
      thing_fini(g, v, l, t);
    }
  }
}

//
// Check if it is ok to end the tick
// e.g. something fell in lava and now needs to delay the end of the tick whilst its animation finishes
//
static void level_tick_ok_to_end_check(Gamep g, Levelsp v, Levelp l)
{
  TRACE();

  l->tick_wait_on_things = false;
  l->tick_wait_on_anim   = false;

  //
  // The player has died and the dead menu has been closed
  //
  if (game_request_to_end_game_get(g)) {
    l->tick_end_requested = true;
    return;
  }

  if (static_cast< uint32_t >(! worklist.empty()) != 0U) {
    l->tick_wait_on_things = true;
  }

  FOR_ALL_THINGS_ON_LEVEL(g, v, l, t)
  {
    //
    // We need to wait for things to complete moving into lava for example before we can
    // run temperature checks. Else it looks odd that it catches fire before it reaches
    // the lava.
    //
    if (thing_is_moving(t)) {
      l->tick_wait_on_things = true;
      if (compiler_unused) { //
        LEVEL_DBG(g, v, l, "waiting on moving %s", to_string(g, v, l, t).c_str());
      }
    }

    if (thing_is_jumping(t)) {
      l->tick_wait_on_things = true;
      if (compiler_unused) { //
        LEVEL_DBG(g, v, l, "waiting on jumping %s", to_string(g, v, l, t).c_str());
      }
    }

    //
    // Falling complete?
    //
    if (thing_is_falling(t) != 0) {
      thing_fall_end_check(g, v, l, t);
      if (thing_is_falling(t) != 0) {
        l->tick_wait_on_things = true;
        if (compiler_unused) { //
          LEVEL_DBG(g, v, l, "waiting on falling %s", to_string(g, v, l, t).c_str());
        }
      }
    }

    //
    // Lunging complete?
    //
    if (thing_is_lunging(t) != 0) {
      thing_lunge_end_check(g, v, l, t);
      if (thing_is_lunging(t) != 0) {
        if (compiler_unused) { //
          LEVEL_DBG(g, v, l, "waiting on lunge %s", to_string(g, v, l, t).c_str());
        }
      }
    }

    //
    // Some things like explosions, we want to wait for the explosion to finish before
    // moving to the next tick. Except it adds delays and is currently disabled.
    //
    if (g_opt_tests) {
      if (thing_is_dead(t)) {
        if (! thing_is_scheduled_for_cleanup(t)) {
          if (thing_is_wait_on_dead_anim(t)) {
            l->tick_wait_on_anim = true;
          }
        }
      }
    }
  }
}

//
// Called per frame
//
static void level_tick(Gamep g, Levelsp v, Levelp l, bool tick_begin_requested)
{
  TRACE();

  level_verify(g, v, l);

  //
  // First time tick for this level?
  //
  if (! l->is_tick_has_occurred) {
    l->is_tick_has_occurred = true;

    //
    // Update minimaps and lighting
    //
    level_update_visibility(g, v, l);
  }

  if (l->tick_in_progress) {
    //
    // A tick is running
    //
    level_tick_in_progress(g, v, l);
  } else if (tick_begin_requested) {
    //
    // Allow temperatures to settle prior to starting
    //
    level_tick_begin_temperature(g, v, l);

    //
    // Start the tick
    //
    level_tick_begin(g, v, l);
  } else {
    //
    // Idle state
    //
    level_tick_idle(g, v, l);
  }

  //
  // Move things. Interpolated per frame.
  //
  if (l->tick_in_progress) {
    level_tick_body(g, v, l, v->time_step - v->last_time_step);

    //
    // For things like projectiles, we need to handle teleport and water checks
    // amonst others in the tick body as they move too fast and the projectile
    // can miss being teleported.
    //

    //
    // Handle things interacting with water.
    //
    level_tick_water(g, v, l);
  }

  //
  // Normally this is done in level display as we need to also animate other levels.
  // For tests though, this allows explosions to occur etc..
  //
  if (g_opt_tests) {
    level_anim(g, v, l);
  }

  //
  // Update any tiles that are needed
  //
  level_update_paths(g, v, l);

  //
  // Check if something fell in lava and now needs to delay the end of the tick
  //
  level_tick_ok_to_end_check(g, v, l);

  //
  // If things are no longer moving and we have requested the end of the tick, then we can check
  // for temperature interactions.
  //
  if (l->tick_end_requested && (v->tick != l->tick_temperature) && ! l->tick_wait_on_things) {
    //
    // Only do this once per tick
    //
    l->tick_temperature = v->tick;

    //
    // Need to do the temperature checks after things have moved an also need to give time for death
    // animations to end
    //
    level_tick_end_temperature(g, v, l);

    do {
      l->is_tick_end_delay = false;

      //
      // Handle things interacting with explosions
      //
      level_tick_explosion(g, v, l);

      //
      // Handle special cases that cannot do collision detection
      //
      level_tick_chasm(g, v, l);
      level_tick_teleport(g, v, l);

      //
      // Check if something reacted with lava and is now needing to delay the end of tick
      //
      level_tick_ok_to_end_check(g, v, l);

      //
      // A chasm or explosion or some other event has occurred that we need to handle immediately
      // and delay ending the tick
      //
    } while (l->is_tick_end_delay);
  }

  //
  // Are we done with all checks and ok to end the tick which will trigger thing cleanup.
  //
  if (l->tick_end_requested && ! l->tick_wait_on_things && ! l->tick_wait_on_anim) {
    level_tick_end(g, v, l);

    //
    // Update minimaps and lighting
    //
    level_update_visibility(g, v, l);

    l->tick_ended = true;
  }

  //
  // Remove things scheduled for cleanup
  //
  level_cleanup_things(g, v, l);

  //
  // Remove old popups
  //
  game_popups_age(g);
}

//
// If we've ran long enough, we're done
//
static void level_tick_check_running_time(Gamep g, Levelsp v, Levelp l)
{
  if (v->time_step >= 1.0) {
    //
    // Tick has surpassed its time
    //
    v->time_step = 1.0;

    //
    // We need to give things a change to finish moving
    //
    level_tick_body(g, v, l, 0, true /* tick_is_about_to_end */);

    if (! l->tick_end_requested) {
      if (level_is_player_level(g, v, l)) {
        LEVEL_DBG(g, v, l, "Tick %u: end requested", v->tick);
      }
    }

    l->tick_end_requested = true;
  }
}

static void level_tick_body(Gamep g, Levelsp v, Levelp l, float dt, bool tick_is_about_to_end)
{
  TRACE();

  if (dt < 0) {
    CROAK("negative dt %f", dt);
  }

  if (tick_is_about_to_end) {
    //
    // Need to finish all movement
    //
  } else if (dt == 0) {
    //
    // During tests, the loop can spin really fast. Pointless doing work at that rate and
    // projectiles will not move, so wait until time moves on.
    //
    return;
  }

  auto *player = thing_player(g);
  if (player == nullptr) [[unlikely]] {
    return;
  }
  const int player_speed = thing_speed(player);

  if (compiler_unused) {
    if (level_is_player_level(g, v, l)) {
      LEVEL_DBG(g, v, l, "time_step %f v->last_time_step %f dt %f", v->time_step, v->last_time_step, dt);
    }
  }

  FOR_ALL_THINGS_ON_LEVEL(g, v, l, t)
  {
    //                   Tick 1              Tick 2
    //            =================== ===================
    //            0.00 0.25 0.50 1.00 0.00 0.25 0.50 1.00
    // --------------------------------------------------------
    // speed 400  tick tick tick tick tick tick tick tick
    // speed 200  tick      tick      tick      tick
    // speed 100  tick                tick
    // speed 50   tick
    //
    if (! thing_is_moving(t) && ! thing_is_jumping(t) && ! thing_is_projectile(t)) {
      continue;
    }

    float const t_speed      = thing_speed(t);
    auto        old_thing_dt = t->thing_dt;

    if (thing_is_projectile(t)) {
      t->thing_dt = v->time_step;
    } else {
      t->thing_dt += dt * (t_speed / static_cast< float >(player_speed));
    }

    if (tick_is_about_to_end || (t->thing_dt >= 1.0)) {
      t->thing_dt = 1.0;
    }

    auto thing_dt_change = t->thing_dt - old_thing_dt;

    if (compiler_unused) {
      THING_DBG(t, "level dt %f old_thing_dt %f thing_dt %f thing_dt_change %f speed %d v %d",
                dt,              //
                old_thing_dt,    //
                t->thing_dt,     //
                thing_dt_change, //
                thing_speed(t),  //
                player_speed     //
      );
    }

    if (thing_is_projectile(t)) {
      //
      // Every pixel change, we want to redo collision detection
      //
      if (dt != 0) {
        thing_projectile_move(g, v, l, t, dt);
      }
    } else {
      thing_interpolate(g, v, l, t, t->thing_dt);
    }

    if (tick_is_about_to_end || (t->thing_dt >= 1.0)) {
      thing_move_or_jump_finish(g, v, l, t);

      //
      // Handle interactions for a thing at its new location
      //
      thing_collision_handle(g, v, l, t);

      thing_is_spawned_unset(g, v, l, t);

      //
      // See if this monster can move again this tick
      //
      if (! tick_is_about_to_end && (v->time_step < 1.0)) {
        THING_DBG(t, "additional move possible");
        thing_monst_tick(g, v, l, t);
      }
    }
  }
}

static void level_tick_begin(Gamep g, Levelsp v, Levelp l)
{
  TRACE();

  level_log(g, v, l, "Tick %u: begin", v->tick);
  TRACE_INDENT();

  l->tick_begin_requested = false;
  l->tick_in_progress     = true;
  l->tick_ended           = false;

  //
  // Update cached flags
  //
  level_update_flags(g, v, l);

  //
  // For things that are too far away, move them less often
  //
  bpoint player_at;
  auto  *player = thing_player(g);
  if (player != nullptr) {
    player_at = thing_at(player);
  }

  FOR_ALL_THINGS_ON_LEVEL(g, v, l, t)
  {
    if (thing_is_tickable(t)) {
      thing_tick_begin(g, v, l, t);

      if (! thing_is_dead(t)) {
        if (thing_is_monst(t)) {
          if ((player != nullptr) && (distance(player_at, thing_at(t)) > MAP_WIDTH / 2)) {
            if (d100() > TICK_FAR_OFF_MONST_CHANCE) {
              continue;
            }
          }

          worklist.push_back(t->id);
        }
      }
    }
  }
}

//
// Process monsters per tick. We do this staggered to avoid the framerate being
// choppy when we process a lot of monsters at the start of a tick.
//
static void level_tick_worklist(Gamep g, Levelsp v, Levelp l)
{
  TRACE();

  auto *player_level = thing_player_level(g);
  if (player_level == nullptr) {
    worklist.clear();
    return;
  }

  auto sz = static_cast< int >(worklist.size());
  if (worklist.empty()) {
    return;
  }

  //
  // How many monsters to process per loop
  //
  auto how_much_to_process = std::max(4, sz);

  for (auto w = 0; w < how_much_to_process; w++) {
    auto id = worklist.back();
    worklist.pop_back();

    auto *t = thing_find_optional(g, v, id);
    if (t != nullptr) {
      if (! thing_is_dead(t)) {
        auto *monst_level = thing_level(g, v, t);
        if ((monst_level != nullptr) && (monst_level == player_level)) {
          thing_monst_tick(g, v, monst_level, t);
        }
      }
    }

    if (worklist.empty()) {
      return;
    }
  }
}

static void level_tick_in_progress(Gamep g, Levelsp v, Levelp l)
{
  TRACE();

  if (compiler_unused) {
    LEVEL_DBG(g, v, l, "Tick %u: progress", v->tick);
  }

  //
  // Process monsters per tick. We do this staggered to avoid the framerate being
  // choppy when we process a lot of monsters at the start of a tick.
  //
  level_tick_worklist(g, v, l);

  //
  // If we've ran long enough, we're done
  //
  level_tick_check_running_time(g, v, l);

  //
  // Need to update while moving as raycasting can change
  //
  level_update_visibility(g, v, l);
}

static void level_tick_idle(Gamep g, Levelsp v, Levelp l)
{
  TRACE();

  FOR_ALL_THINGS_ON_LEVEL(g, v, l, t)
  {
    if (thing_is_tickable(t)) {
      thing_tick_idle(g, v, l, t);
    }
  }
}

auto level_tick_begin_requested(Gamep g, Levelsp v, Levelp l, const char *why) -> bool
{
  TRACE();

  //
  // If we've already requested a tick, then indicate to the caller.
  //
  bool ret = true;

  FOR_ALL_TICKING_LEVELS(g, v, iter)
  {
    if (iter->tick_begin_requested) {
      level_log(g, v, iter, "Tick already requested for this level");
      ret = false;
    }
  }

  if (ret) {
    level_log(g, v, l, "Tick %u: requested new tick: %s", v->tick, why);
  } else {
    level_log(g, v, l, "Tick %u: already requested, ignore: %s", v->tick, why);
  }

  FOR_ALL_TICKING_LEVELS(g, v, iter)
  {
    if (! iter->tick_begin_requested) {
      iter->tick_begin_requested = true;
      level_log(g, v, iter, "Tick requested for this level");
    }
  }

  v->requested_fire       = false;
  v->requested_move_up    = false;
  v->requested_move_down  = false;
  v->requested_move_left  = false;
  v->requested_move_right = false;

  return ret;
}

auto level_tick_begin_is_requested(Gamep g, Levelsp v, Levelp l) -> bool
{
  TRACE();

  FOR_ALL_TICKING_LEVELS(g, v, iter)
  {
    if (iter->tick_begin_requested) {
      return true;
    }
  }

  return false;
}

static void level_tick_end(Gamep g, Levelsp v, Levelp l)
{
  TRACE();

  if (level_is_player_level(g, v, l)) {
    LEVEL_DBG(g, v, l, "Tick %u: ending", v->tick);
  }

  l->tick_end_requested = false;
  l->tick_in_progress   = false;

  game_request_to_remake_ui_set(g);

  //
  // Only save once a tick is complete and before the next move is popped below
  //
  if (game_request_to_save_game_get(g)) {
    game_request_to_save_game_unset(g);
    wid_save_select(g);
  }

  //
  // This can pop the next player move
  //
  FOR_ALL_THINGS_ON_LEVEL(g, v, l, t)
  {
    if (thing_is_tickable(t)) {
      thing_tick_end(g, v, l, t);
    }
  }

  if (level_is_player_level(g, v, l)) {
    LEVEL_DBG(g, v, l, "Tick %u: end", v->tick);
  }
}

auto level_tick_is_in_progress(Gamep g, Levelsp v, Levelp l) -> bool
{
  TRACE();

  return l->tick_in_progress;
}

//
// Reset the levels we need to tick
//
static void level_tick_select_reset(Gamep g, Levelsp v, Levelp current_level)
{
  TRACE();

  //
  // Reset the ticking levels
  // And set the current level (useful in debugging)
  //
  FOR_ALL_LEVELS(g, v, iter)
  {
    iter->is_tick_required = false;
    level_is_player_level_unset(g, v, iter);
  }

  level_is_player_level_set(g, v, current_level);
}

//
// Determine the levels we need to tick
//
static void level_tick_select(Gamep g, Levelsp v, Levelp current_level)
{
  TRACE();

  auto *player_level = thing_player_level(g);

  //
  // Reset the levels we need to tick
  //
  level_tick_select_reset(g, v, current_level);

  //
  // Count the levels to tick
  //
  v->level_ticking_count = 0;
  FOR_ALL_LEVELS(g, v, iter)
  {
    if (g_opt_tests) {
      iter->is_tick_required = true;
      v->level_ticking_count++;
      continue;
    }

    if (iter == player_level) {
      iter->is_tick_required = true;
      v->level_ticking_count++;

      auto *level_below = level_select_get_next_level_down(g, v, iter);
      if (level_below != nullptr) {
        level_below->is_tick_required = true;
        v->level_ticking_count++;
      }
      continue;
    }

    //
    // Sometimes things will fall into a level beyond what we normally see
    // and we need to ensure things don't end up hovering over a chasm.
    //
    if (iter->is_tick_requested) {
      iter->is_tick_required = true;
      v->level_ticking_count++;
      continue;
    }

    //
    // Future wizard levels?
    //
  }
}

//
// Do any levels want to tick?
//
static auto level_tick_process_pending_request(Gamep g, Levelsp v, Levelp current_level) -> uint32_t
{
  TRACE();

  //
  // Reset counters
  //
  v->level_tick_request_count = 0;

  if (v->level_tick_in_progress_count != 0U) {
    return 0;
  }

  FOR_ALL_TICKING_LEVELS(g, v, iter)
  {
    if (! iter->tick_begin_requested) {
      continue;
    }

    //
    // We've recorded the request now. Forget it.
    //
    iter->tick_begin_requested = false;

    LEVEL_DBG(g, v, iter, "Tick %u: requested by level %u", v->tick, iter->level_num);

    //
    // If this is the first level requesting a tick, reset the fram counters and move the
    // tick along
    //
    if ((v->level_tick_request_count++) == 0U) {
      v->tick++;
      v->frame_begin    = v->frame;
      v->time_step      = 0.0;
      v->last_time_step = 0.0;
    }
  }

  return v->level_tick_request_count;
}

//
// How many ms have elapsed during processing the tick
//
static void level_tick_update_frame_counter(Levelsp v)
{
  TRACE();

  static uint32_t level_ts_begin;
  static uint32_t level_ts_now;
  auto            t = time_ms();

  //
  // We need a more consistent tick for tests that does not vary with time
  //
  static uint32_t test_t;
  if (g_opt_tests) {
    test_t++;
    t = test_t;
  }

  if (level_ts_begin == 0U) [[unlikely]] {
    level_ts_begin = t;
  }

  level_ts_now = t;
  v->frame += level_ts_now - level_ts_begin;
  level_ts_begin = level_ts_now;
}

//
// How many ms have elapsed during processing the tick
//
static void level_tick_time_step(Gamep g, Levelsp v, Levelp current_level)
{
  TRACE();

  //
  // Calculate the time step, for animation
  //
  auto duration_ms = TICK_DURATION_MS;

  v->last_time_step = v->time_step;
  v->time_step      = (static_cast< float >(v->frame - v->frame_begin)) / static_cast< float >(duration_ms);

  IF_DEBUG2
  { //
    if (level_is_player_level(g, v, current_level)) {
      LEVEL_DBG(g, v, current_level, "Tick %u: tick-count %u %f", v->tick, v->level_ticking_count, v->time_step);
    }
  }
}

//
// Tick all levels that requested it.
//
static void level_tick_all(Gamep g, Levelsp v, Levelp current_level)
{
  TRACE();

  //
  // Record if a level wants to start ticking, prior to processing the levels in case a level
  // requests to tick during processing.
  //
  bool const tick_begin_requested = level_tick_process_pending_request(g, v, current_level) > 0;

  //
  // Do the actual tick of the levels now
  //
  FOR_ALL_TICKING_LEVELS(g, v, iter)
  {
    TRACE();
    level_tick(g, v, iter, tick_begin_requested);
  }
}

//
// We need to keep track of how many ticks have ended and how many still run.
// This is really needed for tests to know when a test is complet.
//
static void level_tick_monitor_progress(Gamep g, Levelsp v, Levelp current_level)
{
  TRACE();

  v->level_tick_done_count        = 0;
  v->level_tick_in_progress_count = 0;
  v->level_tick_request_count     = 0;

  FOR_ALL_TICKING_LEVELS(g, v, iter)
  {
    if (iter->tick_ended) {
      v->level_tick_done_count++;
    }
    if (iter->tick_in_progress) {
      v->level_tick_in_progress_count++;
    }
    if (iter->tick_begin_requested) {
      v->level_tick_request_count++;
    }
  }

  //
  // Are all levels finished ticking?
  //
  IF_DEBUG2
  {
    LEVEL_DBG(g, v, current_level, "Tick %u: req %u in-progress-count %u tick-end-count %u", v->tick, v->level_tick_request_count,
              v->level_tick_in_progress_count, v->level_tick_done_count);

    if ((v->level_tick_done_count != 0U) && (v->level_ticking_count == v->level_tick_done_count)) {
      LEVEL_DBG(g, v, current_level, "Tick %u: all %u levels finished ticking", v->tick, v->level_tick_done_count);
    }
  }
}

//
// Tick all interesting levels i.e. if they have a player or are visible through a chasm
//
void levels_tick(Gamep g, Levelsp v)
{
  TRACE();

  auto *current_level = game_level_get(g, v);
  if (current_level == nullptr) {
    return;
  }

  //
  // Which levels do we need to tick?
  //
  level_tick_select(g, v, current_level);

  //
  // Which levels do we need to tick?
  //
  level_tick_time_step(g, v, current_level);

  //
  // Tick all levels that requested it.
  //
  level_tick_all(g, v, current_level);

  //
  // We need to keep track of how many ticks have ended and how many still run.
  // This is really needed for tests to know when a test is complet.
  //
  level_tick_monitor_progress(g, v, current_level);

  //
  // How many ms have elapsed during processing the tick
  //
  level_tick_update_frame_counter(v);
}
