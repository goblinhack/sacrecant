//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_fpoint.hpp"
#include "my_main.hpp"
#include "my_math.hpp"
#include "my_thing.hpp"
#include "my_thing_inlines.hpp"
#include "my_tp.hpp"
#include "my_types.hpp"

#include <cstring>

[[nodiscard]] auto thing_missile_get_delta_from_dt(Gamep g, Levelsp v, Levelp l, Thingp t, float dt) -> fpoint
{
  TRACE();

  float s = 0;
  float c = 0;
  SINCOSF(t->angle, &s, &c);

  auto *player = thing_player(g);
  if (player == nullptr) [[unlikely]] {
    CROAK("no player struct found");
    return fpoint(0, 0);
  }

  const int   player_speed = thing_speed(g, v, l, player);
  const float t_speed      = thing_speed(g, v, l, t);
  const auto  tile_speed   = (t_speed / static_cast< float >(player_speed));

  auto delta = fpoint(c * dt * tile_speed, s * dt * tile_speed);
  if ((delta.x == 0) && (delta.y == 0)) {
    CROAK("no delta for missile sin %f cos %f dt %f tile_speed %f", s, c, dt, tile_speed);
  }

  return delta;
}

[[nodiscard]] auto thing_missile_get_direction(Gamep g, Levelsp v, Levelp l, Thingp t) -> fpoint
{
  TRACE();

  return unit(thing_missile_get_delta_from_dt(g, v, l, t, 1.0));
}

//
// Add a missile if possible
//
[[nodiscard]] auto thing_spawn_missile(Gamep g, Levelsp v, Levelp l, Thingp me, Tpp what, const fpoint target, ThingEvent *e) -> Thingp
{
  TRACE();

  if (me == nullptr) {
    return nullptr;
  }

  if (! thing_is_able_to_fire_weapons(me)) {
    thing_err(g, v, l, me, "thing trying to spawn missiles when it cannot");
    return nullptr;
  }

  if (what == nullptr) {
    thing_err(g, v, l, me, "no missile to spawn");
    return nullptr;
  }

  auto *ext_struct = thing_ext_struct(g, v, me);
  if (ext_struct == nullptr) {
    thing_err(g, v, l, me, "missing ext struct");
    return nullptr;
  }

  //
  // Too many missiles
  //
  if (thing_missile_fired_by_count_get(g, v, l, me) >= thing_missile_count_max(g, v, l, me)) {
    THING_DBG(g, v, l, me, "trying to fire too many missiles");
    thing_dump_missiles(g, v, l, me);
    return nullptr;
  }

  //
  // Look for a free slot
  //
  FOR_ALL_MISSILE_SLOTS(g, v, l, me, slot, existing_missile)
  {
    if (existing_missile != nullptr) {
      continue;
    }

    //
    // Create the missile. Should be no chance to fail now.
    //
    auto *new_missile = thing_spawn(g, v, l, what, target, e);
    if (new_missile == nullptr) {
      return nullptr;
    }

    memset(slot, 0, sizeof(*slot));
    slot->missile_id         = new_missile->id;
    new_missile->fired_by_id = me->id;
    ext_struct->missiles.count++;

    THING_DBG(g, v, l, me, "spawned missile %s", to_string(g, v, l, new_missile).c_str());
    THING_DBG(g, v, l, new_missile, "new born missile");

    return new_missile;
  }

  //
  // Out of slots; but we checked above
  //
  thing_err(g, v, l, me, "unexpectedly out of missile slots");

  return nullptr;
}

//
// Is this missile attached to a thing?
//
[[nodiscard]] auto thing_missile_fired_by_get(Gamep g, Levelsp v, Levelp l, Thingp me) -> Thingp
{
  if (me == nullptr) {
    return nullptr;
  }

  if (me->fired_by_id == 0U) {
    return nullptr;
  }

  return thing_find(g, v, me->fired_by_id);
}

//
// How many projectils?
//
[[nodiscard]] auto thing_missile_fired_by_count_get(Gamep g, Levelsp v, Levelp l, Thingp me) -> int
{
  if (me == nullptr) {
    return 0;
  }

  if (! thing_is_able_to_fire_weapons(me)) {
    return 0;
  }

  auto *ext_struct = thing_ext_struct(g, v, me);
  if (ext_struct == nullptr) {
    return 0;
  }

  return ext_struct->missiles.count;
}

//
// Detach or kill all missiles (or a specific one)
//
[[nodiscard]] static auto thing_missile_process_all(Gamep g, Levelsp v, Levelp l, Thingp me, Thingp specific_missile, ThingEvent &e) -> bool
{
  TRACE();

  if (me == nullptr) {
    return false;
  }

  if (! thing_is_able_to_fire_weapons(me)) {
    thing_err(g, v, l, me, "non owner trying to detach missiles");
    return false;
  }

  auto *ext_struct = thing_ext_struct(g, v, me);
  if (ext_struct == nullptr) {
    return false;
  }

  bool got_one = false;

  FOR_ALL_MISSILE_SLOTS(g, v, l, me, slot, missile)
  {
    if (missile == nullptr) {
      continue;
    }

    if (specific_missile != nullptr) {
      if (missile != specific_missile) {
        continue;
      }
    }

    if (! static_cast< bool >(missile->fired_by_id)) {
      thing_err(g, v, l, me, "found detached missile: %s", to_string(g, v, l, missile).c_str());
      return false;
    }

    if (ext_struct->missiles.count <= 0) {
      thing_err(g, v, l, me, "has unexpected missile count when detaching: %s", to_string(g, v, l, missile).c_str());
      return false;
    }

    ext_struct->missiles.count--;
    memset(slot, 0, sizeof(*slot));
    missile->fired_by_id = 0;

    if (e.event_type != THING_EVENT_NONE) {
      THING_DBG(g, v, l, me, "kill missile %s", to_string(g, v, l, missile).c_str());
      TRACE_INDENT();
      thing_dead(g, v, l, missile, e);
      got_one = true;
    } else {
      THING_DBG(g, v, l, me, "detach missile %s", to_string(g, v, l, missile).c_str());
      got_one = true;
    }
  }

  if (! got_one) {
    if (specific_missile) {
      THING_DBG(g, v, l, specific_missile, "could not detach missile");
      TRACE_INDENT();
      THING_DBG(g, v, l, me, "from me");
    } else if (ext_struct->missiles.count) {
      THING_DBG(g, v, l, me, "could not detach missile");
    }
  }

  return got_one;
}

//
// Detach all missiles from their owner
//
[[nodiscard]] auto thing_missile_detach_all_fired(Gamep g, Levelsp v, Levelp l, Thingp me) -> bool
{
  TRACE();

  ThingEvent e = {};
  return thing_missile_process_all(g, v, l, me, nullptr, e);
}

//
// Detach a missile from its owner
//
static auto thing_missile_detach_from_firer(Gamep g, Levelsp v, Levelp l, Thingp me, Thingp missile) -> bool
{
  TRACE();

  ThingEvent e = {};
  return thing_missile_process_all(g, v, l, me, missile, e);
}

//
// Detach a missile from its firer
//
[[nodiscard]] auto thing_missile_detach_me_from_firer(Gamep g, Levelsp v, Levelp l, Thingp me) -> bool
{
  TRACE();

  if (me == nullptr) {
    return false;
  }

  if (! thing_is_projectile(me) && ! thing_is_beam_weapon(me)) {
    thing_err(g, v, l, me, "non missile thing trying to detach itself");
    return false;
  }

  auto *fired_by = thing_missile_fired_by_get(g, v, l, me);
  if (fired_by == nullptr) {
    return false; // can be normal if detached
  }

  THING_DBG(g, v, l, me, "detach me from firer");
  THING_DBG(g, v, l, fired_by, "fired by me");
  TRACE_INDENT();

  return thing_missile_detach_from_firer(g, v, l, fired_by, me);
}

//
// Dump all missiles
//
void thing_dump_missiles(Gamep g, Levelsp v, Levelp l, Thingp me)
{
  TRACE();

  if (me == nullptr) {
    return;
  }

  if (! thing_is_able_to_fire_weapons(me)) {
    return;
  }

  auto *ext_struct = thing_ext_struct(g, v, me);
  if (ext_struct == nullptr) {
    return;
  }

  FOR_ALL_MISSILE_SLOTS(g, v, l, me, slot, existing_missile)
  {
    if (existing_missile == nullptr) {
      THING_DBG(g, v, l, me, "slot %d: -", _n_);
      continue;
    }

    auto s = to_string(g, v, l, existing_missile);
    THING_DBG(g, v, l, me, "slot %d: %s", _n_, s.c_str());
  }
}

[[nodiscard]] auto thing_missile_count_max(Gamep g, Levelsp v, Levelp l, Thingp t) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_missile_count_max;
}

[[nodiscard]] auto thing_missile_count_max_set(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }

  if (val > THING_MISSILE_MAX) {
    thing_err(g, v, l, t, "trying to set missile max too high");
    val = THING_MISSILE_MAX;
  }

  return t->_missile_count_max = val;
}

[[nodiscard]] auto thing_missile_count_max_incr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }

  if (t->_missile_count_max + val >= THING_MISSILE_MAX) {
    return t->_missile_count_max = THING_MISSILE_MAX;
  }

  return t->_missile_count_max += val;
}

[[nodiscard]] auto thing_missile_count_max_decr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  if (static_cast< int >(t->_missile_count_max) - val <= 0) {
    return t->_missile_count_max = 0;
  }
  return t->_missile_count_max -= val;
}
