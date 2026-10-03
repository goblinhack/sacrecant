//
// Copyright goblinhack@gmail.com
//

#include "my_bpoint.hpp"
#include "my_callstack.hpp"
#include "my_level.hpp"
#include "my_level_inlines.hpp" // NOLINT
#include "my_thing.hpp"
#include "my_thing_inlines.hpp" // NOLINT
#include "my_types.hpp"

//
// Handle things interacting with chasms
//
// This is needed for special cases that do not normally do collision detection e.g.
// - fire spawned on top of a teleport
//
void level_tick_teleport(Gamep g, Levelsp v, Levelp l)
{
  TRACE();

  int x = 0;
  int y = 0;

  FOR_ALL_MAP_POINTS_NO_BREAK(g, v, l, x, y)
  {
    bpoint p(x, y);
    if (level_is_teleport_bool(g, v, l, p)) {
      FOR_ALL_THINGS_AT(g, v, l, t, p)
      {
        if (! thing_is_able_to_be_teleported(t)) {
          continue;
        }

        THING_DBG(g, v, l, t, "over teleport");

        (void) thing_teleport_handle(g, v, l, t);
      }
    }
  }
}
