//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_main.hpp"
#include "my_thing.hpp"
#include "my_types.hpp"

[[nodiscard]] auto thing_lifespan(Gamep g, Levelsp v, Levelp l, Thingp me) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return me->_lifespan;
}

[[nodiscard]] auto thing_lifespan_set(Gamep g, Levelsp v, Levelp l, Thingp me, int val) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return me->_lifespan = val;
}

[[nodiscard]] auto thing_lifespan_incr(Gamep g, Levelsp v, Levelp l, Thingp me, int val) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return me->_lifespan += val;
}

[[nodiscard]] auto thing_lifespan_decr(Gamep g, Levelsp v, Levelp l, Thingp me, int val) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  if (static_cast< int >(me->_lifespan) - val <= 0) {
    return me->_lifespan = 0;
  }
  return me->_lifespan -= val;
}

[[nodiscard]] auto thing_lifespan_initial(Thingp me) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return me->_lifespan_initial;
}

[[nodiscard]] auto thing_lifespan_initial_set(Gamep g, Levelsp v, Levelp l, Thingp me, int val) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return me->_lifespan_initial = val;
}

[[nodiscard]] auto thing_lifespan_initial_incr(Gamep g, Levelsp v, Levelp l, Thingp me, int val) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return me->_lifespan_initial += val;
}

[[nodiscard]] auto thing_lifespan_initial_decr(Gamep g, Levelsp v, Levelp l, Thingp me, int val) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  if (static_cast< int >(me->_lifespan_initial) - val <= 0) {
    return me->_lifespan_initial = 0;
  }
  return me->_lifespan_initial -= val;
}
