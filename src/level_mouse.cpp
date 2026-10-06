//
// Copyright goblinhack@gmail.com
//

#include "my_bpoint.hpp"
#include "my_callstack.hpp"
#include "my_game.hpp"
#include "my_level.hpp"
#include "my_sdl_event.hpp"
#include "my_tile.hpp"
#include "my_types.hpp"

//
// Get the mouse position, so when displaying tiles we can work out where the cursor is now.
//
void level_mouse_position_get(Gamep g, Levelsp v, Levelp l)
{
  TRACE();

  if ((v == nullptr) || (l == nullptr)) {
    return;
  }

  //
  // Get the visible map bounds
  //
  int visible_map_tl_x = 0;
  int visible_map_tl_y = 0;
  int visible_map_br_x = 0;
  int visible_map_br_y = 0;
  game_visible_map_pix_get(g, &visible_map_tl_x, &visible_map_tl_y, &visible_map_br_x, &visible_map_br_y);

  //
  // Find out what pixel on the map the mouse is over
  //
  int map_mouse_x = sdl.mouse_x - visible_map_tl_x;
  int map_mouse_y = sdl.mouse_y - visible_map_tl_y;

  int const zoom = game_map_zoom_get(g, v, l);

  //
  // Do not allow the mouse events to have an impact when not over the map fbo
  //
  int const map_pix_width  = visible_map_br_x - visible_map_tl_x;
  int const map_pix_height = visible_map_br_y - visible_map_tl_y;

  if (map_mouse_x < 0) {
    level_cursor_set(g, v, bpoint(-1, -1));
    return;
  }
  if (map_mouse_y < 0) {
    level_cursor_set(g, v, bpoint(-1, -1));
    return;
  }
  if (map_mouse_x >= map_pix_width) {
    level_cursor_set(g, v, bpoint(-1, -1));
    return;
  }
  if (map_mouse_y >= map_pix_height) {
    level_cursor_set(g, v, bpoint(-1, -1));
    return;
  }

  float const scale_x = static_cast< float >(map_pix_width) / static_cast< float >(game_map_fbo_width_get(g));
  float const scale_y = static_cast< float >(map_pix_height) / static_cast< float >(game_map_fbo_height_get(g));

  map_mouse_x = static_cast< int >(static_cast< float >(map_mouse_x) / scale_x);
  map_mouse_y = static_cast< int >(static_cast< float >(map_mouse_y) / scale_y);

  map_mouse_x += v->pixel_map_at.x;
  map_mouse_y += v->pixel_map_at.y;

  map_mouse_x /= TILE_WIDTH;
  map_mouse_y /= TILE_HEIGHT;

  map_mouse_x /= zoom;
  map_mouse_y /= zoom;

  //
  // Update the cursor
  //
  bpoint const p(map_mouse_x, map_mouse_y);
  level_cursor_set(g, v, p);
}
