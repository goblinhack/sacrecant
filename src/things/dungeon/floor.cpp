//
// Copyright goblinhack@gmail.com
//

#include "../../my_callstack.hpp"
#include "../../my_tile.hpp"
#include "../../my_tp.hpp"
#include "../../my_tps.hpp"
#include "../../my_types.hpp"

[[nodiscard]] auto tp_load_floor() -> bool
{
  TRACE();

  for (auto variant = 1; variant <= FLOOR_VARIANTS; variant++) {
    std::string const name = "floor" + std::to_string(variant);
    auto             *tp   = tp_load(name);
    // begin sort marker1 {
    tp_flag_set(tp, is_blit_centered);
    tp_flag_set(tp, is_blit_if_has_seen);
    tp_flag_set(tp, is_blit_per_pixel_lighting);
    tp_flag_set(tp, is_blit_shown_in_chasms);
    tp_flag_set(tp, is_floor);
    tp_variant_set(tp, variant);
    tp_weight_set(tp, WEIGHT_VVHEAVY); // grams
    tp_z_depth_set(tp, MAP_Z_DEPTH_FLOOR);
    // end sort marker1 }

    for (auto i = 1; i <= 16; i++) {
      auto *tile = tile_find_mand(name + "." + std::to_string(i));
      tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
      tp_tiles_push_back(tp, THING_ANIM_IDLE, tile);
    }
  }

  return true;
}
