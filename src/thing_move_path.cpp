//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_globals.hpp"
#include "my_main.hpp"
#include "my_thing_callbacks.hpp"
#include "my_thing_inlines.hpp"

//
// Remove the move path
//
void thing_move_path_reset(Gamep g, Levelsp v, Levelp l, Thingp me)
{
  TRACE();

  auto *ext_struct = thing_ext_struct(g, me);
  if (ext_struct == nullptr) {
    return;
  }

  ext_struct->move_path.size      = 0;
  ext_struct->move_path.confirmed = 0;
}

//
// Confirm the move path
//
void thing_move_path_confirm(Gamep g, Levelsp v, Levelp l, Thingp me)
{
  TRACE();

  auto *ext_struct = thing_ext_struct(g, me);
  if (ext_struct == nullptr) {
    return;
  }

  if (ext_struct->move_path.size == 0U) {
    return;
  }

  ext_struct->move_path.confirmed = true;
}

//
// Get the move path size
//
auto thing_move_path_size(Gamep g, Levelsp v, Levelp l, Thingp me) -> int
{
  TRACE();

  auto *ext_struct = thing_ext_struct(g, me);
  if (ext_struct == nullptr) {
    return 0;
  }

  return ext_struct->move_path.size;
}

//
// Return true if there is a move to pop (and pop it)
//
auto thing_move_path_pop(Gamep g, Levelsp v, Levelp l, Thingp me, bpoint &out) -> bool
{
  bool move_confirmed = {};
  return thing_move_path_pop(g, v, l, me, move_confirmed, out);
}

//
// Return true if there is a move to pop (and pop it)
//
auto thing_move_path_pop(Gamep g, Levelsp v, Levelp l, Thingp me, bool &move_confirmed, bpoint &out) -> bool
{
  TRACE();

  move_confirmed = false;

  auto *ext_struct = thing_ext_struct(g, me);
  if (ext_struct == nullptr) {
    return false;
  }

  if (ext_struct->move_path.size == 0) {
    return false;
  }

  out = ext_struct->move_path.points[ 0 ];

  for (int index = 0; index < ext_struct->move_path.size - 1; index++) {
    ext_struct->move_path.points[ index ] = ext_struct->move_path.points[ index + 1 ];
  }
  ext_struct->move_path.size--;

  move_confirmed                  = ext_struct->move_path.confirmed;
  ext_struct->move_path.confirmed = false;

  return true;
}

//
// Copy the given path to the thing
//
static auto thing_move_path_confirmed(Gamep g, Levelsp v, Levelp l, Thingp me, const std::vector< bpoint > &move_path, const bool confirmed)
    -> bool
{
  TRACE();

  auto *ext_struct = thing_ext_struct(g, me);
  if (ext_struct == nullptr) {
    thing_err(me, "no ext struct");
    return false;
  }

  int index                  = 0;
  ext_struct->move_path.size = 0;

  IF_DEBUG2
  {
    THING_DBG(me, "apply path size: %d", static_cast< int >(move_path.size()));
    for (auto p : move_path) {
      THING_DBG(me, " - path: %d,%d", p.x, p.y);
    }
  }

  for (auto p : move_path) {
    ext_struct->move_path.points[ index ].x = p.x;
    ext_struct->move_path.points[ index ].y = p.y;
    ext_struct->move_path.size              = ++index;
    if (index >= ARRAY_SIZE(ext_struct->move_path.points)) {
      break;
    }
  }

  ext_struct->move_path.confirmed = confirmed ? 1 : 0;

  return true;
}

//
// Copy the confirmed path to the thing
//
auto thing_move_path_confirmed(Gamep g, Levelsp v, Levelp l, Thingp me, const std::vector< bpoint > &move_path) -> bool
{
  TRACE();

  return thing_move_path_confirmed(g, v, l, me, move_path, true);
}

//
// Copy the non confirmed path to the thing
//
auto thing_move_path_apply(Gamep g, Levelsp v, Levelp l, Thingp me, const std::vector< bpoint > &move_path) -> bool
{
  TRACE();

  return thing_move_path_confirmed(g, v, l, me, move_path, false);
}

//
// Return true if there is a move to pop.
//
auto thing_move_path_target(Gamep g, Levelsp v, Levelp l, Thingp me, bpoint &out) -> bool
{
  TRACE();

  auto *ext_struct = thing_ext_struct(g, me);
  if (ext_struct == nullptr) {
    return false;
  }

  if (ext_struct->move_path.size == 0) {
    return false;
  }

  out = ext_struct->move_path.points[ ext_struct->move_path.size - 1 ];

  return true;
}
