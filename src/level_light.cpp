//
// Copyright goblinhack@gmail.com
//

#include "my_bpoint.hpp"
#include "my_callstack.hpp"
#include "my_game_defs.hpp"
#include "my_gl.hpp"
#include "my_level.hpp"
#include "my_math.hpp"
#include "my_random.hpp"
#include "my_thing.hpp"
#include "my_thing_inlines.hpp"
#include "my_tile.hpp"
#include "my_tp.hpp"
#include "my_types.hpp"

#include <algorithm>
#include <cstdint>
#include <cstring>
#include <thread>
#include <vector>

static float light_fade[ MAP_WIDTH ];

void level_light_precalculate(Gamep g)
{
  static const char light_fade_map[]
      = "x                                               " // light strength
        " x                                              " //       |
        "  x                                             " //       |
        "   x                                            " //       |
        "    x                                           " //       |
        "     x                                          " //       |
        "      x                                         " //       |
        "       x                                        " //       |
        "        x                                       " //       |
        "         x                                      " //       |
        "          x                                     " //       |
        "           x                                    " //       |
        "            x                                   " //       |
        "             x                                  " //       |
        "              x                                 " //       |
        "               x                                " //       |
        "                x                               " //       |
        "                 x                              " //       |
        "                  x                             " //       |
        "                   x                            " //       |
        "                    x                           " //       |
        "                     x                          " //       |
        "                      x                         " //       |
        "                       x                        " //       |
        "                        x                       " //       |
        "                         x                      " //       |
        "                          x                     " //       |
        "                           x                    " //       |
        "                            x                   " //       |
        "                             x                  " //       |
        "                              x                 " //       |
        "                               x                " //       |
        "                                x               " //       |
        "                                 x              " //       |
        "                                  x             " //       |
        "                                   x            " //       |
        "                                    x           " //       |
        "                                     x          " //       |
        "                                      x         " //       |
        "                                       x        " //       |
        "                                        x       " //       |
        "                                         x      " //       |
        "                                          x     " //       |
        "                                           x    " //       |
        "                                            x   " //       |
        "                                             x  " //       |
        "                                              x " //       |
        "                                               x" //       v
      ;

  for (auto x = 0; x < MAP_WIDTH; x++) {
    for (auto y = 0; y < MAP_HEIGHT; y++) {
      auto c = light_fade_map[ (MAP_WIDTH * y) + x ];
      if (c == 'x') {
        if (light_fade[ x ] == 0) {
          light_fade[ x ] = 1.0F - (static_cast< float >(y) / static_cast< float >(MAP_HEIGHT));
        }
      }
    }
  }
}

//
// Light the pixels of a tile
//
static inline void level_light_per_pixel(const FovContext &ctx, const bpoint &p)
{
  TRACE_DEBUG();

  auto *const light_tile = &ctx.v->light_map.tile[ p.x ][ p.y ];
  float const col_r      = ctx.light_color.r;
  float const col_g      = ctx.light_color.g;
  float const col_b      = ctx.light_color.b;

  uint16_t light_pixel_at_y = (p.y * TILE_WIDTH) - (TILE_WIDTH / 2);
  for (uint8_t pixy = 0; pixy < LIGHT_PIXEL; pixy++, light_pixel_at_y++) {

    uint16_t light_pixel_at_x = (p.x * TILE_WIDTH) - (TILE_WIDTH / 2);
    for (uint8_t pixx = 0; pixx < LIGHT_PIXEL; pixx++, light_pixel_at_x++) {

      float const dist_in_pixels = DISTANCE_f(light_pixel_at_x, light_pixel_at_y, // newline
                                              (float) ctx.thing_at_in_pixels.x, (float) ctx.thing_at_in_pixels.y);

      //
      // No point in calculating beyond the maximum light distance.
      //
      if (dist_in_pixels >= ctx.light_strength_in_pixels) [[unlikely]] {
        continue;
      }

      auto light_fade_index
          = static_cast< uint8_t >(static_cast< int >((dist_in_pixels / ctx.light_strength_in_pixels) * static_cast< float >(MAP_WIDTH)));

#ifdef DEBUG_BUILD
      //
      // Should not happen
      //
      if ((light_fade_index >= MAP_WIDTH)) [[unlikely]] {
        light_fade_index = MAP_WIDTH - 1;
        CROAK("unexpected overflow");
      }
#endif

      auto       *light_pixel = &light_tile->pixels.pixel[ pixx ][ pixy ];
      float const fade        = light_fade[ light_fade_index ];

      light_pixel->r = std::min(255, static_cast< int >(light_pixel->r + (fade * col_r)));
      light_pixel->g = std::min(255, static_cast< int >(light_pixel->g + (fade * col_g)));
      light_pixel->b = std::min(255, static_cast< int >(light_pixel->b + (fade * col_b)));
    }
  }
}

//
// All light from all light sources, combined.
//
static void level_light_calculate_all_things(Gamep g, Levelsp v, Levelp l)
{
  TRACE();

  //
  // Calculate all lit tiles for non player things
  //
  FOR_ALL_THINGS_ON_LEVEL(g, v, l, t)
  {
    int max_radius = thing_is_light_source(t);
    if (max_radius == 0) [[likely]] {
      continue;
    }

    if (thing_is_player(t)) [[unlikely]] {
      continue;
    }

    auto *light = thing_light_struct(g, t);

    //
    // If a monster is moving between two tiles, as the lighting is tile based,
    // we will end up calculating the light at the new tile. However, the
    // monster is moving pixel at a time from the old tile. This means some of
    // the tiles at the limits of the lighting will end up out of range and black.
    // This results in trailing dark squares. Fix this by adding some extra to
    // the radius to account for the move interpolation.
    //
    if (thing_is_moving(t)) [[unlikely]] {
      max_radius += 2;
    }

    FovContext ctx;

    ctx.g                        = g;
    ctx.v                        = v;
    ctx.l                        = l;
    ctx.me                       = t;
    ctx.pov                      = thing_at(g, v, l, t);
    ctx.thing_at_in_pixels       = thing_pix_at(t);
    ctx.max_radius               = max_radius;
    ctx.can_see_tile             = &light->is_lit;
    ctx.has_seen_tile            = nullptr;
    ctx.light_walls              = true;
    ctx.light_color              = tp_light_color(thing_tp(t));
    ctx.light_strength_in_pixels = thing_is_light_source(t) * TILE_WIDTH;

    //
    // Flicker the light strength?
    //
    if (thing_is_light_flicker(t)) [[unlikely]] {
      auto      depth      = OS_RANDOM_RANGE(0, 10);
      int const pixel_step = 1 * depth;

      if (ctx.light_strength_in_pixels > pixel_step) {
        ctx.light_strength_in_pixels -= pixel_step;
      }
    }

    ctx.can_see_callback = level_light_per_pixel;

    level_fov(ctx);
  }
}

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
  if (l->level_num != player->level_num) {
    return;
  }

  //
  // Clear the pixel lighting buffer
  //
  v->light_map = {};

  //
  // Calculate thing lighting
  //
  std::vector< std::thread > threads;

  threads.emplace_back(level_light_calculate_all_things, g, v, l);

  //
  // Calculate player lighting while we wait
  //
  level_raycast(g, v, l, FBO_MAP_LIGHT);

  for (auto &t : threads) {
    t.join();
  }
}
