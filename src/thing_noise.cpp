//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_game_defs.hpp"
#include "my_main.hpp"
#include "my_thing.hpp"
#include "my_thing_inlines.hpp"
#include "my_tp.hpp"
#include "my_types.hpp"

[[nodiscard]] auto thing_noise(Gamep g, Levelsp v, Levelp l, Thingp me) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    no_thing_pointer();
    return 0;
  }

  auto *ext = thing_ext_struct(g, v, me);
  if (ext == nullptr) {
    return 0;
  }

  auto noise = ext->_noise;

  //
  // Allow boots to take precedence over a sacrifice
  //
  if (thing_is_stealthy(g, v, l, me)) {
    IF_DEBUG3 { THING_DBG(g, v, l, me, "noise: %d (0 due to stealth)", noise); }
    noise = 0;
  }

  if (thing_is_noisy(g, v, l, me)) {
    noise *= 2;
    IF_DEBUG3 { THING_DBG(g, v, l, me, "noise: %d (noisy)", noise * 2); }
  }

  IF_DEBUG3 { THING_DBG(g, v, l, me, "noise: %d", noise); }

  return noise;
}

[[nodiscard]] auto thing_noise_set(Gamep g, Levelsp v, Levelp l, Thingp me, int val) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    no_thing_pointer();
    return 0;
  }

  auto *ext = thing_ext_struct(g, v, me);
  if (ext == nullptr) {
    return 0;
  }

  if (val > THING_NOISE_MAX) {
    thing_err(g, v, l, me, "value overflow: %d", val);
    return 0;
  }

  if (val < 0) {
    thing_err(g, v, l, me, "value underflow: %d", val);
    return 0;
  }

  ext->_noise = val;

  return thing_noise(g, v, l, me);
}

[[nodiscard]] auto thing_noise_incr(Gamep g, Levelsp v, Levelp l, Thingp me, int val) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    no_thing_pointer();
    return 0;
  }

  auto *ext = thing_ext_struct(g, v, me);
  if (ext == nullptr) {
    return 0;
  }

  if (ext->_noise + val > THING_NOISE_MAX) {
    return ext->_noise = THING_NOISE_MAX;
  }

  return thing_noise_set(g, v, l, me, ext->_noise + val);
}

[[nodiscard]] auto thing_noise_decr(Gamep g, Levelsp v, Levelp l, Thingp me, int val) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    no_thing_pointer();
    return 0;
  }

  auto *ext = thing_ext_struct(g, v, me);
  if (ext == nullptr) {
    return 0;
  }

  if (static_cast< int >(ext->_noise) - val <= 0) {
    return ext->_noise = 0;
  }

  return thing_noise_set(g, v, l, me, ext->_noise - val);
}

[[nodiscard]] auto thing_is_noisy(Gamep g, Levelsp v, Levelp l, Thingp me) -> bool
{
  TRACE_DEBUG();

  if (me == nullptr) {
    no_thing_pointer();
    return false;
  }

  FOR_ALL_HOOKS(g, v, l, me, buff)
  {
    if (thing_is_noisy(g, v, l, buff)) {
      return true;
    }
  }

  FOR_ALL_ACTIVE_ITEMS(g, v, l, me, item)
  {
    if (thing_is_noisy(g, v, l, item)) {
      return true;
    }
  }

  return tp_flag(thing_tp(me), is_noisy) != 0;
}
