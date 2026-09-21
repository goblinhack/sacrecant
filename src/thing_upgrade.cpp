//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_main.hpp"
#include "my_thing.hpp"
#include "my_thing_inlines.hpp"
#include "my_tp.hpp"
#include "my_types.hpp"

bool thing_is_upgradable(Gamep g, Levelsp v, Levelp l, Thingp me, TpSpellUpgrade u)
{
  TRACE();

  return thing_on_upgrade_possible(g, v, l, me, u);
}

bool thing_is_upgradable(Gamep g, Levelsp v, Levelp l, Thingp me)
{
  TRACE();

  for (auto i : tp_spell_upgrades_get(thing_tp(me))) {
    TpSpellUpgrade u = i.second;
    if (thing_on_upgrade_possible(g, v, l, me, u)) {
      return true;
    }
  }

  return false;
}

void thing_on_upgrade_possible_set(Tpp tp, thing_on_upgrade_possible_t callback)
{
  TRACE();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return;
  }
  tp->on_upgrade_possible = callback;
}

bool thing_on_upgrade_possible(Gamep g, Levelsp v, Levelp l, Thingp me, TpSpellUpgrade u)
{
  TRACE();
  auto *tp = thing_tp(me);
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return false;
  }
  if (tp->on_upgrade_possible == nullptr) {
    return false;
  }
  return tp->on_upgrade_possible(g, v, l, me, u);
}

void thing_on_upgrade_do_set(Tpp tp, thing_on_upgrade_do_t callback)
{
  TRACE();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return;
  }
  tp->on_upgrade_do = callback;
}

bool thing_on_upgrade_do(Gamep g, Levelsp v, Levelp l, Thingp me, TpSpellUpgrade u)
{
  TRACE();
  auto *tp = thing_tp(me);
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return false;
  }
  if (tp->on_upgrade_do == nullptr) {
    return false;
  }
  return tp->on_upgrade_do(g, v, l, me, u);
}
