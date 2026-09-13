//
// Copyright goblinhack@gmail.com
//

#include "my_ascii.hpp"
#include "my_callstack.hpp"
#include "my_game.hpp"
#include "my_main.hpp" // NOLINT
#include "my_sdl_proto.hpp"
#include "my_sound.hpp"
#include "my_spoint.hpp"
#include "my_types.hpp"
#include "my_ui.hpp"
#include "my_wid.hpp"
#include "my_wid_popup.hpp"
#include "my_wids.hpp"

#include <SDL_keyboard.h>
#include <SDL_keycode.h>
#include <cstdint>

static WidPopup *wid_options_menu_window;

void wid_options_menu_destroy(Gamep g)
{
  if (wid_options_menu_window == nullptr) {
    return;
  }

  con("Options menu: destroy");
  TRACE_INDENT();

  delete wid_options_menu_window;
  wid_options_menu_window = nullptr;
}

[[nodiscard]] static auto wid_options_menu_display(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  con("Options menu: display");
  TRACE_INDENT();

  wid_options_menu_destroy(g);
  wid_cfg_display_select(g);
  return true;
}

[[nodiscard]] static auto wid_options_menu_seed(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  con("Options menu: seed");
  TRACE_INDENT();

  wid_options_menu_destroy(g);
  wid_seed_select(g);
  return true;
}

[[nodiscard]] static auto wid_options_menu_player_name(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  con("Options menu: name");
  TRACE_INDENT();

  wid_options_menu_destroy(g);
  wid_player_name_select(g);
  return true;
}

[[nodiscard]] static auto wid_options_menu_mouse(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  con("Options menu: mouse");
  TRACE_INDENT();

  wid_options_menu_destroy(g);
  wid_cfg_mouse_select(g);
  return true;
}

[[nodiscard]] static auto wid_options_menu_keyboard(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  con("Options menu: keyboard");
  TRACE_INDENT();

  wid_options_menu_destroy(g);
  wid_cfg_help_select(g);
  return true;
}

[[nodiscard]] static auto wid_options_menu_sound(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  con("Options menu: sound");
  TRACE_INDENT();

  wid_options_menu_destroy(g);
  wid_cfg_sound_select(g);
  return true;
}

[[nodiscard]] static auto wid_options_menu_back(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  con("Options menu: back");
  TRACE_INDENT();

  wid_options_menu_destroy(g);
  wid_main_menu_select(g);
  return true;
}

[[nodiscard]] static auto wid_options_menu_key_down(Gamep g, Widp w, const struct SDL_Keysym *key) -> bool
{
  con("Options menu: key down");
  TRACE_INDENT();

  if (sdlk_eq(*key, game_key_console_get(g))) {
    (void) sound_play(g, "keypress");
    return false;
  }

  switch (key->mod) {
    case KMOD_LCTRL :
    case KMOD_RCTRL :
    default :
      switch (key->sym) {
        default :
          {
            TRACE();
            auto c = wid_event_to_char(key);
            switch (c) {
              case 'd' :
              case 'D' :
                (void) sound_play(g, "keypress");
                (void) wid_options_menu_display(g, nullptr, 0, 0, 0);
                return true;
              case 'm' :
              case 'M' :
                (void) sound_play(g, "keypress");
                (void) wid_options_menu_mouse(g, nullptr, 0, 0, 0);
                return true;
              case 's' :
              case 'S' :
                (void) sound_play(g, "keypress");
                (void) wid_options_menu_sound(g, nullptr, 0, 0, 0);
                return true;
              case 'k' :
              case 'K' :
                (void) sound_play(g, "keypress");
                (void) wid_options_menu_keyboard(g, nullptr, 0, 0, 0);
                return true;
              case 'c' :
              case 'C' :
                (void) sound_play(g, "keypress");
                (void) wid_options_menu_seed(g, nullptr, 0, 0, 0);
                return true;
              case 'p' :
              case 'P' :
                (void) sound_play(g, "keypress");
                (void) wid_options_menu_player_name(g, nullptr, 0, 0, 0);
                return true;
              case 'b' :
              case 'B' :
              case SDLK_ESCAPE :
                (void) sound_play(g, "keypress");
                (void) wid_options_menu_back(g, nullptr, 0, 0, 0);
                return true;
            }
          }
      }
  }

  return false;
}

void wid_options_menu_select(Gamep g)
{
  con("Options menu: select");
  TRACE_INDENT();

  wid_main_menu_hide(g);

  if (wid_options_menu_window != nullptr) {
    wid_options_menu_destroy(g);
  }

  auto box_height = 2;
  auto box_step   = 3;

  int const    menu_height = 23;
  int const    menu_width  = UI_WID_POPUP_WIDTH_NORMAL;
  spoint const outer_tl((TERM_WIDTH / 2) - (menu_width / 2), (TERM_HEIGHT / 2) - (menu_height / 2));
  spoint const outer_br((TERM_WIDTH / 2) + (menu_width / 2), (TERM_HEIGHT / 2) + (menu_height / 2));

  wid_options_menu_window = new WidPopup(g, "Options menu", outer_tl, outer_br, nullptr, "nothing", false, false);

  auto button_width = outer_br.x - outer_tl.x - 2;

  {
    TRACE();
    Widp w = wid_options_menu_window->wid_popup_container;
    wid_set_on_key_down(w, wid_options_menu_key_down);
  }

  int y_at = 0;
  {
    TRACE();
    auto *p = wid_options_menu_window->wid_text_area->wid_text_area;
    auto *w = wid_new_menu_button(g, p, "Choose seed");

    spoint const tl(0, y_at);
    spoint const br(button_width, y_at + box_height);
    wid_set_on_mouse_down(w, wid_options_menu_seed);
    wid_set_pos(w, tl, br);
    wid_set_text(w, UI_HIGHLIGHT_FMT_STR "C" UI_FMT_STR "hange seed"); // codespell:ignore
  }
  y_at += box_step;
  {
    TRACE();
    auto *p = wid_options_menu_window->wid_text_area->wid_text_area;
    auto *w = wid_new_menu_button(g, p, "Choose player name");

    spoint const tl(0, y_at);
    spoint const br(button_width, y_at + box_height);
    wid_set_on_mouse_down(w, wid_options_menu_player_name);
    wid_set_pos(w, tl, br);
    wid_set_text(w, UI_HIGHLIGHT_FMT_STR "P" UI_FMT_STR "layer name"); // codespell:ignore
  }
  y_at += box_step;
  {
    TRACE();
    auto *p = wid_options_menu_window->wid_text_area->wid_text_area;
    auto *w = wid_new_menu_button(g, p, "display");

    spoint const tl(0, y_at);
    spoint const br(button_width, y_at + box_height);
    wid_set_on_mouse_down(w, wid_options_menu_display);
    wid_set_pos(w, tl, br);
    wid_set_text(w, UI_HIGHLIGHT_FMT_STR "D" UI_RESET_FMT "isplay"); // codespell:ignore
  }
  y_at += box_step;
  {
    TRACE();
    auto *p = wid_options_menu_window->wid_text_area->wid_text_area;
    auto *w = wid_new_menu_button(g, p, "mouse");

    spoint const tl(0, y_at);
    spoint const br(button_width, y_at + box_height);
    wid_set_on_mouse_down(w, wid_options_menu_mouse);
    wid_set_pos(w, tl, br);
    wid_set_text(w, UI_HIGHLIGHT_FMT_STR "M" UI_RESET_FMT "ouse"); // codespell:ignore
  }
  y_at += box_step;
  {
    TRACE();
    auto *p = wid_options_menu_window->wid_text_area->wid_text_area;
    auto *w = wid_new_menu_button(g, p, "sound");

    spoint const tl(0, y_at);
    spoint const br(button_width, y_at + box_height);
    wid_set_on_mouse_down(w, wid_options_menu_sound);
    wid_set_pos(w, tl, br);
    wid_set_text(w, UI_HIGHLIGHT_FMT_STR "S" UI_RESET_FMT "ound"); // codespell:ignore
  }
  y_at += box_step;
  {
    TRACE();
    auto *p = wid_options_menu_window->wid_text_area->wid_text_area;
    auto *w = wid_new_menu_button(g, p, "keyboard");

    spoint const tl(0, y_at);
    spoint const br(button_width, y_at + box_height);
    wid_set_on_mouse_down(w, wid_options_menu_keyboard);
    wid_set_pos(w, tl, br);
    wid_set_text(w, UI_HIGHLIGHT_FMT_STR "K" UI_RESET_FMT "eyboard"); // codespell:ignore
  }
  y_at += box_step;
  {
    TRACE();
    auto *p = wid_options_menu_window->wid_text_area->wid_text_area;
    auto *w = wid_new_back_button(g, p, "BACK");

    spoint const tl(0, y_at);
    spoint const br(button_width, y_at + box_height);
    wid_set_on_mouse_down(w, wid_options_menu_back);
    wid_set_pos(w, tl, br);
  }

  wid_update(g, wid_options_menu_window->wid_text_area->wid_text_area);
}
