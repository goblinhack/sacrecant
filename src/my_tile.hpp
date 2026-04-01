//
// Copyright goblinhack@gmail.com
//

#ifndef MY_TILE_HPP
#define MY_TILE_HPP

#include "my_color.hpp"
#include "my_enum.hpp"
#include "my_fpoint.hpp"
#include "my_gl.hpp"
#include "my_spoint.hpp"
#include "my_types.hpp"

class Tile;

//
// This is the actual tile sizw
//
enum { TILE_HEIGHT = 12, TILE_WIDTH = 12 };

//
// This is the tile size with outline padding. Only the actual tile size is used in game.
//
#define OUTLINE_TILE_HEIGHT (TILE_WIDTH + 2)
#define OUTLINE_TILE_WIDTH  (TILE_HEIGHT + 2)

//
// The tiles are 14x14 to allow for outlines, however the screen lighting is done in
// 12x12, ignoring the extra pixel. So all walls and floors etc... are 12x12.
//
#define LIGHT_PIXEL (TILE_WIDTH)

#define TILE_HEIGHT_MAX OUTLINE_TILE_HEIGHT
#define TILE_WIDTH_MAX  OUTLINE_TILE_WIDTH // Largest tile for collisions

//
// Frame buffer objects
//
#define TILE_LAYER_ENUM(list_macro)                                                                                                        \
  CLANG_FORMAT_INDENT()                               /* dummy line for clang indentation fixup */                                         \
  list_macro(TILE_LAYER_BG_0, "TILE_LAYER_BG_0"),     /* newline */                                                                        \
      list_macro(TILE_LAYER_BG_1, "TILE_LAYER_BG_1"), /* newline */                                                                        \
      list_macro(TILE_LAYER_FG_0, "TILE_LAYER_FG_0"), /* newline */                                                                        \
      list_macro(TILE_LAYER_FG_1, "TILE_LAYER_FG_1"), /* newline */

ENUM_DEF_H(TILE_LAYER_ENUM, TileLayerEnum);

auto tile_tex(Tilep tile) -> class Tex *;

auto tile_init() -> bool;
auto tile_is_alive_on_end_of_anim(Tilep t) -> bool;
auto tile_is_cleanup_on_end_of_anim(Tilep t) -> bool;
auto tile_is_dead(Tilep) -> bool;
auto tile_is_end_of_anim(Tilep t) -> bool;
auto tile_is_loggable(Tilep t) -> bool;
auto tile_is_open(Tilep) -> bool;
auto tile_height(Tilep tile) -> int;
auto tile_width(Tilep tile) -> int;
auto tile_gl_binding(Tilep t) -> int;
auto tile_name(Tilep t) -> std::string;
auto string2tile(const char **s, int *len = nullptr) -> Tilep;
auto string2tile(std::string &s, int *len = nullptr) -> Tilep;
auto tile_find_mand(const std::string &name) -> Tilep;
auto tile_find(const std::string &name) -> Tilep;
auto tile_index_to_tile(int i) -> Tilep;
auto tile_delay_ms(Tilep t) -> uint32_t;
auto tile_global_index(Tilep t) -> uint32_t;
auto tile_index(Tilep tile) -> uint16_t;
auto tile_move(Tilep) -> uint32_t;
void tile_coords(Tilep tile, float *x1, float *y1, float *x2, float *y2);
void tile_size_set(Tilep t, uint32_t w, uint32_t h);
void tile_delay_ms_set(Tilep t, uint32_t val);
void tile_fini();
void tile_free(Tilep);
void tile_global_index_set(Tilep t, uint32_t val);
void tile_is_alive_on_end_of_anim_set(Tilep t);
void tile_is_cleanup_on_end_of_anim_set(Tilep t);
void tile_load(const char *, uint32_t width, uint32_t height, int nargs, ...);
void tile_from_fbo(Gamep g, FboEnum fbo);

void tile_load_arr(const char *file, const char *alias, uint32_t w, uint32_t h, int nargs, const char *arr[]);
void tile_blit_apply_submerge_pct(Gamep g, spoint &tl, spoint &br, Tilep tile, float &x1, float &x2, float &y1, float &y2, float percent);
void tile_load_arr_sprites(const char *file, const char *alias, uint32_t tile_width, uint32_t tile_height, int nargs, const char *arr[],
                           int gl_mode = GL_NEAREST);

struct LightPixels;

void tile_blit(const Tilep &tile, spoint tl, spoint br, const color &c);
void tile_blit(const Tilep &tile, float x1, float x2, float y1, float y2, spoint tl, spoint br, const color &c);
void tile_blit(const Tilep &tile, float x1, float x2, float y1, float y2, spoint tl, spoint br, const color &c,
               struct LightPixels *light_pixels, bool blit_flush_per_line);
void tile_blit(const Tilep &tile, spoint tl, spoint br, const color &color_tl, const color &color_tr, const color &color_bl,
               const color &color_br);
void tile_blit_section(const Tilep &tile, const fpoint &tile_tl, const fpoint &tile_br, spoint tl, spoint br, const color &color_tl,
                       const color &color_tr, const color &color_bl, const color &color_br);
void tile_blit_outlined(const Tilep &tile, float x1, float x2, float y1, float y2, spoint tl, spoint br, const color &c,
                        const color &outline, int single_pix_size, bool square);
void tile_blit_outline_w_black_inside(const Tilep &tile, float x1, float x2, float y1, float y2, spoint tl, spoint br, const color &c);
void tile_blit_outline_w_invis_inside(const Tilep &tile, float x1, float x2, float y1, float y2, spoint tl, spoint br, const color &c);

#endif
