//
// Copyright goblinhack@gmail.com
//

#include "my_age_map_inlines.hpp"
#include "my_callstack.hpp"
#include "my_fov_map_inlines.hpp"
#include "my_globals.hpp"
#include "my_level.hpp"
#include "my_level_inlines.hpp"
#include "my_level_light_inlines.hpp"
#include "my_main.hpp"
#include "my_math.hpp"
#include "my_thing_inlines.hpp"
#include "my_tile.hpp"
#include "my_ui.hpp"

#include <array>
#include <cmath>
#include <cstring>

using Ray = struct Ray {
  int16_t depth_furthest;
};

using RayPixel = struct RayPixel {
  spoint p;
  float  distance;
};

class Raycast
{
public:
  Raycast();
  ~Raycast();

  void ray_pixel_line_draw(int16_t index, spoint p0, spoint p1);
  void ray_pixel_add(int16_t index, spoint p0, spoint p1);
  void ray_lengths_precalculate(Gamep g, Levelsp v, Levelp l);
  void raycast_do(Gamep g, Levelsp v, Levelp l);
  void raycast_render(Gamep g, Levelsp v, Levelp l);

  //
  // This is how far the light rays reach
  //
  int ray_max_length_in_pixels {};

  //
  // What FBO to render to
  //
  FboEnum fbo {FBO_NONE};

  //
  // The lenght of each ray when it is cast
  //
  Ray rays[ LIGHT_MAX_RAYS_MAX ] {};

  //
  // The precalculated ray lines
  //
  std::array< std::vector< RayPixel >, LIGHT_MAX_RAYS_MAX > ray_pixels;

  std::vector< float > ray_gl_cmds;
};

static Raycast *player_raycast;

Raycast::Raycast()
{
  TRACE();

  NEWPTR(MTYPE_LIGHT, this, "Raycast");
}

Raycast::~Raycast()
{
  TRACE();

  OLDPTR(MTYPE_LIGHT, this);
}

static float player_light_fade[ MAP_WIDTH ];

static void level_raycast_precalculate(Gamep g)
{
  static const char player_light_fade_map[]
      = "xx                                              " // light strength
        "  xx                                            " //       |
        "    x                                           " //       |
        "     x                                          " //       |
        "      x                                         " //       |
        "       x                                        " //       |
        "        x                                       " //       |
        "        x                                       " //       |
        "         x                                      " //       |
        "         x                                      " //       |
        "          x                                     " //       |
        "          x                                     " //       |
        "           x                                    " //       |
        "           x                                    " //       |
        "            x                                   " //       |
        "            x                                   " //       |
        "             x                                  " //       |
        "             x                                  " //       |
        "              x                                 " //       |
        "              x                                 " //       |
        "               x                                " //       |
        "               x                                " //       |
        "               x                                " //       |
        "                x                               " //       |
        "                x                               " //       |
        "                x                               " //       |
        "                 x                              " //       |
        "                 x                              " //       |
        "                 x                              " //       |
        "                 x                              " //       |
        "                 x                              " //       |
        "                  x                             " //       |
        "                  x                             " //       |
        "                  x                             " //       |
        "                   x                            " //       |
        "                   x                            " //       |
        "                   x                            " //       |
        "                    x                           " //       |
        "                    x                           " //       |
        "                     x                          " //       |
        "                     x                          " //       |
        "                      x                         " //       |
        "                       x                        " //       |
        "                        x                       " //       |
        "                         x                      " //       |
        "                          xx                    " //       |
        "                            xxxxxxxxxxxxxxxxxxxx" //       |
        "                                                " //       v
      ;

  for (auto x = 0; x < MAP_WIDTH; x++) {
    for (auto y = 0; y < MAP_HEIGHT; y++) {
      {
        auto c = player_light_fade_map[ (MAP_WIDTH * y) + x ];
        if (c == 'x') {
          if (player_light_fade[ x ] == 0) {
            player_light_fade[ x ] = 1.0F - (static_cast< float >(y) / static_cast< float >(MAP_HEIGHT));
          }
        }
      }
    }
  }
}

void Raycast::ray_pixel_add(int16_t index, const spoint p0, const spoint p1)
{
  RayPixel ray;
  ray.p        = p1;
  ray.distance = DISTANCEf(p0.x, p0.y, p1.x, p1.y);
  ray_pixels[ index ].push_back(ray);
}

// http://www.edepot.com/linee.html
void Raycast::ray_pixel_line_draw(int16_t index, const spoint p0, const spoint p1)
{
  ray_pixels[ index ].resize(0);

  const spoint start = p0;
  auto         x     = p0.x;
  auto         y     = p0.y;
  auto         x2    = p1.x;
  auto         y2    = p1.y;

  bool yLonger  = false;
  int  shortLen = y2 - y;
  int  longLen  = x2 - x;

  if (abs(shortLen) > abs(longLen)) {
    int const swap = shortLen;
    shortLen       = longLen;
    longLen        = swap;
    yLonger        = true;
  }
  int decInc = 0;
  if (longLen == 0) {
    decInc = 0;
  } else {
    decInc = (shortLen << 16) / longLen;
  }

  if (yLonger) {
    if (longLen > 0) {
      longLen += y;
      for (int j = 0x8000 + (x << 16); y <= longLen; ++y) {
        ray_pixel_add(index, start, spoint(j >> 16, y));
        j += decInc;
      }
      return;
    }
    longLen += y;
    for (int j = 0x8000 + (x << 16); y >= longLen; --y) {
      ray_pixel_add(index, start, spoint(j >> 16, y));
      j -= decInc;
    }
    return;
  }

  if (longLen > 0) {
    longLen += x;
    for (int j = 0x8000 + (y << 16); x <= longLen; ++x) {
      ray_pixel_add(index, start, spoint(x, j >> 16));
      j += decInc;
    }
    return;
  }
  longLen += x;
  for (int j = 0x8000 + (y << 16); x >= longLen; --x) {
    ray_pixel_add(index, start, spoint(x, j >> 16));
    j -= decInc;
  }
}

//
// Generate the right ray lengths.
//
void Raycast::ray_lengths_precalculate(Gamep g, Levelsp v, Levelp l)
{
  TRACE();

  float const dr = static_cast< float > RAD_360 / (static_cast< float >(LIGHT_MAX_RAYS_MAX));
  for (int i = 0; i < LIGHT_MAX_RAYS_MAX; i++) {
    float cosr = 0;
    float sinr = 0;
    SINCOSF(dr * (float) i, &sinr, &cosr);
    ray_pixel_line_draw(i, spoint(0, 0),
                        spoint(static_cast< int >(static_cast< float >(ray_max_length_in_pixels) * cosr),
                               static_cast< int >(static_cast< float >(ray_max_length_in_pixels) * sinr)));
  }
}

//
// A player specific version of this function.
//
static inline void level_light_per_pixel_player(const FovContext &ctx, const bpoint &p)
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

      float const dist_in_pixels = DISTANCEf(light_pixel_at_x, light_pixel_at_y, // newline
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
      float const fade        = player_light_fade[ light_fade_index ];

      light_pixel->player_r = std::min(255, static_cast< int >(light_pixel->player_r + (fade * col_r)));
      light_pixel->player_g = std::min(255, static_cast< int >(light_pixel->player_g + (fade * col_g)));
      light_pixel->player_b = std::min(255, static_cast< int >(light_pixel->player_b + (fade * col_b)));
    }
  }
}

static void level_raycast_light_tile(const FovContext &ctx, const bpoint &tile, ThingLightp light, ThingExtp ext)
{
  TRACE_DEBUG();

  //
  // Only apply color to the tile once
  //
  if (! fov_map_get(&light->is_lit, tile.x, tile.y)) {
    fov_map_set(&light->is_lit, tile.x, tile.y, 1U);
    age_map_set(&ext->has_seen, tile.x, tile.y, 1U);
    fov_map_set(&ext->can_see, tile.x, tile.y, 1U);
    level_light_per_pixel_player(ctx, tile);
  }
}

void Raycast::raycast_do(Gamep g, Levelsp v, Levelp l)
{
  TRACE();

  auto *player = thing_player(g);
  if (player == nullptr) [[unlikely]] {
    return;
  }

  auto *ext = thing_ext_struct(g, player);
  if (ext == nullptr) [[unlikely]] {
    return;
  }

  auto *light = thing_light_struct(g, player);
  if (light == nullptr) {
    return;
  }

  //
  // Reset the fov ray lengths. I don't think we need this.
  //
  //  memset(rays, 0, sizeof(rays));

  //
  // Reset what we can see
  //
  light->is_lit = {};
  ext->can_see  = {};

  //
  // The light source
  //
  const int tile_w = TILE_WIDTH;
  const int tile_h = TILE_HEIGHT;

  auto *tp = thing_tp(player);

  //
  // Local cache of things blocking light
  //
  Thingp light_blocker_cache[ MAP_WIDTH ][ MAP_HEIGHT ];

  for (auto x = 0; x < MAP_WIDTH; x++) {
    for (auto y = 0; y < MAP_HEIGHT; y++) {
      light_blocker_cache[ x ][ y ] = level_light_blocker_at(g, v, l, bpoint(x, y), player);
    }
  }

  FovContext ctx;

  ctx.g                        = g;
  ctx.v                        = v;
  ctx.l                        = l;
  ctx.me                       = player;
  ctx.pov                      = thing_at(player);
  ctx.light_color              = tp_light_color(tp);
  ctx.light_strength_in_pixels = thing_is_light_source(player) * TILE_WIDTH;
  ctx.thing_at_in_pixels       = thing_pix_at(player);

  //
  // Center the light on the player
  //
  spoint light_pos = thing_pix_at(player);

  light_pos.x += TILE_WIDTH / 2;
  light_pos.y += TILE_HEIGHT / 2;

  //
  // Walk the light rays in a circle. Find the nearest walls and then let
  // the light leak a little.
  //
  for (auto i = 0; i < LIGHT_MAX_RAYS_MAX; i++) {
    const int16_t end_of_points = static_cast< uint16_t >(ray_pixels[ i ].size() - 1);
    auto         *ray           = &rays[ i ];
    auto          ray_pixel     = ray_pixels[ i ].begin();
    int16_t       step          = 0;
    uint8_t       prev_tile_x   = -1;
    uint8_t       prev_tile_y   = -1;

    for (;; step++) {
      //
      // oob?
      //
      if ((step >= end_of_points)) [[unlikely]] {
        break;
      }

      //
      // Beyond vision limits?
      //
      if ((ray_pixel->distance > ray_max_length_in_pixels)) [[unlikely]] {
        break;
      }

      int16_t p1x    = light_pos.x + ray_pixel->p.x;
      int16_t p1y    = light_pos.y + ray_pixel->p.y;
      uint8_t tile_x = p1x / tile_w;
      uint8_t tile_y = p1y / tile_h;
      ray_pixel++;

      //
      // Ignore the same tile. i.e. do not light it more than once.
      //
      [[likely]] if (((tile_x == prev_tile_x) && (tile_y == prev_tile_y))) {
        continue;
      }

      //
      // Oob can happen in custom levels with no edges
      //
      bpoint tile(tile_x, tile_y);
      if (is_oob(tile)) [[unlikely]] {
        break;
      }

      prev_tile_x = tile_x;
      prev_tile_y = tile_y;

      level_raycast_light_tile(ctx, tile, light, ext);

      //
      // This is for foliage so we don't obscure too much where we stand
      //
      if (step < TILE_WIDTH / 2) {
        continue;
      }

      //
      // Once we hit an obstacle to vision, how far do we allow the ray of light to penetrate
      //
      auto obs_to_vision_start_distance       = ray_pixel->distance;
      auto obs_to_vision_penetration_distance = (obs_to_vision_start_distance + static_cast< float >(TILE_WIDTH)) - 2;

      //
      // Keep track of the type of object we hit
      //
      Thingp obs_to_vision      = nullptr;
      Tpp    tp_obs_to_vision   = nullptr;
      Thingp next_obs_to_vision = nullptr;

      //
      // Did the light ray hit an obstacle?
      //
      obs_to_vision = light_blocker_cache[ tile.x ][ tile.y ];
      if (obs_to_vision != nullptr) {
        //
        // What type of obstacle?
        //
        tp_obs_to_vision = thing_tp(obs_to_vision);

        //
        // We hit a wall. Keep walking until we exit the wall or we reach the light limit.
        //
        int16_t step_inside_wall = step;

        for (;;) {
          //
          // oob?
          //
          if ((step_inside_wall >= end_of_points)) [[unlikely]] {
            break;
          }

          //
          // Check if we've progressed far enough into the wall
          //
          if ((ray_pixel->distance > obs_to_vision_penetration_distance)) [[unlikely]] {
            break;
          }

          p1x    = light_pos.x + ray_pixel->p.x;
          p1y    = light_pos.y + ray_pixel->p.y;
          tile_x = p1x / tile_w;
          tile_y = p1y / tile_h;
          ray_pixel++;

          //
          // Ignore the same tile. i.e. do not light it more than once.
          //
          [[likely]] if (((tile_x == prev_tile_x) && (tile_y == prev_tile_y))) {
            step_inside_wall++;
            continue;
          }

          //
          // Oob can happen in custom levels with no edges
          //
          tile.x = tile_x;
          tile.y = tile_y;
          if (is_oob(tile)) [[unlikely]] {
            break;
          }

          prev_tile_x = tile_x;
          prev_tile_y = tile_y;

          //
          // If we've left the wall, we're done
          //
          next_obs_to_vision = light_blocker_cache[ tile.x ][ tile.y ];
          if (next_obs_to_vision == nullptr) {
            break;
          }

          //
          // If we've hit a different type of object, e.g. wall versus rock
          //
          if (thing_tp(next_obs_to_vision) != tp_obs_to_vision) {
            break;
          }

          level_raycast_light_tile(ctx, tile, light, ext);
          step_inside_wall++;
        }

        //
        // This is how far the light travelled into the wall
        //
        step = step_inside_wall;
        break;
      }
    }

    ray->depth_furthest = step;
  }

  //
  // Indicate that we need to generate new triangle gans
  //
  raycast_render(g, v, l);
}

//
// Re-generate triangle fans and render to the FBO
//
void Raycast::raycast_render(Gamep g, Levelsp v, Levelp l)
{
  TRACE();

  if (g_opt_tests) {
    return;
  }

  auto *player = thing_player(g);
  if (player == nullptr) [[unlikely]] {
    return;
  }

  spoint light_pos = thing_pix_at(player);

  light_pos.x += TILE_WIDTH / 2;
  light_pos.y += TILE_HEIGHT / 2;

  int w = 0;
  int h = 0;
  fbo_get_size(g, FBO_MAP_LIGHT, w, h);
  gl_enter_2d_mode(g, w, h);

  blit_fbo_bind(fbo);
  {
    glBlendFunc(GL_ONE, GL_ZERO);
    gl_clear();

    blit_init();

    //
    // Walk the light rays in a circle.
    //
    PUSH_POINT(light_pos.x, light_pos.y);

    for (auto i = 0; i < LIGHT_MAX_RAYS_MAX; i++) {
      auto         *ray = &rays[ i ];
      spoint const &p   = ray_pixels[ i ][ ray->depth_furthest ].p;
      int16_t const p1x = light_pos.x + p.x;
      int16_t const p1y = light_pos.y + p.y;
      PUSH_POINT(p1x, p1y);
    }

    //
    // Complete the circle with the first point again.
    //
    {
      auto         *ray = &rays[ 0 ];
      spoint const &p   = ray_pixels[ 0 ][ ray->depth_furthest ].p;
      int16_t const p1x = light_pos.x + p.x;
      int16_t const p1y = light_pos.y + p.y;
      PUSH_POINT(p1x, p1y);
    }

    auto sz = bufp - gl_array_buf;
    ray_gl_cmds.resize(sz);
    std::copy(gl_array_buf, bufp, ray_gl_cmds.begin());
    blit_flush_triangle_fan();
  }
  blit_fbo_unbind();
}

static auto raycast_new(int ray_max_length_in_pixels, FboEnum fbo) -> Raycast *
{
  TRACE();

  auto *l = new Raycast();

  l->ray_max_length_in_pixels = ray_max_length_in_pixels;
  if (ray_max_length_in_pixels == 0) {
    CROAK("no light power");
  }

  l->fbo = fbo;

  return l;
}

void level_raycast(Gamep g, Levelsp v, Levelp l, FboEnum fbo)
{
  //
  // Do not generate open gl stuff on any other thread
  //
  if (g_thread_id != MAIN_THREAD) {
    return;
  }

  if ((g == nullptr) || (v == nullptr) || (l == nullptr)) {
    return;
  }

  auto *player = thing_player(g);
  if (player == nullptr) [[unlikely]] {
    return;
  }

  //
  // First time init
  //
  if (player_raycast == nullptr) {
    //
    // This is how far the light rays reach
    //
    auto ray_max_length_in_pixels = thing_distance_vision(player) * TILE_WIDTH;

    player_raycast = raycast_new(ray_max_length_in_pixels, fbo);
    if (player_raycast == nullptr) {
      return;
    }
    player_raycast->ray_lengths_precalculate(g, v, l);
  }

  player_raycast->raycast_do(g, v, l);
}

void level_raycast_init(Gamep g)
{
  TRACE();
  level_raycast_precalculate(g);
}

void level_raycast_fini()
{
  TRACE();
  delete player_raycast;
  player_raycast = nullptr;
}
