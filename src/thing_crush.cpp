//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_main.hpp" // NOLINT
#include "my_thing.hpp"
#include "my_tp.hpp"
#include "my_types.hpp"

//
// Add a key to the things inventory
//
// "me" here could be a barrel
//
[[nodiscard]] auto thing_crush(Gamep g, Levelsp v, Levelp l, Thingp me, Thingp crusher) -> bool
{
  TRACE();

  if (! thing_is_crushable(me)) {
    return false;
  }

  if (thing_is_flying(g, v, l, me) || thing_is_levitating(g, v, l, me)) {
    return false;
  }

  if (thing_is_flying(g, v, l, crusher) || thing_is_levitating(g, v, l, crusher)) {
    return false;
  }

  //
  // Crush grass?
  //
  if (thing_is_grass(me)) {
    if (! thing_is_able_to_crush_grass(crusher)) {
      return false;
    }
  }

  //
  // Crush me
  //
  ThingEvent e {
      .reason     = "by crushing",            //
      .event_type = THING_EVENT_CRUSH_DAMAGE, //
      .source     = crusher                   //
  };

  THING_DBG(g, v, l, me, "dead due to crushing");
  TRACE_INDENT();

  thing_dead(g, v, l, me, e);
  return true;
}
