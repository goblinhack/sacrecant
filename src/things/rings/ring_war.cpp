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

static auto tp_ring_war_description_get(Gamep g, Levelsp v, Levelp l, Thingp me) -> std::string
{
  TRACE();

  return "ring, war";
}

static auto tp_ring_war_detail_get(Gamep g, Levelsp v, Levelp l, Thingp me) -> std::string
{
  TRACE();

  return UI_INFO1_FMT_STR
      "The ring of war gives off a menacing red light and, when worn, " //
      "will transform your puny fists into vessels of pain.\n"          //
      UI_INFO2_FMT_STR
      "More specifically, you will see bonuses to your attack and damage rolls. " //
      "Damage applies even to staves and wands.\n"                                //
      UI_INFO3_FMT_STR
      "Being as crazed as you are, you will have no need for such weaknesses as defence, "
      "so expect a significant drop in your defensive ability.\n";
}

[[nodiscard]] auto tp_load_ring_war() -> bool
{
  TRACE();

  auto *tp   = tp_load("ring_war"); // keep as string for scripts
  auto  name = tp_name(tp);

  // begin sort marker1 {
  thing_description_set(tp, tp_ring_war_description_get);
  thing_detail_set(tp, tp_ring_war_detail_get);
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
  tp_light_color_set(tp, "red");
  tp_name_a_or_an_set(tp, "a ring of war");
  tp_name_apostrophize_set(tp, "ring of war's");
  tp_name_long_set(tp, "ring of war");
  tp_name_pluralize_set(tp, "rings of war");
  tp_name_short_set(tp, "ring, war");
  tp_priority_set(tp, THING_PRIORITY_OBJECT);
  tp_rarity_set(tp, THING_RARITY_UNCOMMON);
  tp_stat_set(tp, THING_STAT_ATT, "1d4+14");
  tp_stat_set(tp, THING_STAT_DEF, "6");
  tp_stat_set(tp, THING_STAT_DMG, "1d4+14");
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
