//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_main.hpp"
#include "my_thing.hpp"
#include "my_types.hpp"

[[nodiscard]] auto thing_dormant(Gamep g, Levelsp v, Levelp l, Thingp me) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return me->_dormant;
}

[[nodiscard]] auto thing_dormant_set(Gamep g, Levelsp v, Levelp l, Thingp me, int val) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    ERR("no thing pointer");
    return 0;
  }

  thing_anim_init(g, v, l, me, THING_ANIM_DORMANT);

  return me->_dormant = val;
}

[[nodiscard]] auto thing_dormant_incr(Gamep g, Levelsp v, Levelp l, Thingp me, int val) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return me->_dormant += val;
}

[[nodiscard]] auto thing_dormant_decr(Gamep g, Levelsp v, Levelp l, Thingp me, int val) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  if (static_cast< int >(me->_dormant) - val <= 0) {
    me->_dormant = 0;
  } else {
    me->_dormant -= val;
  }

  if (! me->_dormant) {
    thing_anim_init(g, v, l, me, THING_ANIM_IDLE);
  }

  return me->_dormant;
}

[[nodiscard]] auto thing_dormant_max(Thingp me) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return me->_dormant_max;
}

[[nodiscard]] auto thing_dormant_max_set(Gamep g, Levelsp v, Levelp l, Thingp me, int val) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return me->_dormant_max = val;
}

[[nodiscard]] auto thing_dormant_max_incr(Gamep g, Levelsp v, Levelp l, Thingp me, int val) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return me->_dormant_max += val;
}

[[nodiscard]] auto thing_dormant_max_decr(Gamep g, Levelsp v, Levelp l, Thingp me, int val) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  if (static_cast< int >(me->_dormant_max) - val <= 0) {
    return me->_dormant_max = 0;
  }
  return me->_dormant_max -= val;
}
