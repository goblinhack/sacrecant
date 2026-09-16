//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_level.hpp"
#include "my_thing.hpp"
#include "my_tp.hpp"
#include "my_types.hpp"

void thing_explosion_handle(Gamep g, Levelsp v, Levelp l, Thingp me)
{
  TRACE();

  if (thing_is_ethereal(g, v, l, me)) {
    return;
  }

  auto  at         = thing_at(g, v, l, me);
  auto *source     = level_is_explosion(g, v, l, at);
  auto  event_type = THING_EVENT_EXPLOSION_DAMAGE;
  auto  damage     = thing_damage_calculate(g, v, l, source, event_type);

  ThingEvent e {
      .reason     = "by explosion damage", //
      .event_type = event_type,            //
      .damage     = damage,                //
      .source     = source,                //
  };

  THING_DBG(g, v, l, me, "apply explosion damage");
  TRACE_INDENT();

  thing_damage_apply(g, v, l, me, e);
}
