//
// Copyright goblinhack@gmail.com
//

#include "../../my_callstack.hpp"
#include "../../my_cpp_template.hpp"
#include "../../my_dice_rolls.hpp"
#include "../../my_random.hpp"
#include "../../my_thing.hpp"
#include "../../my_thing_callbacks.hpp"
#include "../../my_thing_inlines.hpp"
#include "../../my_tile.hpp"
#include "../../my_tp.hpp"
#include "../../my_tps.hpp"
#include "../../my_types.hpp"
#include "../../my_ui.hpp"
#include "my_boss_common.hpp"

static const std::initializer_list< bpoint > additional_body_tiles = {
    bpoint(-2, 0),
    bpoint(-1, 0),
    bpoint(0, 0),
    bpoint(1, 0), //
};

static const std::initializer_list< bpoint > tentacle_list = {
    bpoint(0, -3), bpoint(-3, -3), bpoint(3, -3), bpoint(-3, 0), bpoint(3, 0), bpoint(-3, 3), bpoint(3, 3), bpoint(0, 3),
};

static auto tp_boss2_description_get(Gamep g, Levelsp v, Levelp l, Thingp me) -> std::string
{
  TRACE();

  if (thing_is_dead(me)) {
    return "dead kraken";
  }
  return "kraken";
}

static auto tp_boss2_detail_get(Gamep g, Levelsp v, Levelp l, Thingp me) -> std::string
{
  TRACE();

  return                                                                                                                                   //
      UI_INFO1_FMT_STR "The Kraken. The greatest monster of all the oceans, known through millennia for dragging sailors to their doom.\n" //
      UI_INFO2_FMT_STR "Quite what it is doing in a dungeon is a mystery.\n"                                                               //
      UI_INFO3_FMT_STR "Abandon hope all ye that gaze upon the Kraken and its mighty tentacles.\n"                                         //
      UI_INFO4_FMT_STR "By this point, your hope is probably quite low anyway, but if you still cling onto any, best to abandon it.\n";    //
}

static auto tp_boss2_assess_tp(Gamep g, Levelsp v, Levelp l, Tpp tp, Thingp me) -> ThingEnvironType
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

static auto tp_boss2_assess_tile(Gamep g, Levelsp v, Levelp l, const bpoint &at, Thingp me) -> ThingEnvironType
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

static void tp_boss2_on_death(Gamep g, Levelsp v, Levelp l, Thingp me, ThingEvent &e)
{
  TRACE();

  (void) thing_spawn(g, v, l, tp_first(is_effect_blood), me);

  thing_sound_play(g, v, l, me, "monster_roar_epic");

  tp_boss_on_death(g, v, l, me, e);
}

static void tp_boss2_tick_begin(Gamep g, Levelsp v, Levelp l, Thingp me)
{
  TRACE();

  auto target = thing_monst_target(g, v, l, me);
  if (level_is_player(g, v, l, target)) {
    (void) thing_attack_at(g, v, l, me, target);
  }
}

static bool tp_boss2_on_attacking(Gamep g, Levelsp v, Levelp l, Thingp attacker, Thingp target, ThingEvent &e)
{
  THING_DBG(g, v, l, attacker, "boss attack");
  TRACE_INDENT();

  (void) thing_spawn(g, v, l, tp_first(is_effect_attack), target);

  thing_sound_play(g, v, l, attacker, "monster_roar");

  if (d100() < 10) {
    if (! thing_dormant(g, v, l, attacker)) {
      (void) thing_dormant_set(g, v, l, attacker, 20);
    }
  }

  TpSpecialAttack d;

  if (thing_special_attack_get_random(g, v, l, attacker, target, d)) {
    e.special_attack = d;
    e.event_type     = d.event_type;
    e.damage         = d.dice.roll();
  }

  return true;
}

static void tp_boss2_on_pushed(Gamep g, Levelsp v, Levelp l, Thingp me)
{
  TRACE();

  auto at = thing_at(g, v, l, me);

  for (auto p : additional_body_tiles) {
    (void) thing_push_additional(g, v, l, me, at + p);
  }
}

static void tp_boss2_on_popped(Gamep g, Levelsp v, Levelp l, Thingp me)
{
  TRACE();

  auto at = thing_at(g, v, l, me);

  for (auto p : additional_body_tiles) {
    (void) thing_pop_additional(g, v, l, me, at + p);
  }
}

static bool tp_boss2_on_missing(Gamep g, Levelsp v, Levelp l, Thingp attacker, Thingp target, ThingEvent &e)
{
  TRACE_INDENT();

  if (adjacent(thing_at(g, v, l, attacker), thing_at(g, v, l, target))) {
    (void) thing_spawn(g, v, l, tp_first(is_effect_attack), target);
    thing_sound_play(g, v, l, attacker, "hiss");
  }

  return true;
}

void tp_boss2_on_spawned(Gamep g, Levelsp v, Levelp l, Thingp me, ThingEvent *e_maybe_null)
{
  TRACE();
  THING_DBG(g, v, l, me, "boss2 spawned");

  auto at = thing_at(g, v, l, me);

  for (auto p : tentacle_list) {
    (void) thing_mob_spawn_a_minion_at(g, v, l, me, tp_first(is_tentacle), at + p);
  }
}

bool tp_boss2_on_damage(Gamep g, Levelsp v, Levelp l, Thingp me, ThingEvent &e)
{
  TRACE();

  auto attacker = thing_get_attacker(g, v, l, e.source);

  if (thing_dormant(g, v, l, me) != 0) {
    if (attacker && thing_is_player(attacker)) {
      topcon(UI_IMPORTANT_FMT_STR "The Kraken takes no damage while it is deep in the water." UI_RESET_FMT);
    }
    return false;
  }

  if (d100() < 20) {
    (void) thing_dormant_set(g, v, l, me, 20);
  }

  return true;
}

[[nodiscard]] auto tp_load_boss2() -> bool
{
  auto *tp   = tp_load("boss2"); // keep as string for scripts
  auto  name = tp_name(tp);

  // begin sort marker1 {
  thing_assess_tile_set(tp, tp_boss2_assess_tile);
  thing_assess_tp_set(tp, tp_boss2_assess_tp);
  thing_description_set(tp, tp_boss2_description_get);
  thing_detail_set(tp, tp_boss2_detail_get);
  thing_on_attacking_set(tp, tp_boss2_on_attacking);
  thing_on_damage_set(tp, tp_boss2_on_damage);
  thing_on_death_set(tp, tp_boss2_on_death);
  thing_on_missing_set(tp, tp_boss2_on_missing);
  thing_on_popped_set(tp, tp_boss2_on_popped);
  thing_on_pushed_set(tp, tp_boss2_on_pushed);
  thing_on_spawned_set(tp, tp_boss2_on_spawned);
  thing_on_tick_begin_set(tp, tp_boss2_tick_begin);
  tp_additional_body_tiles_set(tp, additional_body_tiles);
  tp_attack_count_max_per_tick_set(tp, 1);
  tp_chance_set(tp, THING_CHANCE_CONTINUE_TO_BURN, "1d6"); // fumble => intensify / keep burning / crit => stop burning
  tp_chance_set(tp, THING_CHANCE_START_BURNING, "1d2");    // fumble => flames spread to you
  tp_damage_set(tp, THING_EVENT_MELEE_DAMAGE, "1d4");
  tp_distance_vision_set(tp, MAP_WIDTH / 2);
  tp_dormant_set(tp, "1d4");
  tp_flag_set(tp, is_able_to_be_buffed);
  tp_flag_set(tp, is_able_to_be_invisible);
  tp_flag_set(tp, is_able_to_breathe);
  tp_flag_set(tp, is_able_to_fall);
  tp_flag_set(tp, is_able_to_move_diagonally);
  tp_flag_set(tp, is_able_to_move);
  tp_flag_set(tp, is_animated_can_hflip);
  tp_flag_set(tp, is_animated);
  tp_flag_set(tp, is_attackable_by_player);
  tp_flag_set(tp, is_blasted_when_dead);
  tp_flag_set(tp, is_blit_centered);
  tp_flag_set(tp, is_blit_hit_solid_white);
  tp_flag_set(tp, is_blit_shown_in_chasms);
  tp_flag_set(tp, is_boss);
  tp_flag_set(tp, is_boss2);
  tp_flag_set(tp, is_collision_circle_large);
  tp_flag_set(tp, is_corpse_on_death);
  tp_flag_set(tp, is_described_cursor);
  tp_flag_set(tp, is_described_when_killed);
  tp_flag_set(tp, is_flesh);
  tp_flag_set(tp, is_loggable);
  tp_flag_set(tp, is_memory_ext);
  tp_flag_set(tp, is_mob_kill_minions_on_death);
  tp_flag_set(tp, is_mob);
  tp_flag_set(tp, is_monst);
  tp_flag_set(tp, is_multi_tile);
  tp_flag_set(tp, is_obs_to_beam);
  tp_flag_set(tp, is_obs_to_jumping_onto);
  tp_flag_set(tp, is_obs_to_movement);
  tp_flag_set(tp, is_obs_to_teleporting_onto);
  tp_flag_set(tp, is_able_to_be_teleported);
  tp_flag_set(tp, is_obs_to_wall_walker);
  tp_flag_set(tp, is_physics_explosion);
  tp_flag_set(tp, is_physics_temperature);
  tp_flag_set(tp, is_removable_when_dead_on_err);
  tp_flag_set(tp, is_shown_health);
  tp_flag_set(tp, is_tickable);
  tp_flag_set(tp, is_vision_360_degrees);
  tp_health_set(tp, "100+12d8");
  tp_hearing_threshold_set(tp, 2); // smaller values => better hearing
  tp_is_immune_to_add(tp, THING_EVENT_WATER_DAMAGE);
  tp_minion_max_set(tp, (int) tentacle_list.size());
  tp_name_a_or_an_set(tp, "kraken");
  tp_name_apostrophize_set(tp, "kraken's");
  tp_name_long_set(tp, "kraken");
  tp_name_pluralize_set(tp, "kraken");
  tp_name_short_set(tp, "kraken");
  tp_priority_set(tp, THING_PRIORITY_MONST);
  tp_rarity_set(tp, THING_RARITY_COMMON);
  tp_score_value_set(tp, 2000);
  tp_speed_set(tp, 50);
  tp_stat_set(tp, THING_STAT_ATT, "1d4+14");
  tp_stat_set(tp, THING_STAT_DEF, "1d4+14");
  tp_stat_set(tp, THING_STAT_DMG, "1d4+14");
  tp_temperature_damage_at_set(tp, 200); // celsius
  tp_temperature_initial_set(tp, 20);    // celsius
  tp_temperature_melts_at_set(tp, 250);  // celsius
  tp_weight_set(tp, WEIGHT_VVHEAVY);     // grams
  tp_z_depth_set(tp, MAP_Z_DEPTH_FLOATING_MONST);
  // end sort marker1 }

  tp_special_attack_add(tp,
                        TpSpecialAttack {
                            .type          = "1",                      //
                            .event_type    = THING_EVENT_CRUSH_DAMAGE, //
                            .name          = "crush attack",           //
                            .roll          = "8d8",                    //
                            .d100          = 100,
                            .when_adjacent = true,
                        });

  auto delay = 1000;

  for (auto frame = 0; frame < 8; frame++) {
    auto *tile = tile_find_mand(name + std::string(".idle.") + std::to_string(frame));
    tile_size_set(tile, BOSS2_TILE_WIDTH, BOSS2_TILE_HEIGHT);
    tile_delay_ms_set(tile, delay);
    tp_tiles_push_back(tp, THING_ANIM_IDLE, tile);
  }

  for (auto frame = 0; frame < 4; frame++) {
    auto *tile = tile_find_mand(name + std::string(".dormant.") + std::to_string(frame));
    tile_size_set(tile, BOSS2_TILE_WIDTH, BOSS2_TILE_HEIGHT);
    tile_delay_ms_set(tile, delay);
    tp_tiles_push_back(tp, THING_ANIM_DORMANT, tile);
  }

  for (auto frame = 0; frame < 4; frame++) {
    auto *tile = tile_find_mand(name + std::string(".dead.") + std::to_string(frame));
    tile_size_set(tile, BOSS2_TILE_WIDTH, BOSS2_TILE_HEIGHT);
    tile_delay_ms_set(tile, delay);
    tp_tiles_push_back(tp, THING_ANIM_DEAD, tile);

    //
    // We want the boss to vanish and not all into a chasm
    //
    if (frame == 3) {
      tile_is_cleanup_on_end_of_anim_set(tile);
    }
  }

  return true;
}
