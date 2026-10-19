//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_game.hpp"
#include "my_thing.hpp"
#include "my_thing_inlines.hpp"
#include "my_tp.hpp"
#include "my_types.hpp"

#include <algorithm>
#include <limits>

[[nodiscard]] auto thing_stamina(Gamep g, Levelsp v, Levelp l, Thingp me) -> int
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

  if (thing_is_tireless(g, v, l, me)) {
    return ext->_stamina = ext->_stamina_max;
  }

  return ext->_stamina;
}

[[nodiscard]] auto thing_stamina_set(Gamep g, Levelsp v, Levelp l, Thingp me, int val) -> int
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

  if (val > std::numeric_limits< decltype(ext->_stamina) >::max()) {
    thing_err(g, v, l, me, "value overflow: %d", val);
    return 0;
  }

  game_request_to_remake_ui_set(g);

  ext->_stamina = val;
  if (ext->_stamina_max != 0) {
    ext->_stamina = std::min(ext->_stamina_max, ext->_stamina);
  }

  if (thing_is_tireless(g, v, l, me)) {
    if (thing_is_player(me)) {
      THING_DBG(g, v, l, me, "stamina set to %d (tireless)", ext->_stamina);
    }

    ext->_stamina = ext->_stamina_max;
  }

  if (thing_is_player(me)) {
    THING_DBG(g, v, l, me, "stamina set to %d", ext->_stamina);
  }

  return ext->_stamina;
}

[[nodiscard]] auto thing_stamina_incr(Gamep g, Levelsp v, Levelp l, Thingp me, int val) -> int
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

  return thing_stamina_set(g, v, l, me, ext->_stamina + val);
}

[[nodiscard]] auto thing_stamina_decr(Gamep g, Levelsp v, Levelp l, Thingp me, int val) -> int
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

  if (static_cast< int >(ext->_stamina) - val <= 0) {
    return ext->_stamina = 0;
  }
  THING_DBG(g, v, l, me, "val %d", val);

  return thing_stamina_set(g, v, l, me, ext->_stamina - val);
}

[[nodiscard]] auto thing_stamina_max(Gamep g, Levelsp v, Levelp l, Thingp me) -> int
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

  return ext->_stamina_max;
}

[[nodiscard]] auto thing_stamina_max_set(Gamep g, Levelsp v, Levelp l, Thingp me, int val) -> int
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

  if (val > std::numeric_limits< decltype(ext->_stamina_max) >::max()) {
    thing_err(g, v, l, me, "value overflow: %d", val);
    return 0;
  }

  game_request_to_remake_ui_set(g);

  auto new_stamina_max = ext->_stamina_max = val;

  if (ext->_stamina > new_stamina_max) {
    (void) thing_stamina_set(g, v, l, me, new_stamina_max);
  }

  if (thing_is_player(me)) {
    THING_DBG(g, v, l, me, "stamina max set to %d", ext->_stamina_max);
  }

  return new_stamina_max;
}

[[nodiscard]] auto thing_stamina_max_incr(Gamep g, Levelsp v, Levelp l, Thingp me, int val) -> int
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

  return thing_stamina_max_set(g, v, l, me, ext->_stamina_max + val);
}

[[nodiscard]] auto thing_stamina_max_decr(Gamep g, Levelsp v, Levelp l, Thingp me, int val) -> int
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

  if (static_cast< int >(ext->_stamina_max) - val <= 0) {
    return ext->_stamina_max = 0;
  }

  return thing_stamina_max_set(g, v, l, me, ext->_stamina_max - val);
}

[[nodiscard]] auto thing_is_shown_stamina(Thingp me) -> bool
{
  TRACE_DEBUG();

  if (me == nullptr) {
    no_thing_pointer();
    return false;
  }
  return tp_flag(thing_tp(me), is_shown_stamina) != 0;
}
