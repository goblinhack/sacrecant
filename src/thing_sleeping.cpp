//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_globals.hpp"
#include "my_main.hpp"
#include "my_thing.hpp"
#include "my_time.hpp"
#include "my_tp.hpp"

auto thing_is_sleeping(Thingp me) -> bool
{
  TRACE_DEBUG();

  if (me == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return me->_is_sleeping;
}

void thing_is_sleeping_set(Gamep g, Levelsp v, Levelp l, Thingp me, bool val)
{
  TRACE_DEBUG();

  if (me == nullptr) {
    ERR("no thing pointer");
    return;
  }
  me->_is_sleeping = val;
}

void thing_is_sleeping_unset(Gamep g, Levelsp v, Levelp l, Thingp me)
{
  TRACE_DEBUG();

  thing_is_sleeping_set(g, v, l, me, false);
}
