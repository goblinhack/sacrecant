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
#include <cstdint>
#include <string>
#include <vector>

#ifdef ENABLE_DEBUG_AI_ASTAR
static std::array< std::array< char, MAP_HEIGHT >, MAP_WIDTH > astar_debug;
#endif

using Cost = float;

//
// Used to break ties between nodes of the same cost, in the order they were created. This is wide
// enough that it cannot wrap during one search, which would upset the ordering.
//
static int32_t g_tiebreak;

class Nodecost
{
public:
  Nodecost() = default;

  explicit Nodecost(Cost c) : cost(c), tiebreak(++g_tiebreak) {}

  Cost    cost;
  int32_t tiebreak;
};

//
// There is one node per grid element, so Node is deliberately left uninitialized. It is always filled
// in by node_init() and the caller before being used, so there is no need to clear them all for every search.
//
class Node
{
public:
  Node() = default;
  class Node *came_from;
  Nodecost    cost;
  bpoint      at;
};

//
// The open set is a binary heap, ordered by cost and then by creation order. When the cost of a node
// improves it is simply pushed again, and the old entry is skipped when it comes to the top.
//
class OpenEntry
{
public:
  Cost    cost;
  int32_t tiebreak;
  Node   *node;
};

//
// Where each grid element is in the search.
//
enum : uint8_t { ASTAR_UNSEEN = 0, ASTAR_OPEN = 1, ASTAR_CLOSED = 2 };

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
  std::vector< OpenEntry > open_heap;

  //
  // Start and desired end of the search
  //
  bpoint src {};
  bpoint dst {};

  //
  // Does this thing move through walls in any direction
  //
  bool wall_walker {};

  //
  // Which nodes in the grid we've searched and added to the open or closed lists
  //
  std::array< std::array< uint8_t, MAP_HEIGHT >, MAP_WIDTH > state = {};

  //
  // A node per grid element. Avoids needing to allocate memory
  //
  std::array< std::array< Node, MAP_HEIGHT >, MAP_WIDTH > nodes {};

  //
  // Cache of where this thing can move to. The values are only read once the matching "set" flag is on.
  //
  std::array< std::array< bool, MAP_HEIGHT >, MAP_WIDTH >    can_move_to_possible_cached {};
  std::array< std::array< uint8_t, MAP_HEIGHT >, MAP_WIDTH > can_move_to_possible_cached_set = {};
  std::array< std::array< int16_t, MAP_HEIGHT >, MAP_WIDTH > can_move_to_cost_cached {};
  std::array< std::array< uint8_t, MAP_HEIGHT >, MAP_WIDTH > can_move_to_cost_cached_set = {};

  [[nodiscard]] auto        can_move_to_possible(const bpoint &to) -> bool;
  [[nodiscard]] auto        can_move_to_cost(const bpoint &to) -> int16_t;
  [[nodiscard]] auto        heuristic(bpoint at) const -> Cost;
  [[nodiscard]] auto        node_init(bpoint next_hop, Nodecost cost) -> Node *;
  [[nodiscard]] auto        solve(bool allow_diagonal) -> std::vector< bpoint >;
  [[nodiscard]] static auto create_path(const Node *came_from) -> std::vector< bpoint >;

  void               add_to_closed(Node *n);
  void               add_to_open(Node *n);
  void               dump();
  void               eval_neighbor(Node *current, const bpoint &delta);
  void               remove_from_open(Node *n);
  [[nodiscard]] auto pop_open() -> Node *;

  static void init();
};

void Astar::add_to_open(Node *n)
{
  auto  p = n->at;
  auto *o = &state[ p.x ][ p.y ];
  if (*o == ASTAR_OPEN) {
    thing_err(g, v, l, me, "already in open");
    return;
  }

  *o = ASTAR_OPEN;

  open_heap.push_back({n->cost.cost, n->cost.tiebreak, n});
  std::ranges::push_heap(open_heap, [](const OpenEntry &a, const OpenEntry &b) -> bool {
    //
    // The heap keeps its "largest" entry on top, so this puts the cheapest on top.
    //
    return (a.cost > b.cost) || ((a.cost == b.cost) && (a.tiebreak > b.tiebreak));
  });
}

void Astar::add_to_closed(Node *n)
{
  auto  p = n->at;
  auto *o = &state[ p.x ][ p.y ];
  if (*o == ASTAR_CLOSED) {
    thing_err(g, v, l, me, "already in closed");
    return;
  }

  *o = ASTAR_CLOSED;
}

void Astar::remove_from_open(Node *n)
{
  auto  p = n->at;
  auto *o = &state[ p.x ][ p.y ];
  if (*o != ASTAR_OPEN) {
    thing_err(g, v, l, me, "not in open");
    return;
  }

  //
  // The entry in the heap is left behind. It is recognised as out of date when it reaches the top.
  //
  *o = ASTAR_UNSEEN;
}

//
// Take the cheapest node off the open set. Returns null if there is none.
//
[[nodiscard]] auto Astar::pop_open() -> Node *
{
  while (! open_heap.empty()) {
    std::ranges::pop_heap(open_heap, [](const OpenEntry &a, const OpenEntry &b) -> bool {
      return (a.cost > b.cost) || ((a.cost == b.cost) && (a.tiebreak > b.tiebreak));
    });
    auto const e = open_heap.back();
    open_heap.pop_back();

    //
    // Skip entries for nodes that have since been closed or given a better cost.
    //
    auto *n = e.node;
    if ((state[ n->at.x ][ n->at.y ] == ASTAR_OPEN) && (n->cost.tiebreak == e.tiebreak)) {
      return n;
    }
  }

  return nullptr;
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
  const bpoint next_hop(static_cast< int8_t >(current->at.x + delta.x), static_cast< int8_t >(current->at.y + delta.y));

  //
  // If in the closed set already, ignore.
  //
  auto const next_state = state[ next_hop.x ][ next_hop.y ];
  if (next_state == ASTAR_CLOSED) {
    return;
  }

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

  Cost cost = current->cost.cost + (heuristic(next_hop) / 10);
  cost += can_move_to_cost(next_hop);

  if (next_state != ASTAR_OPEN) {
    auto  ncost         = Nodecost(cost);
    Node *neighbor      = node_init(next_hop, ncost);
    neighbor->came_from = current;
    add_to_open(neighbor);
    return;
  }

  Node *neighbor = &nodes[ next_hop.x ][ next_hop.y ];
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
  if ((to.x == dst.x) && (to.y == dst.y)) {
    return true;
  }

  if (can_move_to_possible_cached_set[ to.x ][ to.y ] == 0U) {
    can_move_to_possible_cached_set[ to.x ][ to.y ]    = 1U;
    return can_move_to_possible_cached[ to.x ][ to.y ] = thing_can_move_to_possible(g, v, l, me, to, true);
  }

  return can_move_to_possible_cached[ to.x ][ to.y ];
}

[[nodiscard]] auto Astar::can_move_to_cost(const bpoint &to) -> int16_t
{
  if ((to.x == dst.x) && (to.y == dst.y)) {
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

    if (! thing_is_flying(g, v, l, me) && ! thing_is_levitating(g, v, l, me)) {
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

  open_heap.reserve(256);

  if (allow_diagonal) {
    wall_walker = thing_is_wall_walker(g, v, l, me);
  }

  auto  ncost         = Nodecost(heuristic(src));
  auto *neighbor      = node_init(src, ncost);
  neighbor->came_from = nullptr;
  add_to_open(neighbor);

  for (;;) {
    Node *current = pop_open();
    if (current == nullptr) {
      break;
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
      if (wall_walker) {
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

  //
  // The search indexes straight into the grid, so make sure the end points are on it.
  //
  if ((src.x < 0) || (src.y < 0) || (src.x >= MAP_WIDTH) || (src.y >= MAP_HEIGHT) || (dst.x < 0) || (dst.y < 0) || (dst.x >= MAP_WIDTH)
      || (dst.y >= MAP_HEIGHT)) [[unlikely]] {
    return {};
  }

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
