//
// Copyright goblinhack@gmail.com
//

#include "my_ascii.hpp"
#include "my_callstack.hpp"
#include "my_game.hpp"
#include "my_hiscore.hpp"
#include "my_main.hpp"
#include "my_main.hpp" // NOLINT
#include "my_sdl_proto.hpp"
#include "my_sound.hpp"
#include "my_spoint.hpp"
#include "my_string.hpp"
#include "my_types.hpp"
#include "my_ui.hpp"
#include "my_wid.hpp"
#include "my_wid_popup.hpp"
#include "my_wids.hpp"

#include <SDL_keyboard.h>
#include <SDL_keycode.h>
#include <cstdint>
#include <cstdio>
#include <utility>

static WidPopup *wid_hiscore_window;

static void wid_hiscore_destroy(Gamep g)
{
  if (wid_hiscore_window == nullptr) {
    return;
  }

  con("Hiscore menu: destroy");
  TRACE_INDENT();

  delete wid_hiscore_window;
  wid_hiscore_window = nullptr;
  wid_main_menu_select(g);
}

[[nodiscard]] static auto wid_hiscore_key_down(Gamep g, Widp w, const struct SDL_Keysym *key) -> bool
{
  con("Hiscore menu: key down");
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
              case '\n' :
              case 'b' :
              case 'B' :
              case SDLK_ESCAPE :
                {
                  TRACE();
                  (void) sound_play(g, "select");
                  wid_hiscore_destroy(g);
                  return true;
                }
            }
          }
      }
  }

  //
  // So screenshots can work
  //
  return false;
}

[[nodiscard]] static auto wid_hiscore_mouse_down(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();
  wid_hiscore_destroy(g);
  return true;
}

void wid_hiscores_show(Gamep g)
{
  con("Hiscore menu: show");
  TRACE_INDENT();

  wid_hiscore_destroy(g);

  int const menu_height = 26;
  int const menu_width  = 120;

  const auto *name           = "Name";
  int const   name_field_len = UI_MAX_PLAYER_NAME_LEN + 1;

  const auto *when           = "Date of Demise";
  int const   when_field_len = 25;

  const auto *completed           = "Completed";
  int const   completed_field_len = 9;

  const auto *reason           = "Reason of Unfair Demise";
  int const   reason_field_len = 52;

  spoint const outer_tl((TERM_WIDTH / 2) - (menu_width / 2), (TERM_HEIGHT / 2) - (menu_height / 2));
  spoint const outer_br((TERM_WIDTH / 2) + (menu_width / 2), (TERM_HEIGHT / 2) + (menu_height / 2));
  wid_hiscore_window = new WidPopup(g, "hiscores", outer_tl, outer_br, nullptr, "", false, false);

  {
    TRACE();
    Widp w = wid_hiscore_window->wid_popup_container;
    wid_set_on_key_down(w, wid_hiscore_key_down);
  }

  auto *hiscores = game_hiscores_get(g);
  auto  h        = hiscores->hiscores.begin();
  bool  first    = true;
  auto  index    = 0;

  // NOLINTBEGIN
  const char *colors[ HiScore::max_displayed ] = {
      "green", "yellow", "yellow", "yellow", "gray50", "gray50", "gray60", "gray60", "gray70", "gray70",
  };
  // NOLINTEND

  while (h != hiscores->hiscores.end()) {

    if (index >= ARRAY_SIZE(colors)) {
      break;
    }

    char tmp[ 200 ];

    if (first) {
      first = false;

      const auto *col = "red";

      snprintf(tmp, SIZEOF(tmp) - 1, "%%%%fg=%s$%7s %-*s %-*s %-*s %*s", //
               col, "Score",                                             //
               name_field_len, capitalize(name).c_str(),                 //
               when_field_len, when,                                     //
               completed_field_len, completed,                           //
               reason_field_len, reason);

      wid_hiscore_window->log(g, tmp);
    }

    std::string when_val = h->when;
    if (std::cmp_greater(when_val.length(), when_field_len)) {
      when_val[ when_field_len ] = '\0';
    }

    if (when_val.empty()) {
      when_val = "-";
    }

    std::string reason_val = capitalize_first(h->reason);
    if (std::cmp_greater(reason_val.length(), reason_field_len)) {
      reason_val[ reason_field_len ] = '\0';
    }

    if (reason_val.empty()) {
      reason_val = "-";
    }

    const auto *col = colors[ index++ ];
    snprintf(tmp, SIZEOF(tmp) - 1, "%%%%fg=%s$%07u %-*s %-*s %-*u %*s", //
             col, h->score,                                             //
             name_field_len, h->name.c_str(),                           //
             when_field_len, when_val.c_str(),                          //
             completed_field_len, h->levels_completed,                  //
             reason_field_len, reason_val.c_str());

    wid_hiscore_window->log_empty_line(g);
    wid_hiscore_window->log(g, tmp);
    h++;
  }

  {
    TRACE();
    auto *p = wid_hiscore_window->wid_text_area->wid_text_area;
    auto *w = wid_new_back_button(g, p, "hiscore");

    spoint const tl((menu_width / 2) - 4, menu_height - 4);
    spoint const br((menu_width / 2) + 3, menu_height - 2);

    wid_set_on_mouse_down(w, wid_hiscore_mouse_down);
    wid_set_pos(w, tl, br);
  }

  wid_update(g, wid_hiscore_window->wid_text_area->wid_text_area);
}
