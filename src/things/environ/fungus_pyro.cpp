//
// Copyright goblinhack@gmail.com
//

#include "../../my_callstack.hpp"
#include "../../my_dice_rolls.hpp"
#include "../../my_level_inlines.hpp"
#include "../../my_thing_callbacks.hpp"
#include "../../my_thing_inlines.hpp"
#include "../../my_tile.hpp"
#include "../../my_tp.hpp"
#include "../../my_tps.hpp"
#include "../../my_types.hpp"
#include "../../my_ui.hpp"

static auto tp_fungus_pyro_description_get(Gamep g, Levelsp v, Levelp l, Thingp me) -> std::string
{
  TRACE();

  return "bright orange fungus, looks spicy.";
}

static auto tp_fungus_pyro_detail_get(Gamep g, Levelsp v, Levelp l, Thingp me) -> std::string
{
  TRACE();

  return                                                                                                                     //
      UI_INFO1_FMT_STR "This bright orange fungus reproduces through means of spreading its spores on wisps of fire.\n"      //
      UI_INFO2_FMT_STR "The slightest touch can set off a firestorm, so beware, unless you become pyrofungus fertilizer.\n"; //
}

static bool tp_fungus_pyro_spore(Gamep g, Levelsp v, Levelp l, Thingp me, ThingEvent &e)
{
  TRACE();

  auto at = thing_at(g, v, l, me);

  const std::initializer_list< bpoint > points = {
      bpoint(-1, -1), bpoint(0, -1), bpoint(1, -1), bpoint(-1, 0), bpoint(1, 0), bpoint(-1, 1), bpoint(0, 1), bpoint(1, 1),
  };

  //
  // Spawn adjacent fire
  //
  for (auto delta : points) {
    auto p = at + delta;

    if (is_oob(p)) {
      continue;
    }

    //
    // Rock, for example?
    //
    if (level_is_obs_to_fire_bool(g, v, l, p)) {
      continue;
    }

    //
    // Some other gas is here already, don't spawn more
    //
    if (level_is_fire_bool(g, v, l, p)) {
      continue;
    }

    THING_DBG(g, v, l, me, "spawn fire");

    (void) thing_spawn(g, v, l, tp_first(is_fire_spready), p);
  }

  return true;
}

static void tp_fungus_pyro_on_death(Gamep g, Levelsp v, Levelp l, Thingp me, ThingEvent &e)
{
  TRACE();

  THING_DBG(g, v, l, me, "dead, spore");
  (void) tp_fungus_pyro_spore(g, v, l, me, e);
}

static bool tp_fungus_pyro_on_damage(Gamep g, Levelsp v, Levelp l, Thingp me, ThingEvent &e)
{
  TRACE();

  THING_DBG(g, v, l, me, "damaged, spore");
  return tp_fungus_pyro_spore(g, v, l, me, e);
}

[[nodiscard]] auto tp_load_pyro_fungus() -> bool
{
  TRACE();

  auto *tp   = tp_load("fungus_pyro"); // keep as string for scripts
  auto  name = tp_name(tp);

  // begin sort marker1 {
  thing_description_set(tp, tp_fungus_pyro_description_get);
  thing_detail_set(tp, tp_fungus_pyro_detail_get);
  thing_on_damage_set(tp, tp_fungus_pyro_on_damage);
  thing_on_death_set(tp, tp_fungus_pyro_on_death);
  tp_chance_set(tp, THING_CHANCE_CONTINUE_TO_BURN, "1d2"); // fumble => intensify / keep burning / crit => stop burning
  tp_chance_set(tp, THING_CHANCE_START_BURNING, "1d2");    // fumble => flames spread to you
  tp_distance_light_penetration_pixels_set(tp, TILE_WIDTH / 2);
  tp_flag_set(tp, is_able_to_fall);
  tp_flag_set(tp, is_always_hit);
  tp_flag_set(tp, is_attackable_by_monst);
  tp_flag_set(tp, is_attackable_by_player);
  tp_flag_set(tp, is_blit_hit_outline_w_black_inside);
  tp_flag_set(tp, is_blit_if_has_seen);
  tp_flag_set(tp, is_blit_obscures);
  tp_flag_set(tp, is_blit_on_ground);
  tp_flag_set(tp, is_blit_shown_in_chasms);
  tp_flag_set(tp, is_collision_circle_large);
  tp_flag_set(tp, is_described_cursor);
  tp_flag_set(tp, is_fungus);
  tp_flag_set(tp, is_loggable);
  tp_flag_set(tp, is_obs_to_movement);
  tp_flag_set(tp, is_obs_to_vision);
  tp_flag_set(tp, is_physics_explosion);
  tp_flag_set(tp, is_physics_temperature);
  tp_flag_set(tp, is_removable_when_dead_on_err);
  tp_flag_set(tp, is_soft_landing);
  tp_flag_set(tp, is_submergible);
  tp_flag_set(tp, is_tickable);
  tp_health_set(tp, "1d5");
  tp_is_immune_to_add(tp, THING_EVENT_FIRE_DAMAGE);
  tp_is_immune_to_add(tp, THING_EVENT_WATER_DAMAGE);
  tp_name_a_or_an_set(tp, "pyro fungus");
  tp_name_apostrophize_set(tp, "pyro fungi'");
  tp_name_long_set(tp, "pyro fungus");
  tp_name_pluralize_set(tp, "pyro fungus");
  tp_name_short_set(tp, "pyro fungus");
  tp_priority_set(tp, THING_PRIORITY_FOLIAGE);
  tp_temperature_burns_at_set(tp, 1000); // celsius
  tp_temperature_damage_at_set(tp, 500); // celsius
  tp_temperature_initial_set(tp, 50);    // celsius
  tp_weight_set(tp, WEIGHT_LIGHT);       // grams
  tp_z_depth_set(tp, MAP_Z_DEPTH_FOLIAGE);
  // end sort marker1 }

  for (auto frame = 0; frame < 5; frame++) {
    auto *tile = tile_find_mand(name + std::string(".idle.") + std::to_string(frame));
    tile_size_set(tile, OUTLINE_TILE_WIDTH, OUTLINE_TILE_HEIGHT);
    tp_tiles_push_back(tp, THING_ANIM_IDLE, tile);
  }

  return true;
}
