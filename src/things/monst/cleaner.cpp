//
// Copyright goblinhack@gmail.com
//

#include "../../my_callstack.hpp"
#include "../../my_thing.hpp"
#include "../../my_thing_callbacks.hpp"
#include "../../my_thing_inlines.hpp"
#include "../../my_tile.hpp"
#include "../../my_tp.hpp"
#include "../../my_tps.hpp"
#include "../../my_types.hpp"
#include "../../my_ui.hpp"

static auto tp_cleaner_description_get(Gamep g, Levelsp v, Levelp l, Thingp me) -> std::string
{
  TRACE();

  if (thing_is_dead(me)) {
    return "dead cleaner";
  }
  return "living mass of slime known as a cleaner";
}

static auto tp_cleaner_detail_get(Gamep g, Levelsp v, Levelp l, Thingp me) -> std::string
{
  TRACE();

  return                                                                                                                    //
      UI_INFO1_FMT_STR "Dungeon cleaners are mindless masses of jelly that like to eat treasure. And you.\n"                //
      UI_INFO2_FMT_STR "They move slowly and hoover up items to be digested slowly in their gloopy innards.\n"              //
      UI_INFO3_FMT_STR "Although sluggish, they can surprise with a sudden engulfing jump. Best not be where they land..."; //
}

static auto tp_cleaner_assess_tp(Gamep g, Levelsp v, Levelp l, Tpp tp, Thingp me) -> ThingEnvironType
{
  TRACE_DEBUG();

  if (tp_is_lava(tp)) {
    return THING_ENVIRON_HATES;
  }

  if (tp_is_fire(tp)) {
    return THING_ENVIRON_HATES;
  }

  if (tp_is_chasm(tp)) {
    return THING_ENVIRON_HATES;
  }

  if (tp_is_water_shallow(tp)) {
    return THING_ENVIRON_HATES;
  }

  if (tp_is_water_deep(tp)) {
    return THING_ENVIRON_HATES;
  }

  if (tp_is_treasure(tp)) {
    return THING_ENVIRON_LIKES;
  }

  if (tp_is_item(tp)) {
    return THING_ENVIRON_LIKES;
  }

  return THING_ENVIRON_NEUTRAL;
}

static auto tp_cleaner_assess_tile(Gamep g, Levelsp v, Levelp l, const bpoint &at, Thingp me) -> ThingEnvironType
{
  TRACE_DEBUG();

  if (level_is_lava_cached(g, v, l, at)) {
    return THING_ENVIRON_HATES;
  }

  if (level_is_fire_cached(g, v, l, at)) {
    return THING_ENVIRON_HATES;
  }

  if (level_is_chasm_cached(g, v, l, at)) {
    return THING_ENVIRON_HATES;
  }

  if (level_is_water_shallow_cached(g, v, l, at)) {
    return THING_ENVIRON_HATES;
  }

  if (level_is_water_deep_cached(g, v, l, at)) {
    return THING_ENVIRON_HATES;
  }

  if (level_is_treasure_cached(g, v, l, at)) {
    if (thing_inventory_get_item_count(g, v, l, me) < thing_items_collected_max(me)) {
      return THING_ENVIRON_LIKES;
    }
  }

  if (level_is_item_cached(g, v, l, at)) {
    if (thing_inventory_get_item_count(g, v, l, me) < thing_items_collected_max(me)) {
      return THING_ENVIRON_LIKES;
    }
  }

  return THING_ENVIRON_NEUTRAL;
}

static bool tp_cleaner_on_attacking(Gamep g, Levelsp v, Levelp l, Thingp attacker, Thingp target, ThingEvent &e)
{
  TRACE();

  (void) thing_spawn(g, v, l, tp_first(is_effect_attack), target);

  thing_sound_play(g, v, l, attacker, "squelch");

  return true;
}

static bool tp_cleaner_on_missing(Gamep g, Levelsp v, Levelp l, Thingp attacker, Thingp target, ThingEvent &e)
{
  TRACE();

  (void) thing_spawn(g, v, l, tp_first(is_effect_attack), target);

  thing_sound_play(g, v, l, attacker, "squelch");

  return true;
}

static void tp_cleaner_on_death(Gamep g, Levelsp v, Levelp l, Thingp me, ThingEvent &e)
{
  TRACE();

  (void) thing_spawn(g, v, l, tp_first(is_effect_blood), me);

  thing_sound_play(g, v, l, me, "squelch");
}

[[nodiscard]] auto tp_load_cleaner() -> bool
{
  auto *tp   = tp_load("cleaner"); // keep as string for scripts
  auto  name = tp_name(tp);

  // begin sort marker1 {
  thing_assess_tile_set(tp, tp_cleaner_assess_tile);
  thing_assess_tp_set(tp, tp_cleaner_assess_tp);
  thing_description_set(tp, tp_cleaner_description_get);
  thing_detail_set(tp, tp_cleaner_detail_get);
  thing_on_attacking_set(tp, tp_cleaner_on_attacking);
  thing_on_death_set(tp, tp_cleaner_on_death);
  thing_on_missing_set(tp, tp_cleaner_on_missing);
  tp_attack_count_max_per_tick_set(tp, 1);
  tp_chance_set(tp, THING_CHANCE_CONTINUE_TO_BURN, "1d2"); // fumble => intensify / keep burning / crit => stop burning
  tp_chance_set(tp, THING_CHANCE_START_BURNING, "1d2");    // fumble => flames spread to you
  tp_damage_set(tp, THING_EVENT_ENGULF_DAMAGE, "3d6");
  tp_damage_set(tp, THING_EVENT_MELEE_DAMAGE, "2d6");
  tp_distance_jump_set(tp, 2);
  tp_distance_jump_set(tp, 3);
  tp_distance_vision_set(tp, 10);
  tp_flag_set(tp, is_able_to_be_buffed);
  tp_flag_set(tp, is_able_to_be_ensnared);
  tp_flag_set(tp, is_able_to_be_invisible);
  tp_flag_set(tp, is_able_to_be_levitated);
  tp_flag_set(tp, is_able_to_be_teleported);
  tp_flag_set(tp, is_able_to_choose_targets);
  tp_flag_set(tp, is_able_to_collect_items);
  tp_flag_set(tp, is_able_to_collect_keys);
  tp_flag_set(tp, is_able_to_crush_grass);
  tp_flag_set(tp, is_able_to_drop_all_items_on_death);
  tp_flag_set(tp, is_able_to_eat_items);
  tp_flag_set(tp, is_able_to_eat_treasure);
  tp_flag_set(tp, is_able_to_engulf);
  tp_flag_set(tp, is_able_to_fall_sound);
  tp_flag_set(tp, is_able_to_fall);
  tp_flag_set(tp, is_able_to_jump_land_then_pounce);
  tp_flag_set(tp, is_able_to_jump);
  tp_flag_set(tp, is_able_to_lunge);
  tp_flag_set(tp, is_able_to_move);
  tp_flag_set(tp, is_animated_can_hflip);
  tp_flag_set(tp, is_animated);
  tp_flag_set(tp, is_attackable_by_player);
  tp_flag_set(tp, is_biome_bogland);
  tp_flag_set(tp, is_biome_dungeon);
  tp_flag_set(tp, is_biome_nethervoid);
  tp_flag_set(tp, is_blit_centered);
  tp_flag_set(tp, is_blit_hit_outline_w_invis_inside);
  tp_flag_set(tp, is_blit_shown_in_chasms);
  tp_flag_set(tp, is_collision_circle_large);
  tp_flag_set(tp, is_combustible); // will continue to burn once on fire
  tp_flag_set(tp, is_corpse_on_death);
  tp_flag_set(tp, is_described_cursor);
  tp_flag_set(tp, is_flammable);
  tp_flag_set(tp, is_gelatinous);
  tp_flag_set(tp, is_light_source, 2);
  tp_flag_set(tp, is_loggable);
  tp_flag_set(tp, is_monst);
  tp_flag_set(tp, is_obs_to_beam);
  tp_flag_set(tp, is_obs_to_movement);
  tp_flag_set(tp, is_obs_to_teleporting_onto);
  tp_flag_set(tp, is_obs_to_wall_walker);
  tp_flag_set(tp, is_physics_explosion);
  tp_flag_set(tp, is_physics_temperature);
  tp_flag_set(tp, is_removable_when_dead_on_err);
  tp_flag_set(tp, is_shown_health);
  tp_flag_set(tp, is_slime);
  tp_flag_set(tp, is_soft_landing);
  tp_flag_set(tp, is_submergible); // is seen submerged when in water
  tp_flag_set(tp, is_tickable);
  tp_flag_set(tp, is_vision_360_degrees);
  tp_health_set(tp, "1d10+4");
  tp_hearing_threshold_set(tp, 8); // smaller values => better hearing
  tp_is_immune_to_add(tp, THING_EVENT_WATER_DAMAGE);
  tp_items_collected_max_set(tp, 3);
  tp_light_color_set(tp, "pink");
  tp_monst_group_add(tp, MONST_GROUP2);
  tp_name_a_or_an_set(tp, "a dungeon cleaner");
  tp_name_apostrophize_set(tp, "dungeon cleaners'");
  tp_name_long_set(tp, "dungeon cleaner");
  tp_name_pluralize_set(tp, "dungeon cleaners");
  tp_name_short_set(tp, "dungeon cleaner");
  tp_priority_set(tp, THING_PRIORITY_MONST);
  tp_rarity_set(tp, THING_RARITY_COMMON);
  tp_score_value_set(tp, 100);
  tp_speed_set(tp, 50);
  tp_stat_set(tp, THING_STAT_ATT, "1d4+14");
  tp_stat_set(tp, THING_STAT_DEF, "2");
  tp_temperature_burns_at_set(tp, 30);  // celsius
  tp_temperature_damage_at_set(tp, 30); // celsius
  tp_temperature_initial_set(tp, 20);   // celsius
  tp_weight_set(tp, WEIGHT_VVHEAVY);    // grams
  tp_z_depth_set(tp, MAP_Z_DEPTH_ENGULFING_MONST);
  // end sort marker1 }

  auto delay = 200;

  for (auto frame = 0; frame < 4; frame++) {
    auto *tile = tile_find_mand(name + std::string(".idle.") + std::to_string(frame));
    tile_size_set(tile, OUTLINE_TILE_WIDTH, OUTLINE_TILE_HEIGHT);
    tile_delay_ms_set(tile, delay);
    tp_tiles_push_back(tp, THING_ANIM_IDLE, tile);
  }

  for (auto frame = 0; frame < 1; frame++) {
    auto *tile = tile_find_mand(name + std::string(".dead.") + std::to_string(frame));
    tile_size_set(tile, OUTLINE_TILE_WIDTH, OUTLINE_TILE_HEIGHT);
    tile_delay_ms_set(tile, delay);
    tp_tiles_push_back(tp, THING_ANIM_DEAD, tile);
  }

  return true;
}
