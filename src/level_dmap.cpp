//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_dmap.hpp"
#include "my_globals.hpp"
#include "my_level.hpp"
#include "my_main.hpp"
#include "my_thing.hpp"
#include "my_thing_inlines.hpp"
#include "my_time.hpp"

void level_dmap(Gamep g, Levelsp v, Levelp l)
{
  TRACE();
  auto started = time_ms();
  FOR_ALL_THINGS_ON_LEVEL_UNSAFE(g, v, l, t) { thing_dmap(g, v, l, t); }
  LEVEL_DBG(g, v, l, "dmap generation took %u ms", time_ms() - started);
}

void levels_dmap(Gamep g, Levelsp v)
{
  log("levels dmap");
  TRACE_INDENT();

  if (g == nullptr) {
    CROAK("no game pointer");
  }

  if (v == nullptr) {
    CROAK("no levels pointer");
  }

  FOR_ALL_LEVELS(g, v, iter) { level_dmap(g, v, iter); }
}
