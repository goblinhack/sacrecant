//
// Copyright goblinhack@gmail.com
//

#include "my_age_map_inlines.hpp"
#include "my_bpoint.hpp"
#include "my_callstack.hpp"
#include "my_fov_map_inlines.hpp"
#include "my_game_defs.hpp"
#include "my_game_inlines.hpp"
#include "my_level.hpp"
#include "my_level_inlines.hpp"
#include "my_main.hpp"
#include "my_random.hpp"
#include "my_thing.hpp"
#include "my_thing_callbacks.hpp"
#include "my_thing_inlines.hpp"
#include "my_tp.hpp"
#include "my_types.hpp"
#include <cstdint>
#include <string>

//
// Already over the player? Used for engulfers.
//
static auto thing_monst_over_target_player(Gamep g, Levelsp v, Levelp l, Thingp me) -> bool
{
  TRACE();

  auto *player = thing_player(g);
  if (player == nullptr) [[unlikely]] {
    return false;
  }

  if (compiler_unused) {
    thing_can_see_dump(g, v, l, me);
  }

  auto *player_level = game_level_get(g, v, player->level_num);
  auto *monst_level  = game_level_get(g, v, me->level_num);
  if (player_level != monst_level) {
    return false;
  }

  auto target   = thing_at(g, v, l, player);
  auto monst_at = thing_at(g, v, l, me);
  if (target == monst_at) {
    THING_DBG(g, v, l, me, "choose target: over player");
    thing_monst_target_set(g, v, l, me, target);
    return true;
  }

  return false;
}

//
// Can we chase the player?
//
static auto thing_monst_apply_target_path_if_possible(Gamep g, Levelsp v, Levelp l, Thingp me, const bpoint &target_in) -> bool
{
  TRACE();

  bool avoiding {};
  auto target   = target_in;
  auto monst_at = thing_at(g, v, l, me);
  auto dist     = distance(monst_at, target);
  auto v_dist   = thing_distance_vision(g, v, l, me);
  if (v_dist == 0) {
    thing_err(g, v, l, me, "choose target: monst has no vision distance");
    return false;
  }

  if (dist > v_dist) {
    THING_DBG(g, v, l, me, "choose target: player is too far (player %f) (vision %d)", dist, v_dist);
    return false;
  }

  if (dist < thing_distance_avoid_target(me)) {
    THING_DBG(g, v, l, me, "choose target: player is too close (player %f) (vision %d)", dist, v_dist);
    TRACE_INDENT();

    //
    // Can we attack here?
    //
    if (dist <= 1) {
      if (level_is_attackable_by_monst(g, v, l, target) != nullptr) {
        THING_DBG(g, v, l, me, "choose target: close enough to attack");
        TRACE_INDENT();

        if (thing_attack_at(g, v, l, me, target)) {
          THING_DBG(g, v, l, me, "choose target: close attack");
          monst_state_change(g, v, l, me, MONST_STATE_NORMAL);
          return false;
        }
      }
    }

    THING_DBG(g, v, l, me, "choose target: opposite direction (%d,%d)", target.x, target.y);
    target   = monst_at + (monst_at - target);
    avoiding = true;
  }

  THING_DBG(g, v, l, me, "astar thing_monst_choose_target_player");
  auto p = astar_solve(g, v, l, me, monst_at, target);
  if (p.empty()) {
    THING_DBG(g, v, l, me, "choose target: no path to target at (%d,%d)", target.x, target.y);
    return false;
  }

  //
  // Some monsters don't like to get too close
  //
  if (! avoiding) {
    auto d = thing_distance_avoid_target(me);
    while (! p.empty() && (d-- > 0)) {
      p.pop_back();
    }
  }

  if (thing_move_path_apply(g, v, l, me, p)) {
    thing_monst_target_set(g, v, l, me, target);
    THING_DBG(g, v, l, me, "choose target: found path to target");
    return true;
  }

  THING_DBG(g, v, l, me, "choose target: failed to apply path to target");
  return false;
}

//
// Can we hear the player at some location close by?
//
static auto thing_monst_can_hear_player_nearby(Gamep g, Levelsp v, Levelp l, Thingp me) -> bool
{
  TRACE();

  auto *player = thing_player(g);
  if (player == nullptr) [[unlikely]] {
    return false;
  }

  auto at     = thing_at(g, v, l, me);
  auto target = thing_at(g, v, l, player);

  //
  // Can we hear anything half way?
  //
  auto middle = (target + at) / 2;
  THING_DBG(g, v, l, me, "try middle hearing target %d,%d", middle.x, middle.y);
  if (thing_vision_can_hear_tile(g, v, l, me, middle)) {
    THING_DBG(g, v, l, me, "choose target: can hear something half way to the player");
    if (thing_monst_apply_target_path_if_possible(g, v, l, me, middle)) {
      return true;
    }
  }

  //
  // Try some alternative hearing targets
  //
  for (auto tries = 0; tries < 20; tries++) {
    auto alt_target = middle + bpoint(PCG_RANDOM_RANGE_INCLUSIVE(-5, 5), PCG_RANDOM_RANGE_INCLUSIVE(-5, 5));
    if (is_oob(alt_target)) {
      continue;
    }

    THING_DBG(g, v, l, me, "try alt hearing target %d,%d", alt_target.x, alt_target.y);
    if (thing_vision_can_hear_tile(g, v, l, me, alt_target)) {
      THING_DBG(g, v, l, me, "choose target: can hear something near the player");
      if (thing_monst_apply_target_path_if_possible(g, v, l, me, alt_target)) {
        return true;
      }
    }
  }

  return false;
}

//
// Can we chase the player?
//
static auto thing_monst_choose_target_player(Gamep g, Levelsp v, Levelp l, Thingp me) -> bool
{
  TRACE();

  auto *player = thing_player(g);
  if (player == nullptr) [[unlikely]] {
    return false;
  }

  if (compiler_unused) {
    thing_can_see_dump(g, v, l, me);
  }

  auto *player_level = game_level_get(g, v, player->level_num);
  auto *monst_level  = game_level_get(g, v, me->level_num);
  if (player_level != monst_level) {
    THING_DBG(g, v, l, me, "choose target: different level from player");
    return false;
  }

  //
  // Can the monst hear this tile?
  //
  auto at       = thing_at(g, v, l, me);
  auto target   = thing_at(g, v, l, player);
  auto can_hear = thing_vision_can_hear_tile(g, v, l, me, target);

  //
  // Can the monst see this tile?
  //
  if (! thing_vision_can_see_tile(g, v, l, me, target)) {
    THING_DBG(g, v, l, me, "choose target: cannot see player");

    //
    // No. Can it hear something?
    //
    if (can_hear) {
      THING_DBG(g, v, l, me, "choose target: cannot see, but can hear player");
    } else if (thing_monst_can_hear_player_nearby(g, v, l, me)) {
      THING_DBG(g, v, l, me, "choose target: cannot see, but can hear player nearby");
      return true;
    } else {
      THING_DBG(g, v, l, me, "choose target: cannot see and cannot hear player");
      return false;
    }
  }

  //
  // We can technically see the player. However, they might be hidden.
  //
  THING_DBG(g, v, l, me, "choose target: can see player");

  //
  // Invisible
  //
  if (thing_is_invisible(g, v, l, player)) {
    //
    // Can the monst see regardless?
    //
    if (! thing_is_able_to_see_invisible(me)) {
      //
      // But silent?
      //
      if (can_hear) {
        THING_DBG(g, v, l, me, "choose target: cannot see invisible, but can hear player");
      } else if (thing_monst_can_hear_player_nearby(g, v, l, me)) {
        THING_DBG(g, v, l, me, "choose target: cannot see invisible, but can hear player nearby");
        return true;
      } else {
        THING_DBG(g, v, l, me, "choose target: cannot see invisible and cannot hear player");

        if (adjacent(me->attacked_at, at)) {
          if (level_is_player_bool(g, v, l, me->attacked_at)) {
            THING_DBG(g, v, l, me, "choose target: cannot see invisible but player is still here");
          } else {
            THING_DBG(g, v, l, me, "choose target: cannot see invisible and player is gone");
            return false;
          }
        } else {
          return false;
        }
      }
    }
  }

  return thing_monst_apply_target_path_if_possible(g, v, l, me, target);
}

//
// If not the player, is there something we like
//
static auto thing_monst_choose_best_target(Gamep g, Levelsp v, Levelp l, Thingp me) -> bool
{
  THING_DBG(g, v, l, me, "choose target: best");
  TRACE_INDENT();

  auto at = thing_at(g, v, l, me);

  auto *ext = thing_ext_struct(g, v, me);
  if (ext == nullptr) [[unlikely]] {
    thing_err(g, v, l, me, "no ext pointer");
    return false;
  }

  if (! thing_is_able_to_choose_targets(me)) {
    return false;
  }

  bpoint best_target;
  float  best_score = 0;
  float  score      = 0;

  for (auto y = 0; y < MAP_HEIGHT; y++) {
    for (auto x = 0; x < MAP_WIDTH; x++) {
      bpoint const target(x, y);

      if (static_cast< uint32_t >(fov_map_get(&ext->can_see, x, y)) != 0U) {
        score = 2;
      } else if (static_cast< uint32_t >(age_map_get(&ext->has_seen, x, y)) != 0U) {
        score = 1;
      } else {
        continue;
      }

      bool skip {};
      switch (thing_assess_tile(g, v, l, target, me)) {
        case THING_ENVIRON_HATES :    skip = true; break;
        case THING_ENVIRON_DISLIKES : skip = true; break;
        case THING_ENVIRON_NEUTRAL :  skip = true; break;
        case THING_ENVIRON_LIKES :    score *= 100; break;
        case THING_ENVIRON_ENUM_MAX : break;
      }

      if (skip) {
        continue;
      }

      auto p = astar_solve(g, v, l, me, at, target);
      if (p.empty()) {
        continue;
      }

      if (! thing_move_path_apply(g, v, l, me, p)) {
        continue;
      }

      score -= distance(at, target);

      if (compiler_unused) {
        THING_DBG(g, v, l, me, "consider target: (%d,%d) score %f", target.x, target.y, best_score);
      }

      if (score > best_score) {
        best_score  = score;
        best_target = target;
        thing_monst_target_set(g, v, l, me, target);
      }
    }
  }

  if (best_score > 0) {
    thing_monst_target_set(g, v, l, me, best_target);
    THING_DBG(g, v, l, me, "choose target: best (%d,%d) score %f", best_target.x, best_target.y, best_score);
    return true;
  }

  return false;
}

//
// Choose somewhere random that we can see
//
static auto thing_monst_choose_something_we_can_wander_to(Gamep g, Levelsp v, Levelp l, Thingp me) -> bool
{
  THING_DBG(g, v, l, me, "choose target: something we can wander to");
  TRACE_INDENT();

  auto at = thing_at(g, v, l, me);

  auto *ext = thing_ext_struct(g, v, me);
  if (ext == nullptr) [[unlikely]] {
    thing_err(g, v, l, me, "no ext pointer");
    return false;
  }

  auto *mob = thing_minion_mob_get(g, v, l, me);

  //
  // How far to look for a target? We want to look beyond normal vision
  // when wandering as we want to explore uncharted areas
  //
  int radius = 0;
  if (mob == nullptr) {
    radius = thing_distance_vision(g, v, l, me) * 2;
  } else {
    radius = thing_distance_minion_from_mob_max(g, v, l, me);
  }

  THING_DBG(g, v, l, me, "choose target: radius: %d", radius);
  TRACE_INDENT();

  if (radius == 0) {
    thing_err(g, v, l, me, "unexpected value for radius");
    return false;
  }

  auto diameter  = radius * 2;
  auto tries     = 0;
  auto max_tries = 100;

  int  best_lowest_score = 999999;
  bool found_path        = false;

  //
  // Could avoid memsetting this by using uninitialized memory but
  // it means the tests are not deterministic
  //
  bool tried[ MAP_WIDTH ][ MAP_HEIGHT ] = {{0}};

  //
  // Keep trying to find a target
  //
  while (tries++ < max_tries) {
    bpoint target;

    if (tries == max_tries / 2) {
      radius /= 2;
      if (radius == 0) {
        radius = 1;
      }
    }

    //
    // Get a valid tile.
    //
    target.x = static_cast< int >(at.x) - radius + PCG_RANDOM_RANGE(0, diameter);
    target.y = static_cast< int >(at.y) - radius + PCG_RANDOM_RANGE(0, diameter);

    if (is_oob_or_border(target)) {
      continue;
    }

    THING_DBG(g, v, l, me, "choose target: try: (%d,%d)", target.x, target.y);

    if (tried[ target.x ][ target.y ]) {
      continue;
    }
    tried[ target.x ][ target.y ] = true;

    //
    // Check this is a tile we want to potentially walk to.
    // Just because we can see it, doesn't mean it's somewhere we want to go.
    //
    if (! thing_can_move_to_ai(g, v, l, me, target)) {
      continue;
    }

    //
    // If we get here for a minion, make sure the minion stays close to the mob
    //
    if (mob != nullptr) {
      if (distance(target, thing_at(g, v, l, mob)) >= thing_distance_minion_from_mob_max(g, v, l, me)) {
        continue;
      }
    }

    //
    // Prefer older tiles, so the monster explores more
    //
    int score = age_map_get(&ext->has_seen, target.x, target.y) * 100;

    //
    // Prefer further away tiles for exploration
    //
    score += 100 - static_cast< int >(distance(target, at));

    if (score >= best_lowest_score) {
      continue;
    }

    auto p = astar_solve(g, v, l, me, at, target);
    if (p.empty()) {
      continue;
    }

    if (! thing_move_path_apply(g, v, l, me, p)) {
      continue;
    }

    if (compiler_unused) {
      THING_DBG(g, v, l, me, "best %d,%d score %d", target.x, target.y, score);
    }

    thing_monst_target_set(g, v, l, me, target);
    best_lowest_score = score;
    found_path        = true;
    break;
  }

  return found_path;
}

//
// Return true on a successful move (or a popup asking more info)
//
[[nodiscard]] static auto thing_monst_move_try(Gamep g, Levelsp v, Levelp l, Thingp me, bpoint target, bpoint to) -> bool
{
  THING_DBG(g, v, l, me, "move try");
  TRACE_INDENT();

  auto at = thing_at(g, v, l, me);

  //
  // Check if enguilfed
  //
  if (thing_is_engulfed(me)) {
    //
    // Give a chance of escape
    //
    THING_DBG(g, v, l, me, "move try: not possible, engulfed, lunge");
    if (thing_stat_success(g, v, l, me, THING_STAT_LUCK, TARGET_ROLL_ENGULFED)) {
      (void) thing_is_engulfed_try_unset(g, v, l, me);
    } else {
      (void) thing_lunge(g, v, l, me, to);
      return false;
    }
  }

  //
  // Check if ensnared
  //
  if (thing_is_ensnared(me)) {
    //
    // Give a chance of escape
    //
    THING_DBG(g, v, l, me, "move try: not possible, ensnared, lunge");
    if (thing_stat_success(g, v, l, me, THING_STAT_DEX, TARGET_ROLL_ENSNARED)) {
      (void) thing_is_ensnared_try_unset(g, v, l, me);
    } else {
      (void) thing_lunge(g, v, l, me, to);
      return false;
    }
  }

  //
  // This is for spiders that do not move conventionally, but jump
  //
  if (! thing_is_able_to_move(me) || thing_is_able_to_jump_attack(me)) {
    THING_DBG(g, v, l, me, "move try: cannot move, jump?");
    if (! adjacent(at, to)) {
      if (thing_jump_to(g, v, l, me, to, false)) {
        return true;
      }
    }

    //
    // Try to reach the final target
    //
    if (! adjacent(at, target)) {
      if (thing_jump_to(g, v, l, me, target, false)) {
        return true;
      }
    }

    //
    // Jump half way?
    //
    auto middle = (target + to) / 2;
    if (! adjacent(at, middle)) {
      if (thing_jump_to(g, v, l, me, middle, false)) {
        return true;
      }
    }

    //
    // Try some alternative jump targets
    //
    for (auto tries = 0; tries < 10; tries++) {
      auto alt_target = middle + bpoint(PCG_RANDOM_RANGE_INCLUSIVE(-2, 2), PCG_RANDOM_RANGE_INCLUSIVE(-2, 2));
      if (! adjacent(at, alt_target)) {
        if (thing_jump_to(g, v, l, me, alt_target, false)) {
          return true;
        }
      }
    }

    //
    // Gave up
    //
    (void) thing_lunge(g, v, l, me, to);
    return false;
  }

  if (thing_can_move_to_attempt(g, v, l, me, to)) {
    TRACE_INDENT();

    THING_DBG(g, v, l, me, "move try: successfully moved");
    return true;
  }

  if (thing_can_move_to_attempt_by_shoving(g, v, l, me, to)) {
    //
    // Can we shove it out of the way to move?
    //
    THING_DBG(g, v, l, me, "move try: can move to by shoving");
    TRACE_INDENT();

    if (thing_shove_to(g, v, l, me, to)) {
      //
      // Do not step onto the thing we just shoved.
      //
      return level_is_dead_on_shoving(g, v, l, to) == nullptr;
    }
  }

  if (thing_can_move_to_attempt_by_opening(g, v, l, me, to)) {
    //
    // Can we open it allow movement?
    //
    THING_DBG(g, v, l, me, "move try: can move to by opening");
    TRACE_INDENT();

    if (thing_move_to(g, v, l, me, to)) {
      return true;
    }
  }

  if (thing_can_move_to_attempt_by_engulfing(g, v, l, me, to)) {
    //
    // Can we open it allow movement?
    //
    THING_DBG(g, v, l, me, "move try: can move to by engulfing");
    TRACE_INDENT();

    if (thing_move_to(g, v, l, me, to)) {
      return true;
    }
  }

  //
  // Can it jump there?
  //
  if (! adjacent(at, to)) {
    if (thing_jump_to(g, v, l, me, to, false)) {
      return true;
    }
  }

  //
  // Bumped into obstacle
  //
  THING_DBG(g, v, l, me, "move try: failed");
  (void) thing_lunge(g, v, l, me, to);
  return false;
}

[[nodiscard]] static auto thing_monst_attack_at(Gamep g, Levelsp v, Levelp l, Thingp me, const bpoint &target) -> bool
{
  auto at = thing_at(g, v, l, me);

  if (thing_is_able_to_fire_weapons(me)) {
    // ok
  } else if (adjacent(at, target)) {
    // ok
  } else if ((at == target)) {
    // ok
  } else {
    return false;
  }

  THING_DBG(g, v, l, me, "monst attack at %d,%d", target.x, target.y);
  TRACE_INDENT();

  if (thing_is_flesh_eater(me)) {
    if (level_alive_is_flesh(g, v, l, target) != nullptr) {
      THING_DBG(g, v, l, me, "monst: try attack on flesh");
      if (thing_attack_at(g, v, l, me, target)) {
        THING_DBG(g, v, l, me, "monst: attack success");
        return true;
      }
      THING_DBG(g, v, l, me, "monst: attack missed");
      return true; // still success of a sort
    }
  }

  if (level_is_attackable_by_monst_bool(g, v, l, target)) {
    if (thing_attack_at(g, v, l, me, target)) {
      THING_DBG(g, v, l, me, "monst: attack success");
      return true;
    }
    THING_DBG(g, v, l, me, "monst: attack missed");
    return true; // still success of a sort
  }

  THING_DBG(g, v, l, me, "nothing to attack");

  return false;
}

[[nodiscard]] static auto thing_monst_attack(Gamep g, Levelsp v, Levelp l, Thingp me) -> bool
{
  THING_DBG(g, v, l, me, "monst attack");
  TRACE_INDENT();

  auto target = thing_monst_target(me);
  return thing_monst_attack_at(g, v, l, me, target);
}

//
// Move to the next path on the popped path if it exits.
//
[[nodiscard]] static auto thing_monst_move_to_next(Gamep g, Levelsp v, Levelp l, Thingp me) -> bool
{
  TRACE();

  auto target = thing_monst_target(me);
  THING_DBG(g, v, l, me, "move to next, target (%d,%d)", target.x, target.y);
  TRACE_INDENT();

  auto at = thing_at(g, v, l, me);

  //
  // If already moving, do not pop the next path tile
  //
  if (thing_is_moving(me)) {
    THING_DBG(g, v, l, me, "move to next: already moving");
    return false;
  }

  //
  // Get the next tile to move to
  //
  bpoint nexthop = {};

  if (! thing_move_path_pop(g, v, l, me, nexthop)) {
    //
    // If could not pop, then no path is left
    //
    THING_DBG(g, v, l, me, "move to next: no move path to pop");
    return false;
  }

  THING_DBG(g, v, l, me, "move to next: nexthop (%d,%d)", nexthop.x, nexthop.y);
  TRACE_INDENT();

  //
  // If we have remaining moves and it is not possible to move to the next tile
  // then we pop moves off of the path to see if we can reach any of them
  //
  if (thing_move_path_size(g, v, l, me) != 0) {
    if (thing_can_move_to_ai(g, v, l, me, nexthop)) {
      THING_DBG(g, v, l, me, "nexthop allowed by ai");
    } else {
      THING_DBG(g, v, l, me, "nexthop blocked by ai (remaining path size %d)", thing_move_path_size(g, v, l, me));
      TRACE_INDENT();

      while (thing_move_path_pop(g, v, l, me, nexthop)) {
        THING_DBG(g, v, l, me, "pop nexthop (%d,%d)", nexthop.x, nexthop.y);
        TRACE_INDENT();

        //
        // Need to check the truncated path isn't something we don't like, like lava
        //
        if (thing_can_move_to_ai(g, v, l, me, nexthop)) {
          THING_DBG(g, v, l, me, "nexthop (%d,%d) allowed by ai", nexthop.x, nexthop.y);
          TRACE_INDENT();

          if (adjacent(at, nexthop)) {
            if (thing_move_to(g, v, l, me, nexthop)) {
              THING_DBG(g, v, l, me, "moved to nexthop");
              return false;
            }
          } else {
            if (thing_jump_to(g, v, l, me, nexthop)) {
              //
              // If could jump, then abort the path walk
              //
              if (thing_is_able_to_jump_land_then_pounce(g, v, l, me)) {
                THING_DBG(g, v, l, me, "jumped to nexthop; pounce?");
                return false;
              }

              THING_DBG(g, v, l, me, "jumped to nexthop; stop");
              monst_state_change(g, v, l, me, MONST_STATE_NORMAL);
              return true;
            }
          }
        } else {
          THING_DBG(g, v, l, me, "nexthop (%d,%d) blocked by ai", nexthop.x, nexthop.y);
        }
      }

      //
      // Something was in the way of jumping. Best to stop rather than accidentally
      // walk into a chasm.
      //
      THING_DBG(g, v, l, me, "move to next: not possible, lunge");
      TRACE_INDENT();

      (void) thing_lunge(g, v, l, me, nexthop);
      return false;
    }
  }

  if (! thing_monst_move_try(g, v, l, me, target, nexthop)) {
    //
    // If could not move, then abort the path walk
    //
    if (level_is_invisible_bool(g, v, l, nexthop, me)) {
      THING_DBG(g, v, l, me, "move to next: could not move due to invisible thing");
      TRACE_INDENT();

      if (thing_monst_attack_at(g, v, l, me, nexthop)) {
        THING_DBG(g, v, l, me, "move to next: could not move due to invisible thing, attacked it");
        TRACE_INDENT();
        thing_monst_target_set(g, v, l, me, nexthop);
        return true;
      }
    }

    THING_DBG(g, v, l, me, "move to next: could not move");
    return false;
  }

  //
  // If we're mid jump already, stop
  //
  if (thing_is_jumping(me)) {
    THING_DBG(g, v, l, me, "move to next: jumping already");
    return true;
  }

  if (! thing_is_able_to_move(me)) {
    THING_DBG(g, v, l, me, "move to next: unable to move");
    return true;
  }

  if (! thing_is_levitating(g, v, l, me)) {
    if (thing_is_able_to_jump(g, v, l, me)) {
      //
      // Jump over chasm?
      //
      if (level_is_chasm_bool(g, v, l, nexthop)) {
        return thing_jump_to(g, v, l, me, target);
      }

      //
      // Jump over lava or something else we hate?
      //
      switch (thing_assess_tile(g, v, l, nexthop, me)) {
        case THING_ENVIRON_HATES :    return thing_jump_to(g, v, l, me, target);
        case THING_ENVIRON_DISLIKES : break;
        case THING_ENVIRON_NEUTRAL :  break;
        case THING_ENVIRON_LIKES :    break;
        case THING_ENVIRON_ENUM_MAX : break;
      }
    }
  }

  return thing_move_to(g, v, l, me, nexthop);
}

[[nodiscard]] static auto thing_monst_choose_target(Gamep g, Levelsp v, Levelp l, Thingp me) -> bool
{
  THING_DBG(g, v, l, me, "choose target");
  TRACE_INDENT();

  if (thing_is_jumping(me)) {
    THING_DBG(g, v, l, me, "choose target: wait for jump to complete");
    return true;
  }

  if (thing_monst_over_target_player(g, v, l, me)) {
    THING_DBG(g, v, l, me, "choose target: over player");
    monst_state_change(g, v, l, me, MONST_STATE_CHASING);
    return true;
  }

  if (thing_monst_choose_target_player(g, v, l, me)) {
    THING_DBG(g, v, l, me, "choose target: found player");
    monst_state_change(g, v, l, me, MONST_STATE_CHASING);
    return true;
  }

  if (thing_monst_choose_best_target(g, v, l, me)) {
    THING_DBG(g, v, l, me, "choose target: found best target");
    monst_state_change(g, v, l, me, MONST_STATE_CHASING);
    return true;
  }

  if (thing_is_minion(me)) {
    THING_DBG(g, v, l, me, "choose target: one near mob?");
    TRACE_INDENT();
    if (thing_minion_choose_target_near_mob(g, v, l, me)) {
      THING_DBG(g, v, l, me, "choose target: minion found target near mob");
      monst_state_change(g, v, l, me, MONST_STATE_WANDER);
      return true;
    }
  }

  THING_DBG(g, v, l, me, "choose target: one we can see?");
  if (thing_monst_choose_something_we_can_wander_to(g, v, l, me)) {
    TRACE_INDENT();
    THING_DBG(g, v, l, me, "choose target: monst found a target it can see");
    monst_state_change(g, v, l, me, MONST_STATE_WANDER);
    return true;
  }

  THING_DBG(g, v, l, me, "choose target: none");

  return false;
}

//
// Called multiple times per tick potentially. If the monster has enough
// move_remaining then it can move or attack again.
//
void thing_monst_event_loop(Gamep g, Levelsp v, Levelp l, Thingp me)
{
  THING_DBG(g, v, l, me, "monst event loop, move_rem %d", thing_move_remaining(me));
  TRACE_INDENT();

  auto *player = thing_player(g);
  if (player == nullptr) [[unlikely]] {
    return;
  }

  const int player_speed = thing_speed(g, v, l, player);
  auto      rem          = thing_move_remaining(me);
  auto      at           = thing_at(g, v, l, me);

  //
  // If we have run out of moves, stop
  //
  if (rem < player_speed) {
    THING_DBG(g, v, l, me, "no more moves this tick, move_rem %d", thing_move_remaining(me));

    //
    // But can we attack even if we cannot move?
    //
    if (rem > 0) {
      if (tp_attack_count_max_per_tick_get(thing_tp(me)) > 1) {
        if (thing_monst_attack(g, v, l, me)) {
          THING_DBG(g, v, l, me, "in between move: attack");
          (void) thing_move_remaining_set(g, v, l, me, 0);
        }
      }
    }

    return;
  }

  (void) thing_move_remaining_decr(g, v, l, me, player_speed);
  THING_DBG(g, v, l, me, "move_rem %d", thing_move_remaining(me));

  if (compiler_unused) {
    thing_can_see_dump(g, v, l, me);
  }

  //
  // Early state check
  //
  switch (monst_state(g, v, l, me)) {
    case MONST_STATE_INIT : //
      monst_state_change(g, v, l, me, MONST_STATE_NORMAL);
      [[fallthrough]];
    case MONST_STATE_NORMAL : //
      if (thing_monst_attack(g, v, l, me)) {
        return;
      }

      (void) thing_monst_choose_target(g, v, l, me);
      break;
    case MONST_STATE_DEAD :
      //
      break;
    case MONST_STATE_CHASING :
      //
      // Might need to rething the target if it has moved. If we can no longer
      // see the player, keep going as it might have moved and we could be able
      // to see them again.
      //
      if (! thing_monst_choose_target_player(g, v, l, me)) {
        THING_DBG(g, v, l, me, "lost target, but keep going");
      }
      break;
    case MONST_STATE_WANDER :
      //
      // While wandering, if we see the player, change direction, but keep wandering
      // to the original target if we can't reach them
      //
      if (thing_monst_choose_target_player(g, v, l, me)) {
        THING_DBG(g, v, l, me, "sighted player");
        monst_state_change(g, v, l, me, MONST_STATE_CHASING);
      }
      break;
    case MONST_STATE_ENUM_MAX : break;
  }

  //
  // Post thinking state check
  //
  auto old_target = thing_monst_target(me);
  switch (monst_state(g, v, l, me)) {
    case MONST_STATE_INIT : //
      break;
    case MONST_STATE_NORMAL : //
      break;
    case MONST_STATE_DEAD :
      //
      break;
    case MONST_STATE_CHASING : [[fallthrough]];
    case MONST_STATE_WANDER :
      //
      // If we are unable to move, or we reach the target, move back to normal state so we can decide what to do.
      //
      if (! thing_monst_move_to_next(g, v, l, me)) {
        THING_DBG(g, v, l, me, "end of move, old target (%d,%d)", old_target.x, old_target.y);

        monst_state_change(g, v, l, me, MONST_STATE_NORMAL);

        //
        // To avoid one move of sitting idle, can we choose a new target and keep on moving?
        //
        at = thing_at(g, v, l, me);
        if ((at == old_target) || adjacent(at, old_target)) {
          //
          // We're probably lunging at the player right now. No need to try to move again.
          //
          THING_DBG(g, v, l, me, "end of move: adjacent to target");
          TRACE_INDENT();

          //
          // Can we engulf or eat the target?
          //
          if (thing_can_move_to_attempt_by_engulfing(g, v, l, me, old_target)) {
            THING_DBG(g, v, l, me, "move try: can engulf adjacent");
            TRACE_INDENT();

            if (thing_move_to(g, v, l, me, old_target)) {
              break;
            }
          }

          //
          // Can we attack here?
          //
          if (thing_monst_attack_at(g, v, l, me, old_target)) {
            THING_DBG(g, v, l, me, "end of move: attack");
            break;
          }
          break;
        }

        //
        // Did we get a new target?
        //
        if (! thing_monst_choose_target(g, v, l, me)) {
          break;
        }

        auto new_target = thing_monst_target(me);
        if (old_target == new_target) {
          //
          // If it is the same target, do not try to fail to move to the same tile again.
          //
          monst_state_change(g, v, l, me, MONST_STATE_NORMAL);

          //
          // Can we attack here?
          //
          if (thing_monst_attack_at(g, v, l, me, new_target)) {
            THING_DBG(g, v, l, me, "end of move: same target as before, attacked");
            break;
          }

          THING_DBG(g, v, l, me, "end of move: same target as before, do not continue");
          break;
        }

        THING_DBG(g, v, l, me, "end of move: have a new target (%d,%d)", new_target.x, new_target.y);
        (void) thing_monst_move_to_next(g, v, l, me);
        THING_DBG(g, v, l, me, "end of move: done");
      }
      break;
    case MONST_STATE_ENUM_MAX : break;
  }
}

[[nodiscard]] auto monst_state_to_string(MonstState state) -> std::string
{
  TRACE();
  return MonstState_to_string(state);
}

[[nodiscard]] auto monst_state(Gamep g, Levelsp v, Levelp l, Thingp me) -> MonstState
{
  TRACE();
  return me->_monst_state;
}

void monst_state_change(Gamep g, Levelsp v, Levelp l, Thingp me, MonstState new_state)
{
  TRACE();

  auto old_state = me->_monst_state;

  switch (old_state) {
    case MONST_STATE_INIT :
      //
      break;
    case MONST_STATE_DEAD :
      //
      return;
    case MONST_STATE_NORMAL :
      //
      break;
    case MONST_STATE_CHASING :
      //
      break;
    case MONST_STATE_WANDER :
      //
      break;
    case MONST_STATE_ENUM_MAX : break;
  }

  if (me->_monst_state == new_state) {
    IF_DEBUG { THING_DBG(g, v, l, me, "same state: %s", monst_state_to_string(new_state).c_str()); }
    return;
  }

  //
  // Set here to stop recursion.
  //
  me->_monst_state = new_state;

  //
  // Why oh why change state
  //
  IF_DEBUG
  {
    THING_DBG(g, v, l, me, "state change: %s -> %s", monst_state_to_string(old_state).c_str(), monst_state_to_string(new_state).c_str());
  }

  switch (new_state) {
    case MONST_STATE_INIT :
      //
      break;
    case MONST_STATE_DEAD :
      //
      break;
    case MONST_STATE_NORMAL :
      //
      break;
    case MONST_STATE_CHASING :
      //
      break;
    case MONST_STATE_WANDER :
      //
      break;
    case MONST_STATE_ENUM_MAX : break;
  }
}

//
// Called at the beginning of each tick
//
void thing_monst_tick(Gamep g, Levelsp v, Levelp l, Thingp me)
{
  TRACE();

  THING_DBG(g, v, l, me, "monst tick");

  if (thing_is_dead(me)) {
    return;
  }

  if (! thing_is_monst(me)) {
    return;
  }

  if (! level_is_player_level(g, v, l)) {
    return;
  }

  //
  // Give the thing ability to move. This will be decremented each thing_monst_event_loop
  //
  (void) thing_move_remaining_incr(g, v, l, me, thing_speed(g, v, l, me));

  THING_DBG(g, v, l, me, "monst tick, move_rem %d dt %f", thing_move_remaining(me), (float) me->thing_dt);
  TRACE_INDENT();

  thing_monst_event_loop(g, v, l, me);
}

[[nodiscard]] auto thing_is_monst(Thingp me) -> bool
{
  TRACE_DEBUG();

  if (me == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(me), is_monst) != 0;
}

[[nodiscard]] auto thing_is_monst1(Thingp me) -> bool
{
  TRACE_DEBUG();

  if (me == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(me), is_monst1) != 0;
}

[[nodiscard]] auto thing_is_monst2(Thingp me) -> bool
{
  TRACE_DEBUG();

  if (me == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(me), is_monst2) != 0;
}

[[nodiscard]] auto thing_monst_target(Thingp me) -> bpoint
{
  TRACE_DEBUG();

  if (me == nullptr) {
    CROAK("no thing pointer");
  }

  return me->_monst_target;
}

void thing_monst_target_set(Gamep g, Levelsp v, Levelp l, Thingp me, const bpoint &val)
{
  TRACE_DEBUG();

  if (me == nullptr) {
    CROAK("no thing pointer");
  }

  me->_monst_target = val;

  THING_DBG(g, v, l, me, "target set: %d,%d", val.x, val.y);
}
