//
// Copyright goblinhack@gmail.com
//

#include "my_bpoint.hpp"
#include "my_callstack.hpp"
#include "my_game_defs.hpp"
#include "my_main.hpp"
#include "my_spoint.hpp"
#include "my_thing.hpp"
#include "my_thing_inlines.hpp"
#include "my_types.hpp"

#include <cstdint>
#include <cstdlib>

static float recoil_amount[ MAP_WIDTH ];

void level_recoil_precalculate(Gamep g)
{
  static const char recoil_map[ MAP_WIDTH * MAP_HEIGHT + 1 ]
      // recoil distance ------------------------------->
      = "x                                               " //
        "       x                                        " //
        "              x                                 " //
        "                     x                          " //
        "                            x                   " //
        "                     x                          " //
        "              x                                 " //
        "       x                                        " //
        "     x                                          " //
        "    x                                           " //
        "   x                                            " //
        "  x                                             " //
        " x                                              " //
        " x                                              " //
        " x                                              " //
        " x                                              " //
        "x                                               " //
        "                                                " //
        "                                                " //
        "                                                " //
        "                                                " //
        "                                                " //
        "                                                " //
        "                                                " //
        "                                                " //
        "                                                " //
        "                                                " //
        "                                                " //
        "                                                " //
        "                                                " //
        "                                                " //
        "                                                " //
        "                                                " //
        "                                                " //
        "                                                " //
        "                                                " //
        "                                                " //
        "                                                " //
        "                                                " //
        "                                                " //
        "                                                " //
        "                                                " //
        "                                                " //
        "                                                " //
        "                                                " //
        "                                                " //
        "                                                " //
        "                                                " //
      ;

  for (auto x = 0; x < MAP_WIDTH; x++) {
    for (auto y = 0; y < MAP_HEIGHT; y++) {
      auto c = recoil_map[ (MAP_WIDTH * y) + x ];
      if (c == 'x') {
        if (recoil_amount[ y ] == 0) {
          recoil_amount[ y ] = 0.5F * (static_cast< float >(y) / static_cast< float >(MAP_HEIGHT));
        }
      }
    }
  }
}

void thing_is_recoil_set(Gamep g, Levelsp v, Levelp l, Thingp me, bool val)
{
  TRACE_DEBUG();

  if (me == nullptr) {
    no_thing_pointer();
    return;
  }

  auto *ext = thing_ext_struct(g, v, me);
  if (ext == nullptr) [[unlikely]] {
    return;
  }

  //
  // Once recoil, it is treated as a counter
  //
  if (val) {
    //
    // Start recoil if not doing do
    //
    if (static_cast< bool >(ext->_recoil_ms)) {
      return;
    }
  } else {
    //
    // Stop recoil
    //
    if (! static_cast< bool >(ext->_recoil_ms)) {
      return;
    }
  }
  ext->_recoil_ms = static_cast< uint16_t >(val);
}

static auto thing_is_recoil_incr(Gamep g, Levelsp v, Thingp me, int val) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    no_thing_pointer();
    return 0;
  }

  auto *ext = thing_ext_struct(g, v, me);
  if (ext == nullptr) [[unlikely]] {
    return 0;
  }

  return ext->_recoil_ms += val;
}

//
// Lunging time step
//
void thing_recoil_time_step(Gamep g, Levelsp v, Levelp l, Thingp me, int time_step)
{
  TRACE();

  (void) thing_is_recoil_incr(g, v, me, time_step);

  if (compiler_unused) {
    THING_DBG(g, v, l, me, "recoil incr %u", thing_is_recoiling(g, v, me));
  }

  thing_recoil_end_check(g, v, l, me);
}

[[nodiscard]] auto thing_recoil(Gamep g, Levelsp v, Levelp l, Thingp me, const bpoint &to) -> bool
{
  TRACE();

  if (thing_is_dead(me)) {
    return false;
  }

  if (thing_is_sleeping(me)) {
    return false;
  }

  if (thing_is_moving(me)) {
    return false;
  }

  if (! thing_is_able_to_recoil(me)) {
    return false;
  }

  if (thing_is_recoiling(g, v, me) != 0) {
    return false;
  }

  auto *ext = thing_ext_struct(g, v, me);
  if (ext == nullptr) [[unlikely]] {
    return false;
  }

  if (compiler_unused) {
    THING_DBG(g, v, l, me, "recoil to (%d,%d)", to.x, to.y);
  }

  //
  // Set the direction from recoil
  //
  auto at    = thing_at(g, v, l, me);
  auto delta = unit(to - at);
  auto dest  = at + delta;

  thing_is_recoil_set(g, v, l, me, true);

  ext->recoil_dir = thing_dir_from_delta(g, v, l, me, dest);

  return true;
}

//
// Check if we're done recoil
//
void thing_recoil_end_check(Gamep g, Levelsp v, Levelp l, Thingp me)
{
  TRACE();

  auto *ext = thing_ext_struct(g, v, me);
  if (ext == nullptr) [[unlikely]] {
    return;
  }

  if (thing_is_recoiling(g, v, me) >= THING_RECOIL_ANIM_MS) {
    if (compiler_unused) {
      THING_DBG(g, v, l, me, "recoil done");
    }

    thing_is_recoil_set(g, v, l, me, false);
  }
}

void thing_recoil_modify_position(Gamep g, Levelsp v, Levelp l, Thingp me, spoint &tl, spoint &br)
{
  TRACE();

  if (thing_is_recoiling(g, v, me) == 0) {
    return;
  }

  auto *ext = thing_ext_struct(g, v, me);
  if (ext == nullptr) [[unlikely]] {
    return;
  }

  auto dir  = thing_get_direction(ext->recoil_dir);
  auto pct  = static_cast< float >(ext->_recoil_ms) / static_cast< float >(THING_RECOIL_ANIM_MS);
  auto idxs = ARRAY_SIZE(recoil_amount);
  auto idx  = static_cast< int >(idxs * pct);

  if (idx >= idxs) {
    return;
  }

  float const w = abs(br.x - tl.x);

  float const amount = recoil_amount[ idx ];

  auto off_x = static_cast< float >(dir.x) * w * amount;
  auto off_y = static_cast< float >(dir.y) * w * amount;

  tl.x += static_cast< short >(off_x);
  tl.y += static_cast< short >(off_y);
  br.x += static_cast< short >(off_x);
  br.y += static_cast< short >(off_y);
}
