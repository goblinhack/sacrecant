//
// Copyright goblinhack@gmail.com
//

#include "my_bpoint.hpp"
#include "my_callstack.hpp"
#include "my_main.hpp"
#include "my_thing.hpp"
#include "my_thing_callbacks.hpp"
#include "my_thing_inlines.hpp"
#include "my_tp.hpp"
#include "my_tp_class.hpp"
#include "my_types.hpp"
#include <string>

void thing_description_set(Tpp tp, thing_description_get_t callback)
{
  TRACE();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return;
  }
  tp->description_get = callback;
}

[[nodiscard]] auto thing_description_get(Gamep g, Levelsp v, Levelp l, Thingp me) -> std::string
{
  TRACE();
  auto *tp = thing_tp(me);
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
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
    ERR("no thing template pointer");
    return;
  }
  tp->detail_get = callback;
}

[[nodiscard]] auto thing_detail_get(Gamep g, Levelsp v, Levelp l, Thingp me) -> std::string
{
  TRACE();
  auto *tp = thing_tp(me);
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
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
    ERR("no thing template pointer");
    return;
  }
  tp->z_depth_get = callback;
}

void thing_mouse_down_set(Tpp tp, thing_mouse_down_t callback)
{
  TRACE();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return;
  }
  tp->mouse_down = callback;
}

[[nodiscard]] auto thing_mouse_down(Gamep g, Levelsp v, Levelp l, Thingp me, int x, int y, int button) -> bool
{
  TRACE();
  auto *tp = thing_tp(me);
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return false;
  }
  if (tp->mouse_down == nullptr) {
    return false;
  }
  return tp->mouse_down(g, v, l, me, x, y, button);
}

void thing_on_pushed_set(Tpp tp, thing_on_pushed_t callback)
{
  TRACE();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return;
  }
  tp->on_pushed = callback;
}

void thing_on_pushed(Gamep g, Levelsp v, Levelp l, Thingp me)
{
  TRACE();
  auto *tp = thing_tp(me);
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return;
  }
  if (tp->on_pushed == nullptr) {
    return;
  }
  tp->on_pushed(g, v, l, me);
}

void thing_on_popped_set(Tpp tp, thing_on_popped_t callback)
{
  TRACE();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return;
  }
  tp->on_popped = callback;
}

void thing_on_popped(Gamep g, Levelsp v, Levelp l, Thingp me)
{
  TRACE();
  auto *tp = thing_tp(me);
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return;
  }
  if (tp->on_popped == nullptr) {
    return;
  }
  tp->on_popped(g, v, l, me);
}

void thing_on_tick_idle_set(Tpp tp, thing_on_tick_idle_t callback)
{
  TRACE();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return;
  }
  tp->on_tick_idle = callback;
}

void thing_on_tick_idle(Gamep g, Levelsp v, Levelp l, Thingp me)
{
  TRACE();
  auto *tp = thing_tp(me);
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return;
  }
  if (tp->on_tick_idle == nullptr) {
    return;
  }
  tp->on_tick_idle(g, v, l, me);
}

void thing_on_tick_begin_set(Tpp tp, thing_on_tick_begin_t callback)
{
  TRACE();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return;
  }
  tp->on_tick_begin = callback;
}

void thing_on_tick_begin(Gamep g, Levelsp v, Levelp l, Thingp me)
{
  TRACE();
  auto *tp = thing_tp(me);
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return;
  }
  if (tp->on_tick_begin == nullptr) {
    return;
  }
  tp->on_tick_begin(g, v, l, me);
}

void thing_on_tick_end_set(Tpp tp, thing_on_tick_end_t callback)
{
  TRACE();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return;
  }
  tp->on_tick_end = callback;
}

void thing_on_tick_end(Gamep g, Levelsp v, Levelp l, Thingp me)
{
  TRACE();
  auto *tp = thing_tp(me);
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return;
  }
  if (tp->on_tick_end == nullptr) {
    return;
  }
  tp->on_tick_end(g, v, l, me);
}

void thing_on_level_populated_set(Tpp tp, thing_on_level_populated_t callback)
{
  TRACE();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return;
  }
  tp->on_level_populated = callback;
}

void thing_on_level_populated(Gamep g, Levelsp v, Levelp l, Thingp me)
{
  TRACE();
  auto *tp = thing_tp(me);
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
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
    ERR("no thing template pointer");
    return;
  }
  tp->on_level_enter = callback;
}

void thing_on_level_enter(Gamep g, Levelsp v, Levelp l, Thingp me)
{
  TRACE();
  auto *tp = thing_tp(me);
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
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
    ERR("no thing template pointer");
    return;
  }
  tp->on_level_leave = callback;
}

void thing_on_level_leave(Gamep g, Levelsp v, Levelp l, Thingp me)
{
  TRACE();
  auto *tp = thing_tp(me);
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return;
  }
  if (tp->on_level_leave == nullptr) {
    return;
  }
  tp->on_level_leave(g, v, l, me);
}

void thing_on_engulf_request_set(Tpp tp, thing_on_engulf_request_t callback)
{
  TRACE();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return;
  }
  tp->on_engulf_request = callback;
}

[[nodiscard]] auto thing_on_engulf_request(Gamep g, Levelsp v, Levelp l, Thingp me, Thingp engulfer) -> bool
{
  THING_DBG(g, v, l, me, "%s", __FUNCTION__);
  TRACE_INDENT();

  auto *tp = thing_tp(me);
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return false;
  }
  if (tp->on_engulf_request == nullptr) {
    //
    // Assume success
    //
    return true;
  }
  if (! thing_is_player(engulfer) && ! thing_is_monst(engulfer)) {
    thing_err(g, v, l, engulfer, "unexpected thing for %s", __FUNCTION__);
    return false;
  }
  auto ret = tp->on_engulf_request(g, v, l, me, engulfer);
  if (ret) {
    THING_DBG(g, v, l, me, "engulfs");
  } else {
    THING_DBG(g, v, l, me, "fails to engulf");
  }
  return ret;
}

void thing_on_open_request_set(Tpp tp, thing_on_open_request_t callback)
{
  TRACE();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return;
  }
  tp->on_open_request = callback;
}

[[nodiscard]] auto thing_on_open_request(Gamep g, Levelsp v, Levelp l, Thingp me, Thingp opener) -> bool
{
  THING_DBG(g, v, l, me, "%s", __FUNCTION__);
  TRACE_INDENT();

  auto *tp = thing_tp(me);
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return false;
  }
  if (tp->on_open_request == nullptr) {
    //
    // Assume success
    //
    return true;
  }
  if (! thing_is_player(opener) && ! thing_is_monst(opener)) {
    thing_err(g, v, l, opener, "unexpected thing for %s", __FUNCTION__);
    return false;
  }
  auto ret = tp->on_open_request(g, v, l, me, opener);
  if (ret) {
    THING_DBG(g, v, l, me, "opens");
  } else {
    THING_DBG(g, v, l, me, "fails to open");
  }
  return ret;
}

void thing_on_close_request_set(Tpp tp, thing_on_close_request_t callback)
{
  TRACE();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return;
  }
  tp->on_close_request = callback;
}

[[nodiscard]] auto thing_on_close_request(Gamep g, Levelsp v, Levelp l, Thingp me, Thingp closer) -> bool
{
  THING_DBG(g, v, l, me, "%s", __FUNCTION__);
  TRACE_INDENT();

  auto *tp = thing_tp(me);
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return false;
  }
  if (tp->on_close_request == nullptr) {
    //
    // Assume success
    //
    return true;
  }
  if (! thing_is_player(closer) && ! thing_is_monst(closer)) {
    thing_err(g, v, l, closer, "unexpected thing for %s", __FUNCTION__);
    return false;
  }
  auto ret = tp->on_close_request(g, v, l, me, closer);
  if (ret) {
    THING_DBG(g, v, l, me, "closes");
  } else {
    THING_DBG(g, v, l, me, "fails to close");
  }
  return ret;
}

void thing_on_death_set(Tpp tp, thing_on_death_t callback)
{
  TRACE();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return;
  }
  tp->on_death = callback;
}

void thing_on_death(Gamep g, Levelsp v, Levelp l, Thingp me, ThingEvent &e)
{
  TRACE();
  auto *tp = thing_tp(me);
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
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
    ERR("no thing template pointer");
    return;
  }
  tp->on_damage = callback;
}

[[nodiscard]] auto thing_on_damage(Gamep g, Levelsp v, Levelp l, Thingp me, ThingEvent &e) -> bool
{
  TRACE();
  auto *tp = thing_tp(me);
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
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
    ERR("no thing template pointer");
    return;
  }
  tp->on_attacking = callback;
}

[[nodiscard]] auto thing_on_attacking(Gamep g, Levelsp v, Levelp l, Thingp attacker, Thingp target, ThingEvent &e) -> bool
{
  TRACE();
  auto *tp = thing_tp(attacker);
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return false;
  }
  if (tp->on_attacking == nullptr) {
    return true;
  }
  return tp->on_attacking(g, v, l, attacker, target, e);
}

void thing_on_killing_set(Tpp tp, thing_on_killing_t callback)
{
  TRACE();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return;
  }
  tp->on_killing = callback;
}

[[nodiscard]] auto thing_on_killing(Gamep g, Levelsp v, Levelp l, Thingp attacker, Thingp target, ThingEvent &e) -> bool
{
  TRACE();
  auto *tp = thing_tp(attacker);
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return false;
  }
  if (tp->on_killing == nullptr) {
    return true;
  }
  return tp->on_killing(g, v, l, attacker, target, e);
}

void thing_on_missing_set(Tpp tp, thing_on_missing_t callback)
{
  TRACE();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return;
  }
  tp->on_missing = callback;
}

[[nodiscard]] auto thing_on_missing(Gamep g, Levelsp v, Levelp l, Thingp attacker, Thingp target, ThingEvent &e) -> bool
{
  TRACE();
  auto *tp = thing_tp(attacker);
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return false;
  }
  if (tp->on_missing == nullptr) {
    return true;
  }
  return tp->on_missing(g, v, l, attacker, target, e);
}

void thing_on_moved_set(Tpp tp, thing_on_moved_t callback)
{
  TRACE();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return;
  }
  tp->on_moved = callback;
}

void thing_on_moved(Gamep g, Levelsp v, Levelp l, Thingp me)
{
  TRACE();
  auto *tp = thing_tp(me);
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return;
  }
  if (tp->on_moved == nullptr) {
    return;
  }
  tp->on_moved(g, v, l, me);
}

void thing_on_moving_to_set(Tpp tp, thing_on_moving_to_t callback)
{
  TRACE();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return;
  }
  tp->on_moving_to = callback;
}

void thing_on_moving_to(Gamep g, Levelsp v, Levelp l, Thingp me, const bpoint &to)
{
  TRACE();
  auto *tp = thing_tp(me);
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return;
  }
  if (tp->on_moving_to == nullptr) {
    return;
  }
  tp->on_moving_to(g, v, l, me, to);
}

void thing_on_teleported_set(Tpp tp, thing_on_teleported_t callback)
{
  TRACE();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return;
  }
  tp->on_teleported = callback;
}

void thing_on_teleported(Gamep g, Levelsp v, Levelp l, Thingp me)
{
  TRACE();
  auto *tp = thing_tp(me);
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return;
  }
  if (tp->on_teleported == nullptr) {
    return;
  }
  tp->on_teleported(g, v, l, me);
}

void thing_on_activated_set(Tpp tp, thing_on_activated_t callback)
{
  TRACE();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return;
  }
  tp->on_activated = callback;
}

auto thing_on_activated(Gamep g, Levelsp v, Levelp l, Thingp trap, Thingp user) -> bool
{
  TRACE();
  auto *tp = thing_tp(trap);
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return false;
  }
  if (tp->on_activated == nullptr) {
    return false;
  }
  return tp->on_activated(g, v, l, trap, user);
}

void thing_on_jump_begin_set(Tpp tp, thing_on_jump_begin_t callback)
{
  TRACE();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return;
  }
  tp->on_jump_begin = callback;
}

void thing_on_jump_begin(Gamep g, Levelsp v, Levelp l, Thingp me)
{
  TRACE();
  auto *tp = thing_tp(me);
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
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
    ERR("no thing template pointer");
    return;
  }
  tp->on_jump_end = callback;
}

void thing_on_jump_end(Gamep g, Levelsp v, Levelp l, Thingp me)
{
  TRACE();
  auto *tp = thing_tp(me);
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return;
  }
  if (tp->on_jump_end == nullptr) {
    return;
  }
  tp->on_jump_end(g, v, l, me);
}

void thing_on_thrown_begin_set(Tpp tp, thing_on_thrown_begin_t callback)
{
  TRACE();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return;
  }
  tp->on_thrown_begin = callback;
}

void thing_on_thrown_begin(Gamep g, Levelsp v, Levelp l, Thingp me, Thingp thrower)
{
  TRACE();
  auto *tp = thing_tp(me);
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return;
  }
  if (tp->on_thrown_begin == nullptr) {
    return;
  }
  tp->on_thrown_begin(g, v, l, me, thrower);
}

void thing_on_thrown_end_set(Tpp tp, thing_on_thrown_end_t callback)
{
  TRACE();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return;
  }
  tp->on_thrown_end = callback;
}

void thing_on_thrown_end(Gamep g, Levelsp v, Levelp l, Thingp me, Thingp thrower)
{
  TRACE();
  auto *tp = thing_tp(me);
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return;
  }
  if (tp->on_thrown_end == nullptr) {
    if (thing_is_player(thrower)) {
      thing_sound_play(g, v, l, thrower, "item_drop");
    }
    return;
  }
  tp->on_thrown_end(g, v, l, me, thrower);
}

void thing_on_shoved_set(Tpp tp, thing_on_shoved_t callback)
{
  TRACE();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return;
  }
  tp->on_shoved = callback;
}

void thing_on_shoved(Gamep g, Levelsp v, Levelp l, Thingp me, Thingp shover)
{
  TRACE();
  auto *tp = thing_tp(me);
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return;
  }
  if (tp->on_shoved == nullptr) {
    return;
  }
  if (! thing_is_player(shover) && ! thing_is_monst(shover)) {
    thing_err(g, v, l, shover, "unexpected thing for %s", __FUNCTION__);
    return;
  }
  tp->on_shoved(g, v, l, me, shover);
}

void thing_on_fall_begin_set(Tpp tp, thing_on_fall_begin_t callback)
{
  TRACE();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return;
  }
  tp->on_fall_begin = callback;
}

void thing_on_fall_begin(Gamep g, Levelsp v, Levelp l, Thingp me)
{
  TRACE();
  auto *tp = thing_tp(me);
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
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
    ERR("no thing template pointer");
    return;
  }
  tp->on_melt = callback;
}

void thing_on_melt(Gamep g, Levelsp v, Levelp l, Thingp me)
{
  TRACE();
  auto *tp = thing_tp(me);
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
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
    ERR("no thing template pointer");
    return;
  }
  tp->on_fall_end = callback;
}

void thing_on_fall_end(Gamep g, Levelsp v, Levelp l, Thingp me)
{
  TRACE();
  auto *tp = thing_tp(me);
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
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
    ERR("no thing template pointer");
    return;
  }
  tp->display_get_tile_info = callback;
}

[[nodiscard]] auto thing_display_get_tile_info(Gamep g, Levelsp v, Levelp l, const bpoint &p, Tpp tp, Thingp t_maybe_null) -> Tilep
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
    ERR("no thing template pointer");
    return;
  }
  tp->assess_tile = callback;
}

[[nodiscard]] auto thing_assess_tile(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> ThingEnvironType
{
  TRACE_DEBUG();

  auto *tp = thing_tp(me);
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return THING_ENVIRON_NEUTRAL;
  }
  if (tp->assess_tile == nullptr) {
    return THING_ENVIRON_NEUTRAL;
  }
  return tp->assess_tile(g, v, l, p, me);
}

void thing_assess_tp_set(Tpp tp, thing_assess_tp_t callback)
{
  TRACE();

  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return;
  }
  tp->assess_tp = callback;
}

[[nodiscard]] auto thing_assess_tp(Gamep g, Levelsp v, Levelp l, Tpp tp, Thingp me) -> ThingEnvironType
{
  TRACE_DEBUG();

  auto *me_tp = thing_tp(me);
  if (me_tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return THING_ENVIRON_NEUTRAL;
  }
  if (me_tp->assess_tp == nullptr) {
    return THING_ENVIRON_NEUTRAL;
  }
  return me_tp->assess_tp(g, v, l, tp, me);
}
