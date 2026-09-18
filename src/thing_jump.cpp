//
// Copyright goblinhack@gmail.com
//

#include "my_bpoint.hpp"
#include "my_callstack.hpp"
#include "my_dice_rolls.hpp"
#include "my_fpoint.hpp"
#include "my_game_defs.hpp"
#include "my_level.hpp"
#include "my_level_inlines.hpp"
#include "my_line.hpp"
#include "my_main.hpp"
#include "my_spoint.hpp"
#include "my_thing.hpp"
#include "my_thing_callbacks.hpp"
#include "my_thing_inlines.hpp"
#include "my_tile.hpp"
#include "my_tp.hpp"
#include "my_types.hpp"
#include "my_ui.hpp"

#include <cmath>
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

    //
    // Splash!
    //
    if (level_is_water_shallow_bool(g, v, l, thing_at(g, v, l, me)) || level_is_water_deep_bool(g, v, l, thing_at(g, v, l, me))) {
      thing_sound_play(g, v, l, me, "splash");
    }
  }
}

void thing_is_jumping_unset(Gamep g, Levelsp v, Levelp l, Thingp me)
{
  TRACE_DEBUG();

  if (! me->_is_jumping) {
    return;
  }

  THING_DBG(g, v, l, me, "jump end");

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
  auto curr_at = thing_at(g, v, l, me);
  //
  // Need to allow diagonal jumps of n tiles
  //
  // x--a-->
  // |    .
  // |   .
  // b  . sqrt((a*a) + (b*b))
  // | .
  // |.
  // v
  const auto how_far_i_want_to_jump = std::floor(distance(curr_at, to));

  THING_DBG(g, v, l, me, "curr_at (%d,%d), jump to (%d,%d)", curr_at.x, curr_at.y, to.x, to.y);
  THING_DBG(g, v, l, me, "how_far_i_want_to_jump %f, how_far_i_can_jump %d", how_far_i_want_to_jump, how_far_i_can_jump);
  TRACE_INDENT();

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
    THING_DBG(g, v, l, me, "trying to jump too far");
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
  auto at        = thing_at(g, v, l, me);
  auto jump_path = draw_line(at, to);

  for (auto intermediate : std::ranges::reverse_view(jump_path)) {
    auto *what = level_is_obs_to_jumping_over(g, v, l, intermediate, me);
    if (what != nullptr) {
      return what;
    }
  }
  return nullptr;
}

//
// Handles player and monster jumps
//
[[nodiscard]] auto thing_jump_to(Gamep g, Levelsp v, Levelp l, Thingp me, bpoint to, bool warn) -> bool
{
  THING_DBG(g, v, l, me, "jump to (%d,%d)", to.x, to.y);
  TRACE_INDENT();

  if (is_oob_or_border(to)) [[unlikely]] {
    return false;
  }

  auto at = thing_at(g, v, l, me);
  if (to == at) {
    return false;
  }

  if (! thing_is_able_to_jump(g, v, l, me)) {
    return false;
  }

  //
  // Give a chance of escape
  //
  if (thing_is_engulfed(me)) {
    THING_DBG(g, v, l, me, "move to next: not possible, engulfed, lunge");
    if (thing_stat_success(g, v, l, me, THING_STAT_LUCK, TARGET_ROLL_ENGULFED)) {
      if (thing_is_player(me)) {
        topcon(UI_IMPORTANT_FMT_STR "You were engulfed but break free!" UI_RESET_FMT);
      }
      (void) thing_is_engulfed_try_unset(g, v, l, me);
    } else {
      (void) thing_lunge(g, v, l, me, to);
      if (thing_is_player(me)) {
        topcon(UI_IMPORTANT_FMT_STR "You are engulfed and cannot jump!" UI_RESET_FMT);
        (void) level_tick_begin_requested(g, v, l, "player failed to move");
      }
      return false;
    }
  }

  //
  // Give a chance of escape
  //
  if (thing_is_ensnared(me)) {
    THING_DBG(g, v, l, me, "move to next: not possible, ensnared, lunge");
    if (thing_stat_success(g, v, l, me, THING_STAT_DEX, TARGET_ROLL_ENSNARED)) {
      if (thing_is_player(me)) {
        topcon(UI_IMPORTANT_FMT_STR "You were ensnared but break free!" UI_RESET_FMT);
      }
      (void) thing_is_ensnared_try_unset(g, v, l, me);
    } else {
      (void) thing_lunge(g, v, l, me, to);
      if (thing_is_player(me)) {
        topcon(UI_IMPORTANT_FMT_STR "You are ensnared and cannot jump!" UI_RESET_FMT);
        (void) level_tick_begin_requested(g, v, l, "player failed to move");
      }
      return false;
    }
  }

  //
  // If jumping too far, truncate the jump
  //
  auto how_far_i_can_jump = thing_distance_jump(g, v, l, me);
  if (how_far_i_can_jump == 0) {
    if (thing_is_wooden_leg(g, v, l, me)) {
      //
      // Struggle on
      //
    } else {
      if (thing_is_player(me)) {
        topcon("You are too tired to jump.");
      }
      return false;
    }
  }

  thing_jump_truncate(g, v, l, me, to, how_far_i_can_jump);

  auto how_far_i_want_to_jump = static_cast< int >(floor(distance(at, to)));

  THING_DBG(g, v, l, me, "jump to (%d,%d) (final, how_far_i_can_jump:%d how_far_i_want_to_jump %d)", to.x, to.y, how_far_i_can_jump,
            how_far_i_want_to_jump);
  TRACE_INDENT();

  //
  // Check if jumping over something we cannot
  //
  auto *obs = thing_jump_something_in_the_way(g, v, l, me, to);
  if (obs != nullptr) {
    if (thing_is_player(me)) {
      if (warn) {
        auto the_thing = thing_name_long_the(g, v, l, obs);
        topcon(UI_WARN_FMT_STR "You cannot jump over %s." UI_RESET_FMT, the_thing.c_str());
      }
    }
    return false;
  }

  bool blocked = {};

  //
  // No jumping onto lava
  //
  if (thing_is_monst(me)) {
    if (! thing_can_move_to_ai(g, v, l, me, to)) {
      blocked = true;
      THING_DBG(g, v, l, me, "ai blocked jump");
    }
  }

  //
  // No landing in solid obstacles
  //
  if (to != at) {
    if (thing_is_wooden_leg(g, v, l, me)) {
      if (level_is_obs_to_movement(g, v, l, to, me) != nullptr) {
        blocked = true;
        if (thing_is_player(me)) {
          if (warn) {
            topcon("There is something in the way there.");
          }
        }

        THING_DBG(g, v, l, me, "something in the way of jumping onto");
      }
    } else {
      if (level_is_obs_to_jumping_onto(g, v, l, to, me) != nullptr) {
        blocked = true;
        if (thing_is_player(me)) {
          if (warn) {
            topcon("There is something in the way of jumping there.");
          }
        }

        THING_DBG(g, v, l, me, "something in the way of jumping onto");
      }
    }
  }

  if (blocked) {
    //
    // We could be trying to land on the player. Try again, but with a shorter distance.
    //
    if (how_far_i_want_to_jump > 1) {
      thing_jump_truncate(g, v, l, me, to, how_far_i_want_to_jump - 1);
      THING_DBG(g, v, l, me, "try truncated jump to (%d,%d)", to.x, to.y);

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
  // Look in the direction of jumping
  //
  auto dx = to.x - at.x;
  auto dy = to.y - at.y;
  thing_set_dir_from_delta(g, v, l, me, dx, dy);

  //
  // Halve stamina for successfiul jumps
  //
  if (thing_is_wooden_leg(g, v, l, me)) {
    (void) thing_stamina_decr(g, v, l, me, 1);
  } else {
    auto stamina = static_cast< int >(static_cast< float >(thing_stamina(g, v, l, me)) * 0.8);
    (void) thing_stamina_set(g, v, l, me, stamina);
  }

  THING_DBG(g, v, l, me, "jump begin delta %d,%d", dx, dy);

  //
  // Drag the engulfed for the jump
  //
  if (thing_is_able_to_engulf(me)) {
    FOR_ALL_THINGS_AT(g, v, l, it, at)
    {
      if (thing_is_engulfed(it)) {
        THING_DBG(g, v, l, it, "is engulfed and needs to follow the engulfer");

        if (! thing_warp_to(g, v, l, it, to)) {
          THING_DBG(g, v, l, it, "is engulfed but could not be moved");
          (void) thing_is_engulfed_try_unset(g, v, l, it);
        }
      }
    }
  }

  return true;
}

[[nodiscard]] auto thing_distance_jump_max(Gamep g, Levelsp v, Levelp l, Thingp me) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    ERR("no thing pointer");
    return 0;
  }

  return me->_distance_jump;
}

[[nodiscard]] auto thing_distance_jump(Gamep g, Levelsp v, Levelp l, Thingp me) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    ERR("no thing pointer");
    return 0;
  }

  auto d = me->_distance_jump;

  auto stamina     = thing_stamina(g, v, l, me);
  auto stamina_max = thing_stamina_max(g, v, l, me);

  if (stamina < stamina_max / 2) {
    d /= 2;
  }

  if (stamina < stamina_max / 4) {
    d /= 4;
  }

  if (stamina != 0) {
    if (d == 0) {
      d = 1;
    }
  }

  return d;
}

[[nodiscard]] auto thing_distance_jump_set(Gamep g, Levelsp v, Levelp l, Thingp me, int val) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return me->_distance_jump = val;
}

[[nodiscard]] auto thing_distance_jump_incr(Gamep g, Levelsp v, Levelp l, Thingp me, int val) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return me->_distance_jump += val;
}

[[nodiscard]] auto thing_distance_jump_decr(Gamep g, Levelsp v, Levelp l, Thingp me, int val) -> int
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

[[nodiscard]] auto thing_is_obs_to_jumping_out_of(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_obs_to_jumping_out_of) != 0;
}

[[nodiscard]] auto thing_is_obs_to_jumping_over(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }

  //
  // Unless open
  //
  if (thing_is_openable(t)) {
    if (thing_is_open(t)) {
      return false;
    }
  }

  return tp_flag(thing_tp(t), is_obs_to_jumping_over) != 0;
}

[[nodiscard]] auto thing_is_obs_to_jumping_onto(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }

  //
  // Unless open
  //
  if (thing_is_openable(t)) {
    if (thing_is_open(t)) {
      return false;
    }
  }

  return tp_flag(thing_tp(t), is_obs_to_jumping_onto) != 0;
}

[[nodiscard]] auto thing_is_able_to_jump_land_then_pounce(Gamep g, Levelsp v, Levelp l, Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }

  if (thing_is_dead(t)) {
    return false;
  }

  if (thing_is_levitating(g, v, l, t)) {
    return false;
  }

  return tp_flag(thing_tp(t), is_able_to_jump_land_then_pounce) != 0;
}

[[nodiscard]] auto thing_is_able_to_jump(Gamep g, Levelsp v, Levelp l, Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }

  if (thing_is_dead(t)) {
    return false;
  }

  if (thing_is_levitating(g, v, l, t)) {
    return false;
  }

  return tp_flag(thing_tp(t), is_able_to_jump) != 0;
}

[[nodiscard]] auto thing_is_able_to_jump_attack(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }

  bool const out = tp_flag(thing_tp(t), is_able_to_jump_attack) != 0;

  if (out) {
    if (d100() < thing_jump_attack_pct_chance(t)) {
      return false;
    }
  }

  return out;
}

[[nodiscard]] auto thing_jump_attack_pct_chance(Thingp t) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_jump_attack_pct_chance;
}

[[nodiscard]] auto thing_jump_attack_pct_chance_set(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_jump_attack_pct_chance = val;
}

[[nodiscard]] auto thing_jump_attack_pct_chance_incr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_jump_attack_pct_chance += val;
}

[[nodiscard]] auto thing_jump_attack_pct_chance_decr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  if (static_cast< int >(t->_jump_attack_pct_chance) - val <= 0) {
    return t->_jump_attack_pct_chance = 0;
  }
  return t->_jump_attack_pct_chance -= val;
}
