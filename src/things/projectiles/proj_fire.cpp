//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_dice_rolls.hpp"
#include "my_globals.hpp"
#include "my_level.hpp"
#include "my_thing_callbacks.hpp"
#include "my_tile.hpp"
#include "my_tp.hpp"
#include "my_tps.hpp"
#include "my_types.hpp"

static void tp_proj_fire_on_spawned(Gamep g, Levelsp v, Levelp l, Thingp me, ThingEvent *e_maybe_null)
{
  TRACE();

  thing_sound_play(g, v, l, me, "projectile");
}

static void tp_proj_fire_on_death(Gamep g, Levelsp v, Levelp l, Thingp me, ThingEvent &e)
{
  TRACE();

  thing_sound_play(g, v, l, me, "explosion");
}

static void tp_proj_fire_on_moved(Gamep g, Levelsp v, Levelp l, Thingp me)
{
  TRACE();

  //
  // The proj_fire doesn't heat water up quick enough, so add this cheap effect
  //
  if (level_is_water_shallow_bool(g, v, l, thing_at(g, v, l, me))) {
    if (! level_is_steam_bool(g, v, l, thing_at(g, v, l, me))) {
      if (d100() < 50) {
        (void) thing_spawn(g, v, l, tp_first(is_steam), thing_at(g, v, l, me));
      }
    }
  }
}

[[nodiscard]] auto tp_load_proj_fire() -> bool
{
  auto *tp   = tp_load("proj_fire"); // keep as string for scripts
  auto  name = tp_name(tp);

  // begin sort marker1 {
  thing_on_death_set(tp, tp_proj_fire_on_death);
  thing_on_moved_set(tp, tp_proj_fire_on_moved);
  thing_on_spawned_set(tp, tp_proj_fire_on_spawned);
  tp_damage_set(tp, THING_EVENT_FIRE_DAMAGE, "1d4");
  tp_flag_set(tp, is_able_to_be_teleported);
  tp_flag_set(tp, is_animated);
  tp_flag_set(tp, is_blit_centered);
  tp_flag_set(tp, is_blit_if_has_seen);
  tp_flag_set(tp, is_blit_shown_in_chasms);
  tp_flag_set(tp, is_collision_circle_small);
  tp_flag_set(tp, is_collision_hit_first_on_tile);
  tp_flag_set(tp, is_dead_on_collision);
  tp_flag_set(tp, is_gaseous);
  tp_flag_set(tp, is_light_source, 2);
  tp_flag_set(tp, is_loggable);
  tp_flag_set(tp, is_physics_temperature);
  tp_flag_set(tp, is_projectile);
  tp_flag_set(tp, is_removable_on_err);
  tp_flag_set(tp, is_tickable);
  tp_health_set(tp, "1");
  tp_light_color_set(tp, "orange");
  tp_name_a_or_an_set(tp, "a ball of fire");
  tp_name_apostrophize_set(tp, "ball of fire's");
  tp_name_long_set(tp, "ball of fire");
  tp_name_pluralize_set(tp, "balls of fire");
  tp_name_short_set(tp, "ball of fire");
  tp_priority_set(tp, THING_PRIORITY_WEAPON);
  tp_speed_set(tp, 800);
  tp_temperature_initial_set(tp, 500); // celsius
  tp_weight_set(tp, WEIGHT_NONE);      // grams
  tp_z_depth_set(tp, MAP_Z_DEPTH_WEAPON);
  // end sort marker1 }

  auto delay = 200;

  for (auto frame = 0; frame < 2; frame++) {
    auto *tile = tile_find_mand(name + "." + std::to_string(frame));
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tile_delay_ms_set(tile, delay);
    tp_tiles_push_back(tp, THING_ANIM_IDLE, tile);
  }

  return true;
}
