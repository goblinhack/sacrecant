//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_thing.hpp"
#include "my_thing_callbacks.hpp"
#include "my_thing_inlines.hpp"
#include "my_tile.hpp"
#include "my_tp.hpp"
#include "my_tps.hpp"
#include "my_types.hpp"
#include "my_ui.hpp"

static auto tp_mantisman_description_get(Gamep g, Levelsp v, Levelp l, Thingp t) -> std::string
{
  TRACE();

  if (thing_is_dead(t)) {
    return "dead mantisman";
  }
  return "mantisman";
}

static auto tp_mantisman_detail_get(Gamep g, Levelsp v, Levelp l, Thingp t) -> std::string
{
  TRACE();

  return                                                                                               //
      UI_INFO1_FMT_STR "MantisMan: half-mantis, half-man but which half?\n"                            //
      UI_INFO2_FMT_STR "These tall bipedal insectoids linger in the spaces that nature forgets.\n"     //
      UI_INFO3_FMT_STR "They hunger for human flesh especially and hang around in chittering groups. " //
      UI_INFO3_FMT_STR "The one thing these mantises do not do... is pray.\n"                          //
      UI_INFO4_FMT_STR "Watch out for their jump attack!\n";
}

static auto tp_mantisman_assess_tile(Gamep g, Levelsp v, Levelp l, const bpoint &at, Thingp t) -> ThingEnviron
{
  TRACE_DEBUG();

  if (level_is_lava_cached(g, v, l, at)) {
    return THING_ENVIRON_HATES;
  }

  if (level_is_chasm_cached(g, v, l, at)) {
    return THING_ENVIRON_HATES;
  }

  if (level_is_water_cached(g, v, l, at)) {
    return THING_ENVIRON_DISLIKES;
  }

  return THING_ENVIRON_NEUTRAL;
}

static void tp_mantisman_on_death(Gamep g, Levelsp v, Levelp l, Thingp t, ThingEvent &e)
{
  TRACE();

  (void) thing_spawn(g, v, l, tp_first(is_effect_blood), t);

  thing_sound_play(g, v, l, t, "monst_death");
}

static bool tp_mantisman_on_attacking(Gamep g, Levelsp v, Levelp l, Thingp me, Thingp it, ThingEvent &e)
{
  TRACE();

  (void) thing_spawn(g, v, l, tp_first(is_effect_attack), it);

  return true;
}

auto tp_load_mantisman() -> bool
{
  auto *tp   = tp_load("mantisman"); // keep as string for scripts
  auto  name = tp_name(tp);

  // begin sort marker1 {
  thing_assess_tile_set(tp, tp_mantisman_assess_tile);
  thing_description_set(tp, tp_mantisman_description_get);
  thing_detail_set(tp, tp_mantisman_detail_get);
  thing_on_attacking_set(tp, tp_mantisman_on_attacking);
  thing_on_death_set(tp, tp_mantisman_on_death);
  tp_chance_set(tp, THING_CHANCE_CONTINUE_TO_BURN, "1d6"); // roll max to continue burning
  tp_chance_set(tp, THING_CHANCE_START_BURNING, "1d2");    // roll max to continue burning
  tp_damage_set(tp, THING_EVENT_MELEE_DAMAGE, "1d3");
  tp_distance_jump_set(tp, 2);
  tp_distance_vision_set(tp, 10);
  tp_flag_set(tp, is_able_to_crush_grass);
  tp_flag_set(tp, is_able_to_fall_sound);
  tp_flag_set(tp, is_able_to_fall);
  tp_flag_set(tp, is_able_to_jump);
  tp_flag_set(tp, is_able_to_lunge);
  tp_flag_set(tp, is_able_to_move_diagonally);
  tp_flag_set(tp, is_animated_can_hflip);
  tp_flag_set(tp, is_animated);
  tp_flag_set(tp, is_attackable_by_player);
  tp_flag_set(tp, is_blit_centered);
  tp_flag_set(tp, is_blit_hit_outline_w_invis_inside);
  tp_flag_set(tp, is_blit_shown_in_chasms);
  tp_flag_set(tp, is_burnable); // is capable of being burned by fire
  tp_flag_set(tp, is_collision_circle_large);
  tp_flag_set(tp, is_corpse_on_death);
  tp_flag_set(tp, is_described_cursor);
  tp_flag_set(tp, is_health_bar_shown);
  tp_flag_set(tp, is_hit_when_dead);
  tp_flag_set(tp, is_insectoid);
  tp_flag_set(tp, is_loggable);
  tp_flag_set(tp, is_mantisman);
  tp_flag_set(tp, is_monst);
  tp_flag_set(tp, is_obs_to_jumping_onto);
  tp_flag_set(tp, is_obs_to_movement);
  tp_flag_set(tp, is_obs_to_teleporting_onto);
  tp_flag_set(tp, is_physics_explosion);
  tp_flag_set(tp, is_physics_temperature);
  tp_flag_set(tp, is_removable_when_dead_on_err);
  tp_flag_set(tp, is_submergible); // is seen submerged when in water
  tp_flag_set(tp, is_tickable);
  tp_flag_set(tp, is_vision_180_degrees);
  tp_health_set(tp, "2d6");
  tp_is_immunity_add(tp, THING_EVENT_WATER_DAMAGE);
  tp_monst_group_add(tp, MONST_GROUP_EASY);
  tp_name_a_or_an_set(tp, "a mantisman");
  tp_name_apostrophize_set(tp, "mantismans'");
  tp_name_long_set(tp, "mantisman");
  tp_name_pluralize_set(tp, "mantismen");
  tp_name_short_set(tp, "mantisman");
  tp_priority_set(tp, THING_PRIORITY_MONST);
  tp_score_value_set(tp, 3);
  tp_speed_set(tp, 100);
  tp_temperature_burns_at_set(tp, 50);  // celsius
  tp_temperature_damage_at_set(tp, 35); // celsius
  tp_temperature_initial_set(tp, 20);   // celsius
  tp_weight_set(tp, WEIGHT_HEAVY);      // grams
  tp_z_depth_set(tp, MAP_Z_DEPTH_OBJ);
  // end sort marker1 }

  auto delay = 500;

  for (auto loops = 0; loops < 10; loops++) {
    for (auto frame = 0; frame < 4; frame++) {
      auto *tile = tile_find_mand(name + std::string(".idle.") + std::to_string(frame));
      tile_size_set(tile, OUTLINE_TILE_WIDTH, OUTLINE_TILE_HEIGHT);
      tile_delay_ms_set(tile, delay);
      tp_tiles_push_back(tp, THING_ANIM_IDLE, tile);
    }
  }

  for (auto frame = 0; frame < 6; frame++) {
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
