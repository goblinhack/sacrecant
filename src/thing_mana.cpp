//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_game.hpp"
#include "my_main.hpp"
#include "my_thing.hpp"
#include "my_types.hpp"
#include <algorithm>
#include <limits>

[[nodiscard]] auto thing_mana(Gamep g, Levelsp v, Levelp l, Thingp t) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_mana;
}

[[nodiscard]] auto thing_mana_set(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }

  if (val > std::numeric_limits< decltype(t->_mana) >::max()) {
    thing_err(g, v, l, t, "value overflow: %d", val);
    return 0;
  }

  game_request_to_remake_ui_set(g);
  t->_mana = val;
  if (t->_mana_max != 0) {
    t->_mana = std::min(t->_mana_max, t->_mana);
  }
  return t->_mana;
}

[[nodiscard]] auto thing_mana_incr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  game_request_to_remake_ui_set(g);
  t->_mana += val;
  if (t->_mana_max != 0) {
    t->_mana = std::min(t->_mana_max, t->_mana);
  }
  return t->_mana;
}

[[nodiscard]] auto thing_mana_decr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  game_request_to_remake_ui_set(g);

  if (static_cast< int >(t->_mana) - val <= 0) {
    return t->_mana = 0;
  }

  return t->_mana -= val;
}

[[nodiscard]] auto thing_mana_max(Gamep g, Levelsp v, Levelp l, Thingp t) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_mana_max;
}

[[nodiscard]] auto thing_mana_max_set(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }

  if (val > std::numeric_limits< decltype(t->_mana_max) >::max()) {
    thing_err(g, v, l, t, "value overflow: %d", val);
    return 0;
  }

  game_request_to_remake_ui_set(g);
  return t->_mana_max = val;
}

[[nodiscard]] auto thing_mana_max_incr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  game_request_to_remake_ui_set(g);
  return t->_mana_max += val;
}

[[nodiscard]] auto thing_mana_max_decr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  game_request_to_remake_ui_set(g);

  if (static_cast< int >(t->_mana_max) - val <= 0) {
    return t->_mana_max = 0;
  }

  return t->_mana_max -= val;
}
