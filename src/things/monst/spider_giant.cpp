//
// Copyright goblinhack@gmail.com
//

#include "../../my_callstack.hpp"
#include "../../my_dice_rolls.hpp"
#include "../../my_thing.hpp"
#include "../../my_thing_callbacks.hpp"
#include "../../my_thing_inlines.hpp"
#include "../../my_tile.hpp"
#include "../../my_tp.hpp"
#include "../../my_tps.hpp"
#include "../../my_types.hpp"
#include "../../my_ui.hpp"

static auto tp_spider_giant_description_get(Gamep g, Levelsp v, Levelp l, Thingp me) -> std::string
{
  TRACE();

  if (thing_is_dead(me)) {
    return "dead giant spider";
  }
  return "giant spider";
}

static auto tp_spider_giant_detail_get(Gamep g, Levelsp v, Levelp l, Thingp me) -> std::string
{
  TRACE();

  return                                                                                                                    //
      UI_INFO1_FMT_STR "Green ihcor drips from the fangs of this uncomfortably large spider. One might even say giant...\n" //
      UI_INFO2_FMT_STR "It moves silently with its many eyes watching you over intently. It seems poised to jump.\n";       //
}

static auto tp_spider_giant_assess_tp(Gamep g, Levelsp v, Levelp l, Tpp tp, Thingp me) -> ThingEnvironType
{
  TRACE_DEBUG();

  if (tp_is_lava(tp)) {
    return THING_ENVIRON_HATES;
  }

  if (tp_is_fire(tp)) {
    return THING_ENVIRON_HATES;
  }

  if (tp_is_water_shallow(tp)) {
    return THING_ENVIRON_DISLIKES;
  }

  if (tp_is_water_deep(tp)) {
    return THING_ENVIRON_DISLIKES;
  }

  if (tp_is_spiderweb(tp)) {
    return THING_ENVIRON_LIKES;
  }

  if (tp_is_spider(tp)) {
    return THING_ENVIRON_HATES;
  }

  if (tp_is_flesh(tp)) {
    return THING_ENVIRON_LIKES;
  }

  return THING_ENVIRON_NEUTRAL;
}

static auto tp_spider_giant_assess_tile(Gamep g, Levelsp v, Levelp l, const bpoint &at, Thingp me) -> ThingEnvironType
{
  TRACE_DEBUG();

  if (level_is_lava_cached(g, v, l, at)) {
    return THING_ENVIRON_HATES;
  }

  if (level_is_fire_cached(g, v, l, at)) {
    return THING_ENVIRON_HATES;
  }

  if (level_is_water_shallow_cached(g, v, l, at)) {
    return THING_ENVIRON_HATES;
  }

  if (level_is_water_deep_cached(g, v, l, at)) {
    return THING_ENVIRON_HATES;
  }

  if (level_is_spiderweb_cached(g, v, l, at)) {
    return THING_ENVIRON_LIKES;
  }

  if (level_is_spider_cached(g, v, l, at)) {
    return THING_ENVIRON_HATES;
  }

  if (level_is_flesh_cached(g, v, l, at)) {
    return THING_ENVIRON_LIKES;
  }

  return THING_ENVIRON_NEUTRAL;
}

static void tp_spider_giant_on_death(Gamep g, Levelsp v, Levelp l, Thingp me, ThingEvent &e)
{
  TRACE();

  (void) thing_spawn(g, v, l, tp_first(is_effect_blood), me);

  thing_sound_play(g, v, l, me, "monst_death");
}

static bool tp_spider_giant_on_attacking(Gamep g, Levelsp v, Levelp l, Thingp attacker, Thingp target, ThingEvent &e)
{
  TRACE();

  (void) thing_spawn(g, v, l, tp_first(is_effect_attack), target);

  thing_sound_play(g, v, l, attacker, "hiss");

  return true;
}

static bool tp_spider_giant_on_killing(Gamep g, Levelsp v, Levelp l, Thingp attacker, Thingp target, ThingEvent &e)
{
  TRACE();

  (void) thing_spawn(g, v, l, tp_first(is_spider_baby), target);

  if (d100() < 10) {
    (void) thing_spawn(g, v, l, tp_first(is_spiderweb), target);
  }

  return true;
}

static bool tp_spider_giant_on_missing(Gamep g, Levelsp v, Levelp l, Thingp attacker, Thingp target, ThingEvent &e)
{
  TRACE();

  (void) thing_spawn(g, v, l, tp_first(is_effect_attack), target);

  thing_sound_play(g, v, l, attacker, "hiss");

  return true;
}

[[nodiscard]] auto tp_load_spider() -> bool
{
  auto *tp   = tp_load("spider_giant"); // keep as string for scripts
  auto  name = tp_name(tp);

  // begin sort marker1 {
  thing_assess_tile_set(tp, tp_spider_giant_assess_tile);
  thing_assess_tp_set(tp, tp_spider_giant_assess_tp);
  thing_description_set(tp, tp_spider_giant_description_get);
  thing_detail_set(tp, tp_spider_giant_detail_get);
  thing_on_attacking_set(tp, tp_spider_giant_on_attacking);
  thing_on_death_set(tp, tp_spider_giant_on_death);
  thing_on_killing_set(tp, tp_spider_giant_on_killing);
  thing_on_missing_set(tp, tp_spider_giant_on_missing);
  tp_attack_count_max_per_tick_set(tp, 1);
  tp_attack_count_max_per_tick_set(tp, 2);
  tp_chance_set(tp, THING_CHANCE_CONTINUE_TO_BURN, "1d6"); // fumble => intensify / keep burning / crit => stop burning
  tp_chance_set(tp, THING_CHANCE_START_BURNING, "1d2");    // fumble => flames spread to you
  tp_damage_set(tp, THING_EVENT_MELEE_DAMAGE, "2d6");
  tp_distance_jump_set(tp, 3);
  tp_distance_vision_set(tp, 12);
  tp_flag_set(tp, is_able_to_be_buffed);
  tp_flag_set(tp, is_able_to_be_invisible);
  tp_flag_set(tp, is_able_to_be_levitated);
  tp_flag_set(tp, is_able_to_be_teleported);
  tp_flag_set(tp, is_able_to_choose_targets);
  tp_flag_set(tp, is_able_to_fall_sound);
  tp_flag_set(tp, is_able_to_fall);
  tp_flag_set(tp, is_able_to_jump);
  tp_flag_set(tp, is_able_to_lunge);
  tp_flag_set(tp, is_able_to_move_diagonally);
  tp_flag_set(tp, is_animated_can_hflip);
  tp_flag_set(tp, is_animated);
  tp_flag_set(tp, is_attackable_by_player);
  tp_flag_set(tp, is_biome_bogland);
  tp_flag_set(tp, is_biome_dungeon);
  tp_flag_set(tp, is_blit_centered);
  tp_flag_set(tp, is_blit_hit_outline_w_invis_inside);
  tp_flag_set(tp, is_blit_shown_in_chasms);
  tp_flag_set(tp, is_burnable); // is capable of being burned by fire
  tp_flag_set(tp, is_collision_circle_large);
  tp_flag_set(tp, is_described_cursor);
  tp_flag_set(tp, is_flesh_eater);
  tp_flag_set(tp, is_insectoid);
  tp_flag_set(tp, is_loggable);
  tp_flag_set(tp, is_monst);
  tp_flag_set(tp, is_obs_to_beam);
  tp_flag_set(tp, is_obs_to_jumping_onto);
  tp_flag_set(tp, is_obs_to_movement);
  tp_flag_set(tp, is_obs_to_teleporting_onto);
  tp_flag_set(tp, is_obs_to_wall_walker);
  tp_flag_set(tp, is_physics_explosion);
  tp_flag_set(tp, is_physics_temperature);
  tp_flag_set(tp, is_physics_trap);
  tp_flag_set(tp, is_removable_when_dead_on_err);
  tp_flag_set(tp, is_shown_health);
  tp_flag_set(tp, is_spider_giant);
  tp_flag_set(tp, is_spider);
  tp_flag_set(tp, is_submergible); // is seen submerged when in water
  tp_flag_set(tp, is_tickable);
  tp_flag_set(tp, is_vision_360_degrees);
  tp_health_set(tp, "2d8");
  tp_hearing_threshold_set(tp, 2); // smaller values => better hearing
  tp_name_a_or_an_set(tp, "a giant spider");
  tp_name_apostrophize_set(tp, "giant spiders'");
  tp_name_long_set(tp, "giant spider");
  tp_name_pluralize_set(tp, "giant spiders");
  tp_name_short_set(tp, "spider");
  tp_priority_set(tp, THING_PRIORITY_MONST);
  tp_rarity_set(tp, THING_RARITY_COMMON);
  tp_score_value_set(tp, 100);
  tp_speed_set(tp, 25);
  tp_stat_set(tp, THING_STAT_ATT, "1d8+10");
  tp_stat_set(tp, THING_STAT_DEF, "8");
  tp_temperature_burns_at_set(tp, 50);  // celsius
  tp_temperature_damage_at_set(tp, 35); // celsius
  tp_temperature_initial_set(tp, 20);   // celsius
  tp_weight_set(tp, WEIGHT_HEAVY);      // grams
  tp_z_depth_set(tp, MAP_Z_DEPTH_MONST);
  // end sort marker1 }

  auto delay = 800;

  for (auto frame = 0; frame < 3; frame++) {
    auto *tile = tile_find_mand(name + std::string(".idle.") + std::to_string(frame));
    tile_size_set(tile, OUTLINE_TILE_WIDTH, OUTLINE_TILE_HEIGHT);
    tile_delay_ms_set(tile, delay);
    tp_tiles_push_back(tp, THING_ANIM_IDLE, tile);
  }

  return true;
}
