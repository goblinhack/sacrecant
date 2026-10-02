//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_level.hpp"
#include "my_main.hpp"
#include "my_thing.hpp"
#include "my_thing_inlines.hpp"
#include "my_tp.hpp"
#include "my_types.hpp"

[[nodiscard]] auto thing_is_submergible(Gamep g, Levelsp v, Levelp l, Thingp me) -> bool
{
  TRACE_DEBUG();

  if (me == nullptr) {
    ERR("no thing pointer");
    return false;
  }

  if (thing_is_flying(g, v, l, me) || thing_is_levitating(g, v, l, me)) {
    return false;
  }

  return tp_flag(thing_tp(me), is_submergible) != 0;
}

[[nodiscard]] auto thing_is_submerged(Gamep g, Levelsp v, Levelp l, Thingp me) -> bool
{
  TRACE_DEBUG();

  if (me == nullptr) {
    ERR("no thing pointer");
    return false;
  }

  return me->_submerged_pct > 0;
}

[[nodiscard]] auto thing_submerged_pct_set(Gamep g, Levelsp v, Levelp l, Thingp me, int val) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    ERR("no thing pointer");
    return 0;
  }

  if (! thing_is_submergible(g, v, l, me)) {
    val = 0;
  }

  return me->_submerged_pct = val;
}

[[nodiscard]] auto thing_submerged_pct_incr(Gamep g, Levelsp v, Levelp l, Thingp me, int val) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    ERR("no thing pointer");
    return 0;
  }

  if (! thing_is_submergible(g, v, l, me)) {
    val = 0;
  }

  return me->_submerged_pct += val;
}

[[nodiscard]] auto thing_submerged_pct_decr(Gamep g, Levelsp v, Levelp l, Thingp me, int val) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    ERR("no thing pointer");
    return 0;
  }

  if (! thing_is_submergible(g, v, l, me)) {
    val = 0;
  }

  if (me->_submerged_pct - val <= 0) {
    return me->_submerged_pct = 0;
  }
  return me->_submerged_pct -= val;
}

//
// Update submerged status
//
void thing_submerged_update(Gamep g, Levelsp v, Levelp l, Thingp me)
{
  TRACE();

  if (! thing_is_submergible(g, v, l, me)) {
    return;
  }

  auto at = thing_at(g, v, l, me);
  (void) thing_submerged_pct_set(g, v, l, me, 0);
  if (level_is_water_deep_cached(g, v, l, at)) {
    (void) thing_submerged_pct_set(g, v, l, me, 75);
  } else if (level_is_water_shallow_cached(g, v, l, at)) {
    (void) thing_submerged_pct_set(g, v, l, me, 50);
  } else if (level_is_lava_cached(g, v, l, at)) {
    (void) thing_submerged_pct_set(g, v, l, me, 50);
  }
}
