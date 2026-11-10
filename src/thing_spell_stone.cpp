//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_thing.hpp"
#include "my_thing_inlines.hpp"
#include "my_tp.hpp"
#include "my_types.hpp"

//
// Add a spell_stone to the things inventory
//
[[nodiscard]] auto thing_collect_spell_stone(Gamep g, Levelsp v, Levelp l, Thingp owner, Thingp it) -> bool
{
  TRACE();

  if (! thing_is_spell_stone(it)) {
    return false;
  }

  if (! thing_is_able_to_collect_spell_stones(owner)) {
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

  return true;
}
