//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_level.hpp"
#include "my_thing_callbacks.hpp"
#include "my_tile.hpp"
#include "my_tp.hpp"
#include "my_tps.hpp"
#include "my_types.hpp"

static auto tp_water_description_get(Gamep g, Levelsp v, Levelp l, Thingp t) -> std::string
{
  TRACE();

  switch (l->biome) {
    case BIOME_BOGLAND :    return "murky smelly water";
    case BIOME_NETHERVOID : return "water that reflects starlight";
    case BIOME_GRAVEYARD :  return "foul smelling water";
    case BIOME_UNDERHELL :  return "unexpected water";
    case BIOME_DUNGEON :    [[fallthrough]];
    case BIOME_NONE :       [[fallthrough]];
    case BIOME_ENUM_MAX :   return "shallow water";
  }
}

auto tp_load_water() -> bool
{
  TRACE();

  auto *tp   = tp_load("water"); // keep as string for scripts
  auto  name = tp_name(tp);
  // begin sort marker1 {
  thing_description_set(tp, tp_water_description_get);
  tp_damage_set(tp, THING_EVENT_WATER_DAMAGE, "1d6");
  tp_flag_set(tp, is_animated);
  tp_flag_set(tp, is_blit_centered);
  tp_flag_set(tp, is_blit_flush_per_line);
  tp_flag_set(tp, is_blit_if_has_seen);
  tp_flag_set(tp, is_blit_per_pixel_lighting);
  tp_flag_set(tp, is_blit_shown_in_chasms);
  tp_flag_set(tp, is_described_cursor);
  tp_flag_set(tp, is_obs_to_fire);
  tp_flag_set(tp, is_obs_to_jumping_out_of);
  tp_flag_set(tp, is_physics_temperature);
  tp_flag_set(tp, is_teleport_blocked);
  tp_flag_set(tp, is_tick_end_delay);
  tp_flag_set(tp, is_tiled);
  tp_flag_set(tp, is_water);
  tp_health_set(tp, "1d10"); // to allow it to be damaged by fire
  tp_name_a_or_an_set(tp, "shallow water");
  tp_name_apostrophize_set(tp, "shallow waters'");
  tp_name_long_set(tp, "shallow water");
  tp_name_pluralize_set(tp, "shallow waters");
  tp_name_short_set(tp, "shallow water");
  tp_priority_set(tp, THING_PRIORITY_WATER);
  tp_temperature_burns_at_set(tp, 100);  // celsius
  tp_temperature_damage_at_set(tp, 100); // celsius
  tp_temperature_initial_set(tp, 10);    // celsius
  tp_weight_set(tp, WEIGHT_VVVHEAVY);    // grams
  tp_z_depth_set(tp, MAP_Z_DEPTH_LIQUID);
  // end sort marker1 }

  for (auto frame = 0; frame < 8; frame++) {
    auto  frame_string = std::to_string(frame);
    auto *tile         = tile_find_mand(name + ".IS_JOIN_BL." + frame_string);
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_BL, tile);
    tile = tile_find_mand(name + ".IS_JOIN_BL2." + frame_string);
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_BL2, tile);
    tile = tile_find_mand(name + ".IS_JOIN_BLOCK." + frame_string);
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_BLOCK, tile);
    tile = tile_find_mand(name + ".IS_JOIN_BR." + frame_string);
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_BR, tile);
    tile = tile_find_mand(name + ".IS_JOIN_BR2." + frame_string);
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_BR2, tile);
    tile = tile_find_mand(name + ".IS_JOIN_HORIZ." + frame_string);
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_HORIZ, tile);
    tile = tile_find_mand(name + ".IS_JOIN_LEFT." + frame_string);
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_LEFT, tile);
    tile = tile_find_mand(name + ".IS_JOIN_NODE." + frame_string);
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_NODE, tile);
    tile = tile_find_mand(name + ".IS_JOIN_RIGHT." + frame_string);
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_RIGHT, tile);
    tile = tile_find_mand(name + ".IS_JOIN_T_1." + frame_string);
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_T_1, tile);
    tile = tile_find_mand(name + ".IS_JOIN_T_2." + frame_string);
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_T_2, tile);
    tile = tile_find_mand(name + ".IS_JOIN_T_3." + frame_string);
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_T_3, tile);
    tile = tile_find_mand(name + ".IS_JOIN_T." + frame_string);
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_T, tile);
    tile = tile_find_mand(name + ".IS_JOIN_T180_1." + frame_string);
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_T180_1, tile);
    tile = tile_find_mand(name + ".IS_JOIN_T180_2." + frame_string);
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_T180_2, tile);
    tile = tile_find_mand(name + ".IS_JOIN_T180_3." + frame_string);
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_T180_3, tile);
    tile = tile_find_mand(name + ".IS_JOIN_T180." + frame_string);
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_T180, tile);
    tile = tile_find_mand(name + ".IS_JOIN_T270_1." + frame_string);
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_T270_1, tile);
    tile = tile_find_mand(name + ".IS_JOIN_T270_2." + frame_string);
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_T270_2, tile);
    tile = tile_find_mand(name + ".IS_JOIN_T270_3." + frame_string);
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_T270_3, tile);
    tile = tile_find_mand(name + ".IS_JOIN_T270." + frame_string);
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_T270, tile);
    tile = tile_find_mand(name + ".IS_JOIN_T90_1." + frame_string);
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_T90_1, tile);
    tile = tile_find_mand(name + ".IS_JOIN_T90_2." + frame_string);
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_T90_2, tile);
    tile = tile_find_mand(name + ".IS_JOIN_T90_3." + frame_string);
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_T90_3, tile);
    tile = tile_find_mand(name + ".IS_JOIN_T90." + frame_string);
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_T90, tile);
    tile = tile_find_mand(name + ".IS_JOIN_TL." + frame_string);
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_TL, tile);
    tile = tile_find_mand(name + ".IS_JOIN_TL2." + frame_string);
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_TL2, tile);
    tile = tile_find_mand(name + ".IS_JOIN_TOP." + frame_string);
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_TOP, tile);
    tile = tile_find_mand(name + ".IS_JOIN_BOT." + frame_string);
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_BOT, tile);
    tile = tile_find_mand(name + ".IS_JOIN_TR." + frame_string);
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_TR, tile);
    tile = tile_find_mand(name + ".IS_JOIN_TR2." + frame_string);
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_TR2, tile);
    tile = tile_find_mand(name + ".IS_JOIN_VERT." + frame_string);
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_VERT, tile);
    tile = tile_find_mand(name + ".IS_JOIN_X." + frame_string);
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_X, tile);
    tile = tile_find_mand(name + ".IS_JOIN_X1_180." + frame_string);
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_X1_180, tile);
    tile = tile_find_mand(name + ".IS_JOIN_X1_270." + frame_string);
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_X1_270, tile);
    tile = tile_find_mand(name + ".IS_JOIN_X1_90." + frame_string);
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_X1_90, tile);
    tile = tile_find_mand(name + ".IS_JOIN_X1." + frame_string);
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_X1, tile);
    tile = tile_find_mand(name + ".IS_JOIN_X2_180." + frame_string);
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_X2_180, tile);
    tile = tile_find_mand(name + ".IS_JOIN_X2_270." + frame_string);
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_X2_270, tile);
    tile = tile_find_mand(name + ".IS_JOIN_X2_90." + frame_string);
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_X2_90, tile);
    tile = tile_find_mand(name + ".IS_JOIN_X2." + frame_string);
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_X2, tile);
    tile = tile_find_mand(name + ".IS_JOIN_X3_180." + frame_string);
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_X3_180, tile);
    tile = tile_find_mand(name + ".IS_JOIN_X3." + frame_string);
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_X3, tile);
    tile = tile_find_mand(name + ".IS_JOIN_X4_180." + frame_string);
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_X4_180, tile);
    tile = tile_find_mand(name + ".IS_JOIN_X4_270." + frame_string);
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_X4_270, tile);
    tile = tile_find_mand(name + ".IS_JOIN_X4_90." + frame_string);
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_X4_90, tile);
    tile = tile_find_mand(name + ".IS_JOIN_X4." + frame_string);
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tp_tiles_push_back(tp, THING_ANIM_JOIN_X4, tile);
  }

  return true;
}
