//
// Copyright goblinhack@gmail.com
//

#include <limits>
#include <utility>

#include "my_callstack.hpp"
#include "my_thing.hpp"
#include "my_types.hpp"

[[nodiscard]] auto thing_speed(Gamep g, Levelsp v, Levelp l, Thingp me) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    no_thing_pointer();
    return 0;
  }

  auto *engulfer = thing_engulfer(g, v, l, me);
  if (engulfer != nullptr) {
    return thing_speed(g, v, l, engulfer);
  }

  return me->_speed;
}

[[nodiscard]] auto thing_speed_set(Gamep g, Levelsp v, Levelp l, Thingp me, int val) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    no_thing_pointer();
    return 0;
  }

  if (std::cmp_greater(val, std::numeric_limits< decltype(me->_speed) >::max())) {
    thing_err(g, v, l, me, "value overflow: %d", val);
    return 0;
  }

  return me->_speed = val;
}

[[nodiscard]] auto thing_speed_incr(Gamep g, Levelsp v, Levelp l, Thingp me, int val) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    no_thing_pointer();
    return 0;
  }
  return me->_speed += val;
}

[[nodiscard]] auto thing_speed_decr(Gamep g, Levelsp v, Levelp l, Thingp me, int val) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    no_thing_pointer();
    return 0;
  }
  if (static_cast< int >(me->_speed) - val <= 0) {
    return me->_speed = 0;
  }
  return me->_speed -= val;
}
