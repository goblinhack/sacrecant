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

static auto tp_boots_spider_description_get(Gamep g, Levelsp v, Levelp l, Thingp me) -> std::string
{
  TRACE();

  return "boots, spider";
}

static auto tp_boots_spider_detail_get(Gamep g, Levelsp v, Levelp l, Thingp me) -> std::string
{
  TRACE();

  return UI_INFO1_FMT_STR "These black and chitinous boots will allow you to step as lightly as a spider.\n"                    //
      UI_INFO2_FMT_STR "More importantly, you will be better able to avoid being caught in traps and webs (*).\n"               //
      UI_INFO3_FMT_STR "And, your skin will take on a shiny and slightly disgusting chitinous sheen. But hey, defence bonus.\n" //
      UI_INFO4_FMT_STR "(*) Terms and conditions apply. Fumble rolls are still possible with traps...\n";
}

static bool tp_boots_spider_on_use(Gamep g, Levelsp v, Levelp l, Thingp me, Thingp user)
{
  TRACE();

  if (thing_is_player(user)) {
    topcon(UI_GOOD_FMT_STR "You feel spritely and a bit creepy in these spider boots." UI_RESET_FMT);
    thing_sound_play(g, v, l, user, "bonus");
  }

  return true;
}

[[nodiscard]] auto tp_load_boots_spider() -> bool
{
  TRACE();

  auto *tp   = tp_load("boots_spider"); // keep as string for scripts
  auto  name = tp_name(tp);

  // begin sort marker1 {
  thing_description_set(tp, tp_boots_spider_description_get);
  thing_detail_set(tp, tp_boots_spider_detail_get);
  thing_on_use_set(tp, tp_boots_spider_on_use);
  tp_flag_set(tp, is_able_to_be_buffed);
  tp_flag_set(tp, is_able_to_be_ensnared_blocked);
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
  tp_flag_set(tp, is_boots);
  tp_flag_set(tp, is_collectable);
  tp_flag_set(tp, is_collision_circle_large);
  tp_flag_set(tp, is_described_cursor);
  tp_flag_set(tp, is_droppable);
  tp_flag_set(tp, is_inventory_item);
  tp_flag_set(tp, is_item);
  tp_flag_set(tp, is_leather);
  tp_flag_set(tp, is_loggable);
  tp_flag_set(tp, is_physics_temperature);
  tp_flag_set(tp, is_submergible); // is seen submerged when in water
  tp_flag_set(tp, is_throwable);
  tp_flag_set(tp, is_tick_on_drop);
  tp_flag_set(tp, is_tickable);
  tp_flag_set(tp, is_treasure);
  tp_flag_set(tp, is_wearable);
  tp_health_set(tp, "1d10");
  tp_name_a_or_an_set(tp, "spider boots");
  tp_name_apostrophize_set(tp, "spider boots'");
  tp_name_long_set(tp, "spider boots");
  tp_name_pluralize_set(tp, "spider boots");
  tp_name_short_set(tp, "boots, spider");
  tp_priority_set(tp, THING_PRIORITY_OBJECT);
  tp_rarity_set(tp, THING_RARITY_UNCOMMON);
  tp_stat_set(tp, THING_STAT_DEF, "1d4+10");
  tp_stat_set(tp, THING_STAT_DEX, "1d4+14");
  tp_stat_set(tp, THING_STAT_LUCK, "6");
  tp_temperature_burns_at_set(tp, 1000);  // celsius
  tp_temperature_damage_at_set(tp, 1000); // celsius
  tp_temperature_initial_set(tp, 20);     // celsius
  tp_weight_set(tp, WEIGHT_LIGHT);        // grams
  tp_z_depth_set(tp, MAP_Z_DEPTH_OBJ);
  // end sort marker1 }

  auto delay = 1000;

  for (auto frame = 0; frame < 1; frame++) {
    auto *tile = tile_find_mand(name + std::string(".") + std::to_string(frame));
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tile_delay_ms_set(tile, delay);
    tp_tiles_push_back(tp, THING_ANIM_IDLE, tile);
  }

  return true;
}
