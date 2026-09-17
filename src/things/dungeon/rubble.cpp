//
// Copyright goblinhack@gmail.com
//

#include "../../my_callstack.hpp"
#include "../../my_thing_callbacks.hpp"
#include "../../my_tile.hpp"
#include "../../my_tp.hpp"
#include "../../my_tps.hpp"
#include "../../my_types.hpp"

static auto tp_rubble_description_get(Gamep g, Levelsp v, Levelp l, Thingp me) -> std::string
{
  TRACE();

  return "pile of rubble";
}

[[nodiscard]] auto tp_load_rubble() -> bool
{
  TRACE();

  auto *tp   = tp_load("rubble"); // keep as string for scripts
  auto  name = tp_name(tp);

  // begin sort marker1 {
  thing_description_set(tp, tp_rubble_description_get);
  tp_flag_set(tp, is_able_to_fall_sound);
  tp_flag_set(tp, is_able_to_fall);
  tp_flag_set(tp, is_blit_if_has_seen);
  tp_flag_set(tp, is_blit_on_ground);
  tp_flag_set(tp, is_wall_walker_pass_through);
  tp_flag_set(tp, is_blit_shown_in_chasms);
  tp_flag_set(tp, is_collision_square);
  tp_flag_set(tp, is_described_cursor);
  tp_flag_set(tp, is_flat);
  tp_flag_set(tp, is_obs_to_cursor_path);
  tp_flag_set(tp, is_obs_to_falling_onto);
  tp_flag_set(tp, is_obs_to_fire);
  tp_flag_set(tp, is_obs_to_jumping_onto);
  tp_flag_set(tp, is_obs_to_movement);
  tp_flag_set(tp, is_obs_to_paths);
  tp_flag_set(tp, is_obs_to_spawning);
  tp_flag_set(tp, is_physics_explosion);
  tp_flag_set(tp, is_rubble);
  tp_flag_set(tp, is_stone);
  tp_flag_set(tp, is_submergible); // is seen submerged when in water
  tp_health_set(tp, "1d100");
  tp_is_immune_to_add(tp, THING_EVENT_FIRE_DAMAGE);
  tp_is_immune_to_add(tp, THING_EVENT_MELEE_DAMAGE);
  tp_is_immune_to_add(tp, THING_EVENT_WATER_DAMAGE);
  tp_name_a_or_an_set(tp, "rubble");
  tp_name_apostrophize_set(tp, "rubble'");
  tp_name_long_set(tp, "rubble");
  tp_name_pluralize_set(tp, "rubble");
  tp_name_short_set(tp, "rubble");
  tp_priority_set(tp, THING_PRIORITY_WALL);
  tp_weight_set(tp, WEIGHT_VHEAVY); // grams
  tp_z_depth_set(tp, MAP_Z_DEPTH_OBJ);
  // end sort marker1 }

  for (auto frame = 0; frame < 1; frame++) {
    const auto delay = 1000; /* ms */
    auto      *tile  = tile_find_mand("rubble." + std::to_string(frame));
    tile_delay_ms_set(tile, delay);
    tp_tiles_push_back(tp, THING_ANIM_IDLE, tile);
    tile_size_set(tile, OUTLINE_TILE_WIDTH, OUTLINE_TILE_HEIGHT);
  }

  return true;
}
