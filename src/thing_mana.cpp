//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_level.hpp"
#include "my_main.hpp"
#include "my_thing.hpp"
#include "my_thing_callbacks.hpp"
#include "my_thing_inlines.hpp"
#include "my_tp.hpp"
#include "my_types.hpp"
#include <string>

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

auto thing_mana_cost_for(Gamep g, Levelsp v, Levelp l, Thingp spell, Thingp user) -> int
{
  TRACE_DEBUG();

  if (spell == nullptr) {
    ERR("no thing pointer");
    return 0;
  }

  if (user == nullptr) {
    ERR("no thing pointer");
    return 0;
  }

  auto cost   = thing_mana_cost(g, v, l, spell);
  auto arcana = thing_spell_arcana(g, v, l, spell);

  switch (thing_stat_mod(g, v, l, user, arcana)) {
    case -9 : [[fallthrough]];
    case -8 : [[fallthrough]];
    case -7 : [[fallthrough]];
    case -6 : [[fallthrough]];
    case -5 : cost *= 2; break;
    case -4 : break;
    case -3 : break;
    case -2 : break;
    case -1 : break;
    case 0 :  break;
    case 1 :  break;
    case 2 :  break;
    case 3 :  break;
    case 4 :  break;
    case 5 :  [[fallthrough]];
    case 6 :  [[fallthrough]];
    case 7 :  [[fallthrough]];
    case 8 :  [[fallthrough]];
    case 9 :  cost /= 2; break;
  }

  if (cost <= 0) {
    cost = 1;
  }

  return cost;
}

[[nodiscard]] auto thing_mana_cost(Gamep g, Levelsp v, Levelp l, Thingp t) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_mana_cost;
}

[[nodiscard]] auto thing_mana_cost_set(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_mana_cost = val;
}

[[nodiscard]] auto thing_mana_cost_incr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_mana_cost += val;
}

[[nodiscard]] auto thing_mana_cost_decr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  if (static_cast< int >(t->_mana_cost) - val <= 0) {
    return t->_mana_cost = 0;
  }
  return t->_mana_cost -= val;
}
