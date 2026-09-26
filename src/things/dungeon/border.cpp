//
// Copyright goblinhack@gmail.com
//

#include "../../my_callstack.hpp"
#include "../../my_tile.hpp"
#include "../../my_tp.hpp"
#include "../../my_tps.hpp"
#include "../../my_types.hpp"

[[nodiscard]] auto tp_load_border() -> bool
{
  TRACE();

  std::string const name = "border";
  auto             *tp   = tp_load(name);
  // begin sort marker1 {
  tp_distance_light_penetration_pixels_set(tp, 1);
  tp_flag_set(tp, is_blit_centered);
  tp_flag_set(tp, is_blit_if_has_seen);
  tp_flag_set(tp, is_blit_shown_in_chasms);
  tp_flag_set(tp, is_border);
  tp_flag_set(tp, is_collision_square);
  tp_flag_set(tp, is_critical_to_dungeon_design);
  tp_flag_set(tp, is_indestructible);
  tp_flag_set(tp, is_obs_to_beam);
  tp_flag_set(tp, is_obs_to_cursor_path);
  tp_flag_set(tp, is_obs_to_ethereal);
  tp_flag_set(tp, is_obs_to_explosion);
  tp_flag_set(tp, is_obs_to_falling_onto);
  tp_flag_set(tp, is_obs_to_fire);
  tp_flag_set(tp, is_obs_to_hearing);
  tp_flag_set(tp, is_obs_to_jumping_onto);
  tp_flag_set(tp, is_obs_to_jumping_over);
  tp_flag_set(tp, is_obs_to_movement);
  tp_flag_set(tp, is_obs_to_paths);
  tp_flag_set(tp, is_obs_to_spawning);
  tp_flag_set(tp, is_obs_to_teleporting_onto);
  tp_flag_set(tp, is_obs_to_throwing_onto);
  tp_flag_set(tp, is_obs_to_vision);
  tp_flag_set(tp, is_obs_to_wall_walker);
  tp_flag_set(tp, is_tiled);
  tp_priority_set(tp, THING_PRIORITY_LOWEST);
  tp_weight_set(tp, WEIGHT_VVHEAVY); // grams
  tp_z_depth_set(tp, MAP_Z_DEPTH_WALL);
  // end sort marker1 }

  auto *tile = tile_find_mand(name + ".IS_JOIN_BL");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_JOIN_BL, tile);
  tile = tile_find_mand(name + ".IS_JOIN_BL2");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_JOIN_BL2, tile);
  tile = tile_find_mand(name + ".IS_JOIN_BLOCK");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_JOIN_BLOCK, tile);
  tile = tile_find_mand(name + ".IS_JOIN_BR");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_JOIN_BR, tile);
  tile = tile_find_mand(name + ".IS_JOIN_BR2");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_JOIN_BR2, tile);
  tile = tile_find_mand(name + ".IS_JOIN_HORIZ");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_JOIN_HORIZ, tile);
  tile = tile_find_mand(name + ".IS_JOIN_LEFT");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_JOIN_LEFT, tile);
  tile = tile_find_mand(name + ".IS_JOIN_NODE");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_JOIN_NODE, tile);
  tile = tile_find_mand(name + ".IS_JOIN_RIGHT");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_JOIN_RIGHT, tile);
  tile = tile_find_mand(name + ".IS_JOIN_T_1");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_JOIN_T_1, tile);
  tile = tile_find_mand(name + ".IS_JOIN_T_2");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_JOIN_T_2, tile);
  tile = tile_find_mand(name + ".IS_JOIN_T_3");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_JOIN_T_3, tile);
  tile = tile_find_mand(name + ".IS_JOIN_T");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_JOIN_T, tile);
  tile = tile_find_mand(name + ".IS_JOIN_T180_1");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_JOIN_T180_1, tile);
  tile = tile_find_mand(name + ".IS_JOIN_T180_2");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_JOIN_T180_2, tile);
  tile = tile_find_mand(name + ".IS_JOIN_T180_3");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_JOIN_T180_3, tile);
  tile = tile_find_mand(name + ".IS_JOIN_T180");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_JOIN_T180, tile);
  tile = tile_find_mand(name + ".IS_JOIN_T270_1");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_JOIN_T270_1, tile);
  tile = tile_find_mand(name + ".IS_JOIN_T270_2");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_JOIN_T270_2, tile);
  tile = tile_find_mand(name + ".IS_JOIN_T270_3");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_JOIN_T270_3, tile);
  tile = tile_find_mand(name + ".IS_JOIN_T270");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_JOIN_T270, tile);
  tile = tile_find_mand(name + ".IS_JOIN_T90_1");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_JOIN_T90_1, tile);
  tile = tile_find_mand(name + ".IS_JOIN_T90_2");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_JOIN_T90_2, tile);
  tile = tile_find_mand(name + ".IS_JOIN_T90_3");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_JOIN_T90_3, tile);
  tile = tile_find_mand(name + ".IS_JOIN_T90");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_JOIN_T90, tile);
  tile = tile_find_mand(name + ".IS_JOIN_TL");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_JOIN_TL, tile);
  tile = tile_find_mand(name + ".IS_JOIN_TL2");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_JOIN_TL2, tile);
  tile = tile_find_mand(name + ".IS_JOIN_TOP");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_JOIN_TOP, tile);
  tile = tile_find_mand(name + ".IS_JOIN_BOT");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_JOIN_BOT, tile);
  tile = tile_find_mand(name + ".IS_JOIN_TR");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_JOIN_TR, tile);
  tile = tile_find_mand(name + ".IS_JOIN_TR2");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_JOIN_TR2, tile);
  tile = tile_find_mand(name + ".IS_JOIN_VERT");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_JOIN_VERT, tile);
  tile = tile_find_mand(name + ".IS_JOIN_X");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_JOIN_X, tile);
  tile = tile_find_mand(name + ".IS_JOIN_X1_180");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_JOIN_X1_180, tile);
  tile = tile_find_mand(name + ".IS_JOIN_X1_270");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_JOIN_X1_270, tile);
  tile = tile_find_mand(name + ".IS_JOIN_X1_90");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_JOIN_X1_90, tile);
  tile = tile_find_mand(name + ".IS_JOIN_X1");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_JOIN_X1, tile);
  tile = tile_find_mand(name + ".IS_JOIN_X2_180");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_JOIN_X2_180, tile);
  tile = tile_find_mand(name + ".IS_JOIN_X2_270");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_JOIN_X2_270, tile);
  tile = tile_find_mand(name + ".IS_JOIN_X2_90");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_JOIN_X2_90, tile);
  tile = tile_find_mand(name + ".IS_JOIN_X2");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_JOIN_X2, tile);
  tile = tile_find_mand(name + ".IS_JOIN_X3_180");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_JOIN_X3_180, tile);
  tile = tile_find_mand(name + ".IS_JOIN_X3");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_JOIN_X3, tile);
  tile = tile_find_mand(name + ".IS_JOIN_X4_180");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_JOIN_X4_180, tile);
  tile = tile_find_mand(name + ".IS_JOIN_X4_270");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_JOIN_X4_270, tile);
  tile = tile_find_mand(name + ".IS_JOIN_X4_90");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_JOIN_X4_90, tile);
  tile = tile_find_mand(name + ".IS_JOIN_X4");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_JOIN_X4, tile);

  return true;
}
