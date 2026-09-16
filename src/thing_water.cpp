//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_level.hpp"
#include "my_thing.hpp"
#include "my_tp.hpp"
#include "my_types.hpp"

void thing_water_handle(Gamep g, Levelsp v, Levelp l, Thingp me)
{
  TRACE();

  if (me->tick_water == v->tick) {
    return;
  }
  me->tick_water = v->tick;

  THING_DBG(g, v, l, me, "over water");

  auto   at     = thing_at(g, v, l, me);
  Thingp source = nullptr;
  if (level_is_water_deep(g, v, l, at) != nullptr) {
    source = level_is_water_deep(g, v, l, at);
  } else {
    source = level_is_water_shallow(g, v, l, at);
  }

  if (source == nullptr) {
    thing_err(g, v, l, me, "no source of water found to handle");
    return;
  }

  auto event_type = THING_EVENT_WATER_DAMAGE;
  auto damage     = thing_damage(g, v, l, source, event_type);

  ThingEvent e {
      .reason     = "by water damage", //
      .event_type = event_type,        //
      .damage     = damage,            //
      .source     = source,            //
  };

  THING_DBG(g, v, l, me, "apply water damage");
  TRACE_INDENT();

  thing_damage_apply(g, v, l, me, e);
}
