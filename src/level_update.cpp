//
// Copyright goblinhack@gmail.com
//

#include "my_bits.hpp"
#include "my_callstack.hpp"
#include "my_globals.hpp"
#include "my_level.hpp"
#include "my_level_inlines.hpp"
#include "my_main.hpp"
#include "my_random.hpp"
#include "my_tile.hpp"

#include <cstring>
#include <map>

void level_update_paths_set(Gamep g, Levelsp v, Levelp l, const bpoint &p)
{
  TRACE();

  l->request_to_update_paths = true;

  for (auto dx = -1; dx <= 1; dx++) {
    for (auto dy = -1; dy <= 1; dy++) {
      auto x = p.x + dx;
      auto y = p.y + dy;
      if (! is_oob(x, y)) {
        l->is_modified_tile[ x ][ y ] = 1U;
      }
    }
  }
}

//
// Update dmaps to things like mobs, player etc...
//

void level_update_paths(Gamep g, Levelsp v, Levelp l)
{
  TRACE();

  if (! l->request_to_update_paths) {
    return;
  }
  l->request_to_update_paths = false;

  level_update_tiles(g, v, l);
  level_dmap(g, v, l);

  memset(l->is_modified_tile, 0, SIZEOF(l->is_modified_tile));
}

//
// Update lighting and minimaps when things change substantially, like a teleport, or level change
//
void level_update_visibility(Gamep g, Levelsp v, Levelp l)
{
  TRACE();

  auto *player = thing_player(g);
  if (player == nullptr) [[unlikely]] {
    return;
  }

  if ((g == nullptr) || (v == nullptr) || (l == nullptr)) {
    return;
  }

  //
  // If the player is not on the level being lit, then nothing to do
  //
  if (l->level_num != player->level_num) {
    return;
  }

  //
  // Per tile visibility changes
  //
  bool update_per_pixel_visibility = false;

  if (l->tick_in_progress) {
    //
    // Limit the updates unless asked (due to a moving thing likely) as this path
    // is called many times per second.
    //
    if (l->request_to_update_per_pixel_visibility) {
      l->request_to_update_per_pixel_visibility = false;
      update_per_pixel_visibility               = true;
    }
  } else {
    //
    // Some event, like tick end or teleport occurred.
    //
    update_per_pixel_visibility = true;
  }

  if (update_per_pixel_visibility) {
    // topcon("P");
    //
    // We only care about pixel moves.
    //
    level_light_calculate_all(g, v, l);
  }

  //
  // Per pixel visibility changes
  //
  bool update_per_tile_visibility = false;

  if (l->tick_in_progress) {
    //
    // Limit the updates unless asked (due to a moving thing likely) as this path
    // is called many times per second.
    //
    if (l->request_to_update_per_tile_visibility) {
      l->request_to_update_per_tile_visibility = false;
      update_per_tile_visibility               = true;
    }
  } else {
    //
    // Some event, like tick end or teleport occurred.
    //
    update_per_tile_visibility = true;
  }

  if (update_per_tile_visibility) {
    //
    // Per tick check what the player can see.
    //
    level_has_seen_update(g, v, l);

    //
    // What can monsters see?
    //
    level_vision_calculate_all(g, v, l);

    //
    // Update minimaps
    //
    level_minimaps_update(g, v, l);
  }
}
