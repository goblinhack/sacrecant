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

static auto tp_voider_description_get(Gamep g, Levelsp v, Levelp l, Thingp me) -> std::string
{
  TRACE();

  if (thing_is_dead(me)) {
    return "dead voider";
  }
  return "voider";
}

static auto tp_voider_detail_get(Gamep g, Levelsp v, Levelp l, Thingp me) -> std::string
{
  TRACE();

  return                                                                                                                                   //
      UI_INFO1_FMT_STR "Voiders are interdimensional creatures that, unluckily for you, are currently in this dimension.\n"                //
      UI_INFO2_FMT_STR "A floating mass of tendrils, their main attack is to engulf their prey and digest them slowly.\n"                  //
      UI_INFO3_FMT_STR "Even in death, the voider is dangerous as they have a tendency to obliterate their surroundings when destroyed.\n" //
      UI_INFO4_FMT_STR "Remember the wise saying, 'best avoid a voider'\n";                                                                //
}

static auto tp_voider_assess_tp(Gamep g, Levelsp v, Levelp l, Tpp tp, Thingp me) -> ThingEnvironType
{
  TRACE_DEBUG();

  return THING_ENVIRON_NEUTRAL;
}

static auto tp_voider_assess_tile(Gamep g, Levelsp v, Levelp l, const bpoint &at, Thingp me) -> ThingEnvironType
{
  TRACE_DEBUG();

  return THING_ENVIRON_NEUTRAL;
}

static void tp_voider_on_death(Gamep g, Levelsp v, Levelp l, Thingp me, ThingEvent &e)
{
  TRACE();

  (void) thing_spawn(g, v, l, tp_first(is_effect_blood), me);

  thing_sound_play(g, v, l, me, "monst_death");

  const std::initializer_list< bpoint > points = {
      bpoint(-1, -1), bpoint(1, -1), bpoint(0, -1), bpoint(-1, 0), bpoint(1, 0), bpoint(0, 0), bpoint(-1, 1), bpoint(1, 1), bpoint(0, 1),
  };

  for (auto delta : points) {
    auto at = thing_at(g, v, l, me);
    auto p  = at + delta;

    if (level_is_critical_to_dungeon_design_bool(g, v, l, p)) {
      continue;
    }

    if (level_is_dirt_bool(g, v, l, p) || level_is_floor(g, v, l, p)) {
      (void) thing_spawn(g, v, l, tp_first(is_explosion), p);
      (void) thing_spawn(g, v, l, tp_first(is_chasm), p);
    }
  }
}

static bool tp_voider_on_attacking(Gamep g, Levelsp v, Levelp l, Thingp attacker, Thingp target, ThingEvent &e)
{
  TRACE();

  (void) thing_spawn(g, v, l, tp_first(is_effect_attack), target);

  thing_sound_play(g, v, l, attacker, "hiss");

  return true;
}

static bool tp_voider_on_missing(Gamep g, Levelsp v, Levelp l, Thingp attacker, Thingp target, ThingEvent &e)
{
  TRACE();

  (void) thing_spawn(g, v, l, tp_first(is_effect_attack), target);

  thing_sound_play(g, v, l, attacker, "hiss");

  return true;
}

static void tp_voider_tick_begin(Gamep g, Levelsp v, Levelp l, Thingp me)
{
  TRACE();

  auto target = thing_monst_target(me);
  if (level_is_player(g, v, l, target)) {
    (void) thing_attack_at(g, v, l, me, target);
  }
}

[[nodiscard]] auto tp_load_voider() -> bool
{
  auto *tp   = tp_load("voider"); // keep as string for scripts
  auto  name = tp_name(tp);

  // begin sort marker1 {
  thing_assess_tile_set(tp, tp_voider_assess_tile);
  thing_assess_tp_set(tp, tp_voider_assess_tp);
  thing_description_set(tp, tp_voider_description_get);
  thing_detail_set(tp, tp_voider_detail_get);
  thing_on_attacking_set(tp, tp_voider_on_attacking);
  thing_on_death_set(tp, tp_voider_on_death);
  thing_on_missing_set(tp, tp_voider_on_missing);
  thing_on_tick_begin_set(tp, tp_voider_tick_begin);
  tp_attack_count_max_per_tick_set(tp, 1);
  tp_chance_set(tp, THING_CHANCE_CONTINUE_TO_BURN, "1d6"); // fumble => intensify / keep burning / crit => stop burning
  tp_chance_set(tp, THING_CHANCE_START_BURNING, "1d2");    // fumble => flames spread to you
  tp_damage_set(tp, THING_EVENT_ENGULF_DAMAGE, "3d6");
  tp_damage_set(tp, THING_EVENT_MELEE_DAMAGE, "1d4");
  tp_distance_vision_set(tp, 12);
  tp_flag_set(tp, is_able_to_be_buffed);
  tp_flag_set(tp, is_able_to_be_invisible);
  tp_flag_set(tp, is_able_to_be_levitated);
  tp_flag_set(tp, is_able_to_be_teleported);
  tp_flag_set(tp, is_able_to_engulf);
  tp_flag_set(tp, is_able_to_lunge);
  tp_flag_set(tp, is_able_to_move_diagonally);
  tp_flag_set(tp, is_able_to_move);
  tp_flag_set(tp, is_able_to_see_invisible);
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
  tp_flag_set(tp, is_combustible);
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
  tp_flag_set(tp, is_physics_explosion);
  tp_flag_set(tp, is_removable_when_dead_on_err);
  tp_flag_set(tp, is_shown_health);
  tp_flag_set(tp, is_tickable);
  tp_flag_set(tp, is_vision_360_degrees);
  tp_health_set(tp, "3d4");
  tp_hearing_threshold_set(tp, 8); // smaller values => better hearing
  tp_monst_group_add(tp, MONST_GROUP2);
  tp_name_a_or_an_set(tp, "an voider");
  tp_name_apostrophize_set(tp, "voider's");
  tp_name_long_set(tp, "voider");
  tp_name_pluralize_set(tp, "voiders");
  tp_name_short_set(tp, "voider");
  tp_priority_set(tp, THING_PRIORITY_MONST);
  tp_rarity_set(tp, THING_RARITY_COMMON);
  tp_score_value_set(tp, 200);
  tp_speed_set(tp, 50);
  tp_stat_set(tp, THING_STAT_ATT, "1d4+14");
  tp_stat_set(tp, THING_STAT_DEF, "6");
  tp_temperature_initial_set(tp, 0); // celsius
  tp_weight_set(tp, WEIGHT_HUMAN);   // grams
  tp_z_depth_set(tp, MAP_Z_DEPTH_FLOATING_MONST);
  tp_flag_set(tp, is_described_when_killed);
  // end sort marker1 }

  auto delay = 1000;

  for (auto frame = 0; frame < 2; frame++) {
    auto *tile = tile_find_mand(name + std::string(".idle.") + std::to_string(frame));
    tile_size_set(tile, OUTLINE_TILE_WIDTH, OUTLINE_TILE_HEIGHT);
    tile_delay_ms_set(tile, delay);
    tp_tiles_push_back(tp, THING_ANIM_IDLE, tile);
  }

  return true;
}
