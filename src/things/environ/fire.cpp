//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_dice_rolls.hpp"
#include "my_level.hpp"
#include "my_level_inlines.hpp"
#include "my_main.hpp"
#include "my_thing.hpp"
#include "my_thing_callbacks.hpp"
#include "my_thing_inlines.hpp"
#include "my_tile.hpp"
#include "my_tp.hpp"
#include "my_tps.hpp"
#include "my_types.hpp"

static auto tp_fire_description_get(Gamep g, Levelsp v, Levelp l, Thingp t) -> std::string
{
  TRACE();

  return "brightly burning fire";
}

static void tp_fire_tick_begin(Gamep g, Levelsp v, Levelp l, Thingp t)
{
  TRACE();

  //
  // Don't spawn fire too soon after creation or we get a firestorm
  //
  if (thing_age(t) <= 1) {
    return;
  }

  const std::initializer_list< bpoint > points = {
      bpoint(-1, -1), bpoint(0, -1), bpoint(1, -1), bpoint(-1, 0), bpoint(1, 0), bpoint(-1, 1), bpoint(0, 1), bpoint(1, 1),
  };

  //
  // Spawn adjacent fire
  //
  for (auto delta : points) {
    auto at = thing_at(t);
    auto p  = at + delta;

    //
    // Rock, for example?
    //
    if (level_is_obs_to_fire(g, v, l, p) != nullptr) {
      continue;
    }

    //
    // Fire is here already, don't spawn more
    //
    if (level_is_fire(g, v, l, p) != nullptr) {
      continue;
    }

    //
    // Give the fire a pause
    //
    if (level_is_smoke(g, v, l, p) != nullptr) {
      continue;
    }

    //
    // Nothing to burn here?
    //
    if (level_alive_is_burnable(g, v, l, p) == nullptr) {
      continue;
    }

    if (d100() < 20 + (thing_age(t) * 10)) {
      //
      // The older the fire gets, the more chance of spreading
      //
      if (compiler_unused) {
        log("fire spread check: ok");
      }
    } else {
      //
      // Too young to spread fire.
      //
      if (compiler_unused) {
        log("fire spread check; too young");
      }
      continue;
    }

    //
    // Give things a chance of not catching on fire
    //
    bool something_to_burn_here = false;

    FOR_ALL_THINGS_AT(g, v, l, it, p)
    {
      if (tp_chance_success(thing_tp(it), THING_CHANCE_START_BURNING)) {
        THING_DBG(it, "can spawn fire here");
        something_to_burn_here = true;
      }
    }

    if (! something_to_burn_here) {
      continue;
    }

    THING_DBG(t, "spawn spreading fire");
    (void) thing_spawn(g, v, l, tp_first(is_fire), p);
  }
}

static void tp_fire_on_death(Gamep g, Levelsp v, Levelp l, Thingp t, ThingEvent &e)
{
  TRACE();

  //
  // Allow things to continue to burn if we still have some burnable material
  //
  if (level_alive_is_combustible(g, v, l, thing_at(t)) != nullptr) {
    if (level_is_fire(g, v, l, thing_at(t)) == nullptr) {
      THING_DBG(t, "spawn fire to continue to burn");
      (void) thing_spawn(g, v, l, tp_first(is_fire), t);
    }
  }

  if (level_is_smoke(g, v, l, thing_at(t)) == nullptr) {
    if (level_is_combustible(g, v, l, thing_at(t)) != nullptr) {
      THING_DBG(t, "spawn smoke over dying fire");
      (void) thing_spawn(g, v, l, tp_first(is_smoke), t);
    }
  }
}

static void tp_fire_on_fall_begin(Gamep g, Levelsp v, Levelp l, Thingp t)
{
  TRACE();

  //
  // I quite like the idea of fire falling to the level below
  //
  // Unless the player is on fire. In which case we want the flames to
  // die, else they follow them down and they stay on fire.
  //
  auto *player = thing_player(g);
  auto  at     = thing_at(t);
  if ((player != nullptr) && (at == thing_at(player))) {
    ThingEvent e {
        .reason     = "by falling",     //
        .event_type = THING_EVENT_FALL, //
    };

    THING_DBG(t, "dead due to falling");
    TRACE_INDENT();

    thing_dead(g, v, l, t, e);
    return;
  }

  if (level_is_smoke(g, v, l, thing_at(t)) == nullptr) {
    THING_DBG(t, "spawn smoke over falling fire");
    (void) thing_spawn(g, v, l, tp_random(g, v, l, is_smoke), t);
  }
}

auto tp_load_fire() -> bool
{
  TRACE();

  auto *tp   = tp_load("fire"); // keep as string for scripts
  auto  name = tp_name(tp);

  // begin sort marker1 {
  thing_description_set(tp, tp_fire_description_get);
  thing_on_death_set(tp, tp_fire_on_death);
  thing_on_fall_begin_set(tp, tp_fire_on_fall_begin);
  thing_on_tick_begin_set(tp, tp_fire_tick_begin);
  tp_damage_set(tp, THING_EVENT_FIRE_DAMAGE, "1d6");
  tp_flag_set(tp, is_able_to_fall);
  tp_flag_set(tp, is_animated);
  tp_flag_set(tp, is_blit_centered);
  tp_flag_set(tp, is_blit_if_has_seen);
  tp_flag_set(tp, is_blit_shown_in_chasms);
  tp_flag_set(tp, is_blit_shown_in_overlay);
  tp_flag_set(tp, is_cursor_path_hazard);
  tp_flag_set(tp, is_cursor_path_warning);
  tp_flag_set(tp, is_described_cursor);
  tp_flag_set(tp, is_fire);
  tp_flag_set(tp, is_gaseous);
  tp_flag_set(tp, is_light_source, 5);
  tp_flag_set(tp, is_loggable);
  tp_flag_set(tp, is_physics_temperature);
  tp_flag_set(tp, is_physics_water);
  tp_flag_set(tp, is_removable_on_err);
  tp_flag_set(tp, is_submergible);
  tp_flag_set(tp, is_tick_end_delay);
  tp_flag_set(tp, is_tickable);
  tp_health_set(tp, "1d5"); // to allow it to be damaged by water
  tp_is_immunity_add(tp, THING_EVENT_FIRE_DAMAGE);
  tp_is_immunity_add(tp, THING_EVENT_HEAT_DAMAGE);
  tp_lifespan_set(tp, "1d6+3");
  tp_light_color_set(tp, "orange");
  tp_name_a_or_an_set(tp, "fire");
  tp_name_apostrophize_set(tp, "fires'");
  tp_name_long_set(tp, "fire");
  tp_name_pluralize_set(tp, "fires");
  tp_name_short_set(tp, "fire");
  tp_priority_set(tp, THING_PRIORITY_FIRE);
  tp_temperature_burns_at_set(tp, 500); // celsius
  tp_temperature_initial_set(tp, 500);  // celsius
  tp_weight_set(tp, WEIGHT_NONE);       // grams
  tp_z_depth_set(tp, MAP_Z_DEPTH_GAS);
  // end sort marker1 }

  auto delay = 200;

  for (auto frame = 0; frame < 16; frame++) {
    auto *tile = tile_find_mand(name + std::string(".idle.") + std::to_string(frame));
    tile_size_set(tile, OUTLINE_TILE_WIDTH, OUTLINE_TILE_HEIGHT);
    tile_delay_ms_set(tile, delay);
    tp_tiles_push_back(tp, THING_ANIM_IDLE, tile);
  }

  return true;
}
