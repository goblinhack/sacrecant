//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_game.hpp"
#include "my_main.hpp"
#include "my_thing.hpp"
#include "my_thing_inlines.hpp"
#include "my_types.hpp"

#include <algorithm>
#include <limits>

[[nodiscard]] auto thing_mana(Gamep g, Levelsp v, Levelp l, Thingp me) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return me->_mana;
}

[[nodiscard]] auto thing_mana_set(Gamep g, Levelsp v, Levelp l, Thingp me, int val) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    ERR("no thing pointer");
    return 0;
  }

  if (val > std::numeric_limits< decltype(me->_mana) >::max()) {
    thing_err(g, v, l, me, "value overflow: %d", val);
    return 0;
  }

  game_request_to_remake_ui_set(g);
  me->_mana = val;
  if (me->_mana_max != 0) {
    me->_mana = std::min(me->_mana_max, me->_mana);
  }

  if (thing_is_player(me)) {
    THING_DBG(g, v, l, me, "mana set to %d", me->_mana);
  }

  return me->_mana;
}

[[nodiscard]] auto thing_mana_incr(Gamep g, Levelsp v, Levelp l, Thingp me, int val) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  game_request_to_remake_ui_set(g);
  me->_mana += val;
  if (me->_mana_max != 0) {
    me->_mana = std::min(me->_mana_max, me->_mana);
  }
  return me->_mana;
}

[[nodiscard]] auto thing_mana_decr(Gamep g, Levelsp v, Levelp l, Thingp me, int val) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  game_request_to_remake_ui_set(g);

  if (static_cast< int >(me->_mana) - val <= 0) {
    return me->_mana = 0;
  }

  return me->_mana -= val;
}

[[nodiscard]] auto thing_mana_max(Gamep g, Levelsp v, Levelp l, Thingp me) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return me->_mana_max;
}

[[nodiscard]] auto thing_mana_max_set(Gamep g, Levelsp v, Levelp l, Thingp me, int val) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    ERR("no thing pointer");
    return 0;
  }

  if (val > std::numeric_limits< decltype(me->_mana_max) >::max()) {
    thing_err(g, v, l, me, "value overflow: %d", val);
    return 0;
  }

  game_request_to_remake_ui_set(g);
  return me->_mana_max = val;
}

[[nodiscard]] auto thing_mana_max_incr(Gamep g, Levelsp v, Levelp l, Thingp me, int val) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  game_request_to_remake_ui_set(g);
  return me->_mana_max += val;
}

[[nodiscard]] auto thing_mana_max_decr(Gamep g, Levelsp v, Levelp l, Thingp me, int val) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  game_request_to_remake_ui_set(g);

  if (static_cast< int >(me->_mana_max) - val <= 0) {
    return me->_mana_max = 0;
  }

  return me->_mana_max -= val;
}

auto thing_spell_mana_cost_for(Gamep g, Levelsp v, Levelp l, Thingp spell, Thingp user) -> int
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

  auto cost   = thing_spell_mana_cost(g, v, l, spell);
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

[[nodiscard]] auto thing_spell_mana_cost(Gamep g, Levelsp v, Levelp l, Thingp me) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return me->_spell_mana_cost;
}

[[nodiscard]] auto thing_spell_mana_cost_set(Gamep g, Levelsp v, Levelp l, Thingp me, int val) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return me->_spell_mana_cost = val;
}

[[nodiscard]] auto thing_spell_mana_cost_incr(Gamep g, Levelsp v, Levelp l, Thingp me, int val) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return me->_spell_mana_cost += val;
}

[[nodiscard]] auto thing_spell_mana_cost_decr(Gamep g, Levelsp v, Levelp l, Thingp me, int val) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  if (static_cast< int >(me->_spell_mana_cost) - val <= 0) {
    return me->_spell_mana_cost = 0;
  }
  return me->_spell_mana_cost -= val;
}
