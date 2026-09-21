//
// Copyright goblinhack@gmail.com
//

#include "my_ascii.hpp"
#include "my_callstack.hpp"
#include "my_game.hpp"
#include "my_main.hpp"
#include "my_sdl_proto.hpp"
#include "my_sound.hpp"
#include "my_spoint.hpp"
#include "my_types.hpp"
#include "my_ui.hpp"
#include "my_wid.hpp"
#include "my_wid_popup.hpp"
#include "my_wid_warning.hpp"
#include "my_wids.hpp"
#include <SDL_keyboard.h>
#include <SDL_keycode.h>
#include <cstdint>

WidPopup *wid_warning_window;

static void wid_warning_destroy()
{
  TRACE();
  delete wid_warning_window;
  wid_warning_window = nullptr;
}

[[nodiscard]] static auto wid_warning_key_down(Gamep g, Widp w, const struct SDL_Keysym *key) -> bool
{
  DBG("Key press for wid warning");
  TRACE();

  if (sdlk_eq(*key, game_key_console_get(g))) {
    (void) sound_play(g, "keypress");
    return false;
  }

  auto callback = reinterpret_cast< wid_warning_callback_t >(wid_get_void_context(w));

  switch (key->mod) {
    case KMOD_LCTRL :
    case KMOD_RCTRL :
    default :
      switch (key->sym) {
        default :
          {
            auto c = wid_event_to_char(key);
            switch (c) {
              case 'y' :
              case 'Y' :
                (void) sound_play(g, "keypress");
                DBG("widget warning: yes");
                if (callback != nullptr) {
                  (void) sound_play(g, "keypress");
                  (callback)(g, true);
                }
                wid_warning_destroy();
                break;
              case 'n' :
              case 'N' :
                (void) sound_play(g, "keypress");
                DBG("widget warning: no");
                if (callback != nullptr) {
                  (void) sound_play(g, "keypress");
                  (callback)(g, false);
                }
                wid_warning_destroy();
                break;
              case '\n' :
              case 'b' :
              case 'B' :
              case SDLK_ESCAPE :
                {
                  (void) sound_play(g, "keypress");
                  if (callback != nullptr) {
                    DBG("widget warning: default, no");
                    (void) sound_play(g, "keypress");
                    (callback)(g, false);
                  }
                  wid_warning_destroy();
                  return true;
                }
            }
          }
      }
  }

  return true;
}

[[nodiscard]] static auto wid_warning_yes(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  DBG("widget warning: yes");
  TRACE();

  auto callback = reinterpret_cast< wid_warning_callback_t >(wid_get_void_context(w));
  if (callback != nullptr) {
    (callback)(g, true);
  }
  wid_warning_destroy();
  return true;
}

[[nodiscard]] static auto wid_warning_no(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  DBG("widget warning: no");
  TRACE();

  auto callback = reinterpret_cast< wid_warning_callback_t >(wid_get_void_context(w));
  if (callback != nullptr) {
    (callback)(g, false);
  }
  wid_warning_destroy();
  return true;
}

void wid_warning(Gamep g, const std::string &warning, wid_warning_callback_t callback)
{
  TRACE();

  if (wid_warning_window != nullptr) {
    wid_warning_destroy();
  }

  auto         m = TERM_WIDTH / 2;
  auto         n = TERM_HEIGHT / 4;
  spoint const tl(m - (UI_WID_POPUP_WIDTH_WIDE / 2), n - 5);
  spoint const br(m + (UI_WID_POPUP_WIDTH_WIDE / 2), n + 5);
  auto         width = br.x - tl.x;

  wid_warning_window = new WidPopup(g, "game warning", tl, br, nullptr, "", false, false);
  wid_set_on_key_down(wid_warning_window->wid_popup_container, wid_warning_key_down);
  wid_set_do_not_lower(wid_warning_window->wid_popup_container);
  wid_set_void_context(wid_warning_window->wid_popup_container, reinterpret_cast< void * >(callback));

  wid_warning_window->log_empty_line(g);
  wid_warning_window->log(g, warning);
  wid_warning_window->log_empty_line(g);

  auto y_at = 4;
  {
    TRACE();
    auto *p = wid_warning_window->wid_text_area->wid_text_area;
    auto *w = wid_new_red_button(g, p, "No");

    spoint const tl1((width / 2) - 12, y_at + 2);
    spoint const br1((width / 2) - 2, y_at + 4);
    wid_set_on_mouse_down(w, wid_warning_no);
    wid_set_void_context(w, reinterpret_cast< void * >(callback));
    wid_set_pos(w, tl1, br1);
    wid_set_text(w, "No");
  }

  {
    TRACE();
    auto *p = wid_warning_window->wid_text_area->wid_text_area;
    auto *w = wid_new_green_button(g, p, "Yes");

    spoint const tl2((width / 2) + 0, y_at + 2);
    spoint const br2((width / 2) + 10, y_at + 4);
    wid_set_on_mouse_down(w, wid_warning_yes);
    wid_set_void_context(w, reinterpret_cast< void * >(callback));
    wid_set_pos(w, tl2, br2);
    wid_set_text(w, "Yes");
  }
  wid_update(g, wid_warning_window->wid_text_area->wid_text_area);
}
