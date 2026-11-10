//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_game.hpp"
#include "my_thing.hpp"
#include "my_thing_inlines.hpp"
#include "my_types.hpp"

#include <algorithm>
#include <limits>

[[nodiscard]] auto thing_sac_points(Gamep g, Levelsp v, Levelp l, Thingp me) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    no_thing_pointer();
    return 0;
  }

  auto *ext = thing_ext_struct(g, v, me);
  if (ext == nullptr) {
    return 0;
  }

  return ext->_sac_points;
}

[[nodiscard]] auto thing_sac_points_set(Gamep g, Levelsp v, Levelp l, Thingp me, int val) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    no_thing_pointer();
    return 0;
  }

  auto *ext = thing_ext_struct(g, v, me);
  if (ext == nullptr) {
    return 0;
  }

  THING_DBG(g, v, l, me, "sac points set to %u", val);

  if (val > std::numeric_limits< decltype(ext->_sac_points) >::max()) {
    thing_err(g, v, l, me, "value overflow: %d", val);
    return 0;
  }

  game_request_to_remake_ui_set(g);
  ext->_sac_points = val;

  THING_DBG(g, v, l, me, "sac points now %u", ext->_sac_points);
  return ext->_sac_points;
}

[[nodiscard]] auto thing_sac_points_incr(Gamep g, Levelsp v, Levelp l, Thingp me, int val) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    no_thing_pointer();
    return 0;
  }

  auto *ext = thing_ext_struct(g, v, me);
  if (ext == nullptr) {
    return 0;
  }

  THING_DBG(g, v, l, me, "sac points incr %u", val);
  return thing_sac_points_set(g, v, l, me, ext->_sac_points + val);
}

[[nodiscard]] auto thing_sac_points_decr(Gamep g, Levelsp v, Levelp l, Thingp me, int val) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    no_thing_pointer();
    return 0;
  }

  auto *ext = thing_ext_struct(g, v, me);
  if (ext == nullptr) {
    return 0;
  }

  THING_DBG(g, v, l, me, "sac points decr %u", val);
  return thing_sac_points_set(g, v, l, me, ext->_sac_points + val);
}

