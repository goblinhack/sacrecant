//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_level.hpp"
#include "my_main.hpp"
#include "my_thing.hpp"
#include "my_thing_callbacks.hpp"
#include "my_thing_inlines.hpp"
#include "my_tp.hpp"
#include "my_types.hpp"
#include "my_ui.hpp"

//
// Called at the beginning of each tick
//
void thing_tick_begin(Gamep g, Levelsp v, Levelp l, Thingp me)
{
  TRACE();

  auto at = thing_at(g, v, l, me);

  //
  // Check if still engulfed
  //
  if (thing_is_engulfed(me)) {
    if (level_alive_is_able_to_engulf(g, v, l, at) == nullptr) {
      if (thing_is_player(me)) {
        topcon(UI_IMPORTANT_FMT_STR "You seem to be free!" UI_RESET_FMT);
      }
      (void) thing_is_engulfed_try_unset(g, v, l, me);
    }
  }

  //
  // Check if still ensnared
  //
  if (thing_is_ensnared(me)) {
    if (level_alive_is_able_to_ensnare(g, v, l, at) == nullptr) {
      if (thing_is_player(me)) {
        topcon(UI_IMPORTANT_FMT_STR "You seem to be free!" UI_RESET_FMT);
      }
      (void) thing_is_ensnared_try_unset(g, v, l, me);
    }
  }

  //
  // Update hidden and submerged status
  //
  if (! thing_is_moving(me)) {
    //
    // Only if not moving, else the player appears to jump briefly forward
    // before being interpolated
    //
    thing_update_pos(g, v, l, me);
  }

  //
  // Reset damage counts.
  //
  (void) thing_damage_this_tick_set(g, v, l, me, 0);

  //
  // Reset noise counts
  //
  (void) thing_noise_decr(g, v, l, me, 1);

  //
  // Reset attack counts.
  //
  (void) thing_attack_count_per_tick_set(g, v, l, me, 0);

  //
  // Per thing callback
  //
  thing_on_tick_begin(g, v, l, me);

  //
  // Resurrection?
  //
  if (thing_is_corpse(me)) {
    if (thing_is_able_to_resurrect(me)) {
      if (thing_ticks_to_stay_dead_initial(me) != 0) {
        if (thing_ticks_to_stay_dead_decr(g, v, l, me) == 0) {
          if (tp_chance_success(thing_tp(me), THING_CHANCE_RESURRECTION)) {
            (void) thing_resurrect(g, v, l, me);
          }
        }
      } else {
        if (tp_chance_success(thing_tp(me), THING_CHANCE_RESURRECTION)) {
          (void) thing_resurrect(g, v, l, me);
        }
      }
    }
  }

  if (thing_is_dead(me)) {
    return;
  }

  //
  // Lifespan tick
  //
  if (thing_lifespan_initial(me) != 0) {
    if (thing_lifespan_decr(g, v, l, me) == 0) {
      ThingEvent e {
          .reason     = "ran out of life",            //
          .event_type = THING_EVENT_LIFESPAN_EXPIRED, //
      };

      THING_DBG(g, v, l, me, "dead due to out of life");
      TRACE_INDENT();

      thing_dead(g, v, l, me, e);
      return;
    }
  }

  (void) thing_age_incr(g, v, l, me);

  //
  // Tick buffs and items
  //
  FOR_ALL_HOOKS(g, v, l, me, buff) { thing_on_tick_begin(g, v, l, buff); }

  FOR_ALL_ACTIVE_ITEMS(g, v, l, me, item) { thing_on_tick_begin(g, v, l, item); }

  thing_continue_to_burn_check(g, v, l, me);
  if (thing_is_dead(me)) {
    return;
  }
}

//
// Called when the level is idle. i.e. multiple times per tick
//
void thing_tick_idle(Gamep g, Levelsp v, Levelp l, Thingp me)
{
  TRACE();

  //
  // Per thing callback
  //
  thing_on_tick_idle(g, v, l, me);
}

//
// Called at the end of each tick
//
void thing_tick_end(Gamep g, Levelsp v, Levelp l, Thingp me)
{
  TRACE();

  //
  // Per thing callback
  //
  thing_on_tick_end(g, v, l, me);

  //
  // Mark the tick as completed
  //
  if (me->tick == v->tick) {
    return;
  }
  me->tick = v->tick;
}
