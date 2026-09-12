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

[[nodiscard]] auto thing_health(Gamep g, Levelsp v, Levelp l, Thingp t) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_health;
}

[[nodiscard]] auto thing_health_set(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }

  if (val > std::numeric_limits< decltype(t->_health) >::max()) {
    thing_err(g, v, l, t, "value overflow: %d", val);
    return 0;
  }

  game_request_to_remake_ui_set(g);
  t->_health = val;
  if (t->_health_max != 0) {
    t->_health = std::min(t->_health_max, t->_health);
  }

  if (thing_is_player(t)) {
    THING_DBG(g, v, l, t, "health set to %d", t->_health);
  }

  return t->_health;
}

[[nodiscard]] auto thing_health_incr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  game_request_to_remake_ui_set(g);
  t->_health += val;
  if (t->_health_max != 0) {
    t->_health = std::min(t->_health_max, t->_health);
  }
  return t->_health;
}

[[nodiscard]] auto thing_health_decr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  game_request_to_remake_ui_set(g);

  if (static_cast< int >(t->_health) - val <= 0) {
    return t->_health = 0;
  }

  return t->_health -= val;
}

[[nodiscard]] auto thing_health_max(Gamep g, Levelsp v, Levelp l, Thingp t) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_health_max;
}

[[nodiscard]] auto thing_health_max_set(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }

  if (val > std::numeric_limits< decltype(t->_health_max) >::max()) {
    thing_err(g, v, l, t, "value overflow: %d", val);
    return 0;
  }

  game_request_to_remake_ui_set(g);

  auto new_health_max = t->_health_max = val;

  if (t->_health > new_health_max) {
    (void) thing_health_set(g, v, l, t, new_health_max);
  }

  if (thing_is_player(t)) {
    THING_DBG(g, v, l, t, "health max set to %d", t->_health_max);
  }

  return new_health_max;
}

[[nodiscard]] auto thing_health_max_incr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  game_request_to_remake_ui_set(g);
  return t->_health_max += val;
}

[[nodiscard]] auto thing_health_max_decr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  game_request_to_remake_ui_set(g);

  if (static_cast< int >(t->_health_max) - val <= 0) {
    return t->_health_max = 0;
  }

  return t->_health_max -= val;
}

[[nodiscard]] auto thing_is_shown_health(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_shown_health) != 0;
}
