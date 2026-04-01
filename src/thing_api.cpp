//
// Copyright goblinhack@gmail.com
//

#include <utility>

#include "my_callstack.hpp"
#include "my_game.hpp"
#include "my_globals.hpp"
#include "my_main.hpp"
#include "my_thing_callbacks.hpp"
#include "my_thing_inlines.hpp"

auto thing_at(Thingp t) -> bpoint
{
  TRACE_DEBUG();

  if (t == nullptr) {
    CROAK("no thing pointer");
  }
  return make_bpoint(t->_at);
}

void thing_at_set(Gamep g, Levelsp v, Levelp l, Thingp t, const bpoint &val)
{
  TRACE_DEBUG();

  if (t == nullptr) {
    CROAK("no thing pointer");
  }

  auto valf = make_fpoint(val);

  if (thing_is_player(t)) {
    if (t->_at != valf) {
      l->request_to_update_per_tile_visibility = true;
    }
  }

  t->_old_at = t->_at;
  t->_at     = valf;
}

auto thing_real_at(Thingp t) -> fpoint
{
  TRACE_DEBUG();

  if (t == nullptr) {
    CROAK("no thing pointer");
  }
  return t->_at;
}

void thing_at_set(Gamep g, Levelsp v, Levelp l, Thingp t, const fpoint &val)
{
  TRACE_DEBUG();

  if (t == nullptr) {
    CROAK("no thing pointer");
  }

  if (thing_is_player(t)) {
    if (t->_at != val) {
      l->request_to_update_per_tile_visibility = true;
    }
  }

  t->_old_at = t->_at;
  t->_at     = val;
}

auto thing_old_at(Thingp t) -> bpoint
{
  TRACE_DEBUG();

  if (t == nullptr) {
    CROAK("no thing pointer");
  }
  return make_bpoint(t->_old_at);
}

auto thing_prev_pix_at(Thingp t) -> spoint
{
  TRACE_DEBUG();

  if (t == nullptr) {
    CROAK("no thing pointer");
  }
  return t->_prev_pix_at;
}

void thing_prev_pix_at_set(Gamep g, Levelsp v, Levelp l, Thingp t, const spoint &val)
{
  TRACE_DEBUG();

  if (t == nullptr) {
    CROAK("no thing pointer");
  }
  t->_prev_pix_at = val;
}

void thing_pix_at_set(Gamep g, Levelsp v, Levelp l, Thingp t, const spoint &val)
{
  TRACE_DEBUG();

  if (t == nullptr) {
    CROAK("no thing pointer");
  }

  if (thing_is_player(t)) {
    if (t->_curr_pix_at != val) {
      l->request_to_update_per_pixel_visibility = true;
    }
  }

  t->_prev_pix_at = t->_curr_pix_at;
  t->_curr_pix_at = val;
}

void thing_pix_at_set(Gamep g, Levelsp v, Levelp l, Thingp t, short x, short y)
{
  TRACE_DEBUG();

  if (t == nullptr) {
    CROAK("no thing pointer");
  }

  spoint const val(x, y);

  if (thing_is_player(t)) {
    if (t->_curr_pix_at != val) {
      l->request_to_update_per_pixel_visibility = true;
    }
  }

  t->_prev_pix_at = t->_curr_pix_at;
  t->_curr_pix_at = val;
}

auto thing_moving_from(Thingp t) -> bpoint
{
  TRACE_DEBUG();

  if (t == nullptr) {
    CROAK("no thing pointer");
  }
  return t->_moving_from;
}

void thing_moving_from_set(Thingp t, const bpoint &val)
{
  TRACE_DEBUG();

  if (t == nullptr) {
    CROAK("no thing pointer");
  }
  t->_moving_from = val;
}

auto thing_speed(Thingp t) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_speed;
}

auto thing_speed_set(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }

  if (std::cmp_greater(val, std::numeric_limits< decltype(t->_speed) >::max())) {
    thing_err(t, "value overflow: %d", val);
    return 0;
  }

  return t->_speed = val;
}

auto thing_weight(Thingp t) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_weight;
}

auto thing_weight_set(Gamep g, Levelsp v, Levelp l, Thingp t, uint32_t val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }

  if (val > std::numeric_limits< decltype(t->_weight) >::max()) {
    thing_err(t, "value overflow: %d", val);
    return 0;
  }

  return t->_weight = val;
}

auto thing_health(Thingp t) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_health;
}

auto thing_health_set(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }

  if (val > std::numeric_limits< decltype(t->_health) >::max()) {
    thing_err(t, "value overflow: %d", val);
    return 0;
  }

  game_request_to_remake_ui_set(g);
  return t->_health = val;
}

auto thing_health_incr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  game_request_to_remake_ui_set(g);
  return t->_health += val;
}

auto thing_health_decr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  game_request_to_remake_ui_set(g);

  if (static_cast< int >(t->_health) - val <= 0) {
    return t->_health = 0;
  }

  return t->_health -= val;
}

void thing_is_falling_set(Gamep g, Levelsp v, Levelp l, Thingp t, bool val)
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return;
  }

  //
  // Once falling, it is treated as a counter
  //
  if (val) {
    //
    // Start falling if not doing do
    //
    if (static_cast< bool >(t->_fall_ms)) {
      return;
    }
  } else {
    //
    // Stop falling
    //
    if (! static_cast< bool >(t->_fall_ms)) {
      return;
    }
  }
  t->_fall_ms = static_cast< uint16_t >(val);

  if (val) {
    thing_on_fall_begin(g, v, l, t);
  } else {
    thing_on_fall_end(g, v, l, t);
  }
}

auto thing_is_falling_incr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }

  if (t->_fall_ms + val > THING_FALL_TIME_MS) {
    return t->_fall_ms = THING_FALL_TIME_MS;
  }

  return t->_fall_ms += val;
}

auto thing_is_falling_continues(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }

  return t->_is_falling_continues;
}

void thing_is_falling_continues_set(Gamep g, Levelsp v, Levelp l, Thingp t, bool val)
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return;
  }

  if (t->_is_falling_continues == static_cast< int >(val)) {
    return;
  }
  t->_is_falling_continues = val;

  if (val) {
    THING_DBG(t, "is scheduled for cleanup");
  }

  level_request_to_cleanup_things_set(g, v, l);
}

void thing_is_falling_continues_unset(Gamep g, Levelsp v, Levelp l, Thingp t)
{
  TRACE_DEBUG();

  thing_is_falling_continues_set(g, v, l, t, false);
}

auto thing_temperature(Thingp t) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_temperature;
}

auto thing_temperature_set(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }

  if (val > std::numeric_limits< decltype(t->_temperature) >::max()) {
    thing_err(t, "value overflow: %d", val);
    return 0;
  }

  if (! thing_is_physics_temperature(t)) {
    return 0;
  }

  //
  // Don't keep on heating up forever!
  //
  auto *tp    = thing_tp(t);
  auto  limit = std::max(tp_temperature_burns_at_get(tp), tp_temperature_melts_at_get(tp));
  if ((limit != 0) && (val > limit)) {
    val = limit;
  }

  IF_DEBUG2
  { //
    THING_DBG(t, "temperature set to %u degrees", val);
  }

  return t->_temperature = val;
}

auto thing_temperature_incr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return thing_temperature_set(g, v, l, t, t->_temperature + val);
}

auto thing_temperature_decr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return thing_temperature_set(g, v, l, t, t->_temperature - val);
}

auto thing_damage_this_tick(Thingp t) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_damage_this_tick;
}

auto thing_damage_this_tick_set(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }

  if (val > std::numeric_limits< decltype(t->_damage_this_tick) >::max()) {
    thing_err(t, "value overflow: %d", val);
    return 0;
  }

  return t->_damage_this_tick = val;
}

auto thing_damage_this_tick_incr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_damage_this_tick += val;
}

auto thing_damage_this_tick_decr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_damage_this_tick -= val;
}

auto thing_keys_carried(Thingp t) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_keys_carried;
}

auto thing_keys_carried_set(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }

  if (val > std::numeric_limits< decltype(t->_keys_carried) >::max()) {
    thing_err(t, "value overflow: %d", val);
    return 0;
  }

  return t->_keys_carried = val;
}

auto thing_keys_carried_incr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_keys_carried += val;
}

auto thing_keys_carried_decr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }

  if (static_cast< int >(t->_keys_carried) - val <= 0) {
    return t->_keys_carried = 0;
  }

  return t->_keys_carried -= val;
}

void thing_is_dead_set(Gamep g, Levelsp v, Levelp l, Thingp t, bool val)
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return;
  }

  if (t->_is_dead == static_cast< int >(val)) {
    return;
  }
  t->_is_dead = val;

  if (val) {
    THING_DBG(t, "is dead");
  }
}

void thing_is_dead_unset(Gamep g, Levelsp v, Levelp l, Thingp t)
{
  TRACE_DEBUG();

  thing_is_dead_set(g, v, l, t, false);
}

auto thing_is_burning(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return t->_is_burning;
}

void thing_is_burning_set(Gamep g, Levelsp v, Levelp l, Thingp t, bool val)
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return;
  }

  if (t->_is_burning == static_cast< int >(val)) {
    return;
  }
  t->_is_burning = val;

  if (val) {
    THING_DBG(t, "is burning, %u degrees", thing_temperature(t));
  } else {
    //
    // Reset the temperature
    //
    auto *tp = thing_tp(t);
    (void) thing_temperature_set(g, v, l, t, tp_temperature_initial_get(tp));

    THING_DBG(t, "is no longer burning, %u degrees", thing_temperature(t));
  }

  thing_is_burning_handle(g, v, l, t);
}

void thing_is_burning_unset(Gamep g, Levelsp v, Levelp l, Thingp t)
{
  TRACE_DEBUG();

  thing_is_burning_set(g, v, l, t, false);
}

auto thing_is_corpse(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return t->_is_corpse;
}

void thing_is_corpse_set(Gamep g, Levelsp v, Levelp l, Thingp t, bool val)
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return;
  }

  if (t->_is_corpse == static_cast< int >(val)) {
    return;
  }
  t->_is_corpse = val;

  if (val) {
    THING_DBG(t, "is corpse");
  }
}

void thing_is_corpse_unset(Gamep g, Levelsp v, Levelp l, Thingp t)
{
  TRACE_DEBUG();

  thing_is_corpse_set(g, v, l, t, false);
}

auto thing_is_scheduled_for_cleanup(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }

  return t->_is_scheduled_for_cleanup;
}

void thing_is_scheduled_for_cleanup_set(Gamep g, Levelsp v, Levelp l, Thingp t, bool val)
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return;
  }

  if (t->_is_scheduled_for_cleanup == static_cast< int >(val)) {
    return;
  }
  t->_is_scheduled_for_cleanup = val;

  if (val) {
    THING_DBG(t, "is scheduled for cleanup");
  }

  level_request_to_cleanup_things_set(g, v, l);
}

void thing_is_scheduled_for_cleanup_unset(Gamep g, Levelsp v, Levelp l, Thingp t)
{
  TRACE_DEBUG();

  thing_is_scheduled_for_cleanup_set(g, v, l, t, false);
}

auto thing_is_on_map(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return t->_is_on_map;
}

void thing_is_on_map_set(Gamep g, Levelsp v, Levelp l, Thingp t, bool val)
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return;
  }
  t->_is_on_map = val;
}

void thing_is_on_map_unset(Gamep g, Levelsp v, Levelp l, Thingp t)
{
  TRACE_DEBUG();

  thing_is_on_map_set(g, v, l, t, false);
}

auto thing_is_animated_can_hflip(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_animated_can_hflip) != 0;
}

auto thing_is_animated_no_dir(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_animated_no_dir) != 0;
}

auto thing_is_animated_sync_first(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_animated_sync_first) != 0;
}

auto thing_is_animated(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_animated) != 0;
}

auto thing_is_barrel(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_barrel) != 0;
}

auto thing_is_blit_centered(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_blit_centered) != 0;
}

auto thing_is_blit_flush_per_line(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_blit_flush_per_line) != 0;
}

auto thing_is_blit_outlined(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_blit_outlined) != 0;
}

auto thing_is_blit_square_outlined(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_blit_square_outlined) != 0;
}

auto thing_is_brazier(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_brazier) != 0;
}

auto thing_is_bridge(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_bridge) != 0;
}

auto thing_is_chasm(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_chasm) != 0;
}

auto thing_is_corridor(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_corridor) != 0;
}

auto thing_is_collision_square(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_collision_square) != 0;
}

auto thing_is_obs_to_cursor_path(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }

  //
  // Unless open
  //
  if (thing_is_openable(t)) {
    if (thing_is_open(t)) {
      return false;
    }
  }

  return tp_flag(thing_tp(t), is_obs_to_cursor_path) != 0;
}

auto thing_is_cursor_path_hazard(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_cursor_path_hazard) != 0;
}

auto thing_is_cursor_path(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_cursor_path) != 0;
}

auto thing_is_cursor(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_cursor) != 0;
}

auto thing_is_deep_water(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_deep_water) != 0;
}

auto thing_is_described_cursor(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_described_cursor) != 0;
}

auto thing_is_dirt(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_dirt) != 0;
}

auto thing_is_door_unlocked(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_door_unlocked) != 0;
}

auto thing_is_dungeon_entrance(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_dungeon_entrance) != 0;
}

auto thing_is_entrance(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_entrance) != 0;
}

auto thing_is_exit(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_exit) != 0;
}

auto thing_is_floor(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_floor) != 0;
}

auto thing_is_foliage(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_foliage) != 0;
}

auto thing_is_ghost(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_ghost) != 0;
}

auto thing_is_kobalos(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_kobalos) != 0;
}

auto thing_is_grass(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_grass) != 0;
}

auto thing_is_key(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_key) != 0;
}

auto thing_is_lava(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_lava) != 0;
}

auto thing_is_level_across_icon(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_level_across_icon) != 0;
}

auto thing_is_level_curr(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_level_curr) != 0;
}

auto thing_is_level_down_icon(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_level_down_icon) != 0;
}

auto thing_is_level_final_icon(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_level_final_icon) != 0;
}

auto thing_is_level_next_icon(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_level_next_icon) != 0;
}

auto thing_is_level_not_visited_icon(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_level_not_visited_icon) != 0;
}

auto thing_is_level_visited_icon(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_level_visited_icon) != 0;
}

auto thing_is_minion(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_minion) != 0;
}

auto thing_is_mob(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_mob) != 0;
}

auto thing_is_mob1(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_mob1) != 0;
}

auto thing_is_mob2(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_mob2) != 0;
}

auto thing_is_monst(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_monst) != 0;
}

auto thing_is_monst_group_easy(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_monst_group_easy) != 0;
}

auto thing_is_monst_group_hard(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_monst_group_hard) != 0;
}

auto thing_is_obs_to_movement(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }

  //
  // Unless open
  //
  if (thing_is_openable(t)) {
    if (thing_is_open(t)) {
      return false;
    }
  }

  return tp_flag(thing_tp(t), is_obs_to_movement) != 0;
}

auto thing_is_pillar(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_pillar) != 0;
}

auto thing_is_rock(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_rock) != 0;
}

auto thing_is_door_secret(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_door_secret) != 0;
}

auto thing_is_slime(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_slime) != 0;
}

auto thing_is_teleport(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_teleport) != 0;
}

auto thing_is_tickable(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_tickable) != 0;
}

auto thing_is_tiled(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_tiled) != 0;
}

auto thing_is_trap(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_trap) != 0;
}

auto thing_is_treasure(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_treasure) != 0;
}

auto thing_is_unused1(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused1) != 0;
}

auto thing_is_unused2(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused2) != 0;
}

auto thing_is_unused3(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused3) != 0;
}

auto thing_is_unused4(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused4) != 0;
}

auto thing_is_unused5(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused5) != 0;
}

auto thing_is_unused6(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused6) != 0;
}

auto thing_is_unused7(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused7) != 0;
}

auto thing_is_unused8(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused8) != 0;
}

auto thing_is_unused9(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused9) != 0;
}

auto thing_is_unused10(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused10) != 0;
}

auto thing_is_unused11(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused11) != 0;
}

auto thing_is_unused12(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused12) != 0;
}

auto thing_is_unused13(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused13) != 0;
}

auto thing_is_unused14(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused14) != 0;
}

auto thing_is_unused15(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused15) != 0;
}

auto thing_is_unused16(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused16) != 0;
}

auto thing_is_unused17(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused17) != 0;
}

auto thing_is_unused18(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused18) != 0;
}

auto thing_is_unused19(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused19) != 0;
}

auto thing_is_unused20(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused20) != 0;
}

auto thing_is_unused21(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused21) != 0;
}

auto thing_is_unused22(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused22) != 0;
}

auto thing_is_unused23(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused23) != 0;
}

auto thing_is_unused24(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused24) != 0;
}

auto thing_is_unused25(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused25) != 0;
}

auto thing_is_unused26(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused26) != 0;
}

auto thing_is_unused27(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused27) != 0;
}

auto thing_is_unused28(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused28) != 0;
}

auto thing_is_unused29(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused29) != 0;
}

auto thing_is_unused30(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused30) != 0;
}

auto thing_is_unused31(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused31) != 0;
}

auto thing_is_unused32(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused32) != 0;
}

auto thing_is_unused33(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused33) != 0;
}

auto thing_is_unused34(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused34) != 0;
}

auto thing_is_unused35(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused35) != 0;
}

auto thing_is_unused36(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused36) != 0;
}

auto thing_is_unused37(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused37) != 0;
}

auto thing_is_unused38(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused38) != 0;
}

auto thing_is_unused39(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused39) != 0;
}

auto thing_is_unused40(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused40) != 0;
}

auto thing_is_unused41(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused41) != 0;
}

auto thing_is_unused42(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused42) != 0;
}

auto thing_is_unused43(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused43) != 0;
}

auto thing_is_unused44(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused44) != 0;
}

auto thing_is_unused45(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused45) != 0;
}

auto thing_is_unused46(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused46) != 0;
}

auto thing_is_unused47(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused47) != 0;
}

auto thing_is_unused48(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused48) != 0;
}

auto thing_is_blit_hit_outline_w_black_inside(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_blit_hit_outline_w_black_inside) != 0;
}

auto thing_is_able_to_fall_sound(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_able_to_fall_sound) != 0;
}

auto thing_is_collision_hit_first_on_tile(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_collision_hit_first_on_tile) != 0;
}

auto thing_is_collision_hit_all_on_tile(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_collision_hit_all_on_tile) != 0;
}

auto thing_is_blit_hit_outline_w_invis_inside(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_blit_hit_outline_w_invis_inside) != 0;
}

auto thing_is_blit_when_obscured_outline(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_blit_when_obscured_outline) != 0;
}

auto thing_is_attackable_by_player(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_attackable_by_player) != 0;
}

auto thing_is_attackable_by_monst(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_attackable_by_monst) != 0;
}

auto thing_is_effect(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_effect) != 0;
}

auto thing_is_effect_attack(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_effect_attack) != 0;
}

auto thing_is_effect_blood(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_effect_blood) != 0;
}

auto thing_is_hit_when_dead(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_hit_when_dead) != 0;
}

auto thing_is_able_to_fire_projectiles(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_able_to_fire_projectiles) != 0;
}

auto thing_is_blitzhound(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_blitzhound) != 0;
}

auto thing_is_unused63(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused63) != 0;
}

auto thing_is_grouped_thing(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_grouped_thing) != 0;
}

auto thing_is_vision_360_degrees(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_vision_360_degrees) != 0;
}

auto thing_is_vision_180_degrees(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_vision_180_degrees) != 0;
}

auto thing_is_able_to_lunge(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_able_to_lunge) != 0;
}

auto thing_is_obs_to_paths(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_obs_to_paths) != 0;
}

auto thing_is_removable_when_dead_on_err(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_removable_when_dead_on_err) != 0;
}

auto thing_is_removable_on_err(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_removable_on_err) != 0;
}

auto thing_is_obs_when_dead(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_obs_when_dead) != 0;
}

auto thing_is_able_to_fall_repeatedly(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_able_to_fall_repeatedly) != 0;
}

auto thing_is_able_to_move_through_walls(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_able_to_move_through_walls) != 0;
}

auto thing_is_able_to_move_diagonally(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_able_to_move_diagonally) != 0;
}

auto thing_is_blit_on_ground(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_blit_on_ground) != 0;
}

auto thing_is_insectoid(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_insectoid) != 0;
}

auto thing_is_mantisman(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_mantisman) != 0;
}

auto thing_is_dmap(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_dmap) != 0;
}

auto thing_is_obs_to_spawning(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_obs_to_spawning) != 0;
}

auto thing_is_mob_kill_minions_on_death(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_mob_kill_minions_on_death) != 0;
}

auto thing_is_border(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_border) != 0;
}

auto thing_is_collision_detection_enabled(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_collision_detection_enabled) != 0;
}

auto thing_is_gold(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_gold) != 0;
}

auto thing_is_glass(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_glass) != 0;
}

auto thing_is_plant(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_plant) != 0;
}

auto thing_is_wood(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_wood) != 0;
}

auto thing_is_metal(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_metal) != 0;
}

auto thing_is_flesh(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_flesh) != 0;
}

auto thing_is_stone(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_stone) != 0;
}

auto thing_is_meltable(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_meltable) != 0;
}

auto thing_is_blit_per_pixel_lighting(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_blit_per_pixel_lighting) != 0;
}

auto thing_is_dead_on_collision(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_dead_on_collision) != 0;
}

auto thing_is_obs_to_jumping_out_of(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_obs_to_jumping_out_of) != 0;
}

auto thing_is_cursor_path_warning(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_cursor_path_warning) != 0;
}

auto thing_is_collision_circle_large(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_collision_circle_large) != 0;
}

auto thing_is_collision_circle_small(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_collision_circle_small) != 0;
}

auto thing_is_fireball(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_fireball) != 0;
}

auto thing_is_projectile(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_projectile) != 0;
}

auto thing_is_health_bar_shown(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_health_bar_shown) != 0;
}

auto thing_is_item_mergeable(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_item_mergeable) != 0;
}

auto thing_is_door_locked(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_door_locked) != 0;
}

auto thing_is_inventory_item(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_inventory_item) != 0;
}

auto thing_is_able_to_open(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_able_to_open) != 0;
}

auto thing_is_item(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_item) != 0;
}

auto thing_is_able_to_crush_grass(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_able_to_crush_grass) != 0;
}

auto thing_is_able_to_collect_keys(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_able_to_collect_keys) != 0;
}

auto thing_is_able_to_collect_items(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_able_to_collect_items) != 0;
}

auto thing_is_collectable(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_collectable) != 0;
}

auto thing_is_blit_when_obscured_faded(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_blit_when_obscured_faded) != 0;
}

auto thing_is_openable(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_openable) != 0;
}

auto thing_is_tick_end_delay(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_tick_end_delay) != 0;
}

auto thing_is_damage_capped(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_damage_capped) != 0;
}

auto thing_is_physics_explosion(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_physics_explosion) != 0;
}

auto thing_is_explosion(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_explosion) != 0;
}

auto thing_is_obs_to_explosion(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }

  //
  // Unless open
  //
  if (thing_is_openable(t)) {
    if (thing_is_open(t)) {
      return false;
    }
  }

  return tp_flag(thing_tp(t), is_obs_to_explosion) != 0;
}

auto thing_is_obs_to_jump_over(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }

  //
  // Unless open
  //
  if (thing_is_openable(t)) {
    if (thing_is_open(t)) {
      return false;
    }
  }

  return tp_flag(thing_tp(t), is_obs_to_jump_over) != 0;
}

auto thing_is_teleport_blocked(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_teleport_blocked) != 0;
}

auto thing_is_cursor_path_none(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_cursor_path_none) != 0;
}

auto thing_is_able_to_jump(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_able_to_jump) != 0;
}

auto thing_is_blit_obscures(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_blit_obscures) != 0;
}

auto thing_is_flying(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_flying) != 0;
}

auto thing_is_needs_move_confirm(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_needs_move_confirm) != 0;
}

auto thing_is_obs_to_falling_onto(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }

  //
  // Unless open
  //
  if (thing_is_openable(t)) {
    if (thing_is_open(t)) {
      return false;
    }
  }

  return tp_flag(thing_tp(t), is_obs_to_falling_onto) != 0;
}

auto thing_is_obs_to_jumping_onto(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }

  //
  // Unless open
  //
  if (thing_is_openable(t)) {
    if (thing_is_open(t)) {
      return false;
    }
  }

  return tp_flag(thing_tp(t), is_obs_to_jumping_onto) != 0;
}

auto thing_is_obs_to_teleporting_onto(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }

  //
  // Unless open
  //
  if (thing_is_openable(t)) {
    if (thing_is_open(t)) {
      return false;
    }
  }

  return tp_flag(thing_tp(t), is_obs_to_teleporting_onto) != 0;
}

auto thing_is_able_to_fall(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_able_to_fall) != 0;
}

auto thing_is_physics_water(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_physics_water) != 0;
}

auto thing_is_indestructible(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_indestructible) != 0;
}

auto thing_is_floating(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_floating) != 0;
}

auto thing_is_obs_to_fire(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }

  //
  // Unless open
  //
  if (thing_is_openable(t)) {
    if (thing_is_open(t)) {
      return false;
    }
  }

  return tp_flag(thing_tp(t), is_obs_to_fire) != 0;
}

auto thing_is_combustible(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_combustible) != 0;
}

auto thing_is_blit_shown_in_chasms(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_blit_shown_in_chasms) != 0;
}

auto thing_is_gaseous(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_gaseous) != 0;
}

auto thing_is_extinguished_on_death(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_extinguished_on_death) != 0;
}

auto thing_is_broken_on_death(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_broken_on_death) != 0;
}

auto thing_is_undead(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_undead) != 0;
}

auto thing_is_wait_on_dead_anim(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_wait_on_dead_anim) != 0;
}

auto thing_is_corpse_on_death(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_corpse_on_death) != 0;
}

auto thing_is_monst_group_mob(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_monst_group_mob) != 0;
}

auto thing_is_blit_if_has_seen(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_blit_if_has_seen) != 0;
}

auto thing_is_smoke(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_smoke) != 0;
}

auto thing_is_steam(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_steam) != 0;
}

auto thing_is_fire(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_fire) != 0;
}

auto thing_is_burnable(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_burnable) != 0;
}

auto thing_is_dead_on_shoving(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_dead_on_shoving) != 0;
}

auto thing_is_able_to_shove(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_able_to_shove) != 0;
}

auto thing_is_shovable(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_shovable) != 0;
}

auto thing_is_loggable(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }

  if (g_opt_tests) {
    //
    // Everything is logged
    //
    return true;
  }

  IF_DEBUG2
  {
    //
    // Everything is logged
    //
    return true;
  }

  return tp_flag(thing_tp(t), is_loggable) != 0;
}

auto thing_is_crushable(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_crushable) != 0;
}

auto thing_is_ethereal(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_ethereal) != 0;
}

auto thing_is_blit_shown_in_overlay(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_blit_shown_in_overlay) != 0;
}

auto thing_is_levitating(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_levitating) != 0;
}

auto thing_is_item_equipable(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_item_equipable) != 0;
}

auto thing_is_item_droppable(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_item_droppable) != 0;
}

auto thing_is_able_to_walk_through_walls(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_able_to_walk_through_walls) != 0;
}

auto thing_is_wall(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_wall) != 0;
}

auto thing_is_water(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_water) != 0;
}

auto thing_value1(Thingp t) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_value1;
}

auto thing_value1_set(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_value1 = val;
}

auto thing_value1_incr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_value1 += val;
}

auto thing_value1_decr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  if (static_cast< int >(t->_value1) - val <= 0) {
    return t->_value1 = 0;
  }
  return t->_value1 -= val;
}
auto thing_value2(Thingp t) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_value2;
}

auto thing_value2_set(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_value2 = val;
}

auto thing_value2_incr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_value2 += val;
}

auto thing_value2_decr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  if (static_cast< int >(t->_value2) - val <= 0) {
    return t->_value2 = 0;
  }
  return t->_value2 -= val;
}
auto thing_value3(Thingp t) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_value3;
}

auto thing_value3_set(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_value3 = val;
}

auto thing_value3_incr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_value3 += val;
}

auto thing_value3_decr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  if (static_cast< int >(t->_value3) - val <= 0) {
    return t->_value3 = 0;
  }
  return t->_value3 -= val;
}
auto thing_value4(Thingp t) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_value4;
}

auto thing_value4_set(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_value4 = val;
}

auto thing_value4_incr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_value4 += val;
}

auto thing_value4_decr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  if (static_cast< int >(t->_value4) - val <= 0) {
    return t->_value4 = 0;
  }
  return t->_value4 -= val;
}
auto thing_value5(Thingp t) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_value5;
}

auto thing_value5_set(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_value5 = val;
}

auto thing_value5_incr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_value5 += val;
}

auto thing_value5_decr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  if (static_cast< int >(t->_value5) - val <= 0) {
    return t->_value5 = 0;
  }
  return t->_value5 -= val;
}
auto thing_value6(Thingp t) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_value6;
}

auto thing_value6_set(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_value6 = val;
}

auto thing_value6_incr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_value6 += val;
}

auto thing_value6_decr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  if (static_cast< int >(t->_value6) - val <= 0) {
    return t->_value6 = 0;
  }
  return t->_value6 -= val;
}
auto thing_value7(Thingp t) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_value7;
}

auto thing_value7_set(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_value7 = val;
}

auto thing_value7_incr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_value7 += val;
}

auto thing_value7_decr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  if (static_cast< int >(t->_value7) - val <= 0) {
    return t->_value7 = 0;
  }
  return t->_value7 -= val;
}
auto thing_value8(Thingp t) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_value8;
}

auto thing_value8_set(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_value8 = val;
}

auto thing_value8_incr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_value8 += val;
}

auto thing_value8_decr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  if (static_cast< int >(t->_value8) - val <= 0) {
    return t->_value8 = 0;
  }
  return t->_value8 -= val;
}
auto thing_value9(Thingp t) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_value9;
}

auto thing_value9_set(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_value9 = val;
}

auto thing_value9_incr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_value9 += val;
}

auto thing_value9_decr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  if (static_cast< int >(t->_value9) - val <= 0) {
    return t->_value9 = 0;
  }
  return t->_value9 -= val;
}
auto thing_value10(Thingp t) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_value10;
}

auto thing_value10_set(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_value10 = val;
}

auto thing_value10_incr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_value10 += val;
}

auto thing_value10_decr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  if (static_cast< int >(t->_value10) - val <= 0) {
    return t->_value10 = 0;
  }
  return t->_value10 -= val;
}
auto thing_value11(Thingp t) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_value11;
}

auto thing_value11_set(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_value11 = val;
}

auto thing_value11_incr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_value11 += val;
}

auto thing_value11_decr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  if (static_cast< int >(t->_value11) - val <= 0) {
    return t->_value11 = 0;
  }
  return t->_value11 -= val;
}
auto thing_value12(Thingp t) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_value12;
}

auto thing_value12_set(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_value12 = val;
}

auto thing_value12_incr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_value12 += val;
}

auto thing_value12_decr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  if (static_cast< int >(t->_value12) - val <= 0) {
    return t->_value12 = 0;
  }
  return t->_value12 -= val;
}
auto thing_value13(Thingp t) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_value13;
}

auto thing_value13_set(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_value13 = val;
}

auto thing_value13_incr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_value13 += val;
}

auto thing_value13_decr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  if (static_cast< int >(t->_value13) - val <= 0) {
    return t->_value13 = 0;
  }
  return t->_value13 -= val;
}
auto thing_value14(Thingp t) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_value14;
}

auto thing_value14_set(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_value14 = val;
}

auto thing_value14_incr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_value14 += val;
}

auto thing_value14_decr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  if (static_cast< int >(t->_value14) - val <= 0) {
    return t->_value14 = 0;
  }
  return t->_value14 -= val;
}
auto thing_value15(Thingp t) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_value15;
}

auto thing_value15_set(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_value15 = val;
}

auto thing_value15_incr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_value15 += val;
}

auto thing_value15_decr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  if (static_cast< int >(t->_value15) - val <= 0) {
    return t->_value15 = 0;
  }
  return t->_value15 -= val;
}
auto thing_value16(Thingp t) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_value16;
}

auto thing_value16_set(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_value16 = val;
}

auto thing_value16_incr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_value16 += val;
}

auto thing_value16_decr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  if (static_cast< int >(t->_value16) - val <= 0) {
    return t->_value16 = 0;
  }
  return t->_value16 -= val;
}
auto thing_value17(Thingp t) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_value17;
}

auto thing_value17_set(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_value17 = val;
}

auto thing_value17_incr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_value17 += val;
}

auto thing_value17_decr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  if (static_cast< int >(t->_value17) - val <= 0) {
    return t->_value17 = 0;
  }
  return t->_value17 -= val;
}
auto thing_value18(Thingp t) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_value18;
}

auto thing_value18_set(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_value18 = val;
}

auto thing_value18_incr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_value18 += val;
}

auto thing_value18_decr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  if (static_cast< int >(t->_value18) - val <= 0) {
    return t->_value18 = 0;
  }
  return t->_value18 -= val;
}
auto thing_value19(Thingp t) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_value19;
}

auto thing_value19_set(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_value19 = val;
}

auto thing_value19_incr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_value19 += val;
}

auto thing_value19_decr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  if (static_cast< int >(t->_value19) - val <= 0) {
    return t->_value19 = 0;
  }
  return t->_value19 -= val;
}

auto thing_minion_max(Thingp t) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_minion_max;
}

auto thing_minion_max_set(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }

  if (val > THING_MINION_MAX) {
    thing_croak(t, "trying to set minion max too high");
    val = THING_MINION_MAX;
  }

  return t->_minion_max = val;
}

auto thing_minion_max_incr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }

  if (t->_minion_max + val >= THING_MINION_MAX) {
    return t->_minion_max = THING_MINION_MAX;
  }

  return t->_minion_max += val;
}

auto thing_minion_max_decr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  if (static_cast< int >(t->_minion_max) - val <= 0) {
    return t->_minion_max = 0;
  }
  return t->_minion_max -= val;
}

auto thing_projectile_max(Thingp t) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_projectile_max;
}

auto thing_projectile_max_set(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }

  if (val > THING_PROJECTILE_MAX) {
    thing_err(t, "trying to set projectile max too high");
    val = THING_PROJECTILE_MAX;
  }

  return t->_projectile_max = val;
}

auto thing_projectile_max_incr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }

  if (t->_projectile_max + val >= THING_PROJECTILE_MAX) {
    return t->_projectile_max = THING_PROJECTILE_MAX;
  }

  return t->_projectile_max += val;
}

auto thing_projectile_max_decr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  if (static_cast< int >(t->_projectile_max) - val <= 0) {
    return t->_projectile_max = 0;
  }
  return t->_projectile_max -= val;
}

auto thing_distance_minion_from_mob_max(Thingp t) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_distance_minion_from_mob_max;
}

auto thing_distance_minion_from_mob_max_set(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_distance_minion_from_mob_max = val;
}

auto thing_distance_vision(Thingp t) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_distance_vision;
}

auto thing_distance_vision_set(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_distance_vision = val;
}

auto thing_distance_vision_incr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_distance_vision += val;
}

auto thing_distance_vision_decr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  if (static_cast< int >(t->_distance_vision) - val <= 0) {
    return t->_distance_vision = 0;
  }
  return t->_distance_vision -= val;
}

auto thing_variant(Thingp t) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_variant;
}

auto thing_variant_set(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_variant = val;
}

auto thing_priority(Thingp t) -> ThingPriorityType
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return THING_PRIORITY_LOWEST;
  }
  if (thing_is_dead(t)) {
    return THING_PRIORITY_DEAD;
  }
  return t->_priority;
}

auto thing_priority_set(Gamep g, Levelsp v, Levelp l, Thingp t, ThingPriorityType val) -> ThingPriorityType
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return THING_PRIORITY_LOWEST;
  }
  return t->_priority = val;
}

auto thing_distance_jump(Thingp t) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_distance_jump;
}

auto thing_distance_jump_set(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_distance_jump = val;
}

auto thing_distance_jump_incr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_distance_jump += val;
}

auto thing_distance_jump_decr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  if (static_cast< int >(t->_distance_jump) - val <= 0) {
    return t->_distance_jump = 0;
  }
  return t->_distance_jump -= val;
}

auto thing_lifespan(Thingp t) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_lifespan;
}

auto thing_lifespan_set(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_lifespan = val;
}

auto thing_lifespan_incr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_lifespan += val;
}

auto thing_lifespan_decr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  if (static_cast< int >(t->_lifespan) - val <= 0) {
    return t->_lifespan = 0;
  }
  return t->_lifespan -= val;
}

auto thing_age(Thingp t) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_age;
}

auto thing_age_set(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_age = val;
}

auto thing_age_incr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_age += val;
}

auto thing_age_decr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  if (t->_age - val <= 0) {
    return t->_age = 0;
  }
  return t->_age -= val;
}

auto thing_move_remaining(Thingp t) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_move_remaining;
}

auto thing_move_remaining_set(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_move_remaining = val;
}

auto thing_move_remaining_incr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_move_remaining += val;
}

auto thing_move_remaining_decr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  if (t->_move_remaining - val <= 0) {
    return t->_move_remaining = 0;
  }
  return t->_move_remaining -= val;
}

auto thing_collision_radius(Thingp t) -> float
{
  TRACE_DEBUG();

  return thing_is_collision_circle_small(t) ? THING_COLLISION_CIRCLE_SMALL_RADIUS : THING_COLLISION_CIRCLE_LARGE_RADIUS;
}

auto thing_target(Thingp t) -> bpoint
{
  TRACE_DEBUG();

  if (t == nullptr) {
    CROAK("no thing pointer");
  }

  return t->_target;
}

void thing_target_set(Gamep g, Levelsp v, Levelp l, Thingp t, const bpoint &val)
{
  TRACE_DEBUG();

  if (t == nullptr) {
    CROAK("no thing pointer");
  }

  t->_target = val;
}
