//
// Copyright goblinhack@gmail.com
//

#include "my_dmap.hpp"
#include "my_main.hpp"
#include "my_sprintf.hpp"
#include "my_thing.hpp"

#include <algorithm>
#include <array>
#include <print>
#include <utility>

void dmap_print(const Dmap *D, bpoint at, bpoint tl, bpoint br)
{
  int x = 0;
  int y = 0;

  int minx = 0;
  int miny = 0;
  int maxx = 0;
  int maxy = 0;
  if (tl.x < br.x) {
    minx = tl.x;
    maxx = br.x;
  } else {
    minx = br.x;
    maxx = tl.x;
  }
  if (tl.y < br.y) {
    miny = tl.y;
    maxy = br.y;
  } else {
    miny = br.y;
    maxy = tl.y;
  }

  minx = std::max(minx, 0);
  miny = std::max(miny, 0);
  if (maxx >= MAP_WIDTH) {
    maxx = MAP_WIDTH - 1;
  }
  if (maxy >= MAP_HEIGHT) {
    maxy = MAP_HEIGHT - 1;
  }

  bool all_walls = false;

  //
  // Try to minimize the DMAP area if it is mostly walls at the edges, for speed.
  //
  all_walls = true;
  for (x = minx; (x <= maxx) && all_walls; x++) {
    for (y = miny; (y <= maxy) && all_walls; y++) {
      all_walls = (D->val[ x ][ y ] == DMAP_IS_WALL);
    }
    if (all_walls) {
      minx = x;
    }
  }

  all_walls = true;
  for (x = maxx; (x > minx) && all_walls; x--) {
    for (y = miny; (y <= maxy) && all_walls; y++) {
      all_walls = (D->val[ x ][ y ] == DMAP_IS_WALL);
    }
    if (all_walls) {
      maxx = x;
    }
  }

  all_walls = true;
  for (y = miny; (y <= maxy) && all_walls; y++) {
    for (x = minx; (x <= maxx) && all_walls; x++) {
      all_walls = (D->val[ x ][ y ] == DMAP_IS_WALL);
    }
    if (all_walls) {
      miny = y;
    }
  }

  all_walls = true;
  for (y = maxy; (y > miny) && all_walls; y--) {
    for (x = minx; (x <= maxx) && all_walls; x++) {
      all_walls = (D->val[ x ][ y ] == DMAP_IS_WALL);
    }
    if (all_walls) {
      maxy = y;
    }
  }

  log("DMAP: %p tl %d,%d br %d %d at %d,%d", (void *) D, minx, miny, maxx, maxy, at.x, at.y);

  for (y = miny; y <= maxy; y++) {
    std::string debug;
    for (x = minx; x <= maxx; x++) {
      uint8_t e = D->val[ x ][ y ];
      if (bpoint(x, y) == at) {
        debug += ("  @");
        continue;
      }
      if (e == DMAP_IS_WALL) {
        debug += (" ##");
        continue;
      }
      if (e == DMAP_IS_PASSABLE) {
        debug += ("  p");
        continue;
      }

      if (e > 0) {
        debug += std::format("{:3X}", e);
      } else {
        debug += "  *";
      }
    }
    log("DMAP: %s", debug.c_str());
  }
}

void dmap_print(const Dmap *D)
{
  int x = 0;
  int y = 0;

  log("DMAP:");

  for (y = 0; y < MAP_HEIGHT; y++) {
    std::string debug;
    for (x = 0; x < MAP_WIDTH; x++) {
      uint8_t e = D->val[ x ][ y ];
      if (e == DMAP_IS_WALL) {
        debug += ("##");
        continue;
      }
      if (e == DMAP_IS_PASSABLE) {
        debug += (" .");
        continue;
      }

      if (e > 0) {
        debug += std::format("{:2X}", e);
      } else {
        debug += " *";
      }
    }
    log("DMAP: %s", debug.c_str());
  }
}

//
// Pre:
//
// 9 9 9 9 9
// 9 9 9 9 9
// 9 9 9 9 9
// 9 9 0 9 9
// 9 9 9 9 9
// 9 9 9 9 9
// 9 9 9 9 9
//
// Post:
//
// 3 3 3 3 3
// 2 2 2 2 2
// 2 1 1 1 2
// 3 1 0 1 2
// 2 1 1 1 2
// 2 2 2 2 2
// 3 3 3 3 3
//
void dmap_process(Dmap *D, bpoint tl, bpoint br)
{
  uint8_t  x       = 0;
  uint8_t  y       = 0;
  uint8_t  a       = 0;
  uint8_t  b       = 0;
  uint8_t  c       = 0;
  uint8_t  d       = 0;
  uint8_t *e       = nullptr;
  uint8_t  f       = 0;
  uint8_t  g       = 0;
  uint8_t  h       = 0;
  uint8_t  i       = 0;
  uint8_t  lowest  = 0;
  uint8_t  changed = 0;
  uint8_t  minx    = 0;
  uint8_t  miny    = 0;
  uint8_t  maxx    = 0;
  uint8_t  maxy    = 0;
  auto     orig    = *D;

  if (tl.x < br.x) {
    minx = tl.x;
    maxx = br.x;
  } else {
    minx = br.x;
    maxx = tl.x;
  }
  if (tl.y < br.y) {
    miny = tl.y;
    maxy = br.y;
  } else {
    miny = br.y;
    maxy = tl.y;
  }

  //
  // Need a wall around the dmap or the search will sort of trickle off the map
  //
  for (y = miny; y <= maxy; y++) {
    D->val[ minx ][ y ] = DMAP_IS_WALL;
    D->val[ maxx ][ y ] = DMAP_IS_WALL;
  }
  for (x = minx; x <= maxx; x++) {
    D->val[ x ][ miny ] = DMAP_IS_WALL;
    D->val[ x ][ maxy ] = DMAP_IS_WALL;
  }

  //
  // Walk inside the above boundary.
  //
  minx++;
  miny++;
  maxx--;
  maxy--;

  do {
    changed = 0U;

    for (y = miny; y <= maxy; y++) {
      for (x = minx; x <= maxx; x++) {
        if (orig.val[ x ][ y ] == DMAP_IS_WALL) {
          continue;
        }

        e = &D->val[ x ][ y ];

        a = D->val[ x - 1 ][ y - 1 ];
        b = D->val[ x ][ y - 1 ];
        c = D->val[ x + 1 ][ y - 1 ];
        d = D->val[ x - 1 ][ y ];
        f = D->val[ x + 1 ][ y ];
        g = D->val[ x - 1 ][ y + 1 ];
        h = D->val[ x ][ y + 1 ];
        i = D->val[ x + 1 ][ y + 1 ];

        //
        // a b c
        // d e f
        // g h i
        //
        // No corner cutting
        //
        if ((b == DMAP_IS_WALL) && (d == DMAP_IS_WALL)) {
          a = DMAP_IS_WALL;
        }
        if ((b == DMAP_IS_WALL) && (f == DMAP_IS_WALL)) {
          c = DMAP_IS_WALL;
        }
        if ((d == DMAP_IS_WALL) && (h == DMAP_IS_WALL)) {
          g = DMAP_IS_WALL;
        }
        if ((f == DMAP_IS_WALL) && (h == DMAP_IS_WALL)) {
          i = DMAP_IS_WALL;
        }

        if (a < b) {
          lowest = a;
        } else {
          lowest = b;
        }

        lowest = std::min(c, lowest);
        lowest = std::min(d, lowest);
        lowest = std::min(f, lowest);
        lowest = std::min(g, lowest);
        lowest = std::min(h, lowest);
        lowest = std::min(i, lowest);

        if (*e - lowest >= 2) {
          *e      = lowest + 1;
          changed = 1U;
        }
      }
    }
  } while (static_cast< bool >(changed));
}

//
// Pre:
//
// 0 0 0 0 0
// 0 0 0 0 0
// 0 0 0 0 0
// 0 0 3 0 0
// 0 0 0 0 0
// 0 0 0 0 0
// 0 0 0 0 0
//
// Post:
//
// 0 0 0 0 0
// 0 1 1 1 0
// 1 2 2 2 1
// 1 2 3 2 1
// 1 2 2 2 1
// 0 1 1 1 0
// 0 0 0 0 0
//
void dmap_process_reverse(Dmap *D, bpoint tl, bpoint br)
{
  uint8_t  x       = 0;
  uint8_t  y       = 0;
  uint8_t  a       = 0;
  uint8_t  b       = 0;
  uint8_t  c       = 0;
  uint8_t  d       = 0;
  uint8_t *e       = nullptr;
  uint8_t  f       = 0;
  uint8_t  g       = 0;
  uint8_t  h       = 0;
  uint8_t  i       = 0;
  uint8_t  highest = 0;
  uint8_t  changed = 0;
  uint8_t  minx    = 0;
  uint8_t  miny    = 0;
  uint8_t  maxx    = 0;
  uint8_t  maxy    = 0;
  auto     orig    = *D;

  if (tl.x < br.x) {
    minx = tl.x;
    maxx = br.x;
  } else {
    minx = br.x;
    maxx = tl.x;
  }
  if (tl.y < br.y) {
    miny = tl.y;
    maxy = br.y;
  } else {
    miny = br.y;
    maxy = tl.y;
  }

  //
  // Need a wall around the dmap or the search will sort of trickle off the map
  //
  for (y = miny; y <= maxy; y++) {
    D->val[ minx ][ y ] = DMAP_IS_WALL;
    D->val[ maxx ][ y ] = DMAP_IS_WALL;
  }
  for (x = minx; x <= maxx; x++) {
    D->val[ x ][ miny ] = DMAP_IS_WALL;
    D->val[ x ][ maxy ] = DMAP_IS_WALL;
  }

  //
  // Walk inside the above boundary.
  //
  minx++;
  miny++;
  maxx--;
  maxy--;

  do {
    changed = 0U;

    for (y = miny; y <= maxy; y++) {
      for (x = minx; x <= maxx; x++) {
        if (orig.val[ x ][ y ] == DMAP_IS_WALL) {
          continue;
        }

        e = &D->val[ x ][ y ];

        a = D->val[ x - 1 ][ y - 1 ];
        b = D->val[ x ][ y - 1 ];
        c = D->val[ x + 1 ][ y - 1 ];
        d = D->val[ x - 1 ][ y ];
        f = D->val[ x + 1 ][ y ];
        g = D->val[ x - 1 ][ y + 1 ];
        h = D->val[ x ][ y + 1 ];
        i = D->val[ x + 1 ][ y + 1 ];

        //
        // a b c
        // d e f
        // g h i
        //
        // No corner cutting
        //
        if ((b == DMAP_IS_WALL) && (d == DMAP_IS_WALL)) {
          a = DMAP_IS_WALL;
        }
        if ((b == DMAP_IS_WALL) && (f == DMAP_IS_WALL)) {
          c = DMAP_IS_WALL;
        }
        if ((d == DMAP_IS_WALL) && (h == DMAP_IS_WALL)) {
          g = DMAP_IS_WALL;
        }
        if ((f == DMAP_IS_WALL) && (h == DMAP_IS_WALL)) {
          i = DMAP_IS_WALL;
        }

        if (a >= DMAP_IS_PASSABLE) {
          a = 0;
        }
        if (b >= DMAP_IS_PASSABLE) {
          b = 0;
        }
        if (c >= DMAP_IS_PASSABLE) {
          c = 0;
        }
        if (d >= DMAP_IS_PASSABLE) {
          d = 0;
        }
        if (f >= DMAP_IS_PASSABLE) {
          f = 0;
        }
        if (g >= DMAP_IS_PASSABLE) {
          g = 0;
        }
        if (h >= DMAP_IS_PASSABLE) {
          h = 0;
        }
        if (i >= DMAP_IS_PASSABLE) {
          i = 0;
        }

        if (a > b) {
          highest = a;
        } else {
          highest = b;
        }

        highest = std::max(c, highest);
        highest = std::max(d, highest);
        highest = std::max(f, highest);
        highest = std::max(g, highest);
        highest = std::max(h, highest);
        highest = std::max(i, highest);

        if (*e - highest >= 2) {
          *e      = highest - 1;
          changed = 1U;
        }
      }
    }
  } while (static_cast< bool >(changed));
}

static auto dmap_solve_(const Dmap *D, const bpoint start, const std::vector< bpoint > &all_deltas, bool allow_diagonals)
    -> std::vector< bpoint >
{
  std::array< std::array< bool, MAP_HEIGHT >, MAP_WIDTH > walked = {};
  std::vector< bpoint >                                   out    = {};
  auto                                                    at     = start;

  out.push_back(start);

  for (; /*ever*/;) {
    auto x = at.x;
    auto y = at.y;

    if (((x >= MAP_WIDTH - 1) || (y >= MAP_HEIGHT - 1) || (x < 1) || (y < 1))) [[unlikely]] {
      return out;
    }

    uint8_t lowest = D->val[ x ][ y ];
    bool    got    = false;
    bpoint  best   = {};

    if (D->val[ x ][ y ] == DMAP_IS_WALL) {
      return out;
    }

    for (const auto &d : all_deltas) {
      auto t  = at + d;
      auto tx = t.x;
      auto ty = t.y;

      if (walked[ tx ][ ty ]) {
        continue;
      }

      if (D->val[ tx ][ ty ] == DMAP_IS_WALL) {
        continue;
      }

      if (D->val[ tx ][ ty ] == DMAP_IS_PASSABLE) {
        continue;
      }

      //
      // Don't allow diagonal shortcuts if an obstacle would be crossed in an adjacent tile
      // Yes, this should be a not check to allow the player to move diagonally.
      //
      if (! allow_diagonals) {
        if (d.x < 0) {
          if (d.y < 0) {
            if ((D->val[ at.x - 1 ][ at.y ] == DMAP_IS_WALL) || (D->val[ at.x ][ at.y - 1 ] == DMAP_IS_WALL)) {
              continue;
            }
          } else if (d.y > 0) {
            if ((D->val[ at.x - 1 ][ at.y ] == DMAP_IS_WALL) || (D->val[ at.x ][ at.y + 1 ] == DMAP_IS_WALL)) {
              continue;
            }
          }
        } else if (d.x > 0) {
          if (d.y < 0) {
            if ((D->val[ at.x + 1 ][ at.y ] == DMAP_IS_WALL) || (D->val[ at.x ][ at.y - 1 ] == DMAP_IS_WALL)) {
              continue;
            }
          } else if (d.y > 0) {
            if ((D->val[ at.x + 1 ][ at.y ] == DMAP_IS_WALL) || (D->val[ at.x ][ at.y + 1 ] == DMAP_IS_WALL)) {
              continue;
            }
          }
        }
      }

      int const c = D->val[ tx ][ ty ];
      if (std::cmp_less_equal(c, lowest)) {
        got    = true;
        best   = t;
        lowest = c;
      }
    }

    if (! got) {
      return out;
    }

    out.push_back(best);
    at                         = best;
    walked[ best.x ][ best.y ] = true;
  }
  return out;
}

#if 0
static auto dmap_solve_allow_diagonal(const Dmap *D, const bpoint start) -> std::vector< bpoint >
{
  static const std::vector< bpoint > all_deltas = {
      bpoint(-1, -1), bpoint(1, -1), bpoint(-1, 1), bpoint(1, 1), bpoint(0, -1), bpoint(-1, 0), bpoint(1, 0), bpoint(0, 1),
  };
  return dmap_solve_(D, start, all_deltas, true);
}
#endif

static auto dmap_solve_manhattan(const Dmap *D, const bpoint start) -> std::vector< bpoint >
{
  static const std::vector< bpoint > all_deltas = {
      bpoint(0, -1),
      bpoint(-1, 0),
      bpoint(1, 0),
      bpoint(0, 1),
  };
  return dmap_solve_(D, start, all_deltas, false);
}

//
// Try to solve diagonally first. Then fallback to manhattan if we cannot.
//
auto dmap_solve(Gamep g, Levelsp v, Levelp l, Thingp t, const Dmap *D, const bpoint start) -> std::vector< bpoint >
{
  //
  // No path? Intentionally not allowing diagonal moves here as that allows the
  // player to take a shortcut around walls. e.g.
  //
  // ..###    ..###
  // ..#$#    ..#$#
  // .@### -> ..###
  // .....    ..@..
  //
  // if we allow the diagonal move then the hidden treasure in the middle of the
  // wall gets exposed.
  //
  auto p = dmap_solve_manhattan(D, start);

  //
  // Shorten paths, but don't cut corners
  //
  // x..##    x..##
  // xx.##    .x.##
  // .xx## -> ..x##
  // ..xxx    ..xxx
  //
  thing_path_shorten(g, v, l, t, p);
  return p;
}
