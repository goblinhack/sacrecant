//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_color_defs.hpp"
#include "my_globals.hpp"
#include "my_main.hpp"
#include "my_string.hpp"
#include "my_ui.hpp"
#include "my_wid_text_box.hpp"

WidTextBox::~WidTextBox()
{
  TRACE();

  extern Gamep game;
  auto        *g = game;

  if (wid_horiz_scroll != nullptr) {
    wid_destroy(g, &wid_horiz_scroll);
  }
  if (wid_vert_scroll != nullptr) {
    wid_destroy(g, &wid_vert_scroll);
  }
  wid_destroy(g, &wid_text_last);
  wid_destroy(g, &wid_text_area);
}

WidTextBox::WidTextBox(Gamep g, spoint vtl, spoint vbr, Widp vparent, bool horiz_scroll, bool vert_scroll, int vscroll_height_in)
    : scroll_height(vscroll_height_in), tl(vtl), br(vbr), wid_parent(vparent)
{
  TRACE();

  int w = br.x - tl.x;
  int h = br.y - tl.y;
  width = w;

  if (scroll_height == -1) {
    if (vert_scroll) {
      scroll_height = h * 2;
    } else {
      scroll_height = h;
    }
  }

  line_count = 0;

  {
    if (vparent != nullptr) {
      wid_text_box_container = wid_new_square_button(g, vparent, "widget text box");
      wid_set_shape_none(wid_text_box_container);
    } else {
      wid_text_box_container = wid_new_window(g, "widget text box");
      wid_set_style(wid_text_box_container, UI_WID_STYLE_GREEN);
    }
    wid_set_pos(wid_text_box_container, tl, br);
  }

  {
    spoint const inner_tl(1, 1);
    spoint const inner_br(w - 1, h - 1);

    wid_text_area = wid_new_square_button(g, wid_text_box_container, "widget text inner area");
    wid_set_pos(wid_text_area, inner_tl, inner_br);
    wid_set_shape_none(wid_text_area);
    // wid_set_style( wid_text_area, UI_WID_STYLE_RED);

    w = inner_br.x - inner_tl.x;
    h = inner_br.y - inner_tl.y;
  }

  {
    int row = 0;

    Widp child {};
    Widp prev {};

    int lines_of_text = 0;
    if (vert_scroll) {
      lines_of_text = scroll_height;
    } else {
      lines_of_text = wid_get_height(wid_text_area);
    }
    int row_bottom = lines_of_text;
    height         = lines_of_text;

    for (row = 0; row < lines_of_text; row++) {
      row_bottom--;
      spoint const text_tl(0, row_bottom);
      spoint const text_br(w, row_bottom);

      child = wid_new_container(g, wid_text_area, "");
      children.push_back(child);

      wid_set_shape_none(child);
      wid_set_pos(child, text_tl, text_br);
      wid_set_text_centerx(child);

      wid_set_prev(child, prev);
      prev = child;

      if (row == 0) {
        wid_text_last = child;
      }

      wid_set_color(child, WID_COLOR_TEXT_FG, UI_POPUP_TEXT_COLOR);
      wid_set_name(child, "text_box output");
    }

    wid_raise(g, wid_text_last);
  }

  if (vert_scroll) {
    wid_vert_scroll = wid_new_vert_scroll_bar(g, wid_text_box_container, "text box vert scroll", wid_text_area);
  }

  if (horiz_scroll) {
    wid_horiz_scroll = wid_new_horiz_scroll_bar(g, wid_text_box_container, "text box horiz scroll", wid_text_area);
  }

  wid_update(g, wid_text_box_container);

  if (horiz_scroll) {
    wid_hide(g, wid_get_parent(wid_horiz_scroll));
    wid_hide(g, wid_horiz_scroll);
  }

  if (vert_scroll) {
    wid_visible(g, wid_get_parent(wid_vert_scroll));
    wid_visible(g, wid_vert_scroll);
  }
}

//
// Get the wid on the bottom of the list/screen.
//
auto WidTextBox::log_internal(Gamep g, const std::string &str, wid_text_format format, const std::string &col) -> Widp
{
  TRACE();

  Widp tmp {};
  Widp text_wid {};

  if (wid_vert_scroll == nullptr) {
    if (line_count < height) {
      text_wid = children[ height - line_count - 1 ];
      wid_set_text(text_wid, str);
      line_count++;
      wid_update(g, wid_text_box_container);
    } else {
      ::log("text box overflow on [%s] height %d line_count %d", str.c_str(), height, line_count);
      return nullptr;
    }
  } else {
    if (line_count < scroll_height) {
      text_wid = children[ scroll_height - line_count - 1 ];
      wid_set_text(text_wid, str);
    } else {
      wid_scroll_text(wid_text_last);
      tmp = wid_get_head(wid_text_last);
      if (tmp != nullptr) {
        wid_set_text(tmp, str);
      }
      text_wid = tmp;
    }

    if (wid_vert_scroll != nullptr) {
      wid_move_to_top(g, wid_vert_scroll);
    }
    line_count++;

    int const show_scrollbars_at = wid_get_height(wid_text_area);
    if (line_count > show_scrollbars_at) {
      if (wid_horiz_scroll != nullptr) {
        wid_visible(g, wid_get_parent(wid_horiz_scroll));
      }
      if (wid_vert_scroll != nullptr) {
        wid_visible(g, wid_get_parent(wid_vert_scroll));
      }
    }
  }

  if (text_wid != nullptr) {
    if (format == TEXT_FORMAT_LHS) {
      wid_set_text_lhs(text_wid);
    } else if (format == TEXT_FORMAT_RHS) {
      wid_set_text_rhs(text_wid);
    }
  }

  if (! col.empty()) {
    wid_set_color(text_wid, WID_COLOR_TEXT_FG, color_find(col.c_str()));
  }
  return text_wid;
}

//
// Log a message to the text box
//
auto WidTextBox::log(Gamep g, const std::string &s, wid_text_format format, const std::string &col) -> Widp
{
  TRACE();

  int const chars_per_line = wid_get_width(wid_text_area);
  Widp      w              = nullptr;

  auto d = split(s, chars_per_line);

  for (const auto &c : d) {
    //
    // Handles %%fg=...$ with no text due to a split
    //
    if (length_without_format(c) != 0) {
      w = log_internal(g, c, format, col);
    }
  }
  return w;
}

//
// Log a blank line to the text box
//
auto WidTextBox::log_empty_line(Gamep g) -> Widp
{
  TRACE();

  return log_internal(g, " ");
}
