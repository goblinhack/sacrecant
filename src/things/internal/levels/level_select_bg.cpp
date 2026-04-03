//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_tile.hpp"
#include "my_tp.hpp"
#include "my_tps.hpp"
#include "my_types.hpp"

auto tp_load_level_select_bg() -> bool
{
  TRACE();

  auto *tp   = tp_load("level_select_bg"); // keep as string for scripts
  auto  name = tp_name(tp);

  // begin sort marker1 {
  tp_flag_set(tp, is_level_select_bg);
  tp_z_depth_set(tp, MAP_Z_DEPTH_OBJ);
  // end sort marker1 }

  auto *tile = tile_find_mand("is_level_select_bg");
  tp_tiles_push_back(tp, THING_ANIM_IDLE, tile);

  return true;
}
