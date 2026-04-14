//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_level_inlines.hpp"
#include "my_thing_callbacks.hpp"
#include "my_thing_inlines.hpp"

//
// Returns true if the thing can move to this location
//
auto thing_can_move_to_diagonal_is_blocked(Gamep g, Levelsp v, Levelp l, Thingp me, bpoint to) -> bool
{
  TRACE();

  if (is_oob_or_border(to)) [[unlikely]] {
    return false;
  }

  auto at = thing_at(me);
  if (to == at) {
    return false;
  }

  if (thing_is_able_to_walk_through_walls(me)) {
    return false;
  }

  if (! adjacent_diagonal(at, to)) {
    return false;
  }

  //
  // Don't allow shortcuts through diagonal walls
  //
  // Don't allow shortcuts across chasms, hence _possible instead of _ai
  //
  if ((to.x == at.x - 1) && (to.y == at.y - 1)) {
    if (! thing_can_move_to_possible(g, v, l, me, bpoint(at.x - 1, at.y))
        && ! thing_can_move_to_possible(g, v, l, me, bpoint(at.x, at.y - 1))) {
      //
      // Block these paths
      //

      // b#.
      // #a.
      // ...
      THING_DBG(me, "check diagonal move to %d,%d; blocked (a)", to.x, to.y);
      return true;
    }
  }

  if ((to.x == at.x - 1) && (to.y == at.y + 1)) {
    if (! thing_can_move_to_possible(g, v, l, me, bpoint(at.x - 1, at.y))
        && ! thing_can_move_to_possible(g, v, l, me, bpoint(at.x, at.y + 1))) {
      //
      // Block these paths
      //

      // ...
      // #a.
      // b#.
      THING_DBG(me, "check diagonal move to %d,%d; blocked (b)", to.x, to.y);
      return true;
    }
  }

  if ((to.x == at.x + 1) && (to.y == at.y - 1)) {
    if (! thing_can_move_to_possible(g, v, l, me, bpoint(at.x + 1, at.y))
        && ! thing_can_move_to_possible(g, v, l, me, bpoint(at.x, at.y - 1))) {
      //
      // Block these paths
      //

      // .#b
      // .a#
      // ...
      THING_DBG(me, "check diagonal move to %d,%d; blocked (c)", to.x, to.y);
      return true;
    }
  }

  if ((to.x == at.x + 1) && (to.y == at.y + 1)) {
    if (! thing_can_move_to_possible(g, v, l, me, bpoint(at.x + 1, at.y))
        && ! thing_can_move_to_possible(g, v, l, me, bpoint(at.x, at.y + 1))) {
      //
      // Block these paths
      //

      // ...
      // .a#
      // .#b
      THING_DBG(me, "check diagonal move to %d,%d; blocked (d)", to.x, to.y);
      return true;
    }
  }

  THING_DBG(me, "check diagonal move to %d,%d; ok", to.x, to.y);
  return false;
}

//
// Returns true if the thing can move to this location
//
auto thing_can_move_to_attempt(Gamep g, Levelsp v, Levelp l, Thingp me, bpoint to) -> bool
{
  TRACE();

  if (is_oob_or_border(to)) [[unlikely]] {
    (void) thing_lunge(g, v, l, me, to);
    return false;
  }

  auto at = thing_at(me);
  if (to == at) {
    return true;
  }

  THING_DBG(me, "can move to attempt to %d,%d", to.x, to.y);

  if (! adjacent(at, to)) {
    THING_DBG(me, "can move to attempt to %d,%d; not adjacent", to.x, to.y);
    return false;
  }

  auto dx = to.x - at.x;
  auto dy = to.y - at.y;
  thing_set_dir_from_delta(me, dx, dy);

  //
  // Don't allow shortcuts through diagonal walls
  //
  // Don't allow shortcuts across chasms, hence _possible instead of _ai
  //
  if (thing_can_move_to_diagonal_is_blocked(g, v, l, me, to)) {
    (void) thing_lunge(g, v, l, me, to);
    return false;
  }

  if (! thing_can_move_to_possible(g, v, l, me, to)) {
    (void) thing_lunge(g, v, l, me, to);
    return false;
  }

  return true;
}

//
// Returns true if for the purposes of AI, a monst would be able to move
// to this location. For a chasm, it is unlikely a monst would want to if
// it means falling to its demise.
//
// True - can move here
// False - cannot move here, or don't want to move here
//
auto thing_can_move_to_ai(Gamep g, Levelsp v, Levelp l, Thingp me, bpoint to) -> bool
{
  TRACE_DEBUG();

  FOR_ALL_THINGS_AT_UNSAFE(g, v, l, it, to)
  {
    //
    // Allow walking over the dead
    //
    if (thing_is_dead(it)) {
      if (thing_is_obs_when_dead(it)) {
        return false;
      }
      continue;
    }

    //
    // Allow AI to make a path through unlocked doors
    //
    if (thing_is_door_unlocked(it)) {
      if (! thing_is_able_to_open(me)) {
        return false;
      }
    }

    //
    // We allow paths to be made through chasms, lava, monsters. This allows
    // the monster to try and reach the player over these. If it cannot, it
    // will just lunge at the player
    //

    //
    // Allow AI to make a path through walls
    //
    if (thing_is_wall(it)) {
      if (thing_is_able_to_move_through_walls(me)) {
        continue;
      }
    }

    //
    // A wall or pillar or somesuch?
    //
    if (thing_is_obs_to_paths(it)) {
      return false;
    }
  }

  return true;
}

//
// Returns true if it is POSSIBLE for this thing to move to this location.
// e.g. it is possible for a chasm or monst to move into a chasm. Does not
// mean the monst wants to do that.
//
// We do not change direction upon this attempt; purely a check only
//
auto thing_can_move_to_possible(Gamep g, Levelsp v, Levelp l, Thingp me, bpoint to) -> bool
{
  TRACE_DEBUG();

  //
  // If a minion, check we're not moving too far from the mob
  //
  if (thing_is_minion(me)) {
    if (! thing_minion_can_move_to_possible(g, v, l, me, to)) {
      return false;
    }
  }

  FOR_ALL_THINGS_AT_UNSAFE(g, v, l, it, to)
  {
    //
    // Allow walking over the dead
    //
    if (thing_is_dead(it)) {
      if (thing_is_obs_when_dead(it)) {
        return false;
      }
      continue;
    }

    //
    // Allow walking through unlocked doors
    //
    if (thing_is_door_unlocked(it)) {
      if (! thing_is_able_to_open(me)) {
        return false;
      }
    }

    //
    // Allow walking through walls
    //
    if (thing_is_wall(it)) {
      if (thing_is_able_to_move_through_walls(me)) {
        continue;
      }
    }

    //
    // AI will create paths over chasms. This allows monsters to try to cross
    // them. Howver if they cannot jump over, then they will just lunge at the
    // player.
    //
    if (thing_is_chasm(it)) {
      if (thing_is_monst(me)) {
        if (! thing_is_able_to_fall(me)) {
          continue;
        }
        if (! thing_is_able_to_jump(me)) {
          return false;
        }
      }
    }

    //
    // AI will create paths over lava. This allows monsters to try to cross
    // them. Howver if they cannot jump over, then they will just lunge at the
    // player.
    //
    if (thing_is_lava(it)) {
      if (thing_is_monst(me)) {
        if (thing_is_burnable(me)) {
          return false;
        }

        if (thing_is_combustible(me)) {
          return false;
        }
      }
    }

    //
    // No stacking of monsters
    //
    if (thing_is_monst(me)) {
      if (thing_is_monst(it)) {
        return false;
      }
    }

    //
    // A wall or pillar or somesuch?
    //
    if (thing_is_obs_to_movement(it)) {
      return false;
    }

    //
    // Avoid tiles we do not like
    //
    switch (thing_assess_tile(g, v, l, to, me)) {
      case THING_ENVIRON_HATES :    return false;
      case THING_ENVIRON_DISLIKES :
      case THING_ENVIRON_NEUTRAL :
      case THING_ENVIRON_LIKES :
      case THING_ENVIRON_ENUM_MAX : break;
    }
  }

  return true;
}

//
// Returns true if we can move to this location by shoving
//
auto thing_can_move_to_attempt_by_shoving(Gamep g, Levelsp v, Levelp l, Thingp me, bpoint to) -> bool
{
  TRACE();

  if (is_oob_or_border(to)) [[unlikely]] {
    return false;
  }

  auto at = thing_at(me);
  if (to == at) {
    return true;
  }

  auto dx = to.x - at.x;
  auto dy = to.y - at.y;
  thing_set_dir_from_delta(me, dx, dy);

  if (! thing_is_able_to_shove(me)) {
    (void) thing_lunge(g, v, l, me, to);
    return false;
  }

  //
  // Needs to be something we can shove
  //
  if (level_is_shovable(g, v, l, to) == nullptr) {
    return false;
  }

  FOR_ALL_THINGS_AT(g, v, l, it, to)
  {
    //
    // A wall or pillar or somesuch?
    //
    if (thing_is_obs_to_movement(it)) {
      //
      // But make exceptions for things like braziers
      //
      if (thing_is_shovable(it)) {
        continue;
      }

      return false;
    }
  }

  return true;
}
