//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_globals.hpp"
#include "my_level.hpp"
#include "my_main.hpp"
#include "my_thing_callbacks.hpp"

void thing_is_spawned_set(Gamep g, Levelsp v, Levelp l, Thingp t, bool val)
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
    thing_on_spawned(g, v, l, t);
  }
}

void thing_is_spawned_unset(Gamep g, Levelsp v, Levelp l, Thingp t)
{
  TRACE_DEBUG();

  thing_is_spawned_set(g, v, l, t, false);
}

auto thing_spawn(Gamep g, Levelsp v, Levelp l, Tpp tp, const fpoint &at) -> Thingp
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
    thing_err(t, "failed to push");
    return nullptr;
  }

  {
    THING_DBG(t, "spawned");
  }

  //
  // This is reset each tick
  //
  if (thing_is_tick_end_delay(t)) {
    l->is_tick_end_delay = true;
  }

  thing_is_spawned_set(g, v, l, t);

  return t;
}

auto thing_spawn(Gamep g, Levelsp v, Levelp l, Tpp tp, const bpoint &at) -> Thingp { return thing_spawn(g, v, l, tp, make_fpoint(at)); }

auto thing_spawn(Gamep g, Levelsp v, Levelp l, Tpp tp, Thingp spawner) -> Thingp
{
  if (spawner == nullptr) {
    ERR("no thing pointer");
    return nullptr;
  }

  auto dir = thing_get_direction_grid(g, v, l, spawner);

  auto *spawned = thing_spawn(g, v, l, tp, thing_at(spawner));
  if (spawned == nullptr) {
    return nullptr;
  }

  thing_set_dir_from_delta(spawned, dir.x, dir.y);

  return spawned;
}
