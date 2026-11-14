//
// Copyright goblinhack@gmail.com
//

#include "my_age_map_inlines.hpp"
#include "my_bpoint.hpp"
#include "my_callstack.hpp"
#include "my_color_defs.hpp"
#include "my_fov_map_inlines.hpp"
#include "my_game_defs.hpp"
#include "my_gl.hpp"
#include "my_globals.hpp"
#include "my_level.hpp"
#include "my_level_inlines.hpp"
#include "my_main.hpp"
#include "my_math.hpp"
#include "my_ptrcheck.hpp"
#include "my_random.hpp"
#include "my_spoint.hpp"
#include "my_tex.hpp"
#include "my_thing.hpp"
#include "my_thing_inlines.hpp"
#include "my_tile.hpp"
#include "my_time.hpp"
#include "my_tp.hpp"
#include "my_types.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <utility>
#include <vector>

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
  void render_small_lights(Gamep g, Levelsp v, Levelp l, FboEnum fbo_small_lights) const;

  //
  // This is how far the light rays reach
  //
  int ray_max_length_in_pixels {};

  //
  // What FBO to render to
  //
  FboEnum fbo {FBO_NONE};

  //
  // The length of each ray when it is cast
  //
  Ray rays[ LIGHT_MAX_RAYS_MAX ] {};

  //
  // The precalculated ray lines
  //
  std::array< std::vector< RayPixel >, LIGHT_MAX_RAYS_MAX > ray_pixels;

  Texp light_tex {};
  int  light_tex_id {};
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

void Raycast::ray_pixel_add(int16_t index, const spoint p0, const spoint p1)
{
  RayPixel ray;
  ray.p        = p1;
  ray.distance = DISTANCE_f(p0.x, p0.y, p1.x, p1.y);
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

static void level_raycast_light_tile(const bpoint &tile, ThingPlayerp light, ThingExtp ext)
{
  TRACE_DEBUG();

  //
  // Only apply color to the tile once
  //
  if (! fov_map_get(&light->is_lit, tile.x, tile.y)) {
    fov_map_set(&light->is_lit, tile.x, tile.y, 1U);
    age_map_set(&ext->has_seen, tile.x, tile.y, 1U);
    fov_map_set(&ext->can_see, tile.x, tile.y, 1U);
  }
}

static void level_raycast_get_extents(Gamep g, Levelsp v, Levelp l, Thingp player, int &start_ray, int &ray_count)
{
  start_ray = 0;
  ray_count = LIGHT_MAX_RAYS_MAX;

  //
  // Limit the vision if required
  //
  if (thing_is_vision_180_degrees(g, v, l, player)) {
    int const quad = LIGHT_MAX_RAYS_MAX / 8;

    ray_count = LIGHT_MAX_RAYS_MAX / 2;

    switch (player->dir) {
      case THING_DIR_BR :    start_ray = quad * 7; break;
      case THING_DIR_TR :    start_ray = quad * 5; break;
      case THING_DIR_BL :    start_ray = quad * 1; break;
      case THING_DIR_TL :    start_ray = quad * 3; break;
      case THING_DIR_LEFT :  start_ray = quad * 2; break;
      case THING_DIR_RIGHT : start_ray = quad * 6; break;
      case THING_DIR_DOWN :  start_ray = quad * 0; break;
      case THING_DIR_UP :    start_ray = quad * 4; break;
      case THING_DIR_NONE :
        start_ray = 0;
        ray_count = LIGHT_MAX_RAYS_MAX;
        break;
      default : break;
    }

    //
    // Human vision is 200 degrees, allegedly!
    //
    switch (player->dir) {
      case THING_DIR_BR :    [[fallthrough]];
      case THING_DIR_TR :    [[fallthrough]];
      case THING_DIR_BL :    [[fallthrough]];
      case THING_DIR_TL :    [[fallthrough]];
      case THING_DIR_LEFT :  [[fallthrough]];
      case THING_DIR_RIGHT : [[fallthrough]];
      case THING_DIR_DOWN :  [[fallthrough]];
      case THING_DIR_UP :
        {
          auto human_vision_extra = quad / 4;
          start_ray -= human_vision_extra;
          if (start_ray < 0) {
            start_ray += LIGHT_MAX_RAYS_MAX;
          }
          ray_count += human_vision_extra * 2;
        }
        break;
      case THING_DIR_NONE : break;
      default :             break;
    }
  }
}

void Raycast::raycast_do(Gamep g, Levelsp v, Levelp l)
{
  TRACE();

  auto *player = thing_player(g);
  if (player == nullptr) [[unlikely]] {
    return;
  }

  auto *ext = thing_ext_struct(g, v, player);
  if (ext == nullptr) [[unlikely]] {
    return;
  }

  auto *light = thing_player_struct(g);
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
  // Local cache of things blocking light. The rays only ever touch a fraction of the map (they stop at
  // walls), so work out what blocks light on demand rather than for every tile up front. Nothing
  // changes the level while the rays are walked, so the answer for a tile is stable.
  //
  Thingp  light_blocker_cache[ MAP_WIDTH ][ MAP_HEIGHT ];
  uint8_t light_blocker_cached[ MAP_WIDTH ][ MAP_HEIGHT ] = {};

  auto light_blocker_get = [ & ](const bpoint &at) -> Thingp {
    if (light_blocker_cached[ at.x ][ at.y ] == 0U) {
      light_blocker_cached[ at.x ][ at.y ] = 1U;
      light_blocker_cache[ at.x ][ at.y ]  = level_light_blocker_at(g, v, l, at, player);
    }
    return light_blocker_cache[ at.x ][ at.y ];
  };

  FovContext ctx;

  ctx.g                        = g;
  ctx.v                        = v;
  ctx.l                        = l;
  ctx.me                       = player;
  ctx.pov                      = thing_at(g, v, l, player);
  ctx.light_color              = tp_light_color(tp);
  ctx.thing_at_in_pixels       = thing_pix_at(player);
  ctx.light_strength_in_pixels = thing_is_light_source(g, v, player) * TILE_WIDTH;

  //
  // Flicker the light strength and color?
  //
  if (thing_is_light_flicker(player)) [[unlikely]] {
    auto      depth      = OS_RANDOM_RANGE(0, 10);
    int const pixel_step = 1 * depth;
    int const color_step = 2 * depth;

    if (std::cmp_greater(ctx.light_color.g, color_step)) {
      ctx.light_color.g -= color_step;
    }

    if (std::cmp_greater(ctx.light_color.b, color_step)) {
      ctx.light_color.b -= color_step;
    }

    if (ctx.light_strength_in_pixels > pixel_step) {
      ctx.light_strength_in_pixels -= pixel_step;
    }
  }

  //
  // Center the light on the player
  //
  spoint light_pos = thing_pix_at(player);

  light_pos.x += TILE_WIDTH / 2;
  light_pos.y += TILE_HEIGHT / 2;

  //
  // Limit the vision if required
  //
  int start_ray = 0;
  int ray_count = LIGHT_MAX_RAYS_MAX;
  level_raycast_get_extents(g, v, l, player, start_ray, ray_count);

  //
  // Walk the light rays in a circle. Find the nearest walls and then let
  // the light leak a little.
  //
  for (auto i = 0; i < ray_count; i++) {
    auto          r             = (start_ray + i) % LIGHT_MAX_RAYS_MAX;
    const int16_t end_of_points = static_cast< uint16_t >(ray_pixels[ r ].size() - 1);
    auto         *ray           = &rays[ r ];
    auto          ray_pixel     = ray_pixels[ r ].begin();
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

      level_raycast_light_tile(tile, light, ext);

      //
      // This is for foliage so we don't obscure too much where we stand
      //
      if (step < TILE_WIDTH / 2) {
        continue;
      }

      //
      // Once we hit an obstacle to vision, how far do we allow the ray of light to penetrate
      //
      auto obs_to_vision_start_distance = ray_pixel->distance;

      //
      // Keep track of the type of object we hit
      //
      Thingp obs_to_vision      = nullptr;
      Tpp    tp_obs_to_vision   = nullptr;
      Thingp next_obs_to_vision = nullptr;

      //
      // Did the light ray hit an obstacle?
      //
      obs_to_vision = light_blocker_get(tile);
      if (obs_to_vision != nullptr) {
        //
        // What type of obstacle?
        //
        tp_obs_to_vision = thing_tp(obs_to_vision);

        auto obs_to_vision_penetration_distance = obs_to_vision_start_distance + tp_distance_light_penetration_pixels_get(tp_obs_to_vision);

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
          next_obs_to_vision = light_blocker_get(tile);
          if (next_obs_to_vision == nullptr) {
            break;
          }

          //
          // If we've hit a different type of object, e.g. wall versus rock
          //
          if (thing_tp(next_obs_to_vision) != tp_obs_to_vision) {
            break;
          }

          level_raycast_light_tile(tile, light, ext);
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
  fbo_get_size(g, fbo, w, h);
  gl_enter_2d_mode(g, w, h);

  blit_fbo_bind(fbo);
  {
    glBlendFunc(GL_ONE, GL_ZERO);
    gl_clear();

    blit_init();

    //
    // Limit the vision if required
    //
    int start_ray = 0;
    int ray_count = LIGHT_MAX_RAYS_MAX;
    level_raycast_get_extents(g, v, l, player, start_ray, ray_count);

    //
    // Walk the light rays in a circle.
    //
    PUSH_POINT(light_pos.x, light_pos.y);

    for (auto i = 0; i < ray_count; i++) {
      auto r = (start_ray + i) % LIGHT_MAX_RAYS_MAX;

      auto         *ray = &rays[ r ];
      spoint const &p   = ray_pixels[ r ][ ray->depth_furthest ].p;
      int16_t const p1x = light_pos.x + p.x;
      int16_t const p1y = light_pos.y + p.y;
      PUSH_POINT(p1x, p1y);
    }

    if (thing_is_vision_180_degrees(g, v, l, player)) {
      //
      // Push the same point as not a full circle
      //
      auto i = ray_count - 1;
      auto r = (start_ray + i) % LIGHT_MAX_RAYS_MAX;

      auto         *ray = &rays[ r ];
      spoint const &p   = ray_pixels[ r ][ ray->depth_furthest ].p;
      int16_t const p1x = light_pos.x + p.x;
      int16_t const p1y = light_pos.y + p.y;
      PUSH_POINT(p1x, p1y);
    } else {
      //
      // Complete the circle with the first point again.
      //
      auto          r   = start_ray;
      auto         *ray = &rays[ r ];
      spoint const &p   = ray_pixels[ r ][ ray->depth_furthest ].p;
      int16_t const p1x = light_pos.x + p.x;
      int16_t const p1y = light_pos.y + p.y;
      PUSH_POINT(p1x, p1y);
    }

    blit_flush_triangle_fan();
  }

  blit_fbo_unbind();

  blit_fbo_bind(FBO_MAP_LIGHT);
  {
    spoint const tl(light_pos.x - ray_max_length_in_pixels, light_pos.y - ray_max_length_in_pixels);
    spoint const br(light_pos.x + ray_max_length_in_pixels, light_pos.y + ray_max_length_in_pixels);

    glBlendFunc(GL_ONE, GL_ZERO);
    blit_init();
    blit(light_tex_id, tl.x, tl.y, br.x, br.y, WHITE);
    blit_flush();

    glBlendFunc(GL_DST_ALPHA, GL_DST_ALPHA);
    blit_fbo(g, fbo, RED);
  }
  blit_fbo_unbind();
}

//
// All light from all light sources, combined.
//
void Raycast::render_small_lights(Gamep g, Levelsp v, Levelp l, FboEnum fbo_small_lights) const
{
  TRACE();

  int w = 0;
  int h = 0;
  fbo_get_size(g, fbo_small_lights, w, h);

  gl_enter_2d_mode(g, w, h);
  glBlendFunc(GL_SRC_ALPHA_SATURATE, GL_ONE_MINUS_CONSTANT_ALPHA);

  blit_fbo_bind(fbo_small_lights);
  {
    blit_init();

    //
    // Calculate all lit tiles for non player things
    //
    FOR_ALL_THINGS_ON_LEVEL_NO_BREAK(g, v, l, t)
    {
      int const max_radius = thing_is_light_source(g, v, t);
      if (max_radius == 0) [[likely]] {
        continue;
      }

      if (thing_is_player(t)) [[unlikely]] {
        continue;
      }

      if (thing_is_dead(t)) [[unlikely]] {
        continue;
      }

      auto light_pos = thing_pix_at(t);
      light_pos.x += TILE_WIDTH / 2;
      light_pos.y += TILE_HEIGHT / 2;

      auto light_color = tp_light_color(thing_tp(t));

      if (t->light_radius_in_pixels == 0U) {
        t->light_radius_in_pixels = thing_is_light_source(g, v, t) * TILE_WIDTH;
      }

      //
      // Flicker the light strength?
      //
      if (thing_is_light_flicker(t)) [[unlikely]] {
        if (game_time_have_x_hundredths_passed_since(THING_LIGHT_FLICKER_ANIM_MS, t->last_flicker)) {
          t->light_radius_in_pixels = (thing_is_light_source(g, v, t) * TILE_WIDTH) + OS_RANDOM_RANGE(0, 10);
          t->last_flicker           = game_time_ms_cached();
        }
      }

      auto         rad = t->light_radius_in_pixels;
      spoint const tl(light_pos.x - rad, light_pos.y - rad);
      spoint const br(light_pos.x + rad, light_pos.y + rad);

      blit(light_tex_id, tl.x, tl.y, br.x, br.y, light_color);
    }
    blit_flush();
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

  l->fbo          = fbo;
  l->light_tex    = tex_load("", "light", GL_LINEAR);
  l->light_tex_id = tex_get_gl_binding(l->light_tex);

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
    auto ray_max_length_in_pixels = thing_distance_vision(g, v, l, player) * TILE_WIDTH;

    player_raycast = raycast_new(ray_max_length_in_pixels, fbo);
    if (player_raycast == nullptr) {
      return;
    }
    player_raycast->ray_lengths_precalculate(g, v, l);
  }

  player_raycast->raycast_do(g, v, l);

  if (! g_opt_tests) {
    player_raycast->render_small_lights(g, v, l, FBO_MAP_LIGHT);
  }
  // sdl_fbo_dump(g, FBO_MAP_LIGHT, "FBO_MAP_LIGHT");
}

void level_raycast_init(Gamep g) { TRACE(); }

void level_raycast_fini()
{
  TRACE();
  delete player_raycast;
  player_raycast = nullptr;
}
