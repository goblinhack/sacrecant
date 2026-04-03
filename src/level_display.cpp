//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_game.hpp"
#include "my_game_popups.hpp"
#include "my_gl.hpp"
#include "my_globals.hpp"
#include "my_level.hpp"
#include "my_level_inlines.hpp"
#include "my_main.hpp"
#include "my_sdl_proto.hpp"
#include "my_thing_callbacks.hpp"
#include "my_thing_inlines.hpp"

#include <utility>

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

  if (! level_is_player_level(g, v, l)) {
    return;
  }

  for (auto y = v->miny; y < v->maxy; y++) {
    for (auto x = v->minx; x < v->maxx; x++) {
      bpoint const p(x, y);
      level_display_cursor(g, v, l, p, fbo);
    }
  }
}

static void level_display_slot(Gamep g, Levelsp v, Levelp l, const bpoint &p, int slot, MapZDepth depth, FboEnum fbo)
{
  TRACE_DEBUG();

  Tpp   tp = nullptr;
  auto *t  = thing_and_tp_get_at(g, v, l, p, slot, &tp);
  if (tp == nullptr) [[unlikely]] {
    return;
  }

  if (std::cmp_not_equal(static_cast< int >(thing_z_depth_get(g, v, l, t)), static_cast< int >(depth))) {
    return;
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
static void level_display_fbo_do(Gamep g, Levelsp v, Levelp l, Levelp level_above, FboEnum fbo)
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
  FOR_ALL_MAP_Z_DEPTH(z_depth)
  {
    for (auto y = v->miny; y < v->maxy; y++) {
      for (auto x = v->minx; x < v->maxx; x++) {

        bpoint const p(x, y);
        auto         display_tile = false;

        switch (fbo) {
          case FBO_MAP_BG :
            display_tile = false;
            g_monochrome = true;

            if (level_has_seen_cached(g, v, l, p)) {
              //
              // Has seen previously
              //
              display_tile = true;
            }
            break;
          case FBO_MAP_FG :
            display_tile = false;
            g_monochrome = false;

            if (g_opt_debug1) {
              display_tile = true;
            } else if (is_level_select) {
              //
              // No lighting in level selection
              //
              display_tile = true;
            } else if (thing_vision_can_see_tile(g, v, player_level, player, p)) {
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
              if (level_is_blit_shown_in_overlay(g, v, l, p) != nullptr) {
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
              FOR_ALL_THINGS_AT_UNSAFE(g, v, l, it, p)
              {
                if (thing_is_hidden(it) != 0) {
                  display_tile = true;
                  break;
                }

                if (thing_is_jumping(it)) {
                  display_tile = true;
                  break;
                }
              }
            }
            break;
          default : break;
        }

        if (display_tile) {
          if (level_above != nullptr) {
            if (level_is_chasm(g, v, level_above, p) != nullptr) {
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
}

//
// Render the level to an FBO
//
static void level_display_fbo(Gamep g, Levelsp v, Levelp l, Levelp level_below, FboEnum fbo)
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

    if (level_below != nullptr) {
      level_display_fbo_do(g, v, level_below, l, fbo);
    }

    level_display_fbo_do(g, v, l, nullptr, fbo);

    blit_flush();

    if (fbo == FBO_MAP_FG_OVERLAY) {
      level_display_cursor(g, v, l, fbo);
      blit_flush();
      game_popups_display(g, v, l);
    }
  }
  blit_fbo_unbind();
}

void level_display(Gamep g, Levelsp v, Levelp l)
{
  TRACE_DEBUG();

  //
  // Set later
  //
  v->cursor_visible = false;

  //
  // Soft scroll to the player
  //
  level_scroll_to_focus(g, v, l);

  //
  // Get the next level for falling into and displaying under chasms.
  //
  auto *level_below = level_select_get_next_level_down(g, v, l);

  //
  // Animate both levels
  //
  FOR_ALL_TICKING_LEVELS(g, v, iter) { level_anim(g, v, iter); }

  //
  // Animate the level selection too
  //
  if (level_is_level_select(g, v, l)) {
    level_anim(g, v, l);
  }

  level_display_fbo(g, v, l, level_below, FBO_MAP_BG);
  // sdl_fbo_dump(g, FBO_MAP_BG, "FBO_MAP_BG");
  level_display_fbo(g, v, l, level_below, FBO_MAP_FG);
  // sdl_fbo_dump(g, FBO_MAP_FG, "FBO_MAP_FG");
  level_display_fbo(g, v, l, level_below, FBO_MAP_FG_OVERLAY);
  // sdl_fbo_dump(g, FBO_MAP_FG_OVERLAY, "FBO_MAP_FG_OVERLAY");

  //
  // Save the old pixel offset for restoring it after zoom toggling
  //
  v->pixel_map_at_for_zoom[ game_map_zoom_get(g) ] = v->pixel_map_at;
}

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

void level_blit(Gamep g)
{
  TRACE_DEBUG();

  if (g == nullptr) {
    return;
  }

  auto *v = game_levels_get(g);
  if (v == nullptr) {
    return;
  }

  auto *l = game_level_get(g, v);
  if (l == nullptr) {
    return;
  }

  //
  // Get the pixel extents of the map on screen
  //
  int visible_map_tl_x = 0;
  int visible_map_tl_y = 0;
  int visible_map_br_x = 0;
  int visible_map_br_y = 0;
  game_visible_map_pix_get(g, &visible_map_tl_x, &visible_map_tl_y, &visible_map_br_x, &visible_map_br_y);

  if (DEBUG || level_is_level_select(g, v, l)) {
    //
    // No lighting for level selection
    //
    blit_fbo_bind(FBO_MAP_FG_MERGED);
    {
      gl_clear();
      glBlendFunc(GL_ONE, GL_ZERO);

      blit_fbo(g, FBO_MAP_FG, visible_map_tl_x, visible_map_tl_y, visible_map_br_x, visible_map_br_y);
    }
    blit_fbo_unbind();
  } else {
    //
    // Blit the dark background tiles that have been seen previously
    //
    blit_fbo_bind(FBO_MAP_BG_MERGED);
    {
      gl_clear();
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

      blit_fbo(g, FBO_MAP_BG, visible_map_tl_x, visible_map_tl_y, visible_map_br_x, visible_map_br_y);

      level_blit_light(g, v, l, BLACK);
    }
    blit_fbo_unbind();

    //
    // Blit the light as a mask
    //
    blit_fbo_bind(FBO_MAP_FG_MERGED);
    {
      gl_clear();
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

      level_blit_light(g, v, l, WHITE);

      //
      // Mask out non lit areas of the foreground
      //
      glBlendFunc(GL_DST_ALPHA, GL_ZERO);

      blit_fbo(g, FBO_MAP_FG, visible_map_tl_x, visible_map_tl_y, visible_map_br_x, visible_map_br_y);
    }
    blit_fbo_unbind();
  }

  //
  // Blit things that are always shown once seen (and popups)
  //
  blit_fbo_bind(FBO_MAP_FG_MERGED);
  {
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    blit_fbo(g, FBO_MAP_FG_OVERLAY, visible_map_tl_x, visible_map_tl_y, visible_map_br_x, visible_map_br_y);
  }
  blit_fbo_unbind();

  //
  // Combine the FBOs into the final map
  //
  blit_fbo_bind(FBO_FINAL);
  {
    GLCOLOR(WHITE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    blit_fbo(g, FBO_MAP_BG_MERGED);
    blit_fbo(g, FBO_MAP_FG_MERGED);
  }
  blit_fbo_unbind();

  if (compiler_unused) {
    sdl_fbo_dump(g, FBO_FINAL, "FBO_FINAL");
  }
}
