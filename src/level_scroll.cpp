//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_game.hpp"
#include "my_gl.hpp"
#include "my_globals.hpp"
#include "my_level.hpp"
#include "my_main.hpp"
#include "my_thing_inlines.hpp"
#include "my_time.hpp"

//
// We focus on the current level if on the level select screen.
// Failing that we focus on the player if playing a level.
//
static auto level_scroll_target(Gamep g, Levelsp v) -> Thingp
{
  TRACE();

  //
  // If on the select level, do we have a current level?
  //
  auto *l = game_level_get(g, v);
  if (level_is_level_select(g, v, l)) {
    Thingp target = thing_level_select(g);
    if (target != nullptr) {
      return target;
    }
  }

  return thing_player(g);
}

//
// Soft scroll to the player/level.
//
void level_scroll_to_focus(Gamep g, Levelsp v, Levelp l)
{
  TRACE();

  //
  // We focus on the current level if on the level select screen.
  // Failing that we focus on the player if playing a level.
  //
  Thingp target = level_scroll_target(g, v);
  if (target == nullptr) {
    return;
  }

  //
  // Get the size of the on screen map.
  //
  int w = 0;
  int h = 0;
  fbo_get_size(g, FBO_MAP_FG, w, h);

  //
  // Where are we as a percentage on that map.
  //
  int const    zoom = game_map_zoom_get(g);
  spoint const pix_at(thing_pix_at(target));
  float const  x = ((pix_at.x * zoom) - v->pixel_map_at.x) / static_cast< float >(w);
  float const  y = ((pix_at.y * zoom) - v->pixel_map_at.y) / static_cast< float >(h);

  if (v->scroll_speed == 0U) {
    v->scroll_speed = MAP_SCROLL_SPEED;
  }

  if (v->requested_forced_auto_scroll != 0U) {
    //
    // For a time period e.g. post teleport, we want to ignore mouse moves until the player is
    // centered once more.
    //
  } else if (! v->requested_auto_scroll) {
    //
    // If the player is scrolling the map via the mouse, do not auto scroll.
    //
    if (level_is_level_select(g, v, l)) {
      //
      // Allow mouse scrolling always
      //
      return;
    }
    if ((x < MAP_SCROLL_EDGE_OUTER) || (y < MAP_SCROLL_EDGE_OUTER) || (x > 1 - MAP_SCROLL_EDGE_OUTER) || (y > 1 - MAP_SCROLL_EDGE_OUTER)) {
      //
      // Unless the player has wandered off screen
      //
      switch (player_state(g, v)) {
        case PLAYER_STATE_INIT :
          //
          // Player not initialized yet
          // Ignore auto scroll
          //
          return;
        case PLAYER_STATE_DEAD :
          //
          // Player is dead.
          // Re-enable auto scroll
          //
          v->requested_auto_scroll = true;
          break;
        case PLAYER_STATE_NORMAL :
          //
          // Replace the mouse path
          // Ignore auto scroll
          //
          return;
        case PLAYER_STATE_PATH_REQUESTED :
          //
          // Player wants to start following or replace the current path.
          // Re-enable auto scroll
          //
          return;
        case PLAYER_STATE_MOVE_CONFIRM_REQUESTED :
          //
          // Wait for confirmation.
          // Re-enable auto scroll
          //
          return;
        case PLAYER_STATE_FOLLOWING_PATH :
          //
          // Already following a path, stick to it until completion.
          // Re-enable auto scroll
          //
          v->requested_auto_scroll = true;
          break;
        case PLAYER_STATE_ENUM_MAX : return;
      }
    } else {
      //
      // Ignore auto scroll
      //
      return;
    }
  }

  //
  // Did we scroll any pixels?
  //
  int dx = 0;
  int dy = 0;

  //
  // If too close to the edges, scroll.
  //
  // Scroll faster if really close to the edge.
  //
  auto scroll_inner = MAP_SCROLL_EDGE_INNER;
  auto scroll_outer = MAP_SCROLL_EDGE_OUTER;

  int max_pixel_scroll_inner = MAP_SCROLL_EDGE_INNER_PIXEL;
  int max_pixel_scroll_outer = MAP_SCROLL_EDGE_OUTER_PIXEL;

  //
  // Need to scroll faster when teleporting
  //
  if (v->requested_forced_auto_scroll != 0U) {
    max_pixel_scroll_inner = 10 * MAP_SCROLL_EDGE_INNER_PIXEL;
    max_pixel_scroll_outer = 10 * MAP_SCROLL_EDGE_OUTER_PIXEL;
  }

  if (x > 1.0 - scroll_outer) {
    dx = static_cast< int >((x - scroll_outer) * v->scroll_speed);
    dx = std::min(dx, max_pixel_scroll_outer);
    v->pixel_map_at.x += dx;
  } else if (x > 1.0 - scroll_inner) {
    dx = static_cast< int >((x - scroll_inner) * v->scroll_speed);
    dx = std::min(dx, max_pixel_scroll_inner);
    v->pixel_map_at.x += dx;
  }

  if (x < scroll_outer) {
    dy = static_cast< int >((scroll_outer - x) * v->scroll_speed);
    dy = std::min(dy, max_pixel_scroll_outer);
    v->pixel_map_at.x -= dy;
  } else if (x < scroll_inner) {
    dy = static_cast< int >((scroll_inner - x) * v->scroll_speed);
    dy = std::min(dy, max_pixel_scroll_inner);
    v->pixel_map_at.x -= dy;
  }

  if (y > 1.0 - scroll_outer) {
    dy = static_cast< int >((y - scroll_outer) * v->scroll_speed);
    dy = std::min(dy, max_pixel_scroll_outer);
    v->pixel_map_at.y += dy;
  } else if (y > 1.0 - scroll_inner) {
    dy = static_cast< int >((y - scroll_inner) * v->scroll_speed);
    dy = std::min(dy, max_pixel_scroll_inner);
    v->pixel_map_at.y += dy;
  }

  if (y < scroll_outer) {
    dx = static_cast< int >((scroll_outer - y) * v->scroll_speed);
    dx = std::min(dx, max_pixel_scroll_outer);
    v->pixel_map_at.y -= dx;
  } else if (y < scroll_inner) {
    dx = static_cast< int >((scroll_inner - y) * v->scroll_speed);
    dx = std::min(dx, max_pixel_scroll_inner);
    v->pixel_map_at.y -= dx;
  }

  //
  // Check for overflow
  //
  if (v->pixel_map_at.x > v->pixel_max.x) {
    dx                = 0;
    v->pixel_map_at.x = v->pixel_max.x;
  } else if (v->pixel_map_at.x < 0) {
    dx                = 0;
    v->pixel_map_at.x = 0;
  }

  if (v->pixel_map_at.y > v->pixel_max.y) {
    dy                = 0;
    v->pixel_map_at.y = v->pixel_max.y;
  } else if (v->pixel_map_at.y < 0) {
    dy                = 0;
    v->pixel_map_at.y = 0;
  }

  if (compiler_unused) {
    con("%u [%d,%d] elapsed %u", v->requested_forced_auto_scroll, dx, dy, time_ms() - v->requested_forced_auto_scroll);
  }

  //
  // Have we finished scrolling?
  //
  if ((dx == 0) && (dy == 0)) {
    if (v->requested_forced_auto_scroll != 0U) {
      if (time_have_x_tenths_passed_since(5, v->requested_forced_auto_scroll)) {
        v->requested_forced_auto_scroll = 0;
        v->scroll_speed                 = MAP_SCROLL_SPEED;
      }
    }
  } else if (v->requested_forced_auto_scroll != 0U) {
    if (time_have_x_secs_passed_since(1, v->requested_forced_auto_scroll)) {
      v->requested_forced_auto_scroll = 0;
      v->scroll_speed                 = MAP_SCROLL_SPEED;
    }
  }

  level_bounds_set(g, v, l);
}

//
// Scroll the map e.g. via mouse
//
void level_scroll_delta(Gamep g, Levelsp v, Levelp l, const spoint &delta)
{
  TRACE();

  v->pixel_map_at += delta;
  v->requested_auto_scroll = false;

  level_bounds_set(g, v, l);
}

//
// For a time period e.g. post teleport, we want to ignore mouse moves until the player is
// centered once more.
//
void level_forced_auto_scroll(Gamep g, Levelsp v, Levelp l)
{
  TRACE();

  v->requested_forced_auto_scroll = time_ms();

  level_bounds_set(g, v, l);
}

//
// Jump to the target immediately.
//
void level_scroll_warp_to_focus(Gamep g, Levelsp v, Levelp l)
{
  TRACE();

  VERIFY(MTYPE_LEVELS, v);

  //
  // We focus on the current level if on the level select screen.
  // Failing that we focus on the player if playing a level.
  //
  Thingp target = level_scroll_target(g, v);
  if (target == nullptr) {
    return;
  }

  int const zoom = game_map_zoom_get(g);

  v->pixel_map_at = thing_pix_at(target);
  v->pixel_map_at.x *= zoom;
  v->pixel_map_at.y *= zoom;

  v->pixel_map_at.x -= game_map_fbo_width_get(g) / 2;
  v->pixel_map_at.y -= game_map_fbo_height_get(g) / 2;

  //
  // Accomodate half the player tile size
  //
  auto *player = thing_player(g);
  if (player != nullptr) {
    spoint   tl;
    spoint   br;
    uint16_t tile_index = 0;
    thing_display_get_tile_info(g, v, l, thing_at(player), thing_tp(player), player, tl, br, &tile_index);
    if (tile_index != 0U) {
      if (br.x < tl.x) {
        std::swap(br.x, tl.x);
      }

      if (br.y < tl.y) {
        std::swap(br.y, tl.y);
      }

      v->pixel_map_at.x += (br.x - tl.x) / 2;
      v->pixel_map_at.y += (br.y - tl.y) / 2;
    }
  }

  level_bounds_set(g, v, l);
}
