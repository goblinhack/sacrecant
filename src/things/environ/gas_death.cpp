//
// Copyright goblinhack@gmail.com
//

#include "../../my_callstack.hpp"
#include "../../my_dice_rolls.hpp"
#include "../../my_globals.hpp"
#include "../../my_level_inlines.hpp"
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

  auto at = thing_at(g, v, l, me);

  //
  // Don't spawn gas too soon after creation or we get a gas storm
  //
  if (thing_age(me) > 1) {
    const std::initializer_list< bpoint > points = {
        bpoint(-1, -1), bpoint(0, -1), bpoint(1, -1), bpoint(-1, 0), bpoint(1, 0), bpoint(-1, 1), bpoint(0, 1), bpoint(1, 1),
    };

    //
    // Spawn adjacent gas
    //
    for (auto delta : points) {
      auto p = at + delta;

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
        if (g_opt_tests) {
          new_lifespani = 1;
        }
        (void) thing_lifespan_set(g, v, l, n, new_lifespani);
      }
    }
  }

  //
  // Try to attack
  //
  FOR_ALL_THINGS_AT_UNSAFE(g, v, l, it, at)
  {
    if (it == me) {
      continue;
    }

    if (! thing_is_able_to_breathe(it)) {
      continue;
    }

    if (thing_is_dead(it) || thing_is_corpse(it)) {
      continue;
    }

    if (thing_is_undead(it)) {
      continue;
    }

    if (thing_is_ethereal(g, v, l, it)) {
      continue;
    }

    THING_DBG(g, v, l, it, "gas attack monst");
    TRACE_INDENT();

    auto *source     = me;
    auto  event_type = THING_EVENT_GAS_DAMAGE;
    auto  damage     = thing_damage_calculate(g, v, l, source, event_type);

    if (! damage) {
      continue;
    }

    ThingEvent e {
        .reason     = "by gas damage", //
        .event_type = event_type,      //
        .damage     = damage,          //
        .source     = source,          //
    };

    THING_DBG(g, v, l, it, "apply gas damage");
    TRACE_INDENT();

    thing_damage_apply(g, v, l, it, e);
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
  tp_flag_set(tp, is_tiled);
  tp_name_a_or_an_set(tp, "deathly gas");
  tp_name_apostrophize_set(tp, "deathly gas'");
  tp_name_long_set(tp, "deathly gas");
  tp_name_pluralize_set(tp, "deathly gas");
  tp_name_short_set(tp, "deathly gas");
  tp_priority_set(tp, THING_PRIORITY_GAS);
  tp_weight_set(tp, WEIGHT_NONE); // grams
  tp_z_depth_set(tp, MAP_Z_DEPTH_GAS);
  tp_lifespan_set(tp, "1d8+22");
  // end sort marker1 }

  auto delay = 200;

  for (auto frame = 0; frame < 8; frame++) {
    auto  frame_string = std::to_string(frame);
    auto *tile         = tile_find_mand(name + ".IS_JOIN_BL." + frame_string);
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tile_delay_ms_set(tile, delay);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_BL, tile);
    tile = tile_find_mand(name + ".IS_JOIN_BL2." + frame_string);
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tile_delay_ms_set(tile, delay);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_BL2, tile);
    tile = tile_find_mand(name + ".IS_JOIN_BLOCK." + frame_string);
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tile_delay_ms_set(tile, delay);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_BLOCK, tile);
    tile = tile_find_mand(name + ".IS_JOIN_BR." + frame_string);
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tile_delay_ms_set(tile, delay);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_BR, tile);
    tile = tile_find_mand(name + ".IS_JOIN_BR2." + frame_string);
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tile_delay_ms_set(tile, delay);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_BR2, tile);
    tile = tile_find_mand(name + ".IS_JOIN_HORIZ." + frame_string);
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tile_delay_ms_set(tile, delay);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_HORIZ, tile);
    tile = tile_find_mand(name + ".IS_JOIN_LEFT." + frame_string);
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tile_delay_ms_set(tile, delay);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_LEFT, tile);
    tile = tile_find_mand(name + ".IS_JOIN_NODE." + frame_string);
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tile_delay_ms_set(tile, delay);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_NODE, tile);
    tile = tile_find_mand(name + ".IS_JOIN_RIGHT." + frame_string);
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tile_delay_ms_set(tile, delay);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_RIGHT, tile);
    tile = tile_find_mand(name + ".IS_JOIN_T_1." + frame_string);
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tile_delay_ms_set(tile, delay);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_T_1, tile);
    tile = tile_find_mand(name + ".IS_JOIN_T_2." + frame_string);
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tile_delay_ms_set(tile, delay);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_T_2, tile);
    tile = tile_find_mand(name + ".IS_JOIN_T_3." + frame_string);
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tile_delay_ms_set(tile, delay);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_T_3, tile);
    tile = tile_find_mand(name + ".IS_JOIN_T." + frame_string);
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tile_delay_ms_set(tile, delay);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_T, tile);
    tile = tile_find_mand(name + ".IS_JOIN_T180_1." + frame_string);
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tile_delay_ms_set(tile, delay);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_T180_1, tile);
    tile = tile_find_mand(name + ".IS_JOIN_T180_2." + frame_string);
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tile_delay_ms_set(tile, delay);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_T180_2, tile);
    tile = tile_find_mand(name + ".IS_JOIN_T180_3." + frame_string);
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tile_delay_ms_set(tile, delay);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_T180_3, tile);
    tile = tile_find_mand(name + ".IS_JOIN_T180." + frame_string);
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tile_delay_ms_set(tile, delay);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_T180, tile);
    tile = tile_find_mand(name + ".IS_JOIN_T270_1." + frame_string);
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tile_delay_ms_set(tile, delay);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_T270_1, tile);
    tile = tile_find_mand(name + ".IS_JOIN_T270_2." + frame_string);
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tile_delay_ms_set(tile, delay);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_T270_2, tile);
    tile = tile_find_mand(name + ".IS_JOIN_T270_3." + frame_string);
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tile_delay_ms_set(tile, delay);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_T270_3, tile);
    tile = tile_find_mand(name + ".IS_JOIN_T270." + frame_string);
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tile_delay_ms_set(tile, delay);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_T270, tile);
    tile = tile_find_mand(name + ".IS_JOIN_T90_1." + frame_string);
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tile_delay_ms_set(tile, delay);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_T90_1, tile);
    tile = tile_find_mand(name + ".IS_JOIN_T90_2." + frame_string);
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tile_delay_ms_set(tile, delay);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_T90_2, tile);
    tile = tile_find_mand(name + ".IS_JOIN_T90_3." + frame_string);
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tile_delay_ms_set(tile, delay);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_T90_3, tile);
    tile = tile_find_mand(name + ".IS_JOIN_T90." + frame_string);
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tile_delay_ms_set(tile, delay);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_T90, tile);
    tile = tile_find_mand(name + ".IS_JOIN_TL." + frame_string);
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tile_delay_ms_set(tile, delay);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_TL, tile);
    tile = tile_find_mand(name + ".IS_JOIN_TL2." + frame_string);
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tile_delay_ms_set(tile, delay);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_TL2, tile);
    tile = tile_find_mand(name + ".IS_JOIN_TOP." + frame_string);
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tile_delay_ms_set(tile, delay);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_TOP, tile);
    tile = tile_find_mand(name + ".IS_JOIN_BOT." + frame_string);
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tile_delay_ms_set(tile, delay);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_BOT, tile);
    tile = tile_find_mand(name + ".IS_JOIN_TR." + frame_string);
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tile_delay_ms_set(tile, delay);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_TR, tile);
    tile = tile_find_mand(name + ".IS_JOIN_TR2." + frame_string);
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tile_delay_ms_set(tile, delay);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_TR2, tile);
    tile = tile_find_mand(name + ".IS_JOIN_VERT." + frame_string);
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tile_delay_ms_set(tile, delay);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_VERT, tile);
    tile = tile_find_mand(name + ".IS_JOIN_X." + frame_string);
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tile_delay_ms_set(tile, delay);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_X, tile);
    tile = tile_find_mand(name + ".IS_JOIN_X1_180." + frame_string);
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tile_delay_ms_set(tile, delay);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_X1_180, tile);
    tile = tile_find_mand(name + ".IS_JOIN_X1_270." + frame_string);
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tile_delay_ms_set(tile, delay);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_X1_270, tile);
    tile = tile_find_mand(name + ".IS_JOIN_X1_90." + frame_string);
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tile_delay_ms_set(tile, delay);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_X1_90, tile);
    tile = tile_find_mand(name + ".IS_JOIN_X1." + frame_string);
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tile_delay_ms_set(tile, delay);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_X1, tile);
    tile = tile_find_mand(name + ".IS_JOIN_X2_180." + frame_string);
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tile_delay_ms_set(tile, delay);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_X2_180, tile);
    tile = tile_find_mand(name + ".IS_JOIN_X2_270." + frame_string);
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tile_delay_ms_set(tile, delay);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_X2_270, tile);
    tile = tile_find_mand(name + ".IS_JOIN_X2_90." + frame_string);
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tile_delay_ms_set(tile, delay);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_X2_90, tile);
    tile = tile_find_mand(name + ".IS_JOIN_X2." + frame_string);
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tile_delay_ms_set(tile, delay);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_X2, tile);
    tile = tile_find_mand(name + ".IS_JOIN_X3_180." + frame_string);
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tile_delay_ms_set(tile, delay);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_X3_180, tile);
    tile = tile_find_mand(name + ".IS_JOIN_X3." + frame_string);
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tile_delay_ms_set(tile, delay);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_X3, tile);
    tile = tile_find_mand(name + ".IS_JOIN_X4_180." + frame_string);
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tile_delay_ms_set(tile, delay);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_X4_180, tile);
    tile = tile_find_mand(name + ".IS_JOIN_X4_270." + frame_string);
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tile_delay_ms_set(tile, delay);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_X4_270, tile);
    tile = tile_find_mand(name + ".IS_JOIN_X4_90." + frame_string);
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tile_delay_ms_set(tile, delay);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_X4_90, tile);
    tile = tile_find_mand(name + ".IS_JOIN_X4." + frame_string);
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tile_delay_ms_set(tile, delay);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_X4, tile);
  }

  return true;
}
