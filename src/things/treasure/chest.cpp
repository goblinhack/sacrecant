//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_thing.hpp"
#include "my_thing_callbacks.hpp"
#include "my_thing_inlines.hpp"
#include "my_tile.hpp"
#include "my_tp.hpp"
#include "my_tps.hpp"
#include "my_types.hpp"
#include "my_ui.hpp"

static auto tp_chest_description_get(Gamep g, Levelsp v, Levelp l, Thingp t) -> std::string
{
  TRACE();

  if (thing_is_open(t)) {
    return "open chest";
  }
  if (thing_is_dead(t)) {
    return "broken chest";
  }
  return "closed chest";
}

static auto tp_chest_detail_get(Gamep g, Levelsp v, Levelp l, Thingp t) -> std::string
{
  TRACE();

  if (thing_is_open(t)) {
    return UI_INFO1_FMT_STR "An open chest.";
  }
  if (thing_is_dead(t)) {
    return UI_INFO1_FMT_STR "A broken chest.";
  }
  return UI_INFO1_FMT_STR "A closed chest. What wonders might it contain? Probably none.";
}

[[nodiscard]] static auto tp_chest_on_open_request(Gamep g, Levelsp v, Levelp l, Thingp t, Thingp opener) -> bool
{
  TRACE();

  thing_sound_play(g, v, l, t, "chest_open");

  return true;
}

auto tp_load_chest() -> bool
{
  TRACE();

  auto *tp   = tp_load("chest"); // keep as string for scripts
  auto  name = tp_name(tp);

  // begin sort marker1 {
  thing_description_set(tp, tp_chest_description_get);
  thing_detail_set(tp, tp_chest_detail_get);
  thing_on_open_request_set(tp, tp_chest_on_open_request);
  tp_chance_set(tp, THING_CHANCE_CONTINUE_TO_BURN, "1d2"); // roll max to continue burning
  tp_flag_set(tp, is_able_to_fall_sound);
  tp_flag_set(tp, is_able_to_fall);
  tp_flag_set(tp, is_animated);
  tp_flag_set(tp, is_blit_centered);
  tp_flag_set(tp, is_blit_hit_outline_w_black_inside);
  tp_flag_set(tp, is_blit_shown_in_chasms);
  tp_flag_set(tp, is_burnable); // is capable of being burned by fire
  tp_flag_set(tp, is_collision_circle_large);
  tp_flag_set(tp, is_combustible); // will continue to burn once on fire
  tp_flag_set(tp, is_described_cursor);
  tp_flag_set(tp, is_loggable);
  tp_flag_set(tp, is_openable);
  tp_flag_set(tp, is_physics_explosion);
  tp_flag_set(tp, is_physics_temperature);
  tp_flag_set(tp, is_submergible); // is seen submerged when in water
  tp_flag_set(tp, is_tickable);
  tp_flag_set(tp, is_treasure);
  tp_flag_set(tp, is_wood);
  tp_health_set(tp, "1d20");
  tp_is_immunity_add(tp, THING_EVENT_WATER_DAMAGE);
  tp_name_a_or_an_set(tp, "a treasure chest");
  tp_name_apostrophize_set(tp, "treasure chests'");
  tp_name_long_set(tp, "treasure chest");
  tp_name_pluralize_set(tp, "treasure chests");
  tp_name_short_set(tp, "chest");
  tp_priority_set(tp, THING_PRIORITY_OBJECT);
  tp_temperature_burns_at_set(tp, 100);  // celsius
  tp_temperature_damage_at_set(tp, 100); // celsius
  tp_temperature_initial_set(tp, 20);    // celsius
  tp_weight_set(tp, WEIGHT_VHEAVY);      // grams
  tp_z_depth_set(tp, MAP_Z_DEPTH_OBJ);
  // end sort marker1 }

  auto delay = 1000;

  for (auto frame = 0; frame < 1; frame++) {
    auto *tile = tile_find_mand(name + std::string(".closed.") + std::to_string(frame));
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tile_delay_ms_set(tile, delay);
    tp_tiles_push_back(tp, THING_ANIM_IDLE, tile);
  }

  for (auto frame = 0; frame < 1; frame++) {
    auto *tile = tile_find_mand(name + std::string(".open.") + std::to_string(frame));
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tile_delay_ms_set(tile, delay);
    tp_tiles_push_back(tp, THING_ANIM_OPEN, tile);
  }

  return true;
}
