//
// Copyright goblinhack@gmail.com
//

#include "../../my_callstack.hpp"
#include "../../my_tile.hpp"
#include "../../my_tp.hpp"
#include "../../my_tps.hpp"
#include "../../my_types.hpp"

[[nodiscard]] auto tp_load_dirt() -> bool
{
  TRACE();

  auto *tp   = tp_load("dirt"); // keep as string for scripts
  auto  name = tp_name(tp);
  // begin sort marker1 {
  tp_flag_set(tp, is_blit_centered);
  tp_flag_set(tp, is_blit_if_has_seen);
  tp_flag_set(tp, is_blit_per_pixel_lighting);
  tp_flag_set(tp, is_blit_shown_in_chasms);
  tp_flag_set(tp, is_dirt);
  tp_weight_set(tp, WEIGHT_VVHEAVY); // grams
  tp_z_depth_set(tp, MAP_Z_DEPTH_FLOOR);
  // end sort marker1 }

  auto *tile = tile_find_mand("dirt.1");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_IDLE, tile);
  tile = tile_find_mand("dirt.2");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_IDLE, tile);
  tile = tile_find_mand("dirt.3");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_IDLE, tile);
  tile = tile_find_mand("dirt.4");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_IDLE, tile);
  tile = tile_find_mand("dirt.5");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_IDLE, tile);
  tile = tile_find_mand("dirt.6");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_IDLE, tile);
  tile = tile_find_mand("dirt.7");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_IDLE, tile);
  tile = tile_find_mand("dirt.8");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_IDLE, tile);
  tile = tile_find_mand("dirt.9");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_IDLE, tile);
  tile = tile_find_mand("dirt.10");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_IDLE, tile);
  tile = tile_find_mand("dirt.11");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_IDLE, tile);
  tile = tile_find_mand("dirt.12");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_IDLE, tile);
  tile = tile_find_mand("dirt.13");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_IDLE, tile);
  tile = tile_find_mand("dirt.14");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_IDLE, tile);
  tile = tile_find_mand("dirt.15");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_IDLE, tile);
  tile = tile_find_mand("dirt.16");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_IDLE, tile);

  return true;
}
