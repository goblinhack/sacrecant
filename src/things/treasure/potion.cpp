//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_main.hpp"
#include "my_thing_callbacks.hpp"
#include "my_thing_inlines.hpp"
#include "my_tile.hpp"
#include "my_tp.hpp"
#include "my_tps.hpp"
#include "my_types.hpp"
#include "my_ui.hpp"

static auto tp_potion_description_get(Gamep g, Levelsp v, Levelp l, Thingp t) -> std::string
{
  TRACE();

  return "potion";
}

static auto tp_potion_detail_get(Gamep g, Levelsp v, Levelp l, Thingp t) -> std::string
{
  TRACE();

  return UI_INFO1_FMT_STR "A mysterious purple potion.";
}

[[nodiscard]] static auto tp_potion_on_carry_request(Gamep g, Levelsp v, Levelp l, Thingp t, Thingp collector) -> bool
{
  TRACE();

  if (thing_is_player(collector)) {
    thing_sound_play(g, v, l, collector, "item_collect");
  }

  return true;
}

[[nodiscard]] static auto tp_potion_on_drop_request(Gamep g, Levelsp v, Levelp l, Thingp t, Thingp dropper) -> bool
{
  TRACE();

  if (thing_is_player(dropper)) {
    thing_sound_play(g, v, l, dropper, "item_drop");
  }

  return true;
}

auto tp_load_potion() -> bool
{
  TRACE();

  auto *tp   = tp_load("potion"); // keep as string for scripts
  auto  name = tp_name(tp);

  // begin sort marker1 {
  thing_description_set(tp, tp_potion_description_get);
  thing_detail_set(tp, tp_potion_detail_get);
  thing_on_carry_request_set(tp, tp_potion_on_carry_request);
  thing_on_drop_request_set(tp, tp_potion_on_drop_request);
  tp_chance_set(tp, THING_CHANCE_CONTINUE_TO_BURN, "1d2"); // roll max to continue burning
  tp_flag_set(tp, is_able_to_fall_sound);
  tp_flag_set(tp, is_able_to_fall);
  tp_flag_set(tp, is_animated);
  tp_flag_set(tp, is_blit_centered);
  tp_flag_set(tp, is_blit_hit_outline_w_black_inside);
  tp_flag_set(tp, is_blit_shown_in_chasms);
  tp_flag_set(tp, is_burnable); // is capable of being burned by fire
  tp_flag_set(tp, is_collectable);
  tp_flag_set(tp, is_collision_circle_small);
  tp_flag_set(tp, is_combustible); // will continue to burn once on fire
  tp_flag_set(tp, is_described_cursor);
  tp_flag_set(tp, is_glass);
  tp_flag_set(tp, is_inventory_item);
  tp_flag_set(tp, is_item_droppable);
  tp_flag_set(tp, is_item_mergeable);
  tp_flag_set(tp, is_item);
  tp_flag_set(tp, is_light_source, 2);
  tp_flag_set(tp, is_loggable);
  tp_flag_set(tp, is_physics_explosion);
  tp_flag_set(tp, is_physics_temperature);
  tp_flag_set(tp, is_submergible); // is seen submerged when in water
  tp_flag_set(tp, is_tickable);
  tp_flag_set(tp, is_treasure);
  tp_health_set(tp, "1d4");
  tp_is_immunity_add(tp, THING_EVENT_WATER_DAMAGE);
  tp_light_color_set(tp, "purple");
  tp_name_a_or_an_set(tp, "a potion");
  tp_name_apostrophize_set(tp, "potions'");
  tp_name_long_set(tp, "potion");
  tp_name_pluralize_set(tp, "potions");
  tp_name_short_set(tp, "potion");
  tp_priority_set(tp, THING_PRIORITY_OBJECT);
  tp_temperature_burns_at_set(tp, 30);  // celsius
  tp_temperature_damage_at_set(tp, 30); // celsius
  tp_temperature_initial_set(tp, 20);   // celsius
  tp_weight_set(tp, WEIGHT_LIGHT);      // grams
  tp_z_depth_set(tp, MAP_Z_DEPTH_OBJ);
  // end sort marker1 }

  auto delay = 1000;

  for (auto frame = 0; frame < 2; frame++) {
    auto *tile = tile_find_mand(name + std::string(".") + std::to_string(frame));
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tile_delay_ms_set(tile, delay);
    tp_tiles_push_back(tp, THING_ANIM_IDLE, tile);
  }

  return true;
}
