//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_sound.hpp"
#include "my_thing_callbacks.hpp"
#include "my_tile.hpp"
#include "my_tp.hpp"
#include "my_tps.hpp"
#include "my_types.hpp"

auto tp_load_effect_attack() -> bool
{
  TRACE();

  auto *tp   = tp_load("effect_attack"); // keep as string for scripts
  auto  name = tp_name(tp);

  // begin sort marker1 {
  tp_flag_set(tp, is_animated_can_hflip);
  tp_flag_set(tp, is_animated);
  tp_flag_set(tp, is_blit_centered);
  tp_flag_set(tp, is_blit_shown_in_chasms);
  tp_flag_set(tp, is_blit_shown_in_overlay);
  tp_flag_set(tp, is_effect_attack);
  tp_flag_set(tp, is_effect);
  tp_flag_set(tp, is_removable_on_err);
  tp_flag_set(tp, is_submergible);
  tp_flag_set(tp, is_teleport_blocked);
  tp_flag_set(tp, is_wait_on_dead_anim);
  tp_z_depth_set(tp, MAP_Z_DEPTH_EFFECT);
  // end sort marker1 }

  auto delay = 50;

  for (auto frame = 0; frame < 4; frame++) {
    auto *tile = tile_find_mand(name + std::string(".idle.") + std::to_string(frame));
    tile_delay_ms_set(tile, delay);
    tp_tiles_push_back(tp, THING_ANIM_IDLE, tile);
    tile_size_set(tile, OUTLINE_TILE_WIDTH, OUTLINE_TILE_HEIGHT);

    if (frame == 3) {
      tile_is_cleanup_on_end_of_anim_set(tile);
    }
  }

  return true;
}
