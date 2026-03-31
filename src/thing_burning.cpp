//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_main.hpp"
#include "my_thing_inlines.hpp"
#include "my_ui.hpp"

//
// Do actions upon burning
//
void thing_is_burning_handle(Gamep g, Levelsp v, Levelp l, Thingp t)
{
  TRACE();

  if (thing_is_burning(t)) {
    if (thing_is_player(t)) {
      topcon(UI_IMPORTANT_FMT_STR "You are burning!" UI_RESET_FMT);
    }
  }
}

void thing_continue_to_burn_check(Gamep g, Levelsp v, Levelp l, Thingp t)
{
  TRACE();

  auto *tp = thing_tp(t);

  if (! thing_is_burning(t)) {
    return;
  }

  //
  // Over water?
  //
  if (level_is_water(g, v, l, thing_at(t)) != nullptr) {
    thing_is_burning_unset(g, v, l, t);

    if (thing_is_player(t)) {
      topcon(UI_GOOD_FMT_STR "You extinguish the flames in the cool water!" UI_RESET_FMT);
    }

    if (level_is_steam(g, v, l, thing_at(t)) == nullptr) {
      (void) thing_spawn(g, v, l, tp_first(is_steam), t);
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
    thing_is_burning_unset(g, v, l, t);

    if (thing_is_player(t)) {
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
    if (thing_is_combustible(t)) {
      if (level_count_is_fire(g, v, l, thing_at(t)) < 2) {
        THING_DBG(t, "spawn additional flames");
        TRACE_INDENT();
        (void) thing_spawn(g, v, l, tp_first(is_fire), t);

        if (thing_is_player(t)) {
          topcon(UI_IMPORTANT_FMT_STR "The flames intensify!" UI_RESET_FMT);
        }
      }
    }

    //
    // Don't let the fire age out.
    //
    auto *f = level_is_fire(g, v, l, thing_at(t));
    if (f != nullptr) {
      THING_DBG(t, "keep the fire burning");
      (void) thing_lifespan_incr(g, v, l, f, 2);
    }
  }

  //
  // Continue to burn
  //
  if (level_is_fire(g, v, l, thing_at(t)) == nullptr) {
    THING_DBG(t, "spawn flames");
    (void) thing_spawn(g, v, l, tp_first(is_fire), t);
  }

  if (level_is_smoke(g, v, l, thing_at(t)) == nullptr) {
    THING_DBG(t, "spawn smoke");
    (void) thing_spawn(g, v, l, tp_first(is_smoke), t);
  }
}
