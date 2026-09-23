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
#include "my_ui.hpp"

#include <string>

[[nodiscard]] static auto thing_spell_cast_do(Gamep g, Levelsp v, Levelp l, Thingp spell, Thingp user, ThingEvent &e) -> bool;

auto thing_spell_cast(Gamep g, Levelsp v, Levelp l, Thingp spell, Thingp user, const std::string &option_name) -> bool
{
  TRACE();

  if (option_name.empty()) {
    //
    // Default option
    //
  } else {
    //
    // Check the option exists
    //
    bool found_it {};

    for (const auto &o : tp_spell_options_get(thing_tp(spell))) {
      auto option = o.second;
      if (option.name == option_name) {
        found_it = true;
        break;
      }
    }

    if (! found_it) {
      thing_err(g, v, l, spell, "spell option %s not found", option_name.c_str());
      return false;
    }
  }

  ThingEvent e             = {};
  e.event_type             = THING_EVENT_SPELL_DAMAGE;
  e.source                 = user;
  e.spell_info.spell       = spell;
  e.spell_info.option_name = option_name;

  if (option_name.empty()) {
    THING_DBG(g, v, l, user, "casting (default option)");
  } else {
    THING_DBG(g, v, l, user, "casting (option %s)", option_name.c_str());
  }
  TRACE_INDENT();
  THING_DBG(g, v, l, spell, "this");

  return thing_spell_cast_do(g, v, l, spell, user, e);
}

auto thing_spell_cast_target(Gamep g, Levelsp v, Levelp l, ThingEventp e) -> bool
{
  TRACE();

  if (e == nullptr) {
    return false;
  }

  auto *spell = e->spell_info.spell;
  if (spell == nullptr) {
    err("cannot cast spell, none set");
    return false;
  }

  auto *user = e->source;
  if (user == nullptr) {
    thing_err(g, v, l, spell, "cannot cast spell, no caster");
    return false;
  }

  if (! e->spell_info.target_set) {
    thing_err(g, v, l, user, "cannot cast spell, no target set");
    return false;
  }

  THING_DBG(g, v, l, user, "casting at %d,%d (option %s)", e->spell_info.target.x, e->spell_info.target.y, e->spell_info.option_name.c_str());
  TRACE_INDENT();
  THING_DBG(g, v, l, spell, "this");

  return thing_spell_cast_do(g, v, l, spell, user, *e);
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

  auto cost   = thing_spell_cost(g, v, l, spell);
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

[[nodiscard]] auto thing_spell_cost(Gamep g, Levelsp v, Levelp l, Thingp t) -> int
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

void thing_on_cast_do_set(Tpp tp, thing_on_cast_do_t callback)
{
  TRACE();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return;
  }
  tp->on_cast_do = callback;
}

[[nodiscard]] static auto thing_spell_cast_do(Gamep g, Levelsp v, Levelp l, Thingp spell, Thingp user, ThingEvent &e) -> bool
{
  TRACE();

  auto *tp = thing_tp(spell);
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

  auto old_state = game_state(g);
  auto ok        = tp->on_cast_request(g, v, l, spell, user, e);
  auto new_state = game_state(g);

  if (! ok) {
    return ok;
  }

  auto cost  = thing_spell_mana_cost(g, v, l, spell);
  auto avail = thing_mana(g, v, l, user);
  auto name  = thing_name_long(g, v, l, spell);

  if (avail < cost) {
    if (thing_is_player(user)) {
      topcon(UI_WARN_FMT_STR "You don't have enough Mana for casting spell %s." UI_RESET_FMT, name.c_str());
      game_state_reset(g, "failed to cast spell");
    }
    return false;
  }

  //
  // Probably looking for a target.
  //
  if (old_state != new_state) {
    return ok;
  }

  if (! tp->on_cast_do(g, v, l, spell, user, e)) {
    topcon(UI_WARN_FMT_STR "You fail to cast spell %s." UI_RESET_FMT, name.c_str());
    (void) level_tick_begin_requested(g, v, l, "player cast a spell");
    return false;
  }

  (void) thing_mana_decr(g, v, l, user, cost);
  if (thing_is_player(user)) {
    topcon("You spent %d Mana on casting spell %s.", cost, name.c_str());
    (void) level_tick_begin_requested(g, v, l, "player cast a spell");
  }

  return true;
}

[[nodiscard]] auto thing_spell_radius(Gamep g, Levelsp v, Levelp l, Thingp me) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return me->_spell_radius;
}

[[nodiscard]] auto thing_spell_radius_set(Gamep g, Levelsp v, Levelp l, Thingp me, int val) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    ERR("no thing pointer");
    return 0;
  }

  if (val > std::numeric_limits< decltype(me->_spell_radius) >::max()) {
    thing_err(g, v, l, me, "value overflow: %d", val);
    return 0;
  }

  game_request_to_remake_ui_set(g);

  me->_spell_radius = val;
  if (me->_spell_radius_max != 0) {
    me->_spell_radius = std::min(me->_spell_radius_max, me->_spell_radius);
  }

  if (thing_is_spell(me)) {
    THING_DBG(g, v, l, me, "spell radius set to %d", me->_spell_radius);
  }

  return me->_spell_radius;
}

[[nodiscard]] auto thing_spell_radius_incr(Gamep g, Levelsp v, Levelp l, Thingp me, int val) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    ERR("no thing pointer");
    return 0;
  }

  return thing_spell_radius_set(g, v, l, me, me->_spell_radius + val);
}

[[nodiscard]] auto thing_spell_radius_decr(Gamep g, Levelsp v, Levelp l, Thingp me, int val) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    ERR("no thing pointer");
    return 0;
  }

  if (static_cast< int >(me->_spell_radius) - val <= 0) {
    return me->_spell_radius = 0;
  }

  return thing_spell_radius_set(g, v, l, me, me->_spell_radius - val);
}

[[nodiscard]] auto thing_spell_radius_max(Gamep g, Levelsp v, Levelp l, Thingp me) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return me->_spell_radius_max;
}

[[nodiscard]] auto thing_spell_radius_max_set(Gamep g, Levelsp v, Levelp l, Thingp me, int val) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    ERR("no thing pointer");
    return 0;
  }

  if (val > std::numeric_limits< decltype(me->_spell_radius_max) >::max()) {
    thing_err(g, v, l, me, "value overflow: %d", val);
    return 0;
  }

  game_request_to_remake_ui_set(g);

  auto new_spell_radius_max = me->_spell_radius_max = val;

  if (me->_spell_radius > new_spell_radius_max) {
    (void) thing_spell_radius_set(g, v, l, me, new_spell_radius_max);
  }

  if (thing_is_spell(me)) {
    THING_DBG(g, v, l, me, "spell radius max set to %d", me->_spell_radius_max);
  }

  return new_spell_radius_max;
}

[[nodiscard]] auto thing_spell_radius_max_incr(Gamep g, Levelsp v, Levelp l, Thingp me, int val) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return thing_spell_radius_max_set(g, v, l, me, me->_spell_radius_max + val);
}

[[nodiscard]] auto thing_spell_radius_max_decr(Gamep g, Levelsp v, Levelp l, Thingp me, int val) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    ERR("no thing pointer");
    return 0;
  }

  if (static_cast< int >(me->_spell_radius_max) - val <= 0) {
    return me->_spell_radius_max = 0;
  }

  return thing_spell_radius_max_set(g, v, l, me, me->_spell_radius_max - val);
}

[[nodiscard]] auto thing_spell_range(Gamep g, Levelsp v, Levelp l, Thingp me) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return me->_spell_range;
}

[[nodiscard]] auto thing_spell_range_set(Gamep g, Levelsp v, Levelp l, Thingp me, int val) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    ERR("no thing pointer");
    return 0;
  }

  if (val > std::numeric_limits< decltype(me->_spell_range) >::max()) {
    thing_err(g, v, l, me, "value overflow: %d", val);
    return 0;
  }

  game_request_to_remake_ui_set(g);

  me->_spell_range = val;
  if (me->_spell_range_max != 0) {
    me->_spell_range = std::min(me->_spell_range_max, me->_spell_range);
  }

  if (thing_is_spell(me)) {
    THING_DBG(g, v, l, me, "spell range set to %d", me->_spell_range);
  }

  return me->_spell_range;
}

[[nodiscard]] auto thing_spell_range_incr(Gamep g, Levelsp v, Levelp l, Thingp me, int val) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    ERR("no thing pointer");
    return 0;
  }

  return thing_spell_range_set(g, v, l, me, me->_spell_range + val);
}

[[nodiscard]] auto thing_spell_range_decr(Gamep g, Levelsp v, Levelp l, Thingp me, int val) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    ERR("no thing pointer");
    return 0;
  }

  if (static_cast< int >(me->_spell_range) - val <= 0) {
    return me->_spell_range = 0;
  }

  return thing_spell_range_set(g, v, l, me, me->_spell_range - val);
}

[[nodiscard]] auto thing_spell_range_max(Gamep g, Levelsp v, Levelp l, Thingp me) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return me->_spell_range_max;
}

[[nodiscard]] auto thing_spell_range_max_set(Gamep g, Levelsp v, Levelp l, Thingp me, int val) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    ERR("no thing pointer");
    return 0;
  }

  if (val > std::numeric_limits< decltype(me->_spell_range_max) >::max()) {
    thing_err(g, v, l, me, "value overflow: %d", val);
    return 0;
  }

  game_request_to_remake_ui_set(g);

  auto new_spell_range_max = me->_spell_range_max = val;

  if (me->_spell_range > new_spell_range_max) {
    (void) thing_spell_range_set(g, v, l, me, new_spell_range_max);
  }

  if (thing_is_spell(me)) {
    THING_DBG(g, v, l, me, "spell range max set to %d", me->_spell_range_max);
  }

  return new_spell_range_max;
}

[[nodiscard]] auto thing_spell_range_max_incr(Gamep g, Levelsp v, Levelp l, Thingp me, int val) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return thing_spell_range_max_set(g, v, l, me, me->_spell_range_max + val);
}

[[nodiscard]] auto thing_spell_range_max_decr(Gamep g, Levelsp v, Levelp l, Thingp me, int val) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    ERR("no thing pointer");
    return 0;
  }

  if (static_cast< int >(me->_spell_range_max) - val <= 0) {
    return me->_spell_range_max = 0;
  }

  return thing_spell_range_max_set(g, v, l, me, me->_spell_range_max - val);
}

[[nodiscard]] auto thing_is_able_to_cast_spells(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_able_to_cast_spells) != 0;
}

[[nodiscard]] auto thing_is_unused_spell(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused_spell) != 0;
}

[[nodiscard]] auto thing_is_spell(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_spell) != 0;
}
