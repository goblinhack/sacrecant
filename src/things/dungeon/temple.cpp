//
// Copyright goblinhack@gmail.com
//

#include "../../my_callstack.hpp"
#include "../../my_level.hpp"
#include "../../my_level_inlines.hpp"
#include "../../my_main.hpp"
#include "../../my_thing_callbacks.hpp"
#include "../../my_tile.hpp"
#include "../../my_tp.hpp"
#include "../../my_tps.hpp"
#include "../../my_types.hpp"
#include "../../my_ui.hpp"

static auto tp_temple_description_get(Gamep g, Levelsp v, Levelp l, Thingp me) -> std::string
{
  TRACE();

  return "creepy looking temple";
}

static auto tp_temple_detail_get(Gamep g, Levelsp v, Levelp l, Thingp me) -> std::string
{
  TRACE();

  return                                                                                                    //
      UI_INFO1_FMT_STR "This foul looking place is a sacrificial temple. Dried blood rests on its steps.\n" //
      UI_INFO2_FMT_STR "To enter you must be willing to sacrifice something.\n"                             //
      UI_INFO3_FMT_STR "Specifically, you can enter easily. Leaving is more difficult.\n";
}

[[nodiscard]] auto tp_load_temple() -> bool
{
  TRACE();

  auto *tp   = tp_load("temple"); // keep as string for scripts
  auto  name = tp_name(tp);

  // begin sort marker1 {
  thing_description_set(tp, tp_temple_description_get);
  thing_detail_set(tp, tp_temple_detail_get);
  tp_distance_light_penetration_pixels_set(tp, TILE_WIDTH / 2);
  tp_flag_set(tp, is_animated);
  tp_flag_set(tp, is_blit_if_has_seen);
  tp_flag_set(tp, is_blit_on_ground);
  tp_flag_set(tp, is_blit_shown_in_chasms);
  tp_flag_set(tp, is_collision_circle_large);
  tp_flag_set(tp, is_critical_to_dungeon_design);
  tp_flag_set(tp, is_critical_to_gameplay);
  tp_flag_set(tp, is_described_cursor);
  tp_flag_set(tp, is_loggable);
  tp_flag_set(tp, is_obs_to_spawning);
  tp_flag_set(tp, is_obs_to_teleporting_onto);
  tp_flag_set(tp, is_obs_to_vision);
  tp_flag_set(tp, is_obs_to_wall_walker);
  tp_flag_set(tp, is_stone);
  tp_flag_set(tp, is_submergible); // is seen submerged when in water
  tp_flag_set(tp, is_temple);
  tp_flag_set(tp, is_light_flicker);
  tp_flag_set(tp, is_light_source, 4);
  tp_light_color_set(tp, "orange");
  tp_flag_set(tp, is_tickable);
  tp_name_a_or_an_set(tp, "a temple");
  tp_name_apostrophize_set(tp, "temples'");
  tp_name_long_set(tp, "temple");
  tp_name_pluralize_set(tp, "temples");
  tp_name_short_set(tp, "temple");
  tp_weight_set(tp, WEIGHT_VHEAVY); // grams
  tp_priority_set(tp, THING_PRIORITY_OBJECT);
  tp_z_depth_set(tp, MAP_Z_DEPTH_BG_OBJ);
  // end sort marker1 }

  auto delay = 200;

  for (auto frame = 0; frame < 2; frame++) {
    auto *tile = tile_find_mand(name + std::string(".idle.") + std::to_string(frame));
    tile_delay_ms_set(tile, delay);
    tp_tiles_push_back(tp, THING_ANIM_IDLE, tile);
    tile_size_set(tile, 32, 32);
  }

  return true;
}
