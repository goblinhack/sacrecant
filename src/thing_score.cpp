//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_game.hpp"
#include "my_main.hpp"
#include "my_thing.hpp"
#include "my_types.hpp"

auto thing_score(Gamep g, Thingp t) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    no_thing_pointer();
    return 0;
  }

  auto *ext = thing_player_struct(g);
  if (ext == nullptr) [[unlikely]] {
    return 0;
  }

  return ext->score;
}

auto thing_score_set(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
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

auto thing_score_incr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
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

auto thing_score_decr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
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

auto thing_score_value(Thingp t) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    no_thing_pointer();
    return 0;
  }
  return t->_score_value;
}

auto thing_score_value_set(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    no_thing_pointer();
    return 0;
  }
  return t->_score_value = val;
}

auto thing_score_value_incr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    no_thing_pointer();
    return 0;
  }
  return t->_score_value += val;
}

auto thing_score_value_decr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    no_thing_pointer();
    return 0;
  }
  if (static_cast< int >(t->_score_value) - val <= 0) {
    return t->_score_value = 0;
  }
  return t->_score_value -= val;
}
