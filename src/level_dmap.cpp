//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_level.hpp"
#include "my_level_inlines.hpp" // NOLINT
#include "my_main.hpp"
#include "my_thing.hpp"
#include "my_thing_inlines.hpp" // NOLINT
#include "my_time.hpp"
#include "my_types.hpp"

#include <cmath>

void level_dmap(Gamep g, Levelsp v, Levelp l)
{
  TRACE();
  auto started = user_visible_time_ms();
  FOR_ALL_THINGS_ON_LEVEL_NO_BREAK_UNSAFE(g, v, l, t) { thing_dmap(g, v, l, t); }
  LEVEL_DBG(g, v, l, "dmap generation took %u ms", user_visible_time_ms() - started);
}

void levels_dmap(Gamep g, Levelsp v)
{
  log("levels dmap");
  TRACE_INDENT();

  if (g == nullptr) [[unlikely]] {
    no_game_pointer();
    return;
  }

  if (v == nullptr) [[unlikely]] {
    no_levels_pointer();
    return;
  }

  FOR_ALL_LEVELS(g, v, iter) { level_dmap(g, v, iter); }
}
