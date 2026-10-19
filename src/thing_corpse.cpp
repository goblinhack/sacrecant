//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_game_defs.hpp"
#include "my_level.hpp"
#include "my_level_inlines.hpp" // NOLINT
#include "my_main.hpp"
#include "my_thing.hpp"
#include "my_thing_callbacks.hpp"
#include "my_thing_inlines.hpp" // NOLINT
#include "my_tp.hpp"
#include "my_types.hpp"

//
// Allowed to place a corpse here?
//
[[nodiscard]] auto thing_corpse_allowed(Gamep g, Levelsp v, Levelp l, Thingp t) -> bool
{
  TRACE();

  auto *tp = thing_tp(t);

  //
  // Leaves a corpse?
  //
  if (! thing_is_corpse_on_death(t)) {
    return false;
  }

  //
  // Block corpses for resurrection
  //
  if (thing_is_able_to_resurrect(t)) {
    switch (thing_assess_tile(g, v, l, thing_at(g, v, l, t), t)) {
      case THING_ENVIRON_HATES :    return false;
      case THING_ENVIRON_DISLIKES : return false;
      case THING_ENVIRON_NEUTRAL :  break;
      case THING_ENVIRON_LIKES :    break;
      case THING_ENVIRON_ENUM_MAX : break;
    }
  }

  //
  // Look for other corpses
  //
  auto corpse_count = 0;

  FOR_ALL_THINGS_AT_UNSAFE(g, v, l, o, thing_at(g, v, l, t))
  {
    if (o == t) {
      continue;
    }

    //
    // If there is a corpse of the same thing here, then avoid corpse pileup of similar things
    //
    if (thing_is_dead(o)) {
      if (tp == thing_tp(o)) {
        return false;
      }

      corpse_count++;
    }

    //
    // Again, but across species
    //
    if (corpse_count > THING_CORPSES_PER_TILE) {
      return false;
    }
  }

  return true;
}

[[nodiscard]] auto thing_is_corpse(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    no_thing_pointer();
    return false;
  }
  return t->_is_corpse;
}

void thing_is_corpse_set(Gamep g, Levelsp v, Levelp l, Thingp t, bool val)
{
  TRACE_DEBUG();

  if (t == nullptr) {
    no_thing_pointer();
    return;
  }

  if (t->_is_corpse == static_cast< int >(val)) {
    return;
  }
  t->_is_corpse = val;

  if (val) {
    THING_DBG(g, v, l, t, "is corpse set");
  }
}

void thing_is_corpse_unset(Gamep g, Levelsp v, Levelp l, Thingp t)
{
  TRACE_DEBUG();

  thing_is_corpse_set(g, v, l, t, false);
}

[[nodiscard]] auto thing_is_corpse_on_death(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    no_thing_pointer();
    return false;
  }
  return tp_flag(thing_tp(t), is_corpse_on_death) != 0;
}
