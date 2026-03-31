//
// Copyright goblinhack@gmail.com
//

#include <ranges>

#include "my_game.hpp"
#include "my_sdl_event.hpp"
#include "my_sdl_proto.hpp"
#include "my_sound.hpp"
#include "my_wid_class.hpp"
#include "my_wids.hpp"

bool wid_mouse_visible = true;
bool wid_mouse_two_clicks;
ts_t wid_last_mouse_motion;

//
// Catch recursive cases:
//
static int wid_mouse_motion_recursion;

static int saved_mouse_x;
static int saved_mouse_y;

void wid_mouse_motion_end(Gamep g) {}

void wid_mouse_motion_begin(Gamep g, Widp w, int x, int y) { wid_mouse_motion_end(g); }

void wid_mouse_focus_end(Gamep g_maybe_null)
{
  TRACE();

  Widp w {};

  w                = wid_focus;
  wid_focus        = nullptr;
  wid_focus_locked = nullptr;

  if (w == nullptr) [[unlikely]] {
    return;
  }

  if (w->on_mouse_focus_end != nullptr) {
    w->on_mouse_focus_end(g_maybe_null, w);
  }
}

void wid_mouse_focus_begin(Gamep g, Widp w)
{
  TRACE();

  Widp top {};

  if (w == nullptr) [[unlikely]] {
    wid_mouse_focus_end(g);
    wid_focus = nullptr;

    wid_set_top_focus(g);
    return;
  }

  if (wid_focus == w) {
    return;
  }

  wid_mouse_focus_end(g);

  if (wid_ignore_for_focus(w)) {
    return;
  }

  top = wid_get_top_parent(w);

  wid_focus       = w;
  top->focus_last = w->focus_order;

  if (w->on_mouse_focus_begin != nullptr) {
    w->on_mouse_focus_begin(g, w);
  }
}

void wid_mouse_over_end(Gamep g)
{
  TRACE();

  Widp w {};

  if (static_cast< int >(wid_mouse_visible) == 0) {
    return;
  }

  w = wid_over;
  if (wid_over != nullptr) {
    if (! wid_ignore_events(wid_over)) {
      wid_last_over_event = time_ms_cached();
    }
  }
  wid_over = nullptr;

  if (w == nullptr) [[unlikely]] {
    return;
  }

  wid_set_mode(w, WID_MODE_NORMAL);

  if (wid_exiting) {
    return;
  }

  if (w->on_mouse_over_end != nullptr) {
    w->on_mouse_over_end(g, w);
  }
}

[[nodiscard]] static auto wid_mouse_over_begin(Gamep g, Widp w, uint32_t x, uint32_t y, int relx, int rely, int wheelx, int wheely) -> bool
{
  TRACE();

  if (static_cast< int >(wid_mouse_visible) == 0) {
    return false;
  }

  if (wid_over == w) {
    return true;
  }

  if (wid_ignore_events(w)) {
    return false;
  }

  if ((wheelx != 0) || (wheely != 0)) {
    if (wid_ignore_scroll_events(w)) {
      return false;
    }
  }

  if ((w->on_mouse_over_begin == nullptr) && (w->on_mouse_down == nullptr)) {
    if ((static_cast< bool >(w->cfg[ WID_MODE_OVER ].color_set[ WID_COLOR_BG ]))
        || (static_cast< bool >(w->cfg[ WID_MODE_OVER ].color_set[ WID_COLOR_TEXT_FG ]))) {
      //
      // Changes appearance on mouse over, so choose this wid even
      // if it has no over callback.
      //
    } else {
      //
      // Can ignore. It doesn't really do anything when the mouse
      // is over.
      //
      if ((wid_over == nullptr) && (wid_on_screen_at[ x ][ y ] != nullptr)) {
        //
        // But if we have nothing else, use this
        //
      } else {
        return false;
      }
    }
  }

  if (wid_ignore_being_destroyed(w)) {
    return false;
  }

  wid_mouse_over_end(g);

  wid_over = w;
  if (! wid_ignore_events(wid_over)) {
    wid_last_over_event = time_ms_cached();
    // DBG("mouse over %s mouse %d,%d.", wid_over->name.c_str(), ascii_mouse_x, ascii_mouse_y);
  }

  wid_set_mode(w, WID_MODE_OVER);

  if (w->on_mouse_over_begin != nullptr) {
    (w->on_mouse_over_begin)(g, w, relx, rely, wheelx, wheely);
  }

  return true;
}
void wid_set_on_mouse_down(Widp w, on_mouse_down_t fn)
{
  TRACE();
  w->on_mouse_down = fn;
}

void wid_set_on_mouse_held(Widp w, on_mouse_down_t fn)
{
  TRACE();
  w->on_mouse_held = fn;
}

void wid_set_on_mouse_up(Widp w, on_mouse_up_t fn)
{
  TRACE();
  w->on_mouse_up = fn;
}

void wid_set_on_mouse_motion(Widp w, on_mouse_motion_t fn)
{
  TRACE();
  w->on_mouse_motion = fn;
}

void wid_set_on_mouse_focus_begin(Widp w, on_mouse_focus_begin_t fn)
{
  TRACE();
  w->on_mouse_focus_begin = fn;
}

void wid_set_on_mouse_focus_end(Widp w, on_mouse_focus_end_t fn)
{
  TRACE();
  w->on_mouse_focus_end = fn;
}

void wid_set_on_mouse_over_begin(Widp w, on_mouse_over_begin_t fn)
{
  TRACE();
  w->on_mouse_over_begin = fn;
}

void wid_set_on_mouse_over_end(Widp w, on_mouse_over_end_t fn)
{
  TRACE();
  w->on_mouse_over_end = fn;
}

auto wid_scroll_trough_mouse_down(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();

  int dx = 0;
  int dy = 0;

  std::vector< Widp > worklist;
  for (auto &iter : w->children_display_sorted) {
    auto *child = iter.second;
    worklist.push_back(child);
  }

  for (auto &child : worklist) {
    dx = 0;
    dy = 0;

    if (x < wid_get_tl_x(child)) {
      dx = -1;
    }

    if (x > wid_get_tl_x(child)) {
      dx = 1;
    }

    if (y < wid_get_tl_y(child)) {
      dy = -1;
    }

    if (y > wid_get_tl_y(child)) {
      dy = 1;
    }

    if ((dx != 0) || (dy != 0)) {
      wid_set_mode(child, WID_MODE_ACTIVE);
    }

    if (! wid_get_moveable_horiz(child)) {
      dx = 0;
    }

    if (! wid_get_moveable_vert(child)) {
      dy = 0;
    }

    wid_move_delta(g, child, dx, dy);
  }

  return true;
}

auto wid_scroll_motion(Gamep g, Widp w, int x, int y, int relx, int rely, int wheelx, int wheely) -> bool
{
  TRACE();

  int dx = 0;
  int dy = 0;

  bool const left = SDL_BUTTON(SDL_BUTTON_LEFT) & SDL_GetMouseState(nullptr, nullptr);
  if (left || (wheely != 0) || (wheelx != 0)) {
    dy = (rely != 0) ? rely : -wheely;
    dx = (relx != 0) ? relx : -wheelx;

    if (dx < 0) {
      dx = -1;
    }
    if (dy < 0) {
      dy = -1;
    }
    if (dx > 0) {
      dx = 1;
    }
    if (dy > 0) {
      dy = 1;
    }
  } else {
    return false;
  }

  std::vector< Widp > worklist;
  for (auto &iter : w->children_display_sorted) {
    auto *child = iter.second;
    worklist.push_back(child);
  }

  for (auto &child : worklist) {
    if ((dx != 0) || (dy != 0)) {
      wid_set_mode(child, WID_MODE_ACTIVE);
    }

    if (! wid_get_moveable_horiz(child)) {
      dx = 0;
    }

    if (! wid_get_moveable_vert(child)) {
      dy = 0;
    }

    wid_move_delta(g, child, dx, dy);
  }

  return true;
}

void wid_update_mouse(Gamep g)
{
  TRACE();

  //
  // So if we are now over a new widget that was created on top of the
  // mouse, we activate it.
  //
  int x = 0;
  int y = 0;

  SDL_GetMouseState(&x, &y);

  wid_mouse_motion(g, x, y, 0, 0, 0, 0);
}

auto wid_find_under_mouse() -> Widp
{
  TRACE();

  if (ascii_is_empty(ascii_mouse_x, ascii_mouse_y)) {
    return nullptr;
  }

  auto *w = wid_find_at(ascii_mouse_x, ascii_mouse_y);
  if (w != nullptr) {
    w = wid_get_top_parent(w);
    // con("mouse %d,%d over %s %p", ascii_mouse_x, ascii_mouse_y, w->name.c_str(), w);
    if (wid_ignore_events_only(w)) {
      return nullptr;
    }
    return w;
  }
  return nullptr;
}

auto wid_find_under_mouse_when_scrolling(Gamep g) -> Widp
{
  TRACE();

  if (ascii_is_empty(ascii_mouse_x, ascii_mouse_y)) {
    return nullptr;
  }
  auto *w = wid_find_at(ascii_mouse_x, ascii_mouse_y);
  if (w != nullptr) {
    w = wid_get_top_parent(w);
    if (wid_ignore_scroll_events(w)) {
      // con("ign2 %s.", wid_name(w).c_str());
      return nullptr;
    } // con("over2 %s.", wid_name(w).c_str());
    return w;
  }
  return w;
}

static auto wid_mouse_down_handler_at(Gamep g, Widp w, int x, int y, uint8_t strict) -> Widp
{
  TRACE();

  if (w == nullptr) [[unlikely]] {
    return nullptr;
  }

  if (! static_cast< bool >(w->visible)) {
    return nullptr;
  }

  if (static_cast< bool >(w->ignore_for_mouse_down)) {
    return nullptr;
  }

  if (wid_ignore_events(w)) {
    return nullptr;
  }

  if (static_cast< bool >(strict)) {
    if ((x < w->abs_tl.x) || (y < w->abs_tl.y) || (x > w->abs_br.x) || (y > w->abs_br.y)) {
      return nullptr;
    }
  }

  for (auto &iter : w->children_display_sorted) {
    auto *child = iter.second;

    if ((wid_focus_locked != nullptr) && (wid_get_top_parent(child) != wid_get_top_parent(wid_focus_locked))) {
      continue;
    }

    Widp closer_match = wid_mouse_down_handler_at(g, child, x, y, 1U /* strict */);
    if (closer_match != nullptr) {
      return closer_match;
    }
  }

  if (w->on_mouse_down != nullptr) {
    if ((wid_focus_locked != nullptr) && (wid_get_top_parent(w) != wid_get_top_parent(wid_focus_locked))) {
      return nullptr;
    }

    return w;
  }

  if (wid_get_moveable(w)) {
    if ((wid_focus_locked != nullptr) && (wid_get_top_parent(w) != wid_get_top_parent(wid_focus_locked))) {
      return nullptr;
    }

    return w;
  }

  //
  // Prevent mouse events that occur in the bounds of one window, leaking
  // into lower levels.
  //
  if (w->parent == nullptr) {
    if ((wid_focus_locked != nullptr) && (wid_get_top_parent(w) != wid_get_top_parent(wid_focus_locked))) {
      return nullptr;
    }

    return w;
  }

  return nullptr;
}

static auto wid_mouse_held_handler_at(Gamep g, Widp w, int x, int y, uint8_t strict) -> Widp
{
  TRACE();

  if (w == nullptr) [[unlikely]] {
    return nullptr;
  }

  if (! static_cast< bool >(w->visible)) {
    return nullptr;
  }

  if (wid_ignore_events(w)) {
    return nullptr;
  }

  if (static_cast< bool >(strict)) {
    if ((x < w->abs_tl.x) || (y < w->abs_tl.y) || (x > w->abs_br.x) || (y > w->abs_br.y)) {
      return nullptr;
    }
  }

  for (auto &iter : w->children_display_sorted) {
    auto *child = iter.second;

    if ((wid_focus_locked != nullptr) && (wid_get_top_parent(child) != wid_get_top_parent(wid_focus_locked))) {
      continue;
    }

    Widp closer_match = wid_mouse_held_handler_at(g, child, x, y, 1U /* strict */);
    if (closer_match != nullptr) {
      return closer_match;
    }
  }

  if (w->on_mouse_held != nullptr) {
    if ((wid_focus_locked != nullptr) && (wid_get_top_parent(w) != wid_get_top_parent(wid_focus_locked))) {
      return nullptr;
    }

    return w;
  }

  if (wid_get_moveable(w)) {
    if ((wid_focus_locked != nullptr) && (wid_get_top_parent(w) != wid_get_top_parent(wid_focus_locked))) {
      return nullptr;
    }

    return w;
  }

  //
  // Prevent mouse events that occur in the bounds of one window, leaking
  // into lower levels.
  //
  if (w->parent == nullptr) {
    if ((wid_focus_locked != nullptr) && (wid_get_top_parent(w) != wid_get_top_parent(wid_focus_locked))) {
      return nullptr;
    }

    return w;
  }

  return nullptr;
}

static auto wid_mouse_up_handler_at(Gamep g, Widp w, int x, int y, uint8_t strict) -> Widp
{
  TRACE();

  if (w == nullptr) [[unlikely]] {
    return nullptr;
  }

  if (! static_cast< bool >(w->visible)) {
    return nullptr;
  }

  if (wid_ignore_events(w)) {
    return nullptr;
  }

  if (static_cast< bool >(strict)) {
    if ((x < w->abs_tl.x) || (y < w->abs_tl.y) || (x > w->abs_br.x) || (y > w->abs_br.y)) {
      return nullptr;
    }
  }

  for (auto &iter : w->children_display_sorted) {
    auto *child = iter.second;

    if ((wid_focus_locked != nullptr) && (wid_get_top_parent(child) != wid_get_top_parent(wid_focus_locked))) {
      continue;
    }

    Widp closer_match = wid_mouse_up_handler_at(g, child, x, y, 1U /* strict */);
    if (closer_match != nullptr) {
      return closer_match;
    }
  }

  if (w->on_mouse_up != nullptr) {
    if ((wid_focus_locked != nullptr) && (wid_get_top_parent(w) != wid_get_top_parent(wid_focus_locked))) {
      return nullptr;
    }

    return w;
  }

  if (wid_get_moveable(w)) {
    if ((wid_focus_locked != nullptr) && (wid_get_top_parent(w) != wid_get_top_parent(wid_focus_locked))) {
      return nullptr;
    }

    return w;
  }

  //
  // Prevent mouse events that occur in the bounds of one window, leaking
  // into lower levels.
  //
  if (w->parent == nullptr) {
    if ((wid_focus_locked != nullptr) && (wid_get_top_parent(w) != wid_get_top_parent(wid_focus_locked))) {
      return nullptr;
    }

    return w;
  }

  return nullptr;
}

static auto wid_mouse_down_handler(Gamep g, int x, int y) -> Widp
{
  TRACE();

  Widp w {};

  w = wid_mouse_down_handler_at(g, wid_focus, x, y, 1U /* strict */);
  if (w != nullptr) {
    return w;
  }

  w = wid_mouse_down_handler_at(g, wid_over, x, y, 1U /* strict */);
  if (w != nullptr) {
    return w;
  }

  {
    for (auto &iter : std::ranges::reverse_view(wid_top_level)) {
      auto *c = iter.second;

      if ((wid_focus_locked != nullptr) && (wid_get_top_parent(c) != wid_get_top_parent(wid_focus_locked))) {
        continue;
      }

      c = wid_mouse_down_handler_at(g, c, x, y, 1U /* strict */);
      if (c == nullptr) [[unlikely]] {
        continue;
      }

      return c;
    }
  }

  {
    for (auto &iter : std::ranges::reverse_view(wid_top_level)) {
      auto *c = iter.second;

      if ((wid_focus_locked != nullptr) && (wid_get_top_parent(c) != wid_get_top_parent(wid_focus_locked))) {
        continue;
      }

      c = wid_mouse_down_handler_at(g, c, x, y, 0U /* strict */);
      if (c == nullptr) [[unlikely]] {
        continue;
      }

      return c;
    }
  }

  return nullptr;
}

static auto wid_mouse_held_handler(Gamep g, int x, int y) -> Widp
{
  TRACE();

  Widp w {};

  w = wid_mouse_held_handler_at(g, wid_focus, x, y, 1U /* strict */);
  if (w != nullptr) {
    return w;
  }

  w = wid_mouse_held_handler_at(g, wid_over, x, y, 1U /* strict */);
  if (w != nullptr) {
    return w;
  }

  {
    for (auto &iter : std::ranges::reverse_view(wid_top_level)) {
      auto *c = iter.second;

      if ((wid_focus_locked != nullptr) && (wid_get_top_parent(c) != wid_get_top_parent(wid_focus_locked))) {
        continue;
      }

      c = wid_mouse_held_handler_at(g, c, x, y, 1U /* strict */);
      if (c == nullptr) [[unlikely]] {
        continue;
      }

      return c;
    }
  }

  {
    for (auto &iter : std::ranges::reverse_view(wid_top_level)) {
      auto *c = iter.second;

      if ((wid_focus_locked != nullptr) && (wid_get_top_parent(c) != wid_get_top_parent(wid_focus_locked))) {
        continue;
      }

      c = wid_mouse_held_handler_at(g, c, x, y, 0U /* strict */);
      if (c == nullptr) [[unlikely]] {
        continue;
      }

      return c;
    }
  }

  return nullptr;
}

static auto wid_mouse_up_handler(Gamep g, int x, int y) -> Widp
{
  TRACE();

  Widp w {};

  w = wid_mouse_up_handler_at(g, wid_focus, x, y, 1U /* strict */);
  if (w != nullptr) {
    return w;
  }

  w = wid_mouse_up_handler_at(g, wid_over, x, y, 1U /* strict */);
  if (w != nullptr) {
    return w;
  }

  {
    for (auto &iter : std::ranges::reverse_view(wid_top_level)) {
      auto *c = iter.second;

      if ((wid_focus_locked != nullptr) && (wid_get_top_parent(c) != wid_get_top_parent(wid_focus_locked))) {
        continue;
      }

      c = wid_mouse_up_handler_at(g, c, x, y, 1U /* strict */);
      if (c == nullptr) [[unlikely]] {
        continue;
      }

      return c;
    }
  }

  {
    for (auto &iter : std::ranges::reverse_view(wid_top_level)) {
      auto *c = iter.second;

      if ((wid_focus_locked != nullptr) && (wid_get_top_parent(c) != wid_get_top_parent(wid_focus_locked))) {
        continue;
      }

      c = wid_mouse_up_handler_at(g, c, x, y, 0U /* strict */);
      if (c == nullptr) [[unlikely]] {
        continue;
      }

      return c;
    }
  }

  return nullptr;
}

static auto wid_mouse_motion_handler(int x, int y, int wheelx, int wheely) -> Widp
{
  TRACE();

  Widp w {};

  w = wid_on_screen_at[ x ][ y ];
  if (w != nullptr) {
    if (static_cast< bool >(w->hidden)) {
      return nullptr;
    }
    return w;
  }

  return nullptr;
}

void wid_mouse_motion(Gamep g, int x, int y, int relx, int rely, int wheelx, int wheely)
{
  TRACE();

  int got_one = 0;

  //
  // To avoid lag, just grab the current mouse position.
  //
  SDL_GetMouseState(&x, &y);

  //
  // Map to ascii
  //
  pixel_to_ascii(g, &x, &y);
  if (ascii_ok(x, y) == 0) {
    return;
  }
  ascii_mouse_x = x;
  ascii_mouse_y = y;

  if ((relx != 0) || (rely != 0)) {
    wid_last_mouse_motion = time_ms_cached();
  }

  if (wid_mouse_motion_recursion != 0) {
    return;
  }

  wid_mouse_motion_recursion = 1;

  uint8_t over = 0U;

  for (auto &iter : wid_top_level) {
    auto *w = iter.second;

    if ((wid_focus_locked != nullptr) && (wid_get_top_parent(w) != wid_get_top_parent(wid_focus_locked))) {
      continue;
    }

    //
    // Allow wheel events to go everywhere
    //
    if ((wheelx == 0) && (wheely == 0)) {
      w = wid_find_at(x, y);
      if (w == nullptr) [[unlikely]] {
        continue;
      }
    }

    if (wid_ignore_events(w)) {
      //
      // This wid is ignoring events, but what about the parent?
      //
      w = w->parent;
      while (w != nullptr) {
        if (! wid_ignore_events(w)) {
          break;
        }
        w = w->parent;
      }

      if (w == nullptr) [[unlikely]] {
        continue;
      }
    }

    //
    // Over a new wid.
    //

    while ((w != nullptr) && ! wid_mouse_over_begin(g, w, x, y, relx, rely, wheelx, wheely)) {
      w = w->parent;
    }

    bool done = 0u;

    if (w == nullptr) [[unlikely]] {
      //
      // Allow scrollbar to grab.
      //
    } else {
      //
      // This widget reacted somehow when we went over it. i.e. popup ot
      // function.
      //
      over = 1U;
      // topcon("mouse motion %s mouse %d,%d.", w->name.c_str(), x, y);
    }

    w = wid_mouse_motion_handler(x, y, wheelx, wheely);
    if (w != nullptr) {
      if (wid_mouse_over_begin(g, w, x, y, relx, rely, wheelx, wheely)) {
        over = 1U;
      }

      //
      // If the mouse event is fully processed then do not pass onto
      // scrollbars.
      //
      if (w->on_mouse_motion != nullptr) {
        if ((w->on_mouse_motion)(g, w, x, y, relx, rely, wheelx, wheely)) {
          got_one = 1;
          break;
        }
      }

      if (wid_over == w) {
        if ((wheelx == 0) && (wheely == 0)) {
          break;
        }
      }

      while (w != nullptr) {
        //
        // If there are scrollbars and the wid did not grab the event
        // then scroll for it.
        //
        if (wheely != 0) {
          if ((w->scrollbar_vert != nullptr) && ! wid_get_moveable_no_user_scroll(w->scrollbar_vert)) {

            got_one = 1;
            wid_move_delta(g, w->scrollbar_vert, 0, -wheely);
            done = 1u;
          }
        }

        if (wheelx != 0) {
          if ((w->scrollbar_horiz != nullptr) && ! wid_get_moveable_no_user_scroll(w->scrollbar_horiz)) {

            got_one = 1;
            wid_move_delta(g, w->scrollbar_horiz, -wheelx, 0);
            done = 1u;
          }
        }

        if (done) {
          break;
        }

        //
        // Maybe the container has a scrollbar. Try it.
        //
        w = w->parent;
      }
    }

    if (done) {
      break;
    }
  }

  if (! static_cast< bool >(over)) {
    // DBG("mouse motion not over at %u,%u.", x, y);
    wid_mouse_over_end(g);
  }

  //
  // If nothing then pass the event to the console to allow scrolling
  // of the console.
  //
  if (got_one == 0) {
    if ((wid_console_window != nullptr) && (static_cast< bool >(wid_console_window->visible))) {
      if ((wid_console_container != nullptr) && ((wheelx != 0) || (wheely != 0))) {
        Widp w = wid_console_container->scrollbar_vert;
        if (w != nullptr) {
          w = w->parent;
        }

        if ((w != nullptr) && (w->on_mouse_motion != nullptr)) {
          if ((w->on_mouse_motion)(g, w, x, y, relx, rely, wheelx, wheely)) {
            got_one = 1;
          }
        }
      }
    }
  }

  wid_mouse_motion_recursion = 0;

  if (got_one == 0) {
    if ((relx != 0) || (rely != 0) || (wheelx != 0) || (wheely != 0)) {
      (void) game_mouse_motion(g, x, y, relx, rely, wheelx, wheely);
    }
  }
}

void wid_mouse_down(Gamep g, uint32_t button, int x, int y)
{
  log("widget mouse down");
  TRACE();

  Widp w {};

  pixel_to_ascii(g, &x, &y);
  if (ascii_ok(x, y) == 0) {
    log("widget mouse down, ignore, no pixel");
    return;
  }
  ascii_mouse_x = x;
  ascii_mouse_y = y;

  w = wid_mouse_down_handler(g, x, y);
  if (w == nullptr) [[unlikely]] {
    log("widget mouse down, ignore, no handler");
    return;
  }

  //
  // Raise on mouse.
  //
  if (((w->on_mouse_down != nullptr) && (w->on_mouse_down)(g, w, x, y, button)) || wid_get_moveable(w)) {
    sound_play(g, "click");

    wid_set_focus(g, w);
    wid_set_mode(w, WID_MODE_ACTIVE);
    wid_raise(g, w);

    //
    // Move on mouse.
    //
    if (wid_get_moveable(w)) {
      wid_mouse_motion_begin(g, w, x, y);
      log("widget mouse down, mouse move");
      return;
    }

    if (g != nullptr) {
      game_last_mouse_down_set(g, time_ms_cached());
    }

    log("widget mouse down, processed, raise on mouse");
    return;
  }

  if (wid_get_moveable(w)) {
    wid_set_mode(w, WID_MODE_ACTIVE);
    wid_raise(g, w);
    wid_mouse_motion_begin(g, w, x, y);
    log("widget mouse down, make wid active");
    return;
  }

  if (game_mouse_down(g, x, y, button)) {
    sound_play(g, "click");

    if (g != nullptr) {
      game_last_mouse_down_set(g, time_ms_cached());
    }
  }
}

void wid_mouse_held(Gamep g, uint32_t button, int x, int y)
{
  TRACE();

  Widp w {};

  pixel_to_ascii(g, &x, &y);
  if (ascii_ok(x, y) == 0) {
    return;
  }
  ascii_mouse_x = x;
  ascii_mouse_y = y;

  w = wid_mouse_held_handler(g, x, y);
  if (w == nullptr) [[unlikely]] {
    return;
  }

  //
  // Raise on mouse.
  //
  if (((w->on_mouse_held != nullptr) && (w->on_mouse_held)(g, w, x, y, button)) || wid_get_moveable(w)) {

    wid_set_focus(g, w);
    wid_set_mode(w, WID_MODE_ACTIVE);
    wid_raise(g, w);

    //
    // Move on mouse.
    //
    if (wid_get_moveable(w)) {
      wid_mouse_motion_begin(g, w, x, y);
      return;
    }

    return;
  }

  if (wid_get_moveable(w)) {
    wid_set_mode(w, WID_MODE_ACTIVE);
    wid_raise(g, w);
    wid_mouse_motion_begin(g, w, x, y);
    return;
  }
}

void wid_mouse_up(Gamep g, uint32_t button, int x, int y)
{
  TRACE();

  Widp w {};

  pixel_to_ascii(g, &x, &y);
  if (ascii_ok(x, y) == 0) {
    return;
  }
  ascii_mouse_x = x;
  ascii_mouse_y = y;

  wid_mouse_motion_end(g);

  w = wid_mouse_up_handler(g, x, y);
  if (w == nullptr) [[unlikely]] {
    return;
  }

  if (((w->on_mouse_up != nullptr) && (w->on_mouse_up)(g, w, x, y, button)) || wid_get_moveable(w)) {
    sound_play(g, "click");

    wid_set_mode(w, WID_MODE_ACTIVE);
    wid_raise(g, w);
    return;
  }

  (void) game_mouse_up(g, x, y, button);
}

void wid_mouse_hide(Gamep g, bool value)
{
  TRACE();

  bool const visible = ! value;

  if (visible != wid_mouse_visible) {
    wid_mouse_visible = visible;
    if (static_cast< int >(visible) != 0) {
      sdl_mouse_warp(g, saved_mouse_x, saved_mouse_y);
    } else {
      saved_mouse_x = sdl.mouse_x;
      saved_mouse_y = sdl.mouse_y;
    }
  }
}

void wid_mouse_warp(Gamep g, Widp w)
{
  TRACE();

  int tlx = 0;
  int tly = 0;
  int brx = 0;
  int bry = 0;

  wid_get_abs_coords(w, &tlx, &tly, &brx, &bry);

  int const x = (tlx + brx) / 2;
  int const y = (tly + bry) / 2;

  sdl_mouse_warp(g, x, y);
}

void wid_mouse_move(Gamep g, Widp w)
{
  TRACE();

  int tlx = 0;
  int tly = 0;
  int brx = 0;
  int bry = 0;

  wid_get_abs_coords(w, &tlx, &tly, &brx, &bry);

  int const x = (tlx + brx) / 2;
  int const y = (tly + bry) / 2;

  saved_mouse_x = sdl.mouse_x;
  saved_mouse_y = sdl.mouse_y;

  sdl.mouse_x = x;
  sdl.mouse_y = y;
}
