//
// Copyright goblinhack@gmail.com
//

#include "../../my_callstack.hpp"
#include "../../my_main.hpp"
#include "../../my_thing_callbacks.hpp"
#include "../../my_thing_inlines.hpp"
#include "../../my_tile.hpp"
#include "../../my_tp.hpp"
#include "../../my_tps.hpp"
#include "../../my_types.hpp"
#include "../../my_ui.hpp"

static auto tp_ring_life_description_get(Gamep g, Levelsp v, Levelp l, Thingp me) -> std::string
{
  TRACE();

  return "ring, life";
}

static auto tp_ring_life_detail_get(Gamep g, Levelsp v, Levelp l, Thingp me) -> std::string
{
  TRACE();

  return UI_INFO1_FMT_STR "A ring of life is a very special thing. Should you die in service, your health will be restored to 50%%%.\n" //
      UI_INFO2_FMT_STR "More importantly, you will not be dead! Being not dead, is good.\n"                                             //
      UI_INFO3_FMT_STR "Keep it safe. Keep it secret.\n";
}

static bool tp_ring_life_on_use(Gamep g, Levelsp v, Levelp l, Thingp me, Thingp user)
{
  TRACE();

  if (thing_is_player(user)) {
    topcon(UI_GOOD_FMT_STR "You feel brave wearing this ring." UI_RESET_FMT);
    thing_sound_play(g, v, l, user, "bonus");
  }

  return true;
}

static bool tp_ring_life_on_damage(Gamep g, Levelsp v, Levelp l, Thingp me, ThingEvent &e)
{
  TRACE();

  if (! e.damage) {
    return true; // allow the damage to be applied
  }

  THING_DBG(g, v, l, me, "damage %d", e.damage);
  auto owner = thing_owner(g, v, l, me);
  if (! owner) {
    return true; // allow the damage to be applied
  }

  if (e.damage < thing_health(g, v, l, owner)) {
    THING_DBG(g, v, l, me, "not activated");
    return true; // allow the damage to be applied
  }

  THING_DBG(g, v, l, me, "activated");
  (void) thing_health_set(g, v, l, owner, thing_health_max(g, v, l, owner) / 2);

  if (thing_is_player(owner)) {
    topcon(UI_GOOD_FMT_STR "Your ring of life saves you as it crumbles into dust." UI_RESET_FMT);
    thing_sound_play(g, v, l, owner, "bonus");
  }

  THING_DBG(g, v, l, owner, "new health");
  THING_DBG(g, v, l, me, "dead due to being used");
  TRACE_INDENT();

  thing_dead(g, v, l, me, e);

  return true; // allow the damage to be applied
}

[[nodiscard]] auto tp_load_ring_life() -> bool
{
  TRACE();

  auto *tp   = tp_load("ring_life"); // keep as string for scripts
  auto  name = tp_name(tp);

  // begin sort marker1 {
  thing_description_set(tp, tp_ring_life_description_get);
  thing_detail_set(tp, tp_ring_life_detail_get);
  thing_on_damage_set(tp, tp_ring_life_on_damage);
  thing_on_use_set(tp, tp_ring_life_on_use);
  tp_flag_set(tp, is_able_to_be_buffed);
  tp_flag_set(tp, is_able_to_be_invisible);
  tp_flag_set(tp, is_able_to_be_levitated);
  tp_flag_set(tp, is_able_to_be_teleported);
  tp_flag_set(tp, is_able_to_fall_sound);
  tp_flag_set(tp, is_able_to_fall);
  tp_flag_set(tp, is_active_when_worn);
  tp_flag_set(tp, is_animated);
  tp_flag_set(tp, is_auto_wear);
  tp_flag_set(tp, is_blit_centered);
  tp_flag_set(tp, is_blit_hit_outline_w_black_inside);
  tp_flag_set(tp, is_blit_shown_in_chasms);
  tp_flag_set(tp, is_collectable);
  tp_flag_set(tp, is_collision_circle_small);
  tp_flag_set(tp, is_described_cursor);
  tp_flag_set(tp, is_droppable);
  tp_flag_set(tp, is_gold);
  tp_flag_set(tp, is_inventory_item);
  tp_flag_set(tp, is_item);
  tp_flag_set(tp, is_light_source, 2);
  tp_flag_set(tp, is_loggable);
  tp_flag_set(tp, is_metal);
  tp_flag_set(tp, is_physics_temperature);
  tp_flag_set(tp, is_ring);
  tp_flag_set(tp, is_submergible); // is seen submerged when in water
  tp_flag_set(tp, is_throwable);
  tp_flag_set(tp, is_tick_on_drop);
  tp_flag_set(tp, is_tickable);
  tp_flag_set(tp, is_treasure);
  tp_flag_set(tp, is_wearable);
  tp_health_set(tp, "1d50");
  tp_is_immune_to_add(tp, THING_EVENT_CRUSH_DAMAGE);
  tp_is_immune_to_add(tp, THING_EVENT_FIRE_DAMAGE);
  tp_is_immune_to_add(tp, THING_EVENT_WATER_DAMAGE);
  tp_light_color_set(tp, "white");
  tp_name_a_or_an_set(tp, "a ring of life");
  tp_name_apostrophize_set(tp, "ring of life's");
  tp_name_long_set(tp, "ring of life");
  tp_name_pluralize_set(tp, "rings of life");
  tp_name_short_set(tp, "ring, life");
  tp_priority_set(tp, THING_PRIORITY_OBJECT);
  tp_rarity_set(tp, THING_RARITY_UNCOMMON);
  tp_temperature_burns_at_set(tp, 1000);  // celsius
  tp_temperature_damage_at_set(tp, 1000); // celsius
  tp_temperature_initial_set(tp, 20);     // celsius
  tp_weight_set(tp, WEIGHT_VLIGHT);       // grams
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
