//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_main.hpp"
#include "my_thing.hpp"
#include "my_thing_inlines.hpp"
#include "my_tp.hpp"
#include "my_tp_inlines.hpp"
#include "my_types.hpp"

auto thing_spell_cast(Gamep g, Levelsp v, Levelp l, Thingp spell, Thingp user, int option) -> bool
{
  TRACE();

  topcon("todo cast spell %s option %d", thing_name_short(g, v, l, spell).c_str(), option);
  auto options = tp_spell_options_get(thing_tp(spell));
  if (option) {
    if (option >= (int) options.size()) {
      return false;
    }
  }

  ThingEvent e        = {};
  e.event_int_context = option;
  return thing_on_cast_request(g, v, l, spell, user, e);
}

auto thing_spell_arcana(Gamep g, Levelsp v, Levelp l, Thingp me) -> ThingStatType
{
  TRACE();

  if (thing_stat(g, v, l, me, THING_STAT_ARCANA_FIRE) > THING_STAT_DEFAULT) {
    return THING_STAT_ARCANA_FIRE;
  }
  if (thing_stat(g, v, l, me, THING_STAT_ARCANA_LIFE) > THING_STAT_DEFAULT) {
    return THING_STAT_ARCANA_LIFE;
  }
  if (thing_stat(g, v, l, me, THING_STAT_ARCANA_DEATH) > THING_STAT_DEFAULT) {
    return THING_STAT_ARCANA_DEATH;
  }

  thing_err(g, v, l, me, "no spell arcana");

  return THING_STAT_ARCANA_FIRE;
}

auto thing_spell_cost_for(Gamep g, Levelsp v, Levelp l, Thingp spell, Thingp user) -> int
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

  auto cost   = thing_spell_cost(spell);
  auto arcana = thing_spell_arcana(g, v, l, spell);

  switch (thing_stat_mod(g, v, l, user, arcana)) {
    case -9 : cost += 3; break;
    case -8 : cost += 2; break;
    case -7 : cost += 2; break;
    case -6 : cost += 2; break;
    case -5 : cost += 1; break;
    case -4 : cost += 1; break;
    case -3 : cost += 1; break;
    case -2 : break;
    case -1 : break;
    case 0 :  break;
    case 1 :  break;
    case 2 :  break;
    case 3 :  cost -= 1; break;
    case 4 :  cost -= 1; break;
    case 5 :  cost -= 1; break;
    case 6 :  cost -= 2; break;
    case 7 :  cost -= 2; break;
    case 8 :  cost -= 2; break;
    case 9 :  cost -= 3; break;
  }

  ThingStatType opposing_arcana {};
  bool          opposing_arcana_set {};

  if (arcana == THING_STAT_ARCANA_LIFE) {
    opposing_arcana_set = true;
    opposing_arcana     = THING_STAT_ARCANA_DEATH;
  }
  if (arcana == THING_STAT_ARCANA_DEATH) {
    opposing_arcana_set = true;
    opposing_arcana     = THING_STAT_ARCANA_LIFE;
  }

  if (opposing_arcana_set) {
    switch (thing_stat_mod(g, v, l, user, opposing_arcana)) {
      case -9 : cost -= 3; break;
      case -8 : cost -= 2; break;
      case -7 : cost -= 2; break;
      case -6 : cost -= 2; break;
      case -5 : cost -= 1; break;
      case -4 : cost -= 1; break;
      case -3 : cost -= 1; break;
      case -2 : break;
      case -1 : break;
      case 0 :  break;
      case 1 :  break;
      case 2 :  break;
      case 3 :  cost += 1; break;
      case 4 :  cost += 1; break;
      case 5 :  cost += 1; break;
      case 6 :  cost += 2; break;
      case 7 :  cost += 2; break;
      case 8 :  cost += 2; break;
      case 9 :  cost += 3; break;
    }
  }

  if (cost <= 0) {
    cost = 1;
  }

  return cost;
}

[[nodiscard]] auto thing_spell_cost(Thingp t) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_spell_cost;
}

[[nodiscard]] auto thing_spell_cost_set(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_spell_cost = val;
}

[[nodiscard]] auto thing_spell_cost_incr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_spell_cost += val;
}

[[nodiscard]] auto thing_spell_cost_decr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  if (static_cast< int >(t->_spell_cost) - val <= 0) {
    return t->_spell_cost = 0;
  }
  return t->_spell_cost -= val;
}

void thing_on_cast_request_set(Tpp tp, thing_on_cast_request_t callback)
{
  TRACE();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return;
  }
  tp->on_cast_request = callback;
}

[[nodiscard]] auto thing_on_cast_request(Gamep g, Levelsp v, Levelp l, Thingp item, Thingp user, ThingEvent &e) -> bool
{
  TRACE();
  auto *tp = thing_tp(item);
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return false;
  }

  if (tp->on_cast_request == nullptr) {
    return true;
  }

  if (! thing_is_player(user) && ! thing_is_monst(user)) {
    thing_err(g, v, l, user, "unexpected thing for %s", __FUNCTION__);
    return false;
  }
  return tp->on_cast_request(g, v, l, item, user, e);
}
