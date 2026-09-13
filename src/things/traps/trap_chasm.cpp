//
// Copyright goblinhack@gmail.com
//

#include "../../my_callstack.hpp"
#include "../../my_thing.hpp"
#include "../../my_thing_callbacks.hpp"
#include "../../my_thing_inlines.hpp"
#include "../../my_tile.hpp"
#include "../../my_tp.hpp"
#include "../../my_tps.hpp"
#include "../../my_types.hpp"
#include "../../my_ui.hpp"

static auto tp_trap_chasm_description_get(Gamep g, Levelsp v, Levelp l, Thingp me) -> std::string
{
  TRACE();

  if (thing_is_open(me)) {
    return "sprung trap";
  }

  return "odd looking floor tile";
}

static auto tp_trap_chasm_activated(Gamep g, Levelsp v, Levelp l, Thingp trap, Thingp user) -> bool
{
  TRACE();

  if (! level_is_chasm_bool(g, v, l, thing_at(g, v, l, trap))) {
    if (thing_spawn(g, v, l, tp_first(is_chasm), trap)) {
      auto player_at = thing_at(g, v, l, user);
      if (thing_on_same_level_as_player(g, v, trap)) {
        if (thing_at(g, v, l, trap) == player_at) {
          topcon(UI_IMPORTANT_FMT_STR "The ground opens up beneath you!" UI_RESET_FMT);
        } else if (thing_vision_can_see_tile(g, v, l, user, player_at)) {
          topcon(UI_WARN_FMT_STR "The ground collapses!" UI_RESET_FMT);
        } else {
          topcon("You hear the ground collapse!");
        }
      } else {
        topcon("You hear a very distant landslide!");
      }
    }
  }

  ThingEvent e {
      .reason     = "by activating",  //
      .event_type = THING_EVENT_OPEN, //
  };

  THING_DBG(g, v, l, trap, "dead due to activation");
  TRACE_INDENT();

  thing_dead(g, v, l, trap, e);

  return true;
}

[[nodiscard]] auto tp_load_trap() -> bool
{
  TRACE();

  auto *tp   = tp_load("trap_chasm"); // keep as string for scripts
  auto  name = tp_name(tp);

  // begin sort marker1 {
  thing_description_set(tp, tp_trap_chasm_description_get);
  thing_on_activated_set(tp, tp_trap_chasm_activated);
  tp_flag_set(tp, is_blit_centered);
  tp_flag_set(tp, is_blit_per_pixel_lighting);
  tp_flag_set(tp, is_blit_shown_in_chasms);
  tp_flag_set(tp, is_described_cursor);
  tp_flag_set(tp, is_flat);
  tp_flag_set(tp, is_loggable);
  tp_flag_set(tp, is_obs_to_falling_onto);
  tp_flag_set(tp, is_trap);
  tp_is_immune_to_add(tp, THING_EVENT_WATER_DAMAGE);
  tp_name_a_or_an_set(tp, "a trap");
  tp_name_apostrophize_set(tp, "traps'");
  tp_name_long_set(tp, "trap");
  tp_name_pluralize_set(tp, "traps");
  tp_name_short_set(tp, "trap");
  tp_weight_set(tp, WEIGHT_FEATHER); // grams
  tp_z_depth_set(tp, MAP_Z_DEPTH_OBJ);
  // end sort marker1 }

  for (auto frame = 0; frame < 1; frame++) {
    auto *tile = tile_find_mand(std::string("trap.") + std::to_string(frame));
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tp_tiles_push_back(tp, THING_ANIM_IDLE, tile);
  }

  return true;
}
