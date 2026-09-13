//
// Copyright goblinhack@gmail.com
//

#include "../../my_callstack.hpp"
#include "../../my_globals.hpp"
#include "../../my_main.hpp"
#include "../../my_thing.hpp"
#include "../../my_thing_callbacks.hpp"
#include "../../my_thing_inlines.hpp"
#include "../../my_tile.hpp"
#include "../../my_tp.hpp"
#include "../../my_tps.hpp"
#include "../../my_types.hpp"
#include "../../my_ui.hpp"

static auto tp_pirnana_description_get(Gamep g, Levelsp v, Levelp l, Thingp me) -> std::string
{
  TRACE();

  if (thing_is_dead(me)) {
    return "dead pirnana";
  }
  return "pirnana";
}

static auto tp_pirnana_detail_get(Gamep g, Levelsp v, Levelp l, Thingp me) -> std::string
{
  TRACE();

  return                                                                                                                               //
      UI_INFO1_FMT_STR "The dreaded pirnana - half piranha, half banana - lurks in the deep waters waiting to strike.\n"               //
      UI_INFO2_FMT_STR "Its yellow scales glitter in the dark and, it has been speculated, evolved to lure unwary gold prospectors.\n" //
      UI_INFO3_FMT_STR "However, three fingers and half a consumed foot later, they knew better. This shiny golden fish bites.\n"      //
      UI_INFO4_FMT_STR "It also jumps...\n";                                                                                           //
}

static auto tp_pirnana_assess_tp(Gamep g, Levelsp v, Levelp l, Tpp tp, Thingp me) -> ThingEnvironType
{
  TRACE_DEBUG();

  if (tp_is_water_deep(tp)) {
    return THING_ENVIRON_LIKES;
  }

  if (tp_is_dirt(tp)) {
    return THING_ENVIRON_HATES;
  }

  if (tp_is_floor(tp)) {
    return THING_ENVIRON_HATES;
  }

  return THING_ENVIRON_NEUTRAL;
}

static auto tp_pirnana_assess_tile(Gamep g, Levelsp v, Levelp l, const bpoint &at, Thingp me) -> ThingEnvironType
{
  TRACE_DEBUG();

  if (level_is_water_deep_cached(g, v, l, at)) {
    return THING_ENVIRON_LIKES;
  }

  if (level_is_dirt_cached(g, v, l, at)) {
    return THING_ENVIRON_HATES;
  }

  if (level_is_floor_cached(g, v, l, at)) {
    return THING_ENVIRON_HATES;
  }

  return THING_ENVIRON_NEUTRAL;
}

static void tp_pirnana_on_death(Gamep g, Levelsp v, Levelp l, Thingp me, ThingEvent &e)
{
  TRACE();

  (void) thing_spawn(g, v, l, tp_first(is_effect_blood), me);

  thing_sound_play(g, v, l, me, "monst_death");
}

static void tp_pirnana_on_levitated(Gamep g, Levelsp v, Levelp l, Thingp me)
{
  TRACE();

  ThingEvent e {
      .reason     = "by being out of water", //
      .event_type = THING_EVENT_LEVITATED,   //
  };

  THING_DBG(g, v, l, me, "dead due to levitating");
  TRACE_INDENT();

  thing_dead(g, v, l, me, e);
}

static bool tp_pirnana_on_attacking(Gamep g, Levelsp v, Levelp l, Thingp attacker, Thingp target, ThingEvent &e)
{
  TRACE();

  (void) thing_spawn(g, v, l, tp_first(is_effect_attack), target);

  thing_sound_play(g, v, l, attacker, "hiss");

  return true;
}

static bool tp_pirnana_on_missing(Gamep g, Levelsp v, Levelp l, Thingp attacker, Thingp target, ThingEvent &e)
{
  TRACE();

  (void) thing_spawn(g, v, l, tp_first(is_effect_attack), target);

  thing_sound_play(g, v, l, attacker, "hiss");

  return true;
}

[[nodiscard]] auto tp_load_pirnana() -> bool
{
  auto *tp   = tp_load("pirnana"); // keep as string for scripts
  auto  name = tp_name(tp);

  // begin sort marker1 {
  thing_assess_tile_set(tp, tp_pirnana_assess_tile);
  thing_assess_tp_set(tp, tp_pirnana_assess_tp);
  thing_description_set(tp, tp_pirnana_description_get);
  thing_detail_set(tp, tp_pirnana_detail_get);
  thing_on_attacking_set(tp, tp_pirnana_on_attacking);
  thing_on_death_set(tp, tp_pirnana_on_death);
  thing_on_levitated_set(tp, tp_pirnana_on_levitated);
  thing_on_missing_set(tp, tp_pirnana_on_missing);
  tp_attack_count_max_per_tick_set(tp, 2);
  tp_damage_set(tp, THING_EVENT_MELEE_DAMAGE, "1d6");
  tp_distance_jump_set(tp, 3);
  tp_distance_vision_set(tp, 5);
  tp_flag_set(tp, is_able_to_be_buffed);
  tp_flag_set(tp, is_able_to_be_invisible);
  tp_flag_set(tp, is_able_to_be_levitated);
  tp_flag_set(tp, is_able_to_choose_targets);
  tp_flag_set(tp, is_able_to_fall_sound);
  tp_flag_set(tp, is_able_to_fall);
  tp_flag_set(tp, is_able_to_jump_attack);
  tp_flag_set(tp, is_able_to_jump_land_then_pounce);
  tp_flag_set(tp, is_able_to_jump);
  tp_flag_set(tp, is_able_to_lunge);
  tp_flag_set(tp, is_able_to_move_diagonally);
  tp_flag_set(tp, is_able_to_move);
  tp_flag_set(tp, is_animated_can_hflip);
  tp_flag_set(tp, is_animated);
  tp_flag_set(tp, is_attackable_by_player);
  tp_flag_set(tp, is_biome_bogland);
  tp_flag_set(tp, is_biome_dungeon);
  tp_flag_set(tp, is_blit_centered);
  tp_flag_set(tp, is_blit_hit_outline_w_invis_inside);
  tp_flag_set(tp, is_blit_shown_in_chasms);
  tp_flag_set(tp, is_collision_circle_large);
  tp_flag_set(tp, is_described_cursor);
  tp_flag_set(tp, is_fish);
  tp_flag_set(tp, is_flesh_eater);
  tp_flag_set(tp, is_flesh);
  tp_flag_set(tp, is_loggable);
  tp_flag_set(tp, is_monst);
  tp_flag_set(tp, is_physics_explosion);
  tp_flag_set(tp, is_physics_temperature);
  tp_flag_set(tp, is_removable_when_dead_on_err);
  tp_flag_set(tp, is_shown_health);
  tp_flag_set(tp, is_submergible); // is seen submerged when in water
  tp_flag_set(tp, is_tickable);
  tp_flag_set(tp, is_vision_360_degrees);
  tp_health_set(tp, "2d6");
  tp_hearing_threshold_set(tp, 3); // smaller values => better hearing
  tp_is_immune_to_add(tp, THING_EVENT_WATER_DAMAGE);
  tp_jump_attack_pct_chance_set(tp, 5);
  tp_name_a_or_an_set(tp, "a pirnana");
  tp_name_apostrophize_set(tp, "pirnanas'");
  tp_name_long_set(tp, "pirnana");
  tp_name_pluralize_set(tp, "pirnanas");
  tp_name_short_set(tp, "pirnana");
  tp_priority_set(tp, THING_PRIORITY_MONST);
  tp_rarity_set(tp, THING_RARITY_COMMON);
  tp_score_value_set(tp, 50);
  tp_speed_set(tp, 50);
  tp_stat_set(tp, THING_STAT_ATT, "1d4+10");
  tp_stat_set(tp, THING_STAT_DEF, "16");
  tp_temperature_damage_at_set(tp, 35); // celsius
  tp_temperature_initial_set(tp, 20);   // celsius
  tp_weight_set(tp, WEIGHT_HUMAN);      // grams
  tp_z_depth_set(tp, MAP_Z_DEPTH_OBJ);
  // end sort marker1 }

  auto delay = 500;

  for (auto frame = 0; frame < 2; frame++) {
    auto *tile = tile_find_mand(name + std::string(".idle.") + std::to_string(frame));
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tile_delay_ms_set(tile, delay);
    tp_tiles_push_back(tp, THING_ANIM_IDLE, tile);
  }

  return true;
}
