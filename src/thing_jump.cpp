//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_level_inlines.hpp"
#include "my_line.hpp"
#include "my_main.hpp"
#include "my_thing_inlines.hpp"

#include <ranges>

void thing_is_jumping_set(Gamep g, Levelsp v, Levelp l, Thingp me, bool val)
{
  TRACE_DEBUG();

  if (me == nullptr) {
    ERR("no thing pointer");
    return;
  }

  if (me->_is_jumping == static_cast< int >(val)) {
    return;
  }
  me->_is_jumping = val;

  if (val) {
    thing_on_jump_begin(g, v, l, me);
  } else {
    thing_on_jump_end(g, v, l, me);
  }
}

void thing_is_jumping_unset(Gamep g, Levelsp v, Levelp l, Thingp me)
{
  TRACE_DEBUG();

  THING_DBG(me, "jump end");

  thing_is_jumping_set(g, v, l, me, false);
}

//
// If jumping too far, truncate the jump
//
static void thing_jump_truncate(Gamep g, Levelsp v, Levelp l, Thingp me, bpoint &to, int how_far_i_can_jump)
{
  //
  // Add some random delta for fun and some for diagonals
  //
  auto        curr_at                = thing_at(me);
  float const how_far_i_want_to_jump = distance(curr_at, to);

  //
  // Cannot jump in lava for example
  //
  if (level_is_obs_to_jumping_out_of(g, v, l, curr_at, me) != nullptr) {
    how_far_i_can_jump = 1;
  }

  //
  // Check if trying to jump too far.
  //
  if (how_far_i_want_to_jump > how_far_i_can_jump) {
    //
    // Yep. Trying to jump too far.
    //
    fpoint u = make_fpoint(to) - make_fpoint(curr_at);
    u.unit();
    u *= how_far_i_can_jump;

    fpoint const fto = make_fpoint(curr_at) + u;

    to = make_bpoint(fto);
  }
}

//
// Check if jumping over something we cannot
//
static auto thing_jump_something_in_the_way(Gamep g, Levelsp v, Levelp l, Thingp me, bpoint to) -> Thingp
{
  auto at        = thing_at(me);
  auto jump_path = draw_line(at, to);

  for (auto intermediate : std::ranges::reverse_view(jump_path)) {
    auto *what = level_is_obs_to_jump_over(g, v, l, intermediate, me);
    if (what != nullptr) {
      return what;
    }
  }
  return nullptr;
}

//
// Handles player and monster jumps
//
auto thing_jump_to(Gamep g, Levelsp v, Levelp l, Thingp me, bpoint to, bool warn) -> bool
{
  THING_DBG(me, "jump to %d,%d", to.x, to.y);
  TRACE_INDENT();

  if (is_oob_or_border(to)) [[unlikely]] {
    return false;
  }

  auto at = thing_at(me);
  if (to == at) {
    return false;
  }

  if (! thing_is_able_to_jump(me)) {
    return false;
  }

  //
  // If jumping too far, truncate the jump
  //
  auto how_far_i_can_jump = thing_distance_jump(me);
  if (! how_far_i_can_jump) {
    if (thing_is_player(me)) {
      topcon("You are too tired to jump.");
    }
    return false;
  }

  thing_jump_truncate(g, v, l, me, to, how_far_i_can_jump);

  auto how_far_i_want_to_jump = static_cast< int >(floor(distance(at, to)));

  THING_DBG(me, "jump to %d,%d (final)", to.x, to.y);
  TRACE_INDENT();

  //
  // Check if jumping over something we cannot
  //
  auto *obs = thing_jump_something_in_the_way(g, v, l, me, to);
  if (obs != nullptr) {
    if (thing_is_player(me)) {
      if (warn) {
        auto the_thing = thing_name_long_the(g, v, l, obs);
        topcon("You cannot jump over %s.", the_thing.c_str());
      }
    }
    return false;
  }

  //
  // No landing in solid obstacles
  //
  if (level_is_obs_to_jumping_onto(g, v, l, to, me) != nullptr) {
    if (thing_is_player(me)) {
      if (warn) {
        topcon("There is something in the way of jumping there.");
      }
    }

    THING_DBG(me, "something in the way of jumping onto");
    TRACE_INDENT();

    //
    // We could be trying to land on the player. Try again, but with a shorter distance.
    //
    if (how_far_i_want_to_jump > 1) {
      thing_jump_truncate(g, v, l, me, to, how_far_i_want_to_jump - 1);
      THING_DBG(me, "truncated jump to %d,%d", to.x, to.y);
      return thing_jump_to(g, v, l, me, to, warn);
    }

    return false;
  }

  (void) thing_pop(g, v, me);

  spoint pix_at;
  pix_at.x = at.x * TILE_WIDTH;
  pix_at.y = at.y * TILE_HEIGHT;

  thing_pix_at_set(g, v, l, me, pix_at);
  thing_moving_from_set(me, at);
  thing_at_set(g, v, l, me, to);
  (void) thing_push(g, v, l, me);

  thing_is_jumping_set(g, v, l, me);

  //
  // Halve stamina for successfiul jumps
  //
  auto stamina = (int) ((float) thing_stamina(me) * 0.8);
  (void) thing_stamina_set(g, v, l, me, stamina);

  THING_DBG(me, "jump begin");

  return true;
}

auto thing_distance_jump_max(Thingp me) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    ERR("no thing pointer");
    return 0;
  }

  return me->_distance_jump;
}

auto thing_distance_jump(Thingp me) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    ERR("no thing pointer");
    return 0;
  }

  auto d = me->_distance_jump;

  auto stamina     = thing_stamina(me);
  auto stamina_max = thing_stamina_max(me);

  if (stamina < stamina_max / 2) {
    d /= 2;
  }

  if (stamina < stamina_max / 4) {
    d /= 4;
  }

  if (stamina) {
    if (! d) {
      d = 1;
    }
  }

  return d;
}

auto thing_distance_jump_set(Gamep g, Levelsp v, Levelp l, Thingp me, int val) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return me->_distance_jump = val;
}

auto thing_distance_jump_incr(Gamep g, Levelsp v, Levelp l, Thingp me, int val) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return me->_distance_jump += val;
}

auto thing_distance_jump_decr(Gamep g, Levelsp v, Levelp l, Thingp me, int val) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  if (static_cast< int >(me->_distance_jump) - val <= 0) {
    return me->_distance_jump = 0;
  }
  return me->_distance_jump -= val;
}
