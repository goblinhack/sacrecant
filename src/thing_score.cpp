//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_game.hpp"
#include "my_main.hpp"
#include "my_thing.hpp"
#include "my_thing_inlines.hpp"
#include "my_types.hpp"

auto thing_score(Gamep g, Thingp me) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    no_thing_pointer();
    return 0;
  }

  auto *ext = thing_player_struct(g);
  if (ext == nullptr) [[unlikely]] {
    return 0;
  }

  return ext->score;
}

auto thing_score_set(Gamep g, Levelsp v, Levelp l, Thingp me, int val) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    no_thing_pointer();
    return 0;
  }

  auto *ext = thing_player_struct(g);
  if (ext == nullptr) [[unlikely]] {
    return 0;
  }

  game_request_to_remake_ui_set(g);
  return ext->score = val;
}

auto thing_score_incr(Gamep g, Levelsp v, Levelp l, Thingp me, int val) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    no_thing_pointer();
    return 0;
  }

  auto *ext = thing_player_struct(g);
  if (ext == nullptr) [[unlikely]] {
    return 0;
  }

  game_request_to_remake_ui_set(g);
  return ext->score += val;
}

auto thing_score_decr(Gamep g, Levelsp v, Levelp l, Thingp me, int val) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    no_thing_pointer();
    return 0;
  }

  auto *ext = thing_player_struct(g);
  if (ext == nullptr) [[unlikely]] {
    return 0;
  }

  game_request_to_remake_ui_set(g);
  if (static_cast< int >(ext->score) - val <= 0) {
    return ext->score = 0;
  }

  return ext->score -= val;
}

auto thing_score_value(Gamep g, Levelsp v, Levelp l, Thingp me) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    no_thing_pointer();
    return 0;
  }

  auto *ext = thing_ext_struct(g, v, me);
  if (ext == nullptr) [[unlikely]] {
    return 0;
  }

  return ext->_score_value;
}

auto thing_score_value_set(Gamep g, Levelsp v, Levelp l, Thingp me, int val) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    no_thing_pointer();
    return 0;
  }

  auto *ext = thing_ext_struct(g, v, me);
  if (ext == nullptr) [[unlikely]] {
    return 0;
  }

  return ext->_score_value = val;
}

auto thing_score_value_incr(Gamep g, Levelsp v, Levelp l, Thingp me, int val) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    no_thing_pointer();
    return 0;
  }

  auto *ext = thing_ext_struct(g, v, me);
  if (ext == nullptr) [[unlikely]] {
    return 0;
  }

  return ext->_score_value += val;
}

auto thing_score_value_decr(Gamep g, Levelsp v, Levelp l, Thingp me, int val) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    no_thing_pointer();
    return 0;
  }

  auto *ext = thing_ext_struct(g, v, me);
  if (ext == nullptr) [[unlikely]] {
    return 0;
  }

  if (static_cast< int >(ext->_score_value) - val <= 0) {
    return ext->_score_value = 0;
  }
  return ext->_score_value -= val;
}
