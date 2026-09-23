//
// Copyright goblinhack@gmail.com
//

#include "../../my_callstack.hpp"
#include "../../my_level.hpp"
#include "../../my_main.hpp"
#include "../../my_thing_callbacks.hpp"
#include "../../my_tile.hpp"
#include "../../my_tp.hpp"
#include "../../my_tps.hpp"
#include "../../my_types.hpp"
#include "../../my_ui.hpp"

static auto tp_barrel_description_get(Gamep g, Levelsp v, Levelp l, Thingp me) -> std::string
{
  TRACE();

  return "barrel of oil";
}

static auto tp_barrel_detail_get(Gamep g, Levelsp v, Levelp l, Thingp me) -> std::string
{
  TRACE();

  return UI_INFO1_FMT_STR "A not out of place in a dungeon, barrel of oil.";
}

static void tp_barrel_spawn_explosion(Gamep g, Levelsp v, Levelp l, Thingp me)
{
  TRACE();

  const std::initializer_list< bpoint > points = {
      bpoint(-1, -1), bpoint(1, -1), bpoint(0, -1), bpoint(-1, 0), bpoint(1, 0), bpoint(0, 0), bpoint(-1, 1), bpoint(1, 1), bpoint(0, 1),
  };

  auto at = thing_at(g, v, l, me);

  for (auto delta : points) {
    auto p = at + delta;
    if (level_is_obs_to_explosion(g, v, l, p) == nullptr) {
      if (! level_is_explosion_major_bool(g, v, l, p)) {
        (void) thing_spawn(g, v, l, tp_first(is_explosion_major), p);
      }
    }
  }

  auto *player = thing_player(g);
  if (player != nullptr) {
    if (thing_on_same_level_as_player(g, v, me)) {
      if (thing_vision_can_see_tile(g, v, l, player, at)) {
        topcon(UI_IMPORTANT_FMT_STR "The barrel explodes!" UI_RESET_FMT);
      } else {
        topcon(UI_WARN_FMT_STR "You hear a distant explosion!" UI_RESET_FMT);
      }
    } else {
      topcon("You hear a very muffled explosion!");
    }
  }
}

static void tp_barrel_on_death(Gamep g, Levelsp v, Levelp l, Thingp me, ThingEvent &e)
{
  TRACE();

  tp_barrel_spawn_explosion(g, v, l, me);
}

static void tp_barrel_on_fall_end(Gamep g, Levelsp v, Levelp l, Thingp me)
{
  TRACE();

  //
  // If we fell into another chasm, don't kill the thing yet
  //
  if (level_is_chasm_bool(g, v, l, thing_at(g, v, l, me))) {
    return;
  }

  ThingEvent e {
      .reason     = "by barrel falling", //
      .event_type = THING_EVENT_FALL,    //
  };

  THING_DBG(g, v, l, me, "dead due to fall end");
  TRACE_INDENT();

  thing_dead(g, v, l, me, e);
}

[[nodiscard]] auto tp_load_barrel() -> bool
{
  TRACE();

  auto *tp   = tp_load("barrel"); // keep as string for scripts
  auto  name = tp_name(tp);

  // begin sort marker1 {
  thing_description_set(tp, tp_barrel_description_get);
  thing_detail_set(tp, tp_barrel_detail_get);
  thing_on_death_set(tp, tp_barrel_on_death);
  thing_on_fall_end_set(tp, tp_barrel_on_fall_end);
  tp_chance_set(tp, THING_CHANCE_CONTINUE_TO_BURN, "1d2"); // fumble => intensify / keep burning / crit => stop burning
  tp_chance_set(tp, THING_CHANCE_START_BURNING, "1d2");    // fumble => flames spread to you
  tp_distance_light_penetration_pixels_set(tp, TILE_WIDTH / 2);
  tp_flag_set(tp, is_able_to_be_invisible);
  tp_flag_set(tp, is_able_to_be_levitated);
  tp_flag_set(tp, is_able_to_be_teleported);
  tp_flag_set(tp, is_able_to_fall_sound);
  tp_flag_set(tp, is_able_to_fall);
  tp_flag_set(tp, is_animated);
  tp_flag_set(tp, is_barrel);
  tp_flag_set(tp, is_blit_hit_outline_w_black_inside);
  tp_flag_set(tp, is_blit_if_has_seen);
  tp_flag_set(tp, is_blit_on_ground);
  tp_flag_set(tp, is_blit_shown_in_chasms);
  tp_flag_set(tp, is_broken_on_death);
  tp_flag_set(tp, is_burnable); // is capable of being burned by fire
  tp_flag_set(tp, is_collision_circle_large);
  tp_flag_set(tp, is_combustible); // will continue to burn once on fire
  tp_flag_set(tp, is_described_cursor);
  tp_flag_set(tp, is_loggable);
  tp_flag_set(tp, is_obs_to_beam);
  tp_flag_set(tp, is_obs_to_cursor_path);
  tp_flag_set(tp, is_obs_to_falling_onto);
  tp_flag_set(tp, is_obs_to_jumping_onto);
  tp_flag_set(tp, is_obs_to_movement);
  tp_flag_set(tp, is_obs_to_paths);
  tp_flag_set(tp, is_obs_to_spawning);
  tp_flag_set(tp, is_obs_to_teleporting_onto);
  tp_flag_set(tp, is_obs_to_wall_walker);
  tp_flag_set(tp, is_obs_to_vision);
  tp_flag_set(tp, is_physics_explosion);
  tp_flag_set(tp, is_physics_temperature);
  tp_flag_set(tp, is_physics_trap);
  tp_flag_set(tp, is_shovable);
  tp_flag_set(tp, is_submergible); // is seen submerged when in water
  tp_flag_set(tp, is_tickable);
  tp_flag_set(tp, is_wood);
  tp_health_set(tp, "1");
  tp_is_immune_to_add(tp, THING_EVENT_WATER_DAMAGE);
  tp_name_a_or_an_set(tp, "a barrel");
  tp_name_apostrophize_set(tp, "barrels'");
  tp_name_long_set(tp, "barrel");
  tp_name_pluralize_set(tp, "barrels");
  tp_name_short_set(tp, "barrel");
  tp_priority_set(tp, THING_PRIORITY_OBJECT);
  tp_temperature_burns_at_set(tp, 50);  // celsius
  tp_temperature_damage_at_set(tp, 50); // celsius
  tp_temperature_initial_set(tp, 20);   // celsius
  tp_weight_set(tp, WEIGHT_HEAVY);      // grams
  tp_z_depth_set(tp, MAP_Z_DEPTH_OBJ);
  // end sort marker1 }

  auto delay = 200;

  for (auto frame = 0; frame < 1; frame++) {
    auto *tile = tile_find_mand(name + std::string(".idle.") + std::to_string(frame));
    tile_delay_ms_set(tile, delay);
    tp_tiles_push_back(tp, THING_ANIM_IDLE, tile);
    tile_size_set(tile, OUTLINE_TILE_WIDTH, OUTLINE_TILE_HEIGHT * 2);
  }

  return true;
}
