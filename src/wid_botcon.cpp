//
// Copyright goblinhack@gmail.com
//

#include "my_ascii.hpp"
#include "my_callstack.hpp"
#include "my_color_defs.hpp"
#include "my_globals.hpp"
#include "my_main.hpp"
#include "my_spoint.hpp"
#include "my_string.hpp"
#include "my_types.hpp"
#include "my_ui.hpp"
#include "my_wid.hpp"
#include "my_wids.hpp"

#include <cstdint>
#include <map>
#include <string>
#include <utility>

static void wid_botcon_wid_create(Gamep g);

static Widp wid_botcon_container {};
static Widp wid_botcon_vert_scroll {};
static Widp wid_botcon_input_line {};
static Widp wid_botcon_window {};

static std::string last_msg;

static std::map< uint32_t, std::string > wid_botcon_lines;

void wid_botcon_fini(Gamep g)
{
  TRACE();
  wid_destroy(g, &wid_botcon_container);
  wid_destroy(g, &wid_botcon_vert_scroll);
  wid_destroy(g, &wid_botcon_input_line);
  wid_destroy(g, &wid_botcon_window);
}

[[nodiscard]] auto wid_botcon_init(Gamep g) -> bool
{
  TRACE();
  wid_botcon_wid_create(g);

  last_msg = "";

  return true;
}

//
// Scroll back to the bottom of the screen.
//
static void wid_botcon_reset_scroll(Gamep g)
{
  TRACE();

  if (wid_botcon_vert_scroll == nullptr) {
    return;
  }

  wid_move_to_bottom(g, wid_botcon_vert_scroll);
}

static void wid_botcon_scroll(Widp w, const std::string &str)
{
  TRACE();
  Widp tmp {};

  wid_scroll_text(w);

  //
  // Get the wid on the bottom of the list/screen.
  //
  tmp = wid_get_head(w);
  if (tmp != nullptr) {
    wid_set_text(tmp, str);
  }
}

static void wid_botcon_replace(Widp w, const std::string &str)
{
  TRACE();
  Widp tmp {};

  //
  // Get the wid on the bottom of the list/screen.
  //
  tmp = wid_get_head(w);
  if (tmp != nullptr) {
    wid_set_text(tmp, str);
  }
}

//
// Log a message to the botcon
//
static void wid_botcon_log_(const std::string &s)
{
  TRACE();
  static int log_wid_botcon_buffered_lines;

  if (s.empty()) {
    return;
  }

  extern Gamep game;
  auto        *g = game;

  wid_botcon_reset_scroll(g);

  //
  // Before the botcon is ready, we buffer the logs.
  //
  if (wid_botcon_input_line == nullptr) {
    auto result = wid_botcon_lines.insert(std::make_pair(log_wid_botcon_buffered_lines++, s));

    if (! result.second) {
      CROAK("widget botcon lines insert name [%s] failed", s.c_str());
    }

    return;
  }

  //
  // Flush the logs now the botcon exists.
  //
  wid_botcon_flush(g);

  auto curr_msg = wid_get_text(wid_get_head(wid_botcon_input_line));

  if (! curr_msg.empty() && length_without_format(curr_msg) + length_without_format(s) + 1 < UI_BOTCON_WIDTH) {
    curr_msg = curr_msg + " " + s;
    wid_botcon_replace(wid_botcon_input_line, curr_msg);
    last_msg = "";
  } else if (curr_msg.empty()) {
    last_msg = s;
    wid_botcon_replace(wid_botcon_input_line, s);
  } else {
    last_msg = s;
    wid_botcon_scroll(wid_botcon_input_line, s);
  }
}

void wid_botcon_flush(Gamep g)
{
  TRACE();
  auto iter = wid_botcon_lines.begin();

  while (iter != wid_botcon_lines.end()) {
    wid_botcon_scroll(wid_botcon_input_line, iter->second);
    iter = wid_botcon_lines.erase(iter);
  }

  wid_botcon_reset_scroll(g);
}

//
// Log a message to the botcon
//
void wid_botcon_log(const std::string &s)
{
  TRACE();

  if (g_opt_tests) {
    return;
  }

  int const chars_per_line = UI_BOTCON_WIDTH;

  if (TERM_WIDTH == 0) {
    CROAK("no TERM_WIDTH set");
  }

  if (s.empty()) {
    wid_botcon_scroll(wid_botcon_input_line, "");
    return;
  }

  auto d = split(s, chars_per_line);

  for (const auto &c : d) {
    wid_botcon_log_(c);
  }
}

//
// Create the botcon
//
static void wid_botcon_wid_create(Gamep g)
{
  TRACE();
  int const h = UI_BOTCON_HEIGHT;

  if (wid_botcon_window != nullptr) {
    wid_botcon_fini(g);
  }

  TRACE();
  {
    auto tl = spoint(UI_LEFTBAR_WIDTH, TERM_HEIGHT - h);
    auto br = spoint(UI_LEFTBAR_WIDTH + UI_BOTCON_WIDTH - 1, TERM_HEIGHT - 1);

    wid_botcon_window = wid_new_window(g, "widget botcon");
    wid_set_name(wid_botcon_window, "widget botcon window");
    wid_set_pos(wid_botcon_window, tl, br);
    wid_set_shape_none(wid_botcon_window);
  }

  TRACE();
  {
    auto tl = spoint(0, 0);
    auto br = spoint(UI_BOTCON_WIDTH - 1, h - 1);

    wid_botcon_container = wid_new_container(g, wid_botcon_window, "widget botcon container");
    wid_set_pos(wid_botcon_container, tl, br);
    wid_set_shape_none(wid_botcon_container);
    wid_set_style(wid_botcon_container, UI_WID_STYLE_CONSOLE);
  }

  TRACE();
  {
    int row        = 0;
    int row_bottom = h;

    Widp child {};
    Widp prev {};

    for (row = 0; row < UI_BOTCON_HEIGHT; row++) {
      row_bottom--;
      auto tl = spoint(0, row_bottom);
      auto br = spoint(UI_BOTCON_WIDTH - 1, row_bottom);

      TRACE();
      child = wid_new_container(g, wid_botcon_container, "");

      wid_set_shape_none(child);
      wid_set_pos(child, tl, br);
      wid_set_text_lhs(child);

      wid_set_prev(child, prev);
      prev = child;

      if (row == 0) {
        wid_botcon_input_line = child;
      }

      wid_set_color(child, WID_COLOR_TEXT_FG, UI_BOTCON_TEXT_COLOR);
      wid_set_color(child, WID_COLOR_BG, COLOR_NONE);
      wid_set_name(child, "botcon output");
    }
  }

  TRACE();
  wid_botcon_vert_scroll = wid_new_vert_scroll_bar(g, wid_botcon_window, "", wid_botcon_container);
  wid_visible(g, wid_get_parent(wid_botcon_vert_scroll));
  wid_update(g, wid_botcon_window);
}
