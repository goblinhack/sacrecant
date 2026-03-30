//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_thing_inlines.hpp"

//
// Add a key to the things inventory
//
auto thing_collect_key(Gamep g, Levelsp v, Levelp l, Thingp it, Thingp carrier) -> bool
{
  TRACE();

  if (! thing_is_key(it)) {
    return false;
  }

  if (! thing_is_able_to_collect_keys(carrier)) {
    return false;
  }

  if (! thing_is_player(carrier) && ! thing_is_monst(carrier)) {
    thing_err(carrier, "unexpected thing for %s", __FUNCTION__);
    return false;
  }

  ThingEvent e {
      .reason     = "by carrying",       //
      .event_type = THING_EVENT_CARRIED, //
      .source     = carrier              //
  };

  thing_dead(g, v, l, it, e);

  (void) thing_keys_carried_incr(g, v, l, carrier, 1);

  return true;
}
