//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_main.hpp"
#include "my_thing.hpp"
#include "my_thing_inlines.hpp"
#include "my_tp.hpp"
#include "my_types.hpp"
#include <limits>

//
// Add a key to the things inventory
//
[[nodiscard]] auto thing_collect_key(Gamep g, Levelsp v, Levelp l, Thingp owner, Thingp it) -> bool
{
  TRACE();

  if (! thing_is_key(it)) {
    return false;
  }

  if (! thing_is_able_to_collect_keys(owner)) {
    return false;
  }

  if (! thing_is_player(owner) && ! thing_is_monst(owner)) {
    thing_err(g, v, l, owner, "unexpected thing for %s", __FUNCTION__);
    return false;
  }

  ThingEvent e {
      .reason     = "by carrying",       //
      .event_type = THING_EVENT_CARRIED, //
      .source     = owner                //
  };

  THING_DBG(g, v, l, it, "dead due to carrying");
  TRACE_INDENT();

  thing_dead(g, v, l, it, e);

  (void) thing_keys_carried_incr(g, v, l, owner, 1);

  return true;
}

[[nodiscard]] auto thing_keys_carried(Gamep g, Levelsp v, Levelp l, Thingp me) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    no_thing_pointer();
    return 0;
  }

  auto *ext = thing_ext_struct(g, v, me);
  if (ext == nullptr) {
    return false;
  }

  return ext->_keys_carried;
}

[[nodiscard]] auto thing_keys_carried_set(Gamep g, Levelsp v, Levelp l, Thingp me, int val) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    no_thing_pointer();
    return 0;
  }

  auto *ext = thing_ext_struct(g, v, me);
  if (ext == nullptr) {
    return false;
  }

  if (val > std::numeric_limits< decltype(ext->_keys_carried) >::max()) {
    thing_err(g, v, l, me, "value overflow: %d", val);
    return 0;
  }

  return ext->_keys_carried = val;
}

[[nodiscard]] auto thing_keys_carried_incr(Gamep g, Levelsp v, Levelp l, Thingp me, int val) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    no_thing_pointer();
    return 0;
  }

  auto *ext = thing_ext_struct(g, v, me);
  if (ext == nullptr) {
    return 0;
  }

  return ext->_keys_carried += val;
}

[[nodiscard]] auto thing_keys_carried_decr(Gamep g, Levelsp v, Levelp l, Thingp me, int val) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    no_thing_pointer();
    return 0;
  }

  auto *ext = thing_ext_struct(g, v, me);
  if (ext == nullptr) {
    return 0;
  }

  if (static_cast< int >(ext->_keys_carried) - val <= 0) {
    return ext->_keys_carried = 0;
  }

  return ext->_keys_carried -= val;
}
