//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_globals.hpp"
#include "my_thing.hpp"
#include "my_thing_callbacks.hpp"
#include "my_tile.hpp"
#include "my_tp.hpp"
#include "my_tps.hpp"
#include "my_types.hpp"

static auto tp_smoke_description_get(Gamep g, Levelsp v, Levelp l, Thingp t) -> std::string
{
  TRACE();

  return "thick smoke";
}

auto tp_load_smoke() -> bool
{
  TRACE();

  auto *tp   = tp_load("smoke"); // keep as string for scripts
  auto  name = tp_name(tp);

  // begin sort marker1 {
  thing_description_set(tp, tp_smoke_description_get);
  tp_flag_set(tp, is_animated);
  tp_flag_set(tp, is_blit_centered);
  tp_flag_set(tp, is_blit_shown_in_chasms);
  tp_flag_set(tp, is_described_cursor);
  tp_flag_set(tp, is_gaseous);
  tp_flag_set(tp, is_indestructible);
  tp_flag_set(tp, is_obs_to_vision);
  tp_flag_set(tp, is_removable_on_err);
  tp_flag_set(tp, is_smoke);
  tp_flag_set(tp, is_submergible);
  tp_flag_set(tp, is_teleport_blocked);
  tp_flag_set(tp, is_tickable);
  tp_name_a_or_an_set(tp, "smoke");
  tp_name_apostrophize_set(tp, "smokes'");
  tp_name_long_set(tp, "smoke");
  tp_name_pluralize_set(tp, "smokes");
  tp_name_short_set(tp, "smoke");
  tp_priority_set(tp, THING_PRIORITY_SMOKE);
  tp_weight_set(tp, WEIGHT_NONE); // grams
  tp_z_depth_set(tp, MAP_Z_DEPTH_GAS);
  // end sort marker1 }

  if (g_opt_tests) {
    tp_lifespan_set(tp, "1");
  } else {
    tp_lifespan_set(tp, "1d6+3");
  }

  auto delay = 200;

  for (auto frame = 0; frame < 16; frame++) {
    auto *tile = tile_find_mand(name + std::string(".idle.") + std::to_string(frame));
    tile_delay_ms_set(tile, delay);
    tp_tiles_push_back(tp, THING_ANIM_IDLE, tile);
    tile_size_set(tile, OUTLINE_TILE_WIDTH, OUTLINE_TILE_HEIGHT);
  }

  return true;
}
