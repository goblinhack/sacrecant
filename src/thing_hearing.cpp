//
// Copyright goblinhack@gmail.com
//

#include "my_bpoint.hpp"
#include "my_callstack.hpp"
#include "my_dmap.hpp"
#include "my_level.hpp"
#include "my_main.hpp"
#include "my_thing.hpp"
#include "my_thing_inlines.hpp"
#include "my_types.hpp"

[[nodiscard]] auto thing_vision_can_hear_tile(Gamep g, Levelsp v, Levelp l, Thingp me, bpoint p) -> bool
{
  TRACE();

  auto noise = v->dmap_noise.val[ p.x ][ p.y ];

  if (noise == DMAP_IS_WALL) {
    return false;
  }
  if (noise == DMAP_IS_PASSABLE) {
    return false;
  }
  if (noise == DMAP_IS_GOAL_REVERSE) {
    return false;
  }
  if (noise == DMAP_IS_GOAL) {
    return false;
  }

  auto actual_noise = DMAP_IS_GOAL_REVERSE - noise;

  //
  //        xxxxx
  //     xxx.....xxx
  //   xx...........xx
  //  x...............x
  // x.................x
  // x.................x
  // x........@98765432100000
  //                   |
  //                   + noise level 9
  //
  //           yyyyyyyyynnnnnn (monst with hearing level 1) best hearing
  //           yyyyyyyynnnnnnn (monst with hearing level 2)
  //           yyyyyyynnnnnnnn (monst with hearing level 3)
  //           yyyyyynnnnnnnnn (monst with hearing level 4)
  //           yyyyynnnnnnnnnn (monst with hearing level 5)
  //           yyyynnnnnnnnnnn (monst with hearing level 6)
  //           yyynnnnnnnnnnnn (monst with hearing level 7)
  //           yynnnnnnnnnnnnn (monst with hearing level 8)
  //           ynnnnnnnnnnnnnn (monst with hearing level 9) worst hearing
  //
  // y - can hear
  // n - cannot hear

  if (compiler_unused) {
    THING_DBG(g, v, l, me, "tile dmap value         : %d", noise);
    THING_DBG(g, v, l, me, " actual_noise           : %d", actual_noise);
    THING_DBG(g, v, l, me, " thing_hearing_threshold: %d", thing_hearing_threshold(g, v, l, me));
    THING_DBG(g, v, l, me, " can hear               : %d", static_cast< int >(actual_noise >= thing_hearing_threshold(g, v, l, me)));
  }

  return actual_noise >= thing_hearing_threshold(g, v, l, me);
}

[[nodiscard]] auto thing_hearing_threshold(Gamep g, Levelsp v, Levelp l, Thingp me) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    no_thing_pointer();
    return 0;
  }

  auto *ext = thing_ext_struct(g, v, me);
  if (ext == nullptr) {
    return 0;
  }

  return ext->_hearing_threshold;
}

[[nodiscard]] auto thing_hearing_threshold_set(Gamep g, Levelsp v, Levelp l, Thingp me, int val) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    no_thing_pointer();
    return 0;
  }

  auto *ext = thing_ext_struct(g, v, me);
  if (ext == nullptr) {
    return false;
  }

  return ext->_hearing_threshold = val;
}

[[nodiscard]] auto thing_hearing_threshold_incr(Gamep g, Levelsp v, Levelp l, Thingp me, int val) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    no_thing_pointer();
    return 0;
  }

  auto *ext = thing_ext_struct(g, v, me);
  if (ext == nullptr) {
    return false;
  }

  return ext->_hearing_threshold += val;
}

[[nodiscard]] auto thing_hearing_threshold_decr(Gamep g, Levelsp v, Levelp l, Thingp me, int val) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    no_thing_pointer();
    return 0;
  }

  auto *ext = thing_ext_struct(g, v, me);
  if (ext == nullptr) {
    return false;
  }

  if (static_cast< int >(ext->_hearing_threshold) - val <= 0) {
    return ext->_hearing_threshold = 0;
  }

  return ext->_hearing_threshold -= val;
}
