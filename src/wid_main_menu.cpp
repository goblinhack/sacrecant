//
// Copyright goblinhack@gmail.com
//

#include "config.hpp"
#include "my_ascii.hpp"
#include "my_callstack.hpp"
#include "my_color.hpp"
#include "my_color_defs.hpp"
#include "my_game.hpp"
#include "my_gl.hpp" // NOLINT
#include "my_globals.hpp"
#include "my_main.hpp" // NOLINT
#include "my_music.hpp"
#include "my_sdl_proto.hpp"
#include "my_sound.hpp"
#include "my_spoint.hpp"
#include "my_tile.hpp"
#include "my_types.hpp"
#include "my_ui.hpp"
#include "my_wid.hpp"
#include "my_wid_popup.hpp"
#include "my_wids.hpp"

#include <SDL_keyboard.h>
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <numbers>

static WidPopup *wid_main_menu_window;

void wid_main_menu_destroy(Gamep g)
{
  if (wid_main_menu_window == nullptr) {
    return;
  }

  con("Main menu: destroy");
  TRACE_INDENT();

  delete wid_main_menu_window;
  wid_main_menu_window = nullptr;
}

void wid_main_menu_hide(Gamep g)
{
  con("Main menu: hide");
  TRACE_INDENT();

  if (wid_main_menu_window == nullptr) {
    return;
  }

  wid_hide(g, wid_main_menu_window->wid_popup_container);
}

[[nodiscard]] static auto wid_main_menu_load(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  con("Main menu: load");
  TRACE_INDENT();

  wid_load_select(g);
  return true;
}

[[nodiscard]] static auto wid_main_menu_cfg(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  con("Main menu: cfg");
  TRACE_INDENT();

  wid_options_menu_select(g);
  wid_main_menu_hide(g);
  return true;
}

[[nodiscard]] static auto wid_main_menu_more(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  con("Main menu: more");
  TRACE_INDENT();

  wid_more_select(g);
  wid_main_menu_hide(g);
  return true;
}

[[nodiscard]] static auto game_menu_new_game(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  con("Main menu: new game");
  TRACE_INDENT();

  wid_main_menu_hide(g);
  wid_main_menu_destroy(g);

  (void) sound_play(g, "start");

  if (g_intro_done++ >= 2) {
    wid_player_select(g);
  } else {
    wid_intro1_select(g);
  }

  return true;
}

[[nodiscard]] static auto game_menu_weekly_seed(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  con("Main menu: weekly seed");
  TRACE_INDENT();

  game_seed_set(g, g_opt_seed_name_weekly.c_str());

  return game_menu_new_game(g, w, x, y, button);
}

[[nodiscard]] static auto game_menu_daily_seed(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  con("Main menu: daily seed");
  TRACE_INDENT();

  game_seed_set(g, g_opt_seed_name_daily.c_str());

  return game_menu_new_game(g, w, x, y, button);
}

[[nodiscard]] static auto game_menu_previous_seed(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  con("Main menu: previous seed");
  TRACE_INDENT();

  game_seed_set(g, g_opt_seed_name_previous.c_str());

  return game_menu_new_game(g, w, x, y, button);
}

[[nodiscard]] static auto wid_main_menu_quit(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  con("Main menu: quit");
  TRACE_INDENT();

  wid_quit_select(g);
  wid_main_menu_destroy(g);
  return true;
}

[[nodiscard]] static auto wid_main_menu_key_down(Gamep g, Widp w, const struct SDL_Keysym *key) -> bool
{
  con("Main menu: key down");
  TRACE_INDENT();

  if (sdlk_eq(*key, game_key_console_get(g))) {
    ((void) sound_play(g, "keypress"));
    return false;
  }

  auto s = to_string(*key);

  if (s == "<Space>" || s == "<Return>" || s == "n" || s == "N") {
    TRACE();
    ((void) sound_play(g, "keypress"));
    (void) game_menu_new_game(g, nullptr, 0, 0, 0);
    return true;
  }

  if (s == "l" || s == "L") {
    ((void) sound_play(g, "keypress"));
    (void) wid_main_menu_load(g, nullptr, 0, 0, 0);
    return true;
  }

  if (s == "o" || s == "O") {
    ((void) sound_play(g, "keypress"));
    (void) wid_main_menu_cfg(g, nullptr, 0, 0, 0);
    return true;
  }

  if (s == "m" || s == "M") {
    ((void) sound_play(g, "keypress"));
    (void) wid_main_menu_more(g, nullptr, 0, 0, 0);
    return true;
  }

  if (s == "q" || s == "Q") {
    ((void) sound_play(g, "keypress"));
    (void) wid_main_menu_quit(g, nullptr, 0, 0, 0);
    return true;
  }

  if (s == "p" || s == "P") {
    (void) sound_play(g, "keypress");
    (void) game_menu_previous_seed(g, nullptr, 0, 0, 0);
    return true;
  }

  if (s == "w" || s == "W") {
    (void) sound_play(g, "keypress");
    (void) game_menu_weekly_seed(g, nullptr, 0, 0, 0);
    return true;
  }

  if (s == "d" || s == "D") {
    (void) sound_play(g, "keypress");
    (void) game_menu_daily_seed(g, nullptr, 0, 0, 0);
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

  auto *tile = tile_find_mand("main_bg");

  if (compiler_unused) {
    float const tw = tile_width(tile);
    float const th = tile_height(tile);

    w = (h * tw) / th;
  }

  spoint tl(0, 0);
  spoint br(static_cast< int >(w), static_cast< int >(h));

  if (compiler_unused) {
    auto center = static_cast< int >((game_window_pix_width_get(g) - w) / 2);
    tl.x += center;
    br.x += center;
  }

  blit_init();
  if (compiler_unused) {
    tile_blit(tile, tl, br, fg);
  } else {
    tile_blit(tile, tl, br, WHITE);
  }
  blit_flush();
}

static void game_display_title_fg(Gamep g)
{
  TRACE();

  color const fg = WHITE;

  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  float       w = game_window_pix_width_get(g);
  float const h = game_window_pix_height_get(g);

  auto *tile = tile_find_mand("main_fg");

  if (! wid_is_visible(wid_main_menu_window->wid_popup_container)) {
    tile = tile_find_mand("main_alt_fg");
  }

  float const tw = tile_width(tile);
  float const th = tile_height(tile);

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
[[nodiscard]] auto color_change_hue(const color &in, const float fHue) -> color
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
  if (g_quitting) {
    return;
  }

  con("Main menu: select");
  TRACE_INDENT();

  //
  // Ensure no previous game gfx hang around from the last run
  //
  gl_clear_fbo_all();

  if (wid_main_menu_window == nullptr) {
    (void) music_play(g, "intro");
  }

  if (wid_main_menu_window != nullptr) {
    wid_main_menu_destroy(g);
  }

  const auto button_height = 2;
  const auto button_step   = 3;

  auto menu_height = 15;

  if (! g_opt_seed_name_weekly.empty()) {
    menu_height += 3;

    if (game_seed_name_get(g) == g_opt_seed_name_weekly) {
      DBG("clear old seed as using 'weekly seed'");
      game_seed_clear(g);
      game_seed_set(g, nullptr);
    }
  }

  if (! g_opt_seed_name_daily.empty()) {
    menu_height += 3;

    if (game_seed_name_get(g) == g_opt_seed_name_daily) {
      DBG("clear old seed as using 'daily seed'");
      game_seed_clear(g);
      game_seed_set(g, nullptr);
    }
  }

  if (! g_opt_seed_name_previous.empty()) {
    if ((g_opt_seed_name_previous != g_opt_seed_name_weekly) && //
        (g_opt_seed_name_previous != g_opt_seed_name_daily)) {
      menu_height += 3;

      if (game_seed_name_get(g) == g_opt_seed_name_previous) {
        DBG("clear old seed as using 'previous seed'");
        game_seed_clear(g);
        game_seed_set(g, nullptr);
      }
    }
  }

  auto str1       = "Weekly: seed " + g_opt_seed_name_weekly;
  auto str2       = "Daily: seed " + g_opt_seed_name_daily;
  auto str3       = "Previous: seed " + g_opt_seed_name_previous;
  auto menu_width = std::max(static_cast< int >(str1.size()) + 6, static_cast< int >(UI_WID_POPUP_WIDTH_NORMAL));
  menu_width      = std::max(static_cast< int >(str2.size()) + 6, menu_width);
  menu_width      = std::max(static_cast< int >(str3.size()) + 6, menu_width);

  auto y_at = 0;

  spoint const outer_tl((TERM_WIDTH / 2) - (menu_width / 2), (TERM_HEIGHT / 2) - (menu_height / 2));
  spoint       outer_br;

  outer_br.x = outer_tl.x + menu_width + 1;
  outer_br.y = outer_tl.y + menu_height + 1;

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
    auto *w = wid_new_menu_button(g, p, "New Sacrifice");

    spoint const tl(0, y_at);
    spoint const br(button_width, y_at + button_height);
    wid_set_on_mouse_down(w, game_menu_new_game);
    wid_set_pos(w, tl, br);
    if (game_seed_source_get(g) == SEED_SOURCE_RANDOM) {
      std::string const s = std::string(UI_HIGHLIGHT_FMT_STR) + std::string("N") + std::string(UI_RESET_FMT) + std::string("ew sacrifice")
                          + std::string(UI_RESET_FMT);
      wid_set_text(w, s);
    } else {
      std::string const s = std::string(UI_HIGHLIGHT_FMT_STR) + std::string("N") + std::string(UI_RESET_FMT) + std::string("ew sacrifice:")
                          + std::string(UI_RESET_FMT) + std::string(" ") + game_seed_name_get(g);
      wid_set_text(w, s);
    }
  }
  y_at += button_step;

  if (! g_opt_seed_name_daily.empty()) {
    TRACE();
    auto *p = wid_main_menu_window->wid_text_area->wid_text_area;
    auto *w = wid_new_menu_button(g, p, "Daily Seed");

    spoint const tl(0, y_at);
    spoint const br(button_width, y_at + button_height);
    wid_set_on_mouse_down(w, game_menu_daily_seed);
    wid_set_pos(w, tl, br);
    std::string const s = std::string(UI_HIGHLIGHT_FMT_STR) + std::string("D") + std::string(UI_RESET_FMT) + std::string("aily seed:")
                        + std::string(UI_RESET_FMT) + std::string(" ") + g_opt_seed_name_daily;
    wid_set_text(w, s);
    y_at += button_step;
  }

  if (! g_opt_seed_name_weekly.empty()) {
    TRACE();
    auto *p = wid_main_menu_window->wid_text_area->wid_text_area;
    auto *w = wid_new_menu_button(g, p, "Weekly Seed");

    spoint const tl(0, y_at);
    spoint const br(button_width, y_at + button_height);
    wid_set_on_mouse_down(w, game_menu_weekly_seed);
    wid_set_pos(w, tl, br);
    std::string const s = std::string(UI_HIGHLIGHT_FMT_STR) + std::string("W") + std::string(UI_RESET_FMT) + std::string("eekly seed:")
                        + std::string(UI_RESET_FMT) + std::string(" ") + g_opt_seed_name_weekly;
    wid_set_text(w, s);
    y_at += button_step;
  }

  if (! g_opt_seed_name_previous.empty()) {
    if (g_opt_seed_name_previous != g_opt_seed_name_weekly) {
      TRACE();
      auto *p = wid_main_menu_window->wid_text_area->wid_text_area;
      auto *w = wid_new_menu_button(g, p, "Previous Seed");

      spoint const tl(0, y_at);
      spoint const br(button_width, y_at + button_height);
      wid_set_on_mouse_down(w, game_menu_previous_seed);
      wid_set_pos(w, tl, br);
      std::string const s = std::string(UI_HIGHLIGHT_FMT_STR) + std::string("P") + std::string(UI_RESET_FMT) + std::string("revious seed:")
                          + std::string(UI_RESET_FMT) + std::string(" ") + g_opt_seed_name_previous;
      wid_set_text(w, s);
      y_at += button_step;
    }
  }
  {
    TRACE();
    auto *p = wid_main_menu_window->wid_text_area->wid_text_area;
    auto *w = wid_new_menu_button(g, p, "Load Game");

    spoint const tl(0, y_at);
    spoint const br(button_width, y_at + button_height);
    wid_set_on_mouse_down(w, wid_main_menu_load);
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
    wid_set_on_mouse_down(w, wid_main_menu_cfg);
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
    wid_set_on_mouse_down(w, wid_main_menu_more);
    wid_set_pos(w, tl, br);
    wid_set_text(w, UI_HIGHLIGHT_FMT_STR "M" UI_FMT_STR "ore");
  }
  y_at += button_step;
  {
    TRACE();
    auto *p = wid_main_menu_window->wid_text_area->wid_text_area;
    auto *w = wid_new_menu_button(g, p, "Quit Game");

    spoint const tl(0, y_at);
    spoint const br(button_width, y_at + button_height);
    wid_set_on_mouse_down(w, wid_main_menu_quit);
    wid_set_pos(w, tl, br);
    wid_set_text(w, UI_HIGHLIGHT_FMT_STR "Q" UI_FMT_STR "uit Game");
  }

  wid_update(g, wid_main_menu_window->wid_text_area->wid_text_area);

  game_state_change(g, STATE_MAIN_MENU, "main menu");
}
