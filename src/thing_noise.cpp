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

[[nodiscard]] auto thing_noise(Gamep g, Levelsp v, Levelp l, Thingp t) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }

  auto noise = t->_noise;

  //
  // Allow boots to take precedence over a sacrifice
  //
  if (thing_is_stealthy(g, v, l, t)) {
    IF_DEBUG2 { THING_DBG(g, v, l, t, "noise: %d (0 due to stealth)", noise); }
    noise = 0;
  }

  if (thing_is_noisy(g, v, l, t)) {
    noise *= 2;
    IF_DEBUG2 { THING_DBG(g, v, l, t, "noise: %d (noisy)", noise * 2); }
  }

  IF_DEBUG2 { THING_DBG(g, v, l, t, "noise: %d", noise); }

  return noise;
}

[[nodiscard]] auto thing_noise_set(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }

  if (val > THING_NOISE_MAX) {
    thing_err(g, v, l, t, "value overflow: %d", val);
    return 0;
  }

  if (val < 0) {
    thing_err(g, v, l, t, "value underflow: %d", val);
    return 0;
  }

  t->_noise = val;

  return thing_noise(g, v, l, t);
}

[[nodiscard]] auto thing_noise_incr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }

  if (t->_noise + val > THING_NOISE_MAX) {
    return t->_noise = THING_NOISE_MAX;
  }

  return thing_noise_set(g, v, l, t, t->_noise + val);
}

[[nodiscard]] auto thing_noise_decr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }

  if (static_cast< int >(t->_noise) - val <= 0) {
    return t->_noise = 0;
  }

  return thing_noise_set(g, v, l, t, t->_noise - val);
}

[[nodiscard]] auto thing_is_noisy(Gamep g, Levelsp v, Levelp l, Thingp me) -> bool
{
  TRACE_DEBUG();

  if (me == nullptr) {
    ERR("no thing pointer");
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
