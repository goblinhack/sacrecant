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
#include "../../my_ui.hpp"

static auto tp_gas_explosive_description_get(Gamep g, Levelsp v, Levelp l, Thingp me) -> std::string
{
  TRACE();

  return "foul smelling volatile gas";
}

static void tp_gas_explosive_tick_begin(Gamep g, Levelsp v, Levelp l, Thingp me)
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

      if (is_oob(p)) {
        continue;
      }

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
          log("gas explosive spread check: ok");
        }
      } else {
        //
        // Too young to spread gas_explosive.
        //
        if (compiler_unused) {
          log("gas explosive spread check; too young");
        }
        continue;
      }

      THING_DBG(g, v, l, me, "spawn gas_explosive");

      auto n = thing_spawn(g, v, l, tp_first(is_gas_explosive), p);
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
}

static bool tp_gas_explosive_explode(Gamep g, Levelsp v, Levelp l, Thingp me, ThingEvent &e)
{
  TRACE();

  auto at = thing_at(g, v, l, me);

  const std::initializer_list< bpoint > points = {
      bpoint(-1, -1), bpoint(1, -1), bpoint(0, -1), bpoint(-1, 0), bpoint(1, 0), bpoint(0, 0), bpoint(-1, 1), bpoint(1, 1), bpoint(0, 1),
  };

  for (auto delta : points) {
    auto p = at + delta;

    if (is_oob(p)) {
      continue;
    }

    if (level_is_critical_to_dungeon_design_bool(g, v, l, p)) {
      continue;
    }

    if (level_is_obs_to_explosion_bool(g, v, l, p)) {
      continue;
    }

    if (level_is_explosion_bool(g, v, l, p)) {
      continue;
    }

    THING_DBG(g, v, l, me, "spawn explosion at %d,%d", p.x, p.y);

    (void) thing_spawn(g, v, l, tp_first(is_explosion), p);
  }

  return true;
}

static bool tp_gas_explosive_on_damage(Gamep g, Levelsp v, Levelp l, Thingp me, ThingEvent &e)
{
  TRACE();

  THING_DBG(g, v, l, me, "damaged, explode");
  return tp_gas_explosive_explode(g, v, l, me, e);
}

[[nodiscard]] auto tp_load_gas_explosive() -> bool
{
  TRACE();

  auto *tp   = tp_load("gas_explosive"); // keep as string for scripts
  auto  name = tp_name(tp);

  // begin sort marker1 {
  thing_description_set(tp, tp_gas_explosive_description_get);
  thing_on_damage_set(tp, tp_gas_explosive_on_damage);
  thing_on_tick_begin_set(tp, tp_gas_explosive_tick_begin);
  tp_chance_set(tp, THING_CHANCE_CONTINUE_TO_BURN, "1d2"); // fumble => intensify / keep burning / crit => stop burning
  tp_chance_set(tp, THING_CHANCE_START_BURNING, "1d2");    // fumble => flames spread to you
  tp_flag_set(tp, is_able_to_be_teleported);
  tp_flag_set(tp, is_animated);
  tp_flag_set(tp, is_blit_centered);
  tp_flag_set(tp, is_blit_shown_in_chasms);
  tp_flag_set(tp, is_blit_shown_in_overlay);
  tp_flag_set(tp, is_collision_circle_large);
  tp_flag_set(tp, is_combustible); // will continue to burn once on fire
  tp_flag_set(tp, is_described_cursor);
  tp_flag_set(tp, is_flammable);
  tp_flag_set(tp, is_gas_explosive);
  tp_flag_set(tp, is_gas);
  tp_flag_set(tp, is_gaseous);
  tp_flag_set(tp, is_loggable);
  tp_flag_set(tp, is_physics_explosion);
  tp_flag_set(tp, is_physics_temperature);
  tp_flag_set(tp, is_removable_on_err);
  tp_flag_set(tp, is_tickable);
  tp_flag_set(tp, is_tiled);
  tp_lifespan_set(tp, "1d8+32");
  tp_name_a_or_an_set(tp, "explosive gas");
  tp_name_apostrophize_set(tp, "explosive gas'");
  tp_name_long_set(tp, "explosive gas");
  tp_name_pluralize_set(tp, "explosive gas");
  tp_name_short_set(tp, "explosive gas");
  tp_priority_set(tp, THING_PRIORITY_GAS);
  tp_temperature_burns_at_set(tp, 21);  // celsius
  tp_temperature_damage_at_set(tp, 21); // celsius
  tp_temperature_initial_set(tp, 20);   // celsius
  tp_weight_set(tp, WEIGHT_NONE);       // grams
  tp_z_depth_set(tp, MAP_Z_DEPTH_GAS);
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
