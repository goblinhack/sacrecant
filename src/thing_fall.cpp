//
// Copyright goblinhack@gmail.com
//

#include "my_bpoint.hpp"
#include "my_callstack.hpp"
#include "my_dice_rolls.hpp"
#include "my_game_defs.hpp"
#include "my_game_inlines.hpp"
#include "my_globals.hpp"
#include "my_level.hpp"
#include "my_level_inlines.hpp"
#include "my_main.hpp"
#include "my_random.hpp"
#include "my_thing.hpp"
#include "my_thing_callbacks.hpp"
#include "my_thing_inlines.hpp"
#include "my_tp.hpp"
#include "my_types.hpp"
#include "my_ui.hpp"
#include <cstdint>

//
// Ok to land on this spot?
//
[[nodiscard]] static auto thing_ok_landing_spot(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE();

  if (is_oob_or_border(p)) [[unlikely]] {
    return false;
  }

  if (level_is_obs_to_falling_onto_bool(g, v, l, p)) {
    return false;
  }

  return true;
}

//
// Try to find a spot close to where we landed that is ok to exist in.
// Try to avoid landing in lava if luck allows.
//
static auto thing_choose_landing_spot(Gamep g, Levelsp v, Levelp l, Thingp me, bool &saved) -> bpoint
{
  TRACE();

  auto at = thing_at(g, v, l, me);

  for (auto dist = 0; dist < MAP_WIDTH; dist++) {
    for (auto dx = -dist; dx <= dist; dx++) {
      for (auto dy = -dist; dy <= dist; dy++) {

        bpoint where;

        where.x = at.x + dx;
        where.y = at.y + dy;

        if (is_oob(where)) {
          continue;
        }

        if (thing_ok_landing_spot(g, v, l, where)) {
          //
          // If lucky, no landing on lava
          //
          if (thing_is_player(me) && (level_is_lava(g, v, l, where) != nullptr)) {
            if (thing_stat_success(g, v, l, me, THING_STAT_LUCK, TARGET_ROLL_FALL)) {
              //
              // Continue to look for a landing spot
              //
              saved = true;
              continue;
            }
            return where;
          }
          return where;
        }
      }
    }
  }

  saved = false;

  return at;
}

//
// Try to find a spot close to where we landed that is ok to exist in.
// i.e. no landing inside walls.
//
static auto thing_choose_landing_spot(Gamep g, Levelsp v, Levelp l, Thingp me) -> bpoint
{
  TRACE();

  bool saved {};
  auto where = thing_choose_landing_spot(g, v, l, me, saved);

  if (thing_is_player(me)) {
    if (saved) {
      if (level_is_lava(g, v, l, where) != nullptr) {
        topcon(UI_IMPORTANT_FMT_STR "You manage to avoid the lava as you fall. And then somehow, still slip into it!" UI_RESET_FMT);
      } else {
        topcon(UI_IMPORTANT_FMT_STR "You manage to avoid the lava as you fall!" UI_RESET_FMT);
      }
    }
  }

  return where;
}

//
// How much damage does the thing take on falling.
//
static auto thing_fall_damage(Gamep g, Levelsp v, Levelp l, Thingp me) -> int
{
  TRACE();

  int fall_damage = 0;

  if (thing_is_player(me)) {
    fall_damage = PCG_RANDOM_RANGE(6, 30);
  } else if (thing_is_mob(me) || thing_is_monst(me)) {
    fall_damage = PCG_RANDOM_RANGE(6, thing_health(g, v, l, me) / 2);
  } else {
    fall_damage = d4();
  }

  if (thing_is_falling_continues(me)) {
    fall_damage *= 2;
  }

  //
  // Landing in lava is bad
  //
  if (level_is_lava_bool(g, v, l, thing_at(g, v, l, me))) {
    if (! thing_is_immune_to(g, v, l, me, THING_EVENT_FIRE_DAMAGE)) {
      fall_damage *= 4;
    }
  }

  //
  // Water dampens the fall
  //
  if (level_is_water_shallow_bool(g, v, l, thing_at(g, v, l, me))) {
    if (thing_is_immune_to(g, v, l, me, THING_EVENT_WATER_DAMAGE)) {
      fall_damage /= 2;
    }
  }

  //
  // Deep water dampens it further
  //
  if (level_is_water_deep(g, v, l, thing_at(g, v, l, me)) != nullptr) {
    if (thing_is_immune_to(g, v, l, me, THING_EVENT_WATER_DAMAGE)) {
      fall_damage /= 2;
    }
  }

  //
  // Amplify or nullify due to weight
  //
  auto w = thing_weight(me);
  if (w <= WEIGHT_FEATHER) {
    fall_damage = 0;
  } else if (w >= WEIGHT_HEAVY) {
    fall_damage *= 2;
  }

  return fall_damage;
}

//
// Complete the fall to the next level. If this is the player we also change level.
//
static void thing_fall_end(Gamep g, Levelsp v, Levelp l, Thingp me)
{
  THING_DBG(g, v, l, me, "fall end");
  TRACE_INDENT();

  auto *next_level = level_select_get_next_level_down(g, v, l);
  if (next_level == nullptr) {
    if (g_opt_tests) {
      THING_DBG(g, v, l, me, "no level to fall onto");
    } else {
      thing_err(g, v, l, me, "no level to fall onto");
    }
    return;
  }

  //
  // We can fall into the same level if falling on a boss level
  //
  if (compiler_unused) {
    if (next_level == l) {
      ThingEvent e {
          .reason     = "by falling into nothing",      //
          .event_type = THING_EVENT_FALL,               //
          .damage     = thing_fall_damage(g, v, l, me), //
      };

      THING_DBG(g, v, l, me, "dead due to falling into nothing");
      TRACE_INDENT();

      thing_dead(g, v, l, me, e);
      return;
    }
  }

  //
  // You cannot fall out of boss levels
  //
  switch (level_type(l->level_num + 1)) {
    case LEVEL_TYPE_NONE :   [[fallthrough]];
    case LEVEL_TYPE_NORMAL : [[fallthrough]];
    case LEVEL_TYPE_TEST :   [[fallthrough]];
    case LEVEL_TYPE_ENUM_MAX :
      //
      // Choose a new landing spot for the thing
      //
      if (! thing_warp_to(g, v, next_level, me, thing_choose_landing_spot(g, v, next_level, me))) {
        if (thing_is_player(me)) {
          topcon(UI_IMPORTANT_FMT_STR "You fail to find the ground!" UI_RESET_FMT);
        }
      }
      break;

    case LEVEL_TYPE_BOSS1 : [[fallthrough]];
    case LEVEL_TYPE_BOSS2 : [[fallthrough]];
    case LEVEL_TYPE_BOSS3 : [[fallthrough]];
    case LEVEL_TYPE_BOSS4 : [[fallthrough]];
    case LEVEL_TYPE_BOSS5 :
      //
      // Back to the entrance
      //
      next_level = l;
      break;
  }

  //
  // Ensure we always land at the entrance for boss levels
  //
  if (level_is_boss_level(g, v, next_level)) {
    thing_level_warp_to_entrance(g, v, next_level, me);
  }

  if (thing_is_player(me)) {
    level_scroll_warp_to_focus(g, v, l);
  }

  ThingEvent e {
      .reason     = "by falling",                   //
      .event_type = THING_EVENT_FALL,               //
      .damage     = thing_fall_damage(g, v, l, me), //
  };

  //
  // "You tumble into the void"
  //
  if (thing_is_player(me)) {
    player_fell(g, v, l, next_level, me);
  }

  auto *t_level = game_level_get(g, v, me->level_num);
  if (t_level != nullptr) {
    l = t_level;
  } else {
    thing_err(g, v, l, me, "fell into nothing");
  }

  if (level_is_chasm(g, v, t_level, thing_at(g, v, l, me)) != nullptr) {
    //
    // If we keep on falling through chasms again and again though,
    // we need to break the potential fall loop.
    //
    if (thing_is_falling_continues(me)) {
      if (! thing_is_able_to_fall_repeatedly(me)) {
        e.reason = "repeated falling";

        THING_DBG(g, v, l, me, "dead due to repeated falling");
        TRACE_INDENT();

        thing_dead(g, v, l, me, e);
      }
    }

    //
    // Keep falling with no damage if over a chasm.
    //
    THING_DBG(g, v, l, me, "over a chasm again; keep falling");
    TRACE_INDENT();
    thing_is_falling_continues_set(g, v, l, me);
    thing_is_falling_set(g, v, l, me, true);

    if (thing_is_player(me)) {
      topcon(UI_IMPORTANT_FMT_STR "You continue to fall!" UI_RESET_FMT);
    }
  } else {
    //
    // "You take n damage from falling"
    //
    thing_damage_apply(g, v, l, me, e);
    thing_is_falling_continues_unset(g, v, l, me);
  }

  //
  // Falling can be good
  //
  if (thing_is_burning(me)) {
    thing_is_burning_unset(g, v, l, me);

    if (thing_is_player(me)) {
      topcon(UI_GOOD_FMT_STR "You extinguish the flames as you fall!" UI_RESET_FMT);
    }
  }

  //
  // Splash!
  //
  if (level_is_water_deep(g, v, l, thing_at(g, v, l, me)) != nullptr) {
    thing_sound_play(g, v, l, me, "splash");
    if (thing_is_player(me)) {
      topcon(UI_GOOD_FMT_STR "The deep water dampened your fall!" UI_RESET_FMT);
    }
  } else if (level_is_water_shallow_bool(g, v, l, thing_at(g, v, l, me))) {
    thing_sound_play(g, v, l, me, "splash");
    if (thing_is_player(me)) {
      topcon(UI_GOOD_FMT_STR "The water dampened your fall!" UI_RESET_FMT);
    }
  }
}

//
// Falling time step
//
void thing_fall_time_step(Gamep g, Levelsp v, Levelp l, Thingp me, int time_step)
{
  TRACE();

  (void) thing_is_falling_incr(g, v, l, me, time_step);

  if (compiler_unused) {
    THING_DBG(g, v, l, me, "fall incr %u", thing_is_falling(me));
  }
}

//
// Check if we're done falling
//
void thing_fall_end_check(Gamep g, Levelsp v, Levelp l, Thingp me)
{
  TRACE();

  if (compiler_unused) {
    THING_DBG(g, v, l, me, "fall %u", thing_is_falling(me));
    TRACE_INDENT();
  }

  //
  // Fallen for long enough?
  //
  bool fall_finished = thing_is_falling(me) >= THING_FALL_ANIM_MS;

  //
  // If the player cannot see the thing fall, then complete the fall to avoid
  // any stuttering in frame rate
  //
  if (! fall_finished) {
    auto *player = thing_player(g);
    if (player != nullptr) {
      if (me != player) {
        if (! thing_vision_can_see_tile(g, v, l, player, thing_at(g, v, l, me))) {
          fall_finished = true;
        }
      }
    }
  }

  //
  // Fall complete?
  //
  if (fall_finished) {
    THING_DBG(g, v, l, me, "fall finished");
    TRACE_INDENT();

    thing_fall_end(g, v, l, me);

    auto *t_level = game_level_get(g, v, me->level_num);
    if (t_level == nullptr) {
      thing_err(g, v, l, me, "fell into nothing");
    }

    thing_is_falling_set(g, v, t_level, me, false);

    if (thing_is_player(me)) {
      if (level_is_chasm(g, v, t_level, thing_at(g, v, l, me)) != nullptr) {
        THING_DBG(g, v, t_level, me, "fell again");
        TRACE_INDENT();
        (void) level_tick_begin_requested(g, v, t_level, "player fell again");
      }

      //
      // This seems rather cruel, but...
      //
      if (level_is_lava(g, v, t_level, thing_at(g, v, l, me)) != nullptr) {
        THING_DBG(g, v, t_level, me, "fell into lava");
        TRACE_INDENT();
        (void) level_tick_begin_requested(g, v, t_level, "player fell into lava");
      }

      player_move_requests_reset(g, v);
    }
  }
}

//
// Begin falling
//
void thing_fall(Gamep g, Levelsp v, Levelp l, Thingp me)
{
  TRACE();

  //
  // Fall at the end of the move, or it just looks odd with things falling
  // through the floor before they get to the chasm
  //
  if (thing_is_moving(me)) {
    return;
  }

  if (! thing_is_able_to_fall(g, v, l, me)) {
    return;
  }

  if (thing_is_able_to_fall_sound(me)) {
    thing_sound_play(g, v, l, me, "fall");
  }

  thing_is_falling_set(g, v, l, me, true);

  //
  // Watching some poor thing fall?
  //
  if (thing_is_monst(me)) {
    auto *player = thing_player(g);
    if (player != nullptr) {
      if (thing_vision_can_see_tile(g, v, l, player, thing_at(g, v, l, me))) {
        auto The_thing = thing_name_long_The(g, v, l, me);
        topcon("%s tumbles into the chasm.", The_thing.c_str());
      }
    }
  }

  //
  // Drag the engulfed
  //
  if (thing_is_able_to_engulf(me)) {
    FOR_ALL_THINGS_AT(g, v, l, it, thing_at(g, v, l, me))
    {
      if (thing_is_engulfed(it)) {
        THING_DBG(g, v, l, it, "is engulfed and needs to follow the engulferd down");
        (void) thing_is_engulfed_try_unset(g, v, l, it);
        thing_fall(g, v, l, it);
      }
    }
  }
}

[[nodiscard]] auto thing_is_able_to_fall(Gamep g, Levelsp v, Levelp l, Thingp me) -> bool
{
  TRACE_DEBUG();

  if (me == nullptr) {
    ERR("no thing pointer");
    return false;
  }

  //
  // Can'me fall when levitating
  //
  if (thing_is_levitating(g, v, l, me)) {
    return false;
  }

  //
  // Allow dead floating monsters to fall
  //
  if (thing_is_monst(me)) {
    if (thing_is_dead(me)) {
      return true;
    }
  }

  return tp_flag(thing_tp(me), is_able_to_fall) != 0;
}

[[nodiscard]] auto thing_is_able_to_fall_repeatedly(Thingp me) -> bool
{
  TRACE_DEBUG();

  if (me == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(me), is_able_to_fall_repeatedly) != 0;
}

[[nodiscard]] auto thing_is_obs_to_falling_onto(Thingp me) -> bool
{
  TRACE_DEBUG();

  if (me == nullptr) {
    ERR("no thing pointer");
    return false;
  }

  //
  // Unless open
  //
  if (thing_is_openable(me)) {
    if (thing_is_open(me)) {
      return false;
    }
  }

  return tp_flag(thing_tp(me), is_obs_to_falling_onto) != 0;
}

void thing_is_falling_set(Gamep g, Levelsp v, Levelp l, Thingp me, bool val)
{
  TRACE_DEBUG();

  if (me == nullptr) {
    ERR("no thing pointer");
    return;
  }

  //
  // Once falling, it is treated as a counter
  //
  if (val) {
    //
    // Start falling if not doing do
    //
    if (static_cast< bool >(me->_fall_ms)) {
      return;
    }
  } else {
    //
    // Stop falling
    //
    if (! static_cast< bool >(me->_fall_ms)) {
      return;
    }
  }
  me->_fall_ms = static_cast< uint16_t >(val);

  if (val) {
    thing_on_fall_begin(g, v, l, me);
  } else {
    thing_on_fall_end(g, v, l, me);
  }
}

[[nodiscard]] auto thing_is_falling_incr(Gamep g, Levelsp v, Levelp l, Thingp me, int val) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    ERR("no thing pointer");
    return 0;
  }

  if (me->_fall_ms + val > THING_FALL_ANIM_MS) {
    return me->_fall_ms = THING_FALL_ANIM_MS;
  }

  return me->_fall_ms += val;
}

[[nodiscard]] auto thing_is_falling_continues(Thingp me) -> bool
{
  TRACE_DEBUG();

  if (me == nullptr) {
    ERR("no thing pointer");
    return false;
  }

  return me->_is_falling_continues;
}

void thing_is_falling_continues_set(Gamep g, Levelsp v, Levelp l, Thingp me, bool val)
{
  TRACE_DEBUG();

  if (me == nullptr) {
    ERR("no thing pointer");
    return;
  }

  if (me->_is_falling_continues == static_cast< int >(val)) {
    return;
  }
  me->_is_falling_continues = val;

  if (val) {
    THING_DBG(g, v, l, me, "is falling continues set");
  }

  level_request_to_cleanup_things_set(g, v, l);
}

void thing_is_falling_continues_unset(Gamep g, Levelsp v, Levelp l, Thingp me)
{
  TRACE_DEBUG();

  thing_is_falling_continues_set(g, v, l, me, false);
}

[[nodiscard]] auto thing_is_able_to_fall_sound(Thingp me) -> bool
{
  TRACE_DEBUG();

  if (me == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(me), is_able_to_fall_sound) != 0;
}
