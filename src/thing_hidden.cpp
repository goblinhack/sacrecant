//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_game_defs.hpp"
#include "my_level_inlines.hpp" // NOLINT
#include "my_main.hpp"
#include "my_thing_inlines.hpp" // NOLINT
#include "my_time.hpp"
#include "my_types.hpp"

#include <algorithm>
#include <cstdint>

//
// Hidden anim time step
//
void thing_hidden_time_step(Gamep g, Levelsp v, Levelp l, Thingp me, int /*time_step*/)
{
  TRACE();

  auto        pulse = THING_IS_HIDDEN_PULSE_ANIM_MS; // ms
  float const mid   = pulse / 2;
  auto const  n     = static_cast< float >(game_time_ms_cached() % pulse);
  float       i     = 0;
  uint8_t     a     = 0;

  if (n == mid) {
    i = 255;
  } else if (n > mid) {
    i = (n - mid) / mid;
    i *= 100;
    i = 255 - i;
  } else {
    i = n / mid;
    i *= 100;
    i = 155 + i;
  }

  i = std::max< float >(i, 0);
  i = std::min< float >(i, 255);

  a = static_cast< uint8_t >(i);

  me->_is_hidden = (a / 2) + 100;
}

void thing_is_hidden_set(Gamep g, Levelsp v, Levelp l, Thingp me, bool val)
{
  TRACE_DEBUG();

  if (me == nullptr) {
    ERR("no thing pointer");
    return;
  }

  if (val) {
    if (me->_is_hidden != 0U) {
      return;
    }

    me->_is_hidden = 100;
  } else {
    me->_is_hidden = 0;
  }
}
