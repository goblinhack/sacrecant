//
// Copyright goblinhack@gmail.com
//

#include "my_bpoint.hpp"
#include "my_callstack.hpp"
#include "my_game_defs.hpp"
#include "my_level.hpp"
#include "my_main.hpp" // NOLINT
#include "my_thing.hpp"
#include "my_thing_callbacks.hpp"
#include "my_tp.hpp"
#include "my_types.hpp"

#include <algorithm> // do not remove
#include <array>     // do not remove
#include <cmath>
#include <cstdint>
#include <map>
#include <string>
#include <utility>
#include <vector>

#ifdef ENABLE_DEBUG_AI_ASTAR
static std::array< std::array< char, MAP_HEIGHT >, MAP_WIDTH > astar_debug;
#endif

using Cost = float;

static int16_t g_tiebreak;

class Nodecost
{
public:
  Nodecost() = default;
  explicit Nodecost(Cost c) : cost(c), tiebreak(++g_tiebreak)
  {
    if (compiler_unused) {
      log("new node %f:%u", c, tiebreak);
    }
  }

  auto operator<(const Nodecost &rhs) const -> bool
  {
    if (compiler_unused) {
      log("compare:");
      log(" a node %f:%u", cost, tiebreak);
      log(" b node %f:%u", rhs.cost, rhs.tiebreak);
    }
    if (cost < rhs.cost) {
      return true;
    }
    if (cost > rhs.cost) {
      return false;
    }

    return tiebreak < rhs.tiebreak;
  }

  Cost    cost {};
  int16_t tiebreak {};
};

class Node
{
public:
  Node() = default;

  class Node *came_from {};
  Nodecost    cost;
  bpoint      at {};
};

//
// The nodemap needs to be sorted by distance so std::map must be used
//
using Nodemap = std::map< Nodecost, Node * >;

class Astar
{
public:
  Astar() = default;

  Gamep   g  = {};
  Levelsp v  = {};
  Levelp  l  = {};
  Thingp  me = {};

  //
  // These are the nodes still to evaluate
  //
  Nodemap open_nodes;

  //
  // These are the nodes we have evaluated
  //
  Nodemap closed_nodes;

  //
  // Start and desired end of the search
  //
  bpoint src {};
  bpoint dst {};

  //
  // Indicates which nodes in the grid we've searched and added to the open list
  //
  std::array< std::array< Node *, MAP_HEIGHT >, MAP_WIDTH > open = {};

  //
  // Indicates which nodes in the grid we've searched and added to the closed list
  //
  std::array< std::array< Node *, MAP_HEIGHT >, MAP_WIDTH > closed = {};

  //
  // A node per grid element. Avoids needing to allocate memory
  //
  std::array< std::array< Node, MAP_HEIGHT >, MAP_WIDTH > nodes = {};

  //
  // Cache of where this thing can move to
  //
  std::array< std::array< bool, MAP_HEIGHT >, MAP_WIDTH > can_move_to_possible_cached     = {};
  std::array< std::array< bool, MAP_HEIGHT >, MAP_WIDTH > can_move_to_possible_cached_set = {};

  std::array< std::array< int16_t, MAP_HEIGHT >, MAP_WIDTH > can_move_to_cost_cached     = {};
  std::array< std::array< int16_t, MAP_HEIGHT >, MAP_WIDTH > can_move_to_cost_cached_set = {};

  [[nodiscard]] auto        can_move_to_possible(const bpoint &to) -> bool;
  [[nodiscard]] auto        can_move_to_cost(const bpoint &to) -> int16_t;
  [[nodiscard]] auto        heuristic(bpoint at) const -> Cost;
  [[nodiscard]] auto        node_init(bpoint next_hop, Nodecost cost) -> Node *;
  [[nodiscard]] auto        solve(bool allow_diagonal) -> std::vector< bpoint >;
  [[nodiscard]] static auto create_path(const Node *came_from) -> std::vector< bpoint >;

  void add_to_closed(Node *n);
  void add_to_open(Node *n);
  void dump();
  void eval_neighbor(Node *current, const bpoint &delta);
  void remove_from_open(Node *n);

  static void init();
};

void Astar::add_to_open(Node *n)
{
  auto  p = n->at;
  auto *o = &open[ p.x ][ p.y ];
  if (*o != nullptr) {
    thing_err(g, v, l, me, "already in open");
    return;
  }
  *o = n;

  if (compiler_unused) {
    thing_log(g, v, l, me, "add to open (cost %f:%u at %d,%d)", n->cost.cost, n->cost.tiebreak, p.x, p.y);
  }

  auto result = open_nodes.insert(std::make_pair(n->cost, n));
  if (! result.second) {
    thing_err(g, v, l, me, "open insert fail");
    return;
  }
}

void Astar::add_to_closed(Node *n)
{
  auto  p = n->at;
  auto *o = &closed[ p.x ][ p.y ];
  if (*o != nullptr) {
    thing_err(g, v, l, me, "already in closed");
    return;
  }
  *o = n;

  if (compiler_unused) {
    thing_log(g, v, l, me, "add to closed (cost %f:%u at %d,%d)", n->cost.cost, n->cost.tiebreak, p.x, p.y);
  }

  auto result = closed_nodes.insert(std::make_pair(n->cost, n));
  if (! result.second) {
    thing_err(g, v, l, me, "closed insert fail (cost %f:%u at %d,%d)", n->cost.cost, n->cost.tiebreak, p.x, p.y);
    return;
  }
}

void Astar::remove_from_open(Node *n)
{
  auto  p = n->at;
  auto *o = &open[ p.x ][ p.y ];
  if (*o == nullptr) {
    thing_err(g, v, l, me, "not in open");
    return;
  }
  *o = nullptr;

  if (compiler_unused) {
    thing_log(g, v, l, me, "remove from open (cost %f:%u at %d,%d)", n->cost.cost, n->cost.tiebreak, p.x, p.y);
  }

  open_nodes.erase(n->cost);
}

[[nodiscard]] auto Astar::heuristic(const bpoint at) const -> Cost
{
  //
  // This can create wiggles in the path as we're always looking at the distance
  // to the end point, and sometimes a diagonal move is closer.
  //
  // Euclidean
  return sort_distance(dst, at);
  //
  // Chebyshev for octagonal movement
  // return std::max(abs(dst.x - at.x), abs(dst.y - at.y));
}

[[nodiscard]] auto Astar::node_init(const bpoint next_hop, Nodecost cost) -> Node *
{
  auto *n = &nodes[ next_hop.x ][ next_hop.y ];

  n->at   = next_hop;
  n->cost = cost;

  return n;
}

//
// Evaluate a neighbor for adding to the open set
//
void Astar::eval_neighbor(Node *current, const bpoint &delta)
{
  const auto next_hop = current->at + delta;

  //
  // If in the closed set already, ignore.
  //
  if (closed[ next_hop.x ][ next_hop.y ] != nullptr) {
    return;
  }

  Cost cost = current->cost.cost + (heuristic(next_hop) / 10);

  //
  // If there is something in the way that would block us moving here,
  // like another monster, then encourage a path around that
  //
  if (! can_move_to_possible(next_hop)) {
#ifdef ENABLE_DEBUG_AI_ASTAR
    astar_debug[ next_hop.x ][ next_hop.y ] = 'X';
#endif
    return;
  }

  cost += can_move_to_cost(next_hop);

  Node *neighbor = open[ next_hop.x ][ next_hop.y ];
  if (neighbor == nullptr) {
    auto ncost          = Nodecost(cost);
    neighbor            = node_init(next_hop, ncost);
    neighbor->came_from = current;
    add_to_open(neighbor);
    return;
  }

  if (cost < neighbor->cost.cost) {
    auto ncost = Nodecost(cost);
    remove_from_open(neighbor);
    neighbor->came_from = current;
    neighbor->cost      = ncost;
    add_to_open(neighbor);
  }
}

[[nodiscard]] auto Astar::create_path(const Node *came_from) -> std::vector< bpoint >
{
  static const std::vector< bpoint > empty;

  std::vector< bpoint > out;

  while (came_from != nullptr) {
    if (came_from->came_from != nullptr) {
      out.push_back(came_from->at);
    }
    came_from = came_from->came_from;
  }

  std::ranges::reverse(out);
  return out;
}

void Astar::init()
{
  g_tiebreak = 0;

#ifdef ENABLE_DEBUG_AI_ASTAR
  astar_debug = {};
#endif
}

[[nodiscard]] auto Astar::can_move_to_possible(const bpoint &to) -> bool
{
  if (to == dst) {
    return true;
  }

  if (! can_move_to_possible_cached_set[ to.x ][ to.y ]) {
    can_move_to_possible_cached_set[ to.x ][ to.y ]    = true;
    return can_move_to_possible_cached[ to.x ][ to.y ] = thing_can_move_to_possible(g, v, l, me, to, true);
  }

  return can_move_to_possible_cached[ to.x ][ to.y ];
}

[[nodiscard]] auto Astar::can_move_to_cost(const bpoint &to) -> int16_t
{
  if (to == dst) {
    return 0;
  }

  if (can_move_to_cost_cached_set[ to.x ][ to.y ] == 0U) {
    can_move_to_cost_cached_set[ to.x ][ to.y ] = 1U;
    int16_t cost                                = 0;

    switch (thing_assess_tile(g, v, l, to, me)) {
      case THING_ENVIRON_HATES :    cost = 100; break;
      case THING_ENVIRON_DISLIKES : cost = 50; break;
      case THING_ENVIRON_NEUTRAL :  cost = 1; break;
      case THING_ENVIRON_LIKES :    cost = -1; break;
      case THING_ENVIRON_ENUM_MAX : break;
    }

    if (! thing_is_levitating(g, v, l, me)) {
      if (level_is_chasm_bool(g, v, l, to)) {
        cost += 150;
      }
    }

    return can_move_to_cost_cached[ to.x ][ to.y ] = cost;
  }

  return can_move_to_cost_cached[ to.x ][ to.y ];
}

[[nodiscard]] auto Astar::solve(bool allow_diagonal) -> std::vector< bpoint >
{
  init();

  static const std::vector< bpoint > empty;

  auto  ncost    = Nodecost(heuristic(src));
  auto *neighbor = node_init(src, ncost);

  add_to_open(neighbor);

  while (! open_nodes.empty()) {
    auto  c       = open_nodes.begin();
    Node *current = c->second;

    if (compiler_unused) {
      log("open nodes:");
      for (auto n : open_nodes) {
        auto p = n.first;
        log(" %f:%u", p.cost, p.tiebreak);
      }
    }

    if (compiler_unused) {
      log("closed nodes:");
      for (auto n : closed_nodes) {
        auto p = n.first;
        log(" %f:%u", p.cost, p.tiebreak);
      }
    }

    //
    // Reached the target?
    //
    auto at = current->at;
    if (at == dst) {
      return create_path(current);
    }

#ifdef ENABLE_DEBUG_AI_ASTAR
    astar_debug[ at.x ][ at.y ] = '?';
#endif

    remove_from_open(current);
    add_to_closed(current);

    //
    // No moving into the border
    //
    if ((at.x == 0) || (at.x == MAP_WIDTH - 1) || (at.y == 0) || (at.y == MAP_HEIGHT - 1)) {
      continue;
    }

    eval_neighbor(current, bpoint(-1, 0));
    eval_neighbor(current, bpoint(1, 0));
    eval_neighbor(current, bpoint(0, -1));
    eval_neighbor(current, bpoint(0, 1));

    if (allow_diagonal) {
      if (thing_is_able_to_move_through_walls(me)) {
        //
        // Can move through walls in any direction
        //
        eval_neighbor(current, bpoint(-1, -1));
        eval_neighbor(current, bpoint(-1, 1));
        eval_neighbor(current, bpoint(1, -1));
        eval_neighbor(current, bpoint(1, 1));
      } else {
        //
        // Don't allow shortcuts through diagonal walls
        //
        // Don't allow shortcuts across chasms, hence _possible instead of _ai
        //
        if (! can_move_to_possible(bpoint(at.x - 1, at.y)) || ! can_move_to_possible(bpoint(at.x, at.y - 1))) {
          //
          // Block these paths
          //

          // b#.
          // #a.
          // ...

          // b..
          // #a.
          // ...

          // b#.
          // .a.
          // ...
        } else {
          eval_neighbor(current, bpoint(-1, -1));
        }

        if (! can_move_to_possible(bpoint(at.x - 1, at.y)) || ! can_move_to_possible(bpoint(at.x, at.y + 1))) {
          //
          // Block these paths
          //

          // ...
          // #a.
          // .#.

          // ...
          // #a.
          // ...

          // ...
          // .a.
          // .#.
        } else {
          eval_neighbor(current, bpoint(-1, 1));
        }

        if (! can_move_to_possible(bpoint(at.x + 1, at.y)) || ! can_move_to_possible(bpoint(at.x, at.y - 1))) {
          //
          // Block these paths
          //

          // .#.
          // .a#
          // ...

          // ...
          // .a#
          // ...

          // .#.
          // .a.
          // ...
        } else {
          eval_neighbor(current, bpoint(1, -1));
        }

        if (! can_move_to_possible(bpoint(at.x + 1, at.y)) || ! can_move_to_possible(bpoint(at.x, at.y + 1))) {
          //
          // Block these paths
          //

          // ...
          // .a#
          // .#.

          // ...
          // .a#
          // ...

          // ...
          // .a.
          // .#.
        } else {
          eval_neighbor(current, bpoint(1, 1));
        }
      }
    }
  }

  return empty;
}

void Astar::dump()
{
  THING_DBG(g, v, l, me, "ASTAR:");
  TRACE_INDENT();

  for (auto y = 0; y < MAP_HEIGHT; y++) {
    std::string s;
    for (auto x = 0; x < MAP_WIDTH; x++) {
      std::string buf;

      if (can_move_to_possible(bpoint(x, y))) {
        buf = ".";
      } else {
        buf = "#";
      }

#ifdef ENABLE_DEBUG_AI_ASTAR
      if (astar_debug[ x ][ y ]) {
        buf = astar_debug[ x ][ y ];
      }
#endif

      if (bpoint(x, y) == src) {
        buf = "@";
      }
      if (bpoint(x, y) == dst) {
        buf = "*";
      }

      s += buf;
    }
    THING_DBG(g, v, l, me, "ASTAR:%s", s.c_str());
  }
}

[[nodiscard]] auto astar_solve(Gamep g, Levelsp v, Levelp l, Thingp me, bpoint src, bpoint dst) -> std::vector< bpoint >
{
  TRACE();

  Astar a;
  a.g   = g;
  a.v   = v;
  a.l   = l;
  a.me  = me;
  a.src = src;
  a.dst = dst;

  auto allow_diagonal = thing_is_able_to_move_diagonally(me);
  auto path           = a.solve(allow_diagonal);

#ifdef ENABLE_DEBUG_AI_ASTAR
  for (const auto &p : path) {
    astar_debug[ p.x ][ p.y ] = '^';
  }

  a.dump();
#endif

  return path;
}
