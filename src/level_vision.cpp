//
// Copyright goblinhack@gmail.com
//

#include "my_bpoint.hpp"
#include "my_callstack.hpp"
#include "my_level.hpp"
#include "my_level_inlines.hpp" // NOLINT
#include "my_thing.hpp"
#include "my_thing_inlines.hpp" // NOLINT
#include "my_types.hpp"

#include <thread>
#include <vector>

//
// What can monsters see?
//
void level_vision_calculate_all(Gamep g, Levelsp v, Levelp l)
{
  TRACE();

  if (((g == nullptr) || (v == nullptr) || (l == nullptr))) [[unlikely]] {
    return;
  }

  auto *player = thing_player(g);
  if (player == nullptr) [[unlikely]] {
    return;
  }

  //
  // If the player is not on the level being lit, then nothing to do
  //
  if (l->level_num != player->level_num) {
    return;
  }

  std::vector< std::thread > threads;

  //
  // Calculate all lit tiles for non player things
  //
  FOR_ALL_THINGS_ON_LEVEL_NO_BREAK_UNSAFE(g, v, l, t)
  {
    auto max_radius = thing_distance_vision(g, v, l, t);
    if (max_radius == 0) {
      continue;
    }

    if ((thing_is_player(t))) [[unlikely]] {
      continue;
    }

    if (g_opt_debug2) {
      thing_vision_calculate(g, v, l, t);
    } else {
      threads.emplace_back(thing_vision_calculate, g, v, l, t);
    }
  }

  //
  // Wait on threads
  //
  for (auto &i : threads) {
    i.join();
  }
}

//
// Something blocking the fov?
//
[[nodiscard]] auto level_vision_blocker_at(Gamep g, Levelsp v, Levelp l, Thingp me, const bpoint &at) -> bool
{
  FOR_ALL_THINGS_AT_UNSAFE(g, v, l, it, at)
  {
    if (thing_vision_blocker(g, v, l, me, it)) {
      return true;
    }
  }
  return false;
}
