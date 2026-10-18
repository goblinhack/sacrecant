//
// Copyright goblinhack@gmail.com
//

#include "my_bpoint.hpp"
#include "my_callstack.hpp"
#include "my_dmap.hpp"
#include "my_game_defs.hpp"
#include "my_level.hpp"
#include "my_main.hpp"
#include "my_thing.hpp"
#include "my_types.hpp"
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstring>

void level_hearing_gen(Gamep g, Levelsp v, Levelp l)
{
  TRACE();

  auto *player = thing_player(g);
  if (player == nullptr) [[unlikely]] {
    return;
  }

  //
  // If the player is not on the level being lit, then nothing to do
  //
  if (l->level_arr_idx != player->level_arr_idx) {
    return;
  }

  auto *dmap = &v->dmap_noise;
  memset(dmap, DMAP_IS_WALL, sizeof(*dmap));

  auto stealth = thing_noise(g, v, l, player);
  if (stealth == 0) {
    return;
  }

  auto   end = thing_at(g, v, l, player);
  bpoint dmap_start(end.x - stealth, end.y - stealth);
  bpoint dmap_end(end.x + stealth, end.y + stealth);

  dmap_start.x = std::max(static_cast< int8_t >(0), dmap_start.x);
  dmap_start.y = std::max(static_cast< int8_t >(0), dmap_start.y);
  dmap_end.x   = std::min(static_cast< int8_t >(MAP_WIDTH - 1), dmap_end.x);
  dmap_end.y   = std::min(static_cast< int8_t >(MAP_HEIGHT - 1), dmap_end.y);

  for (auto x = dmap_start.x; x < dmap_end.x; x++) {
    for (auto y = dmap_start.y; y < dmap_end.y; y++) {
      bpoint const p(x, y);
      if (! level_is_obs_to_hearing_bool(g, v, l, p)) {
        dmap->val[ x ][ y ] = DMAP_IS_PASSABLE;
      }
    }
  }

  dmap->val[ end.x ][ end.y ] = DMAP_IS_GOAL_REVERSE - stealth;

  if (compiler_unused) {
    log("stealth %d", stealth);
    log("dmap_start  %d,%d", dmap_start.x, dmap_start.y);
    log("dmap_end    %d,%d", dmap_end.x, dmap_end.y);
  }

  if (compiler_unused) {
    dmap_print(dmap, end, dmap_start, dmap_end);
  }

  dmap_process_reverse(dmap, dmap_start, dmap_end);

  if (compiler_unused) {
    dmap_print(dmap, end, dmap_start, dmap_end);
  }
}
