//
// Copyright goblinhack@gmail.com
//

#include "my_globals.hpp"
#include "my_level.hpp"
#include "my_main.hpp"
#include "my_test.hpp"

#include <format>

//
// Print a level string
//
void level_dump(Gamep g, Levelsp v, Levelp l, int w, int h, const std::string &s)
{
  TRACE();

  level_con(g, v, l, "Level: %u", l->level_num + 1);

  for (int y = 0; y < h; y++) {
    std::string line;

    for (int x = 0; x < w; x++) {
      auto offset = (w * y) + x;
      auto c      = s[ offset ];
      if (c != 0) {
        line += c;
      }
    }

    if (! line.empty()) {
      level_log(g, v, l, "[%s]", line.c_str());
    }
  }
}

//
// Print a level string
//
void level_dump(Gamep g, Levelsp v, Levelp l, int w, int h)
{
  TRACE();

  const std::string s = level_string(g, v, l, w, h);

  level_dump(g, v, l, w, h, s);
}
