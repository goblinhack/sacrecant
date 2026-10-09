//
// Copyright goblinhack@gmail.com
//

#include "../../my_callstack.hpp"
#include "../../my_cpp_template.hpp"
#include "../../my_random.hpp"
#include "../../my_thing.hpp"
#include "../../my_thing_callbacks.hpp"
#include "../../my_thing_inlines.hpp"
#include "../../my_tile.hpp"
#include "../../my_tp.hpp"
#include "../../my_tps.hpp"
#include "../../my_types.hpp"
#include "../../my_ui.hpp"

//
// Open all exits
//
void tp_boss_on_death(Gamep g, Levelsp v, Levelp l, Thingp me, ThingEvent &e)
{
  TRACE();

  FOR_ALL_THINGS_ON_LEVEL_NO_BREAK(g, v, l, it)
  {
    if (thing_is_exit_closed(it)) {
      THING_DBG(g, v, l, me, "open exit");
      TRACE_INDENT();

      thing_dead(g, v, l, it, e);

      if (thing_spawn(g, v, l, tp_first(is_exit), thing_at(g, v, l, it))) {
        topcon("You hear the sound of a boarded up exit being pried open.\n");
      } else {
        topcon("Something went wrong. The exit failed to open!");
      }
    }
  }
}
