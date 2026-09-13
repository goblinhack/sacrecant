//
// Copyright goblinhack@gmail.com
//

#include "../../my_callstack.hpp"
#include "../../my_tile.hpp"
#include "../../my_tp.hpp"
#include "../../my_tps.hpp"
#include "../../my_types.hpp"

[[nodiscard]] auto tp_load_corridor() -> bool
{
  TRACE();

  auto *tp   = tp_load("corridor"); // keep as string for scripts
  auto  name = tp_name(tp);
  // begin sort marker1 {
  tp_flag_set(tp, is_blit_centered);
  tp_flag_set(tp, is_blit_if_has_seen);
  tp_flag_set(tp, is_blit_per_pixel_lighting);
  tp_flag_set(tp, is_blit_shown_in_chasms);
  tp_flag_set(tp, is_corridor);
  tp_weight_set(tp, WEIGHT_VVHEAVY); // grams
  tp_z_depth_set(tp, MAP_Z_DEPTH_FLOOR);
  // end sort marker1 }

  auto *tile = tile_find_mand("corridor.1");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_IDLE, tile);
  tile = tile_find_mand("corridor.2");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_IDLE, tile);
  tile = tile_find_mand("corridor.3");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_IDLE, tile);
  tile = tile_find_mand("corridor.4");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_IDLE, tile);
  tile = tile_find_mand("corridor.5");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_IDLE, tile);
  tile = tile_find_mand("corridor.6");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_IDLE, tile);
  tile = tile_find_mand("corridor.7");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_IDLE, tile);
  tile = tile_find_mand("corridor.8");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_IDLE, tile);
  tile = tile_find_mand("corridor.9");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_IDLE, tile);
  tile = tile_find_mand("corridor.10");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_IDLE, tile);
  tile = tile_find_mand("corridor.11");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_IDLE, tile);
  tile = tile_find_mand("corridor.12");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_IDLE, tile);
  tile = tile_find_mand("corridor.13");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_IDLE, tile);
  tile = tile_find_mand("corridor.14");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_IDLE, tile);
  tile = tile_find_mand("corridor.15");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_IDLE, tile);
  tile = tile_find_mand("corridor.16");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_IDLE, tile);

  return true;
}
