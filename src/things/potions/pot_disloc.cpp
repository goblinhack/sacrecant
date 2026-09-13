//
// Copyright goblinhack@gmail.com
//

#include "../../my_callstack.hpp"
#include "../../my_level_inlines.hpp"
#include "../../my_main.hpp"
#include "../../my_thing_callbacks.hpp"
#include "../../my_thing_inlines.hpp"
#include "../../my_tile.hpp"
#include "../../my_tp.hpp"
#include "../../my_tps.hpp"
#include "../../my_types.hpp"
#include "../../my_ui.hpp"

static auto tp_pot_disloc_description_get(Gamep g, Levelsp v, Levelp l, Thingp me) -> std::string
{
  TRACE();

  return "potion, dislocation";
}

static auto tp_pot_disloc_detail_get(Gamep g, Levelsp v, Levelp l, Thingp me) -> std::string
{
  TRACE();

  return UI_INFO1_FMT_STR "This sparkling blue potion will transport you somewhere new and exciting on the level.\n" //
      UI_INFO2_FMT_STR "At random, so best used in emergencies.\n"                                                   //
      UI_INFO3_FMT_STR "Don't neglect the influence of your luck stat as to where you might land...";
}

static void tp_pot_disloc_on_thrown_end(Gamep g, Levelsp v, Levelp l, Thingp me, Thingp thrower)
{
  THING_DBG(g, v, l, me, "thrown end");
  TRACE_INDENT();

  //
  // Soft landing?
  //
  if ((level_is_soft_landing_bool(g, v, l, thing_at(g, v, l, me)))) {
    return;
  }

  auto at      = thing_at(g, v, l, me);
  bool got_one = {};

  //
  // Try to teleport a monster at the thrown location first.
  //
  FOR_ALL_THINGS_AT_UNSAFE(g, v, l, it, at)
  {
    if (it == me) {
      continue;
    }

    if (! thing_is_able_to_be_teleported(it)) {
      continue;
    }

    if (thing_is_monst(it)) {
      THING_DBG(g, v, l, it, "teleport monst");
      TRACE_INDENT();

      if (thing_teleport_random(g, v, l, it)) {
        got_one = true;
        break;
      }
    }
  }

  //
  // Try to teleport anything else at the thrown location first.
  //
  if (! got_one) {
    FOR_ALL_THINGS_AT_UNSAFE(g, v, l, it, at)
    {
      if (it == me) {
        continue;
      }

      if (! thing_is_able_to_be_teleported(it)) {
        continue;
      }

      THING_DBG(g, v, l, it, "teleport thing");
      TRACE_INDENT();

      if (thing_teleport_random(g, v, l, it)) {
        break;
      }
    }
  }

  //
  // Try to teleport the potion itself
  //
  THING_DBG(g, v, l, me, "self teleport");
  TRACE_INDENT();

  if (thing_teleport_random(g, v, l, me)) {
    return;
  }

  THING_DBG(g, v, l, me, "did not teleport; land instead");
  TRACE_INDENT();

  ThingEvent e {
      .reason     = "by being thrown",  //
      .event_type = THING_EVENT_THROWN, //
  };

  THING_DBG(g, v, l, me, "dead due to being thrown");
  TRACE_INDENT();

  thing_dead(g, v, l, me, e);
}

static bool tp_pot_disloc_on_use(Gamep g, Levelsp v, Levelp l, Thingp me, Thingp user)
{
  TRACE();

  //
  // Try to teleport the potion itself
  //
  if (! thing_teleport_random(g, v, l, user)) {
    if (thing_is_player(user)) {
      topcon(UI_WARN_FMT_STR "That potion seemed to have no effect." UI_RESET_FMT);
      thing_sound_play(g, v, l, user, "error");
    }
  } else {
    if (thing_is_player(user)) {
      topcon(UI_GOOD_FMT_STR "You feel strangely moved." UI_RESET_FMT);
    }
  }

  return true;
}

static void tp_pot_disloc_on_death(Gamep g, Levelsp v, Levelp l, Thingp me, ThingEvent &e)
{
  TRACE();

  if ((e.event_type == THING_EVENT_CARRIED) ||        //
      (e.event_type == THING_EVENT_CARRIED_MERGED) || //
      (e.event_type == THING_EVENT_USED)) {
    return;
  }

  if (e.source && thing_is_player(e.source)) {
    topcon("The potion shatters.");
  }
  thing_sound_play(g, v, l, me, "glass_shatter");

  if (e.source && thing_is_player(e.source)) {
    (void) thing_noise_incr(g, v, l, me, THING_NOISE_SHATTER);
  }

  (void) thing_spawn(g, v, l, tp_first(is_water_shallow), thing_at(g, v, l, me));
}

[[nodiscard]] auto tp_load_pot_disloc() -> bool
{
  TRACE();

  auto *tp   = tp_load("pot_disloc"); // keep as string for scripts
  auto  name = tp_name(tp);

  // begin sort marker1 {
  thing_description_set(tp, tp_pot_disloc_description_get);
  thing_detail_set(tp, tp_pot_disloc_detail_get);
  thing_on_death_set(tp, tp_pot_disloc_on_death);
  thing_on_thrown_end_set(tp, tp_pot_disloc_on_thrown_end);
  thing_on_use_set(tp, tp_pot_disloc_on_use);
  tp_chance_set(tp, THING_CHANCE_CONTINUE_TO_BURN, "1d2"); // fumble => intensify / keep burning / crit => stop burning
  tp_damage_set(tp, THING_EVENT_THROWN_DAMAGE, "1d4");
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
  tp_flag_set(tp, is_burnable); // is capable of being burned by fire
  tp_flag_set(tp, is_collectable);
  tp_flag_set(tp, is_collision_circle_small);
  tp_flag_set(tp, is_combustible); // will continue to burn once on fire
  tp_flag_set(tp, is_described_cursor);
  tp_flag_set(tp, is_droppable);
  tp_flag_set(tp, is_glass);
  tp_flag_set(tp, is_inventory_item);
  tp_flag_set(tp, is_item_mergeable);
  tp_flag_set(tp, is_item);
  tp_flag_set(tp, is_light_source, 2);
  tp_flag_set(tp, is_loggable);
  tp_flag_set(tp, is_physics_explosion);
  tp_flag_set(tp, is_physics_temperature);
  tp_flag_set(tp, is_physics_trap);
  tp_flag_set(tp, is_physics_water);
  tp_flag_set(tp, is_potion);
  tp_flag_set(tp, is_submergible); // is seen submerged when in water
  tp_flag_set(tp, is_throwable);
  tp_flag_set(tp, is_tick_on_drop);
  tp_flag_set(tp, is_tick_on_use);
  tp_flag_set(tp, is_tickable);
  tp_flag_set(tp, is_treasure);
  tp_flag_set(tp, is_usable);
  tp_health_set(tp, "1d6");
  tp_is_immune_to_add(tp, THING_EVENT_WATER_DAMAGE);
  tp_light_color_set(tp, "white");
  tp_name_a_or_an_set(tp, "a potion of dislocation");
  tp_name_apostrophize_set(tp, "potion of dislocation's");
  tp_name_long_set(tp, "potion of dislocation");
  tp_name_pluralize_set(tp, "potions of dislocation");
  tp_name_short_set(tp, "potion, disloc");
  tp_priority_set(tp, THING_PRIORITY_OBJECT);
  tp_rarity_set(tp, THING_RARITY_COMMON);
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
