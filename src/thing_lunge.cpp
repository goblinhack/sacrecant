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

static float lunge_amount[ MAP_WIDTH ];

void level_lunge_precalculate(Gamep g)
{
  static const char lunge_map[ MAP_WIDTH * MAP_HEIGHT + 1 ]
      // lunge distance -------------------------------->
      = "x                                               " //
        "       x                                        " //
        "              x                                 " //
        "                     x                          " //
        "                            x                   " //
        "                                   x            " //
        "                                          x     " //
        "                                               x" //
        "                                               x" //
        "                                               x" //
        "                                              x " //
        "                                            x   " //
        "                                          x     " //
        "                                        x       " //
        "                                      x         " //
        "                                    x           " //
        "                                  x             " //
        "                                x               " //
        "                              x                 " //
        "                            x                   " //
        "                          x                     " //
        "                        x                       " //
        "                      x                         " //
        "                    x                           " //
        "                  x                             " //
        "                x                               " //
        "              x                                 " //
        "            x                                   " //
        "          x                                     " //
        "        x                                       " //
        "      x                                         " //
        "    x                                           " //
        "   x                                            " //
        "  x                                             " //
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
      ;

  for (auto x = 0; x < MAP_WIDTH; x++) {
    for (auto y = 0; y < MAP_HEIGHT; y++) {
      auto c = lunge_map[ (MAP_WIDTH * y) + x ];
      if (c == 'x') {
        if (lunge_amount[ y ] == 0) {
          lunge_amount[ y ] = 0.5F * (static_cast< float >(y) / static_cast< float >(MAP_HEIGHT));
        }
      }
    }
  }
}

void thing_is_lunging_set(Gamep g, Levelsp v, Levelp l, Thingp me, bool val)
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
  // Once lunge, it is treated as a counter
  //
  if (val) {
    //
    // Start lunge if not doing do
    //
    if (static_cast< bool >(ext->_lunge_ms)) {
      return;
    }
  } else {
    //
    // Stop lunge
    //
    if (! static_cast< bool >(ext->_lunge_ms)) {
      return;
    }
  }
  ext->_lunge_ms = static_cast< uint16_t >(val);
}

static auto thing_is_lunging_incr(Gamep g, Levelsp v, Thingp me, int val) -> int
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

  return ext->_lunge_ms += val;
}

//
// Lunging time step
//
void thing_lunge_time_step(Gamep g, Levelsp v, Levelp l, Thingp me, int time_step)
{
  TRACE();

  (void) thing_is_lunging_incr(g, v, me, time_step);

  if (compiler_unused) {
    THING_DBG(g, v, l, me, "lunge incr %u", thing_is_lunging(g, v, me));
  }

  thing_lunge_end_check(g, v, l, me);
}

[[nodiscard]] auto thing_lunge(Gamep g, Levelsp v, Levelp l, Thingp me, const bpoint &to) -> bool
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

  if (! thing_is_able_to_lunge(me)) {
    return false;
  }

  if (thing_is_lunging(g, v, me) != 0) {
    return false;
  }

  auto *ext = thing_ext_struct(g, v, me);
  if (ext == nullptr) [[unlikely]] {
    return false;
  }

  if (compiler_unused) {
    THING_DBG(g, v, l, me, "lunge to (%d,%d)", to.x, to.y);
  }

  //
  // Set the direction from lunge
  //
  auto at    = thing_at(g, v, l, me);
  auto delta = unit(to - at);
  auto dest  = at + delta;

  thing_set_dir_from_delta(g, v, l, me, delta);

  //
  // Looks odd when a distant argusul lunges
  //
  if (! adjacent(to, at)) {
    return false;
  }

  thing_is_lunging_set(g, v, l, me, true);

  ext->lunge_dir = thing_dir_from_delta(g, v, l, me, dest);

  return true;
}

//
// Check if we're done lunging
//
void thing_lunge_end_check(Gamep g, Levelsp v, Levelp l, Thingp me)
{
  TRACE();

  auto *ext = thing_ext_struct(g, v, me);
  if (ext == nullptr) [[unlikely]] {
    return;
  }

  if (thing_is_lunging(g, v, me) >= THING_LUNGE_ANIM_MS) {
    if (compiler_unused) {
      THING_DBG(g, v, l, me, "lunge done");
    }

    thing_is_lunging_set(g, v, l, me, false);
  }
}

void thing_lunge_modify_position(Gamep g, Levelsp v, Levelp l, Thingp me, spoint &tl, spoint &br)
{
  TRACE();

  if (thing_is_lunging(g, v, me) == 0) {
    return;
  }

  auto *ext = thing_ext_struct(g, v, me);
  if (ext == nullptr) [[unlikely]] {
    return;
  }

  auto dir  = thing_get_direction(ext->lunge_dir);
  auto pct  = static_cast< float >(ext->_lunge_ms) / static_cast< float >(THING_LUNGE_ANIM_MS);
  auto idxs = ARRAY_SIZE(lunge_amount);
  auto idx  = static_cast< int >(idxs * pct);

  if (idx >= idxs) {
    return;
  }

  float const w = abs(br.x - tl.x);

  float const amount = lunge_amount[ idx ];

  auto off_x = static_cast< float >(dir.x) * w * amount;
  auto off_y = static_cast< float >(dir.y) * w * amount;

  tl.x += static_cast< short >(off_x);
  tl.y += static_cast< short >(off_y);
  br.x += static_cast< short >(off_x);
  br.y += static_cast< short >(off_y);
}
