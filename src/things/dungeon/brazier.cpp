//
// Copyright goblinhack@gmail.com
//

#include "../../my_callstack.hpp"
#include "../../my_level.hpp"
#include "../../my_main.hpp"
#include "../../my_thing.hpp"
#include "../../my_thing_callbacks.hpp"
#include "../../my_thing_inlines.hpp"
#include "../../my_tile.hpp"
#include "../../my_tp.hpp"
#include "../../my_tps.hpp"
#include "../../my_types.hpp"

static auto tp_brazier_description_get(Gamep g, Levelsp v, Levelp l, Thingp me) -> std::string
{
  TRACE();

  if (thing_is_dead(me)) {
    return "extinguished brazier";
  }

  return "brightly burning brazier";
}

static void tp_brazier_on_shoved(Gamep g, Levelsp v, Levelp l, Thingp me, Thingp shover)
{
  TRACE();

  //
  // If extinguished, do not generate more smoke
  //
  if (thing_is_dead(me)) {
    //
    // Only if recently dead
    //
    if (v->tick - me->tick_dead < 5) {
      if (! level_is_smoke_bool(g, v, l, thing_at(g, v, l, me))) {
        (void) thing_spawn(g, v, l, tp_first(is_smoke), me);
      }
    }
    return;
  }

  //
  // Attempt to spawn fire in the direction of shoving
  //
  if (shover != nullptr) {
    auto at        = thing_at(g, v, l, me);
    auto direction = at - thing_at(g, v, l, shover);
    auto fire_at   = at + direction;

    if (level_is_obs_to_fire(g, v, l, fire_at) != nullptr) {
      //
      // If we can't, then spawn over the brazier
      //
      if (! level_is_fire_bool(g, v, l, thing_at(g, v, l, me))) {
        (void) thing_spawn(g, v, l, tp_first(is_fire_normal), me);
      }
    } else {
      if (! level_is_fire_bool(g, v, l, fire_at)) {
        (void) thing_spawn(g, v, l, tp_first(is_fire_normal), fire_at);
      }

      if (! level_is_smoke_bool(g, v, l, thing_at(g, v, l, me))) {
        (void) thing_spawn(g, v, l, tp_first(is_smoke), me);
      }
    }
  } else {
    //
    // Spawn over the brazier
    //
    if (! level_is_fire_bool(g, v, l, thing_at(g, v, l, me))) {
      (void) thing_spawn(g, v, l, tp_first(is_fire_normal), me);
    }
  }
}

static void tp_brazier_on_death(Gamep g, Levelsp v, Levelp l, Thingp me, ThingEvent &e)
{
  TRACE();

  //
  // Allow things to continue to burn if we still have some burnable material
  //
  if (e.event_type != THING_EVENT_SHOVED) {
    if (! level_is_fire_bool(g, v, l, thing_at(g, v, l, me))) {
      (void) thing_spawn(g, v, l, tp_first(is_fire_normal), me);
    }
  }

  if (! level_is_smoke_bool(g, v, l, thing_at(g, v, l, me))) {
    (void) thing_spawn(g, v, l, tp_first(is_smoke), me);
  }

  thing_sound_play(g, v, l, me, "brazier");

  auto *player = thing_player(g);
  if (player != nullptr) {
    THING_DBG(g, v, l, me, "brazier is dead");
    if (thing_on_same_level_as_player(g, v, me)) {
      auto at = thing_at(g, v, l, me);
      if (thing_vision_can_see_tile(g, v, l, player, at)) {
        topcon("The brazier falls over!");
      } else {
        topcon("You hear a distant clatter!");
      }
    } else {
      topcon("You hear a very distant clatter!");
    }
  }
}

[[nodiscard]] auto tp_load_brazier() -> bool
{
  TRACE();

  auto *tp   = tp_load("brazier"); // keep as string for scripts
  auto  name = tp_name(tp);

  // begin sort marker1 {
  thing_description_set(tp, tp_brazier_description_get);
  thing_on_death_set(tp, tp_brazier_on_death);
  thing_on_shoved_set(tp, tp_brazier_on_shoved);
  tp_flag_set(tp, is_able_to_be_invisible);
  tp_flag_set(tp, is_able_to_be_levitated);
  tp_flag_set(tp, is_able_to_be_teleported);
  tp_flag_set(tp, is_able_to_fall_sound);
  tp_flag_set(tp, is_able_to_fall);
  tp_flag_set(tp, is_animated);
  tp_flag_set(tp, is_blit_centered);
  tp_flag_set(tp, is_blit_hit_outline_w_invis_inside);
  tp_flag_set(tp, is_blit_shown_in_chasms);
  tp_flag_set(tp, is_brazier);
  tp_flag_set(tp, is_collision_circle_large);
  tp_flag_set(tp, is_corpse_on_death);
  tp_flag_set(tp, is_cursor_path_hazard);
  tp_flag_set(tp, is_dead_on_shoving);
  tp_flag_set(tp, is_described_cursor);
  tp_flag_set(tp, is_extinguished_on_death);
  tp_flag_set(tp, is_light_flicker);
  tp_flag_set(tp, is_light_source, 4);
  tp_flag_set(tp, is_loggable);
  tp_flag_set(tp, is_metal);
  tp_flag_set(tp, is_needs_move_confirm);
  tp_flag_set(tp, is_obs_to_falling_onto);
  tp_flag_set(tp, is_obs_to_jumping_onto);
  tp_flag_set(tp, is_obs_to_movement);
  tp_flag_set(tp, is_obs_to_paths);
  tp_flag_set(tp, is_obs_to_spawning);
  tp_flag_set(tp, is_obs_to_teleporting_onto);
  tp_flag_set(tp, is_obs_to_wall_walker);
  tp_flag_set(tp, is_obs_when_dead);
  tp_flag_set(tp, is_physics_explosion);
  tp_flag_set(tp, is_physics_trap);
  tp_flag_set(tp, is_physics_water);
  tp_flag_set(tp, is_shovable);
  tp_flag_set(tp, is_submergible); // is seen submerged when in water
  tp_flag_set(tp, is_tickable);
  tp_light_color_set(tp, "orange");
  tp_name_a_or_an_set(tp, "a brazier");
  tp_name_apostrophize_set(tp, "braziers'");
  tp_name_long_set(tp, "brazier");
  tp_name_pluralize_set(tp, "braziers");
  tp_name_short_set(tp, "brazier");
  tp_priority_set(tp, THING_PRIORITY_BRAZIER);
  tp_weight_set(tp, WEIGHT_MEDIUM); // grams
  tp_z_depth_set(tp, MAP_Z_DEPTH_OBJ);
  // end sort marker1 }

  auto delay = 200;

  for (auto frame = 0; frame < 31; frame++) {
    auto *tile = tile_find_mand(name + std::string(".idle.") + std::to_string(frame));
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tile_delay_ms_set(tile, delay);
    tp_tiles_push_back(tp, THING_ANIM_IDLE, tile);
  }

  for (auto frame = 0; frame < 1; frame++) {
    auto *tile = tile_find_mand(name + std::string(".dead.") + std::to_string(frame));
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tile_delay_ms_set(tile, delay);
    tp_tiles_push_back(tp, THING_ANIM_DEAD, tile);
  }

  return true;
}
