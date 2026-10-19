//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_thing.hpp"
#include "my_thing_inlines.hpp"
#include "my_tp.hpp"
#include "my_types.hpp"

[[nodiscard]] auto thing_resurrect(Gamep g, Levelsp v, Levelp l, Thingp me) -> bool
{
  THING_DBG(g, v, l, me, "resurrect");
  TRACE_INDENT();

  if (! thing_is_able_to_resurrect(me)) {
    THING_DBG(g, v, l, me, "resurrect; not allowed");
    return false;
  }

  auto *tp = thing_tp(me);

  thing_is_dead_unset(g, v, l, me);
  thing_is_corpse_unset(g, v, l, me);
  thing_is_burning_unset(g, v, l, me);

  (void) thing_health_set(g, v, l, me, tp_health_max_get(tp) / 2);
  (void) thing_lifespan_initial_set(g, v, l, me, thing_lifespan_set(g, v, l, me, tp_lifespan_get(tp)));
  (void) thing_ticks_to_stay_dead_initial_set(g, v, l, me, thing_ticks_to_stay_dead_set(g, v, l, me, tp_ticks_to_stay_dead_get(tp)));

  thing_anim_init(g, v, l, me, THING_ANIM_IDLE);

  return true;
}

[[nodiscard]] auto thing_ticks_to_stay_dead(Gamep g, Levelsp v, Levelp l, Thingp me) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    no_thing_pointer();
    return 0;
  }

  auto *ext = thing_ext_struct(g, v, me);
  if (ext == nullptr) {
    return 0;
  }

  return ext->_ticks_to_stay_dead;
}

[[nodiscard]] auto thing_ticks_to_stay_dead_set(Gamep g, Levelsp v, Levelp l, Thingp me, int val) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    no_thing_pointer();
    return 0;
  }

  auto *ext = thing_ext_struct(g, v, me);
  if (ext == nullptr) {
    return 0;
  }

  return ext->_ticks_to_stay_dead = val;
}

[[nodiscard]] auto thing_ticks_to_stay_dead_incr(Gamep g, Levelsp v, Levelp l, Thingp me, int val) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    no_thing_pointer();
    return 0;
  }

  auto *ext = thing_ext_struct(g, v, me);
  if (ext == nullptr) {
    return 0;
  }

  return ext->_ticks_to_stay_dead += val;
}

[[nodiscard]] auto thing_ticks_to_stay_dead_decr(Gamep g, Levelsp v, Levelp l, Thingp me, int val) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    no_thing_pointer();
    return 0;
  }

  auto *ext = thing_ext_struct(g, v, me);
  if (ext == nullptr) {
    return 0;
  }

  if (static_cast< int >(ext->_ticks_to_stay_dead) - val <= 0) {
    return ext->_ticks_to_stay_dead = 0;
  }
  return ext->_ticks_to_stay_dead -= val;
}

[[nodiscard]] auto thing_ticks_to_stay_dead_initial(Gamep g, Levelsp v, Levelp l, Thingp me) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    no_thing_pointer();
    return 0;
  }

  auto *ext = thing_ext_struct(g, v, me);
  if (ext == nullptr) {
    return 0;
  }

  return ext->_ticks_to_stay_dead_initial;
}

[[nodiscard]] auto thing_ticks_to_stay_dead_initial_set(Gamep g, Levelsp v, Levelp l, Thingp me, int val) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    no_thing_pointer();
    return 0;
  }

  auto *ext = thing_ext_struct(g, v, me);
  if (ext == nullptr) {
    return 0;
  }

  return ext->_ticks_to_stay_dead_initial = val;
}

[[nodiscard]] auto thing_ticks_to_stay_dead_initial_incr(Gamep g, Levelsp v, Levelp l, Thingp me, int val) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    no_thing_pointer();
    return 0;
  }

  auto *ext = thing_ext_struct(g, v, me);
  if (ext == nullptr) {
    return 0;
  }

  return ext->_ticks_to_stay_dead_initial += val;
}

[[nodiscard]] auto thing_ticks_to_stay_dead_initial_decr(Gamep g, Levelsp v, Levelp l, Thingp me, int val) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    no_thing_pointer();
    return 0;
  }

  auto *ext = thing_ext_struct(g, v, me);
  if (ext == nullptr) {
    return 0;
  }

  if (static_cast< int >(ext->_ticks_to_stay_dead_initial) - val <= 0) {
    return ext->_ticks_to_stay_dead_initial = 0;
  }
  return ext->_ticks_to_stay_dead_initial -= val;
}

[[nodiscard]] auto thing_is_able_to_resurrect(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    no_thing_pointer();
    return false;
  }
  return tp_flag(thing_tp(t), is_able_to_resurrect) != 0;
}
