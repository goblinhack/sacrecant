//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_level.hpp"
#include "my_thing_callbacks.hpp"
#include "my_thing_inlines.hpp"

//
// Called at the beginning of each tick
//
void thing_tick_begin(Gamep g, Levelsp v, Levelp l, Thingp t)
{
  TRACE();

  //
  // Update hidden and submerged status
  //
  if (! thing_is_moving(t)) {
    //
    // Only if not moving, else the player appears to jump briefly forward
    // before being interpolated
    //
    thing_update_pos(g, v, l, t);
  }

  //
  // Reset damage counts.
  //
  (void) thing_damage_this_tick_set(g, v, l, t, 0);

  //
  // Per thing callback
  //
  thing_on_tick_begin(g, v, l, t);
  if (thing_is_dead(t)) {
    return;
  }

  //
  // Lifespan tick
  //
  if (thing_lifespan(t) != 0) {
    if (thing_lifespan_decr(g, v, l, t) == 0) {
      ThingEvent e {
          .reason     = "ran out of life",            //
          .event_type = THING_EVENT_LIFESPAN_EXPIRED, //
      };

      THING_DBG(t, "dead due to out of life");
      TRACE_INDENT();

      thing_dead(g, v, l, t, e);
      return;
    }
  }

  (void) thing_age_incr(g, v, l, t);

  thing_continue_to_burn_check(g, v, l, t);
  if (thing_is_dead(t)) {
    return;
  }
}

//
// Called when the level is idle. i.e. multiple times per tick
//
void thing_tick_idle(Gamep g, Levelsp v, Levelp l, Thingp t)
{
  TRACE();

  //
  // Per thing callback
  //
  thing_on_tick_idle(g, v, l, t);
}

//
// Called at the end of each tick
//
void thing_tick_end(Gamep g, Levelsp v, Levelp l, Thingp t)
{
  TRACE();

  //
  // Per thing callback
  //
  thing_on_tick_end(g, v, l, t);

  //
  // Mark the tick as completed
  //
  if (t->tick == v->tick) {
    return;
  }
  t->tick = v->tick;
}
