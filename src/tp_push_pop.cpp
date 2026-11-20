//
// Copyright goblinhack@gmail.com
//

#include "my_bpoint.hpp"
#include "my_callstack.hpp"
#include "my_game_defs.hpp"
#include "my_level.hpp"
#include "my_level_inlines.hpp"
#include "my_main.hpp"
#include "my_thing.hpp"
#include "my_tp.hpp"
#include "my_tp_inlines.hpp"
#include "my_types.hpp"
#include <vector>

//
// Push the Tp onto the level
//
[[nodiscard]] auto tp_push(Gamep g, Levelsp v, Levelp l, Tpp tp, const bpoint &at) -> bool
{
  TRACE_DEBUG();

  if (is_oob(at)) [[unlikely]] {
    return false;
  }

  auto id = tp_id_get(tp);
  if (! id) {
    return false;
  }

  bool free_slot_set {};
  int  free_slot {};

  //
  // Already at this location?
  //
  for (auto slot = 0; slot < MAP_TP_SLOTS; slot++) {
    auto o_id = l->tp_id[ at.x ][ at.y ][ slot ];

    if (! free_slot_set && ! o_id) {
      free_slot     = slot;
      free_slot_set = true;
      continue;
    }

    if (o_id == id) {
      return true;
    }
  }

  if (! free_slot_set) {
    tp_err(tp, "out of tp slots at @%d,%d", at.x, at.y);
    return false;
  }

  l->tp_id[ at.x ][ at.y ][ free_slot ] = tp_id_get(tp);
  return true;
}

//
// Pop the Tp from the level
//
[[nodiscard]] auto tp_pop(Gamep g, Levelsp v, Levelp l, Tpp tp, const bpoint &at) -> bool
{
  TRACE_DEBUG();

  if (is_oob(at)) [[unlikely]] {
    return false;
  }

  auto id = tp_id_get(tp);
  if (! id) {
    return false;
  }

  //
  // Already at this location?
  //
  for (auto slot = 0; slot < MAP_TP_SLOTS; slot++) {
    auto o_id = l->tp_id[ at.x ][ at.y ][ slot ];

    if (o_id == id) {
      l->tp_id[ at.x ][ at.y ][ slot ] = 0;
      return true;
    }
  }

  return false;
}
