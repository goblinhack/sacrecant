//
// Copyright goblinhack@gmail.com
//

#ifndef MY_ASCII_HPP
#define MY_ASCII_HPP

#include "my_color.hpp"
#include "my_fpoint.hpp"
#include "my_tex.hpp"
#include "my_tile.hpp"
#include "my_ui.hpp"

enum { TERM_WIDTH_MAX = 130, TERM_HEIGHT_MAX = 70 };
enum { TERM_WIDTH_MIN = 120, TERM_HEIGHT_MIN = 60 };

#define TERM_WIDTH_DEF  TERM_WIDTH_MIN
#define TERM_HEIGHT_DEF TERM_HEIGHT_MIN

class AsciiCell
{
public:
  char  ch[ TILE_LAYER_ENUM_MAX ] {};
  Tilep tile[ TILE_LAYER_ENUM_MAX ] {};

  Texp tex[ TILE_LAYER_ENUM_MAX ] {};

  float tx[ TILE_LAYER_ENUM_MAX ] {};
  float ty[ TILE_LAYER_ENUM_MAX ] {};
  float dx[ TILE_LAYER_ENUM_MAX ] {};
  float dy[ TILE_LAYER_ENUM_MAX ] {};

  color color_tl[ TILE_LAYER_ENUM_MAX ] {};
  color color_bl[ TILE_LAYER_ENUM_MAX ] {};
  color color_tr[ TILE_LAYER_ENUM_MAX ] {};
  color color_br[ TILE_LAYER_ENUM_MAX ] {};

  //
  // Is reset each frame, and so although a pointer potentially should be
  // zeroed out on game load once it is used.
  //
  void *context {};

  AsciiCell() = default;
};

using ascii_key_down_callback   = int (*)(int, int, const struct SDL_Keysym *);
using ascii_mouse_down_callback = int (*)(int, int, int);
using ascii_mouse_over_callback = int (*)(int, int);

using TileLayers = std::array< Tilep, TILE_LAYER_ENUM_MAX >;

using button_args = struct ButtonArgs {
  int x;
  int y;
  int width;
  int over;

  int sdl_mod;
  int sdl_key;
  int mouse_button;

  color col;

  ascii_key_down_callback   key_down;
  ascii_mouse_down_callback mouse_down;
  ascii_mouse_over_callback mouse_over;

  void *context;
};

using box_args = struct BoxArgs {
  int x;
  int y;
  int width;
  int height;
  int over;

  int sdl_mod;
  int sdl_key;
  int mouse_button;

  color col_bg;
  color col_text;

  ascii_key_down_callback   key_down;
  ascii_mouse_down_callback mouse_down;
  ascii_mouse_over_callback mouse_over;

  void *context;
};

[[nodiscard]] auto ascii_get_color(int x, int y, int z) -> color;
[[nodiscard]] auto ascii_get_stat_context(int x, int y) -> void *;
[[nodiscard]] auto ascii_is_empty(int x, int y) -> bool;
[[nodiscard]] auto ascii_ok_for_scissors(int x, int y) -> int;
[[nodiscard]] auto ascii_ok(int x, int y) -> int;
[[nodiscard]] auto ascii_strip(std::string const &text) -> std::string;
[[nodiscard]] auto ascii_strlen(std::string const &text) -> int;
[[nodiscard]] auto ascii_tp_br1_tile(int x, int y, fpoint *) -> int;
[[nodiscard]] auto ascii_tp_tile_mid(int x, int y, fpoint *) -> int;
[[nodiscard]] auto ascii_tp_tl1_tile(int x, int y, fpoint *) -> int;
[[nodiscard]] auto ascii_x_ok(int x) -> int;
[[nodiscard]] auto ascii_y_ok(int y) -> int;

auto ascii_blit_layer(int z, int no_color) -> void;
auto ascii_clear_display() -> void;
auto ascii_clear_scissors() -> void;
auto ascii_dim(int x, int y, int z, float alpha) -> void;
auto ascii_display(Gamep g) -> void;
auto ascii_draw_line(int depth, int x0, int y0, int x1, int y1, char what, color c) -> void;
auto ascii_draw_line(int x0, int y0, int x1, int y1, char what, color c) -> void;
auto ascii_draw_line(int x0, int y0, int x1, int y1, const char *tilename, color c) -> void;
auto ascii_draw_line(int x0, int y0, int x1, int y1, Tilep what, color c) -> void;
auto ascii_dump_to_console(FILE *, bool no_color) -> void;
auto ascii_init() -> void;
auto ascii_put_bg_square(int tlx, int tly, int brx, int bry, char what, color c) -> void;
auto ascii_put_bg_square(int tlx, int tly, int brx, int bry, const char *tilename, color c) -> void;
auto ascii_put_bg_square(int tlx, int tly, int brx, int bry, Tilep tile, color c) -> void;
auto ascii_put_box(box_args b, int style, TileLayers tiles, const char *fmt, ...) -> void;
auto ascii_putf_internal2(int x, int y, color fg, color bg, const std::string &text) -> void;
auto ascii_putf(int x, int y, color fg, color bg, const char *fmt, ...) -> void;
auto ascii_putf(int x, int y, color fg, const char *fmt, ...) -> void;
auto ascii_putf(int x, int y, const char *fmt, ...) -> void;
auto ascii_set_context(int x, int y, void *context) -> void;
auto ascii_set_scissors(spoint tl, spoint br) -> void;
auto ascii_set(int depth, int x, int y, char ch) -> void;
auto ascii_set(int depth, int x, int y, color c) -> void;
auto ascii_set(int depth, int x, int y, const char *tilename, char) -> void;
auto ascii_set(int depth, int x, int y, Texp tex, float tx, float ty, float dx, float dy) -> void;
auto ascii_set(int depth, int x, int y, Tilep tile, char ch) -> void;
auto ascii_set(int depth, int x, int y, Tilep tile, float tx, float ty, float dx, float dy) -> void;
auto ascii_set(int depth, int x, int y, Tilep tile) -> void;
auto pixel_to_ascii(Gamep g, int *x, int *y) -> void;

extern float tile_pix_w;
extern float tile_pix_h;

extern int TERM_WIDTH;
extern int TERM_HEIGHT;

extern int ascii_mouse_x;
extern int ascii_mouse_y;

#endif
