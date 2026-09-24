//
// Copyright goblinhack@gmail.com
//

#include "../../my_callstack.hpp"
#include "../../my_dice_rolls.hpp"
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

static auto tp_skullferno_description_get(Gamep g, Levelsp v, Levelp l, Thingp me) -> std::string
{
  TRACE();

  if (thing_is_dead(me)) {
    return "dead skullferno";
  }
  return "skullferno";
}

static auto tp_skullferno_detail_get(Gamep g, Levelsp v, Levelp l, Thingp me) -> std::string
{
  TRACE();

  return                                                                                                                         //
      UI_INFO1_FMT_STR "Skullferno are the hollowed out-skulls of wizards that were put to the test and found to be lacking.\n " //
      UI_INFO2_FMT_STR "Their tormented moans echo through the darkness as their souls dream of an end to their suffering "      //
                       "with a swift, final death.\n "                                                                           //
      UI_INFO3_FMT_STR "Other than that, they tend to be quite optimistic about things.\n "                                      //
      UI_INFO4_FMT_STR "Beware their fiery breath and their natural immunity to fire...\n";                                      //
}

static auto tp_skullferno_assess_tp(Gamep g, Levelsp v, Levelp l, Tpp tp, Thingp me) -> ThingEnvironType
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

static auto tp_skullferno_assess_tile(Gamep g, Levelsp v, Levelp l, const bpoint &at, Thingp me) -> ThingEnvironType
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

static void tp_skullferno_on_death(Gamep g, Levelsp v, Levelp l, Thingp me, ThingEvent &e)
{
  TRACE();

  (void) thing_spawn(g, v, l, tp_first(is_effect_blood), me);

  thing_sound_play(g, v, l, me, "monst_death");
}

static bool tp_skullferno_on_attacking(Gamep g, Levelsp v, Levelp l, Thingp attacker, Thingp target, ThingEvent &e)
{
  THING_DBG(g, v, l, attacker, "on attack");
  TRACE_INDENT();

  TpSpecialAttack d;

  if (thing_special_attack_get_random(g, v, l, attacker, target, d)) {
    e.special_attack = d;
    e.event_type     = d.event_type;

    if (d.what != "") {
      auto target_at = thing_at(g, v, l, target);
      auto fire_what = tp_find_mand(d.what);

      THING_DBG(g, v, l, attacker, "fire beam weapon");
      TRACE_INDENT();

      (void) thing_proj_launch_at(g, v, l, attacker, fire_what, target_at);
      return false; // prevent melee attack
    }
  }

  if (! adjacent(thing_at(g, v, l, attacker), thing_at(g, v, l, target))) {
    return false; // prevent melee attack
  }

  (void) thing_spawn(g, v, l, tp_first(is_effect_attack), target);
  thing_sound_play(g, v, l, attacker, "hiss");

  return true;
}

static bool tp_skullferno_on_missing(Gamep g, Levelsp v, Levelp l, Thingp attacker, Thingp target, ThingEvent &e)
{
  THING_DBG(g, v, l, attacker, "on attack");
  TRACE_INDENT();

  (void) thing_spawn(g, v, l, tp_first(is_effect_attack), target);

  thing_sound_play(g, v, l, attacker, "hiss");

  return true;
}

static void tp_skullferno_tick_begin(Gamep g, Levelsp v, Levelp l, Thingp me)
{
  TRACE();

  auto target = thing_monst_target(me);
  if (level_is_player(g, v, l, target)) {
    (void) thing_attack_at(g, v, l, me, target);
  }
}

[[nodiscard]] auto tp_load_skullferno() -> bool
{
  auto *tp   = tp_load("skullferno"); // keep as string for scripts
  auto  name = tp_name(tp);

  // begin sort marker1 {
  thing_assess_tile_set(tp, tp_skullferno_assess_tile);
  thing_assess_tp_set(tp, tp_skullferno_assess_tp);
  thing_description_set(tp, tp_skullferno_description_get);
  thing_detail_set(tp, tp_skullferno_detail_get);
  thing_on_attacking_set(tp, tp_skullferno_on_attacking);
  thing_on_death_set(tp, tp_skullferno_on_death);
  thing_on_missing_set(tp, tp_skullferno_on_missing);
  thing_on_tick_begin_set(tp, tp_skullferno_tick_begin);
  tp_attack_count_max_per_tick_set(tp, 1);
  tp_damage_set(tp, THING_EVENT_MELEE_DAMAGE, "1d4");
  tp_distance_avoid_target_set(tp, 3);
  tp_distance_vision_set(tp, 12);
  tp_flag_set(tp, is_able_to_be_buffed);
  tp_flag_set(tp, is_able_to_be_invisible);
  tp_flag_set(tp, is_able_to_be_levitated);
  tp_flag_set(tp, is_able_to_be_teleported);
  tp_flag_set(tp, is_able_to_fire_weapons);
  tp_flag_set(tp, is_able_to_lunge);
  tp_flag_set(tp, is_able_to_move_diagonally);
  tp_flag_set(tp, is_able_to_move);
  tp_flag_set(tp, is_animated_can_hflip);
  tp_flag_set(tp, is_animated);
  tp_flag_set(tp, is_attackable_by_player);
  tp_flag_set(tp, is_biome_dungeon);
  tp_flag_set(tp, is_biome_graveyard);
  tp_flag_set(tp, is_biome_nethervoid);
  tp_flag_set(tp, is_biome_underhell);
  tp_flag_set(tp, is_blasted_when_dead);
  tp_flag_set(tp, is_blit_centered);
  tp_flag_set(tp, is_blit_hit_outline_w_invis_inside);
  tp_flag_set(tp, is_blit_shown_in_chasms);
  tp_flag_set(tp, is_collision_circle_large);
  tp_flag_set(tp, is_corpse_on_death);
  tp_flag_set(tp, is_described_cursor);
  tp_flag_set(tp, is_ethereal);
  tp_flag_set(tp, is_levitating);
  tp_flag_set(tp, is_loggable);
  tp_flag_set(tp, is_monst);
  tp_flag_set(tp, is_obs_to_beam);
  tp_flag_set(tp, is_obs_to_jumping_onto);
  tp_flag_set(tp, is_obs_to_movement);
  tp_flag_set(tp, is_obs_to_teleporting_onto);
  tp_flag_set(tp, is_obs_to_wall_walker);
  tp_flag_set(tp, is_removable_when_dead_on_err);
  tp_flag_set(tp, is_shown_health);
  tp_flag_set(tp, is_tickable);
  tp_flag_set(tp, is_undead);
  tp_flag_set(tp, is_vision_360_degrees);
  tp_flag_set(tp, is_wall_walker);
  tp_health_set(tp, "1d4");
  tp_hearing_threshold_set(tp, 8); // smaller values => better hearing
  tp_is_immune_to_add(tp, THING_EVENT_FIRE_DAMAGE);
  tp_missile_count_max_set(tp, THING_MISSILE_MAX);
  tp_monst_group_add(tp, MONST_GROUP2);
  tp_name_a_or_an_set(tp, "a skullferno");
  tp_name_apostrophize_set(tp, "skullferno's");
  tp_name_long_set(tp, "skullferno");
  tp_name_pluralize_set(tp, "skullfernos");
  tp_name_short_set(tp, "skullferno");
  tp_priority_set(tp, THING_PRIORITY_MONST);
  tp_rarity_set(tp, THING_RARITY_COMMON);
  tp_score_value_set(tp, 100);
  tp_speed_set(tp, 100);
  tp_stat_set(tp, THING_STAT_ATT, "1d8+10");
  tp_stat_set(tp, THING_STAT_DEF, "1d8+10");
  tp_temperature_initial_set(tp, 200); // celsius
  tp_weight_set(tp, WEIGHT_HUMAN);     // grams
  tp_z_depth_set(tp, MAP_Z_DEPTH_FLOATING_MONST);
  tp_flag_set(tp, is_described_when_killed);
  // end sort marker1 }

  tp_special_attack_add(tp,
                        TpSpecialAttack {
                            .type          = "1",                      //
                            .event_type    = THING_EVENT_MELEE_DAMAGE, //
                            .name          = "mandible attack",        //
                            .roll          = "1d8",                    //
                            .d100          = 100,
                            .when_adjacent = true,
                        });

  tp_special_attack_add(tp,
                        TpSpecialAttack {
                            .type         = "2",         //
                            .name         = "mind fire", //
                            .what         = "proj_fire", //
                            .d100         = 20,
                            .when_distant = true,
                        });

  auto delay = 200;

  for (auto frame = 0; frame < 2; frame++) {
    auto *tile = tile_find_mand(name + std::string(".idle.") + std::to_string(frame));
    tile_size_set(tile, OUTLINE_TILE_WIDTH, OUTLINE_TILE_HEIGHT);
    tile_delay_ms_set(tile, delay);
    tp_tiles_push_back(tp, THING_ANIM_IDLE, tile);
  }

  delay = 100;

  for (auto frame = 0; frame < 2; frame++) {
    auto *tile = tile_find_mand(name + std::string(".dead.") + std::to_string(frame));
    tile_size_set(tile, OUTLINE_TILE_WIDTH, OUTLINE_TILE_HEIGHT);
    tile_delay_ms_set(tile, delay);
    tp_tiles_push_back(tp, THING_ANIM_DEAD, tile);

    if (frame == 1) {
      tile_is_cleanup_on_end_of_anim_set(tile);
    }
  }

  return true;
}
