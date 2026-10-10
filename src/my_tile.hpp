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

#define BOSS_TILE_HEIGHT (TILE_WIDTH * 8)
#define BOSS_TILE_WIDTH  (TILE_HEIGHT * 8)

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
#define TILE_LAYER_ENUM(list_macro)                                                                                                             \
  CLANG_FORMAT_INDENT()                                     /* dummy line for clang indentation fixup */                                        \
  list_macro(TILE_LAYER_BOX_BG, "TILE_LAYER_BOX_BG"),       /* newline */                                                                       \
      list_macro(TILE_LAYER_BOX_FG, "TILE_LAYER_BOX_FG"),   /* newline */                                                                       \
      list_macro(TILE_LAYER_TEXT_BG, "TILE_LAYER_TEXT_BG"), /* newline */                                                                       \
      list_macro(TILE_LAYER_TEXT_FG, "TILE_LAYER_TEXT_FG"), /* newline */

ENUM_DEF_H(TILE_LAYER_ENUM, TileLayerEnum);

[[nodiscard]] auto tile_tex(Tilep tile) -> class Tex *;

[[nodiscard]] auto tile_init() -> bool;
[[nodiscard]] auto tile_is_alive_on_end_of_anim(Tilep t) -> bool;
[[nodiscard]] auto tile_is_cleanup_on_end_of_anim(Tilep t) -> bool;
[[nodiscard]] auto tile_is_end_of_anim(Tilep t) -> bool;
[[nodiscard]] auto tile_is_dead(Tilep) -> bool;
[[nodiscard]] auto tile_is_loggable(Tilep t) -> bool;
[[nodiscard]] auto tile_is_open(Tilep) -> bool;
[[nodiscard]] auto tile_height(Tilep tile) -> int;
[[nodiscard]] auto tile_width(Tilep tile) -> int;
[[nodiscard]] auto tile_gl_binding(Tilep t) -> int;
[[nodiscard]] auto tile_name(Tilep t) -> std::string;
[[nodiscard]] auto string2tile(const char **s, int *len = nullptr) -> Tilep;
[[nodiscard]] auto string2tile(std::string &s, int *len = nullptr) -> Tilep;
[[nodiscard]] auto tile_find_mand(const std::string &name) -> Tilep;
[[nodiscard]] auto tile_find(const std::string &name) -> Tilep;
[[nodiscard]] auto tile_index_to_tile(int i) -> Tilep;
[[nodiscard]] auto tile_delay_ms(Tilep t) -> uint32_t;
[[nodiscard]] auto tile_global_index(Tilep t) -> uint32_t;
[[nodiscard]] auto tile_index(Tilep tile) -> uint16_t;
[[nodiscard]] auto tile_move(Tilep) -> uint32_t;

auto tile_coords(Tilep tile, float *x1, float *y1, float *x2, float *y2) -> void;
auto tile_size_set(Tilep t, uint32_t w, uint32_t h) -> void;
auto tile_delay_ms_set(Tilep t, uint32_t val) -> void;
auto tile_fini() -> void;
auto tile_free(Tilep) -> void;
auto tile_global_index_set(Tilep t, uint32_t val) -> void;
auto tile_is_alive_on_end_of_anim_set(Tilep t) -> void;
auto tile_is_cleanup_on_end_of_anim_set(Tilep t) -> void;
auto tile_is_end_of_anim_set(Tilep t) -> void;
auto tile_load(const char *, uint32_t width, uint32_t height, int nargs, ...) -> void;
auto tile_from_fbo(Gamep g, FboEnum fbo) -> void;

auto tile_load_arr(const char *file, const char *alias, uint32_t w, uint32_t h, int nargs, const char *arr[]) -> void;
auto tile_blit_apply_submerge_pct(Gamep g, spoint &tl, spoint &br, Tilep tile, float &x1, float &x2, float &y1, float &y2, float percent)
    -> void;
void tile_load_arr_sprites(const char *file, const char *alias, uint32_t tile_width, uint32_t tile_height, int nargs, const char *arr[],
                           int gl_mode = GL_NEAREST);

struct LightPixels;

auto tile_blit(const Tilep &tile, spoint tl, spoint br, const color &c) -> void;
auto tile_blit(const Tilep &tile, float x1, float x2, float y1, float y2, spoint tl, spoint br, const color &c) -> void;
void tile_blit(const Tilep &tile, float x1, float x2, float y1, float y2, spoint tl, spoint br, const color &c, struct LightPixels *light_pixels,
               bool blit_flush_per_line);
void tile_blit(const Tilep &tile, spoint tl, spoint br, const color &color_tl, const color &color_tr, const color &color_bl,
               const color &color_br);
void tile_blit_section(const Tilep &tile, const fpoint &tile_tl, const fpoint &tile_br, spoint tl, spoint br, const color &color_tl,
                       const color &color_tr, const color &color_bl, const color &color_br);
void tile_blit_outlined(const Tilep &tile, float x1, float x2, float y1, float y2, spoint tl, spoint br, const color &c, const color &outline,
                        int single_pix_size, bool square);
void tile_blit_white_mask(const Tilep &tile, float x1, float x2, float y1, float y2, spoint tl, spoint br);
auto tile_blit_outline_w_black_inside(const Tilep &tile, float x1, float x2, float y1, float y2, spoint tl, spoint br, const color &c) -> void;
auto tile_blit_outline_w_invis_inside(const Tilep &tile, float x1, float x2, float y1, float y2, spoint tl, spoint br, const color &c) -> void;

#endif
