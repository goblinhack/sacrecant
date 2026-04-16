//
// Copyright goblinhack@gmail.com
//

#include <algorithm>
#include <numbers>

#include "config.hpp"
#include "my_ascii.hpp"
#include "my_callstack.hpp"
#include "my_game.hpp"
#include "my_main.hpp"
#include "my_music.hpp"
#include "my_random.hpp"
#include "my_sdl_proto.hpp"
#include "my_sound.hpp"
#include "my_wids.hpp"

static WidPopup *wid_main_menu_window;

void wid_main_menu_destroy(Gamep g)
{
  TRACE();

  delete wid_main_menu_window;
  wid_main_menu_window = nullptr;
}

static void wid_main_menu_hide(Gamep g)
{
  TRACE();

  if (wid_main_menu_window == nullptr) {
    return;
  }

  wid_hide(g, wid_main_menu_window->wid_popup_container);
}

[[nodiscard]] static auto wid_main_menu_load(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();
  wid_load_select(g);
  return true;
}

[[nodiscard]] static auto wid_main_menu_cfg(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();
  wid_options_menu_select(g);
  wid_main_menu_hide(g);
  return true;
}

[[nodiscard]] static auto wid_main_menu_more(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();
  wid_more_select(g);
  wid_main_menu_hide(g);
  return true;
}

[[nodiscard]] static auto game_menu_new_game(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();
  wid_main_menu_hide(g);
  wid_main_menu_destroy(g);

  if (g_intro_done++ >= 2) {
    wid_new_game(g);
  } else {
    wid_intro_select(g);
  }

  return false; // suppress mouse click
}

[[nodiscard]] static auto wid_main_menu_quit(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();
  wid_quit_select(g);
  wid_main_menu_destroy(g);
  return true;
}

[[nodiscard]] static auto wid_main_menu_key_down(Gamep g, Widp w, const struct SDL_Keysym *key) -> bool
{
  TRACE();

  if (sdlk_eq(*key, game_key_console_get(g))) {
    sound_play(g, "keypress");
    return false;
  }

  auto s = to_string(*key);

  if (s == "<Return>" || s == "n" || s == "N") {
    TRACE();
    sound_play(g, "keypress");
    (void) game_menu_new_game(g, nullptr, 0, 0, 0);
    return true;
  }

  if (s == "l" || s == "L") {
    sound_play(g, "keypress");
    (void) wid_main_menu_load(g, nullptr, 0, 0, 0);
    return true;
  }

  if (s == "o" || s == "O") {
    sound_play(g, "keypress");
    (void) wid_main_menu_cfg(g, nullptr, 0, 0, 0);
    return true;
  }

  if (s == "m" || s == "M") {
    sound_play(g, "keypress");
    (void) wid_main_menu_more(g, nullptr, 0, 0, 0);
    return true;
  }

  if (s == "q" || s == "Q") {
    sound_play(g, "keypress");
    (void) wid_main_menu_quit(g, nullptr, 0, 0, 0);
    return true;
  }

  return false;
}

static void game_display_title_bg(Gamep g)
{
  TRACE();

  static color fg  = RED;
  static int   hue = 0;

  GLCOLOR(fg);
  hue = 1;
  if (hue > 255) {
    hue = 0;
  }

  fg   = color_change_hue(fg, hue);
  fg.a = 255;

  if (fg.r + fg.g + fg.b < 100) {
    fg = RED;
  }

  auto bright = static_cast< float >(1.01);
  int  r      = static_cast< int >((static_cast< float >(fg.r)) * bright);
  r           = std::min(r, 255);
  fg.r        = r;
  int green   = static_cast< int >((static_cast< float >(fg.g)) * bright);
  green       = std::min(green, 255);
  fg.g        = green;
  int b       = static_cast< int >((static_cast< float >(fg.b)) * bright);
  b           = std::min(b, 255);
  fg.b        = b;

  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  float       w = game_window_pix_width_get(g);
  float const h = game_window_pix_height_get(g);

  auto       *tile = tile_find_mand("main_bg");
  float const tw   = tile_width(tile);
  float const th   = tile_height(tile);

  w = (h * tw) / th;

  spoint tl(0, 0);
  spoint br(static_cast< int >(w), static_cast< int >(h));

  auto center = static_cast< int >((game_window_pix_width_get(g) - w) / 2);
  tl.x += center;
  br.x += center;

  blit_init();
  tile_blit(tile, tl, br, fg);
  blit_flush();
}

static void game_display_title_fg(Gamep g)
{
  TRACE();

  color const fg = WHITE;

  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  float       w = game_window_pix_width_get(g);
  float const h = game_window_pix_height_get(g);

  auto       *tile = tile_find_mand("main_fg");
  float const tw   = tile_width(tile);
  float const th   = tile_height(tile);

  w = (h * tw) / th;

  spoint tl(0, 0);
  spoint br(static_cast< int >(w), static_cast< int >(h));

  auto center = static_cast< int >((game_window_pix_width_get(g) - w) / 2);
  tl.x += center;
  br.x += center;

  blit_init();
  tile_blit(tile, tl, br, fg);
  blit_flush();
}

static auto clamp(float v) -> uint8_t // define a function to bound and round the input float value to 0-255
{
  TRACE();

  if (v < 0) {
    return 0;
  }
  if (v > 255) {
    return 255;
  }
  return static_cast< uint8_t >(v);
}

// https://stackoverflow.com/questions/8507885/shift-hue-of-an-rgb-color#8509802
auto color_change_hue(const color &in, const float fHue) -> color
{
  TRACE();

  color       out;
  const float cosA = cos(fHue * std::numbers::pi_v< float > / 180); // convert degrees to radians
  const float sinA = sin(fHue * std::numbers::pi_v< float > / 180); // convert degrees to radians
  // calculate the rotation matrix, only depends on Hue
  float const matrix[ 3 ][ 3 ] = {{cosA + ((1.0F - cosA) / 3.0F), (1.0F / 3.0F * (1.0F - cosA)) - (sqrtf(1.0F / 3.0F) * sinA),
                                   (1.0F / 3.0F * (1.0F - cosA)) + (sqrtf(1.0F / 3.0F) * sinA)},
                                  {(1.0F / 3.0F * (1.0F - cosA)) + (sqrtf(1.0F / 3.0F) * sinA), cosA + (1.0F / 3.0F * (1.0F - cosA)),
                                   (1.0F / 3.0F * (1.0F - cosA)) - (sqrtf(1.0F / 3.0F) * sinA)},
                                  {(1.0F / 3.0F * (1.0F - cosA)) - (sqrtf(1.0F / 3.0F) * sinA),
                                   (1.0F / 3.0F * (1.0F - cosA)) + (sqrtf(1.0F / 3.0F) * sinA), cosA + (1.0F / 3.0F * (1.0F - cosA))}};
  // Use the rotation matrix to convert the RGB directly
  out.r = clamp(((static_cast< float >(in.r)) * matrix[ 0 ][ 0 ]) + ((static_cast< float >(in.g)) * matrix[ 0 ][ 1 ])
                + ((static_cast< float >(in.b)) * matrix[ 0 ][ 2 ]));
  out.g = clamp(((static_cast< float >(in.r)) * matrix[ 1 ][ 0 ]) + ((static_cast< float >(in.g)) * matrix[ 1 ][ 1 ])
                + ((static_cast< float >(in.b)) * matrix[ 1 ][ 2 ]));
  out.b = clamp(((static_cast< float >(in.r)) * matrix[ 2 ][ 0 ]) + ((static_cast< float >(in.g)) * matrix[ 2 ][ 1 ])
                + ((static_cast< float >(in.b)) * matrix[ 2 ][ 2 ]));
  return out;
}

static void wid_main_menu_tick(Gamep g, Widp w)
{
  TRACE();

  game_display_title_fg(g);
  game_display_title_bg(g);

  if (wid_main_menu_window != nullptr) {
    ascii_putf(TERM_WIDTH - SIZEOF(MYVER), TERM_HEIGHT - 1, GREEN, BLACK, "v" MYVER);

    std::string const seed_name(game_seed_name_get(g));
    auto              seed_text = "Seed: '" + seed_name + "'";

    switch (game_seed_source_get(g)) {
      case SEED_SOURCE_COMMAND_LINE : seed_text += " (set via cli)"; break;
      case SEED_SOURCE_USER :         seed_text += " (set via user)"; break;
      case SEED_SOURCE_TEST :         seed_text += " (set via test option)"; break;
      case SEED_SOURCE_RANDOM :       seed_text += " (randomly generated)"; break;
    }

    ascii_putf(0, TERM_HEIGHT - 1, YELLOW, BLACK, seed_text.c_str());
  }
}

void wid_main_menu_select(Gamep g)
{
  log("main menu");
  TRACE();

  sound_fade_out(g);

  if (wid_main_menu_window == nullptr) {
    music_play(g, "intro");
  }

  if (wid_main_menu_window != nullptr) {
    wid_main_menu_destroy(g);
  }

  const auto button_height = 2;
  const auto button_step   = 3;
  const auto menu_height   = 16;
  const auto menu_width    = UI_WID_POPUP_WIDTH_NORMAL;

  auto y_at = 0;

  const spoint outer_tl((TERM_WIDTH / 2) - (menu_width / 2), (TERM_HEIGHT / 2) - (menu_height / 2));
  const spoint outer_br((TERM_WIDTH / 2) + (menu_width / 2), (TERM_HEIGHT / 2) + (menu_height / 2));

  wid_main_menu_window = new WidPopup(g, "Main menu", outer_tl, outer_br, nullptr, "nothing", false, false);

  auto button_width = outer_br.x - outer_tl.x - 2;

  {
    TRACE();
    Widp w = wid_main_menu_window->wid_popup_container;
    wid_set_on_key_down(w, wid_main_menu_key_down);
    wid_set_on_tick(w, wid_main_menu_tick);
  }

  {
    TRACE();
    auto *p = wid_main_menu_window->wid_text_area->wid_text_area;
    auto *w = wid_new_menu_button(g, p, "New Game");

    spoint const tl(0, y_at);
    spoint const br(button_width, y_at + button_height);
    wid_set_on_mouse_up(w, game_menu_new_game);
    wid_set_pos(w, tl, br);
    wid_set_text(w, UI_HIGHLIGHT_FMT_STR "N" UI_RESET_FMT "ew game" UI_RESET_FMT);
  }
  y_at += button_step;
  {
    TRACE();
    auto *p = wid_main_menu_window->wid_text_area->wid_text_area;
    auto *w = wid_new_menu_button(g, p, "Load Game");

    spoint const tl(0, y_at);
    spoint const br(button_width, y_at + button_height);
    wid_set_on_mouse_up(w, wid_main_menu_load);
    wid_set_pos(w, tl, br);
    wid_set_text(w, UI_HIGHLIGHT_FMT_STR "L" UI_FMT_STR "oad game");
  }
  y_at += button_step;
  {
    TRACE();
    auto *p = wid_main_menu_window->wid_text_area->wid_text_area;
    auto *w = wid_new_menu_button(g, p, "Options");

    spoint const tl(0, y_at);
    spoint const br(button_width, y_at + button_height);
    wid_set_on_mouse_up(w, wid_main_menu_cfg);
    wid_set_pos(w, tl, br);
    wid_set_text(w, UI_HIGHLIGHT_FMT_STR "O" UI_FMT_STR "ptions");
  }
  y_at += button_step;
  {
    TRACE();
    auto *p = wid_main_menu_window->wid_text_area->wid_text_area;
    auto *w = wid_new_menu_button(g, p, "More");

    spoint const tl(0, y_at);
    spoint const br(button_width, y_at + button_height);
    wid_set_on_mouse_up(w, wid_main_menu_more);
    wid_set_pos(w, tl, br);
    wid_set_text(w, UI_HIGHLIGHT_FMT_STR "M" UI_FMT_STR "ore");
  }
  y_at += button_step;
  {
    TRACE();
    auto *p = wid_main_menu_window->wid_text_area->wid_text_area;
    auto *w = wid_new_cancel_button(g, p, "Quit Game");

    spoint const tl(0, y_at);
    spoint const br(button_width, y_at + button_height);
    wid_set_on_mouse_up(w, wid_main_menu_quit);
    wid_set_pos(w, tl, br);
    wid_set_text(w, UI_HIGHLIGHT_FMT_STR "Q" UI_FMT_STR "uit Game");
  }

  wid_update(g, wid_main_menu_window->wid_text_area->wid_text_area);
}

void wid_new_game(Gamep g)
{
  log("new game");
  TRACE();

  game_state_reset(g, "new game");
  game_create_levels(g);
  game_map_zoom_in(g);
  game_start_playing(g);
  game_state_change(g, STATE_PLAYING, "new game");
}
