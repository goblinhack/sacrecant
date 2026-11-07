//
// Copyright goblinhack@gmail.com
//

#include "my_bpoint.hpp"
#include "my_callstack.hpp"
#include "my_game_defs.hpp"
#include "my_gl.hpp"
#include "my_level.hpp"
#include "my_math.hpp"
#include "my_random.hpp"
#include "my_thing.hpp"
#include "my_thing_inlines.hpp"
#include "my_tile.hpp"
#include "my_tp.hpp"
#include "my_types.hpp"

#include <algorithm>
#include <cstdint>
#include <cstring>
#include <thread>
#include <vector>

static float light_fade[ MAP_WIDTH ];

void level_light_precalculate(Gamep g)
{
  static const char light_fade_map[ (MAP_WIDTH * MAP_HEIGHT) + 1 ]
      = "x                                               " // light strength
        " x                                              " //       |
        "  x                                             " //       |
        "   x                                            " //       |
        "    x                                           " //       |
        "     x                                          " //       |
        "      x                                         " //       |
        "       x                                        " //       |
        "        x                                       " //       |
        "         x                                      " //       |
        "          x                                     " //       |
        "           x                                    " //       |
        "            x                                   " //       |
        "             x                                  " //       |
        "              x                                 " //       |
        "               x                                " //       |
        "                x                               " //       |
        "                 x                              " //       |
        "                  x                             " //       |
        "                   x                            " //       |
        "                    x                           " //       |
        "                     x                          " //       |
        "                      x                         " //       |
        "                       x                        " //       |
        "                        x                       " //       |
        "                         x                      " //       |
        "                          x                     " //       |
        "                           x                    " //       |
        "                            x                   " //       |
        "                             x                  " //       |
        "                              x                 " //       |
        "                               x                " //       |
        "                                x               " //       |
        "                                 x              " //       |
        "                                  x             " //       |
        "                                   x            " //       |
        "                                    x           " //       |
        "                                     x          " //       |
        "                                      x         " //       |
        "                                       x        " //       |
        "                                        x       " //       |
        "                                         x      " //       |
        "                                          x     " //       |
        "                                           x    " //       |
        "                                            x   " //       |
        "                                             x  " //       |
        "                                              x " //       |
        "                                               x" //       v
      ;

  for (auto x = 0; x < MAP_WIDTH; x++) {
    for (auto y = 0; y < MAP_HEIGHT; y++) {
      auto c = light_fade_map[ (MAP_WIDTH * y) + x ];
      if (c == 'x') {
        if (light_fade[ x ] == 0) {
          light_fade[ x ] = 1.0F - (static_cast< float >(y) / static_cast< float >(MAP_HEIGHT));
        }
      }
    }
  }
}

//
// All light from all light sources, combined.
//
void level_light_calculate_all(Gamep g, Levelsp v, Levelp l)
{
  TRACE();

  auto *player = thing_player(g);
  if (player == nullptr) [[unlikely]] {
    return;
  }

  if ((g == nullptr) || (v == nullptr) || (l == nullptr)) [[unlikely]] {
    return;
  }

  //
  // If the player is not on the level being lit, then nothing to do
  //
  if (l->level_arr_idx != player->level_arr_idx) {
    return;
  }

  //
  // Calculate player lighting while we wait
  //
  level_raycast(g, v, l, FBO_MAP_LIGHT_MASK);
}
