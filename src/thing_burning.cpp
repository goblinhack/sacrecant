//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_dice_rolls.hpp"
#include "my_level.hpp"
#include "my_main.hpp"
#include "my_thing.hpp"
#include "my_thing_inlines.hpp"
#include "my_tp.hpp"
#include "my_tp_inlines.hpp"
#include "my_types.hpp"
#include "my_ui.hpp"

void thing_continue_to_burn_check(Gamep g, Levelsp v, Levelp l, Thingp me)
{
  TRACE();

  auto *tp = thing_tp(me);

  if (! thing_is_burning(me)) {
    return;
  }

  //
  // Over water?
  //
  if (level_is_water_deep_bool(g, v, l, thing_at(g, v, l, me))) {
    thing_is_burning_unset(g, v, l, me);

    if (thing_is_player(me)) {
      topcon(UI_GOOD_FMT_STR "You extinguish the flames in the deep water!" UI_RESET_FMT);
    }

    return;
  }

  if (level_is_water_shallow_bool(g, v, l, thing_at(g, v, l, me))) {
    thing_is_burning_unset(g, v, l, me);

    if (thing_is_player(me)) {
      topcon(UI_GOOD_FMT_STR "You extinguish the flames in the cool water!" UI_RESET_FMT);
    }

    if (! level_is_steam_bool(g, v, l, thing_at(g, v, l, me))) {
      (void) thing_spawn(g, v, l, tp_first(is_steam), me);
    }
    return;
  }

  //
  // Good luck
  //
  if (tp_chance_success(tp, THING_CHANCE_CONTINUE_TO_BURN)) {
    //
    // Poor thing
    //
    thing_is_burning_unset(g, v, l, me);

    if (thing_is_player(me)) {
      topcon(UI_GOOD_FMT_STR "You stop burning!" UI_RESET_FMT);
    }
    return;
  }

  //
  // Bad luck
  //
  if (tp_chance_fail(tp, THING_CHANCE_CONTINUE_TO_BURN)) {
    //
    // Spawn more flames?
    //
    if (! thing_is_levitating(g, v, l, me)) {
      if (thing_is_combustible(me)) {
        if (level_count_is_fire(g, v, l, thing_at(g, v, l, me)) < 2) {
          THING_DBG(g, v, l, me, "spawn additional flames");
          TRACE_INDENT();
          (void) thing_spawn(g, v, l, tp_first(is_fire_normal), me);

          if (thing_is_player(me)) {
            if (thing_is_immune_to(g, v, l, me, THING_EVENT_FIRE_DAMAGE)) {
              topcon(UI_IMPORTANT_FMT_STR "The flames intensify, but you don't care!" UI_RESET_FMT);
            } else {
              topcon(UI_IMPORTANT_FMT_STR "The flames intensify!" UI_RESET_FMT);
            }
          }
        }
      }
    }

    //
    // Don't let the fire age out.
    //
    auto *f = level_is_fire(g, v, l, thing_at(g, v, l, me));
    if (f != nullptr) {
      THING_DBG(g, v, l, me, "keep the fire burning");
      (void) thing_lifespan_incr(g, v, l, f, 2);
    }
  }

  ThingEvent e {
      .reason     = "continue to burn",
      .event_type = THING_EVENT_FIRE_DAMAGE, //
      .damage     = d4(),                    //
  };

  if (! level_is_fire_bool(g, v, l, thing_at(g, v, l, me))) {
    if (thing_is_burning(me)) {
      THING_DBG(g, v, l, me, "apply burning damage");
      TRACE_INDENT();

      thing_damage_apply(g, v, l, me, e);
    }
  }

  if (! level_is_smoke_bool(g, v, l, thing_at(g, v, l, me))) {
    THING_DBG(g, v, l, me, "spawn smoke");
    (void) thing_spawn(g, v, l, tp_first(is_smoke), me);
  }
}

[[nodiscard]] auto thing_is_burning(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return t->_is_burning;
}

void thing_is_burning_set(Gamep g, Levelsp v, Levelp l, Thingp t, bool val)
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return;
  }

  if (t->_is_burning == static_cast< int >(val)) {
    return;
  }
  t->_is_burning = val;

  if (val) {
    THING_DBG(g, v, l, t, "is burning set, %u degrees", thing_temperature(t));
  } else {
    //
    // Reset the temperature
    //
    auto *tp = thing_tp(t);
    (void) thing_temperature_set(g, v, l, t, tp_temperature_initial_get(tp));

    THING_DBG(g, v, l, t, "is no longer burning, %u degrees", thing_temperature(t));
  }
}

void thing_is_burning_unset(Gamep g, Levelsp v, Levelp l, Thingp t)
{
  TRACE_DEBUG();

  thing_is_burning_set(g, v, l, t, false);
}
