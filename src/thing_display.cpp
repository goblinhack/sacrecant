//
// Copyright goblinhack@gmail.com
//

#include <cmath>

#include <algorithm>
#include <numbers>

#include "my_callstack.hpp"
#include "my_color_defs.hpp"
#include "my_game.hpp"
#include "my_globals.hpp"
#include "my_main.hpp"
#include "my_math.hpp"
#include "my_thing_callbacks.hpp"
#include "my_thing_inlines.hpp"

void tp_display_init(Tpp tp)
{
  TRACE();

  if (tp_is_blit_centered(tp)) {
    if (tp_is_blit_on_ground(tp)) {
      tp_err(tp, "both blit on_ground and centered are set");
    }
  } else if (tp_is_blit_on_ground(tp)) {
    // ok
  } else if (tp_is_blit_bg(tp)) {
    // ok
  } else {
    //    tp_err(tp, "neither blit on_ground or centered are set");
  }
}

void thing_display_get_tile_info(Gamep g, Levelsp v, Levelp l, const bpoint &p, Tpp tp_maybe_null, Thingp t_maybe_null, spoint &tl,
                                 spoint &br, uint16_t *tile_index)
{
  TRACE_DEBUG();

  int const zoom = game_map_zoom_get(g);
  int const dw   = TILE_WIDTH * zoom;
  int const dh   = TILE_HEIGHT * zoom;
  Tilep     tile = nullptr;

  tile = thing_display_get_tile_info(g, v, l, p, tp_maybe_null, t_maybe_null);
  if (tile != nullptr) {
    //
    // Allow override of the tile
    //
    if (tile_index != nullptr) {
      *tile_index = tile_global_index(tile);
    }
  } else if (t_maybe_null != nullptr) {
    //
    // Things
    //
    if (tile_index != nullptr) {
      *tile_index = t_maybe_null->tile_index;
    }
  } else if (tp_maybe_null != nullptr) {
    //
    // Cursor usually
    //
    tile = tp_tiles_get(tp_maybe_null, THING_ANIM_IDLE, 0);

    if (tile_index != nullptr) {
      *tile_index = tile_global_index(tile);
    }
  } else {
    static Tilep no_tile;
    if (no_tile == nullptr) {
      no_tile = tile_find_mand("none");
    }
    tile = no_tile;

    if (tile_index != nullptr) {
      *tile_index = tile_global_index(tile);
    }
  }

  if (tile_index != nullptr) {
    tile = tile_index_to_tile(*tile_index);
  }

  if (tile == nullptr) {
    tile = tile_find_mand("none");
  }

  if (t_maybe_null != nullptr) {
    //
    // All things
    //
    tl = thing_pix_at(t_maybe_null);
    tl.x *= zoom;
    tl.y *= zoom;
  } else {
    //
    // Cursor
    //
    tl.x = p.x * dw;
    tl.y = p.y * dh;
  }

  tl -= v->pixel_map_at;

  auto pix_height = tile_height(tile) * zoom;
  auto pix_width  = tile_width(tile) * zoom;

  //
  //    centered       centered      on_ground     on_ground
  // ..............     (same)        (same)     ..............
  // ..............  ............  ............  ..............
  // ..............  ............  ............  ..............
  // ..............  ............  ............  ..............
  // ..............  ............  ............  ..............
  // ..............  ............  ............  ..............
  // ....14x14.....  ...12x12....  ...12x12....  ....14x14.....
  // ..............  ............  ............  ..............
  // ..............  ............  ............  ..............
  // ..............  ............  ............  ..............
  // ..............  ............  ............  ..............
  // ..............  ............  ............  ..............
  // ..............  ............  ............  ..............
  // ..............  ............  ............  .............. <---- ground level
  // ..............
  //
  if (tp_maybe_null != nullptr) {
    if (tp_is_blit_centered(tp_maybe_null)) {
      tl.x -= (pix_width - dw) / 2;
      tl.y -= (pix_height - dh) / 2;
    } else if (tp_is_blit_on_ground(tp_maybe_null)) {
      tl.x -= (pix_width - dw) / 2;
      tl.y -= (pix_height - dh);
    }
  }

  if (t_maybe_null != nullptr) {
    if (thing_is_jumping(t_maybe_null)) {
      auto jump_height = static_cast< int >((sin(std::numbers::pi_v< float > * t_maybe_null->thing_dt)) * static_cast< float >(dh));
      jump_height *= THING_JUMP_HEIGHT_TILES;
      tl.y -= jump_height;
      br.y -= jump_height;
    }
  }

  //
  // Update the br coords if we changed the position
  //
  br.x = tl.x + pix_width;
  br.y = tl.y + pix_height;

  //
  // Flippable?
  //
  if ((t_maybe_null != nullptr) && tp_is_animated_can_hflip(tp_maybe_null)) {
    if (thing_is_dir_left(t_maybe_null) || thing_is_dir_tl(t_maybe_null) || thing_is_dir_bl(t_maybe_null)) {
      std::swap(tl.x, br.x);
    }
  }

  if (t_maybe_null != nullptr) {
    thing_lunge_modify_position(g, v, l, t_maybe_null, tl, br);
  }
}

//
// Solid black outline
//
static void thing_display_outlined_blit(Gamep g, Tpp tp, spoint tl, spoint br, Tilep tile, float x1, float x2, float y1, float y2,
                                        const color &fg, const color &outline)
{
  TRACE_DEBUG();

  //
  // Outlined?
  //
  auto single_pix_size = game_map_single_pix_size_get(g);

  //
  // Disable outlines when zoomed out
  //
  if (game_map_zoom_is_full_map_visible(g)) {
    single_pix_size = 0;
  }

  if (tp_is_blit_outlined(tp)) {
    tile_blit_outlined(tile, x1, x2, y1, y2, tl, br, fg, outline, single_pix_size, false);
  } else if (tp_is_blit_square_outlined(tp)) {
    tile_blit_outlined(tile, x1, x2, y1, y2, tl, br, fg, outline, single_pix_size, true);
  }
}

//
// Show an outline if obscured? e.g. foliage and the player hiding in it
//
[[nodiscard]] static auto thing_display_hidden(Gamep g, Levelsp v, Levelp l, const bpoint &p, Tpp tp, Thingp t_maybe_null, spoint tl,
                                               spoint br, Tilep tile, float x1, float x2, float y1, float y2, FboEnum fbo, color fg) -> bool
{
  TRACE_DEBUG();

  //
  // Flash if hidden
  //
  if (t_maybe_null == nullptr) {
    return false;
  }
  auto h = thing_is_hidden(t_maybe_null);
  if (h == 0) {
    return false;
  }

  if (thing_is_blit_when_obscured_faded(t_maybe_null)) {
    fg.a = h;
    tile_blit(tile, x1, x2, y1, y2, tl, br, fg);
  } else if (thing_is_blit_when_obscured_outline(t_maybe_null)) {
    color c = WHITE;
    c.a     = h;
    tile_blit_outline_w_invis_inside(tile, x1, x2, y1, y2, tl, br, c);
  } else {
    thing_err(t_maybe_null, "need to set obscured type");
  }

  return true;
}

static void thing_display_blit(Gamep g, Levelsp v, Levelp l, const bpoint &p, Tpp tp, Thingp t_maybe_null, spoint tl, spoint br, Tilep tile,
                               float x1, float x2, float y1, float y2, FboEnum fbo, color fg, LightPixels *light_pixels = nullptr,
                               bool blit_flush_per_line = false)
{
  TRACE_DEBUG();

  switch (fbo) {
    case FBO_MAP_FG_OVERLAY :
      //
      // Hidden things need to be shown on top of walls or foliage
      //
      if (thing_display_hidden(g, v, l, p, tp, t_maybe_null, tl, br, tile, x1, x2, y1, y2, fbo, fg)) {
        return;
      }

      //
      // Jumping things need to be seen over other things
      //
      if ((t_maybe_null != nullptr) && thing_is_jumping(t_maybe_null)) {
        break;
      }

      //
      // Things like exits and entrances are shown in color once seen
      //
      if (! tp_is_blit_shown_in_overlay(tp)) {
        return;
      }

      break;

    default : break;
  }

  //
  // NOTE: light_pixels is set for things like floors and walls, and blits the tile as lots of individual
  // pixels with their own lighting
  //

  if (tp_is_blit_outlined(tp) || tp_is_blit_square_outlined(tp)) {
    thing_display_outlined_blit(g, tp, tl, br, tile, x1, x2, y1, y2, fg, BLACK);
  } else {
    tile_blit(tile, x1, x2, y1, y2, tl, br, fg, light_pixels, blit_flush_per_line);
  }
}

//
// Display a spinning falling thing
//
static void thing_display_falling(Gamep g, Levelsp v, Levelp l, const bpoint &p, Tpp tp, Thingp t, spoint tl, spoint br, Tilep tile,
                                  float x1, float x2, float y1, float y2, FboEnum fbo, color fg)
{
  TRACE_DEBUG();

  int const fall_height = thing_is_falling(t);
  int const dh
      = static_cast< int >(((0.5F * (static_cast< float >(br.y - tl.y))) / static_cast< float >(THING_FALL_TIME_MS)) * fall_height);

  tl.x += dh;
  tl.y += dh;
  br.x -= dh;
  br.y -= dh;

  auto mid = (tl + br) / static_cast< short >(2);
  blit_flush();
  glPushMatrix();
  glTranslatef(mid.x, mid.y, 0);
  float const ang = dh * 10;
  glRotatef(ang, 0.0F, 0.0F, 1.0F);
  glTranslatef(-mid.x, -mid.y, 0);
  thing_display_blit(g, v, l, p, tp, t, tl, br, tile, x1, x2, y1, y2, fbo, fg);
  blit_flush();
  glPopMatrix();
}

//
// Display a spinning thing
//
static void thing_display_rotated(Gamep g, Levelsp v, Levelp l, const bpoint &p, Tpp tp, Thingp t, spoint tl, spoint br, Tilep tile,
                                  float x1, float x2, float y1, float y2, FboEnum fbo, color fg)
{
  TRACE_DEBUG();

  auto mid = (tl + br) / static_cast< short >(2);
  blit_flush();
  glPushMatrix();
  glTranslatef(mid.x, mid.y, 0);
  float const ang = t->angle * (180.0F / std::numbers::pi_v< float >);
  glRotatef(ang, 0.0F, 0.0F, 1.0F);
  glTranslatef(-mid.x, -mid.y, 0);
  thing_display_blit(g, v, l, p, tp, t, tl, br, tile, x1, x2, y1, y2, fbo, fg);
  blit_flush();
  glPopMatrix();
}

//
// Handle all the various lighting modes to display a thing
//
static void thing_display_it(Gamep g, Levelsp v, Levelp l, const bpoint &p, Tpp tp, Thingp t_maybe_null, spoint tl, spoint br, Tilep tile,
                             float x1, float x2, float y1, float y2, FboEnum fbo, color fg, LightPixels *light_pixels = nullptr)
{
  TRACE_DEBUG();

  IF_DEBUG
  {
    light_pixels = nullptr;
    fg.r         = 255;
    fg.g         = 255;
    fg.b         = 255;
  }

  if (t_maybe_null == nullptr) {
    //
    // Probably the cursor
    //
    thing_display_blit(g, v, l, p, tp, t_maybe_null, tl, br, tile, x1, x2, y1, y2, fbo, fg, nullptr, false);
    return;
  }

  //
  // Rotate when falling
  //
  if (t_maybe_null->angle != 0.0) {
    thing_display_rotated(g, v, l, p, tp, t_maybe_null, tl, br, tile, x1, x2, y1, y2, fbo, fg);
    return;
  }

  //
  // If we have alpha values in the texture, the end of one triangle line and the start of another creates
  // a visible strip
  //
  bool const is_blit_flush_per_line = thing_is_blit_flush_per_line(t_maybe_null);
  thing_display_blit(g, v, l, p, tp, t_maybe_null, tl, br, tile, x1, x2, y1, y2, fbo, fg, light_pixels, is_blit_flush_per_line);

  //
  // Flash red outline if hit
  //
  if (thing_is_hit(t_maybe_null) != 0) {
    float a = (static_cast< float >(thing_is_hit(t_maybe_null)) / static_cast< float >(THING_HIT_FLASH_TIME_MS));
    a *= 255.0F;
    a = std::min(static_cast< int >(a), 255);

    if (thing_is_blit_hit_outline_w_invis_inside(t_maybe_null)) {
      color outline = RED;
      outline.a     = static_cast< uint8_t >(a);
      tile_blit_outline_w_invis_inside(tile, x1, x2, y1, y2, tl, br, outline);
      return;
    }
    if (thing_is_blit_hit_outline_w_black_inside(t_maybe_null)) {
      color outline = RED;
      outline.a     = static_cast< uint8_t >(a);
      tile_blit_outline_w_black_inside(tile, x1, x2, y1, y2, tl, br, outline);
      return;
    }
    color const is_hot = RED;
    fg.r               = is_hot.r;
    fg.g               = is_hot.g;
    fg.b               = is_hot.b;
    fg.a               = static_cast< uint8_t >(a);
    thing_display_blit(g, v, l, p, tp, t_maybe_null, tl, br, tile, x1, x2, y1, y2, fbo, fg, nullptr, false);
    return;
  }

  if (0) {
    if (thing_is_monst(t_maybe_null) && ! thing_is_dead(t_maybe_null)) {
      //
      // If low on health, orange outline
      //
      auto h_max = tp_health_max_get(tp);
      auto h     = thing_health(t_maybe_null);

      if (h < h_max) {
        color const c = CYAN;
        tile_blit_outline_w_invis_inside(tile, x1, x2, y1, y2, tl, br, c);
        return;
      }
    }
  }

  if ((thing_is_hot(t_maybe_null) != 0) && ! thing_is_dead(t_maybe_null)) {
    //
    // Pulse when hot. But not when dead. Else a monster killed by a fireball will pulse!
    //
    if (! thing_is_always_hot(t_maybe_null)) {
      color const hot = ORANGE;
      fg.r            = hot.r;
      fg.g            = hot.g;
      fg.b            = hot.b;
      fg.a            = thing_is_hot(t_maybe_null);
      light_pixels    = nullptr;
      thing_display_blit(g, v, l, p, tp, t_maybe_null, tl, br, tile, x1, x2, y1, y2, fbo, fg, light_pixels, is_blit_flush_per_line);

      if (thing_is_blit_hit_outline_w_invis_inside(t_maybe_null)) {
        tile_blit_outline_w_invis_inside(tile, x1, x2, y1, y2, tl, br, fg);
      } else if (thing_is_blit_hit_outline_w_black_inside(t_maybe_null)) {
        tile_blit_outline_w_black_inside(tile, x1, x2, y1, y2, tl, br, fg);
      }
    }
  }
}

//
// Display a single thing to an FBO
//
void thing_display(Gamep g, Levelsp v, Levelp l, const bpoint &p, Tpp tp, Thingp t_maybe_null, spoint tl, spoint br, uint16_t tile_index,
                   FboEnum fbo)
{
  TRACE_DEBUG();

  bool is_falling = false;

  auto *player = thing_player(g);
  if (player == nullptr) [[unlikely]] {
    return;
  }

  //
  // What level are we currently displaying?
  //
  const auto is_level_select = level_is_level_select(g, v, l);

  if (t_maybe_null != nullptr) {
    is_falling = thing_is_falling(t_maybe_null) > 0;
  }

  if (DEBUG || is_level_select) {
    //
    // No hiding of objects
    //
  } else {
    //
    // What level is the player on?
    //
    auto *player_level = game_level_get(g, v, player->level_num);
    if (player_level == nullptr) [[unlikely]] {
      return;
    }

    //
    // If we're blitting the level below, filter to only things we can see through chasms
    //
    if (player_level != l) {
      if (! tp_is_blit_shown_in_chasms(tp)) {
        return;
      }
    }

    if ((t_maybe_null != nullptr) && ! thing_vision_can_see_tile(g, v, player_level, player, p)) {
      //
      // We cannot see this tile currently.
      //
      if (thing_is_blit_if_has_seen(t_maybe_null)) {
        //
        // But if it has been seen, then show it, if allowed.
        //
        // Note that we use the level_ version. This is because you could have flames and a mob
        // on the same tile. We would only see the flames without this check.
        //
      } else if (is_falling) {
        //
        // The thing is not currently seen and has not been seen previously.
        // If the thing is falling, always show it in the background, else things appear to vanish.
        //
      } else {
        //
        // The thing is not currently seen and has not been seen previously. Hide it.
        //
        return;
      }
    }
  }

  auto *tile = tile_index_to_tile(tile_index);
  if (tile == nullptr) {
    return;
  }

  color fg = WHITE;
  float x1 = 0;
  float x2 = 0;
  float y1 = 0;
  float y2 = 0;
  tile_coords(tile, &x1, &y1, &x2, &y2);

  LightPixels *light_pixels = nullptr;

  if (is_level_select) {
    //
    // No lighting
    //
    fg = WHITE;
  } else if (thing_vision_can_see_tile(g, v, l, player, p)) {
    //
    // Apply lighting to current tiles
    //
    if (fbo == FBO_MAP_FG) {
      auto *pixel = &v->light_map.tile[ p.x ][ p.y ].pixels.pixel[ 0 ][ 0 ];
      fg.r        = std::max(pixel->r, pixel->player_r);
      fg.g        = std::max(pixel->g, pixel->player_g);
      fg.b        = std::max(pixel->b, pixel->player_b);

      //
      // Too many tiny tiles
      //
      if (! game_map_zoom_is_full_map_visible(g)) {
        light_pixels = &v->light_map.tile[ p.x ][ p.y ].pixels;
      }

      //
      // Can only light things that are exactly on pixel boundaries
      //
      if (! tp_is_blit_per_pixel_lighting(tp)) {
        light_pixels = nullptr;
      }

      if (tile_width(tile) != LIGHT_PIXEL) {
        light_pixels = nullptr;
      }
    } else {
      fg = WHITE;
    }
  } else {
    //
    // Default color, which might be monochrome for non visited tiles
    //
  }

  if (t_maybe_null != nullptr) {
    //
    // Handle various effects
    //
    if (is_falling) {
      thing_display_falling(g, v, l, p, tp, t_maybe_null, tl, br, tile, x1, x2, y1, y2, fbo, fg);
      return;
    }

    if (thing_is_water(t_maybe_null)) {
      switch (level_to_biome(g, v, l)) {
        case BIOME_DUNGEON :    break;
        case BIOME_BOGLAND :    fg = GREEN2; break;
        case BIOME_NETHERVOID : break;
        case BIOME_GRAVEYARD :  break;
        case BIOME_UNDERHELL :  break;
        case BIOME_NONE :       [[fallthrough]];
        case BIOME_ENUM_MAX :   break;
      }
    }

    if (thing_is_wall(t_maybe_null)) {
      switch (level_to_biome(g, v, l)) {
        case BIOME_DUNGEON :    break;
        case BIOME_BOGLAND :    fg = GREEN; break;
        case BIOME_NETHERVOID : break;
        case BIOME_GRAVEYARD :  break;
        case BIOME_UNDERHELL :  break;
        case BIOME_NONE :       [[fallthrough]];
        case BIOME_ENUM_MAX :   break;
      }
    }

    int submerged_pct = 0;
    if ((submerged_pct = thing_submerged_pct(t_maybe_null)) != 0) {
      //
      // Submerge the tile if it is over some kind of liquid.
      //
      if (submerged_pct != 0) {
        if (thing_is_reeds(t_maybe_null)) {
          thing_display_it(g, v, l, p, tp, t_maybe_null, tl, br, tile, x1, x2, y1, y2, fbo, fg, light_pixels);
        } else {
          tile_blit_apply_submerge_pct(g, tl, br, tile, x1, x2, y1, y2, thing_submerged_pct(t_maybe_null));
          thing_display_it(g, v, l, p, tp, t_maybe_null, tl, br, tile, x1, x2, y1, y2, fbo, fg, light_pixels);
        }

        //
        // Add a reflection
        //
        if (fbo != FBO_MAP_FG_OVERLAY) {
          fg.a /= 8;
          fg.r /= 2;
          fg.g /= 2;
          fg.b /= 2;
          light_pixels = nullptr;
          auto h       = br.y - tl.y;
          std::swap(br.y, tl.y);
          tl.y += h;
          br.y += h;
        }
      }
    }
  }

  thing_display_it(g, v, l, p, tp, t_maybe_null, tl, br, tile, x1, x2, y1, y2, fbo, fg, light_pixels);
}
