//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_gl.hpp"
#include "my_level.hpp"
#include "my_thing.hpp"
#include "my_types.hpp"

#include <cstring>

//
// All light from all light sources, combined.
//
void level_light_calculate_all(Gamep g, Levelsp v, Levelp l)
{
  TRACE();

  auto *player = thing_player(g);
  if (player == nullptr) [[unlikely]] {
    return;
  }

  if ((g == nullptr) || (v == nullptr) || (l == nullptr)) [[unlikely]] {
    return;
  }

  //
  // If the player is not on the level being lit, then nothing to do
  //
  if (l->level_arr_idx != player->level_arr_idx) {
    return;
  }

  //
  // Calculate player lighting while we wait
  //
  level_raycast(g, v, l, FBO_MAP_LIGHT_MASK);
}
