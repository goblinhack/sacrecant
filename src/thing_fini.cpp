//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_main.hpp"
#include "my_thing.hpp"
#include "my_thing_inlines.hpp"
#include "my_types.hpp"

void thing_fini(Gamep g, Levelsp v, Levelp l, Thingp t)
{
  TRACE();

  IF_DEBUG2
  {
    log("fini: thing id %08" PRIX32 "", t->id);
    THING_DBG(g, v, l, t, "fini");
  }

  if (! thing_is_dead(t)) {
    IF_DEBUG2 { THING_DBG(g, v, l, t, "fini, not dead, need to kill"); }

    ThingEvent e {
        .reason     = "fini event",     //
        .event_type = THING_EVENT_FINI, //
    };
    thing_dead(g, v, l, t, e);

    IF_DEBUG2 { THING_DBG(g, v, l, t, "fini, now dead"); }
  }

  IF_DEBUG2 { THING_DBG(g, v, l, t, "fini, free"); }

  thing_free(g, v, l, t);
}
