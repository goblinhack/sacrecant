//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_thing_callbacks.hpp"
#include "my_thing_inlines.hpp"
#include "my_tile.hpp"
#include "my_tp.hpp"
#include "my_tps.hpp"
#include "my_types.hpp"

static auto tp_reeds_description_get(Gamep g, Levelsp v, Levelp l, Thingp t) -> std::string
{
  TRACE();

  return "sickly looking reeds";
}

static auto tp_reeds_z_depth_get(Gamep g, Levelsp v, Levelp l, Thingp t) -> MapZDepth
{
  TRACE();

  if (thing_is_dead(t)) {
    return MAP_Z_DEPTH_GRASS;
  }
  return MAP_Z_DEPTH_FOLIAGE;
}

auto tp_load_reeds() -> bool
{
  TRACE();

  auto *tp   = tp_load("reeds"); // keep as string for scripts
  auto  name = tp_name(tp);

  // begin sort marker1 {
  thing_description_set(tp, tp_reeds_description_get);
  thing_z_depth_set(tp, tp_reeds_z_depth_get);
  tp_chance_set(tp, THING_CHANCE_CONTINUE_TO_BURN, "1d2"); // roll max to continue burning
  tp_chance_set(tp, THING_CHANCE_START_BURNING, "1d2");    // roll max to continue burning
  tp_flag_set(tp, is_able_to_fall);
  tp_flag_set(tp, is_blit_hit_outline_w_black_inside);
  tp_flag_set(tp, is_blit_if_has_seen);
  tp_flag_set(tp, is_blit_obscures);
  tp_flag_set(tp, is_blit_on_ground);
  tp_flag_set(tp, is_blit_shown_in_chasms);
  tp_flag_set(tp, is_collision_circle_large);
  tp_flag_set(tp, is_corpse_on_death);
  tp_flag_set(tp, is_described_cursor);
  tp_flag_set(tp, is_loggable);
  tp_flag_set(tp, is_obs_to_vision);
  tp_flag_set(tp, is_physics_explosion);
  tp_flag_set(tp, is_physics_temperature);
  tp_flag_set(tp, is_plant);
  tp_flag_set(tp, is_reeds);
  tp_flag_set(tp, is_removable_when_dead_on_err);
  tp_flag_set(tp, is_submergible);
  tp_flag_set(tp, is_teleport_blocked);
  tp_flag_set(tp, is_tickable);
  tp_health_set(tp, "1d5");
  tp_is_immunity_add(tp, THING_EVENT_WATER_DAMAGE);
  tp_name_a_or_an_set(tp, "reeds");
  tp_name_apostrophize_set(tp, "reedss'");
  tp_name_long_set(tp, "reeds");
  tp_name_pluralize_set(tp, "reeds");
  tp_name_short_set(tp, "reeds");
  tp_priority_set(tp, THING_PRIORITY_FOLIAGE);
  tp_temperature_burns_at_set(tp, 100); // celsius
  tp_temperature_damage_at_set(tp, 50); // celsius
  tp_temperature_initial_set(tp, 20);   // celsius
  tp_weight_set(tp, WEIGHT_LIGHT);      // grams
  tp_z_depth_set(tp, MAP_Z_DEPTH_FOLIAGE);
  // end sort marker1 }

  for (auto frame = 0; frame < 14; frame++) {
    auto *tile = tile_find_mand(name + std::string(".idle.") + std::to_string(frame));
    tile_size_set(tile, OUTLINE_TILE_WIDTH, OUTLINE_TILE_HEIGHT);
    tp_tiles_push_back(tp, THING_ANIM_IDLE, tile);
  }

  for (auto frame = 0; frame < 1; frame++) {
    auto *tile = tile_find_mand(name + std::string(".dead.") + std::to_string(frame));
    tile_size_set(tile, OUTLINE_TILE_WIDTH, OUTLINE_TILE_HEIGHT);
    tp_tiles_push_back(tp, THING_ANIM_DEAD, tile);
  }

  for (auto frame = 0; frame < 1; frame++) {
    auto *tile = tile_find_mand(name + std::string(".burnt.") + std::to_string(frame));
    tile_size_set(tile, OUTLINE_TILE_WIDTH, OUTLINE_TILE_HEIGHT);
    tp_tiles_push_back(tp, THING_ANIM_BURNT, tile);
  }

  return true;
}
