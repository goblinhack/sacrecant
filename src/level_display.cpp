//
// Copyright goblinhack@gmail.com
//

#include "my_bpoint.hpp"
#include "my_callstack.hpp"
#include "my_color.hpp"
#include "my_color_defs.hpp"
#include "my_game.hpp"
#include "my_game_defs.hpp"
#include "my_game_popups.hpp"
#include "my_gl.hpp" // NOLINT
#include "my_globals.hpp"
#include "my_level.hpp"
#include "my_level_inlines.hpp"
#include "my_main.hpp"
#include "my_sdl_proto.hpp" // NOLINT
#include "my_spoint.hpp"
#include "my_thing.hpp"
#include "my_thing_inlines.hpp"
#include "my_time.hpp"
#include "my_tp.hpp"
#include "my_types.hpp"
#include "my_wid.hpp"

#include <cstdint>

static void level_blit_light(Gamep g, Levelsp v, Levelp l, color c)
{
  TRACE_DEBUG();

  //
  // Get the pixel extents of the map on screen
  //
  int visible_map_tl_x = 0;
  int visible_map_tl_y = 0;
  int visible_map_br_x = 0;
  int visible_map_br_y = 0;
  game_visible_map_pix_get(g, &visible_map_tl_x, &visible_map_tl_y, &visible_map_br_x, &visible_map_br_y);

  if (game_map_zoom_is_full_map_visible(g)) {
    //
    // Zoomed out. Full map visible.
    //
    blit_init();
    blit(g_fbo_tex_id[ FBO_MAP_LIGHT ], 0, 1, 1, 0, visible_map_tl_x, visible_map_tl_y, visible_map_br_x, visible_map_br_y, c);
    blit_flush();
  } else {
    //
    // Zoomed in. Partial map visible.
    //
    spoint tl1;
    spoint br1;
    spoint tl2;
    spoint br2;
    //
    // Get the on screen pixel co-oords of the top left and bottom right tiles
    //
    //    auto single_pix_size = game_map_single_pix_size_get(g);
    thing_display_get_tile_info(g, v, l, bpoint(0, 0), NULL_TP, NULL_THING, tl1, br1, nullptr);
    thing_display_get_tile_info(g, v, l, bpoint(MAP_WIDTH - 1, MAP_HEIGHT - 1), NULL_TP, NULL_THING, tl2, br2, nullptr);

    tl1.x += visible_map_tl_x;
    tl1.y += visible_map_tl_y;
    br2.x += visible_map_tl_x;
    br2.y += visible_map_tl_y;

    //
    // glScissor co-ordinates are inverted
    //
    auto y = game_window_pix_height_get(g) - visible_map_br_y;
    auto w = visible_map_br_x - visible_map_tl_x;
    auto h = visible_map_br_y - visible_map_tl_y;

    //
    // As we display the light map zoomed in, we need to clip it
    //
    glEnable(GL_SCISSOR_TEST);
    glScissor(visible_map_tl_x, y, w, h);

    //
    // Blit the entire light map, scaled to the pixel size of the zoomed in mode
    //
    blit_init();
    blit(g_fbo_tex_id[ FBO_MAP_LIGHT ], 0, 1, 1, 0, tl1.x, tl1.y, br2.x, br2.y, c);
    blit_flush();

    // sdl_fbo_dump(g, FBO_MAP_LIGHT, "FBO_MAP_LIGHT");

    glDisable(GL_SCISSOR_TEST);
  }
}

static void level_display_cursor(Gamep g, Levelsp v, Levelp l, const bpoint &p, FboEnum fbo)
{
  TRACE_DEBUG();

  Tpp tp = nullptr;

  switch (v->cursor[ p.x ][ p.y ]) {
    case CURSOR_NONE :
      //
      // Normal case. No cursor or anything else here.
      //
      return;
    case CURSOR_PATH :
      {
        //
        // No cursor path during level selection
        //
        if (level_is_level_select(g, v, l)) {
          return;
        }

        //
        // If the player is dead, then don't show the move path, but do allow them to move
        // the cursor around so they can still look at the level/cause of death.
        //
        auto *player = thing_player(g);
        if ((player != nullptr) && thing_is_dead(player)) {
          break;
        }

        //
        // Cursors do not use up slots on the map, to avoid them interacting with anything
        //
        static Tpp tp_once;
        if (tp_once == nullptr) {
          tp_once = tp_find_mand("cursor_path");
        }
        tp = tp_once;
        break;
      }
    case CURSOR_AT :
      {
        //
        // Cursors do not use up slots on the map, to avoid them interacting with anything
        //
        static Tpp tp_once;
        if (tp_once == nullptr) {
          tp_once = tp_find_mand("cursor_at");
        }
        tp                = tp_once;
        v->cursor_visible = true;
        break;
      }
  }

  if (tp != nullptr) {
    spoint   tl;
    spoint   br;
    uint16_t tile_index = 0;
    thing_display_get_tile_info(g, v, l, p, tp, NULL_THING, tl, br, &tile_index);
    thing_display(g, v, l, p, tp, NULL_THING, tl, br, tile_index, fbo);
  }
}

//
// Top level cursor
//
static void level_display_cursor(Gamep g, Levelsp v, Levelp l, FboEnum fbo)
{
  TRACE_DEBUG();

  if (level_is_level_select(g, v, l)) {
    //
    // ok
    //
  } else if (! level_is_player_level(g, v, l)) {
    //
    // Don't show the cursor on the lower level
    //
    return;
  }

  for (auto y = v->miny; y < v->maxy; y++) {
    for (auto x = v->minx; x < v->maxx; x++) {
      bpoint const p(x, y);
      level_display_cursor(g, v, l, p, fbo);
    }
  }
}

static void level_display_slot(Gamep g, Levelsp v, Levelp l, const bpoint &p, int slot, MapZDepthType depth, FboEnum fbo)
{
  TRACE_DEBUG();

  Tpp   tp = nullptr;
  auto *t  = thing_and_tp_get_at(g, v, l, p, slot, &tp);
  if (tp == nullptr) [[unlikely]] {
    return;
  }

  auto thing_depth = thing_z_depth_get(g, v, l, t);
  if (thing_depth != depth) {
    return;
  }

  //
  // Do not show the special overlay tile unless showing the overlay it belongs in
  //
  if (fbo != FBO_MAP_LAVA_OVERLAY) {
    if (thing_is_lava_bg(t)) {
      return;
    }
  }

  switch (fbo) {
    case FBO_MAP_BG_PREVIOUSLY_SEEN_TILES : break;

    case FBO_MAP_BG_FLOOR_WATER_LAVA :
      if (depth > MAP_Z_DEPTH_LAVA) {
        return;
      }
      break;

    case FBO_MAP_LAVA_OVERLAY :
      if (depth != MAP_Z_DEPTH_LAVA) {
        return;
      }
      break;

    case FBO_MAP_FG :
      if (depth <= MAP_Z_DEPTH_LAVA) {
        return;
      }
      break;

    case FBO_MAP_FG_OVERLAY : break;
    default :                 break;
  }

  spoint   tl;
  spoint   br;
  uint16_t tile_index = 0;
  thing_display_get_tile_info(g, v, l, p, tp, t, tl, br, &tile_index);
  thing_display(g, v, l, p, tp, t, tl, br, tile_index, fbo);
}

//
// Render the level to an FBO
//
static void level_display_fbo_do(Gamep g, Levelsp v, Levelp level_above, Levelp l, FboEnum fbo)
{
  TRACE_DEBUG();

  const bool is_level_select = level_is_level_select(g, v, l);

  auto *player = thing_player(g);
  if (player == nullptr) [[unlikely]] {
    return;
  }

  auto *player_level = thing_player_level(g);
  if (player_level == nullptr) {
    return;
  }

  //
  // Display tiles in z prio order
  //
  for (auto y = v->miny; y < v->maxy; y++) {
    FOR_ALL_MAP_Z_DEPTH(z_depth)
    {
      for (auto x = v->minx; x < v->maxx; x++) {

        bpoint const p(x, y);
        auto         display_tile = false;

        switch (fbo) {
          case FBO_MAP_BG_PREVIOUSLY_SEEN_TILES :
            display_tile = false;
            g_monochrome = true;

            if (level_has_seen_cached(g, v, l, p)) {
              //
              // Has seen previously
              //
              display_tile = true;
            }
            break;

          case FBO_MAP_BG_FLOOR_WATER_LAVA :
            display_tile = false;
            g_monochrome = false;

            if (thing_vision_can_see_tile(g, v, player_level, player, p)) {
              //
              // Can see currently
              //
              display_tile = true;
            }
            break;

          case FBO_MAP_LAVA_OVERLAY :
            g_monochrome = false;

            if (level_above != nullptr) {
              display_tile = level_is_lava_cached(g, v, l, p);
            } else {
              if (level_has_seen_cached(g, v, l, p)) {
                display_tile = level_is_lava_cached(g, v, l, p);
              }
            }
            break;

          case FBO_MAP_FG :
            display_tile = false;
            g_monochrome = false;

            if (thing_vision_can_see_tile(g, v, player_level, player, p)) {
              //
              // Can see currently
              //
              display_tile = true;
            }
            break;

          case FBO_MAP_FG_OVERLAY :
            display_tile = false;
            g_monochrome = false;

            //
            // Only show things we have seen previously
            //
            if (level_has_seen_cached(g, v, l, p)) {
              //
              // Certain things, like exits, once seen, are always lit above the light
              //
              if (level_is_blit_shown_in_overlay_bool(g, v, l, p)) {
                //
                // Always show
                //
                display_tile = true;
              }
            }

            //
            // Need to show hidden things in the overlay. e.g. a ghost inside a wall
            //
            if (! display_tile) {
              //
              // But only for base tiles that we have seen already. Else a hidden ghost
              // might briefly appear off screen
              //
              if (thing_vision_can_see_tile(g, v, player_level, player, p)) {
                FOR_ALL_THINGS_AT_UNSAFE(g, v, l, it, p)
                {
                  if (thing_is_player(it) && thing_is_invisible(g, v, l, it)) {
                    display_tile = true;
                    break;
                  }

                  if (thing_is_hidden(it) != 0) {
                    display_tile = true;
                    break;
                  }

                  if (thing_is_levitating(g, v, l, it) || thing_is_jumping(it) || thing_is_thrown(it)) {
                    display_tile = true;
                    break;
                  }
                }
              }
            }
            break;
          default : break;
        }

        if (g_opt_debug1) {
          display_tile = true;
        } else if (is_level_select) {
          //
          // No lighting in level selection
          //
          display_tile = true;
        }

        if (display_tile) {
          if (level_above != nullptr) {
            if (level_is_chasm_cached(g, v, level_above, p)) {
              //
              // Only show this tile if the level above is a chasm
              //
            } else {
              //
              // Viewing through a chasm
              //
              display_tile = false;
            }
          }

          if (display_tile) {
            //
            // Display all things at this location (for this z depth)
            //
            for (auto slot = 0; slot < MAP_SLOTS; slot++) {
              level_display_slot(g, v, l, p, slot, z_depth, fbo);
            }
          }
        }

        g_monochrome = false;
      }
    }
  }

  IF_NODEBUG
  {
    if (fbo == FBO_MAP_LAVA_OVERLAY) {
      auto         z_depth = MAP_Z_DEPTH_LAVA;
      bpoint const p(0, 0);
      auto         display_tile = false;

      switch (fbo) {
        case FBO_MAP_LAVA_OVERLAY :
          g_monochrome = false;
          display_tile = level_is_lava_bg_cached(g, v, l, p);
          break;

        default : break;
      }

      if (display_tile) {
        //
        // Display all things at this location (for this z depth)
        //
        for (auto slot = 0; slot < MAP_SLOTS; slot++) {
          level_display_slot(g, v, l, p, slot, z_depth, fbo);
        }
      }

      g_monochrome = false;
    }
  }
}

//
// Render the level to a specific FBO. This will involve filtering based on the FBO.
//
static void level_display_fbo(Gamep g, Levelsp v, Levelp level_above, Levelp l, FboEnum fbo)
{
  TRACE_DEBUG();

  //
  // What level is the player on?
  //
  auto *player = thing_player(g);
  if (player == nullptr) [[unlikely]] {
    return;
  }

  blit_fbo_bind(fbo);
  {
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    gl_clear();

    blit_init();
    level_display_fbo_do(g, v, level_above, l, fbo);
    blit_flush();

    //
    // Blit the cursor if needed
    //
    if (fbo == FBO_MAP_FG_OVERLAY) {
      if (wid_over == nullptr) {
        switch (game_state(g)) {
          case STATE_THROW_ITEM :         [[fallthrough]];
          case STATE_LEVEL_SELECT_MENU :  [[fallthrough]];
          case STATE_PLAYER_SELECT_MENU : [[fallthrough]];
          case STATE_PLAYING :
            blit_init();
            level_display_cursor(g, v, l, fbo);
            blit_flush();
            break;
          case STATE_COLLECT_MENU :      [[fallthrough]];
          case STATE_DEAD_MENU :         [[fallthrough]];
          case STATE_GENERATED :         [[fallthrough]];
          case STATE_GENERATING :        [[fallthrough]];
          case STATE_INIT :              [[fallthrough]];
          case STATE_INVENTORY_MENU :    [[fallthrough]];
          case STATE_ITEM_MENU :         [[fallthrough]];
          case STATE_KEYBOARD_MENU :     [[fallthrough]];
          case STATE_LOAD_MENU :         [[fallthrough]];
          case STATE_LOADED :            [[fallthrough]];
          case STATE_MAIN_MENU :         [[fallthrough]];
          case STATE_MOVE_WARNING_MENU : [[fallthrough]];
          case STATE_QUIT_MENU :         [[fallthrough]];
          case STATE_QUITTING :          [[fallthrough]];
          case STATE_SAVE_MENU :         [[fallthrough]];
          case STATE_GAME_OVER_MENU :    [[fallthrough]];
          case STATE_THROW_MENU :        [[fallthrough]];
          case GAME_STATE_ENUM_MAX :     break;
        }
      }

      //
      // Popups (like damage to the player)
      //
      game_popups_display(g, v, l);
    }
  }
  blit_fbo_unbind();
}

//
// Display the entire level into either
//
// FBO_FULL_SCREEN_LEVEL_CURR - the current player level
// FBO_FULL_SCREEN_LEVEL_BELOW - the level below
//
static void level_display_fbos(Gamep g, Levelsp v, Levelp level_above, Levelp l)
{
  TRACE_DEBUG();

  gl_enter_2d_mode(g, game_map_fbo_width_get(g), game_map_fbo_height_get(g));

  const bool is_level_select = level_is_level_select(g, v, l);

  if (level_above == nullptr) {
    level_display_fbo(g, v, level_above, l, FBO_MAP_BG_PREVIOUSLY_SEEN_TILES);
    // sdl_fbo_dump(g, FBO_MAP_BG_PREVIOUSLY_SEEN_TILES, "FBO_MAP_BG_PREVIOUSLY_SEEN_TILES");
  }

  level_display_fbo(g, v, level_above, l, FBO_MAP_BG_FLOOR_WATER_LAVA);
  // sdl_fbo_dump(g, FBO_MAP_BG_FLOOR_WATER_LAVA, "FBO_MAP_BG_FLOOR_WATER_LAVA");

  level_display_fbo(g, v, level_above, l, FBO_MAP_LAVA_OVERLAY);
  // sdl_fbo_dump(g, FBO_MAP_LAVA_OVERLAY, "FBO_MAP_LAVA_OVERLAY");

  level_display_fbo(g, v, level_above, l, FBO_MAP_FG);
  // sdl_fbo_dump(g, FBO_MAP_FG, "FBO_MAP_FG");

  if (level_above == nullptr) {
    level_display_fbo(g, v, level_above, l, FBO_MAP_FG_OVERLAY);
    // sdl_fbo_dump(g, FBO_MAP_FG_OVERLAY, "FBO_MAP_FG_OVERLAY");
  }

  //
  // Get the pixel extents of the map on screen
  //
  int visible_map_tl_x = 0;
  int visible_map_tl_y = 0;
  int visible_map_br_x = 0;
  int visible_map_br_y = 0;
  game_visible_map_pix_get(g, &visible_map_tl_x, &visible_map_tl_y, &visible_map_br_x, &visible_map_br_y);

  gl_enter_2d_mode(g, game_window_pix_width_get(g), game_window_pix_height_get(g));

  if (DEBUG || is_level_select) {
    //
    // No lighting for level selection
    //
    blit_fbo_bind(FBO_FULL_SCREEN_VISIBLE_TILES);
    {
      //
      // Blit the floor tiles, water, lava, ripples
      //
      glBlendFunc(GL_ONE, GL_ZERO);
      blit_fbo(g, FBO_MAP_BG_FLOOR_WATER_LAVA, visible_map_tl_x, visible_map_tl_y, visible_map_br_x, visible_map_br_y, WHITE);

      //
      // Blit the lava scrolling pattern
      //
      glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
      blit_fbo(g, FBO_MAP_LAVA_OVERLAY, visible_map_tl_x, visible_map_tl_y, visible_map_br_x, visible_map_br_y, WHITE);

      //
      // Blit all other tiles, walls, objects
      //
      glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
      blit_fbo(g, FBO_MAP_FG, visible_map_tl_x, visible_map_tl_y, visible_map_br_x, visible_map_br_y, WHITE);

      //
      // Note, no light masking
      //
    }
    blit_fbo_unbind();
  } else {
    //
    // Blit the dark background tiles that have been seen previously
    //
    blit_fbo_bind(FBO_FULL_SCREEN_PREVIOUSLY_SEEN_TILES);
    {
      //
      // Blit the floor tiles, water, lava, ripples
      //
      glBlendFunc(GL_ONE, GL_ZERO);
      blit_fbo(g, FBO_MAP_BG_PREVIOUSLY_SEEN_TILES, visible_map_tl_x, visible_map_tl_y, visible_map_br_x, visible_map_br_y, WHITE);

      //
      // Mask out non lit areas of the foreground
      //
      glBlendFunc(GL_ZERO, GL_ONE_MINUS_SRC_ALPHA);
      level_blit_light(g, v, l, WHITE);
    }
    blit_fbo_unbind();

    //
    // Blit the light as a mask
    //
    blit_fbo_bind(FBO_FULL_SCREEN_VISIBLE_TILES);
    {
      //
      // Blit the floor tiles, water, lava, ripples
      //
      glBlendFunc(GL_ONE, GL_ZERO); // no need to gl_clear
      blit_fbo(g, FBO_MAP_BG_FLOOR_WATER_LAVA, visible_map_tl_x, visible_map_tl_y, visible_map_br_x, visible_map_br_y, WHITE);

      //
      // Blit the lava scrolling pattern
      //
      glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
      blit_fbo(g, FBO_MAP_LAVA_OVERLAY, visible_map_tl_x, visible_map_tl_y, visible_map_br_x, visible_map_br_y, WHITE);

      //
      // Blit all other tiles, walls, objects
      //
      glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
      blit_fbo(g, FBO_MAP_FG, visible_map_tl_x, visible_map_tl_y, visible_map_br_x, visible_map_br_y, WHITE);

      //
      // Mask out non lit areas of the foreground
      //
      glBlendFunc(GL_DST_ALPHA, GL_ONE);
      level_blit_light(g, v, l, COLOR_NONE);
    }
    blit_fbo_unbind();
  }

  //
  // Blit to the level below and then next time we enter here, the level above on top of that.
  // There are holes in the level above (chasms) that allow you to see the level below through them.
  //
  if (level_above != nullptr) {
    //
    // This is the level below. We only need to show visibile tiles.
    //
    blit_fbo_bind(FBO_FULL_SCREEN_LEVEL_BELOW);
    {
      glBlendFunc(GL_ONE, GL_ZERO); // no need to gl_clear
      blit_fbo(g, FBO_FULL_SCREEN_VISIBLE_TILES, WHITE);
    }
    blit_fbo_unbind();
  } else {
    //
    // This is the level above.
    //
    blit_fbo_bind(FBO_FULL_SCREEN_LEVEL_CURR);
    {
      glBlendFunc(GL_ONE, GL_ZERO); // no need to gl_clear

      if (is_level_select) {
        //
        // Level selection has no hidden tiles
        //
        blit_fbo(g, FBO_FULL_SCREEN_VISIBLE_TILES, WHITE);
      } else {
        //
        // Show the previously seen tiles and then the current visible ones
        //
        blit_fbo(g, FBO_FULL_SCREEN_PREVIOUSLY_SEEN_TILES, WHITE);

        //
        // Overlay the visible tiles
        //
        glBlendFunc(GL_ONE_MINUS_DST_ALPHA, GL_ONE);
        blit_fbo(g, FBO_FULL_SCREEN_VISIBLE_TILES, WHITE);
      }

      //
      // Blit things that are always shown (regardless of debug mode) once seen (and popups)
      //
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      blit_fbo(g, FBO_MAP_FG_OVERLAY, visible_map_tl_x, visible_map_tl_y, visible_map_br_x, visible_map_br_y, WHITE);
    }
    blit_fbo_unbind();
  }

  // sdl_fbo_dump(g, FBO_FULL_SCREEN_LEVEL_CURR, "FBO_FULL_SCREEN_LEVEL_CURR");
  // sdl_fbo_dump(g, FBO_FULL_SCREEN_LEVEL_BELOW, "FBO_FULL_SCREEN_LEVEL_BELOW");
  // sdl_fbo_dump(g, FBO_FULL_SCREEN_PREVIOUSLY_SEEN_TILES, "FBO_FULL_SCREEN_PREVIOUSLY_SEEN_TILES");
  // sdl_fbo_dump(g, FBO_FULL_SCREEN_VISIBLE_TILES, "FBO_FULL_SCREEN_VISIBLE_TILES");
}

//
// Merge the level below and the one above into a single FBO we can blit each loop
//
static void level_display_merge(Gamep g, Levelsp v, Levelp l)
{
  TRACE_DEBUG();

  //
  // Give a tint to the level below
  //
  auto c = GRAY50;

  switch (level_to_biome(g, v, l)) {
    case BIOME_DUNGEON : break;
    case BIOME_BOGLAND :
      c   = GREEN;
      c.b = 200;
      break;
    case BIOME_NETHERVOID :
      c   = GRAY20;
      c.b = 200;
      break;
    case BIOME_GRAVEYARD :
      c   = GREEN;
      c.b = 200;
      break;
    case BIOME_UNDERHELL :
      c   = RED;
      c.b = 200;
      break;
    case BIOME_NONE :     [[fallthrough]];
    case BIOME_ENUM_MAX : break;
  }

  blit_fbo_bind(FBO_FULL_SCREEN_LEVEL_MERGED);
  {
    //
    // Combine the FBOs into the final map
    //
    glBlendFunc(GL_ONE, GL_ZERO); // no need to gl_clear
    blit_fbo(g, FBO_FULL_SCREEN_LEVEL_BELOW, c);

    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    blit_fbo(g, FBO_FULL_SCREEN_LEVEL_CURR, WHITE);
  }
  blit_fbo_unbind();

  // sdl_fbo_dump(g, FBO_FULL_SCREEN_LEVEL_BELOW, "FBO_FULL_SCREEN_LEVEL_BELOW");
  // sdl_fbo_dump(g, FBO_FULL_SCREEN_LEVEL_CURR, "FBO_FULL_SCREEN_LEVEL_CURR");
}

void level_display(Gamep g, Levelsp v, Levelp l)
{
  TRACE_DEBUG();

  //
  // Do we need to update the frame buffer as rapidly as the event loop?
  //
  static uint32_t update {};
  if (! game_time_have_x_hundredths_passed_since(1, update)) {
    return;
  }
  update = game_time_ms();

  //
  // Set later
  //
  v->cursor_visible = false;

  //
  // Soft scroll to the player
  //
  level_scroll_to_focus(g, v, l);

  //
  // Animate both levels
  //
  FOR_ALL_TICKING_LEVELS(g, v, iter) { level_anim(g, v, iter); }

  if (level_is_level_select(g, v, l)) {
    //
    // Animate the level selection too
    //
    level_anim(g, v, l);
  } else {
    //
    // Get the next level for falling into and displaying under chasms.
    //
    auto *level_below = level_select_get_next_level_down(g, v, l);
    if (level_below != nullptr) {
      level_display_fbos(g, v, l, level_below);
    }
  }

  level_display_fbos(g, v, nullptr, l);

  level_display_merge(g, v, l);

  //
  // Save the old pixel offset for restoring it after zoom toggling
  //
  v->pixel_map_at_for_zoom[ game_map_zoom_get(g) ] = v->pixel_map_at;

  //
  // Light flicker
  //
  static uint32_t last_flicker {};
  if (game_time_have_x_hundredths_passed_since(THING_LIGHT_FLICKER_ANIM_MS, last_flicker)) {
    level_light_calculate_all(g, v, l);
    last_flicker = game_time_ms_cached();
  }
}

void level_blit(Gamep g)
{
  TRACE_DEBUG();

  //
  // Blit the merged lower and current level in one
  //
  glBlendFunc(GL_ONE, GL_ZERO); // no need to gl_clear
  blit_fbo(g, FBO_FULL_SCREEN_LEVEL_MERGED, WHITE);
}
