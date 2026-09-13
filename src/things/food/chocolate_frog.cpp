//
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
#include "my_ui.hpp"

static auto tp_chocolate_frog_description_get(Gamep g, Levelsp v, Levelp l, Thingp me) -> std::string
{
  TRACE();

  return "chocolate frog";
}

static auto tp_chocolate_frog_detail_get(Gamep g, Levelsp v, Levelp l, Thingp me) -> std::string
{
  TRACE();

  return                                                                                         //
      UI_INFO1_FMT_STR "The food of the gods. Guaranteed to improve both your mood and health."; //
}

static bool tp_chocolate_frog_on_eaten(Gamep g, Levelsp v, Levelp l, Thingp me, Thingp user)
{
  TRACE();

  auto old_health = thing_health(g, v, l, user);
  auto new_health = thing_health_incr(g, v, l, user, thing_health_max(g, v, l, user) / 4);

  if (old_health == new_health) {
    if (thing_is_player(user)) {
      topcon(UI_WARN_FMT_STR "That frog was nice but didn't improve your health." UI_RESET_FMT);
      thing_sound_play(g, v, l, user, "error");
    }
  } else {
    if (thing_is_player(user)) {
      topcon(UI_GOOD_FMT_STR "That frog hit the spot." UI_RESET_FMT);
      thing_sound_play(g, v, l, user, "bonus");
    }
  }

  if (thing_hook_add(g, v, l, user, tp_find_mand("buff_good_luck"))) {
    topcon("You feel lucky!");
  }

  return true;
}

[[nodiscard]] auto tp_load_chocolate_frog() -> bool
{
  TRACE();

  auto *tp   = tp_load("chocolate_frog"); // keep as string for scripts
  auto  name = tp_name(tp);

  // begin sort marker1 {
  thing_description_set(tp, tp_chocolate_frog_description_get);
  thing_detail_set(tp, tp_chocolate_frog_detail_get);
  thing_on_eaten_set(tp, tp_chocolate_frog_on_eaten);
  tp_chance_set(tp, THING_CHANCE_CONTINUE_TO_BURN, "1d2"); // fumble => intensify / keep burning / crit => stop burning
  tp_chance_set(tp, THING_CHANCE_START_BURNING, "1d100");  // fumble => flames spread to you
  tp_flag_set(tp, is_able_to_fall_sound);
  tp_flag_set(tp, is_able_to_fall);
  tp_flag_set(tp, is_active_when_carried);
  tp_flag_set(tp, is_animated);
  tp_flag_set(tp, is_blit_hit_outline_w_black_inside);
  tp_flag_set(tp, is_blit_on_ground);
  tp_flag_set(tp, is_blit_shown_in_chasms);
  tp_flag_set(tp, is_burnable); // is seen submerged when in water
  tp_flag_set(tp, is_chocolate);
  tp_flag_set(tp, is_collectable);
  tp_flag_set(tp, is_collision_circle_large);
  tp_flag_set(tp, is_crushable);
  tp_flag_set(tp, is_described_cursor);
  tp_flag_set(tp, is_droppable);
  tp_flag_set(tp, is_edible);
  tp_flag_set(tp, is_flat);
  tp_flag_set(tp, is_inventory_item);
  tp_flag_set(tp, is_item_mergeable);
  tp_flag_set(tp, is_item);
  tp_flag_set(tp, is_loggable);
  tp_flag_set(tp, is_lucky);
  tp_flag_set(tp, is_meltable);
  tp_flag_set(tp, is_physics_explosion);
  tp_flag_set(tp, is_physics_temperature);
  tp_flag_set(tp, is_submergible); // is seen submerged when in water
  tp_flag_set(tp, is_tick_on_drop);
  tp_flag_set(tp, is_tick_on_eaten);
  tp_flag_set(tp, is_treasure);
  tp_name_a_or_an_set(tp, "a chocolate frog");
  tp_name_apostrophize_set(tp, "chocolate frogs'");
  tp_name_long_set(tp, "chocolate frog");
  tp_name_pluralize_set(tp, "chocolate frogs");
  tp_name_short_set(tp, "choc frog");
  tp_priority_set(tp, THING_PRIORITY_OBJECT);
  tp_rarity_set(tp, THING_RARITY_COMMON);
  tp_stat_set(tp, THING_STAT_LUCK, "12");
  tp_temperature_burns_at_set(tp, 100); // celsius
  tp_temperature_damage_at_set(tp, 30); // celsius
  tp_temperature_initial_set(tp, 20);   // celsius
  tp_weight_set(tp, WEIGHT_LIGHT);      // grams
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
