//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_level_inlines.hpp"
#include "my_thing_callbacks.hpp"
#include "my_thing_inlines.hpp"

static float lunge_amount[ MAP_WIDTH ];

void level_lunge_precalculate(Gamep g)
{
  static const char lunge_map[]
      // lunge distance ------------------------------>
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
    ERR("no thing pointer");
    return;
  }

  //
  // Once lunge, it is treated as a counter
  //
  if (val) {
    //
    // Start lunge if not doing do
    //
    if (static_cast< bool >(me->_lunge_ms)) {
      return;
    }
  } else {
    //
    // Stop lunge
    //
    if (! static_cast< bool >(me->_lunge_ms)) {
      return;
    }
  }
  me->_lunge_ms = static_cast< uint16_t >(val);
}

static auto thing_is_lunging_incr(Gamep g, Levelsp v, Levelp l, Thingp me, int val) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    ERR("no thing pointer");
    return 0;
  }

  return me->_lunge_ms += val;
}

//
// Lunging time step
//
void thing_lunge_time_step(Gamep g, Levelsp v, Levelp l, Thingp me, int time_step)
{
  TRACE();

  (void) thing_is_lunging_incr(g, v, l, me, time_step);

  if (compiler_unused) {
    THING_DBG(me, "lunge incr %u", thing_is_lunging(me));
  }

  thing_lunge_end_check(g, v, l, me);
}

auto thing_lunge(Gamep g, Levelsp v, Levelp l, Thingp me, const bpoint &to) -> bool
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

  if (thing_is_lunging(me) != 0) {
    return false;
  }

  if (compiler_unused) {
    THING_DBG(me, "lunge to @%d,%d", to.x, to.y);
  }

  //
  // Set the direction from lunge
  //
  auto at = thing_at(me);
  thing_set_dir_from_delta(me, to.x - at.x, to.x - at.x);

  thing_is_lunging_set(g, v, l, me, true);

  me->lunging_to = to;

  return true;
}

//
// Check if we're done lunging
//
void thing_lunge_end_check(Gamep g, Levelsp v, Levelp l, Thingp me)
{
  TRACE();

  if (thing_is_lunging(me) >= THING_LUNGE_TIME_MS) {
    if (compiler_unused) {
      THING_DBG(me, "lunge to @%d,%d done", me->lunging_to.x, me->lunging_to.y);
    }

    thing_is_lunging_set(g, v, l, me, false);
  }
}

void thing_lunge_modify_position(Gamep g, Levelsp v, Levelp l, Thingp me, spoint &tl, spoint &br)
{
  TRACE();

  if (thing_is_lunging(me) == 0) {
    return;
  }

  auto dir = me->lunging_to - thing_at(me);

  auto pct  = static_cast< float >(me->_lunge_ms) / static_cast< float >(THING_LUNGE_TIME_MS);
  auto idxs = ARRAY_SIZE(lunge_amount);
  auto idx  = static_cast< int >(idxs * pct);

  if (idx >= idxs) {
    return;
  }

  float const w = abs(br.x - tl.x);

  float const amount = lunge_amount[ idx ];

  auto off_x = static_cast< float >(dir.x) * w * amount;
  auto off_y = static_cast< float >(dir.y) * w * amount;

  if (compiler_unused) {
    THING_DBG(me, "lunge to @%d,%d idx %d off %f,%f am %f w %f", me->lunging_to.x, me->lunging_to.y, idx, off_x, off_y, amount, w);
  }

  tl.x += static_cast< short >(off_x);
  tl.y += static_cast< short >(off_y);
  br.x += static_cast< short >(off_x);
  br.y += static_cast< short >(off_y);
}
