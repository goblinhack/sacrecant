//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_thing_callbacks.hpp"
#include "my_tile.hpp"
#include "my_tp.hpp"
#include "my_tps.hpp"
#include "my_types.hpp"

static void tp_vault_melt(Gamep g, Levelsp v, Levelp l, Thingp t)
{
  TRACE();

  ThingEvent e {
      .reason     = "by melting",     //
      .event_type = THING_EVENT_MELT, //
  };

  thing_dead(g, v, l, t, e);
  (void) thing_spawn(g, v, l, tp_first(is_lava), t);
}

auto tp_load_vault() -> bool
{
  TRACE();

  for (auto variant = 1; variant <= WALL_VARIANTS; variant++) {
    std::string const tile_name = "wall" + std::to_string(variant);
    std::string const name      = "vault" + std::to_string(variant);
    auto             *tp        = tp_load(name);
    // begin sort marker1 {
    thing_on_melt_set(tp, tp_vault_melt);
    tp_flag_set(tp, is_blit_centered);
    tp_flag_set(tp, is_blit_if_has_seen);
    tp_flag_set(tp, is_blit_obscures);
    tp_flag_set(tp, is_blit_per_pixel_lighting);
    tp_flag_set(tp, is_blit_shown_in_chasms);
    tp_flag_set(tp, is_collision_square);
    tp_flag_set(tp, is_meltable);
    tp_flag_set(tp, is_obs_to_cursor_path);
    tp_flag_set(tp, is_obs_to_explosion);
    tp_flag_set(tp, is_obs_to_falling_onto);
    tp_flag_set(tp, is_obs_to_fire);
    tp_flag_set(tp, is_obs_to_jump_over);
    tp_flag_set(tp, is_obs_to_jumping_onto);
    tp_flag_set(tp, is_obs_to_movement);
    tp_flag_set(tp, is_obs_to_paths);
    tp_flag_set(tp, is_obs_to_spawning);
    tp_flag_set(tp, is_obs_to_teleporting_onto);
    tp_flag_set(tp, is_obs_to_vision);
    tp_flag_set(tp, is_physics_explosion);
    tp_flag_set(tp, is_physics_temperature);
    tp_flag_set(tp, is_stone);
    tp_flag_set(tp, is_teleport_blocked);
    tp_flag_set(tp, is_tiled);
    tp_flag_set(tp, is_vault);
    tp_health_set(tp, "1d200+100");
    tp_is_immunity_add(tp, THING_EVENT_MELEE_DAMAGE);
    tp_is_immunity_add(tp, THING_EVENT_WATER_DAMAGE);
    tp_name_a_or_an_set(tp, "a vault");
    tp_name_apostrophize_set(tp, "vaults'");
    tp_name_long_set(tp, "vault");
    tp_name_pluralize_set(tp, "vaults");
    tp_name_short_set(tp, "vault");
    tp_priority_set(tp, THING_PRIORITY_WALL);
    tp_temperature_damage_at_set(tp, 500); // celsius
    tp_temperature_initial_set(tp, 20);    // celsius
    tp_temperature_melts_at_set(tp, 1000); // celsius
    tp_tile_name_set(tp, tile_name);
    tp_variant_set(tp, variant);
    tp_weight_set(tp, WEIGHT_VVHEAVY); // grams
    tp_z_depth_set(tp, MAP_Z_DEPTH_WALL);
    // end sort marker1 }

    auto *tile = tile_find_mand(tile_name + ".IS_JOIN_BL");
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_BL, tile);
    tile = tile_find_mand(tile_name + ".IS_JOIN_BL2");
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_BL2, tile);
    tile = tile_find_mand(tile_name + ".IS_JOIN_BLOCK");
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_BLOCK, tile);
    tile = tile_find_mand(tile_name + ".IS_JOIN_BR");
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_BR, tile);
    tile = tile_find_mand(tile_name + ".IS_JOIN_BR2");
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_BR2, tile);
    tile = tile_find_mand(tile_name + ".IS_JOIN_HORIZ");
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_HORIZ, tile);
    tile = tile_find_mand(tile_name + ".IS_JOIN_LEFT");
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_LEFT, tile);
    tile = tile_find_mand(tile_name + ".IS_JOIN_NODE");
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_NODE, tile);
    tile = tile_find_mand(tile_name + ".IS_JOIN_RIGHT");
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_RIGHT, tile);
    tile = tile_find_mand(tile_name + ".IS_JOIN_T_1");
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_T_1, tile);
    tile = tile_find_mand(tile_name + ".IS_JOIN_T_2");
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_T_2, tile);
    tile = tile_find_mand(tile_name + ".IS_JOIN_T_3");
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_T_3, tile);
    tile = tile_find_mand(tile_name + ".IS_JOIN_T");
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_T, tile);
    tile = tile_find_mand(tile_name + ".IS_JOIN_T180_1");
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_T180_1, tile);
    tile = tile_find_mand(tile_name + ".IS_JOIN_T180_2");
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_T180_2, tile);
    tile = tile_find_mand(tile_name + ".IS_JOIN_T180_3");
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_T180_3, tile);
    tile = tile_find_mand(tile_name + ".IS_JOIN_T180");
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_T180, tile);
    tile = tile_find_mand(tile_name + ".IS_JOIN_T270_1");
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_T270_1, tile);
    tile = tile_find_mand(tile_name + ".IS_JOIN_T270_2");
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_T270_2, tile);
    tile = tile_find_mand(tile_name + ".IS_JOIN_T270_3");
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_T270_3, tile);
    tile = tile_find_mand(tile_name + ".IS_JOIN_T270");
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_T270, tile);
    tile = tile_find_mand(tile_name + ".IS_JOIN_T90_1");
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_T90_1, tile);
    tile = tile_find_mand(tile_name + ".IS_JOIN_T90_2");
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_T90_2, tile);
    tile = tile_find_mand(tile_name + ".IS_JOIN_T90_3");
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_T90_3, tile);
    tile = tile_find_mand(tile_name + ".IS_JOIN_T90");
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_T90, tile);
    tile = tile_find_mand(tile_name + ".IS_JOIN_TL");
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_TL, tile);
    tile = tile_find_mand(tile_name + ".IS_JOIN_TL2");
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_TL2, tile);
    tile = tile_find_mand(tile_name + ".IS_JOIN_TOP");
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_TOP, tile);
    tile = tile_find_mand(tile_name + ".IS_JOIN_BOT");
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_BOT, tile);
    tile = tile_find_mand(tile_name + ".IS_JOIN_TR");
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_TR, tile);
    tile = tile_find_mand(tile_name + ".IS_JOIN_TR2");
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_TR2, tile);
    tile = tile_find_mand(tile_name + ".IS_JOIN_VERT");
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_VERT, tile);
    tile = tile_find_mand(tile_name + ".IS_JOIN_X");
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_X, tile);
    tile = tile_find_mand(tile_name + ".IS_JOIN_X1_180");
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_X1_180, tile);
    tile = tile_find_mand(tile_name + ".IS_JOIN_X1_270");
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_X1_270, tile);
    tile = tile_find_mand(tile_name + ".IS_JOIN_X1_90");
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_X1_90, tile);
    tile = tile_find_mand(tile_name + ".IS_JOIN_X1");
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_X1, tile);
    tile = tile_find_mand(tile_name + ".IS_JOIN_X2_180");
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_X2_180, tile);
    tile = tile_find_mand(tile_name + ".IS_JOIN_X2_270");
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_X2_270, tile);
    tile = tile_find_mand(tile_name + ".IS_JOIN_X2_90");
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_X2_90, tile);
    tile = tile_find_mand(tile_name + ".IS_JOIN_X2");
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_X2, tile);
    tile = tile_find_mand(tile_name + ".IS_JOIN_X3_180");
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_X3_180, tile);
    tile = tile_find_mand(tile_name + ".IS_JOIN_X3");
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_X3, tile);
    tile = tile_find_mand(tile_name + ".IS_JOIN_X4_180");
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_X4_180, tile);
    tile = tile_find_mand(tile_name + ".IS_JOIN_X4_270");
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_X4_270, tile);
    tile = tile_find_mand(tile_name + ".IS_JOIN_X4_90");
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_X4_90, tile);
    tile = tile_find_mand(tile_name + ".IS_JOIN_X4");
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_X4, tile);
  }

  return true;
}
