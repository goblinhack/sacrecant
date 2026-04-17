//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_dice_rolls.hpp"
#include "my_game.hpp"
#include "my_level_inlines.hpp"
#include "my_random.hpp"
#include "my_thing_inlines.hpp"
#include "my_ui.hpp"

//
// Ok to land on this spot?
//
[[nodiscard]] static auto thing_ok_landing_spot(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE();

  if (is_oob_or_border(p)) [[unlikely]] {
    return false;
  }

  if (level_is_obs_to_falling_onto(g, v, l, p) != nullptr) {
    return false;
  }

  return true;
}

//
// Try to find a spot close to where we landed that is ok to exist in.
// i.e. no landing inside walls.
//
static auto thing_choose_landing_spot(Gamep g, Levelsp v, Levelp l, Thingp t) -> bpoint
{
  TRACE();

  auto at   = thing_at(t);
  int  dist = 1;

  for (;;) {
    if (thing_ok_landing_spot(g, v, l, at)) {
      return at;
    }

    for (auto dx = -dist; dx <= dist; dx++) {
      for (auto dy = -dist; dy <= dist; dy++) {
        bpoint q;
        q.x = at.x + dx;
        q.y = at.y + dy;

        if (thing_ok_landing_spot(g, v, l, q)) {
          return q;
        }
      }
    }

    dist++;
  }

  return at;
}

//
// How much damage does the thing take on falling.
//
static auto thing_fall_damage(Gamep g, Levelsp v, Levelp l, Thingp t) -> int
{
  TRACE();

  int fall_dmg = 0;

  if (thing_is_player(t)) {
    fall_dmg = PCG_RANDOM_RANGE(6, 30);
  } else if (thing_is_mob(t) || thing_is_monst(t)) {
    fall_dmg = PCG_RANDOM_RANGE(6, thing_health(t) / 2);
  } else {
    fall_dmg = d4();
  }

  if (thing_is_falling_continues(t)) {
    fall_dmg *= 2;
  }

  //
  // Landing in lava is bad
  //
  if (level_is_lava(g, v, l, thing_at(t)) != nullptr) {
    if (! thing_is_immune_to(t, THING_EVENT_HEAT_DAMAGE)) {
      fall_dmg *= 4;
    }
  }

  //
  // Water dampens the fall
  //
  if (level_is_water(g, v, l, thing_at(t)) != nullptr) {
    if (thing_is_immune_to(t, THING_EVENT_WATER_DAMAGE)) {
      fall_dmg /= 2;
    }
  }

  //
  // Deep water dampens it further
  //
  if (level_is_deep_water(g, v, l, thing_at(t)) != nullptr) {
    if (thing_is_immune_to(t, THING_EVENT_WATER_DAMAGE)) {
      fall_dmg /= 2;
    }
  }

  //
  // Amplify or nullify due to weight
  //
  auto w = thing_weight(t);
  if (w <= WEIGHT_FEATHER) {
    fall_dmg = 0;
  } else if (w >= WEIGHT_HEAVY) {
    fall_dmg *= 2;
  }

  return fall_dmg;
}

//
// Complete the fall to the next level. If this is the player we also change level.
//
static void thing_fall_end(Gamep g, Levelsp v, Levelp l, Thingp t)
{
  THING_DBG(t, "fall end");
  TRACE_INDENT();

  auto *next_level = level_select_get_next_level_down(g, v, l);
  if (next_level == nullptr) {
    if (g_opt_tests) {
      THING_DBG(t, "no level to fall onto");
    } else {
      thing_err(t, "no level to fall onto");
    }
    return;
  }

  if (next_level == l) {
    ThingEvent e {
        .reason     = "by falling into nothing",     //
        .event_type = THING_EVENT_FALL,              //
        .damage     = thing_fall_damage(g, v, l, t), //
    };

    THING_DBG(t, "dead due to falling into nothing");
    TRACE_INDENT();

    thing_dead(g, v, l, t, e);
    return;
  }

  //
  // Choose a new landing spot for the thing
  //
  auto new_location = thing_choose_landing_spot(g, v, next_level, t);

  //
  // Move the thing there
  //
  if (! thing_warp_to(g, v, next_level, t, new_location)) {
    topcon("You fail to find the ground!");
  }

  if (thing_is_player(t)) {
    level_scroll_warp_to_focus(g, v, l);
  }

  ThingEvent e {
      .reason     = "by falling",                  //
      .event_type = THING_EVENT_FALL,              //
      .damage     = thing_fall_damage(g, v, l, t), //
  };

  //
  // "You tumble into the vuid"
  //
  if (thing_is_player(t)) {
    player_fell(g, v, l, next_level, t);
  }

  auto *t_level = game_level_get(g, v, t->level_num);
  if (t_level != nullptr) {
    l = t_level;
  } else {
    thing_err(t, "fell into nothing");
  }

  if (level_is_chasm(g, v, t_level, thing_at(t)) != nullptr) {
    //
    // If we keep on falling through chasms again and again though,
    // we need to break the potential fall loop.
    //
    if (thing_is_falling_continues(t)) {
      if (! thing_is_able_to_fall_repeatedly(t)) {
        e.reason = "repeated falling";

        THING_DBG(t, "dead due to repeated falling");
        TRACE_INDENT();

        thing_dead(g, v, l, t, e);
      }
    }

    //
    // Keep falling with no damage if over a chasm.
    //
    THING_DBG(t, "over a chasm again; keep falling");
    TRACE_INDENT();
    thing_is_falling_continues_set(g, v, l, t);
    thing_is_spawned_set(g, v, l, t);
  } else {
    //
    // "You take n damage from falling"
    //
    thing_damage(g, v, l, t, e);
    thing_is_falling_continues_unset(g, v, l, t);
  }

  //
  // Falling can be good
  //
  if (thing_is_burning(t)) {
    thing_is_burning_unset(g, v, l, t);

    if (thing_is_player(t)) {
      topcon(UI_GOOD_FMT_STR "You extinguish the flames as you fall!" UI_RESET_FMT);
    }
  }
}

//
// Falling time step
//
void thing_fall_time_step(Gamep g, Levelsp v, Levelp l, Thingp t, int time_step)
{
  TRACE();

  (void) thing_is_falling_incr(g, v, l, t, time_step);

  if (compiler_unused) {
    THING_DBG(t, "fall incr %u", thing_is_falling(t));
  }
}

//
// Check if we're done falling
//
void thing_fall_end_check(Gamep g, Levelsp v, Levelp l, Thingp t)
{
  TRACE();

  if (compiler_unused) {
    THING_DBG(t, "fall %u", thing_is_falling(t));
    TRACE_INDENT();
  }

  //
  // Fallen for long enough?
  //
  bool fall_finished = thing_is_falling(t) >= THING_FALL_TIME_MS;

  //
  // If the player cannot see the thing fall, then complete the fall to avoid
  // any stuttering in frame rate
  //
  if (! fall_finished) {
    auto *player = thing_player(g);
    if (player != nullptr) {
      if (t != player) {
        if (! thing_vision_can_see_tile(g, v, l, player, thing_at(t))) {
          fall_finished = true;
        }
      }
    }
  }

  //
  // Fall complete?
  //
  if (fall_finished) {
    thing_fall_end(g, v, l, t);

    auto *t_level = game_level_get(g, v, t->level_num);
    if (t_level == nullptr) {
      thing_err(t, "fell into nothing");
    }

    thing_is_falling_set(g, v, t_level, t, false);

    if (thing_is_player(t)) {
      if (level_is_chasm(g, v, t_level, thing_at(t)) != nullptr) {
        THING_DBG(t, "fell again");
        TRACE_INDENT();
        (void) level_tick_begin_requested(g, v, t_level, "player fell again");
      }

      //
      // This seems rather cruel, but...
      //
      if (level_is_lava(g, v, t_level, thing_at(t)) != nullptr) {
        THING_DBG(t, "fell into lava");
        TRACE_INDENT();
        (void) level_tick_begin_requested(g, v, t_level, "player fell into lava");
      }
    }
  }
}

//
// Begin falling
//
void thing_fall(Gamep g, Levelsp v, Levelp l, Thingp t)
{
  TRACE();

  //
  // Fall at the end of the move, or it just looks odd with things falling
  // through the floor before they get to the chasm
  //
  if (thing_is_moving(t)) {
    return;
  }

  if (! thing_is_able_to_fall(t)) {
    return;
  }

  if (thing_is_able_to_fall_sound(t)) {
    thing_sound_play(g, v, l, t, "fall");
  }

  thing_is_falling_set(g, v, l, t, true);
}
