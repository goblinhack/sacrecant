//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_dice_rolls.hpp"
#include "my_level.hpp"
#include "my_sound.hpp"
#include "my_thing_callbacks.hpp"
#include "my_tile.hpp"
#include "my_tp.hpp"
#include "my_tps.hpp"
#include "my_types.hpp"
#include "my_ui.hpp"

static auto tp_kobalos_mob_description_get(Gamep g, Levelsp v, Levelp l, Thingp t) -> std::string
{
  TRACE();

  return "kobalos spawning grounds";
}

static auto tp_kobalos_mob_detail_get(Gamep g, Levelsp v, Levelp l, Thingp t) -> std::string
{
  TRACE();

  return UI_INFO1_FMT_STR "This hideous, puslating creation is a kobalos spawning device.";
}

static void tp_kobalos_mob_on_death(Gamep g, Levelsp v, Levelp l, Thingp t, ThingEvent &e)
{
  TRACE();

  thing_sound_play(g, v, l, t, "explosion");
}

static void tp_kobalos_mob_tick_begin(Gamep g, Levelsp v, Levelp l, Thingp t)
{
  TRACE();

  if (d100() < 90) {
    return;
  }

  if (level_is_monst(g, v, l, thing_at(t)) == nullptr) {
    (void) thing_mob_spawn_a_minion(g, v, l, t, tp_first(is_kobalos));
  }
}

auto tp_load_kobalos_mob() -> bool
{
  auto *tp   = tp_load("kobalos_mob"); // keep as string for scripts
  auto  name = tp_name(tp);

  // begin sort marker1 {
  thing_description_set(tp, tp_kobalos_mob_description_get);
  thing_detail_set(tp, tp_kobalos_mob_detail_get);
  thing_on_death_set(tp, tp_kobalos_mob_on_death);
  thing_on_tick_begin_set(tp, tp_kobalos_mob_tick_begin);
  tp_flag_set(tp, is_able_to_fall_sound);
  tp_flag_set(tp, is_able_to_fall);
  tp_flag_set(tp, is_animated_can_hflip);
  tp_flag_set(tp, is_animated);
  tp_flag_set(tp, is_blit_centered);
  tp_flag_set(tp, is_blit_hit_effect2);
  tp_flag_set(tp, is_blit_shown_in_chasms);
  tp_flag_set(tp, is_burnable); // is capable of being burned by fire
  tp_flag_set(tp, is_collision_square);
  tp_flag_set(tp, is_combustible); // will continue to burn once on fire
  tp_flag_set(tp, is_corpse_on_death);
  tp_flag_set(tp, is_cursor_path_hazard);
  tp_flag_set(tp, is_described_cursor);
  tp_flag_set(tp, is_dmap);
  tp_flag_set(tp, is_health_bar_shown);
  tp_flag_set(tp, is_light_source, 3);
  tp_flag_set(tp, is_loggable);
  tp_flag_set(tp, is_mob);
  tp_flag_set(tp, is_mob2);
  tp_flag_set(tp, is_obs_to_jumping_onto);
  tp_flag_set(tp, is_obs_to_movement);
  tp_flag_set(tp, is_obs_to_teleporting_onto);
  tp_flag_set(tp, is_physics_explosion);
  tp_flag_set(tp, is_physics_temperature);
  tp_flag_set(tp, is_physics_water);
  tp_flag_set(tp, is_shovable);
  tp_flag_set(tp, is_stone);
  tp_flag_set(tp, is_submergible); // is seen submerged when in water
  tp_flag_set(tp, is_tickable);
  tp_flag_set(tp, is_wait_on_dead_anim);
  tp_health_set(tp, "1d3+3");
  tp_light_color_set(tp, "green");
  tp_minion_max_set(tp, 5);
  tp_name_a_or_an_set(tp, "a kobalos mob");
  tp_name_apostrophize_set(tp, "kobalos mob'");
  tp_name_long_set(tp, "kobalos mob");
  tp_name_pluralize_set(tp, "kobalos mobs");
  tp_name_short_set(tp, "kobalos mob");
  tp_priority_set(tp, THING_PRIORITY_MOB);
  tp_speed_set(tp, 100);
  tp_temperature_burns_at_set(tp, 100);  // celsius
  tp_temperature_damage_at_set(tp, 100); // celsius
  tp_temperature_initial_set(tp, 20);    // celsius
  tp_weight_set(tp, WEIGHT_HEAVY);       // grams
  tp_z_depth_set(tp, MAP_Z_DEPTH_OBJ);
  // end sort marker1 }

  auto delay = 1000;

  for (auto frame = 0; frame < 2; frame++) {
    auto *tile = tile_find_mand(name + std::string(".idle.") + std::to_string(frame));
    tile_delay_ms_set(tile, delay);
    tp_tiles_push_back(tp, THING_ANIM_IDLE, tile);
    tile_size_set(tile, OUTLINE_TILE_WIDTH, OUTLINE_TILE_HEIGHT);
  }

  delay = 100;

  for (auto frame = 0; frame < 6; frame++) {
    auto *tile = tile_find_mand(name + std::string(".dead.") + std::to_string(frame));
    tile_delay_ms_set(tile, delay);
    tp_tiles_push_back(tp, THING_ANIM_DEAD, tile);
    tile_size_set(tile, OUTLINE_TILE_WIDTH, OUTLINE_TILE_HEIGHT);

    if (frame == 5) {
      tile_is_cleanup_on_end_of_anim_set(tile);
    }
  }

  return true;
}
