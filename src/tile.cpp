//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_color_defs.hpp"
#include "my_cpp_template.hpp"
#include "my_game.hpp"
#include "my_globals.hpp"
#include "my_level.hpp"
#include "my_main.hpp"
#include "my_pixel.hpp"
#include "my_ptrcheck.hpp"
#include "my_sdl_proto.hpp"
#include "my_size.hpp"
#include "my_string.hpp"
#include "my_tex.hpp"
#include "my_tile.hpp"

#include <cmath>

#include <array>
#ifdef WRITE_TILED
#include <libgen.h>
#endif
#include <vector>

static std::unordered_map< std::string, class Tile * > all_tiles;
static std::vector< class Tile * >                     all_tiles_array;

static bool tile_init_done;

class Tile
{
public:
  Tile();
  ~Tile();
  explicit Tile(const class Tile *tile);

  std::string name;

  //
  // Grabbed by a template
  //
  uint8_t in_use {};
  int     global_index {};

  //
  // Index within the overall texture, left to right, top to bottom.
  //
  uint16_t index {};

  int pix_width {};
  int pix_height {};

  //
  // Texture co-ordinates within the image.
  //
  float x1 {};
  float y1 {};
  float x2 {};
  float y2 {};

  //
  // Percentage points that indicate the start of the pixels within the tile
  // texture for use in collisions.
  //
  float px1 {};
  float py1 {};
  float px2 {};
  float py2 {};

  class Tex *tex {};
  class Tex *tex_monochrome {};
  class Tex *tex_mask {};
  class Tex *tex_outline {};

  uint8_t pix[ TILE_WIDTH_MAX ][ TILE_HEIGHT_MAX ] {};

  //
  // Delay in ms between frames.
  //
  uint32_t delay_ms {};

  uint8_t frame {};
  uint8_t dir {};

  bool internal_has_dir_anim     : 1 {};
  bool is_alive_on_end_of_anim   : 1 {};
  bool is_cleanup_on_end_of_anim : 1 {};
  bool is_end_of_anim            : 1 {};
  bool is_outline                : 1 {};
  bool is_loggable               : 1 {};

private:
  int _gl_binding {};
  int _gl_binding_monochrome {};
  int _gl_binding_mask {};
  int _gl_binding_outline {};

public:
  [[nodiscard]] auto gl_binding() const -> int;
  [[nodiscard]] auto gl_binding_monochrome() const -> int;
  [[nodiscard]] auto gl_binding_mask() const -> int;
  [[nodiscard]] auto gl_binding_outline() const -> int;

  void set_gl_binding(int v);
  void set_gl_binding_monochrome(int v);
  void set_gl_binding_mask(int v);
  void set_gl_binding_outline(int v);
};

Tile::Tile() { NEWPTR(MTYPE_TILE, this, "Tile"); }

Tile::~Tile() { OLDPTR(MTYPE_TILE, this); }

auto tile_init() -> bool
{
  TRACE();

  tile_init_done = true;

  return true;
}

void tile_fini()
{
  TRACE();

  if (tile_init_done) {
    tile_init_done = false;
  }

  for (auto &t : all_tiles) {
    VERIFY(MTYPE_TILE, t.second);
    delete t.second;
  }

  all_tiles.clear();
  all_tiles_array.clear();
}

Tile::Tile(const class Tile *tile)
    :                                                             //
      global_index(all_tiles_array.size() + 1),                   //
      pix_width(tile->pix_width),                                 //
      pix_height(tile->pix_height),                               //
      x1(tile->x1),                                               //
      y1(tile->y1),                                               //
      x2(tile->x2),                                               //
      y2(tile->y2),                                               //
      tex(tile->tex),                                             //
      tex_monochrome(tile->tex_monochrome),                       //
      tex_mask(tile->tex_mask),                                   //
      tex_outline(tile->tex_outline),                             //
      delay_ms(tile->delay_ms),                                   //
      dir(tile->dir),                                             //
      internal_has_dir_anim(tile->internal_has_dir_anim),         //
      is_alive_on_end_of_anim(tile->is_alive_on_end_of_anim),     //
      is_cleanup_on_end_of_anim(tile->is_cleanup_on_end_of_anim), //
      is_end_of_anim(tile->is_end_of_anim),                       //
      is_loggable(tile->is_loggable)                              //
{
  TRACE();

  NEWPTR(MTYPE_TILE, this, "tile copy");

#ifdef ENABLE_TILE_BOUNDS
  px1 = tile->px1;
  py1 = tile->py1;
  px2 = tile->px2;
  py2 = tile->py2;
#endif

  set_gl_binding(tile->gl_binding());
  set_gl_binding_monochrome(tile->gl_binding_monochrome());
  set_gl_binding_mask(tile->gl_binding_mask());
  set_gl_binding_outline(tile->gl_binding_outline());

  memcpy(pix, tile->pix, sizeof(pix));

  name = tile->name + " " + std::to_string(global_index);

  auto result = all_tiles.insert(std::make_pair(name, this));
  if (! result.second) {
    err("tile copy insert name [%s] failed", name.c_str());
  }
  all_tiles_array.push_back(this);
}

void tile_load_arr(const char *file, const char *alias, uint32_t width, uint32_t height, int nargs, const char *arr[])
{
  TRACE();

  Texp tex = tex_load(file, alias, GL_NEAREST);

  float const fw = static_cast< float >(1.0) / (((static_cast< float >(tex_get_width(tex)))) / ((static_cast< float >(width))));
  float const fh = static_cast< float >(1.0) / (((static_cast< float >(tex_get_height(tex)))) / ((static_cast< float >(height))));

  int x   = 0;
  int y   = 0;
  int idx = 0;

  isize pixel_size;

  pixel_size.w = width;
  pixel_size.h = height;

  while (nargs-- > 0) {
    std::string const name = arr[ idx++ ];

    if (! name.empty()) {
      if (tile_find(name) != nullptr) {
        CROAK("tile name [%s] already used", name.c_str());
      }

      auto *t      = new Tile(); // std::make_shared< class Tile >();
      auto  result = all_tiles.insert(std::make_pair(name, t));
      if (! result.second) {
        err("tile insert name [%s] failed", name.c_str());
      }

      //
      // Global array of all tiles
      //
      all_tiles_array.push_back(t);
      t->global_index = all_tiles_array.size();

      t->name       = name;
      t->index      = idx - 1;
      t->pix_width  = width;
      t->pix_height = height;
      t->tex        = tex;
      t->set_gl_binding(tex_get_gl_binding(tex));

      t->x1 = fw * (static_cast< float >(x));
      t->y1 = fh * (static_cast< float >(y));
      t->x2 = t->x1 + fw;
      t->y2 = t->y1 + fh;

#ifdef ENABLE_DEBUG_TILE
      printf("Tile: %-10s %ux%u (%u, %u)", name.c_str(), width, height, x, y);
#endif

#ifdef ENABLE_TILE_BOUNDS
      if ((pixel_size.w <= TILE_WIDTH_MAX) && (pixel_size.h <= TILE_HEIGHT_MAX)) {
        SDL_Surface *s = tex_get_surface(tex);

        spoint off(pixel_size.w * x, pixel_size.h * y);

        spoint MAX(pixel_size.w * x, pixel_size.h * y);

        spoint MIN((pixel_size.w * x) + pixel_size.w - 1, (pixel_size.h * y) + pixel_size.h - 1);

        int x1, y1;

        for (y1 = pixel_size.h - 1; y1 >= 0; y1--) {
          for (x1 = 0; x1 < pixel_size.w; x1++) {

            spoint at((pixel_size.w * x) + x1, (pixel_size.h * y) + y1);

            color p;
            GET_PIXEL(s, at.x, at.y, p);

            //
            // If solid...
            //
            if (p.a >= 0xef) {
              MIN.x = std::min(at.x, MIN.x);
              MIN.y = std::min(at.y, MIN.y);
              MAX.x = std::max(at.x, MAX.x);
              MAX.y = std::max(at.y, MAX.y);
#ifdef ENABLE_DEBUG_TILE
              printf("X");
#endif
              if ((x1 < TILE_WIDTH_MAX) && (y1 < TILE_HEIGHT_MAX)) {
                t->pix[ x1 ][ y1 ] = 1U;
              }
            } else if (p.a > 0) {
#ifdef ENABLE_DEBUG_TILE
              printf(".");
#endif
            } else {
#ifdef ENABLE_DEBUG_TILE
              printf(" ");
#endif
            }
          }
#ifdef ENABLE_DEBUG_TILE
          printf("\n");
#endif
        }

        t->px1 = (((float) (MIN.x) - off.x)) / ((float) pixel_size.w);
        t->px2 = (((float) (MAX.x) - off.x + 1)) / ((float) pixel_size.w);
        t->py1 = (((float) (MIN.y) - off.y)) / ((float) pixel_size.h);
        t->py2 = (((float) (MAX.y) - off.y + 1)) / ((float) pixel_size.h);
      }
#endif

#ifdef ENABLE_DEBUG_TILE
      printf("^^^  %s %f %f %f %f\n", name.c_str(), t->px1, t->py1, t->px2, t->py2);
#endif
    }

    x++;

    //
    // Check the whole tile can be read
    //
    if ((x * width) + (width - 1) >= tex_get_width(tex)) {
      x = 0;
      y++;
    }

    if (y * height > tex_get_height(tex)) {
      if (! name.empty()) {
        CROAK("overflow reading tile arr[%s]", name.c_str());
      } else {
        err("overflow reading tile arr at x %d y %d", x, y);
      }
    }
  }
}

void tile_load_arr_sprites(const char *file, const char *alias, uint32_t tile_width, uint32_t tile_height, int nargs, const char *arr[],
                           int gl_mode)
{
  TRACE();

  Texp tex            = nullptr;
  Texp tex_monochrome = nullptr;
  Texp tex_mask       = nullptr;
  Texp tex_outline    = nullptr;

  tex_load_sprites(&tex, &tex_monochrome, &tex_mask, &tex_outline, file, alias, tile_width, tile_height, gl_mode);

  float const fw = static_cast< float >(1.0) / (((static_cast< float >(tex_get_width(tex)))) / ((static_cast< float >(tile_width))));
  float const fh = static_cast< float >(1.0) / (((static_cast< float >(tex_get_height(tex)))) / ((static_cast< float >(tile_height))));

  int x   = 0;
  int y   = 0;
  int idx = 0;

#ifdef WRITE_TILED
  uint32_t rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
  rmask = 0xff000000;
  gmask = 0x00ff0000;
  bmask = 0x0000ff00;
  amask = 0x000000ff;
#else
  rmask = 0x000000ff;
  gmask = 0x0000ff00;
  bmask = 0x00ff0000;
  amask = 0xff000000;
#endif
  auto tmp = SDL_CreateRGBSurface(0, tex_get_width(tex) * 2, tex_get_height(tex) * 2, 32, rmask, gmask, bmask, amask);
#endif

  isize pixel_size;

  pixel_size.w = tile_width;
  pixel_size.h = tile_height;

  while (nargs-- > 0) {
    std::string const name = arr[ idx++ ];

    if (! name.empty()) {
      if (tile_find(name) != nullptr) {
        CROAK("tile name [%s] already used", name.c_str());
      }

      IF_DEBUG2
      { //
        log("add tile name [%s]", name.c_str());
      }

      auto *t      = new Tile(); // std::make_shared< class Tile >();
      auto  result = all_tiles.insert(std::make_pair(name, t));
      if (! result.second) {
        CROAK("tile insert name [%s] failed", name.c_str());
      }

      //
      // Global array of all tiles
      //
      all_tiles_array.push_back(t);
      t->global_index = all_tiles_array.size();

      t->name           = name;
      t->index          = idx - 1;
      t->pix_width      = tile_width;
      t->pix_height     = tile_height;
      t->tex            = tex;
      t->tex_monochrome = tex_monochrome;
      t->tex_mask       = tex_mask;
      t->tex_outline    = tex_outline;

      t->set_gl_binding(tex_get_gl_binding(t->tex));
      t->set_gl_binding_monochrome(tex_get_gl_binding(t->tex_monochrome));
      t->set_gl_binding_mask(tex_get_gl_binding(t->tex_mask));
      t->set_gl_binding_outline(tex_get_gl_binding(t->tex_outline));

      t->x1 = fw * (static_cast< float >(x));
      t->y1 = fh * (static_cast< float >(y));
      t->x2 = t->x1 + fw;
      t->y2 = t->y1 + fh;

#ifdef ENABLE_DEBUG_TILE
      printf("Tile: %-10s %ux%u (%u, %u)", name.c_str(), tile_width, tile_height, x, y);
#endif

#ifdef ENABLE_TILE_BOUNDS
      if ((pixel_size.w <= TILE_WIDTH_MAX) && (pixel_size.h <= TILE_HEIGHT_MAX)) {
        SDL_Surface *s = tex_get_surface(tex);

        spoint off(pixel_size.w * x, pixel_size.h * y);

        spoint MAX(pixel_size.w * x, pixel_size.h * y);

        spoint MIN((pixel_size.w * x) + pixel_size.w - 1, (pixel_size.h * y) + pixel_size.h - 1);

        int x1, y1;

        for (y1 = pixel_size.h - 1; y1 >= 0; y1--) {
          for (x1 = 0; x1 < pixel_size.w; x1++) {

            spoint at((pixel_size.w * x) + x1, (pixel_size.h * y) + y1);

            color p;
            GET_PIXEL(s, at.x, at.y, p);

#ifdef WRITE_TILED
            spoint at2(((pixel_size.w + 2) * x) + x1, ((pixel_size.h + 2) * y) + y1);
            PUT_PIXEL(tmp, at2.x, at2.y, p);
#endif

            //
            // If solid...
            //
            if (p.a >= 0xef) {
              MIN.x = std::min(at.x, MIN.x);
              MIN.y = std::min(at.y, MIN.y);
              MAX.x = std::max(at.x, MAX.x);
              MAX.y = std::max(at.y, MAX.y);
#ifdef ENABLE_DEBUG_TILE
              printf("X");
#endif
              if ((x1 < TILE_WIDTH_MAX) && (y1 < TILE_HEIGHT_MAX)) {
                t->pix[ x1 ][ y1 ] = 1U;
              }
            } else if (p.a > 0) {
#ifdef ENABLE_DEBUG_TILE
              printf(".");
#endif
            } else {
#ifdef ENABLE_DEBUG_TILE
              printf(" ");
#endif
            }
          }
#ifdef ENABLE_DEBUG_TILE
          printf("\n");
#endif
        }

        t->px1 = (((float) (MIN.x) - off.x)) / ((float) pixel_size.w);
        t->px2 = (((float) (MAX.x) - off.x + 1)) / ((float) pixel_size.w);
        t->py1 = (((float) (MIN.y) - off.y)) / ((float) pixel_size.h);
        t->py2 = (((float) (MAX.y) - off.y + 1)) / ((float) pixel_size.h);
      }
#endif

#ifdef ENABLE_DEBUG_TILE
      printf("^^^  %s %f %f %f %f\n", name.c_str(), t->px1, t->py1, t->px2, t->py2);
#endif
    }

    x++;

    //
    // Check the whole tile can be read
    //
    if ((x * tile_width) + (tile_width - 1) >= tex_get_width(tex)) {
      x = 0;
      y++;
    }

    if (y * tile_height > tex_get_height(tex)) {
      if (! name.empty()) {
        CROAK("overflow reading tile arr[%s]", name.c_str());
      } else {
        CROAK("overflow reading tile arr at x %d y %d", x, y);
      }
    }
  }

#ifdef WRITE_TILED
  con("save %s", file);
  std::string bmp = std::string(basename((char *) file)) + ".bmp";
  SDL_SaveBMP(tmp, bmp.c_str());
#endif
}

//
// Creae a texture from an FBO
//
void tile_from_fbo(Gamep g, FboEnum fbo)
{
  TRACE();

  int w = 0;
  int h = 0;
  fbo_get_size(g, fbo, w, h);
  auto name = FboEnum_to_string(fbo);

  if (tile_find(name) != nullptr) {
    CROAK("tile name [%s] already used", name.c_str());
  }

  auto *tex = tex_from_fbo(g, fbo);
  if (tex == nullptr) {
    CROAK("tile name [%s] failed to create tex", name.c_str());
  }

  auto *t      = new Tile(); // std::make_shared< class Tile >();
  auto  result = all_tiles.insert(std::make_pair(name, t));
  if (! result.second) {
    CROAK("tile insert name [%s] failed", name.c_str());
  }

  //
  // Global array of all tiles
  //
  all_tiles_array.push_back(t);
  t->global_index = all_tiles_array.size();

  t->name       = name;
  t->pix_width  = w;
  t->pix_height = h;
  t->tex        = tex;
  t->set_gl_binding(tex_get_gl_binding(t->tex));

  t->x1 = 0;
  t->y1 = 0;
  t->x2 = 1;
  t->y2 = 1;
}

//
// Find an existing tile.
//
auto tile_find(const std::string &name) -> Tilep
{
  TRACE();

  if (name.empty()) {
    return nullptr;
  }

  auto result = all_tiles.find(name);
  if (result == all_tiles.end()) {
    return nullptr;
  }

  return result->second;
}

auto tile_find_mand(const std::string &name) -> Tilep
{
  TRACE();

  if (name.empty()) {
    err("no tile name given");
    return nullptr;
  }

  auto result = all_tiles.find(name);
  if (result == all_tiles.end()) {
    CROAK("tile name %s not found", name.c_str());
    return nullptr;
  }

  return result->second;
}

auto tile_width(Tilep tile) -> int
{
  TRACE_DEBUG();
  return tile->pix_width;
}

auto tile_height(Tilep tile) -> int
{
  TRACE_DEBUG();
  return tile->pix_height;
}

auto tile_tex(Tilep tile) -> Texp
{
  TRACE_DEBUG();
  return tile->tex;
}

auto tile_index(Tilep tile) -> uint16_t
{
  TRACE_DEBUG();
  return tile->index;
}

void tile_coords(Tilep tile, float *x1, float *y1, float *x2, float *y2)
{
  TRACE_DEBUG();

  *x1 = tile->x1;
  *y1 = tile->y1;
  *x2 = tile->x2;
  *y2 = tile->y2;
}

auto string2tile(const char **s, int *len) -> Tilep
{
  TRACE();
  static char        name[ MAXSHORTSTR ];
  static const char *eo_name = name + MAXSHORTSTR;
  const char        *c       = *s;
  char              *t       = name; // NOLINT

  while (t < eo_name) {
    if ((*c == '\0') || (*c == '$')) {
      break;
    }

    *t++ = *c++;
  }

  if (c == eo_name) {
    return nullptr;
  }

  *t++ = '\0';
  *s += (t - name);

  if (len != nullptr) {
    *len = t - name;
  }

  auto result = all_tiles.find(name);
  if (result == all_tiles.end()) {
    err("unknown tile [%s]", name);
    return nullptr;
  }

  return result->second;
}

auto string2tile(std::string &s, int *len) -> Tilep
{
  TRACE();
  auto        iter = s.begin();
  std::string name;

  while (iter != s.end()) {
    auto c = *iter;

    if ((c == '\0') || (c == '$')) {
      break;
    }

    name += c;
    iter++;
  }

  if (iter == s.end()) {
    err("unknown tile [%s]", name.c_str());
  }

  if (len != nullptr) {
    *len = iter - s.begin();
  }

  auto result = all_tiles.find(name);
  if (result == all_tiles.end()) {
    err("unknown tile [%s]", name.c_str());
  }

  return result->second;
}

auto tile_index_to_tile(int i) -> Tilep
{
  TRACE_DEBUG();

  if (i == 0) [[unlikely]] {
    return nullptr;
  }

  return all_tiles_array[ i - 1 ];
}

auto tile_name(Tilep t) -> std::string
{
  TRACE();
  return t->name;
}

auto tile_delay_ms(Tilep t) -> uint32_t
{
  TRACE_DEBUG();

  if (! static_cast< bool >(t->delay_ms)) {
    return 5000;
  }
  return t->delay_ms;
}
void tile_delay_ms_set(Tilep t, uint32_t val)
{
  TRACE();
  t->delay_ms = val;
}

void tile_size_set(Tilep t, uint32_t w, uint32_t h)
{
  TRACE();

  float const dx = (t->x2 - t->x1) / static_cast< float >(t->pix_width);
  float const cx = (t->pix_width - w) / 2;
  t->x1 += cx * dx;
  t->x2 -= cx * dx;

  float const dy = (t->y2 - t->y1) / static_cast< float >(t->pix_width);
  float const cy = (t->pix_width - w) / 2;
  t->y1 += cy * dy;
  t->y2 -= cy * dy;

  t->pix_width  = w;
  t->pix_height = h;
}

auto tile_global_index(Tilep t) -> uint32_t
{
  TRACE();
  return t->global_index;
}
void tile_global_index_set(Tilep t, uint32_t val)
{
  TRACE();
  t->global_index = val;
}

auto tile_is_loggable(Tilep t) -> bool
{
  TRACE();
  return t->is_loggable;
}

auto tile_is_end_of_anim(Tilep t) -> bool
{
  TRACE();
  return t->is_end_of_anim;
}

auto tile_is_cleanup_on_end_of_anim(Tilep t) -> bool
{
  TRACE();
  return t->is_cleanup_on_end_of_anim;
}

void tile_is_cleanup_on_end_of_anim_set(Tilep t)
{
  TRACE();
  t->is_cleanup_on_end_of_anim = true;
}

auto tile_is_alive_on_end_of_anim(Tilep t) -> bool
{
  TRACE();
  return t->is_alive_on_end_of_anim;
}

void tile_is_alive_on_end_of_anim_set(Tilep t)
{
  TRACE();
  t->is_alive_on_end_of_anim = true;
}

auto Tile::gl_binding() const -> int
{
  TRACE();
  if (g_monochrome) {
    return _gl_binding_monochrome;
  }
  return _gl_binding;
}

void Tile::set_gl_binding(int v)
{
  TRACE();
  _gl_binding = v;
}

auto tile_gl_binding(Tilep t) -> int
{
  TRACE();
  return t->gl_binding();
}

auto Tile::gl_binding_monochrome() const -> int
{
  TRACE();
  return _gl_binding_monochrome;
}

void Tile::set_gl_binding_monochrome(int v)
{
  TRACE();
  _gl_binding_monochrome = v;
}

auto Tile::gl_binding_mask() const -> int
{
  TRACE();
  return _gl_binding_mask;
}

void Tile::set_gl_binding_mask(int v)
{
  TRACE();
  _gl_binding_mask = v;
}

auto Tile::gl_binding_outline() const -> int
{
  TRACE();
  return _gl_binding_outline;
}

void Tile::set_gl_binding_outline(int v)
{
  TRACE();
  _gl_binding_outline = v;
}

void tile_blit(const Tilep &tile, const spoint tl, const spoint br, const color &c)
{
  TRACE_DEBUG();

  float x1 = 0;
  float x2 = 0;
  float y1 = 0;
  float y2 = 0;

  x1 = tile->x1;
  x2 = tile->x2;
  y1 = tile->y1;
  y2 = tile->y2;

  auto binding = tile->gl_binding();
  blit(binding, x1, y2, x2, y1, tl.x, br.y, br.x, tl.y, c);
}

void tile_blit(const Tilep &tile, float x1, float x2, float y1, float y2, const spoint tl, const spoint br, const color &c)
{
  TRACE_DEBUG();

  blit(tile->gl_binding(), x1, y2, x2, y1, tl.x, br.y, br.x, tl.y, c);
}

void tile_blit(const Tilep &tile, float x1, float x2, float y1, float y2, const spoint tl, const spoint br, const color &c,
               LightPixels *light_pixels, bool is_blit_flush_per_line)
{
  TRACE_DEBUG();

  if (light_pixels != nullptr) {
    blit(tile->gl_binding(), x1, y2, x2, y1, tl.x, br.y, br.x, tl.y, c, light_pixels, is_blit_flush_per_line);
  } else {
    blit(tile->gl_binding(), x1, y2, x2, y1, tl.x, br.y, br.x, tl.y, c);
  }
}

void tile_blit(const Tilep &tile, spoint tl, spoint br, const color &color_tl, const color &color_tr, const color &color_bl,
               const color &color_br)
{
  TRACE_DEBUG();

  float const x1 = tile->x1;
  float const x2 = tile->x2;
  float const y1 = tile->y1;
  float const y2 = tile->y2;

  blit(tile->gl_binding(), x1, y2, x2, y1, tl.x, br.y, br.x, tl.y, color_tl, color_tr, color_bl, color_br);
}

void tile_blit_section(const Tilep &tile, const fpoint &tile_tl, const fpoint &tile_br, const spoint tl, const spoint br,
                       const color &color_tl, const color &color_tr, const color &color_bl, const color &color_br)
{
  TRACE_DEBUG();

  float       x1 = 0;
  float       x2 = 0;
  float       y1 = 0;
  float       y2 = 0;
  float const tw = tile->x2 - tile->x1;
  float const th = tile->y2 - tile->y1;

  x1 = tile->x1 + (tile_tl.x * tw);
  x2 = tile->x1 + (tile_br.x * tw);
  y1 = tile->y1 + (tile_tl.y * th);
  y2 = tile->y1 + (tile_br.y * th);

  blit(tile->gl_binding(), x1, y2, x2, y1, tl.x, br.y, br.x, tl.y, color_tl, color_tr, color_bl, color_br);
}

//
// Outline only
//
void tile_blit_outline(const Tilep &tile, float x1, float x2, float y1, float y2, const spoint tl, const spoint br, const color &c)
{
  TRACE_DEBUG();

  blit(tile->gl_binding_outline(), x1, y2, x2, y1, tl.x, br.y, br.x, tl.y, c);
}

//
// Add a black outline
//
void tile_blit_outlined(const Tilep &tile, float x1, float x2, float y1, float y2, const spoint tl, const spoint br, const color &c,
                        const color &outline, int single_pix_size, bool square)
{
  TRACE_DEBUG();

  auto binding = tile->gl_binding_mask();

  if (single_pix_size != 0) {
    int dx = single_pix_size - 1;
    int dy = single_pix_size - 1;

    if (dx == 0) {
      dx = 1;
    }
    if (dy == 0) {
      dy = 1;
    }

    if (square) {
      blit(binding, x1, y2, x2, y1, tl.x - dx, br.y - dy, br.x - dx, tl.y - dy, outline);
      blit(binding, x1, y2, x2, y1, tl.x + dx, br.y + dy, br.x + dx, tl.y + dy, outline);
      blit(binding, x1, y2, x2, y1, tl.x - dx, br.y + dy, br.x - dx, tl.y + dy, outline);
      blit(binding, x1, y2, x2, y1, tl.x + dx, br.y - dy, br.x + dx, tl.y - dy, outline);
    }
    blit(binding, x1, y2, x2, y1, tl.x + dx, br.y, br.x + dx, tl.y, outline);
    blit(binding, x1, y2, x2, y1, tl.x - dx, br.y, br.x - dx, tl.y, outline);
    blit(binding, x1, y2, x2, y1, tl.x, br.y + dy, br.x, tl.y + dy, outline);
    blit(binding, x1, y2, x2, y1, tl.x, br.y - dy, br.x, tl.y - dy, outline);
  }

  binding = tile->gl_binding();
  blit(binding, x1, y2, x2, y1, tl.x, br.y, br.x, tl.y, c);
}

//
// Shift the coordinates of a tile by a given percentage, so the bottom is
// trimmed and looks submerged.
//
void tile_blit_apply_submerge_pct(Gamep g, spoint &tl, spoint &br, Tilep tile, float &x1, float &x2, float &y1, float &y2, float percent)
{
  TRACE_DEBUG();

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
  // changes to
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
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  //
  float const pix_height     = br.y - tl.y;
  auto        pc             = percent / 100.0F;
  auto        pix_removed    = static_cast< int >(pc * pix_height);
  auto        one_pix_height = static_cast< int >(pix_height / tile_height(tile));

  //
  // Round the pixel removed amount to the nearest whole pixel
  //
  pix_removed -= pix_removed % one_pix_height;

  //
  // The actual removed percentage based on the number of pixels
  // removed and not the original percentage which might have
  // been on an uneven pixel boundary
  //
  pc = pix_removed / pix_height;

  //
  // Now we know how many pixels are being removed, we can apply
  // the percentage to the texture
  //
  auto tex_height  = y2 - y1;
  auto tex_removed = pc * tex_height;

  //
  // Remove the submerged part of the tile
  //
  br.y -= pix_removed;
  y2 -= tex_removed;

  //
  // Now shift the tile down so it looks as if it is on the waterline
  // Move if down by the amount removed
  //
  tl.y += pix_removed;
  br.y += pix_removed;
}
