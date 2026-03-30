//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_dmap.hpp"
#include "my_globals.hpp"
#include "my_level.hpp"
#include "my_level_inlines.hpp"
#include "my_main.hpp"

#include <algorithm>

static std::vector< bpoint > cursor_path;

using PathCost = struct PathCost {
  std::vector< bpoint > path;
  int                   cost = {};
};

void level_cursor_set(Gamep g, Levelsp v, bpoint p)
{
  TRACE();

  //
  // Need to update what is under the cursor
  //
  v->cursor_moved  = (v->cursor_at != v->old_cursor_at);
  v->old_cursor_at = v->cursor_at;

  if (compiler_unused) {
    BOTCON("cursor %d,%d", v->cursor_at.x, v->cursor_at.y);
  }

  if (! is_oob(p)) {
    v->cursor_at       = p;
    v->cursor_at_valid = true;
  } else {
    v->cursor_at_valid = false;
  }
}

auto level_cursor_is_valid(Gamep g, Levelsp v) -> bool
{
  TRACE();

  return v->cursor_at_valid;
}

//
// Create the cursor path, avoiding things like lava
//
// For the 1st pass, restrict to tiles we have walked on
// For the 2nd pass, any tiles will do, but no hazards
// For the 3rd pass, any tiles will do as long as not consecutive hazard tiles.
// For the 4th pass, any tiles will do as long as not walls
//
static auto level_cursor_path_draw_line_attempt(Gamep g, Levelsp v, Levelp l, Thingp player, bpoint start, bpoint end, int attempt)
    -> std::vector< bpoint >
{
  TRACE();

  static std::vector< bpoint > const empty;

  bool   prev_tile_was_hazard = {};
  Dmap   dmap {};
  bpoint dmap_start = start;
  bpoint dmap_end   = end;

  int minx = 0;
  int miny = 0;
  int maxx = 0;
  int maxy = 0;
  if (dmap_start.x < dmap_end.x) {
    minx = dmap_start.x;
    maxx = dmap_end.x;
  } else {
    minx = dmap_end.x;
    maxx = dmap_start.x;
  }
  if (dmap_start.y < dmap_end.y) {
    miny = dmap_start.y;
    maxy = dmap_end.y;
  } else {
    miny = dmap_end.y;
    maxy = dmap_start.y;
  }

  auto border = game_tiles_visible_across_get(g);
  minx -= border;
  miny -= border;
  maxx += border;
  maxy += border;

  minx = std::max(minx, 0);
  miny = std::max(miny, 0);
  if (maxx >= MAP_WIDTH) {
    maxx = MAP_WIDTH - 1;
  }
  if (maxy >= MAP_HEIGHT) {
    maxy = MAP_HEIGHT - 1;
  }

  //
  // Seems better to use the entire map, as that way we can scroll and still see a path
  // even if the player is some way off.
  //
  minx = 0;
  miny = 0;
  maxx = MAP_WIDTH - 1;
  maxy = MAP_HEIGHT - 1;

  switch (attempt) {
    case 4 :
      //
      // For the 4th pass, any tiles will do as long as not walls
      //
      if (level_has_seen(g, v, l, end)) {
        for (auto y = miny; y < maxy; y++) {
          for (auto x = minx; x < maxx; x++) {
            bpoint const p(x, y);

            if (level_alive_is_obs_to_cursor_path(g, v, l, p) != nullptr) {
              dmap.val[ x ][ y ] = DMAP_IS_WALL;
            } else {
              dmap.val[ x ][ y ] = DMAP_IS_PASSABLE;
            }
          }
        }
      } else {
        //
        // However if this is a tile we've not seen, then don't just offer chasms
        // as a viable path just because we can't perhaps see the way through foliage!
        //
        for (auto y = miny; y < maxy; y++) {
          for (auto x = minx; x < maxx; x++) {
            bpoint const p(x, y);

            if ((level_alive_is_obs_to_cursor_path(g, v, l, p) != nullptr) || (level_alive_is_cursor_path_hazard(g, v, l, p) != nullptr)) {
              dmap.val[ x ][ y ] = DMAP_IS_WALL;
            } else {
              dmap.val[ x ][ y ] = DMAP_IS_PASSABLE;
            }
          }
        }
      }
      break;
    case 3 :
      //
      // For the 3rd pass, any tiles will do as long as not consecutive hazard tiles.
      //
      for (auto y = miny; y < maxy; y++) {
        for (auto x = minx; x < maxx; x++) {
          bpoint const p(x, y);

          //
          // Any tile will do as long as not consecutive hazard tiles.
          //
          if (prev_tile_was_hazard) {
            if ((level_alive_is_obs_to_cursor_path(g, v, l, p) != nullptr) || (level_alive_is_cursor_path_hazard(g, v, l, p) != nullptr)) {
              dmap.val[ x ][ y ] = DMAP_IS_WALL;
            } else {
              dmap.val[ x ][ y ] = DMAP_IS_PASSABLE;
            }
          } else {
            if (level_alive_is_obs_to_cursor_path(g, v, l, p) != nullptr) {
              dmap.val[ x ][ y ] = DMAP_IS_WALL;
            } else {
              dmap.val[ x ][ y ] = DMAP_IS_PASSABLE;
            }

            //
            // If a hazard, then don't let the next tile be one too
            //
            prev_tile_was_hazard = (level_is_cursor_path_hazard(g, v, l, p) != nullptr);
          }
        }
      }
      break;
    case 2 :
    case 1 :
      //
      // Common code for pass 1 and 2
      //
      if (level_is_cursor_path_hazard(g, v, l, thing_at(player)) != nullptr) {
        //
        // If standing on a hazard, then plot a course that allows travel over hazards.
        // Any path except through walls.
        //
        for (auto y = miny; y < maxy; y++) {
          for (auto x = minx; x < maxx; x++) {
            bpoint const p(x, y);

            if (level_alive_is_obs_to_cursor_path(g, v, l, p) != nullptr) {
              dmap.val[ x ][ y ] = DMAP_IS_WALL;
            } else {
              dmap.val[ x ][ y ] = DMAP_IS_PASSABLE;
            }
          }
        }
      } else if (level_is_cursor_path_hazard(g, v, l, v->cursor_at) != nullptr) {
        //
        // Here the cursor is over a hazard. Plot a course that allows travel via other hazards.
        //
        bool                                     got_one = false;
        std::initializer_list< ThingFlag > const init    = {is_lava, is_chasm, is_water};

        for (auto i : init) {
          if (level_flag(g, v, l, i, bpoint(v->cursor_at.x, v->cursor_at.y), player) != nullptr) {
            got_one = true;

            //
            // If the cursor is on a hazard then allow creating a path via hazards.
            //
            // However, be careful. If we click on lava, and the shortest path
            // to that lava is via a chasm, then we do not want to jump into the
            // chasm.
            //
            for (auto y = miny; y < maxy; y++) {
              for (auto x = minx; x < maxx; x++) {
                bpoint const p(x, y);

                //
                // But we still can't walk through walls to get to the hazard
                //
                if (level_alive_is_obs_to_cursor_path(g, v, l, p) != nullptr) {
                  dmap.val[ x ][ y ] = DMAP_IS_WALL;
                  continue;
                }

                if (level_is_cursor_path_hazard(g, v, l, p) != nullptr) {
                  if (level_flag(g, v, l, i, p, player) == nullptr) {
                    dmap.val[ x ][ y ] = DMAP_IS_WALL;
                    continue;
                  }
                  dmap.val[ x ][ y ] = DMAP_IS_PASSABLE;
                  continue;
                }

                dmap.val[ x ][ y ] = DMAP_IS_PASSABLE;
              }
            }
          }
        }

        //
        // There is a hazard on the cursor path. Default to the normal path to play safe
        // and avoid more hazards.
        //
        if (! got_one) {
          for (auto y = miny; y < maxy; y++) {
            for (auto x = minx; x < maxx; x++) {
              bpoint const p(x, y);

              if ((level_alive_is_obs_to_cursor_path(g, v, l, p) != nullptr) || (level_is_cursor_path_hazard(g, v, l, p) != nullptr)) {
                dmap.val[ x ][ y ] = DMAP_IS_WALL;
              } else {
                dmap.val[ x ][ y ] = DMAP_IS_PASSABLE;
              }
            }
          }
        }
      } else {
        //
        // Normal path. Avoid hazards.
        //
        for (auto y = miny; y < maxy; y++) {
          for (auto x = minx; x < maxx; x++) {
            bpoint const p(x, y);

            //
            // Avoid hazards
            //
            if ((level_is_obs_to_cursor_path(g, v, l, p) != nullptr) || (level_alive_is_cursor_path_hazard(g, v, l, p) != nullptr)) {
              dmap.val[ x ][ y ] = DMAP_IS_WALL;
            } else {
              dmap.val[ x ][ y ] = DMAP_IS_PASSABLE;
            }
          }
        }
      }
      break;
  }

  //
  // Prune the path. Skip tiles we've never seen. Limit to previously walked tiles in some cases.
  //
  for (auto y = miny; y < maxy; y++) {
    for (auto x = minx; x < maxx; x++) {
      bpoint const p(x, y);

      if (DEBUG) {
        //
        // In debug mode we can see everything and it looks odd if we can't walk to whatever point we see
        //
      } else {
        //
        // If we've NEVER seen this tile, skip it
        //
        if (! level_has_seen(g, v, l, p)) {
          dmap.val[ x ][ y ] = DMAP_IS_WALL;
          continue;
        }
      }

      //
      // Limit to previously walked tiles
      //
      if (attempt == 1) {
        if (l->player_has_walked_tile[ x ][ y ] == 0U) {
          dmap.val[ x ][ y ] = DMAP_IS_WALL;
          continue;
        }
      }
    }
  }

  dmap_start = bpoint(minx, miny);
  dmap_end   = bpoint(maxx, maxy);

  dmap.val[ end.x ][ end.y ]     = DMAP_IS_GOAL;
  dmap.val[ start.x ][ start.y ] = DMAP_IS_PASSABLE;

  if (compiler_unused) {
    dmap_print(&dmap, end, dmap_start, dmap_end);
  }
  dmap_process(&dmap, dmap_start, dmap_end);
  if (compiler_unused) {
    dmap_print(&dmap, end, dmap_start, dmap_end);
  }

  auto p         = dmap_solve(g, v, l, player, &dmap, start);
  auto path_size = p.size();
  if (path_size == 0U) {
    return empty;
  }

  //
  // If we could not reach the target, then the path will just plot the distance from the start, which is not what
  // we want.
  //
  if (p[ path_size - 1 ] != end) {
    if (compiler_unused) {
      LOG("did not reach %d,%d", end.x, end.y);
    }
    return empty;
  }

  if (compiler_unused) {
    TOPCON("attempt %d len %d", attempt, static_cast< int >(p.size()));
  }

  return p;
}

//
// Create a cursor path between the two points
//
// Returns true on success
//
static auto level_cursor_path_draw_line(Gamep g, Levelsp v, Levelp l, const bpoint &start, bpoint end) -> std::vector< bpoint >
{
  static std::vector< bpoint > const empty;

  auto *player = thing_player(g);
  if (player == nullptr) [[unlikely]] {
    return empty;
  }

  //
  // Choose the shortest path of each attempt.
  //
  // The first path prefers visited tiles.
  //
  const int max_attempts = 5;

  std::vector< PathCost > paths;

  for (auto attempt = 1; attempt < max_attempts; attempt++) {
    PathCost pc;
    pc.path = level_cursor_path_draw_line_attempt(g, v, l, player, start, end, attempt);
    if (pc.path.empty()) {
      continue;
    }
    pc.cost = thing_path_cost(g, v, l, player, pc.path);
    paths.push_back(pc);
  }

  if (paths.empty()) {
    return empty;
  }

  std::ranges::sort(paths, [](const PathCost &a, const PathCost &b) -> bool { return (a.cost < b.cost); });

  IF_DEBUG
  {
    auto idx = 0;
    for (const auto &pc : paths) {
      thing_log(player, "path[%d]: cost:%d len:%d", idx++, pc.cost, static_cast< int >(pc.path.size()));
    }
  }

  return paths[ 0 ].path;
}

//
// Stop following the current path
//
void level_cursor_path_reset(Gamep g, Levelsp v, Levelp l)
{
  auto *player = thing_player(g);
  if (player == nullptr) [[unlikely]] {
    //
    // If no player, clear the cursor
    //
    memset(v->cursor, 0, SIZEOF(v->cursor));
    return;
  }

  auto *ext_struct = thing_ext_struct(g, player);
  if (ext_struct == nullptr) {
    //
    // If no player, clear the cursor
    //
    memset(v->cursor, 0, SIZEOF(v->cursor));
    return;
  }

  player_state_change(g, v, l, PLAYER_STATE_NORMAL);
  thing_move_path_reset(g, v, l, player);
  memset(v->cursor, 0, SIZEOF(v->cursor));
}

//
// Stop following the current path
//
void level_cursor_path_reset(Gamep g)
{
  auto *v = game_levels_get(g);
  if (v == nullptr) {
    return;
  }

  auto *l = game_level_get(g, v);
  if (l == nullptr) {
    return;
  }

  level_cursor_path_reset(g, v, l);
}

//
// Copy the given path to the thing
//
void level_cursor_copy_path_to_player(Gamep g, Levelsp v, Levelp l, std::vector< bpoint > &move_path)
{
  auto *player = thing_player(g);
  if (player == nullptr) [[unlikely]] {
    //
    // If no player, clear the cursor
    //
    memset(v->cursor, 0, SIZEOF(v->cursor));
    return;
  }

  auto *ext_struct = thing_ext_struct(g, player);
  if (ext_struct == nullptr) {
    //
    // If no player, clear the cursor
    //
    memset(v->cursor, 0, SIZEOF(v->cursor));
    return;
  }

  switch (player_state(g, v)) {
    case PLAYER_STATE_INIT :
      //
      // Player not initialized yet
      //
      break;
    case PLAYER_STATE_DEAD :
      //
      // Player is dead.
      // Ok to replace the path so the player can look around when dead.
      //
      break;
    case PLAYER_STATE_NORMAL :
      //
      // Replace the mouse path
      //
      break;
    case PLAYER_STATE_PATH_REQUESTED :
      //
      // Player wants to start following or replace the current path.
      //
      player_state_change(g, v, l, PLAYER_STATE_FOLLOWING_PATH);
      break;
    case PLAYER_STATE_MOVE_CONFIRM_REQUESTED :
      //
      // Wait for confirmation.
      //
      return;
    case PLAYER_STATE_FOLLOWING_PATH :
      //
      // Already following a path, stick to it until completion.
      //
      return;
    case PLAYER_STATE_ENUM_MAX : return;
  }

  //
  // Copy the latest mouse path to the player
  //
  IF_DEBUG2
  { //
    THING_DBG(player, "apply cursor path size: %d", static_cast< int >(move_path.size()));
  }
  if (thing_move_path_apply(g, v, l, player, move_path)) {
    move_path.clear();
  }
}

//
// Recreate the mouse path
//
static void level_cursor_path_create(Gamep g, Levelsp v, Levelp l)
{
  auto *player = thing_player(g);
  if (player == nullptr) [[unlikely]] {
    //
    // If no player, clear the cursor
    //
    memset(v->cursor, 0, SIZEOF(v->cursor));
    return;
  }

  //
  // Only create the cursor path if the player is on this level. The level
  // select level for example, has no player.
  //
  if (player->level_num != l->level_num) {
    memset(v->cursor, 0, SIZEOF(v->cursor));

    //
    // Helpful to have a cursor when selecting levels
    //
    if (level_is_level_select(g, v, l)) {
      v->cursor[ v->cursor_at.x ][ v->cursor_at.y ] = CURSOR_AT;
    }

    return;
  }

  //
  // Clear the path.
  //
  memset(v->cursor, 0, SIZEOF(v->cursor));

  //
  // Draw the path
  //
  cursor_path = level_cursor_path_draw_line(g, v, l, thing_at(player), v->cursor_at);

  IF_DEBUG2
  { //
    THING_DBG(player, "cursor path size: %d", static_cast< int >(cursor_path.size()));
  }

  for (auto p : cursor_path) {
    v->cursor[ p.x ][ p.y ] = CURSOR_PATH;
    IF_DEBUG2
    { //
      THING_DBG(player, " - cursor path: %d,%d", p.x, p.y);
    }
  }
  v->cursor[ v->cursor_at.x ][ v->cursor_at.y ] = CURSOR_AT;
}

//
// Recreate the mouse path and what we see under it
//
void level_cursor_path_recreate(Gamep g, Levelsp v, Levelp l)
{
  //
  // Only if over the map
  //
  if (! level_cursor_is_valid(g, v)) {
    return;
  }

  //
  // Recreate the mouse path
  //
  level_cursor_path_create(g, v, l);

  //
  // If in level select mode, update what we're hovering over
  //
  level_select_mouse_motion(g, v, l);
}

//
// Apply the mouse path to the player
//
void level_cursor_copy_mouse_path_to_player(Gamep g, Levelsp v, Levelp l)
{
  //
  // Only if over the map
  //
  if (! level_cursor_is_valid(g, v)) {
    return;
  }

  //
  // Update the player with the path.
  //
  level_cursor_copy_path_to_player(g, v, l, cursor_path);

  //
  // If in level select mode, update what we're hovering over
  //
  level_select_mouse_motion(g, v, l);
}

auto level_cursor_path_size(Gamep g) -> int { return static_cast< int >(cursor_path.size()); }
