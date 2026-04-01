//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_level.hpp"
#include "my_thing_callbacks.hpp"
#include "my_tile.hpp"
#include "my_tp.hpp"
#include "my_tps.hpp"
#include "my_types.hpp"
#include "my_ui.hpp"

static auto tp_ghost_description_get(Gamep g, Levelsp v, Levelp l, Thingp t) -> std::string
{
  TRACE();

  return "vengeful spirit";
}

static auto tp_ghost_detail_get(Gamep g, Levelsp v, Levelp l, Thingp t) -> std::string
{
  TRACE();

  return                                                              //
      UI_INFO1_FMT_STR "The spirit of one less fortunate than you.\n" //
      UI_INFO2_FMT_STR "Beware, such spirits can traverse chasms and float through solid rock.";
}

static auto tp_ghost_assess_tile(Gamep g, Levelsp v, Levelp l, const bpoint &at, Thingp t) -> ThingEnviron
{
  TRACE_DEBUG();

  if (level_is_water_cached(g, v, l, at)) {
    return THING_ENVIRON_HATES;
  }

  return THING_ENVIRON_NEUTRAL;
}

static bool tp_ghost_on_attacking(Gamep g, Levelsp v, Levelp l, Thingp me, Thingp it, ThingEvent &e)
{
  TRACE();

  (void) thing_spawn(g, v, l, tp_first(is_effect_attack), it);

  return true;
}

static void tp_ghost_on_death(Gamep g, Levelsp v, Levelp l, Thingp t, ThingEvent &e)
{
  TRACE();

  thing_sound_play(g, v, l, t, "monst_death");
}

auto tp_load_ghost() -> bool
{
  auto *tp   = tp_load("ghost"); // keep as string for scripts
  auto  name = tp_name(tp);

  // begin sort marker1 {
  thing_assess_tile_set(tp, tp_ghost_assess_tile);
  thing_description_set(tp, tp_ghost_description_get);
  thing_detail_set(tp, tp_ghost_detail_get);
  thing_on_attacking_set(tp, tp_ghost_on_attacking);
  thing_on_death_set(tp, tp_ghost_on_death);
  tp_damage_set(tp, THING_EVENT_MELEE_DAMAGE, "1d1");
  tp_distance_minion_from_mob_max_set(tp, 6);
  tp_distance_vision_set(tp, 12);
  tp_flag_set(tp, is_able_to_lunge);
  tp_flag_set(tp, is_able_to_move_diagonally);
  tp_flag_set(tp, is_able_to_move_through_walls);
  tp_flag_set(tp, is_able_to_walk_through_walls);
  tp_flag_set(tp, is_animated_can_hflip);
  tp_flag_set(tp, is_animated);
  tp_flag_set(tp, is_attackable_by_player);
  tp_flag_set(tp, is_blit_centered);
  tp_flag_set(tp, is_blit_hit_effect1);
  tp_flag_set(tp, is_blit_shown_in_chasms);
  tp_flag_set(tp, is_blit_when_obscured_faded);
  tp_flag_set(tp, is_collision_circle_large);
  tp_flag_set(tp, is_corpse_on_death);
  tp_flag_set(tp, is_described_cursor);
  tp_flag_set(tp, is_ethereal);
  tp_flag_set(tp, is_floating);
  tp_flag_set(tp, is_ghost);
  tp_flag_set(tp, is_health_bar_shown);
  tp_flag_set(tp, is_levitating);
  tp_flag_set(tp, is_light_source, 1);
  tp_flag_set(tp, is_loggable);
  tp_flag_set(tp, is_minion);
  tp_flag_set(tp, is_monst);
  tp_flag_set(tp, is_removable_when_dead_on_err);
  tp_flag_set(tp, is_tickable);
  tp_flag_set(tp, is_undead);
  tp_flag_set(tp, is_vision_180_degrees);
  tp_health_set(tp, "1");
  tp_is_immunity_add(tp, THING_EVENT_WATER_DAMAGE);
  tp_light_color_set(tp, "cyan");
  tp_name_a_or_an_set(tp, "a ghost");
  tp_name_apostrophize_set(tp, "ghosts'");
  tp_name_long_set(tp, "ghost");
  tp_name_pluralize_set(tp, "ghosts");
  tp_name_short_set(tp, "ghost");
  tp_priority_set(tp, THING_PRIORITY_MONST);
  tp_speed_set(tp, 100);
  tp_temperature_initial_set(tp, -10); // celsius
  tp_weight_set(tp, WEIGHT_FEATHER);   // grams
  tp_z_depth_set(tp, MAP_Z_DEPTH_OBJ);
  // end sort marker1 }

  auto delay = 1000;

  for (auto frame = 0; frame < 2; frame++) {
    auto *tile = tile_find_mand(name + std::string(".idle.") + std::to_string(frame));
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tile_delay_ms_set(tile, delay);
    tp_tiles_push_back(tp, THING_ANIM_IDLE, tile);
  }

  delay = 100;

  for (auto frame = 0; frame < 3; frame++) {
    auto *tile = tile_find_mand(name + std::string(".dead.") + std::to_string(frame));
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tile_delay_ms_set(tile, delay);
    tp_tiles_push_back(tp, THING_ANIM_DEAD, tile);

    if (frame == 2) {
      tile_is_cleanup_on_end_of_anim_set(tile);
    }
  }

  return true;
}
