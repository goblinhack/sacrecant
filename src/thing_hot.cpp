//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_game_defs.hpp"
#include "my_main.hpp"
#include "my_thing.hpp"
#include "my_thing_inlines.hpp"
#include "my_time.hpp"
#include "my_tp_inlines.hpp"
#include "my_types.hpp"
#include <algorithm>
#include <cstdint>

[[nodiscard]] auto thing_is_always_hot(Thingp me) -> bool
{
  TRACE_DEBUG();

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
void thing_hot_time_step(Gamep g, Levelsp v, Levelp l, Thingp me, int /*time_step*/)
{
  TRACE_DEBUG();

  if (! thing_is_physics_temperature(me)) {
    return;
  }

  if (thing_is_always_hot(me)) {
    return;
  }

  auto T = thing_temperature(me);

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
    THING_DBG(g, v, l, me, "lower_limit %d upper_limit %d threshold %d temp %d", lower_limit, upper_limit, threshold, T);
  }

  if (T < threshold) {
    me->_is_hot = 0;
    return;
  }

  //
  // Pulse the animation
  //
  auto        pulse = THING_IS_HOT_PULSE_ANIM_MS; // ms
  float const mid   = pulse / 2;
  auto const  n     = static_cast< float >(game_time_ms_cached() % pulse);
  float       i     = 0;
  uint8_t     a     = 0;

  if (n == mid) {
    i = 255;
  } else if (n > mid) {
    i = (n - mid) / mid;
    i *= 100;
    i = 255 - i;
  } else {
    i = n / mid;
    i *= 100;
    i = 155 + i;
  }

  i = std::max< float >(i, 0);
  i = std::min< float >(i, 255);

  a = static_cast< uint8_t >(i);

  me->_is_hot = (a / 2) + 120;
}

//
// Hot animation time step
//
[[nodiscard]] auto thing_is_hot_check(Gamep g, Levelsp v, Levelp l, Thingp me) -> bool
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
