//
// Copyright goblinhack@gmail.com
//

#include <algorithm>

#include "my_ascii.hpp"
#include "my_callstack.hpp"
#include "my_color_defs.hpp"
#include "my_font.hpp"
#include "my_game.hpp"
#include "my_globals.hpp"
#include "my_main.hpp"
#include "my_string.hpp"
#include "my_time.hpp"
#include "my_tp.hpp"

struct Ascii {
  //
  // In ascii mode, where on the ASCII we are
  //
  spoint mouse_at {};
  //
  // UI triggers for ASCII co-ords.
  //
  std::array< std::array< int, TERM_HEIGHT_MAX >, TERM_WIDTH_MAX > sdl_mod {};
  std::array< std::array< int, TERM_HEIGHT_MAX >, TERM_WIDTH_MAX > sdl_key {};
  std::array< std::array< int, TERM_HEIGHT_MAX >, TERM_WIDTH_MAX > mouse_button {};

  //
  // Callbacks for ASCII co-ords.
  //
  std::array< std::array< ascii_key_down_callback, TERM_HEIGHT_MAX >, TERM_WIDTH_MAX >   key_down {};
  std::array< std::array< ascii_mouse_down_callback, TERM_HEIGHT_MAX >, TERM_WIDTH_MAX > mouse_down {};
  std::array< std::array< ascii_mouse_over_callback, TERM_HEIGHT_MAX >, TERM_WIDTH_MAX > mouse_over {};
};

static struct Ascii ascii;

int TERM_WIDTH;
int TERM_HEIGHT;
int ascii_mouse_x;
int ascii_mouse_y;

static std::array< std::array< AsciiCell, TERM_HEIGHT_MAX >, TERM_WIDTH_MAX > *cells;

void ascii_init() { ascii_clear_display(); }

#ifdef ENABLE_UI_ASCII_MOUSE
//
// For drawing the mouse cursor.
//
static spoint mouse_tile_tl;
static spoint mouse_tile_br;
#endif

static spoint scissors_tl;
static spoint scissors_br;

static bool scissors_enabled = false;
static bool mouse_found      = 0;

auto ascii_ok(int x, int y) -> int
{
  if ((x < 0)) [[unlikely]] {
    return 0;
  }

  if ((x >= TERM_WIDTH)) [[unlikely]] {
    return 0;
  }

  if ((y < 0)) [[unlikely]] {
    return 0;
  }

  if ((y >= TERM_HEIGHT)) [[unlikely]] {
    return 0;
  }

  return 1;
}

auto ascii_x_ok(int x) -> int
{
  if ((x < 0)) [[unlikely]] {
    return 0;
  }

  if ((x >= TERM_WIDTH)) [[unlikely]] {
    return 0;
  }

  return 1;
}

auto ascii_y_ok(int y) -> int
{
  if ((y < 0)) [[unlikely]] {
    return 0;
  }

  if ((y >= TERM_HEIGHT)) [[unlikely]] {
    return 0;
  }

  return 1;
}

void ascii_clear_scissors() { scissors_enabled = false; }

void ascii_set_scissors(spoint tl, spoint br)
{
  scissors_enabled = true;
  scissors_tl      = tl;
  scissors_br      = br;
}

void pixel_to_ascii(Gamep g, int *x, int *y)
{
  float mx = *x;
  float my = *y;

  auto w = game_ascii_pix_width_get(g);
  auto h = game_ascii_pix_height_get(g);

  if ((w == 0) || (h == 0)) {
    *x = 0;
    *y = 0;
    return;
  }

  mx /= w;
  my /= h;

  mx = std::min< float >(mx, TERM_WIDTH - 1);

  my = std::min< float >(my, TERM_HEIGHT - 1);

  *x = static_cast< int >(mx);
  *y = static_cast< int >(my);

  // topcon("%d,%d", *x, *y);
}

auto ascii_ok_for_scissors(int x, int y) -> int
{
  if ((x < 0) || (y < 0) || (x >= TERM_WIDTH) || (y >= TERM_HEIGHT)) {
    return 0;
  }

  if (scissors_enabled) {
    if ((x < scissors_tl.x) || (x > scissors_br.x)) {
      return 0;
    }

    if ((y < scissors_tl.y) || (y > scissors_br.y)) {
      return 0;
    }
  }

  return ascii_ok(x, y);
}

auto ascii_is_empty(int x, int y) -> bool
{
  AsciiCell const *cell = &(*cells)[ x ][ y ];

  for (auto depth = 0; depth < TILE_LAYER_ENUM_MAX; depth++) {
    if (cell->tile[ depth ] != nullptr) {
      return false;
    }
    if (cell->tex[ depth ] != nullptr) {
      return false;
    }
  }
  return true;
}

void ascii_set(int depth, int x, int y, color col)
{
  if ((ascii_ok_for_scissors(x, y) == 0)) [[unlikely]] {
    return;
  }

  AsciiCell *cell = &(*cells)[ x ][ y ];

  cell->color_tl[ depth ] = col;
  cell->color_tr[ depth ] = col;
  cell->color_bl[ depth ] = col;
  cell->color_br[ depth ] = col;
}

void ascii_set_context(int x, int y, void *context)
{
  if (context == nullptr) {
    return;
  }

  if ((ascii_ok_for_scissors(x, y) == 0)) [[unlikely]] {
    return;
  }

  AsciiCell *cell = &(*cells)[ x ][ y ];

  cell->context = context;
}

auto ascii_get_stat_context(int x, int y) -> void *
{
  if (ascii_ok(x, y) == 0) {
    return nullptr;
  }

  AsciiCell const *cell = &(*cells)[ x ][ y ];

  return cell->context;
}

void ascii_set(int depth, int x, int y, const Texp tex, float tx, float ty, float dx, float dy)
{
  if ((ascii_ok_for_scissors(x, y) == 0)) [[unlikely]] {
    return;
  }

  AsciiCell *cell = &(*cells)[ x ][ y ];

  cell->ch[ depth ]  = 0;
  cell->tex[ depth ] = tex;
  cell->tx[ depth ]  = tx;
  cell->ty[ depth ]  = ty;
  cell->dx[ depth ]  = dx;
  cell->dy[ depth ]  = dy;
}

void ascii_set(int depth, int x, int y, const Tilep tile)
{
  if ((ascii_ok_for_scissors(x, y) == 0)) [[unlikely]] {
    return;
  }

  AsciiCell *cell = &(*cells)[ x ][ y ];

  cell->ch[ depth ]   = 0;
  cell->tile[ depth ] = tile;
  cell->tx[ depth ]   = 0;
  cell->ty[ depth ]   = 0;
  cell->dx[ depth ]   = 1;
  cell->dy[ depth ]   = 1;
}

void ascii_set(int depth, int x, int y, const Tilep tile, char ch)
{
  if ((ascii_ok_for_scissors(x, y) == 0)) [[unlikely]] {
    return;
  }

  AsciiCell *cell = &(*cells)[ x ][ y ];

  cell->ch[ depth ]   = ch;
  cell->tile[ depth ] = tile;
  cell->tx[ depth ]   = 0;
  cell->ty[ depth ]   = 0;
  cell->dx[ depth ]   = 1;
  cell->dy[ depth ]   = 1;
}

void ascii_set(int depth, int x, int y, const Tilep tile, float tx, float ty, float dx, float dy)
{
  if ((ascii_ok_for_scissors(x, y) == 0)) [[unlikely]] {
    return;
  }

  AsciiCell *cell = &(*cells)[ x ][ y ];

  cell->ch[ depth ]   = 0;
  cell->tile[ depth ] = tile;
  cell->tx[ depth ]   = tx;
  cell->ty[ depth ]   = ty;
  cell->dx[ depth ]   = dx;
  cell->dy[ depth ]   = dy;
}

#if 0
static void ascii_set(int depth, int x, int y, const char *tilename) { ascii_set(depth, x, y, tile_find(tilename)); }
#endif

void ascii_set(int depth, int x, int y, const char ch) { ascii_set(depth, x, y, font_ui->font_get_tile(ch), ch); }

void ascii_putf_internal2(int x, int y, color fg, color bg, const std::string &text)
{
  Tilep tile      = nullptr;
  int   bg_set    = 0;
  auto  text_iter = text.begin();

  // printf("ascii_putf_internal2 [%s]/%ld scissors x %d y %d scissors %d %d %d %d %d\n", text.c_str(), text.size(),
  // x, y,
  //        scissors_tl.x, scissors_tl.y, scissors_br.x, scissors_br.y, scissors_enabled);

  //
  // Check for out of bounds. Cannot check for x here as a message could start off screen and end on screen.
  //
  if ((y < 0)) [[unlikely]] {
    return;
  }

  if ((y >= TERM_HEIGHT)) [[unlikely]] {
    return;
  }

  if (color_eq(bg, COLOR_NONE)) {
    bg_set = 1;
  }

  tile         = nullptr;
  bool got_pct = false;
  char ch      = 0;

  for (;;) {
    if (text.end() - text_iter <= 0) {
      break;
    }

    if (text_iter == text.end()) {
      if (! got_pct) {
        break;
      }
      got_pct = false;
    } else {
      ch = *text_iter;
      text_iter++;

      auto len = text.end() - text_iter;
      if (len > 0) {
        if (ch == L'%') [[unlikely]] {
          got_pct = true;
          if ((len > 3) && (std::string(text_iter, text_iter + 3) == "fg=")) {
            text_iter += 3;
            auto tmp  = std::string(text_iter, text.end());
            int  slen = 0;
            fg        = string2color(tmp, &slen);
            text_iter += slen + 1;
            got_pct = false;
            continue;
          }
          if ((len > 3) && (std::string(text_iter, text_iter + 3) == "bg=")) {
            text_iter += 3;
            auto tmp  = std::string(text_iter, text.end());
            int  slen = 0;
            bg        = string2color(tmp, &slen);
            text_iter += slen + 1;

            bg_set  = 1;
            got_pct = false;
            continue;
          }
          if ((len > 4) && (std::string(text_iter, text_iter + 4) == "tex=")) {
            text_iter += 4;
            got_pct = false;
            continue;
          }
          if ((len > 3) && (std::string(text_iter, text_iter + 3) == "tp=")) {
            text_iter += 3;
            auto        tmp  = std::string(text_iter, text.end());
            int         slen = 0;
            const char *c    = tmp.c_str();
            auto       *tp   = string2tp(&c, &slen);
            text_iter += slen - 1;

            tile    = tp_first_tile(tp, THING_ANIM_IDLE);
            got_pct = false;
            continue;
          }
          if ((len > 5) && (std::string(text_iter, text_iter + 5) == "tile=")) {
            text_iter += 5;
            auto tmp  = std::string(text_iter, text.end());
            int  slen = 0;
            tile      = string2tile(tmp, &slen);
            text_iter += slen - 1;
            got_pct = false;
            continue;
          }
          continue;
        }
      }
    }

    //
    // Outside the scissors, ignore
    //
    if ((ascii_ok_for_scissors(x, y) == 0)) [[unlikely]] {
      x++;
      continue;
    }

    //
    // If not found print a ? tile
    //
    if (tile == nullptr) {
      tile = font_ui->font_get_tile(ch);
      if (tile == nullptr) {
        tile = tile_find_mand(FONT_TILENAME_UNKNOWN_STR);
      }
    }

    auto saved_fg = fg;

    //
    // Use a special char to represent the cursor. A bit of a hack.
    //
    auto is_cursor = (ch == static_cast< char > FONT_CHAR_CURSOR);
    if ((is_cursor)) [[unlikely]] {
      static uint32_t last;
      static bool     first = 1u;

      if (first) {
        first = 0u;
        last  = time_ms_cached();
      }

      //
      // Allow the cursor to change color. A bit of a hack. Again.
      //
      if (time_have_x_tenths_passed_since(10, last)) {
        fg   = UI_CURSOR_COLOR;
        last = time_ms_cached();
      } else if (time_have_x_tenths_passed_since(5, last)) {
        fg = UI_CURSOR_COLOR;
      } else {
        fg = UI_CURSOR_OTHER_COLOR;
      }
    }

    AsciiCell *cell = &(*cells)[ x ][ y ];
    x++;

    auto fg_depth = TILE_LAYER_FG_0;

    cell->tile[ fg_depth ]     = tile;
    cell->ch[ fg_depth ]       = ch;
    cell->color_tl[ fg_depth ] = fg;
    cell->color_tr[ fg_depth ] = fg;
    cell->color_bl[ fg_depth ] = fg;
    cell->color_br[ fg_depth ] = fg;

    if (bg_set != 0) {
      //
      // If we are displaying a color in the background then use a solid tile.
      //
      auto bg_depth = TILE_LAYER_BG_0;

      if ((static_cast< bool >(bg.r)) || (static_cast< bool >(bg.g)) || (static_cast< bool >(bg.b)) || (static_cast< bool >(bg.a))) {
        static Tilep block_tile;
        if (block_tile == nullptr) [[unlikely]] {
          block_tile = tile_find_mand(FONT_TILENAME_BLOCK_STR);
        }
        cell->tile[ bg_depth ] = block_tile;
      } else {
        //
        // Else clear the background
        //
        cell->tile[ bg_depth ] = nullptr;
      }

      cell->ch[ bg_depth ]       = ch;
      cell->color_tl[ bg_depth ] = bg;
      cell->color_tr[ bg_depth ] = bg;
      cell->color_bl[ bg_depth ] = bg;
      cell->color_br[ bg_depth ] = bg;
    }

    if ((is_cursor)) [[unlikely]] {
      fg = saved_fg;
    }

    tile = nullptr;
  }
}

auto ascii_strlen(std::string const &text) -> int
{
  auto text_iter = text.begin();
  int  x         = 0;

  bool got_pct = false;
  char ch      = 0;

  for (;;) {
    if (text.end() - text_iter <= 0) {
      break;
    }

    if (text_iter == text.end()) {
      if (! got_pct) {
        break;
      }
      got_pct = false;
    } else {
      ch = *text_iter;
      text_iter++;

      if (text_iter != text.end()) {
        if (ch == L'%') {
          if (text_iter != text.end()) {
            if (*text_iter == L'%') {
              text_iter++;
            }
          }

          auto len = text.end() - text_iter;
          if (len > 0) {
            if ((len > 3) && (std::string(text_iter, text_iter + 3) == "fg=")) {
              text_iter += 3;
              if (text_iter != text.end()) {
                auto tmp = std::string(text_iter, text.end());

                int slen = 0;
                (void) string2color(tmp, &slen);
                text_iter += slen + 1;
              }
              continue;
            }
            if ((len > 3) && (std::string(text_iter, text_iter + 3) == "bg=")) {
              text_iter += 3;
              auto tmp = std::string(text_iter, text.end());

              int slen = 0;
              (void) string2color(tmp, &slen);
              text_iter += slen + 1;

              continue;
            }
            if ((len > 3) && (std::string(text_iter, text_iter + 3) == "tp=")) {
              text_iter += 3;
              auto        tmp  = std::string(text_iter, text.end());
              int         slen = 0;
              const char *c    = tmp.c_str();
              (void) string2tp(&c, &slen);
              text_iter += slen - 1;
              got_pct = false;
              continue;
            }
            if ((len > 4) && (std::string(text_iter, text_iter + 4) == "tex=")) {
              text_iter += 4;
              got_pct = false;
              continue;
            }
            if ((len > 5) && (std::string(text_iter, text_iter + 5) == "tile=")) {
              text_iter += 5;
              auto tmp  = std::string(text_iter, text.end());
              int  slen = 0;
              (void) string2tile(tmp, &slen);
              text_iter += slen - 1;
              got_pct = false;
              continue;
            }
          } else {
            break;
          }
        }
      }
    }

    x++;
  }

  return x;
}

auto ascii_strip(std::string const &text) -> std::string
{
  auto        text_iter = text.begin();
  std::string out;

  for (;;) {
    auto ch = *text_iter;
    text_iter++;

    if (ch == '\0') {
      break;
    }

    if (ch == '%') {
      if (text_iter != text.end()) {
        if (*text_iter == '%') {
          text_iter++;
        }
      }

      if (std::string(text_iter, text_iter + 3) == "fg=") {
        text_iter += 3;
        auto tmp  = std::string(text_iter, text.end());
        int  slen = 0;
        (void) string2color(tmp, &slen);
        text_iter += slen + 1;
        continue;
      }
      if (std::string(text_iter, text_iter + 3) == "bg=") {
        text_iter += 3;
        auto tmp  = std::string(text_iter, text.end());
        int  slen = 0;
        (void) string2color(tmp, &slen);
        text_iter += slen + 1;
        continue;
      }
      if (std::string(text_iter, text_iter + 3) == "tp=") {
        text_iter += 3;
        auto        tmp  = std::string(text_iter, text.end());
        int         slen = 0;
        const char *c    = tmp.c_str();
        (void) string2tp(&c, &slen);
        text_iter += slen - 1;
        continue;
      }
      if (std::string(text_iter, text_iter + 4) == "tex=") {
        text_iter += 4;
        continue;
      }
      if (std::string(text_iter, text_iter + 5) == "tile=") {
        text_iter += 5;
        auto tmp  = std::string(text_iter, text.end());
        int  slen = 0;
        (void) string2tile(tmp, &slen);
        text_iter += slen - 1;
        continue;
      }
    }
    out += ch;
  }
  return out;
}

static void ascii_putf_internal(int x, int y, color fg, color bg, const char *fmt, va_list args)
{
  char buf[ MAXLONGSTR ];

  auto wrote = vsnprintf(buf, MAXLONGSTR, fmt, args);

  //
  // Only a single nul is written, but as we read 2 at a time...
  //
  if ((wrote != 0) && (wrote < MAXLONGSTR - 1)) {
    buf[ wrote + 1 ] = '\0';
  }

  auto b = std::string(buf);

  ascii_putf_internal2(x, y, fg, bg, b);
}

void ascii_putf(int x, int y, const char *fmt, ...)
{
  va_list args = {};

  va_start(args, fmt);
  ascii_putf_internal(x, y, WHITE, COLOR_NONE, fmt, args);
  va_end(args);
}

void ascii_putf(int x, int y, color fg, const char *fmt, ...)
{
  va_list args = {};

  va_start(args, fmt);
  ascii_putf_internal(x, y, fg, COLOR_NONE, fmt, args);
  va_end(args);
}

void ascii_putf(int x, int y, color fg, color bg, const char *fmt, ...)
{
  va_list args = {};

  va_start(args, fmt);
  ascii_putf_internal(x, y, fg, bg, fmt, args);
  va_end(args);
}

#ifdef ENABLE_UI_ASCII_MOUSE
static void ascii_display_mouse(spoint mouse_tile_tl, spoint mouse_tile_br, spoint mouse_at)
{
  GLCOLOR(WHITE);

  blit_init();
  tile_blit(tile_find_mand(FONT_TILENAME_POINTER_STR), spoint(mouse_tile_tl.x, mouse_tile_tl.y), spoint(mouse_tile_br.x, mouse_tile_br.y));
  blit_flush();
  //
  // Save where we are at
  //
  ascii.mouse_at = mouse_at;
}
#endif

void ascii_put_bg_square(int tlx, int tly, int brx, int bry, Tilep tile, color col)
{
  int x = 0;
  int y = 0;

  for (x = tlx; x <= brx; x++) {
    for (y = tly; y <= bry; y++) {
      ascii_set(TILE_LAYER_BG_0, x, y, tile);
      ascii_set(TILE_LAYER_BG_0, x, y, col);
    }
  }
}

void ascii_put_bg_square(int tlx, int tly, int brx, int bry, const char *tilename, color col)
{
  ascii_put_bg_square(tlx, tly, brx, bry, tile_find(tilename), col);
}

void ascii_put_bg_square(int tlx, int tly, int brx, int bry, char what, color col)
{
  ascii_put_bg_square(tlx, tly, brx, bry, font_ui->font_get_tile(what), col);
}

static void ascii_map_thing_replace(int depth, int x, int y, Tilep tile, color col)
{
  ascii_set(depth, x, y, tile);
  ascii_set(depth, x, y, col);
}

using ascii_draw_char_fn = void (*)(int depth, int x, int y, char ch);
using ascii_draw_col_fn  = void (*)(int depth, int x, int y, color col);

static void ascii_draw_line(int depth, int x0, int y0, int x1, int y1, Tilep tile, color col)
{
  int const dx  = abs(x1 - x0);
  int const sx  = x0 < x1 ? 1 : -1;
  int const dy  = -abs(y1 - y0);
  int const sy  = y0 < y1 ? 1 : -1;
  int       err = dx + dy;
  int       e2  = 0; /* error value e_xy */

  for (;;) { /* loop */
    ascii_map_thing_replace(depth, x0, y0, tile, col);
    if (x0 == x1 && y0 == y1) {
      break;
    }
    e2 = 2 * err;
    if (e2 >= dy) {
      err += dy;
      x0 += sx;
    } /* e_xy+e_x > 0 */
    if (e2 <= dx) {
      err += dx;
      y0 += sy;
    } /* e_xy+e_y < 0 */
  }
}

void ascii_draw_line(int depth, int x0, int y0, int x1, int y1, char what, color col)
{
  ascii_draw_line(depth, x0, y0, x1, y1, font_ui->font_get_tile(what), col);
}

#if 0
static void ascii_draw_line(int depth, int x0, int y0, int x1, int y1, const char *tilename, color col)
{
  ascii_draw_line(depth, x0, y0, x1, y1, tile_find(tilename), col);
}
#endif

//
// Display one z layer of the ascii.
//
static void ascii_blit(Gamep g)
{
  int x      = 0;
  int y      = 0;
  int tile_x = 0;
  int tile_y = 0;

  GLCOLOR(WHITE);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

#ifdef ENABLE_UI_ASCII_MOUSE
  float mx = sdl.mouse_x;
  float my = sdl.mouse_y;
#endif
  const auto dw = game_ascii_pix_width_get(g);
  const auto dh = game_ascii_pix_height_get(g);

  tile_y = 0;
  for (y = 0; y < TERM_HEIGHT; y++) {

    tile_x = 0;
    for (x = 0; x < TERM_WIDTH; x++) {

      const AsciiCell *cell = &(*cells)[ x ][ y ];

      spoint tile_tl = {};
      spoint tile_br = {};

      tile_tl.x = tile_x;
      tile_tl.y = tile_y;
      tile_br.x = tile_x + dw;
      tile_br.y = tile_y + dh;

#ifdef ENABLE_UI_ASCII_MOUSE
      //
      // Get the mouse position to use. We use this to find the mouse tile that we are over.
      //
      if (! mouse_found) {
        if ((mx < tile_br.x) && (my < tile_br.y) && (mx >= tile_tl.x) && (my >= tile_tl.y)) {
          mouse_found = true;

          mouse_tile_tl = tile_tl;
          mouse_tile_br = tile_br;

          ascii.mouse_at = point(x, y);
        }
      }
#endif

      //
      // Background
      //
      auto depth = TILE_LAYER_BG_0;
      if (cell->tex[ depth ] != nullptr) {
        Texp tex = cell->tex[ depth ];

        blit(tex_get_gl_binding(tex), cell->tx[ depth ], cell->ty[ depth ], cell->tx[ depth ] + cell->dx[ depth ],
             cell->ty[ depth ] + cell->dy[ depth ], tile_tl.x, tile_tl.y, tile_br.x, tile_br.y, WHITE);
      } else if (cell->tile[ depth ] != nullptr) {
        color const bg_color_tl = cell->color_tl[ depth ];
        color const bg_color_tr = cell->color_tr[ depth ];
        color const bg_color_bl = cell->color_bl[ depth ];
        color const bg_color_br = cell->color_br[ depth ];

        tile_blit(cell->tile[ depth ], tile_tl, tile_br, bg_color_tl, bg_color_tr, bg_color_bl, bg_color_br);
      }

      tile_x += dw;
    }

    tile_y += dh;
  }

  //
  // Tiles
  //
  tile_y = 0;
  for (y = 0; y < TERM_HEIGHT; y++) {

    tile_x = 0;
    for (x = 0; x < TERM_WIDTH; x++) {

      const AsciiCell *cell = &(*cells)[ x ][ y ];

      spoint tile_tl = {};
      spoint tile_br = {};

      tile_tl.x = tile_x;
      tile_tl.y = tile_y;
      tile_br.x = tile_x + dw;
      tile_br.y = tile_y + dh;

      {
        auto  depth = TILE_LAYER_BG_1;
        auto *tile  = cell->tile[ depth ];
        if (tile != nullptr) {
          color const color_tl = cell->color_tl[ depth ];
          color const color_tr = cell->color_tr[ depth ];
          color const color_bl = cell->color_bl[ depth ];
          color const color_br = cell->color_br[ depth ];

          tile_blit_section(cell->tile[ depth ], fpoint(cell->tx[ depth ], cell->ty[ depth ]),
                            fpoint(cell->tx[ depth ] + cell->dx[ depth ], cell->ty[ depth ] + cell->dy[ depth ]), tile_tl, tile_br,
                            color_tl, color_tr, color_bl, color_br);
        }
      }

      for (int depth = TILE_LAYER_FG_1; depth < TILE_LAYER_ENUM_MAX; depth++) {
        auto *tile = cell->tile[ depth ];
        if (tile != nullptr) {
          color const color_tl = cell->color_tl[ depth ];
          color const color_tr = cell->color_tr[ depth ];
          color const color_bl = cell->color_bl[ depth ];
          color const color_br = cell->color_br[ depth ];

          tile_blit_section(tile, fpoint(cell->tx[ depth ], cell->ty[ depth ]),
                            fpoint(cell->tx[ depth ] + cell->dx[ depth ], cell->ty[ depth ] + cell->dy[ depth ]), tile_tl, tile_br,
                            color_tl, color_tr, color_bl, color_br);
        }
      }

      tile_x += dw;
    }

    tile_y += dh;
  }

  tile_y = 0;
  for (y = 0; y < TERM_HEIGHT; y++) {
    tile_x = 0;
    for (x = 0; x < TERM_WIDTH; x++) {
      const AsciiCell *cell = &(*cells)[ x ][ y ];

      spoint tile_tl = {};
      spoint tile_br = {};

      tile_tl.x = tile_x;
      tile_tl.y = tile_y;
      tile_br.x = tile_x + dw;
      tile_br.y = tile_y + dh;

      //
      // Foreground
      //
      {
        auto  depth = TILE_LAYER_FG_0;
        Tilep tile  = cell->tile[ depth ];

        if (tile != nullptr) {
          //
          // As the font is not square, if showing a non square tile, then make it so
          //
          if (tile_width(tile) != UI_FONT_WIDTH) {
            tile_br.y       = tile_y + dw;
            auto centralize = (dh - dw) / 2;
            tile_tl.y += centralize;
            tile_br.y += centralize;
          }

          color const fg_color_tl = cell->color_tl[ depth ];
          color const fg_color_tr = cell->color_tr[ depth ];
          color const fg_color_bl = cell->color_bl[ depth ];
          color const fg_color_br = cell->color_br[ depth ];

          tile_blit(tile, tile_tl, tile_br, fg_color_tl, fg_color_tr, fg_color_bl, fg_color_br);
        }
      }

      tile_x += dw;
    }

    tile_y += dh;
  }
}

//
// The big ascii renderer
//
void ascii_display(Gamep g)
{
  mouse_found = 0;

  gl_enter_2d_mode(g, game_window_pix_width_get(g), game_window_pix_height_get(g));

  blit_fbo_bind_locked(FBO_WID);
  {
    gl_clear();
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    blit_init();
    ascii_blit(g);
    blit_flush();

#ifdef ENABLE_UI_ASCII_MOUSE
    if (mouse_found) {
      ascii_display_mouse(mouse_tile_tl, mouse_tile_br, ascii.mouse_at);
    }
#endif
  }
  blit_fbo_unbind_locked();
}

void ascii_clear_display()
{
  if (cells == nullptr) {
    cells = new std::array< std::array< AsciiCell, TERM_HEIGHT_MAX >, TERM_WIDTH_MAX >;
  }

  /*
  delete cells;
  cells = new std::array< std::array< AsciiCell, TERM_HEIGHT_MAX >, TERM_WIDTH_MAX >;
   */

  for (auto y = 0; y < TERM_HEIGHT; y++) {
    for (auto x = 0; x < TERM_WIDTH; x++) {
      (*cells)[ x ][ y ] = {};
    }
  }
}

static void ascii_put_box_(int style, const TileLayers tiles_in, int x1, int y1, int x2, int y2, color col_bg, color col_fg, void *context)
{
  static bool      init;
  static const int MAX_UI_SIZE    = 16;
  static const int MAX_UI_BG_SIZE = MAX_UI_SIZE - 2;
  static std::array< std::array< std::array< std::array< Tilep, MAX_UI_SIZE >, MAX_UI_SIZE >, UI_WID_STYLE_MAX >, UI_TYPES_MAX > tiles = {};

  if (style >= UI_WID_STYLE_MAX) {
    err("unimplemented widget style %d", style);
    return;
  }

  int const ui_type = UI_TYPE_PIXELART;

  if (! init) {
    init = true;
    for (auto styles = 0; styles < UI_WID_STYLE_MAX; styles++) {
      for (auto x = 0; x < MAX_UI_SIZE; x++) {
        for (auto y = 0; y < MAX_UI_SIZE; y++) {
          std::string const name               = "ui." + std::to_string(styles) + "," + std::to_string(x) + "," + std::to_string(y);
          tiles[ ui_type ][ styles ][ x ][ y ] = tile_find_mand(name);
        }
      }
    }
  }

  {
    float const dx = static_cast< float >(1.0) / ((static_cast< float >(x2)) - (static_cast< float >(x1)) + 1);
    float const dy = static_cast< float >(1.0) / ((static_cast< float >(y2)) - (static_cast< float >(y1)) + 1);

    for (auto x = x1; x <= x2; x++) {
      for (auto y = y1; y <= y2; y++) {
        float const tx = (static_cast< float >(x) - x1) * dx;
        float const ty = (static_cast< float >(y) - y1) * dy;

        {
          auto depth = TILE_LAYER_BG_0;
          if ((tiles_in[ depth ] != nullptr) || color_neq(col_bg, COLOR_NONE)) {
            ascii_set(depth + 1, x, y, tiles_in[ depth ], tx, ty, dx, dy);
            ascii_set(depth + 1, x, y, col_bg);
          }
        }

        for (int depth = TILE_LAYER_FG_0; depth < TILE_LAYER_ENUM_MAX - 1; depth++) {
          {
            if ((tiles_in[ depth ] != nullptr) || color_neq(col_fg, COLOR_NONE)) {
              ascii_set(depth + 1, x, y, tiles_in[ depth ], tx, ty, dx, dy);
              ascii_set(depth + 1, x, y, col_fg);
            }
          }
        }

        ascii_set(TILE_LAYER_FG_0, x, y, ' ');
      }
    }
  }

  if (y1 == y2) {
    //
    // Horizontal
    //
    auto y = y1;
    for (auto x = x1; x <= x2; x++) {
      if (style >= 0) {
        ascii_set(TILE_LAYER_BG_0, x, y, tiles[ ui_type ][ style ][ (x % MAX_UI_BG_SIZE) + 1 ][ 0 ]);
      }
      ascii_set(TILE_LAYER_BG_0, x, y, col_bg);
    }
    if (style >= 0) {
      ascii_set(TILE_LAYER_BG_0, x1, y, tiles[ ui_type ][ style ][ 0 ][ 0 ]);
      ascii_set(TILE_LAYER_BG_0, x2, y, tiles[ ui_type ][ style ][ MAX_UI_SIZE - 1 ][ 0 ]);
    }
    return;
  }

  if (x1 == x2) {
    //
    // Vertical
    //
    auto x = x1;
    for (auto y = y1; y <= y2; y++) {
      if (style >= 0) {
        ascii_set(TILE_LAYER_BG_0, x, y, tiles[ ui_type ][ style ][ 0 ][ (y % MAX_UI_BG_SIZE) + 1 ]);
      }
      ascii_set(TILE_LAYER_BG_0, x, y, col_bg);
    }
    if (style >= 0) {
      ascii_set(TILE_LAYER_BG_0, x, y1, tiles[ ui_type ][ style ][ 0 ][ 0 ]);
      ascii_set(TILE_LAYER_BG_0, x, y2, tiles[ ui_type ][ style ][ 0 ][ MAX_UI_SIZE - 1 ]);
    }
    return;
  }

  for (auto y = y1; y <= y2; y++) {
    for (auto x = x1; x <= x2; x++) {
      if (style >= 0) {
        ascii_set(TILE_LAYER_BG_0, x, y, tiles[ ui_type ][ style ][ (x % MAX_UI_BG_SIZE) + 1 ][ (y % MAX_UI_BG_SIZE) + 1 ]);
      }
      ascii_set(TILE_LAYER_BG_0, x, y, col_bg);
    }
  }

  for (auto y = y1 + 1; y <= y2; y++) {
    for (auto x = x1 + 1; x <= x2; x++) {
      if (style >= 0) {
        ascii_set(TILE_LAYER_BG_0, x, y, tiles[ ui_type ][ style ][ (x % MAX_UI_BG_SIZE) + 1 ][ (y % MAX_UI_BG_SIZE) + 1 ]);
      }
      ascii_set(TILE_LAYER_BG_0, x, y, col_bg);
    }
    if (style >= 0) {
      ascii_set(TILE_LAYER_BG_0, x1, y2, tiles[ ui_type ][ style ][ (x1 % MAX_UI_BG_SIZE) + 1 ][ (y2 % MAX_UI_BG_SIZE) + 1 ]);
    }
    ascii_set(TILE_LAYER_BG_0, x1, y2, col_bg);
  }

  for (auto y = y1 + 1; y <= y2 - 1; y++) {
    for (auto x = x1 + 1; x <= x2 - 1; x++) {
      if (style >= 0) {
        ascii_set(TILE_LAYER_BG_0, x, y, tiles[ ui_type ][ style ][ (x % MAX_UI_BG_SIZE) + 1 ][ (y % MAX_UI_BG_SIZE) + 1 ]);
      }
      ascii_set(TILE_LAYER_BG_0, x, y, col_bg);
    }
  }

  for (auto x = x1; x <= x2; x++) {
    for (auto y = y1; y <= y2; y++) {
      ascii_set_context(x, y, context);
      if (style >= 0) {
        ascii_set(TILE_LAYER_BG_0, x, y, tiles[ ui_type ][ style ][ (x % MAX_UI_BG_SIZE) + 1 ][ (y % MAX_UI_BG_SIZE) + 1 ]);
      }
      ascii_set(TILE_LAYER_BG_0, x, y, col_bg);
    }
  }

  for (auto x = x1 + 1; x <= x2 - 1; x++) {
    if (style >= 0) {
      ascii_set(TILE_LAYER_BG_0, x, y1, tiles[ ui_type ][ style ][ (x % MAX_UI_BG_SIZE) + 1 ][ 0 ]);
      ascii_set(TILE_LAYER_BG_0, x, y2, tiles[ ui_type ][ style ][ (x % MAX_UI_BG_SIZE) + 1 ][ MAX_UI_SIZE - 1 ]);
    }
  }

  for (auto y = y1 + 1; y <= y2 - 1; y++) {
    if (style >= 0) {
      ascii_set(TILE_LAYER_BG_0, x1, y, tiles[ ui_type ][ style ][ 0 ][ (y % MAX_UI_BG_SIZE) + 1 ]);
      ascii_set(TILE_LAYER_BG_0, x2, y, tiles[ ui_type ][ style ][ MAX_UI_SIZE - 1 ][ (y % MAX_UI_BG_SIZE) + 1 ]);
    }
  }

  if (style >= 0) {
    ascii_set(TILE_LAYER_BG_0, x1, y1, tiles[ ui_type ][ style ][ 0 ][ 0 ]);
    ascii_set(TILE_LAYER_BG_0, x2, y2, tiles[ ui_type ][ style ][ MAX_UI_SIZE - 1 ][ MAX_UI_SIZE - 1 ]);
    ascii_set(TILE_LAYER_BG_0, x2, y1, tiles[ ui_type ][ style ][ MAX_UI_SIZE - 1 ][ 0 ]);
    ascii_set(TILE_LAYER_BG_0, x1, y2, tiles[ ui_type ][ style ][ 0 ][ MAX_UI_SIZE - 1 ]);
  }
}

static void ascii_put_box_(int style, const TileLayers tiles, int x, int y, int width, int height, color col_bg, color col_text,
                           const char *fmt, va_list args)
{
  if (*fmt == 0) {
    ascii_put_box_(style, tiles, x, y, x + width - 1, y + height - 1, col_bg, col_text, nullptr /* context */);
  } else {
    char buf[ MAXLONGSTR ];
    auto wrote = vsnprintf(buf, MAXLONGSTR, fmt, args);

    //
    // Only a single nul is written, but as we read 2 at a time...
    //
    if ((wrote != 0) && (wrote < MAXLONGSTR - 1)) {
      buf[ wrote + 1 ] = '\0';
    }

    auto      b   = std::string(buf);
    int const len = ascii_strlen(b);

    ascii_put_box_(style, tiles, x, y, x + width - 1, y + height - 1, col_bg, col_text, nullptr /* context */);

    ascii_putf_internal2(x + ((width - len) / 2), y + 1, col_text, COLOR_NONE, b);
  }
}

void ascii_put_box(box_args b, int style, const TileLayers tiles, const char *fmt, ...)
{
  va_list args = {};

  if ((b.width == 0) || (b.height == 0)) {
    b.x      = 0;
    b.y      = 0;
    b.width  = TERM_WIDTH;
    b.height = TERM_HEIGHT;
  }

  int       x = b.x;
  int       y = b.y;
  int const w = b.width;
  int const h = b.height;

  //
  // Draw the box
  //
  va_start(args, fmt);

  ascii_put_box_(style, tiles, b.x, b.y, b.width, b.height, b.col_bg, b.col_text, fmt, args);

  va_end(args);

  //
  // Populate the ascii callbacks for this box.
  //
  int x1 = 0;
  int x2 = 0;
  int y1 = 0;
  int y2 = 0;

  x1 = x;
  y1 = y;
  x2 = x + w;
  y2 = y + h;

  for (x = x1; x <= x2; x++) {
    if ((ascii_x_ok(x) == 0)) [[unlikely]] {
      continue;
    }
    for (y = y1; y <= y2; y++) {
      if ((ascii_y_ok(y) == 0)) [[unlikely]] {
        continue;
      }

      ascii.sdl_mod[ x ][ y ]      = b.sdl_mod;
      ascii.sdl_key[ x ][ y ]      = b.sdl_key;
      ascii.mouse_button[ x ][ y ] = b.mouse_button;

      //
      // Callbacks for ascii co-ords.
      //
      ascii.key_down[ x ][ y ]   = b.key_down;
      ascii.mouse_down[ x ][ y ] = b.mouse_down;
      ascii.mouse_over[ x ][ y ] = b.mouse_over;
    }
  }
}
