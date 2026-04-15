//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_level.hpp"
#include "my_main.hpp"
#include "my_thing.hpp"
#include "my_thing_inlines.hpp"
#include "my_time.hpp"
#include "my_tp.hpp"

auto thing_stamina(Thingp t) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_stamina;
}

auto thing_stamina_set(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }

  if (val > std::numeric_limits< decltype(t->_stamina) >::max()) {
    thing_err(t, "value overflow: %d", val);
    return 0;
  }

  game_request_to_remake_ui_set(g);
  t->_stamina = val;
  if (t->_stamina_max) {
    t->_stamina = std::min(t->_stamina_max, t->_stamina);
  }
  return t->_stamina;
}

auto thing_stamina_incr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  game_request_to_remake_ui_set(g);
  t->_stamina += val;
  if (t->_stamina_max) {
    t->_stamina = std::min(t->_stamina_max, t->_stamina);
  }
  return t->_stamina;
}

auto thing_stamina_decr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  game_request_to_remake_ui_set(g);

  if (static_cast< int >(t->_stamina) - val <= 0) {
    return t->_stamina = 0;
  }

  return t->_stamina -= val;
}

auto thing_stamina_max(Thingp t) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_stamina_max;
}

auto thing_stamina_max_set(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }

  if (val > std::numeric_limits< decltype(t->_stamina_max) >::max()) {
    thing_err(t, "value overflow: %d", val);
    return 0;
  }

  game_request_to_remake_ui_set(g);
  return t->_stamina_max = val;
}

auto thing_stamina_max_incr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  game_request_to_remake_ui_set(g);
  return t->_stamina_max += val;
}

auto thing_stamina_max_decr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  game_request_to_remake_ui_set(g);

  if (static_cast< int >(t->_stamina_max) - val <= 0) {
    return t->_stamina_max = 0;
  }

  return t->_stamina_max -= val;
}

auto thing_is_stamina_visible(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_stamina_visible) != 0;
}
