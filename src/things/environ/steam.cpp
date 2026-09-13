//
// Copyright goblinhack@gmail.com
//

#include "../../my_callstack.hpp"
#include "../../my_thing.hpp"
#include "../../my_thing_callbacks.hpp"
#include "../../my_tile.hpp"
#include "../../my_tp.hpp"
#include "../../my_tps.hpp"
#include "../../my_types.hpp"

static auto tp_steam_description_get(Gamep g, Levelsp v, Levelp l, Thingp me) -> std::string
{
  TRACE();

  return "thick steam";
}

[[nodiscard]] auto tp_load_steam() -> bool
{
  TRACE();

  auto *tp   = tp_load("steam"); // keep as string for scripts
  auto  name = tp_name(tp);

  // begin sort marker2 {
  thing_description_set(tp, tp_steam_description_get);
  tp_damage_set(tp, THING_EVENT_FIRE_DAMAGE, "1d4");
  tp_flag_set(tp, is_able_to_be_teleported);
  tp_flag_set(tp, is_animated);
  tp_flag_set(tp, is_blit_centered);
  tp_flag_set(tp, is_described_cursor);
  tp_flag_set(tp, is_gaseous);
  tp_flag_set(tp, is_indestructible);
  tp_flag_set(tp, is_loggable);
  tp_flag_set(tp, is_physics_temperature);
  tp_flag_set(tp, is_removable_on_err);
  tp_flag_set(tp, is_steam);
  tp_flag_set(tp, is_submergible);
  tp_flag_set(tp, is_tick_end_delay);
  tp_flag_set(tp, is_tickable);
  tp_lifespan_set(tp, "1d6+3");
  tp_name_a_or_an_set(tp, "steam");
  tp_name_apostrophize_set(tp, "steams'");
  tp_name_long_set(tp, "steam");
  tp_name_pluralize_set(tp, "steam");
  tp_name_short_set(tp, "steam");
  tp_priority_set(tp, THING_PRIORITY_STEAM);
  tp_temperature_initial_set(tp, 100); // celsius
  tp_weight_set(tp, WEIGHT_NONE);      // grams
  tp_z_depth_set(tp, MAP_Z_DEPTH_GAS);
  // end sort marker1 }

  auto delay = 200;

  for (auto frame = 0; frame < 16; frame++) {
    auto *tile = tile_find_mand(name + std::string(".idle.") + std::to_string(frame));
    tile_delay_ms_set(tile, delay);
    tp_tiles_push_back(tp, THING_ANIM_IDLE, tile);
  }

  return true;
}
