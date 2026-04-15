//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_thing_callbacks.hpp"
#include "my_tile.hpp"
#include "my_tp.hpp"
#include "my_tps.hpp"
#include "my_types.hpp"

static auto tp_pillar_description_get(Gamep g, Levelsp v, Levelp l, Thingp t) -> std::string
{
  TRACE();

  return "ancient moss covered pillar";
}

auto tp_load_pillar() -> bool
{
  TRACE();

  auto *tp   = tp_load("pillar"); // keep as string for scripts
  auto  name = tp_name(tp);

  // begin sort marker1 {
  thing_description_set(tp, tp_pillar_description_get);
  tp_flag_set(tp, is_able_to_fall_sound);
  tp_flag_set(tp, is_able_to_fall);
  tp_flag_set(tp, is_blit_if_has_seen);
  tp_flag_set(tp, is_blit_on_ground);
  tp_flag_set(tp, is_blit_shown_in_chasms);
  tp_flag_set(tp, is_collision_square);
  tp_flag_set(tp, is_described_cursor);
  tp_flag_set(tp, is_obs_to_cursor_path);
  tp_flag_set(tp, is_obs_to_falling_onto);
  tp_flag_set(tp, is_obs_to_fire);
  tp_flag_set(tp, is_obs_to_jump_over);
  tp_flag_set(tp, is_obs_to_jumping_onto);
  tp_flag_set(tp, is_obs_to_movement);
  tp_flag_set(tp, is_obs_to_paths);
  tp_flag_set(tp, is_obs_to_spawning);
  tp_flag_set(tp, is_obs_to_teleporting_onto);
  tp_flag_set(tp, is_obs_to_vision);
  tp_flag_set(tp, is_physics_explosion);
  tp_flag_set(tp, is_pillar);
  tp_flag_set(tp, is_stone);
  tp_flag_set(tp, is_submergible); // is seen submerged when in water
  tp_flag_set(tp, is_teleport_blocked);
  tp_health_set(tp, "1d500");
  tp_is_immunity_add(tp, THING_EVENT_FIRE_DAMAGE);
  tp_is_immunity_add(tp, THING_EVENT_HEAT_DAMAGE);
  tp_is_immunity_add(tp, THING_EVENT_MELEE_DAMAGE);
  tp_is_immunity_add(tp, THING_EVENT_WATER_DAMAGE);
  tp_name_a_or_an_set(tp, "a pillar");
  tp_name_apostrophize_set(tp, "pillars'");
  tp_name_long_set(tp, "stone pillar");
  tp_name_pluralize_set(tp, "pillars");
  tp_name_short_set(tp, "pillar");
  tp_priority_set(tp, THING_PRIORITY_WALL);
  tp_weight_set(tp, WEIGHT_VVVHEAVY); // grams
  tp_z_depth_set(tp, MAP_Z_DEPTH_OBJ);
  // end sort marker1 }

  for (auto frame = 0; frame < 16; frame++) {
    auto *tile = tile_find_mand(name + std::string(".") + std::to_string(frame));
    tp_tiles_push_back(tp, THING_ANIM_IDLE, tile);
    tile_size_set(tile, OUTLINE_TILE_WIDTH, OUTLINE_TILE_HEIGHT);
  }

  return true;
}
