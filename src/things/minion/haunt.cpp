//
// Copyright goblinhack@gmail.com
//

#include "../../my_callstack.hpp"
#include "../../my_level.hpp"
#include "../../my_thing_callbacks.hpp"
#include "../../my_tile.hpp"
#include "../../my_tp.hpp"
#include "../../my_tps.hpp"
#include "../../my_types.hpp"
#include "../../my_ui.hpp"

static auto tp_haunt_description_get(Gamep g, Levelsp v, Levelp l, Thingp me) -> std::string
{
  TRACE();

  return "haunt, lesser vengeful spirit";
}

static auto tp_haunt_detail_get(Gamep g, Levelsp v, Levelp l, Thingp me) -> std::string
{
  TRACE();

  return                                                                                                     //
      UI_INFO1_FMT_STR "Haunts are the weakest of all spirit types and almost mindless in their pursuit.\n"  //
      UI_INFO2_FMT_STR "With no ability to pass through walls, or even be able to path around a corner, "    //
                       "haunts can often be seen queuing up to attack or hitting themselves off of walls.\n" //
      UI_INFO3_FMT_STR "The ultimate humiliation is to be killed by a haunt and thus become one.\n";         //
}

static auto tp_haunt_assess_tp(Gamep g, Levelsp v, Levelp l, Tpp tp, Thingp me) -> ThingEnvironType
{
  TRACE_DEBUG();

  if (tp_is_water_shallow(tp)) {
    return THING_ENVIRON_HATES;
  }

  if (tp_is_water_deep(tp)) {
    return THING_ENVIRON_HATES;
  }

  return THING_ENVIRON_NEUTRAL;
}

static auto tp_haunt_assess_tile(Gamep g, Levelsp v, Levelp l, const bpoint &at, Thingp me) -> ThingEnvironType
{
  TRACE_DEBUG();

  if (level_is_water_shallow_cached(g, v, l, at)) {
    return THING_ENVIRON_HATES;
  }

  if (level_is_water_deep_cached(g, v, l, at)) {
    return THING_ENVIRON_HATES;
  }

  return THING_ENVIRON_NEUTRAL;
}

static bool tp_haunt_on_attacking(Gamep g, Levelsp v, Levelp l, Thingp attacker, Thingp target, ThingEvent &e)
{
  TRACE();

  (void) thing_spawn(g, v, l, tp_first(is_effect_attack), target);
  thing_sound_play(g, v, l, attacker, "hiss");

  return true;
}

static bool tp_haunt_on_missing(Gamep g, Levelsp v, Levelp l, Thingp attacker, Thingp target, ThingEvent &e)
{
  TRACE();

  (void) thing_spawn(g, v, l, tp_first(is_effect_attack), target);

  thing_sound_play(g, v, l, attacker, "hiss");

  return true;
}

static void tp_haunt_on_death(Gamep g, Levelsp v, Levelp l, Thingp me, ThingEvent &e)
{
  TRACE();

  thing_sound_play(g, v, l, me, "monst_death");
}

[[nodiscard]] auto tp_load_haunt() -> bool
{
  auto *tp   = tp_load("haunt"); // keep as string for scripts
  auto  name = tp_name(tp);

  // begin sort marker1 {
  thing_assess_tile_set(tp, tp_haunt_assess_tile);
  thing_assess_tp_set(tp, tp_haunt_assess_tp);
  thing_description_set(tp, tp_haunt_description_get);
  thing_detail_set(tp, tp_haunt_detail_get);
  thing_on_attacking_set(tp, tp_haunt_on_attacking);
  thing_on_death_set(tp, tp_haunt_on_death);
  thing_on_missing_set(tp, tp_haunt_on_missing);
  tp_attack_count_max_per_tick_set(tp, 1);
  tp_damage_set(tp, THING_EVENT_MELEE_DAMAGE, "1");
  tp_distance_minion_from_mob_max_set(tp, 6);
  tp_distance_vision_set(tp, 12);
  tp_flag_set(tp, is_able_to_be_buffed);
  tp_flag_set(tp, is_able_to_be_invisible);
  tp_flag_set(tp, is_able_to_be_teleported);
  tp_flag_set(tp, is_able_to_lunge);
  tp_flag_set(tp, is_able_to_move_diagonally);
  tp_flag_set(tp, is_able_to_move);
  tp_flag_set(tp, is_able_to_see_through_walls);
  tp_flag_set(tp, is_animated_can_hflip);
  tp_flag_set(tp, is_animated);
  tp_flag_set(tp, is_attackable_by_player);
  tp_flag_set(tp, is_biome_dungeon);
  tp_flag_set(tp, is_biome_graveyard);
  tp_flag_set(tp, is_biome_nethervoid);
  tp_flag_set(tp, is_biome_underhell);
  tp_flag_set(tp, is_blit_centered);
  tp_flag_set(tp, is_blit_hit_outline_w_invis_inside);
  tp_flag_set(tp, is_blit_shown_in_chasms);
  tp_flag_set(tp, is_blit_when_obscured_faded);
  tp_flag_set(tp, is_collision_circle_large);
  tp_flag_set(tp, is_corpse_on_death);
  tp_flag_set(tp, is_described_cursor);
  tp_flag_set(tp, is_dumb_ai);
  tp_flag_set(tp, is_ethereal);
  tp_flag_set(tp, is_haunt);
  tp_flag_set(tp, is_levitating);
  tp_flag_set(tp, is_light_source, 2);
  tp_flag_set(tp, is_loggable);
  tp_flag_set(tp, is_minion);
  tp_flag_set(tp, is_monst);
  tp_flag_set(tp, is_physics_explosion);
  tp_flag_set(tp, is_physics_water);
  tp_flag_set(tp, is_removable_when_dead_on_err);
  tp_flag_set(tp, is_shown_health);
  tp_flag_set(tp, is_tickable);
  tp_flag_set(tp, is_undead);
  tp_flag_set(tp, is_vision_180_degrees);
  tp_health_set(tp, "1");
  tp_hearing_threshold_set(tp, 6); // smaller values => better hearing
  tp_is_immune_to_add(tp, THING_EVENT_WATER_DAMAGE);
  tp_light_color_set(tp, "lime");
  tp_name_a_or_an_set(tp, "a haunt");
  tp_name_apostrophize_set(tp, "haunts'");
  tp_name_long_set(tp, "haunt");
  tp_name_pluralize_set(tp, "haunts");
  tp_name_short_set(tp, "haunt");
  tp_priority_set(tp, THING_PRIORITY_MONST);
  tp_score_value_set(tp, 5);
  tp_speed_set(tp, 100);
  tp_stat_set(tp, THING_STAT_ATT, "6");
  tp_stat_set(tp, THING_STAT_DEF, "1");
  tp_temperature_initial_set(tp, -10); // celsius
  tp_weight_set(tp, WEIGHT_FEATHER);   // grams
  tp_z_depth_set(tp, MAP_Z_DEPTH_FLOATING_MONST);
  // end sort marker1 }

  auto delay = 1000;

  for (auto frame = 0; frame < 4; frame++) {
    auto *tile = tile_find_mand(name + std::string(".idle.") + std::to_string(frame));
    tile_size_set(tile, OUTLINE_TILE_WIDTH, OUTLINE_TILE_HEIGHT);
    tile_delay_ms_set(tile, delay);
    tp_tiles_push_back(tp, THING_ANIM_IDLE, tile);
  }

  delay = 100;

  for (auto frame = 0; frame < 3; frame++) {
    auto *tile = tile_find_mand(name + std::string(".dead.") + std::to_string(frame));
    tile_size_set(tile, OUTLINE_TILE_WIDTH, OUTLINE_TILE_HEIGHT);
    tile_delay_ms_set(tile, delay);
    tp_tiles_push_back(tp, THING_ANIM_DEAD, tile);

    if (frame == 2) {
      tile_is_cleanup_on_end_of_anim_set(tile);
    }
  }

  return true;
}
