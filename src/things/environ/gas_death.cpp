//
// Copyright goblinhack@gmail.com
//

#include "../../my_callstack.hpp"
#include "../../my_dice_rolls.hpp"
#include "../../my_globals.hpp"
#include "../../my_thing.hpp"
#include "../../my_thing_callbacks.hpp"
#include "../../my_thing_inlines.hpp"
#include "../../my_tile.hpp"
#include "../../my_tp.hpp"
#include "../../my_tps.hpp"
#include "../../my_types.hpp"

static auto tp_gas_death_description_get(Gamep g, Levelsp v, Levelp l, Thingp me) -> std::string
{
  TRACE();

  return "thick choking gas";
}

static void tp_gas_death_tick_begin(Gamep g, Levelsp v, Levelp l, Thingp me)
{
  TRACE();

  //
  // Don't spawn gas too soon after creation or we get a gas storm
  //
  if (thing_age(me) <= 1) {
    return;
  }

  const std::initializer_list< bpoint > points = {
      bpoint(-1, -1), bpoint(0, -1), bpoint(1, -1), bpoint(-1, 0), bpoint(1, 0), bpoint(-1, 1), bpoint(0, 1), bpoint(1, 1),
  };

  //
  // Spawn adjacent gas
  //
  for (auto delta : points) {
    auto at = thing_at(g, v, l, me);
    auto p  = at + delta;

    //
    // Rock, for example?
    //
    if (level_is_obs_to_gas_bool(g, v, l, p)) {
      continue;
    }

    //
    // Some other gas is here already, don't spawn more
    //
    if (level_is_gas_bool(g, v, l, p)) {
      continue;
    }

    if (d100() < 20 + (thing_age(me) * 10)) {
      //
      // The older the gas gets, the more chance of spreading
      //
      if (compiler_unused) {
        log("gas death spread check: ok");
      }
    } else {
      //
      // Too young to spread gas_death.
      //
      if (compiler_unused) {
        log("gas death spread check; too young");
      }
      continue;
    }

    THING_DBG(g, v, l, me, "spawn gas_death");

    auto n = thing_spawn(g, v, l, tp_first(is_gas_death), p);
    if (n) {
      float old_lifespan  = thing_lifespan(g, v, l, me);
      float new_lifespan  = old_lifespan * 0.9f;
      int   new_lifespani = (int) ceilf(new_lifespan);
      if (new_lifespani == 0) {
        new_lifespani = 1;
      }
      (void) thing_lifespan_set(g, v, l, n, new_lifespani);
    }
  }
}

[[nodiscard]] auto tp_load_gas_death() -> bool
{
  TRACE();

  auto *tp   = tp_load("gas_death"); // keep as string for scripts
  auto  name = tp_name(tp);

  // begin sort marker1 {
  thing_description_set(tp, tp_gas_death_description_get);
  thing_on_tick_begin_set(tp, tp_gas_death_tick_begin);
  tp_damage_set(tp, THING_EVENT_GAS_DAMAGE, "1d6");
  tp_distance_light_penetration_pixels_set(tp, TILE_WIDTH);
  tp_flag_set(tp, is_able_to_be_teleported);
  tp_flag_set(tp, is_animated);
  tp_flag_set(tp, is_blit_centered);
  tp_flag_set(tp, is_blit_shown_in_chasms);
  tp_flag_set(tp, is_blit_shown_in_overlay);
  tp_flag_set(tp, is_described_cursor);
  tp_flag_set(tp, is_gas_death);
  tp_flag_set(tp, is_gas);
  tp_flag_set(tp, is_gaseous);
  tp_flag_set(tp, is_indestructible);
  tp_flag_set(tp, is_obs_to_vision);
  tp_flag_set(tp, is_removable_on_err);
  tp_flag_set(tp, is_tickable);
  tp_name_a_or_an_set(tp, "deathly gas");
  tp_name_apostrophize_set(tp, "deathly gas'");
  tp_name_long_set(tp, "deathly gas");
  tp_name_pluralize_set(tp, "deathly gas");
  tp_name_short_set(tp, "deathly gas");
  tp_priority_set(tp, THING_PRIORITY_GAS);
  tp_weight_set(tp, WEIGHT_NONE); // grams
  tp_z_depth_set(tp, MAP_Z_DEPTH_GAS);
  // end sort marker1 }

  tp_lifespan_set(tp, "1d8+22");

  auto delay = 200;

  for (auto frame = 0; frame < 16; frame++) {
    auto *tile = tile_find_mand(name + std::string(".idle.") + std::to_string(frame));
    tile_delay_ms_set(tile, delay);
    tp_tiles_push_back(tp, THING_ANIM_IDLE, tile);
    tile_size_set(tile, OUTLINE_TILE_WIDTH, OUTLINE_TILE_HEIGHT);
  }

  return true;
}
