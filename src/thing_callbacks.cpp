//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_globals.hpp"
#include "my_main.hpp"
#include "my_thing_callbacks.hpp"
#include "my_thing_inlines.hpp"
#include "my_tp_class.hpp"

void thing_description_set(Tpp tp, thing_description_get_t callback)
{
  TRACE();
  if (tp == nullptr) [[unlikely]] {
    err("no thing template pointer");
    return;
  }
  tp->description_get = callback;
}

auto thing_description_get(Gamep g, Levelsp v, Levelp l, Thingp me) -> std::string
{
  TRACE();
  auto *tp = thing_tp(me);
  if (tp == nullptr) [[unlikely]] {
    err("no thing template pointer");
    return "<no description>";
  }
  if (tp->description_get == nullptr) {
    return "";
  }
  return tp->description_get(g, v, l, me);
}

void thing_detail_set(Tpp tp, thing_detail_get_t callback)
{
  TRACE();
  if (tp == nullptr) [[unlikely]] {
    err("no thing template pointer");
    return;
  }
  tp->detail_get = callback;
}

auto thing_detail_get(Gamep g, Levelsp v, Levelp l, Thingp me) -> std::string
{
  TRACE();
  auto *tp = thing_tp(me);
  if (tp == nullptr) [[unlikely]] {
    err("no thing template pointer");
    return "<no detail>";
  }
  if (tp->detail_get == nullptr) {
    return "";
  }
  return tp->detail_get(g, v, l, me);
}

void thing_z_depth_set(Tpp tp, thing_z_depth_get_t callback)
{
  TRACE();
  if (tp == nullptr) [[unlikely]] {
    err("no thing template pointer");
    return;
  }
  tp->z_depth_get = callback;
}

auto thing_z_depth_get(Gamep g, Levelsp v, Levelp l, Thingp me) -> MapZDepth
{
  TRACE();
  auto *tp = thing_tp(me);
  if (tp == nullptr) [[unlikely]] {
    err("no thing template pointer");
    return MAP_Z_DEPTH_FLOOR;
  }
  if (tp->z_depth_get == nullptr) {
    return tp_z_depth_get(tp);
  }
  return tp->z_depth_get(g, v, l, me);
}

void thing_mouse_down_set(Tpp tp, thing_mouse_down_t callback)
{
  TRACE();
  if (tp == nullptr) [[unlikely]] {
    err("no thing template pointer");
    return;
  }
  tp->mouse_down = callback;
}

auto thing_mouse_down(Gamep g, Levelsp v, Levelp l, Thingp me, int x, int y, int button) -> bool
{
  TRACE();
  auto *tp = thing_tp(me);
  if (tp == nullptr) [[unlikely]] {
    err("no thing template pointer");
    return false;
  }
  if (tp->mouse_down == nullptr) {
    return false;
  }
  return tp->mouse_down(g, v, l, me, x, y, button);
}

void thing_on_tick_idle_set(Tpp tp, thing_on_tick_idle_t callback)
{
  TRACE();
  if (tp == nullptr) [[unlikely]] {
    err("no thing template pointer");
    return;
  }
  tp->tick_idle = callback;
}

void thing_on_tick_idle(Gamep g, Levelsp v, Levelp l, Thingp me)
{
  TRACE();
  auto *tp = thing_tp(me);
  if (tp == nullptr) [[unlikely]] {
    err("no thing template pointer");
    return;
  }
  if (tp->tick_idle == nullptr) {
    return;
  }
  tp->tick_idle(g, v, l, me);
}

void thing_on_tick_begin_set(Tpp tp, thing_on_tick_begin_t callback)
{
  TRACE();
  if (tp == nullptr) [[unlikely]] {
    err("no thing template pointer");
    return;
  }
  tp->tick_begin = callback;
}

void thing_on_tick_begin(Gamep g, Levelsp v, Levelp l, Thingp me)
{
  TRACE();
  auto *tp = thing_tp(me);
  if (tp == nullptr) [[unlikely]] {
    err("no thing template pointer");
    return;
  }
  if (tp->tick_begin == nullptr) {
    return;
  }
  tp->tick_begin(g, v, l, me);
}

void thing_on_tick_end_set(Tpp tp, thing_on_tick_end_t callback)
{
  TRACE();
  if (tp == nullptr) [[unlikely]] {
    err("no thing template pointer");
    return;
  }
  tp->tick_end = callback;
}

void thing_on_tick_end(Gamep g, Levelsp v, Levelp l, Thingp me)
{
  TRACE();
  auto *tp = thing_tp(me);
  if (tp == nullptr) [[unlikely]] {
    err("no thing template pointer");
    return;
  }
  if (tp->tick_end == nullptr) {
    return;
  }
  tp->tick_end(g, v, l, me);
}

void thing_on_spawned_set(Tpp tp, thing_on_spawned_t callback)
{
  TRACE();
  if (tp == nullptr) [[unlikely]] {
    err("no thing template pointer");
    return;
  }
  tp->on_spawned = callback;
}

void thing_on_spawned(Gamep g, Levelsp v, Levelp l, Thingp me)
{
  TRACE();
  auto *tp = thing_tp(me);
  if (tp == nullptr) [[unlikely]] {
    err("no thing template pointer");
    return;
  }
  if (tp->on_spawned == nullptr) {
    return;
  }
  tp->on_spawned(g, v, l, me);
}

void thing_on_level_populated_set(Tpp tp, thing_on_level_populated_t callback)
{
  TRACE();
  if (tp == nullptr) [[unlikely]] {
    err("no thing template pointer");
    return;
  }
  tp->on_level_populated = callback;
}

void thing_on_level_populated(Gamep g, Levelsp v, Levelp l, Thingp me)
{
  TRACE();
  auto *tp = thing_tp(me);
  if (tp == nullptr) [[unlikely]] {
    err("no thing template pointer");
    return;
  }
  if (tp->on_level_populated == nullptr) {
    return;
  }
  tp->on_level_populated(g, v, l, me);
}

void thing_on_level_enter_set(Tpp tp, thing_on_level_enter_t callback)
{
  TRACE();
  if (tp == nullptr) [[unlikely]] {
    err("no thing template pointer");
    return;
  }
  tp->on_level_enter = callback;
}

void thing_on_level_enter(Gamep g, Levelsp v, Levelp l, Thingp me)
{
  TRACE();
  auto *tp = thing_tp(me);
  if (tp == nullptr) [[unlikely]] {
    err("no thing template pointer");
    return;
  }
  if (tp->on_level_enter == nullptr) {
    return;
  }
  tp->on_level_enter(g, v, l, me);
}

void thing_on_level_leave_set(Tpp tp, thing_on_level_leave_t callback)
{
  TRACE();
  if (tp == nullptr) [[unlikely]] {
    err("no thing template pointer");
    return;
  }
  tp->on_level_leave = callback;
}

void thing_on_level_leave(Gamep g, Levelsp v, Levelp l, Thingp me)
{
  TRACE();
  auto *tp = thing_tp(me);
  if (tp == nullptr) [[unlikely]] {
    err("no thing template pointer");
    return;
  }
  if (tp->on_level_leave == nullptr) {
    return;
  }
  tp->on_level_leave(g, v, l, me);
}

void thing_on_open_request_set(Tpp tp, thing_on_open_request_t callback)
{
  TRACE();
  if (tp == nullptr) [[unlikely]] {
    err("no thing template pointer");
    return;
  }
  tp->on_open_request = callback;
}

auto thing_on_open_request(Gamep g, Levelsp v, Levelp l, Thingp me, Thingp opener) -> bool
{
  TRACE();
  auto *tp = thing_tp(me);
  if (tp == nullptr) [[unlikely]] {
    err("no thing template pointer");
    return false;
  }
  if (tp->on_open_request == nullptr) {
    //
    // Assume success
    //
    return true;
  }
  if (! thing_is_player(opener) && ! thing_is_monst(opener)) {
    thing_err(opener, "unexpected thing for %s", __FUNCTION__);
    return false;
  }
  auto ret = tp->on_open_request(g, v, l, me, opener);
  if (ret) {
    THING_DBG(me, "opens");
  } else {
    THING_DBG(me, "fails to open");
  }
  return ret;
}

void thing_on_close_request_set(Tpp tp, thing_on_close_request_t callback)
{
  TRACE();
  if (tp == nullptr) [[unlikely]] {
    err("no thing template pointer");
    return;
  }
  tp->on_close_request = callback;
}

auto thing_on_close_request(Gamep g, Levelsp v, Levelp l, Thingp me, Thingp closer) -> bool
{
  TRACE();
  auto *tp = thing_tp(me);
  if (tp == nullptr) [[unlikely]] {
    err("no thing template pointer");
    return false;
  }
  if (tp->on_close_request == nullptr) {
    //
    // Assume success
    //
    return true;
  }
  if (! thing_is_player(closer) && ! thing_is_monst(closer)) {
    thing_err(closer, "unexpected thing for %s", __FUNCTION__);
    return false;
  }
  auto ret = tp->on_close_request(g, v, l, me, closer);
  if (ret) {
    THING_DBG(me, "closes");
  } else {
    THING_DBG(me, "fails to close");
  }
  return ret;
}

void thing_on_carry_request_set(Tpp tp, thing_on_carry_request_t callback)
{
  TRACE();
  if (tp == nullptr) [[unlikely]] {
    err("no thing template pointer");
    return;
  }
  tp->on_carry_request = callback;
}

auto thing_on_carry_request(Gamep g, Levelsp v, Levelp l, Thingp me, Thingp carrier) -> bool
{
  TRACE();
  auto *tp = thing_tp(me);
  if (tp == nullptr) [[unlikely]] {
    err("no thing template pointer");
    return false;
  }
  if (tp->on_carry_request == nullptr) {
    //
    // Assume success
    //
    return true;
  }
  if (! thing_is_player(carrier) && ! thing_is_monst(carrier)) {
    thing_err(carrier, "unexpected thing for %s", __FUNCTION__);
    return false;
  }
  return tp->on_carry_request(g, v, l, me, carrier);
}

void thing_on_drop_request_set(Tpp tp, thing_on_drop_request_t callback)
{
  TRACE();
  if (tp == nullptr) [[unlikely]] {
    err("no thing template pointer");
    return;
  }
  tp->on_drop_request = callback;
}

auto thing_on_drop_request(Gamep g, Levelsp v, Levelp l, Thingp me, Thingp dropper) -> bool
{
  TRACE();
  auto *tp = thing_tp(me);
  if (tp == nullptr) [[unlikely]] {
    err("no thing template pointer");
    return false;
  }
  if (tp->on_drop_request == nullptr) {
    //
    // Assume success
    //
    return true;
  }
  if (! thing_is_player(dropper) && ! thing_is_monst(dropper)) {
    thing_err(dropper, "unexpected thing for %s", __FUNCTION__);
    return false;
  }
  return tp->on_drop_request(g, v, l, me, dropper);
}

void thing_on_death_set(Tpp tp, thing_on_death_t callback)
{
  TRACE();
  if (tp == nullptr) [[unlikely]] {
    err("no thing template pointer");
    return;
  }
  tp->on_death = callback;
}

void thing_on_death(Gamep g, Levelsp v, Levelp l, Thingp me, ThingEvent &e)
{
  TRACE();
  auto *tp = thing_tp(me);
  if (tp == nullptr) [[unlikely]] {
    err("no thing template pointer");
    return;
  }
  if (tp->on_death == nullptr) {
    return;
  }
  tp->on_death(g, v, l, me, e);
}

void thing_on_damage_set(Tpp tp, thing_on_damage_t callback)
{
  TRACE();
  if (tp == nullptr) [[unlikely]] {
    err("no thing template pointer");
    return;
  }
  tp->on_damage = callback;
}

auto thing_on_damage(Gamep g, Levelsp v, Levelp l, Thingp me, ThingEvent &e) -> bool
{
  TRACE();
  auto *tp = thing_tp(me);
  if (tp == nullptr) [[unlikely]] {
    err("no thing template pointer");
    return false;
  }
  if (tp->on_damage == nullptr) {
    return true;
  }
  return tp->on_damage(g, v, l, me, e);
}

void thing_on_attacking_set(Tpp tp, thing_on_attacking_t callback)
{
  TRACE();
  if (tp == nullptr) [[unlikely]] {
    err("no thing template pointer");
    return;
  }
  tp->on_attacking = callback;
}

auto thing_on_attacking(Gamep g, Levelsp v, Levelp l, Thingp me, Thingp it, ThingEvent &e) -> bool
{
  TRACE();
  auto *tp = thing_tp(me);
  if (tp == nullptr) [[unlikely]] {
    err("no thing template pointer");
    return false;
  }
  if (tp->on_attacking == nullptr) {
    return true;
  }
  return tp->on_attacking(g, v, l, me, it, e);
}

void thing_on_moved_set(Tpp tp, thing_on_moved_t callback)
{
  TRACE();
  if (tp == nullptr) [[unlikely]] {
    err("no thing template pointer");
    return;
  }
  tp->on_moved = callback;
}

void thing_on_moved(Gamep g, Levelsp v, Levelp l, Thingp me)
{
  TRACE();
  auto *tp = thing_tp(me);
  if (tp == nullptr) [[unlikely]] {
    err("no thing template pointer");
    return;
  }
  if (tp->on_moved == nullptr) {
    return;
  }
  tp->on_moved(g, v, l, me);
}

void thing_on_teleported_set(Tpp tp, thing_on_teleported_t callback)
{
  TRACE();
  if (tp == nullptr) [[unlikely]] {
    err("no thing template pointer");
    return;
  }
  tp->on_teleported = callback;
}

void thing_on_teleported(Gamep g, Levelsp v, Levelp l, Thingp me)
{
  TRACE();
  auto *tp = thing_tp(me);
  if (tp == nullptr) [[unlikely]] {
    err("no thing template pointer");
    return;
  }
  if (tp->on_teleported == nullptr) {
    return;
  }
  tp->on_teleported(g, v, l, me);
}

void thing_on_jump_begin_set(Tpp tp, thing_on_jump_begin_t callback)
{
  TRACE();
  if (tp == nullptr) [[unlikely]] {
    err("no thing template pointer");
    return;
  }
  tp->on_jump_begin = callback;
}

void thing_on_jump_begin(Gamep g, Levelsp v, Levelp l, Thingp me)
{
  TRACE();
  auto *tp = thing_tp(me);
  if (tp == nullptr) [[unlikely]] {
    err("no thing template pointer");
    return;
  }
  if (tp->on_jump_begin == nullptr) {
    return;
  }
  tp->on_jump_begin(g, v, l, me);
}

void thing_on_jump_end_set(Tpp tp, thing_on_jump_end_t callback)
{
  TRACE();
  if (tp == nullptr) [[unlikely]] {
    err("no thing template pointer");
    return;
  }
  tp->on_jump_end = callback;
}

void thing_on_jump_end(Gamep g, Levelsp v, Levelp l, Thingp me)
{
  TRACE();
  auto *tp = thing_tp(me);
  if (tp == nullptr) [[unlikely]] {
    err("no thing template pointer");
    return;
  }
  if (tp->on_jump_end == nullptr) {
    return;
  }
  tp->on_jump_end(g, v, l, me);
}

void thing_on_shoved_set(Tpp tp, thing_on_shoved_t callback)
{
  TRACE();
  if (tp == nullptr) [[unlikely]] {
    err("no thing template pointer");
    return;
  }
  tp->on_shoved = callback;
}

void thing_on_shoved(Gamep g, Levelsp v, Levelp l, Thingp me, Thingp shover)
{
  TRACE();
  auto *tp = thing_tp(me);
  if (tp == nullptr) [[unlikely]] {
    err("no thing template pointer");
    return;
  }
  if (tp->on_shoved == nullptr) {
    return;
  }
  if (! thing_is_player(shover) && ! thing_is_monst(shover)) {
    thing_err(shover, "unexpected thing for %s", __FUNCTION__);
    return;
  }
  tp->on_shoved(g, v, l, me, shover);
}

void thing_on_fall_begin_set(Tpp tp, thing_on_fall_begin_t callback)
{
  TRACE();
  if (tp == nullptr) [[unlikely]] {
    err("no thing template pointer");
    return;
  }
  tp->on_fall_begin = callback;
}

void thing_on_fall_begin(Gamep g, Levelsp v, Levelp l, Thingp me)
{
  TRACE();
  auto *tp = thing_tp(me);
  if (tp == nullptr) [[unlikely]] {
    err("no thing template pointer");
    return;
  }
  if (tp->on_fall_begin == nullptr) {
    return;
  }
  tp->on_fall_begin(g, v, l, me);
}

void thing_on_melt_set(Tpp tp, thing_on_melt_t callback)
{
  TRACE();
  if (tp == nullptr) [[unlikely]] {
    err("no thing template pointer");
    return;
  }
  tp->on_melt = callback;
}

void thing_on_melt(Gamep g, Levelsp v, Levelp l, Thingp me)
{
  TRACE();
  auto *tp = thing_tp(me);
  if (tp == nullptr) [[unlikely]] {
    err("no thing template pointer");
    return;
  }
  if (tp->on_melt == nullptr) {
    return;
  }
  tp->on_melt(g, v, l, me);
}

void thing_on_fall_end_set(Tpp tp, thing_on_fall_end_t callback)
{
  TRACE();
  if (tp == nullptr) [[unlikely]] {
    err("no thing template pointer");
    return;
  }
  tp->on_fall_end = callback;
}

void thing_on_fall_end(Gamep g, Levelsp v, Levelp l, Thingp me)
{
  TRACE();
  auto *tp = thing_tp(me);
  if (tp == nullptr) [[unlikely]] {
    err("no thing template pointer");
    return;
  }

  if (tp->on_fall_end == nullptr) {
    return;
  }

  tp->on_fall_end(g, v, l, me);
}

void thing_display_get_tile_info_set(Tpp tp, thing_display_get_tile_info_t callback)
{
  TRACE();

  if (tp == nullptr) [[unlikely]] {
    err("no thing template pointer");
    return;
  }
  tp->display_get_tile_info = callback;
}

auto thing_display_get_tile_info(Gamep g, Levelsp v, Levelp l, const bpoint &p, Tpp tp, Thingp t_maybe_null) -> Tilep
{
  TRACE_DEBUG();

  if (tp == nullptr) [[unlikely]] {
    return nullptr;
  }
  if (tp->display_get_tile_info == nullptr) {
    return nullptr;
  }
  return tp->display_get_tile_info(g, v, l, p, tp, t_maybe_null);
}

void thing_assess_tile_set(Tpp tp, thing_assess_tile_t callback)
{
  TRACE();

  if (tp == nullptr) [[unlikely]] {
    err("no thing template pointer");
    return;
  }
  tp->assess_tile = callback;
}

auto thing_assess_tile(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> ThingEnviron
{
  TRACE_DEBUG();

  auto *tp = thing_tp(me);
  if (tp == nullptr) [[unlikely]] {
    err("no thing template pointer");
    return THING_ENVIRON_NEUTRAL;
  }
  if (tp->assess_tile == nullptr) {
    return THING_ENVIRON_NEUTRAL;
  }
  return tp->assess_tile(g, v, l, p, me);
}
