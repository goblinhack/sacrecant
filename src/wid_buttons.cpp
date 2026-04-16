//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_game.hpp"
#include "my_main.hpp"
#include "my_ui.hpp"
#include "my_wids.hpp"

void wid_gray_out_button(Gamep g, Widp w)
{
  TRACE();

  wid_set_mode(w, WID_MODE_OVER);
  wid_set_style(w, UI_WID_STYLE_SOLID_WHITE);
  wid_set_color(w, WID_COLOR_BG, GRAY50);
  wid_set_color(w, WID_COLOR_TEXT_FG, WHITE);
  wid_set_mode(w, WID_MODE_NORMAL);
  wid_set_style(w, UI_WID_STYLE_SOLID_WHITE);
  wid_set_color(w, WID_COLOR_BG, GRAY40);
}

auto wid_new_back_button(Gamep g, Widp parent, const std::string &name) -> Widp
{
  TRACE();

  auto *w = wid_new_square_button(g, parent, name);
  wid_set_text(w, "BACK");
  wid_set_mode(w, WID_MODE_OVER);
  wid_set_style(w, UI_WID_STYLE_SOLID_WHITE);
  wid_set_color(w, WID_COLOR_BG, RED);
  wid_set_color(w, WID_COLOR_TEXT_FG, WHITE);
  wid_set_mode(w, WID_MODE_NORMAL);
  return w;
}

static void wid_button_pulse(Gamep g, Widp w)
{
  TRACE();

  auto pulse = THING_IS_HOT_PULSE_MS; // ms
  auto mid   = pulse / 2;
  auto n     = time_ms_cached() % pulse;

  if (n > mid) {
    n = mid - n;
  }
  if (! n) {
    n = -1;
  }

  float i = static_cast< int >((255 / static_cast< float >(mid)) * static_cast< float >(n));
  auto  a = ((static_cast< uint8_t >((int) i)) / 2) + 120;

  color c = wid_get_color(w, WID_COLOR_BG);
  c.a     = a;
  wid_set_color(w, WID_COLOR_BG, c);
}

auto wid_new_continue_button(Gamep g, Widp parent, const std::string &name) -> Widp
{
  TRACE();

  auto *w = wid_new_square_button(g, parent, name);
  wid_set_text(w, "CONTINUE");
  wid_set_mode(w, WID_MODE_OVER);
  wid_set_style(w, UI_WID_STYLE_SOLID_WHITE);
  wid_set_color(w, WID_COLOR_BG, GREEN);
  wid_set_color(w, WID_COLOR_TEXT_FG, WHITE);
  wid_set_mode(w, WID_MODE_NORMAL);
  wid_set_style(w, UI_WID_STYLE_SOLID_WHITE);
  wid_set_color(w, WID_COLOR_BG, GREEN2);
  wid_set_on_tick(w, wid_button_pulse);
  return w;
}

auto wid_new_menu_button(Gamep g, Widp parent, const std::string &name) -> Widp
{
  TRACE();

  auto *w = wid_new_square_button(g, parent, name);
  wid_set_mode(w, WID_MODE_OVER);
  wid_set_style(w, UI_WID_STYLE_SOLID_WHITE);
  wid_set_color(w, WID_COLOR_BG, GREEN);
  wid_set_color(w, WID_COLOR_TEXT_FG, WHITE);
  wid_set_mode(w, WID_MODE_NORMAL);
  return w;
}

auto wid_new_button(Gamep g, Widp parent, const std::string &name) -> Widp
{
  TRACE();

  auto *w = wid_new_square_button(g, parent, name);
  wid_set_mode(w, WID_MODE_OVER);
  wid_set_style(w, UI_WID_STYLE_SOLID_WHITE);
  wid_set_color(w, WID_COLOR_BG, GREEN);
  wid_set_color(w, WID_COLOR_TEXT_FG, WHITE);
  wid_set_mode(w, WID_MODE_NORMAL);
  wid_set_style(w, UI_WID_STYLE_SOLID_WHITE);
  wid_set_color(w, WID_COLOR_BG, GRAY20);
  return w;
}

auto wid_new_green_button(Gamep g, Widp parent, const std::string &name) -> Widp
{
  TRACE();

  auto *w = wid_new_square_button(g, parent, name);
  wid_set_mode(w, WID_MODE_OVER);
  wid_set_style(w, UI_WID_STYLE_SOLID_WHITE);
  wid_set_color(w, WID_COLOR_BG, GREEN);
  wid_set_color(w, WID_COLOR_TEXT_FG, WHITE);
  wid_set_mode(w, WID_MODE_NORMAL);
  wid_set_style(w, UI_WID_STYLE_GREEN);
  return w;
}

auto wid_new_red_button(Gamep g, Widp parent, const std::string &name) -> Widp
{
  TRACE();

  auto *w = wid_new_square_button(g, parent, name);
  wid_set_mode(w, WID_MODE_OVER);
  wid_set_style(w, UI_WID_STYLE_SOLID_WHITE);
  wid_set_color(w, WID_COLOR_BG, RED);
  wid_set_color(w, WID_COLOR_TEXT_FG, WHITE);
  wid_set_mode(w, WID_MODE_NORMAL);
  wid_set_style(w, UI_WID_STYLE_RED);
  return w;
}

auto wid_new_save_button(Gamep g, Widp parent, const std::string &name) -> Widp
{
  TRACE();

  return wid_new_green_button(g, parent, name);
}

auto wid_new_cancel_button(Gamep g, Widp parent, const std::string &name) -> Widp
{
  TRACE();

  return wid_new_red_button(g, parent, name);
}
