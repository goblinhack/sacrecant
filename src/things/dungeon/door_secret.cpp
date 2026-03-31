//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_main.hpp"
#include "my_sound.hpp"
#include "my_thing.hpp"
#include "my_thing_callbacks.hpp"
#include "my_thing_inlines.hpp"
#include "my_tile.hpp"
#include "my_tp.hpp"
#include "my_tps.hpp"
#include "my_types.hpp"

static auto tp_door_secret_description_get(Gamep g, Levelsp v, Levelp l, Thingp t) -> std::string
{
  TRACE();

  if (thing_is_open(t)) {
    return "secret door";
  }

  return "odd looking rock";
}

[[nodiscard]] static auto tp_door_secret_on_open_request(Gamep g, Levelsp v, Levelp l, Thingp t, Thingp opener) -> bool
{
  TRACE();

  ThingEvent e {
      .reason     = "by opening",     //
      .event_type = THING_EVENT_OPEN, //
  };

  if (thing_is_player(opener)) {
    topcon("A secret door creaks open!");
  }

  thing_sound_play(g, v, l, t, "door_open_secret");

  thing_dead(g, v, l, t, e);

  return true;
}

auto tp_load_door_secret() -> bool
{
  TRACE();

  for (auto variant = 1; variant <= WALL_VARIANTS; variant++) {
    std::string const name = "door_secret" + std::to_string(variant);
    auto             *tp   = tp_load(name);
    // begin sort marker1 {
    thing_description_set(tp, tp_door_secret_description_get);
    thing_on_open_request_set(tp, tp_door_secret_on_open_request);
    tp_flag_set(tp, is_blit_centered);
    tp_flag_set(tp, is_blit_if_has_seen);
    tp_flag_set(tp, is_blit_shown_in_chasms);
    tp_flag_set(tp, is_collision_square);
    tp_flag_set(tp, is_described_cursor);
    tp_flag_set(tp, is_dmap);
    tp_flag_set(tp, is_door_secret);
    tp_flag_set(tp, is_loggable);
    tp_flag_set(tp, is_obs_to_cursor_path);
    tp_flag_set(tp, is_obs_to_explosion);
    tp_flag_set(tp, is_obs_to_fire);
    tp_flag_set(tp, is_obs_to_spawning);
    tp_flag_set(tp, is_obs_to_vision);
    tp_flag_set(tp, is_openable);
    tp_flag_set(tp, is_physics_explosion);
    tp_flag_set(tp, is_physics_temperature);
    tp_flag_set(tp, is_stone);
    tp_flag_set(tp, is_teleport_blocked);
    tp_flag_set(tp, is_tickable);
    tp_health_set(tp, "200");
    tp_is_immunity_add(tp, THING_EVENT_WATER_DAMAGE);
    tp_name_a_or_an_set(tp, "a secret door");
    tp_name_apostrophize_set(tp, "secret doors'");
    tp_name_long_set(tp, "secret door");
    tp_name_pluralize_set(tp, "secret door");
    tp_name_short_set(tp, "secret door");
    tp_priority_set(tp, THING_PRIORITY_OBJECT);
    tp_temperature_damage_at_set(tp, 50); // celsius
    tp_temperature_initial_set(tp, 20);   // celsius
    tp_temperature_melts_at_set(tp, 300); // celsius
    tp_variant_set(tp, variant);
    tp_weight_set(tp, WEIGHT_VHEAVY); // grams
    tp_z_depth_set(tp, MAP_Z_DEPTH_OBJ);
    // end sort marker1 }

    auto *tile = tile_find_mand(name + ".idle.0");
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tp_tiles_push_back(tp, THING_ANIM_IDLE, tile);
    tile = tile_find_mand(name + ".open.0");
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tp_tiles_push_back(tp, THING_ANIM_OPEN, tile);
  }

  return true;
}
