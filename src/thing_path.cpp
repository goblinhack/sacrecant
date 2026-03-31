//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_level.hpp"
#include "my_main.hpp"
#include "my_thing.hpp"
#include "my_thing_callbacks.hpp"
#include "my_thing_inlines.hpp"

#include <print>

//
// Shorten paths, but don't cut corners
//
// x..##    x..##
// xx.##    .x.##
// .xx## -> ..x##
// ..xxx    ..xxx
//
void thing_path_shorten(Gamep g, Levelsp v, Levelp l, Thingp t, std::vector< bpoint > &path)
{
  bool const debug = false;

  auto at = thing_at(t);
  if (debug) {
    std::println("start (player {},{})", at.x, at.y);
    for (auto p : path) {
      std::print("({},{}), ", p.x, p.y);
    }
    std::println("");
    std::println("");
  }

  //
  // Trim short paths
  //
  if (path.size() == 2) {
    auto px = at.x;
    auto py = at.y;

    auto n  = path[ 0 ];
    auto nx = n.x;
    auto ny = n.y;

    auto m  = path[ 1 ];
    auto mx = m.x;
    auto my = m.y;

    //
    // .....     .....
    // .....     .....
    // ..@.. --> ..@..
    // .mn..     .m...
    // .....     .....
    //
    // .....     .....
    // .....     .....
    // .X@.. --> .X@..
    // .mn..     .mn..
    // .....     .....
    //
    if (px - 1 == mx && py + 1 == my && px == nx && py + 1 == ny
        && (level_alive_is_obs_to_cursor_path(g, v, l, bpoint(px - 1, py)) == nullptr)) {
      path.erase(path.begin());
      return;
    }

    //
    // .....     .....
    // .....     .....
    // .n@.. --> ..@..
    // .m...     .m...
    // .....     .....
    //
    // .....     .....
    // .....     .....
    // .n@.. --> .n@..
    // .mX..     .mX..
    // .....     .....
    //
    if (px - 1 == mx && py + 1 == my && px - 1 == nx && py == ny
        && (level_alive_is_obs_to_cursor_path(g, v, l, bpoint(px, py + 1)) == nullptr)) {
      path.erase(path.begin());
      return;
    }

    if (px + 1 == mx && py + 1 == my && px == nx && py + 1 == ny
        && (level_alive_is_obs_to_cursor_path(g, v, l, bpoint(px + 1, py)) == nullptr)) {
      path.erase(path.begin());
      return;
    }

    if (px + 1 == mx && py + 1 == my && px + 1 == nx && py == ny
        && (level_alive_is_obs_to_cursor_path(g, v, l, bpoint(px, py + 1)) == nullptr)) {
      path.erase(path.begin());
      return;
    }

    if (px - 1 == mx && py - 1 == my && px == nx && py - 1 == ny
        && (level_alive_is_obs_to_cursor_path(g, v, l, bpoint(px - 1, py)) == nullptr)) {
      path.erase(path.begin());
      return;
    }

    if (px - 1 == mx && py - 1 == my && px - 1 == nx && py == ny
        && (level_alive_is_obs_to_cursor_path(g, v, l, bpoint(px, py - 1)) == nullptr)) {
      path.erase(path.begin());
      return;
    }

    if (px + 1 == mx && py - 1 == my && px == nx && py - 1 == ny
        && (level_alive_is_obs_to_cursor_path(g, v, l, bpoint(px + 1, py)) == nullptr)) {
      path.erase(path.begin());
      return;
    }

    if (px + 1 == mx && py - 1 == my && px + 1 == nx && py == ny
        && (level_alive_is_obs_to_cursor_path(g, v, l, bpoint(px, py - 1)) == nullptr)) {
      path.erase(path.begin());
      return;
    }
  }

  for (; /*ever*/;) {
    auto   modified = false;
    size_t i        = 0;

    for (; /*ever*/;) {
      if (i + 2 >= path.size()) {
        break;
      }

      if (debug) {
        for (auto p : path) {
          std::print("({},{}), ", p.x, p.y);
        }
        std::println("");
      }

      auto p  = path[ i ];
      auto px = p.x;
      auto py = p.y;

      auto n  = path[ i + 1 ];
      auto nx = n.x;
      auto ny = n.y;

      auto m  = path[ i + 2 ];
      auto mx = m.x;
      auto my = m.y;

      if (px - 1 == mx && py + 1 == my && px == nx && py + 1 == ny
          && (level_alive_is_obs_to_cursor_path(g, v, l, bpoint(px - 1, py)) == nullptr)) {
        path.erase(path.begin() + i + 1);
        modified = true;
        break;
      }

      if (px - 1 == mx && py + 1 == my && px - 1 == nx && py == ny
          && (level_alive_is_obs_to_cursor_path(g, v, l, bpoint(px, py + 1)) == nullptr)) {
        path.erase(path.begin() + i + 1);
        modified = true;
        break;
      }

      if (px + 1 == mx && py + 1 == my && px == nx && py + 1 == ny
          && (level_alive_is_obs_to_cursor_path(g, v, l, bpoint(px + 1, py)) == nullptr)) {
        path.erase(path.begin() + i + 1);
        modified = true;
        break;
      }

      if (px + 1 == mx && py + 1 == my && px + 1 == nx && py == ny
          && (level_alive_is_obs_to_cursor_path(g, v, l, bpoint(px, py + 1)) == nullptr)) {
        path.erase(path.begin() + i + 1);
        modified = true;
        break;
      }

      if (px - 1 == mx && py - 1 == my && px == nx && py - 1 == ny
          && (level_alive_is_obs_to_cursor_path(g, v, l, bpoint(px - 1, py)) == nullptr)) {
        path.erase(path.begin() + i + 1);
        modified = true;
        break;
      }

      if (px - 1 == mx && py - 1 == my && px - 1 == nx && py == ny
          && (level_alive_is_obs_to_cursor_path(g, v, l, bpoint(px, py - 1)) == nullptr)) {
        path.erase(path.begin() + i + 1);
        modified = true;
        break;
      }

      if (px + 1 == mx && py - 1 == my && px == nx && py - 1 == ny
          && (level_alive_is_obs_to_cursor_path(g, v, l, bpoint(px + 1, py)) == nullptr)) {
        path.erase(path.begin() + i + 1);
        modified = true;
        break;
      }

      if (px + 1 == mx && py - 1 == my && px + 1 == nx && py == ny
          && (level_alive_is_obs_to_cursor_path(g, v, l, bpoint(px, py - 1)) == nullptr)) {
        path.erase(path.begin() + i + 1);
        modified = true;
        break;
      }

      i++;
    }

    if (! modified) {
      break;
    }
  }

  if (static_cast< uint32_t >(! path.empty()) != 0U) {
    if (path[ 0 ] == at) {
      path.erase(path.begin());
    }
  }

  if (debug) {
    std::println("final (player {},{})", at.x, at.y);
    for (auto p : path) {
      std::print("({},{}), ", p.x, p.y);
    }
    std::println("");
    std::println("");
  }
}

//
// Compute the path cost. Diagonals are longer than grid moves.
//
// Also factors in preferred tiles.
//
auto thing_path_cost(Gamep g, Levelsp v, Levelp l, Thingp me, const std::vector< bpoint > &path) -> int
{
  TRACE();

  int cost = 0;

  if (path.empty()) {
    return cost;
  }

  auto prev = thing_at(me);

  for (auto p : path) {
    //
    // Prefer has seen tiles
    //
    if (thing_is_player(me)) {
      if (level_has_seen(g, v, l, p)) {
        cost += 1;
      } else {
        cost += 2;
      }
    }

    //
    // Diagonal moves are more expensive
    //
    if (adjacent_vert_or_horiz(p, prev)) {
      cost += 3;
    } else {
      cost += 4; // Diagonal
    }

    if (level_alive_is_cursor_path_hazard(g, v, l, p) != nullptr) {
      cost += 10;
    }

    if (level_alive_is_obs_to_cursor_path(g, v, l, p) != nullptr) {
      cost += 10;
    }

    //
    // Factor in preferences
    //
    switch (thing_assess_tile(g, v, l, p, me)) {
      case THING_ENVIRON_HATES :    cost += 100; break;
      case THING_ENVIRON_DISLIKES : cost += 50; break;
      case THING_ENVIRON_NEUTRAL :  cost += 0; break;
      case THING_ENVIRON_LIKES :    cost += -1; break;
      case THING_ENVIRON_ENUM_MAX : break;
    }

    prev = p;
  }

  return cost;
}
