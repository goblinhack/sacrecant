//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_thing_callbacks.hpp"
#include "my_tile.hpp"
#include "my_tp.hpp"
#include "my_tps.hpp"
#include "my_types.hpp"
#include "my_ui.hpp"

#include <string>

static auto tp_exit_description_get(Gamep g, Levelsp v, Levelp l, Thingp t) -> std::string
{
  TRACE();

  return "slime covered stairs leading down";
}

static auto tp_exit_detail_get(Gamep g, Levelsp v, Levelp l, Thingp t) -> std::string
{
  TRACE();

  return UI_INFO1_FMT_STR "Exit here to exit the level.";
}

auto tp_load_exit() -> bool
{
  TRACE();

  auto *tp   = tp_load("exit"); // keep as string for scripts
  auto  name = tp_name(tp);
  // begin sort marker1 {
  thing_description_set(tp, tp_exit_description_get);
  thing_detail_set(tp, tp_exit_detail_get);
  tp_flag_set(tp, is_animated);
  tp_flag_set(tp, is_blit_centered);
  tp_flag_set(tp, is_blit_if_has_seen);
  tp_flag_set(tp, is_blit_shown_in_chasms);
  tp_flag_set(tp, is_blit_shown_in_overlay);
  tp_flag_set(tp, is_described_cursor);
  tp_flag_set(tp, is_dmap);
  tp_flag_set(tp, is_exit);
  tp_flag_set(tp, is_indestructible);
  tp_flag_set(tp, is_loggable);
  tp_flag_set(tp, is_stone);
  tp_flag_set(tp, is_submergible);
  tp_flag_set(tp, is_teleport_blocked);
  tp_name_a_or_an_set(tp, "an exit");
  tp_name_apostrophize_set(tp, "exits'");
  tp_name_long_set(tp, "exit");
  tp_name_pluralize_set(tp, "exits");
  tp_name_short_set(tp, "exit");
  tp_weight_set(tp, WEIGHT_VHEAVY); // grams
  tp_z_depth_set(tp, MAP_Z_DEPTH_OBJ);
  // end sort marker1 }

  for (auto frame = 0; frame < 2; frame++) {
    const auto delay = 1000; /* ms */
    auto      *tile  = tile_find_mand("exit." + std::to_string(frame));
    tile_delay_ms_set(tile, delay);
    tp_tiles_push_back(tp, THING_ANIM_IDLE, tile);
    tile_size_set(tile, OUTLINE_TILE_WIDTH, OUTLINE_TILE_HEIGHT);
  }

  return true;
}
