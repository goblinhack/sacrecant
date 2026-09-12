//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_game.hpp"
#include "my_main.hpp"
#include "my_thing.hpp"
#include "my_types.hpp"
#include <algorithm>
#include <limits>

[[nodiscard]] auto thing_sac_points(Gamep g, Levelsp v, Levelp l, Thingp t) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_sac_points;
}

[[nodiscard]] auto thing_sac_points_set(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }

  if (val > std::numeric_limits< decltype(t->_sac_points) >::max()) {
    thing_err(g, v, l, t, "value overflow: %d", val);
    return 0;
  }

  game_request_to_remake_ui_set(g);
  t->_sac_points = val;
  if (t->_sac_points_max != 0) {
    t->_sac_points = std::min(t->_sac_points_max, t->_sac_points);
  }
  return t->_sac_points;
}

[[nodiscard]] auto thing_sac_points_incr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  game_request_to_remake_ui_set(g);
  t->_sac_points += val;
  if (t->_sac_points_max != 0) {
    t->_sac_points = std::min(t->_sac_points_max, t->_sac_points);
  }
  return t->_sac_points;
}

[[nodiscard]] auto thing_sac_points_decr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  game_request_to_remake_ui_set(g);

  if (static_cast< int >(t->_sac_points) - val <= 0) {
    return t->_sac_points = 0;
  }

  return t->_sac_points -= val;
}

[[nodiscard]] auto thing_sac_points_max(Gamep g, Levelsp v, Levelp l, Thingp t) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_sac_points_max;
}

[[nodiscard]] auto thing_sac_points_max_set(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }

  if (val > std::numeric_limits< decltype(t->_sac_points_max) >::max()) {
    thing_err(g, v, l, t, "value overflow: %d", val);
    return 0;
  }

  game_request_to_remake_ui_set(g);
  return t->_sac_points_max = val;
}

[[nodiscard]] auto thing_sac_points_max_incr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  game_request_to_remake_ui_set(g);
  return t->_sac_points_max += val;
}

[[nodiscard]] auto thing_sac_points_max_decr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  game_request_to_remake_ui_set(g);

  if (static_cast< int >(t->_sac_points_max) - val <= 0) {
    return t->_sac_points_max = 0;
  }

  return t->_sac_points_max -= val;
}
