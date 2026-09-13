//
// Copyright goblinhack@gmail.com
//

#include "../../my_callstack.hpp"
#include "../../my_dice_rolls.hpp"
#include "../../my_thing.hpp"
#include "../../my_thing_callbacks.hpp"
#include "../../my_thing_inlines.hpp"
#include "../../my_tile.hpp"
#include "../../my_tp.hpp"
#include "../../my_tps.hpp"
#include "../../my_types.hpp"
#include "../../my_ui.hpp"
#include "../../my_wids.hpp"

static auto tp_copious_chest_description_get(Gamep g, Levelsp v, Levelp l, Thingp me) -> std::string
{
  TRACE();

  if (thing_is_open(me)) {
    return "open chest";
  }
  if (thing_is_dead(me)) {
    return "broken chest";
  }
  return "closed chest";
}

static auto tp_copious_chest_detail_get(Gamep g, Levelsp v, Levelp l, Thingp me) -> std::string
{
  TRACE();

  if (thing_is_open(me)) {
    return UI_INFO1_FMT_STR "An open casket.";
  }
  if (thing_is_dead(me)) {
    return UI_INFO1_FMT_STR "A broken casket.";
  }
  return UI_INFO1_FMT_STR "A copious casket. What wonders might it contain within its gleaming metal shell?";
}

[[nodiscard]] static auto tp_copious_chest_on_open_request(Gamep g, Levelsp v, Levelp l, Thingp me, Thingp opener) -> bool
{
  TRACE();

  thing_sound_play(g, v, l, me, "chest_open");

  if (thing_is_player(opener)) {
    (void) thing_noise_incr(g, v, l, me, THING_NOISE_CHEST_OPEN);
  }

  std::vector< Thingp > items;

  auto nitems = 5;

  while (d100() < 30) {
    nitems++;
  }

  nitems = std::min(nitems, THING_INVENTORY_MAX);

  THING_DBG(g, v, l, me, "spawn items");
  TRACE_INDENT();

  for (auto i = 0; i < nitems; i++) {
    if (nitems > 100) {
      break;
    }

    auto tp = tp_random(g, v, l, is_treasure);
    if (tp_is_chest(tp)) {
      nitems++;
      continue;
    }

    auto item = thing_spawn(g, v, l, tp, thing_at(g, v, l, me));
    if (item) {
      //
      // Needed to stop auto collect
      //
      (void) thing_collision_handle_done_already(v, item, opener);

      items.push_back(item);
      thing_log(g, v, l, item, "spawned for chest");
    }
  }

  if (thing_is_player(opener)) {
    if (g_opt_tests) {
      (void) thing_carry(g, v, l, opener, items);
    } else {
      wid_collect_show(g, v, l, opener, items);
    }
  }

  return true;
}

static bool tp_copious_chest_on_damage(Gamep g, Levelsp v, Levelp l, Thingp me, ThingEvent &e)
{
  TRACE();

  if (d100() < 10) {
    return true;
  }

  if (e.source && thing_is_player(e.source)) {
    topcon("The metal treasure chest rattles violently!");
  }

  return true; // allow the damage to be applied
}

[[nodiscard]] auto tp_load_copious_chest() -> bool
{
  TRACE();

  auto *tp   = tp_load("copious_chest"); // keep as string for scripts
  auto  name = tp_name(tp);

  // begin sort marker1 {
  thing_description_set(tp, tp_copious_chest_description_get);
  thing_detail_set(tp, tp_copious_chest_detail_get);
  thing_on_damage_set(tp, tp_copious_chest_on_damage);
  thing_on_open_request_set(tp, tp_copious_chest_on_open_request);
  tp_flag_set(tp, is_able_to_be_buffed);
  tp_flag_set(tp, is_able_to_be_invisible);
  tp_flag_set(tp, is_able_to_be_levitated);
  tp_flag_set(tp, is_able_to_be_teleported);
  tp_flag_set(tp, is_able_to_fall_sound);
  tp_flag_set(tp, is_able_to_fall);
  tp_flag_set(tp, is_animated);
  tp_flag_set(tp, is_blit_centered);
  tp_flag_set(tp, is_blit_hit_outline_w_black_inside);
  tp_flag_set(tp, is_blit_shown_in_chasms);
  tp_flag_set(tp, is_chest);
  tp_flag_set(tp, is_collectable); // for engulfers
  tp_flag_set(tp, is_collision_circle_large);
  tp_flag_set(tp, is_described_cursor);
  tp_flag_set(tp, is_item); // for engulfers
  tp_flag_set(tp, is_loggable);
  tp_flag_set(tp, is_metal);
  tp_flag_set(tp, is_openable);
  tp_flag_set(tp, is_physics_explosion);
  tp_flag_set(tp, is_physics_temperature);
  tp_flag_set(tp, is_physics_trap);
  tp_flag_set(tp, is_submergible); // is seen submerged when in water
  tp_flag_set(tp, is_tick_on_drop);
  tp_flag_set(tp, is_tickable);
  tp_flag_set(tp, is_treasure);
  tp_health_set(tp, "1d50");
  tp_is_immune_to_add(tp, THING_EVENT_WATER_DAMAGE);
  tp_name_a_or_an_set(tp, "a treasure casket");
  tp_name_apostrophize_set(tp, "treasure caskets'");
  tp_name_long_set(tp, "treasure casket");
  tp_name_pluralize_set(tp, "treasure caskets");
  tp_name_short_set(tp, "copious casker");
  tp_priority_set(tp, THING_PRIORITY_OBJECT);
  tp_rarity_set(tp, THING_RARITY_UNCOMMON);
  tp_temperature_burns_at_set(tp, 500);  // celsius
  tp_temperature_damage_at_set(tp, 500); // celsius
  tp_temperature_initial_set(tp, 20);    // celsius
  tp_weight_set(tp, WEIGHT_VVHEAVY);     // grams
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
