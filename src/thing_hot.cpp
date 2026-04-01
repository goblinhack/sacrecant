//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_level.hpp"
#include "my_main.hpp"
#include "my_thing.hpp"
#include "my_thing_inlines.hpp"
#include "my_time.hpp"
#include "my_tp.hpp"

auto thing_is_always_hot(Thingp me) -> bool
{
  TRACE();
  if (thing_is_fire(me)) {
    return true;
  }
  if (thing_is_lava(me)) {
    return true;
  }
  if (thing_is_explosion(me)) {
    return true;
  }
  return false;
}

//
// Hot animation time step
//
void thing_hot_time_step(Gamep g, Levelsp v, Levelp l, Thingp me, int time_step)
{
  TRACE();

  auto T = thing_temperature(me);
  if (! thing_is_physics_temperature(me)) {
    return;
  }

  if (thing_is_always_hot(me)) {
    return;
  }

  //
  // If halfway towards burning/melting, make it pulse
  //
  auto *tp          = thing_tp(me);
  auto  upper_limit = std::max(tp_temperature_burns_at_get(tp), tp_temperature_melts_at_get(tp));
  if (upper_limit == 0) {
    return;
  }

  auto lower_limit = tp_temperature_initial_get(tp);
  if (lower_limit == 0) {
    return;
  }

  auto threshold = ((upper_limit - lower_limit) / 2) + lower_limit;

  if (compiler_unused) {
    THING_DBG(me, "lower_limit %d upper_limit %d threshold %d temp %d", lower_limit, upper_limit, threshold, T);
  }

  if (T < threshold) {
    me->_is_hot = 0;
    return;
  }

  auto pulse = THING_IS_HOT_PULSE_MS; // ms
  auto mid   = pulse / 2;
  auto n     = time_ms_cached() % pulse;

  if (n > mid) {
    n = mid - n;
  }

  auto i      = static_cast< int >((255 / static_cast< float >(mid)) * static_cast< float >(n));
  me->_is_hot = ((static_cast< uint8_t >(i)) / 2) + 100;
}

//
// Hot animation time step
//
auto thing_is_hot_check(Gamep g, Levelsp v, Levelp l, Thingp me) -> bool
{
  TRACE();

  auto T = thing_temperature(me);
  if (! thing_is_physics_temperature(me)) {
    return false;
  }

  if (thing_is_always_hot(me)) {
    thing_is_hot_set(g, v, l, me, true);
    return true;
  }

  //
  // If halfway towards burning/melting, make it pulse
  //
  auto *tp          = thing_tp(me);
  auto  upper_limit = std::max(tp_temperature_burns_at_get(tp), tp_temperature_melts_at_get(tp));
  if (upper_limit == 0) {
    return false;
  }

  auto lower_limit = tp_temperature_initial_get(tp);
  if (lower_limit == 0) {
    return false;
  }

  auto threshold = ((upper_limit - lower_limit) / 2) + lower_limit;

  if (T < threshold) {
    thing_is_hot_set(g, v, l, me, false);
    return true;
  }

  thing_is_hot_set(g, v, l, me, true);
  return false;
}

void thing_is_hot_set(Gamep g, Levelsp v, Levelp l, Thingp me, bool val)
{
  TRACE_DEBUG();

  if (me == nullptr) {
    ERR("no thing pointer");
    return;
  }

  if (val) {
    if (me->_is_hot != 0U) {
      return;
    }

    me->_is_hot = 100;
  } else {
    me->_is_hot = 0;
  }
}
