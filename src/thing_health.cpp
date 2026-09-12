//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_game.hpp"
#include "my_main.hpp"
#include "my_thing.hpp"
#include "my_thing_inlines.hpp"
#include "my_tp.hpp"
#include "my_types.hpp"
#include <algorithm>
#include <limits>

[[nodiscard]] auto thing_health(Gamep g, Levelsp v, Levelp l, Thingp me) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return me->_health;
}

[[nodiscard]] auto thing_health_set(Gamep g, Levelsp v, Levelp l, Thingp me, int val) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    ERR("no thing pointer");
    return 0;
  }

  if (val > std::numeric_limits< decltype(me->_health) >::max()) {
    thing_err(g, v, l, me, "value overflow: %d", val);
    return 0;
  }

  game_request_to_remake_ui_set(g);

  me->_health = val;
  if (me->_health_max != 0) {
    me->_health = std::min(me->_health_max, me->_health);
  }

  if (thing_is_player(me)) {
    THING_DBG(g, v, l, me, "health set to %d", me->_health);
  }

  return me->_health;
}

[[nodiscard]] auto thing_health_incr(Gamep g, Levelsp v, Levelp l, Thingp me, int val) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    ERR("no thing pointer");
    return 0;
  }

  return thing_health_set(g, v, l, me, me->_health + val);
}

[[nodiscard]] auto thing_health_decr(Gamep g, Levelsp v, Levelp l, Thingp me, int val) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    ERR("no thing pointer");
    return 0;
  }

  if (static_cast< int >(me->_health) - val <= 0) {
    return me->_health = 0;
  }

  return thing_health_set(g, v, l, me, me->_health - val);
}

[[nodiscard]] auto thing_health_max(Gamep g, Levelsp v, Levelp l, Thingp me) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return me->_health_max;
}

[[nodiscard]] auto thing_health_max_set(Gamep g, Levelsp v, Levelp l, Thingp me, int val) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    ERR("no thing pointer");
    return 0;
  }

  if (val > std::numeric_limits< decltype(me->_health_max) >::max()) {
    thing_err(g, v, l, me, "value overflow: %d", val);
    return 0;
  }

  game_request_to_remake_ui_set(g);

  auto new_health_max = me->_health_max = val;

  if (me->_health > new_health_max) {
    (void) thing_health_set(g, v, l, me, new_health_max);
  }

  if (thing_is_player(me)) {
    THING_DBG(g, v, l, me, "health max set to %d", me->_health_max);
  }

  return new_health_max;
}

[[nodiscard]] auto thing_health_max_incr(Gamep g, Levelsp v, Levelp l, Thingp me, int val) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return thing_health_max_set(g, v, l, me, me->_health_max + val);
}

[[nodiscard]] auto thing_health_max_decr(Gamep g, Levelsp v, Levelp l, Thingp me, int val) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    ERR("no thing pointer");
    return 0;
  }

  if (static_cast< int >(me->_health_max) - val <= 0) {
    return me->_health_max = 0;
  }

  return thing_health_max_set(g, v, l, me, me->_health_max - val);
}

[[nodiscard]] auto thing_is_shown_health(Thingp me) -> bool
{
  TRACE_DEBUG();

  if (me == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(me), is_shown_health) != 0;
}
