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
// - a bridge that is suddenly over a chasm due to the bridge burning
// - a thing that has fallen through a chasm but has landed on a chasm again
//
void level_tick_chasm(Gamep g, Levelsp v, Levelp l)
{
  TRACE();

  int x = 0;
  int y = 0;

  FOR_ALL_MAP_POINTS_NO_BREAK(g, v, l, x, y)
  {
    bpoint p(x, y);

    if (! level_is_chasm_bool(g, v, l, p)) {
      continue;
    }

    FOR_ALL_THINGS_AT(g, v, l, t, p)
    {
      if (! thing_is_able_to_fall(g, v, l, t)) {
        continue;
      }

      THING_DBG(g, v, l, t, "over chasm");

      thing_chasm_handle(g, v, l, t);
    }
  }
}
