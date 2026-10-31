//
// Copyright goblinhack@gmail.com
//

#include "my_ascii.hpp"
#include "my_callstack.hpp"
#include "my_spoint.hpp"
#include "my_types.hpp"
#include "my_wid.hpp"
#include "my_wid_popup.hpp"
#include "my_wids.hpp"
#include <SDL_keyboard.h>
#include <cstdint>
#include <string>

static WidPopup *wid_notice_window;

void wid_notice_destroy()
{
  TRACE();
  delete wid_notice_window;
  wid_notice_window = nullptr;
}

[[nodiscard]] static auto wid_notice_ok(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();
  wid_notice_destroy();
  return true;
}

[[nodiscard]] static auto wid_notice_key_down(Gamep g, Widp w, const struct SDL_Keysym * /*key*/) -> bool
{
  TRACE();
  (void) wid_notice_ok(g, nullptr, 0, 0, 0);
  return true;
}

void wid_notice(const std::string &s)
{
  TRACE();

  extern Gamep game;
  auto        *g = game;

  if (wid_notice_window != nullptr) {
    wid_notice_destroy();
  }

  auto   m = TERM_WIDTH / 2;
  auto   n = TERM_HEIGHT / 2;
  spoint tl(0, 0);
  spoint br(s.size() + 3, 8);
  spoint offset(m, n);
  offset.x -= s.size() / 2;
  tl += offset;
  br += offset;

  auto width = br.x - tl.x;

  wid_notice_window = new WidPopup(g, "game notice", tl, br, nullptr, "", false, false);
  {
    TRACE();
    Widp w = wid_notice_window->wid_popup_container;
    wid_set_on_key_down(w, wid_notice_key_down);
  }

  wid_notice_window->log(g, s);

  int const y_at = 3;
  {
    TRACE();
    auto *p = wid_notice_window->wid_text_area->wid_text_area;
    auto *w = wid_new_back_button(g, p, "ok");

    spoint const tl2((width / 2) - 4, y_at);
    spoint const br2((width / 2) + 4, y_at + 2);
    wid_set_on_mouse_down(w, wid_notice_ok);
    wid_set_pos(w, tl2, br2);
    wid_set_text(w, "Ok!");
  }

  wid_update(g, wid_notice_window->wid_text_area->wid_text_area);
}
