//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_main.hpp"
#include "my_thing.hpp"
#include "my_thing_inlines.hpp"
#include "my_tp.hpp"
#include "my_types.hpp"

[[nodiscard]] auto thing_is_wall_walker(Gamep g, Levelsp v, Levelp l, Thingp me) -> bool
{
  TRACE_DEBUG();

  if (me == nullptr) {
    ERR("no thing pointer");
    return false;
  }

  FOR_ALL_HOOKS(g, v, l, me, buff)
  {
    if (thing_is_wall_walker(g, v, l, buff)) {
      return true;
    }
  }

  FOR_ALL_ACTIVE_ITEMS(g, v, l, me, item)
  {
    if (thing_is_wall_walker(g, v, l, item)) {
      return true;
    }
  }

  return tp_flag(thing_tp(me), is_wall_walker) != 0;
}

[[nodiscard]] auto thing_is_wall_walker_pass_through(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_wall_walker_pass_through) != 0;
}

[[nodiscard]] auto thing_is_obs_to_wall_walker(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_obs_to_wall_walker) != 0;
}
