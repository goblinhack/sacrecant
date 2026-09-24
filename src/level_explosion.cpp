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
// Handle things interacting with explosion
//
void level_tick_explosion(Gamep g, Levelsp v, Levelp l)
{
  TRACE();

  int x = 0;
  int y = 0;

  FOR_ALL_MAP_POINTS(g, v, l, x, y)
  {
    bpoint p(x, y);
    if (! level_is_explosion_bool(g, v, l, p)) {
      continue;
    }

    LEVEL_DBG(g, v, l, "handle explosion at (%d,%d)", p.x, p.y);
    TRACE_INDENT();

    FOR_ALL_THINGS_AT(g, v, l, t, p)
    {
      //
      // Skip dead monsters that take damage from explosion and leave a corpse?
      //
      if (thing_is_dead(t)) {
        THING_DBG(g, v, l, t, "over explosion, but dead");
        continue;
      }

      if (! thing_is_physics_explosion(t)) {
        THING_DBG(g, v, l, t, "over explosion, skip for explosion");
        continue;
      }

      THING_DBG(g, v, l, t, "over explosion");
      TRACE_INDENT();

      thing_explosion_handle(g, v, l, t);
    }
  }
}
