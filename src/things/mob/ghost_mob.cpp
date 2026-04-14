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

static auto tp_ghost_mob_description_get(Gamep g, Levelsp v, Levelp l, Thingp t) -> std::string
{
  TRACE();

  return "pile of bones";
}

static auto tp_ghost_mob_detail_get(Gamep g, Levelsp v, Levelp l, Thingp t) -> std::string
{
  TRACE();

  return                                                                                                       //
      UI_INFO1_FMT_STR "This large pile of bones, whispers and moans as spectral creations dance around it.\n" //
      UI_INFO2_FMT_STR "When such devices are destroyed, all that they summoned will also vanish into oblivion.";
}

static void tp_ghost_mob_on_death(Gamep g, Levelsp v, Levelp l, Thingp t, ThingEvent &e)
{
  TRACE();

  thing_sound_play(g, v, l, t, "explosion");
}

static void tp_ghost_mob_tick_begin(Gamep g, Levelsp v, Levelp l, Thingp t)
{
  TRACE();

  //
  // Spawn sometimes
  //
  if (d100() < 90) {
    return;
  }

  if (level_is_monst(g, v, l, thing_at(t)) == nullptr) {
    (void) thing_mob_spawn_a_minion(g, v, l, t, tp_first(is_ghost));
  }
}

auto tp_load_ghost_mob() -> bool
{
  auto *tp   = tp_load("ghost_mob"); // keep as string for scripts
  auto  name = tp_name(tp);

  // begin sort marker1 {
  thing_description_set(tp, tp_ghost_mob_description_get);
  thing_detail_set(tp, tp_ghost_mob_detail_get);
  thing_on_death_set(tp, tp_ghost_mob_on_death);
  thing_on_tick_begin_set(tp, tp_ghost_mob_tick_begin);
  tp_flag_set(tp, is_able_to_fall_sound);
  tp_flag_set(tp, is_able_to_fall);
  tp_flag_set(tp, is_animated_can_hflip);
  tp_flag_set(tp, is_animated);
  tp_flag_set(tp, is_biome_dungeon);
  tp_flag_set(tp, is_biome_graveyard);
  tp_flag_set(tp, is_biome_nethervoid);
  tp_flag_set(tp, is_biome_underhell);
  tp_flag_set(tp, is_blit_centered);
  tp_flag_set(tp, is_blit_hit_outline_w_black_inside);
  tp_flag_set(tp, is_blit_shown_in_chasms);
  tp_flag_set(tp, is_burnable); // is capable of being burned by fire
  tp_flag_set(tp, is_collision_square);
  tp_flag_set(tp, is_combustible); // will continue to burn once on fire
  tp_flag_set(tp, is_corpse_on_death);
  tp_flag_set(tp, is_cursor_path_hazard);
  tp_flag_set(tp, is_described_cursor);
  tp_flag_set(tp, is_dmap);
  tp_flag_set(tp, is_health_visible);
  tp_flag_set(tp, is_light_source, 3);
  tp_flag_set(tp, is_loggable);
  tp_flag_set(tp, is_mob_kill_minions_on_death);
  tp_flag_set(tp, is_mob);
  tp_flag_set(tp, is_mob1);
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
  tp_health_set(tp, "1d3+2");
  tp_light_color_set(tp, "cyan");
  tp_minion_max_set(tp, 5);
  tp_name_a_or_an_set(tp, "a pile of bones");
  tp_name_apostrophize_set(tp, "pile of bones'");
  tp_name_long_set(tp, "pile of bones");
  tp_name_pluralize_set(tp, "piles of bones");
  tp_name_short_set(tp, "bone mob");
  tp_priority_set(tp, THING_PRIORITY_MOB);
  tp_speed_set(tp, 100);
  tp_temperature_burns_at_set(tp, 100);  // celsius
  tp_temperature_damage_at_set(tp, 100); // celsius
  tp_temperature_initial_set(tp, 0);     // celsius
  tp_weight_set(tp, WEIGHT_HEAVY);       // grams
  tp_z_depth_set(tp, MAP_Z_DEPTH_OBJ);
  // end sort marker1 }

  auto delay = 1000;

  for (auto frame = 0; frame < 2; frame++) {
    auto *tile = tile_find_mand(name + std::string(".idle.") + std::to_string(frame));
    tile_delay_ms_set(tile, delay);
    tp_tiles_push_back(tp, THING_ANIM_IDLE, tile);
  }

  delay = 200;

  for (auto frame = 0; frame < 6; frame++) {
    auto *tile = tile_find_mand(name + std::string(".dead.") + std::to_string(frame));
    tile_delay_ms_set(tile, delay);
    tp_tiles_push_back(tp, THING_ANIM_DEAD, tile);

    if (frame == 5) {
      tile_is_cleanup_on_end_of_anim_set(tile);
    }
  }

  return true;
}
