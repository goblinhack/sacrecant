//
// Copyright goblinhack@gmail.com
//

#include "my_charmap.hpp"
#include "my_globals.hpp"
#include "my_level.hpp"
#include "my_level_inlines.hpp"
#include "my_main.hpp"
#include "my_test.hpp"
#include "my_thing_inlines.hpp"
#include "my_tp_class.hpp"
#include "my_ui.hpp"
#include "my_wids.hpp"

//
// Compare level contents
//
auto level_match_contents(Gamep g, Levelsp v, Levelp l, Testp t, int w, int h, const char *expected) -> bool
{
  TRACE();

  level_log(g, v, l, "match");
  TRACE_INDENT();

  std::string found = level_string(g, v, l, w, h);

  for (int y = 0; y < h; y++) {
    std::string const line;

    for (int x = 0; x < w; x++) {
      auto offset = (w * y) + x;
      auto c      = found[ offset ];
      auto e      = expected[ offset ];
      if (c != e) {
        CON_NEW_LINE();
        level_log(g, v, l, "Expected:");
        level_dump(g, v, l, w, h, expected);
        level_log(g, v, l, "Found:");
        level_dump(g, v, l, w, h);
        level_err(g, v, l, "Level contents not as expected");
        TEST_CON(t, "Level contents not as expected");
        return false;
      }
    }
  }

  //
  // Check for stuff outside the level
  //
  for (int y = 0; y < MAP_HEIGHT; y++) {
    std::string const line;

    if (y < h) {
      continue;
    }

    for (int x = 0; x < MAP_WIDTH; x++) {
      if (x < w) {
        continue;
      }
      bpoint p(x, y);

      FOR_ALL_THINGS_AT(g, v, l, unused, p)
      {
        level_dump(g, v, l, MAP_WIDTH, MAP_HEIGHT);
        CON_NEW_LINE();
        TEST_CON(t, "Level contents found outside of test area");
        return false;
      }
    }
  }

  return true;
}
