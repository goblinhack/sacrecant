//
// Copyright goblinhack@gmail.com
//

#include <algorithm>

#include "my_callstack.hpp"
#include "my_level.hpp"
#include "my_main.hpp"
#include "my_thing_callbacks.hpp"
#include "my_thing_inlines.hpp"
#include "my_tile.hpp"
#include "my_tp.hpp"
#include "my_tps.hpp"
#include "my_types.hpp"
#include "my_ui.hpp"

static auto tp_bridge_description_get(Gamep g, Levelsp v, Levelp l, Thingp me) -> std::string
{
  TRACE();

  return "rickety bridge";
}

static void thing_bridge_replace(Gamep g, Levelsp v, Levelp l, Thingp me)
{
  TRACE();

  //
  // Replace the bridge with the most populous surrounding hazard
  //
  {
    const std::initializer_list< bpoint > points = {
        bpoint(-1, -1), bpoint(1, -1), bpoint(0, -1), bpoint(-1, 0), bpoint(1, 0), bpoint(0, 0), bpoint(-1, 1), bpoint(1, 1), bpoint(0, 1),
    };

    auto lava_count  = 0;
    auto water_count = 0;
    auto chasm_count = 0;

    for (auto delta : points) {
      auto at = thing_at(g, v, l, me);
      auto p  = at + delta;
      lava_count += (level_is_lava_bool(g, v, l, p)) ? 1 : 0;
      water_count += (level_is_water_shallow_bool(g, v, l, p) || level_is_water_deep_bool(g, v, l, p)) ? 1 : 0;
      chasm_count += (level_is_chasm_bool(g, v, l, p)) ? 1 : 0;
    }

    auto max_count = std::max({lava_count, water_count, chasm_count});
    if (max_count != 0) {
      if (max_count == chasm_count) {
        if (! level_is_chasm_bool(g, v, l, thing_at(g, v, l, me))) {
          (void) thing_spawn(g, v, l, tp_first(is_chasm), me);
        }
      } else if (max_count == water_count) {
        //
        // Water needs dirt under it so we can see the transparency
        //
        if (! level_is_water_shallow_bool(g, v, l, thing_at(g, v, l, me))) {
          (void) thing_spawn(g, v, l, tp_first(is_water_shallow), me);
        }
        if (! level_is_dirt_bool(g, v, l, thing_at(g, v, l, me))) {
          (void) thing_spawn(g, v, l, tp_first(is_dirt), me);
        }
      } else if (max_count == lava_count) {
        if (! level_is_lava_bool(g, v, l, thing_at(g, v, l, me))) {
          (void) thing_spawn(g, v, l, tp_first(is_lava), me);
        }
        if (! level_is_dirt_bool(g, v, l, thing_at(g, v, l, me))) {
          (void) thing_spawn(g, v, l, tp_first(is_dirt), me);
        }
      }
    } else {
      //
      // If nothing we can gather from the surrounds, default to chasm
      //
      if (! level_is_chasm_bool(g, v, l, thing_at(g, v, l, me))) {
        (void) thing_spawn(g, v, l, tp_first(is_chasm), me);
      }
    }
  }
}

static void tp_bridge_group_destroy(Gamep g, Levelsp v, Levelp l, Thingp me)
{
  TRACE();

  if (! me->group_id) {
    return;
  }

  std::vector< Thingp > group;
  auto                  group_id = me->group_id;

  FOR_ALL_GROUP_THINGS_ON_LEVEL(g, v, l, o, group_id)
  {
    group.push_back(o);
    thing_group_member_leave(g, v, l, o);
  }

  for (auto o : group) {
    thing_bridge_replace(g, v, l, o);
    thing_fall(g, v, l, o);
  }
}

static void tp_bridge_on_death(Gamep g, Levelsp v, Levelp l, Thingp me, ThingEvent &e)
{
  TRACE();

  if (me->group_id) {
    auto *player = thing_player(g);
    if (player != nullptr) {
      auto player_at = thing_at(g, v, l, player);
      if (thing_on_same_level_as_player(g, v, me)) {
        if (thing_at(g, v, l, me) == player_at) {
          topcon(UI_IMPORTANT_FMT_STR "The bridge collapses under you!" UI_RESET_FMT);
        } else if (thing_vision_can_see_tile(g, v, l, player, player_at)) {
          topcon(UI_WARN_FMT_STR "The bridge collapses!" UI_RESET_FMT);
        } else {
          topcon("You hear a bridge collapse!");
        }
      } else {
        topcon("You hear a very distant bridge collapse!");
      }
    }
  }

  tp_bridge_group_destroy(g, v, l, me);
}

static void tp_bridge_on_fall_begin(Gamep g, Levelsp v, Levelp l, Thingp me)
{
  TRACE();

  tp_bridge_group_destroy(g, v, l, me);
}

static void tp_bridge_on_fall_end(Gamep g, Levelsp v, Levelp l, Thingp me)
{
  TRACE();

  //
  // If we fell into another chasm, don't kill the thing yet
  //
  if (level_is_chasm_bool(g, v, l, thing_at(g, v, l, me))) {
    return;
  }

  ThingEvent e {
      .reason     = "by bridge break", //
      .event_type = THING_EVENT_FALL,  //
  };

  THING_DBG(g, v, l, me, "dead due to bridge breaking");
  TRACE_INDENT();

  thing_dead(g, v, l, me, e);
}

[[nodiscard]] auto tp_load_bridge() -> bool
{
  TRACE();

  auto *tp   = tp_load("bridge"); // keep as string for scripts
  auto  name = tp_name(tp);
  // begin sort marker1 {
  thing_description_set(tp, tp_bridge_description_get);
  thing_on_death_set(tp, tp_bridge_on_death);
  thing_on_fall_begin_set(tp, tp_bridge_on_fall_begin);
  thing_on_fall_end_set(tp, tp_bridge_on_fall_end);
  tp_chance_set(tp, THING_CHANCE_CONTINUE_TO_BURN, "1"); // fumble => intensify / keep burning / crit => stop burning
  tp_chance_set(tp, THING_CHANCE_START_BURNING, "1");    // fumble => flames spread to you
  tp_flag_set(tp, is_able_to_fall_sound);
  tp_flag_set(tp, is_able_to_fall);
  tp_flag_set(tp, is_blit_centered);
  tp_flag_set(tp, is_blit_if_has_seen);
  tp_flag_set(tp, is_blit_per_pixel_lighting);
  tp_flag_set(tp, is_blit_shown_in_chasms);
  tp_flag_set(tp, is_bridge);
  tp_flag_set(tp, is_burnable);    // is capable of being burned by fire
  tp_flag_set(tp, is_combustible); // will continue to burn once on fire
  tp_flag_set(tp, is_described_cursor);
  tp_flag_set(tp, is_grouped_thing);
  tp_flag_set(tp, is_loggable);
  tp_flag_set(tp, is_physics_explosion);
  tp_flag_set(tp, is_physics_temperature);
  tp_flag_set(tp, is_tickable);
  tp_flag_set(tp, is_tiled);
  tp_flag_set(tp, is_wood);
  tp_health_set(tp, "1d12");
  tp_is_immune_to_add(tp, THING_EVENT_WATER_DAMAGE);
  tp_name_a_or_an_set(tp, "a bridge");
  tp_name_apostrophize_set(tp, "bridges'");
  tp_name_long_set(tp, "bridge");
  tp_name_pluralize_set(tp, "bridges");
  tp_name_short_set(tp, "bridge");
  tp_priority_set(tp, THING_PRIORITY_OBJECT);
  tp_temperature_burns_at_set(tp, 100);  // celsius
  tp_temperature_damage_at_set(tp, 100); // celsius
  tp_temperature_initial_set(tp, 20);    // celsius
  tp_weight_set(tp, WEIGHT_MEDIUM);      // grams
  tp_z_depth_set(tp, MAP_Z_DEPTH_LIQUID);
  // end sort marker1 }

  auto *tile = tile_find_mand(name + ".IS_JOIN_BL");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_JOIN_BL, tile);
  tile = tile_find_mand(name + ".IS_JOIN_BL2");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_JOIN_BL2, tile);
  tile = tile_find_mand(name + ".IS_JOIN_BLOCK");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_JOIN_BLOCK, tile);
  tile = tile_find_mand(name + ".IS_JOIN_BR");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_JOIN_BR, tile);
  tile = tile_find_mand(name + ".IS_JOIN_BR2");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_JOIN_BR2, tile);
  tile = tile_find_mand(name + ".IS_JOIN_HORIZ");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_JOIN_HORIZ, tile);
  tile = tile_find_mand(name + ".IS_JOIN_LEFT");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_JOIN_LEFT, tile);
  tile = tile_find_mand(name + ".IS_JOIN_NODE");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_JOIN_NODE, tile);
  tile = tile_find_mand(name + ".IS_JOIN_RIGHT");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_JOIN_RIGHT, tile);
  tile = tile_find_mand(name + ".IS_JOIN_T_1");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_JOIN_T_1, tile);
  tile = tile_find_mand(name + ".IS_JOIN_T_2");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_JOIN_T_2, tile);
  tile = tile_find_mand(name + ".IS_JOIN_T_3");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_JOIN_T_3, tile);
  tile = tile_find_mand(name + ".IS_JOIN_T");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_JOIN_T, tile);
  tile = tile_find_mand(name + ".IS_JOIN_T180_1");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_JOIN_T180_1, tile);
  tile = tile_find_mand(name + ".IS_JOIN_T180_2");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_JOIN_T180_2, tile);
  tile = tile_find_mand(name + ".IS_JOIN_T180_3");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_JOIN_T180_3, tile);
  tile = tile_find_mand(name + ".IS_JOIN_T180");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_JOIN_T180, tile);
  tile = tile_find_mand(name + ".IS_JOIN_T270_1");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_JOIN_T270_1, tile);
  tile = tile_find_mand(name + ".IS_JOIN_T270_2");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_JOIN_T270_2, tile);
  tile = tile_find_mand(name + ".IS_JOIN_T270_3");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_JOIN_T270_3, tile);
  tile = tile_find_mand(name + ".IS_JOIN_T270");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_JOIN_T270, tile);
  tile = tile_find_mand(name + ".IS_JOIN_T90_1");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_JOIN_T90_1, tile);
  tile = tile_find_mand(name + ".IS_JOIN_T90_2");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_JOIN_T90_2, tile);
  tile = tile_find_mand(name + ".IS_JOIN_T90_3");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_JOIN_T90_3, tile);
  tile = tile_find_mand(name + ".IS_JOIN_T90");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_JOIN_T90, tile);
  tile = tile_find_mand(name + ".IS_JOIN_TL");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_JOIN_TL, tile);
  tile = tile_find_mand(name + ".IS_JOIN_TL2");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_JOIN_TL2, tile);
  tile = tile_find_mand(name + ".IS_JOIN_TOP");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_JOIN_TOP, tile);
  tile = tile_find_mand(name + ".IS_JOIN_BOT");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_JOIN_BOT, tile);
  tile = tile_find_mand(name + ".IS_JOIN_TR");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_JOIN_TR, tile);
  tile = tile_find_mand(name + ".IS_JOIN_TR2");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_JOIN_TR2, tile);
  tile = tile_find_mand(name + ".IS_JOIN_VERT");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_JOIN_VERT, tile);
  tile = tile_find_mand(name + ".IS_JOIN_X");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_JOIN_X, tile);
  tile = tile_find_mand(name + ".IS_JOIN_X1_180");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_JOIN_X1_180, tile);
  tile = tile_find_mand(name + ".IS_JOIN_X1_270");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_JOIN_X1_270, tile);
  tile = tile_find_mand(name + ".IS_JOIN_X1_90");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_JOIN_X1_90, tile);
  tile = tile_find_mand(name + ".IS_JOIN_X1");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_JOIN_X1, tile);
  tile = tile_find_mand(name + ".IS_JOIN_X2_180");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_JOIN_X2_180, tile);
  tile = tile_find_mand(name + ".IS_JOIN_X2_270");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_JOIN_X2_270, tile);
  tile = tile_find_mand(name + ".IS_JOIN_X2_90");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_JOIN_X2_90, tile);
  tile = tile_find_mand(name + ".IS_JOIN_X2");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_JOIN_X2, tile);
  tile = tile_find_mand(name + ".IS_JOIN_X3_180");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_JOIN_X3_180, tile);
  tile = tile_find_mand(name + ".IS_JOIN_X3");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_JOIN_X3, tile);
  tile = tile_find_mand(name + ".IS_JOIN_X4_180");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_JOIN_X4_180, tile);
  tile = tile_find_mand(name + ".IS_JOIN_X4_270");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_JOIN_X4_270, tile);
  tile = tile_find_mand(name + ".IS_JOIN_X4_90");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_JOIN_X4_90, tile);
  tile = tile_find_mand(name + ".IS_JOIN_X4");
  tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_JOIN_X4, tile);

  return true;
}
