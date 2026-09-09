//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_level_inlines.hpp" // NOLINT
#include "my_main.hpp"
#include "my_thing.hpp"
#include "my_thing_callbacks.hpp"
#include "my_thing_inlines.hpp" // NOLINT
#include "my_tp.hpp"
#include "my_tp_class.hpp"
#include "my_types.hpp"

[[nodiscard]] auto thing_is_levitating(Gamep g, Levelsp v, Levelp l, Thingp me) -> bool
{
  TRACE_DEBUG();

  if (me == nullptr) {
    ERR("no thing pointer");
    return false;
  }

  if (tp_flag(thing_tp(me), is_levitating) != 0) {
    return true;
  }

  if (! thing_is_able_to_be_levitated(me)) {
    return false;
  }

  FOR_ALL_HOOKS(g, v, l, me, buff)
  {
    if (thing_is_levitating(g, v, l, buff)) {
      return true;
    }
  }

  FOR_ALL_ACTIVE_ITEMS(g, v, l, me, item)
  {
    if (thing_is_levitating(g, v, l, item)) {
      return true;
    }
  }

  return false;
}

[[nodiscard]] auto thing_levitate(Gamep g, Levelsp v, Levelp l, Thingp me) -> bool
{
  TRACE_DEBUG();

  if (me == nullptr) {
    ERR("no thing pointer");
    return false;
  }

  if (tp_flag(thing_tp(me), is_levitating) != 0) {
    return true;
  }

  if (! thing_is_able_to_be_levitated(me)) {
    return false;
  }

  if (thing_buff_add(g, v, l, me, tp_find_mand("buff_levit")) == nullptr) {
    return false;
  }

  thing_on_levitated(g, v, l, me);

  return true;
}

void thing_on_levitated_set(Tpp tp, thing_on_levitated_t callback)
{
  TRACE();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return;
  }
  tp->on_levitated = callback;
}

void thing_on_levitated(Gamep g, Levelsp v, Levelp l, Thingp me)
{
  TRACE();
  auto *tp = thing_tp(me);
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return;
  }
  if (tp->on_levitated == nullptr) {
    return;
  }
  tp->on_levitated(g, v, l, me);
}
