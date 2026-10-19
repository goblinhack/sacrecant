//
// Copyright goblinhack@gmail.com
//

#include "my_bpoint.hpp"
#include "my_callstack.hpp"
#include "my_fpoint.hpp"
#include "my_globals.hpp"
#include "my_level.hpp"
#include "my_main.hpp"
#include "my_thing.hpp"
#include "my_thing_callbacks.hpp"
#include "my_thing_inlines.hpp"
#include "my_tp.hpp"
#include "my_tp_class.hpp"
#include "my_types.hpp"

static void thing_is_spawned_set(Thingp me, bool val)
{
  TRACE_DEBUG();

  if (me == nullptr) {
    no_thing_pointer();
    return;
  }

  if (me->_is_spawned == static_cast< int >(val)) {
    return;
  }
  me->_is_spawned = val;
}

void thing_is_spawned_unset(Gamep g, Levelsp v, Levelp l, Thingp me)
{
  TRACE_DEBUG();

  thing_is_spawned_set(me, false);
}

[[nodiscard]] auto thing_spawn(Gamep g, Levelsp v, Levelp l, Tpp tp, const fpoint &at, ThingEvent *e) -> Thingp
{
  TRACE();

  auto *me = thing_init(g, v, l, tp, at);
  if (me == nullptr) {
    tp_log(tp, "failed to init thing");
    return nullptr;
  }

  if (! thing_push(g, v, l, me)) {
    THING_DBG(g, v, l, me, "failed to push");
    return nullptr;
  }

  //
  // This is reset each tick
  //
  if (thing_is_tick_end_delay(me)) {
    l->is_tick_end_delay = true;
  }

  THING_DBG(g, v, l, me, "spawned");

  //
  // Need to differentiate things spawned normally at game creation and those after
  //
  if (v->tick != 0U) {
    thing_is_spawned_set(me, true);
  }

  //
  // Needed for all things, to get items populated
  //
  thing_on_spawned(g, v, l, me, e);

  return me;
}

[[nodiscard]] auto thing_spawn(Gamep g, Levelsp v, Levelp l, Tpp tp, const bpoint &at, ThingEvent *e) -> Thingp
{
  return thing_spawn(g, v, l, tp, make_fpoint(at), e);
}

[[nodiscard]] auto thing_spawn(Gamep g, Levelsp v, Levelp l, Tpp tp, Thingp spawner, ThingEvent *e) -> Thingp
{
  TRACE();

  if (spawner == nullptr) {
    no_thing_pointer();
    return nullptr;
  }

  auto dir = thing_get_direction_grid(g, v, l, spawner);

  auto *spawned = thing_spawn(g, v, l, tp, thing_at(g, v, l, spawner), e);
  if (spawned == nullptr) {
    return nullptr;
  }

  thing_set_dir_from_delta(g, v, l, spawned, dir.x, dir.y);

  return spawned;
}

void thing_on_spawned_set(Tpp tp, thing_on_spawned_t callback)
{
  TRACE();
  if (tp == nullptr) [[unlikely]] {
    no_tp_pointer();
    return;
  }
  tp->on_spawned = callback;
}

void thing_on_spawned(Gamep g, Levelsp v, Levelp l, Thingp me, ThingEvent *e)
{
  TRACE();
  auto *tp = thing_tp(me);
  if (tp == nullptr) [[unlikely]] {
    no_tp_pointer();
    return;
  }
  if (tp->on_spawned == nullptr) {
    return;
  }
  tp->on_spawned(g, v, l, me, e);
}
