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

static auto tp_ogrik_description_get(Gamep g, Levelsp v, Levelp l, Thingp me) -> std::string
{
  TRACE();

  if (thing_is_dead(me)) {
    return "dead ogrik";
  }
  return "ogrik";
}

static auto tp_ogrik_detail_get(Gamep g, Levelsp v, Levelp l, Thingp me) -> std::string
{
  TRACE();

  return                                                                                                                       //
      UI_INFO1_FMT_STR "Ogrik are iron based lifeforms that outwardly resemble large rocks and can be mistaken for statues.\n" //
      UI_INFO2_FMT_STR "Due to their ferrous nature, they nourish themselves with blood that they drain upon close contact.\n" //
      UI_INFO3_FMT_STR "Although slow moving, once close, they can attack rapidly and overwhelm the unwary.\n"                 //
      UI_INFO4_FMT_STR "Naturally, due to their tough rock-like skin, Ogrik are impervious to most attacks.\n";                //
}

static auto tp_ogrik_assess_tp(Gamep g, Levelsp v, Levelp l, Tpp tp, Thingp me) -> ThingEnvironType
{
  TRACE_DEBUG();

  return THING_ENVIRON_NEUTRAL;
}

static auto tp_ogrik_assess_tile(Gamep g, Levelsp v, Levelp l, const bpoint &at, Thingp me) -> ThingEnvironType
{
  TRACE_DEBUG();

  return THING_ENVIRON_NEUTRAL;
}

static void tp_ogrik_on_death(Gamep g, Levelsp v, Levelp l, Thingp me, ThingEvent &e)
{
  TRACE();

  (void) thing_spawn(g, v, l, tp_first(is_effect_blood), me);

  thing_sound_play(g, v, l, me, "monst_death");
}

static bool tp_ogrik_on_attacking(Gamep g, Levelsp v, Levelp l, Thingp attacker, Thingp target, ThingEvent &e)
{
  TRACE();

  (void) thing_spawn(g, v, l, tp_first(is_effect_attack), target);

  thing_sound_play(g, v, l, attacker, "rock");

  TpSpecialAttack d;

  if (thing_special_attack_get_random(g, v, l, attacker, target, d)) {
    e.special_attack = d;
    e.event_type     = d.event_type;
    e.damage         = d.dice.roll();
  }

  return true;
}

static bool tp_ogrik_on_missing(Gamep g, Levelsp v, Levelp l, Thingp attacker, Thingp target, ThingEvent &e)
{
  TRACE();

  (void) thing_spawn(g, v, l, tp_first(is_effect_attack), target);

  thing_sound_play(g, v, l, attacker, "rock");

  return true;
}

static void tp_ogrik_on_moved(Gamep g, Levelsp v, Levelp l, Thingp me)
{
  TRACE();

  thing_sound_play(g, v, l, me, "rock");
}

[[nodiscard]] auto tp_load_ogrik() -> bool
{
  auto *tp   = tp_load("ogrik"); // keep as string for scripts
  auto  name = tp_name(tp);

  // begin sort marker1 {
  thing_assess_tile_set(tp, tp_ogrik_assess_tile);
  thing_assess_tp_set(tp, tp_ogrik_assess_tp);
  thing_description_set(tp, tp_ogrik_description_get);
  thing_detail_set(tp, tp_ogrik_detail_get);
  thing_on_attacking_set(tp, tp_ogrik_on_attacking);
  thing_on_death_set(tp, tp_ogrik_on_death);
  thing_on_missing_set(tp, tp_ogrik_on_missing);
  thing_on_moved_set(tp, tp_ogrik_on_moved);
  tp_attack_count_max_per_tick_set(tp, 2);
  tp_distance_vision_set(tp, 12);
  tp_flag_set(tp, is_able_to_be_buffed);
  tp_flag_set(tp, is_able_to_be_invisible);
  tp_flag_set(tp, is_able_to_be_levitated);
  tp_flag_set(tp, is_able_to_be_teleported);
  tp_flag_set(tp, is_able_to_crush_grass);
  tp_flag_set(tp, is_able_to_fall_sound);
  tp_flag_set(tp, is_able_to_fall);
  tp_flag_set(tp, is_able_to_lunge);
  tp_flag_set(tp, is_able_to_move_diagonally);
  tp_flag_set(tp, is_able_to_move);
  tp_flag_set(tp, is_animated_can_hflip);
  tp_flag_set(tp, is_animated);
  tp_flag_set(tp, is_attackable_by_player);
  tp_flag_set(tp, is_biome_bogland);
  tp_flag_set(tp, is_biome_dungeon);
  tp_flag_set(tp, is_biome_underhell);
  tp_flag_set(tp, is_blasted_when_dead);
  tp_flag_set(tp, is_blit_hit_outline_w_invis_inside);
  tp_flag_set(tp, is_blit_on_ground);
  tp_flag_set(tp, is_blit_shown_in_chasms);
  tp_flag_set(tp, is_collision_circle_large);
  tp_flag_set(tp, is_corpse_on_death);
  tp_flag_set(tp, is_described_cursor);
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
  tp_flag_set(tp, is_stone);
  tp_flag_set(tp, is_submergible); // is seen submerged when in water
  tp_flag_set(tp, is_tickable);
  tp_flag_set(tp, is_vision_360_degrees);
  tp_health_set(tp, "4d8");
  tp_hearing_threshold_set(tp, 10); // smaller values => better hearing
  tp_is_immune_to_add(tp, THING_EVENT_FIRE_DAMAGE);
  tp_is_immune_to_add(tp, THING_EVENT_WATER_DAMAGE);
  tp_monst_group_add(tp, MONST_GROUP2);
  tp_name_a_or_an_set(tp, "an ogrik");
  tp_name_apostrophize_set(tp, "ogriks'");
  tp_name_long_set(tp, "ogrik");
  tp_name_pluralize_set(tp, "ogrik");
  tp_name_short_set(tp, "ogrik");
  tp_priority_set(tp, THING_PRIORITY_MONST);
  tp_rarity_set(tp, THING_RARITY_COMMON);
  tp_score_value_set(tp, 300);
  tp_speed_set(tp, 25);
  tp_stat_set(tp, THING_STAT_ATT, "1d8+10");
  tp_stat_set(tp, THING_STAT_DEF, "1d8+12");
  tp_stat_set(tp, THING_STAT_DMG, "1d4+14");
  tp_temperature_damage_at_set(tp, 50); // celsius
  tp_temperature_initial_set(tp, 20);   // celsius
  tp_temperature_melts_at_set(tp, 250); // celsius
  tp_weight_set(tp, WEIGHT_VVHEAVY);    // grams
  tp_z_depth_set(tp, MAP_Z_DEPTH_MONST);
  // end sort marker1 }

  tp_special_attack_add(tp,
                        TpSpecialAttack {
                            .type          = "1",                      //
                            .event_type    = THING_EVENT_CRUSH_DAMAGE, //
                            .name          = "crush attack",           //
                            .roll          = "2d8",                    //
                            .d100          = 100,
                            .when_adjacent = true,
                        });

  auto delay = 2000;

  for (auto frame = 0; frame < 7; frame++) {
    auto *tile = tile_find_mand(name + std::string(".idle.") + std::to_string(frame));
    tile_size_set(tile, OUTLINE_TILE_WIDTH, OUTLINE_TILE_HEIGHT * 2);
    tile_delay_ms_set(tile, delay);
    tp_tiles_push_back(tp, THING_ANIM_IDLE, tile);
  }

  delay = 200;

  for (auto frame = 5; frame < 7; frame++) {
    auto *tile = tile_find_mand(name + std::string(".idle.") + std::to_string(frame));
    tile_size_set(tile, OUTLINE_TILE_WIDTH, OUTLINE_TILE_HEIGHT * 2);
    tile_delay_ms_set(tile, delay);
    tp_tiles_push_back(tp, THING_ANIM_IDLE, tile);
  }

  for (auto frame = 0; frame < 1; frame++) {
    auto *tile = tile_find_mand(name + std::string(".dead.") + std::to_string(frame));
    tile_size_set(tile, OUTLINE_TILE_WIDTH, OUTLINE_TILE_HEIGHT * 2);
    tile_delay_ms_set(tile, delay);
    tp_tiles_push_back(tp, THING_ANIM_DEAD, tile);
  }

  return true;
}
