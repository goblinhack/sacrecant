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

auto thing_is_submergible(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_submergible) != 0;
}

auto thing_submerged_pct_set(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_submerged_pct = val;
}

auto thing_submerged_pct_incr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_submerged_pct += val;
}

auto thing_submerged_pct_decr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  if (t->_submerged_pct - val <= 0) {
    return t->_submerged_pct = 0;
  }
  return t->_submerged_pct -= val;
}

//
// Update submerged status
//
void thing_submerged_update(Gamep g, Levelsp v, Levelp l, Thingp t)
{
  TRACE();

  if (! thing_is_submergible(t)) {
    return;
  }

  auto at = thing_at(t);
  (void) thing_submerged_pct_set(g, v, l, t, 0);
  if (level_is_deep_water(g, v, l, at) != nullptr) {
    (void) thing_submerged_pct_set(g, v, l, t, 75);
  } else if (level_is_water(g, v, l, at) != nullptr) {
    (void) thing_submerged_pct_set(g, v, l, t, 50);
  } else if (level_is_lava(g, v, l, at) != nullptr) {
    (void) thing_submerged_pct_set(g, v, l, t, 50);
  }
}
