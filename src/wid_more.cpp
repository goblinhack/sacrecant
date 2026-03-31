//
// Copyright goblinhack@gmail.com
//

#include "my_ascii.hpp"
#include "my_callstack.hpp"
#include "my_game.hpp"
#include "my_main.hpp"
#include "my_sdl_proto.hpp"
#include "my_sound.hpp"
#include "my_wids.hpp"

static WidPopup *wid_more_window;

static void wid_more_destroy()
{
  TRACE();
  delete wid_more_window;
  wid_more_window = nullptr;
}

[[nodiscard]] static auto wid_more_credits(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();
  wid_more_destroy();
  wid_credits_select(g);
  return true;
}

[[nodiscard]] static auto wid_more_hiscores(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();
  wid_more_destroy();
  wid_hiscores_show(g);
  return true;
}

[[nodiscard]] static auto wid_more_back(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();
  wid_more_destroy();
  wid_main_menu_select(g);
  return true;
}

[[nodiscard]] static auto wid_more_key_down(Gamep g, Widp w, const struct SDL_Keysym *key) -> bool
{
  TRACE();

  if (sdlk_eq(*key, game_key_console_get(g))) {
    sound_play(g, "keypress");
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
              case 'c' :
              case 'C' :
                sound_play(g, "keypress");
                (void) wid_more_credits(g, nullptr, 0, 0, 0);
                return true;
              case 'h' :
              case 'H' :
                sound_play(g, "keypress");
                (void) wid_more_hiscores(g, nullptr, 0, 0, 0);
                return true;
              case 'b' :
              case 'B' :
              case SDLK_ESCAPE :
                sound_play(g, "keypress");
                (void) wid_more_back(g, nullptr, 0, 0, 0);
                return true;
            }
          }
      }
  }

  return false;
}

void wid_more_select(Gamep g)
{
  TRACE();
  log("more menu");

  if (wid_more_window != nullptr) {
    wid_more_destroy();
  }

  auto box_height = 2;
  auto box_step   = 3;

  int const    menu_height = 14;
  int const    menu_width  = UI_WID_POPUP_WIDTH_NORMAL;
  spoint const outer_tl((TERM_WIDTH / 2) - (menu_width / 2), (TERM_HEIGHT / 2) - (menu_height / 2));
  spoint const outer_br((TERM_WIDTH / 2) + (menu_width / 2), (TERM_HEIGHT / 2) + (menu_height / 2));
  wid_more_window = new WidPopup(g, "More menu", outer_tl, outer_br, nullptr, "nothing", false, false);

  auto button_width = outer_br.x - outer_tl.x - 2;

  {
    TRACE();
    Widp w = wid_more_window->wid_popup_container;
    wid_set_on_key_down(w, wid_more_key_down);
  }

  int y_at = 0;

  y_at += box_step;
  {
    TRACE();
    auto *p = wid_more_window->wid_text_area->wid_text_area;
    auto *w = wid_new_menu_button(g, p, "Credits");

    spoint const tl(0, y_at);
    spoint const br(button_width, y_at + box_height);
    wid_set_on_mouse_up(w, wid_more_credits);
    wid_set_pos(w, tl, br);
    wid_set_text(w, UI_HIGHLIGHT_FMT_STR "C" UI_FMT_STR "redits");
  }
  y_at += box_step;
  {
    TRACE();
    auto *p = wid_more_window->wid_text_area->wid_text_area;
    auto *w = wid_new_menu_button(g, p, "Hiscores");

    spoint const tl(0, y_at);
    spoint const br(button_width, y_at + box_height);
    wid_set_on_mouse_up(w, wid_more_hiscores);
    wid_set_pos(w, tl, br);
    wid_set_text(w, UI_HIGHLIGHT_FMT_STR "H" UI_FMT_STR "iscores");
  }
  y_at += box_step;
  {
    TRACE();
    auto *p = wid_more_window->wid_text_area->wid_text_area;
    auto *w = wid_new_back_button(g, p, "BACK");

    spoint const tl(0, y_at);
    spoint const br(button_width, y_at + box_height);
    wid_set_on_mouse_up(w, wid_more_back);
    wid_set_pos(w, tl, br);
  }

  wid_update(g, wid_more_window->wid_text_area->wid_text_area);

  game_state_change(g, STATE_MAIN_MENU, "main menu");
}
