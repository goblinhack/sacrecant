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

static void thing_is_spawned_set(Gamep g, Levelsp v, Levelp l, Thingp t, ThingEvent *e, bool val)
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return;
  }

  if (t->_is_spawned == static_cast< int >(val)) {
    return;
  }
  t->_is_spawned = val;

  if (val) {
    //
    // Callback upon spawning
    //
    thing_on_spawned(g, v, l, t, e);
  }
}

void thing_is_spawned_unset(Gamep g, Levelsp v, Levelp l, Thingp t)
{
  TRACE_DEBUG();

  thing_is_spawned_set(g, v, l, t, nullptr, false);
}

[[nodiscard]] auto thing_spawn(Gamep g, Levelsp v, Levelp l, Tpp tp, const fpoint &at, ThingEvent *e) -> Thingp
{
  TRACE();

  auto *t = thing_init(g, v, l, tp, at);
  if (t == nullptr) {
    if (g_opt_test_current == "test_alloc_free_monsts") {
      tp_log(tp, "failed to init thing");
    } else {
      tp_err(tp, "failed to init thing");
    }
    return nullptr;
  }

  if (! thing_push(g, v, l, t)) {
    thing_err(g, v, l, t, "failed to push");
    return nullptr;
  }

  {
    THING_DBG(g, v, l, t, "spawned");
  }

  //
  // This is reset each tick
  //
  if (thing_is_tick_end_delay(t)) {
    l->is_tick_end_delay = true;
  }

  thing_is_spawned_set(g, v, l, t, e, true);

  return t;
}

[[nodiscard]] auto thing_spawn(Gamep g, Levelsp v, Levelp l, Tpp tp, const bpoint &at, ThingEvent *e) -> Thingp
{
  return thing_spawn(g, v, l, tp, make_fpoint(at), e);
}

[[nodiscard]] auto thing_spawn(Gamep g, Levelsp v, Levelp l, Tpp tp, Thingp spawner, ThingEvent *e) -> Thingp
{
  TRACE();

  if (spawner == nullptr) {
    ERR("no thing pointer");
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
    ERR("no thing template pointer");
    return;
  }
  tp->on_spawned = callback;
}

void thing_on_spawned(Gamep g, Levelsp v, Levelp l, Thingp me, ThingEvent *e)
{
  TRACE();
  auto *tp = thing_tp(me);
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return;
  }
  if (tp->on_spawned == nullptr) {
    return;
  }
  tp->on_spawned(g, v, l, me, e);
}
