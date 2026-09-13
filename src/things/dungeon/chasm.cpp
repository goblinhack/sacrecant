//
// Copyright goblinhack@gmail.com
//

#include "../../my_callstack.hpp"
#include "../../my_thing_callbacks.hpp"
#include "../../my_tile.hpp"
#include "../../my_tp.hpp"
#include "../../my_tps.hpp"
#include "../../my_types.hpp"

static auto tp_chasm_description_get(Gamep g, Levelsp v, Levelp l, Thingp me) -> std::string
{
  TRACE();

  return "gaping chasm";
}

[[nodiscard]] auto tp_load_chasm() -> bool
{
  TRACE();

  auto *tp   = tp_load("chasm"); // keep as string for scripts
  auto  name = tp_name(tp);
  // begin sort marker1 {
  thing_description_set(tp, tp_chasm_description_get);
  tp_flag_set(tp, is_blit_centered);
  tp_flag_set(tp, is_blit_if_has_seen);
  tp_flag_set(tp, is_blit_per_pixel_lighting);
  tp_flag_set(tp, is_blit_shown_in_chasms);
  tp_flag_set(tp, is_chasm);
  tp_flag_set(tp, is_cursor_path_hazard);
  tp_flag_set(tp, is_cursor_path_none);
  tp_flag_set(tp, is_cursor_path_warning);
  tp_flag_set(tp, is_described_cursor);
  tp_flag_set(tp, is_needs_move_confirm);
  tp_flag_set(tp, is_obs_to_explosion);
  tp_flag_set(tp, is_obs_to_jumping_onto);
  tp_flag_set(tp, is_soft_landing);
  tp_flag_set(tp, is_teleport_hazard);
  tp_flag_set(tp, is_tick_end_delay);
  tp_flag_set(tp, is_tickable);
  tp_flag_set(tp, is_tiled);
  tp_name_a_or_an_set(tp, "a chasm");
  tp_name_apostrophize_set(tp, "chasms'");
  tp_name_long_set(tp, "chasm");
  tp_name_pluralize_set(tp, "chasms");
  tp_name_short_set(tp, "chasm");
  tp_weight_set(tp, WEIGHT_NONE); // grams
  tp_z_depth_set(tp, MAP_Z_DEPTH_FLOOR);
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
