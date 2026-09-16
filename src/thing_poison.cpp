//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_level_inlines.hpp" // NOLINT
#include "my_thing.hpp"
#include "my_tp.hpp"
#include "my_types.hpp"

//
// Poison the poor thing
//
auto thing_attack_poison(Gamep g, Levelsp v, Levelp l, Thingp it) -> bool
{
  TRACE();

  auto event_type = THING_EVENT_POISON_DAMAGE;
  auto damage     = 1;

  ThingEvent e {
      .reason     = "poison",   //
      .event_type = event_type, //
      .damage     = damage,     //
  };

  THING_DBG(g, v, l, it, "apply poison damage");
  TRACE_INDENT();

  thing_damage_apply(g, v, l, it, e);

  return true;
}
