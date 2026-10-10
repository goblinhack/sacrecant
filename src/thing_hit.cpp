//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_game_defs.hpp"
#include "my_level_inlines.hpp" // NOLINT
#include "my_main.hpp"
#include "my_thing.hpp"
#include "my_thing_inlines.hpp" // NOLINT
#include "my_types.hpp"

//
// Hit time step
//
void thing_hit_time_step(Gamep g, Levelsp v, Levelp l, Thingp t, int time_step)
{
  TRACE();

  (void) thing_is_hit_decr(g, v, l, t, time_step);
}

void thing_is_hit_set(Gamep g, Levelsp v, Levelp l, Thingp t, int val)
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return;
  }

  if (val == THING_HIT_FLASH_ANIM_MS) {
    if (thing_is_blit_hit_solid_white(t)) {
      val = THING_HIT_FLASH_WHITE_ANIM_MS;
    }
  }

  //
  // Once hit, it is treated as a counter
  //
  if (val != 0) {
    //
    // Start the hit counter if not doing do
    //
    if (static_cast< bool >(t->_is_hit)) {
      return;
    }
  } else {
    //
    // Stop hit
    //
    if (! static_cast< bool >(t->_is_hit)) {
      return;
    }
  }

  t->_is_hit = val;
}

[[nodiscard]] auto thing_is_hit_incr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }

  if (t->_is_hit + val > 255) {
    return t->_is_hit = 255;
  }

  return t->_is_hit += val;
}

[[nodiscard]] auto thing_is_hit_decr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }

  if (static_cast< int >(t->_is_hit) - val <= 0) {
    return t->_is_hit = 0;
  }

  return t->_is_hit -= val;
}
