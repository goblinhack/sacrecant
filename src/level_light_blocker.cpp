//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_globals.hpp"
#include "my_level.hpp"
#include "my_level_inlines.hpp"
#include "my_main.hpp"
#include "my_math.hpp"
#include "my_thing_inlines.hpp"
#include "my_tile.hpp"
#include "my_ui.hpp"

#include <array>
#include <cmath>
#include <cstring>

//
// Something blocking the fov?
//
auto level_light_blocker_at(Gamep g, Levelsp v, Levelp l, const bpoint &pov, Thingp me) -> Thingp
{
  FOR_ALL_THINGS_AT_UNSAFE(g, v, l, it, pov)
  {
    //
    // Light blocking and vision are mostly the same. However, the lighting algorithm
    // only really works with whole tiles. If things are submerged, it looks odd.
    //
    if (thing_submerged_pct(it) != 0) {
      continue;
    }

    if (thing_vision_blocker(g, v, l, me, it)) {
      return it;
    }
  }

  return nullptr;
}
