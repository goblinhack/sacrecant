//
// Copyright goblinhack@gmail.com
//

#include "../../my_callstack.hpp"
#include "../../my_thing_callbacks.hpp"
#include "../../my_tile.hpp"
#include "../../my_tp.hpp"
#include "../../my_tps.hpp"
#include "../../my_types.hpp"
#include "../../my_ui.hpp"

#include <string>

static auto tp_exit_closed_description_get(Gamep g, Levelsp v, Levelp l, Thingp me) -> std::string
{
  TRACE();

  return "boarded up stairs";
}

static auto tp_exit_closed_detail_get(Gamep g, Levelsp v, Levelp l, Thingp me) -> std::string
{
  TRACE();

  return UI_INFO1_FMT_STR "A boarded up exit. Must be some way to open it like a boss...";
}

[[nodiscard]] auto tp_load_exit_closed() -> bool
{
  TRACE();

  auto *tp   = tp_load("exit_closed"); // keep as string for scripts
  auto  name = tp_name(tp);
  // begin sort marker1 {
  thing_description_set(tp, tp_exit_closed_description_get);
  thing_detail_set(tp, tp_exit_closed_detail_get);
  tp_flag_set(tp, is_animated);
  tp_flag_set(tp, is_blit_centered);
  tp_flag_set(tp, is_blit_if_has_seen);
  tp_flag_set(tp, is_blit_shown_in_chasms);
  tp_flag_set(tp, is_critical_to_dungeon_design);
  tp_flag_set(tp, is_critical_to_gameplay);
  tp_flag_set(tp, is_described_cursor);
  tp_flag_set(tp, is_exit_closed);
  tp_flag_set(tp, is_indestructible);
  tp_flag_set(tp, is_loggable);
  tp_flag_set(tp, is_stone);
  tp_flag_set(tp, is_submergible);
  tp_name_a_or_an_set(tp, "an boarded up exit");
  tp_name_apostrophize_set(tp, "boarded up exits'");
  tp_name_long_set(tp, "boarded up exit");
  tp_name_pluralize_set(tp, "boarded up exits");
  tp_name_short_set(tp, "boarded up exit");
  tp_weight_set(tp, WEIGHT_VHEAVY); // grams
  tp_z_depth_set(tp, MAP_Z_DEPTH_BG_OBJ);
  // end sort marker1 }

  for (auto frame = 0; frame < 1; frame++) {
    const auto delay = 1000; /* ms */
    auto      *tile  = tile_find_mand("exit_closed." + std::to_string(frame));
    tile_delay_ms_set(tile, delay);
    tp_tiles_push_back(tp, THING_ANIM_IDLE, tile);
    tile_size_set(tile, OUTLINE_TILE_WIDTH, OUTLINE_TILE_HEIGHT);
  }

  return true;
}
