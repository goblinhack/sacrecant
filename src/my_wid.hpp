//
// Copyright goblinhack@gmail.com
//

#ifndef MY_WID_HPP
#define MY_WID_HPP

#include "my_color.hpp"
#include "my_spoint.hpp"
#include "my_time.hpp"
#include "my_types.hpp"

using wid_color = enum wid_color_ {
  WID_COLOR_BG,
  WID_COLOR_TEXT_FG,
  WID_COLOR_TEXT_BG,
  WID_COLOR_MAX,
};

using wid_mode = enum wid_mode_ {
  WID_MODE_NORMAL,
  WID_MODE_OVER,
  WID_MODE_FOCUS,
  WID_MODE_ACTIVE,
  WID_MODE_LAST,
};

#define WID_MODE_FIRST WID_MODE_NORMAL

auto wid_event_to_char(const struct SDL_Keysym *evt) -> char;

using on_joy_button_t   = bool (*)(Gamep, Widp, int x, int y);
using on_key_down_t     = bool (*)(Gamep, Widp, const struct SDL_Keysym *);
using on_key_up_t       = bool (*)(Gamep, Widp, const struct SDL_Keysym *);
using on_mouse_down_t   = bool (*)(Gamep, Widp, int x, int y, uint32_t button);
using on_mouse_held_t   = bool (*)(Gamep, Widp, int x, int y, uint32_t button);
using on_mouse_motion_t = bool (*)(Gamep, Widp, int x, int y, int relx, int rely, int wx, int wy);
using on_mouse_up_t     = bool (*)(Gamep, Widp, int x, int y, uint32_t button);

using on_destroy_begin_t     = void (*)(Gamep, Widp);
using on_destroy_t           = void (*)(Gamep, Widp);
using on_mouse_focus_begin_t = void (*)(Gamep, Widp);
using on_mouse_focus_end_t   = void (*)(Gamep, Widp);
using on_mouse_over_begin_t  = void (*)(Gamep, Widp, int relx, int rely, int wheelx, int wheely);
using on_mouse_over_end_t    = void (*)(Gamep, Widp);
using on_tick_t              = void (*)(Gamep, Widp);
using on_pre_tick_t          = void (*)(Gamep, Widp);
using on_display_t           = void (*)(Gamep, Widp, spoint tl, spoint br);

[[nodiscard]] auto wid_get_void_context(Widp w) -> void *;
[[nodiscard]] auto wid_get_color(Widp w, wid_color which) -> color;

[[nodiscard]] auto wid_count(Widp w, int depth) -> int;
[[nodiscard]] auto wid_get_int_context(Widp w) -> int;
[[nodiscard]] auto wid_get_br_x(Widp w) -> int;
[[nodiscard]] auto wid_get_br_y(Widp w) -> int;
[[nodiscard]] auto wid_get_height(Widp w) -> int;
[[nodiscard]] auto wid_get_style(Widp w) -> int;
[[nodiscard]] auto wid_get_tl_x(Widp w) -> int;
[[nodiscard]] auto wid_get_tl_y(Widp w) -> int;
[[nodiscard]] auto wid_get_width(Widp w) -> int;

[[nodiscard]] auto to_string(Widp w) -> std::string;
[[nodiscard]] auto wid_get_name(Widp w) -> std::string;
[[nodiscard]] auto wid_name(Widp w) -> std::string;
[[nodiscard]] auto wid_get_text(Widp w) -> std::string;
[[nodiscard]] auto wid_get_tooltip(Widp) -> std::string;
[[nodiscard]] auto wid_get_string_context(Widp w) -> std::string;

[[nodiscard]] auto wid_find_at(int x, int y) -> Widp;
[[nodiscard]] auto wid_find(const std::string &name) -> Widp;
[[nodiscard]] auto wid_find_under_mouse() -> Widp;
[[nodiscard]] auto wid_find_under_mouse_when_scrolling(Gamep g) -> Widp;
[[nodiscard]] auto wid_find(Widp w, const std::string &name) -> Widp;
[[nodiscard]] auto wid_get_current_focus() -> Widp;
[[nodiscard]] auto wid_get_focus(Widp w) -> Widp;
[[nodiscard]] auto wid_get_head(Widp w) -> Widp;
[[nodiscard]] auto wid_get_next(Widp w) -> Widp;
[[nodiscard]] auto wid_get_parent(Widp w) -> Widp;
[[nodiscard]] auto wid_get_prev(Widp w) -> Widp;
[[nodiscard]] auto wid_get_scrollbar_horiz(Widp w) -> Widp;
[[nodiscard]] auto wid_get_scrollbar_vert(Widp w) -> Widp;
[[nodiscard]] auto wid_get_tail(Widp w) -> Widp;
[[nodiscard]] auto wid_get_top_parent(Widp w) -> Widp;
[[nodiscard]] auto wid_new_container(Gamep g, Widp parent, const std::string &name) -> Widp;
[[nodiscard]] auto wid_new_horiz_scroll_bar(Gamep g, Widp parent, const std::string &name, Widp scrollbar_owner) -> Widp;
[[nodiscard]] auto wid_new_plain(Gamep g, Widp parent, const std::string &name) -> Widp;
[[nodiscard]] auto wid_new_square_button(Gamep g, Widp parent, const std::string &name) -> Widp;
[[nodiscard]] auto wid_new_window(Gamep g, const std::string &name) -> Widp;
[[nodiscard]] auto wid_new_vert_scroll_bar(Gamep g, Widp parent, const std::string &name, Widp scrollbar_owner) -> Widp;

[[nodiscard]] auto wid_get_do_not_lower(Widp w) -> bool;
[[nodiscard]] auto wid_get_do_not_raise(Widp w) -> bool;
[[nodiscard]] auto wid_get_focusable(Widp w) -> bool;
[[nodiscard]] auto wid_get_mode_top(Widp) -> bool;
[[nodiscard]] auto wid_get_moveable_bounded(Widp w) -> bool;
[[nodiscard]] auto wid_get_moveable_horiz(Widp w) -> bool;
[[nodiscard]] auto wid_get_moveable_no_user_scroll(Widp w) -> bool;
[[nodiscard]] auto wid_get_moveable_vert(Widp w) -> bool;
[[nodiscard]] auto wid_get_moveable(Widp w) -> bool;
[[nodiscard]] auto wid_get_received_input(Widp w) -> bool;
[[nodiscard]] auto wid_get_show_cursor(Widp w) -> bool;
[[nodiscard]] auto wid_get_text_bot(Widp w) -> bool;
[[nodiscard]] auto wid_get_text_centerx(Widp w) -> bool;
[[nodiscard]] auto wid_get_text_centery(Widp w) -> bool;
[[nodiscard]] auto wid_get_text_lhs(Widp w) -> bool;
[[nodiscard]] auto wid_get_text_pos(Widp w, int *x, int *y) -> bool;
[[nodiscard]] auto wid_get_text_rhs(Widp w) -> bool;
[[nodiscard]] auto wid_get_text_top(Widp w) -> bool;
[[nodiscard]] auto wid_ignore_being_destroyed(Widp w) -> bool;
[[nodiscard]] auto wid_ignore_events_only(Widp w) -> bool;
[[nodiscard]] auto wid_ignore_events(Widp w) -> bool;
[[nodiscard]] auto wid_ignore_for_focus(Widp w) -> bool;
[[nodiscard]] auto wid_ignore_scroll_events(Widp w) -> bool;
[[nodiscard]] auto wid_init() -> bool;
[[nodiscard]] auto wid_is_always_hidden(Widp w) -> bool;
[[nodiscard]] auto wid_is_hidden(Widp w) -> bool;
[[nodiscard]] auto wid_is_visible(Widp w) -> bool;
[[nodiscard]] auto wid_receive_input(Gamep g, Widp w, const SDL_Keysym *key) -> bool;
[[nodiscard]] auto wid_scroll_trough_mouse_down(Gamep g, Widp w, int x, int y, uint32_t button) -> bool;
[[nodiscard]] auto wid_some_recent_event_occurred() -> bool;
[[nodiscard]] auto wid_scroll_motion(Gamep g, Widp w, int x, int y, int relx, int rely, int wheelx, int wheely) -> bool;

[[nodiscard]] auto wid_get_mode(Widp w) -> wid_mode;

void wid_destroy_in(Gamep g, Widp w, uint32_t ms);
void wid_destroy_nodelay(Gamep g, Widp *wp);
void wid_destroy_ptr_in(Gamep, Widp *w, uint32_t ms);
void wid_destroy(Gamep g, Widp *wp);
void wid_display_all(Gamep g);
void wid_dump(Widp w, int depth);
void wid_fake_joy_button(Gamep g, int x, int y);
void wid_fini(Gamep g_maybe_null);
void wid_focus_lock(Gamep g, Widp w);
void wid_gc_all_force(Gamep);
void wid_gc_all(Gamep g);
void wid_get_abs_coords_unclipped(Widp w, int *tlx, int *tly, int *brx, int *bry);
void wid_get_abs_coords(Widp w, int *tlx, int *tly, int *brx, int *bry);
void wid_get_abs(Widp w, int *x, int *y);
void wid_get_children_size(Widp owner, int *width, int *height);
void wid_get_pct(Widp w, float *x, float *y);
void wid_get_tl_x_tl_y_br_x_br_y(Widp w, int *tl_x, int *tl_y, int *br_x, int *br_y);
void wid_hide(Gamep g, Widp w);
void wid_joy_button(Gamep g, int x, int y);
void wid_key_down(Gamep g, const struct SDL_Keysym *key, int x, int y);
void wid_key_up(Gamep g, const struct SDL_Keysym *key, int x, int y);
void wid_lower(Gamep g, Widp w_in);
void wid_mouse_down(Gamep g, uint32_t button, int x, int y);
void wid_mouse_focus_begin(Gamep g, Widp w);
void wid_mouse_focus_end(Gamep g_maybe_null);
void wid_mouse_held(Gamep g, uint32_t button, int x, int y);
void wid_mouse_hide(Gamep g, bool value);
void wid_mouse_motion_begin(Gamep g, Widp w, int x, int y);
void wid_mouse_motion_end(Gamep g);
void wid_mouse_motion(Gamep g, int x, int y, int relx, int rely, int wheelx, int wheely);
void wid_mouse_move(Gamep g, Widp w);
void wid_mouse_over_end(Gamep g);
void wid_mouse_up(Gamep g, uint32_t button, int x, int y);
void wid_mouse_warp(Gamep g, Widp w);
void wid_move_delta_pct(Gamep g, Widp w, float dx, float dy);
void wid_move_delta(Gamep g, Widp w, int dx, int dy);
void wid_move_end(Gamep, Widp);
void wid_move_to_abs_centered(Gamep g, Widp w, int dx, int dy);
void wid_move_to_abs(Gamep g, Widp w, int dx, int dy);
void wid_move_to_bottom(Gamep g, Widp w);
void wid_move_to_horiz_pct(Gamep g, Widp w, float pct);
void wid_move_to_left(Gamep g, Widp w);
void wid_move_to_pct_centered(Gamep g, Widp w, float ox, float oy);
void wid_move_to_pct(Gamep g, Widp w, float dx, float dy);
void wid_move_to_right(Gamep g, Widp w);
void wid_move_to_top(Gamep g, Widp w);
void wid_move_to_vert_pct(Gamep g, Widp w, float pct);
void wid_move_to_y_off(Gamep g, Widp w, int off);
void wid_move_to(Gamep g, Widp w, int x, int y);
void wid_raise(Gamep g, Widp w_in);
void wid_resize(Gamep g, Widp w, int width, int height);
void wid_sanity_check(Gamep g);
void wid_scroll_text(Widp w);
void wid_scroll_with_input(Gamep g, Widp w, const std::string &str);
void wid_set_active(Gamep g, Widp w);
void wid_set_color(Widp w, wid_color col, color val);
void wid_set_cursor(Widp w, uint32_t val);
void wid_set_debug(Widp w, bool val = true);
void wid_set_do_not_lower(Widp w, bool val = true);
void wid_set_do_not_raise(Widp w, bool val = true);
void wid_set_first_focus(Gamep g);
void wid_set_focus(Gamep g, Widp w);
void wid_set_focusable(Widp w, bool val = true);
void wid_set_ignore_events(Widp w, bool val = true);
void wid_set_ignore_for_mouse_down(Widp w, bool val = true);
void wid_set_ignore_scroll_events(Widp w, bool val = true);
void wid_set_int_context(Widp w, int /*int_context*/);
void wid_set_mode(Widp w, wid_mode mode);
void wid_set_moveable_bounded(Gamep g, Widp w, bool val = true);
void wid_set_moveable_horiz(Gamep g, Widp w, bool val = true);
void wid_set_moveable_no_user_scroll(Gamep g, Widp w, bool val = true);
void wid_set_moveable_vert(Gamep g, Widp w, bool val = true);
void wid_set_moveable(Gamep g, Widp w, bool val = true);
void wid_set_name(Widp w, const std::string &name);
void wid_set_on_destroy_begin(Widp w, on_destroy_begin_t fn);
void wid_set_on_destroy(Widp w, on_destroy_t fn);
void wid_set_on_display(Widp w, on_display_t fn);
void wid_set_on_joy_button(Widp w, on_joy_button_t fn);
void wid_set_on_key_down(Widp w, on_key_down_t fn);
void wid_set_on_key_up(Widp w, on_key_up_t fn);
void wid_set_on_mouse_down(Widp w, on_mouse_down_t fn);
void wid_set_on_mouse_focus_begin(Widp w, on_mouse_focus_begin_t fn);
void wid_set_on_mouse_focus_end(Widp w, on_mouse_focus_end_t fn);
void wid_set_on_mouse_held(Widp w, on_mouse_down_t fn);
void wid_set_on_mouse_motion(Widp w, on_mouse_motion_t fn);
void wid_set_on_mouse_over_begin(Widp w, on_mouse_over_begin_t fn);
void wid_set_on_mouse_over_end(Widp w, on_mouse_over_end_t fn);
void wid_set_on_mouse_up(Widp w, on_mouse_up_t fn);
void wid_set_on_tick(Widp w, on_tick_t fn);
void wid_set_on_pre_tick(Widp w, on_pre_tick_t fn);
void wid_set_pos(Widp w, spoint tl, spoint br);
void wid_set_prev(Widp w, Widp prev);
void wid_set_received_input(Widp w, bool val = true);
void wid_set_shape_none(Widp w);
void wid_set_shape_square(Widp w);
void wid_set_show_cursor(Widp w, bool val = true);
void wid_set_string_context(Widp w, std::string /*string_context*/);
void wid_set_style(Widp w, int style);
void wid_set_text_bot(Widp w, bool val = true);
void wid_set_text_centerx(Widp w, bool val = true);
void wid_set_text_centery(Widp w, bool val = true);
void wid_set_text_lhs(Widp w, bool val = true);
void wid_set_text_max_len(Widp w, size_t /*max_len*/);
void wid_set_text_rhs(Widp w, bool val = true);
void wid_set_text_top(Widp w, bool val = true);
void wid_set_text(Widp w, int v);
void wid_set_text(Widp w, const std::string &text);
void wid_set_tile(int depth, Widp w, Tilep tile);
void wid_set_tilename(int depth, Widp w, const std::string &name);
void wid_set_top_focus(Gamep g);
void wid_set_top(Gamep, Widp, bool val = true);
void wid_set_void_context(Widp w, void *context);
void wid_toggle_hidden(Gamep g, Widp w);
void wid_unset_focus_lock();
void wid_unset_focus(Gamep g);
void wid_update_mouse(Gamep g);
void wid_update(Gamep g, Widp w);
void wid_visible(Gamep g, Widp w);

extern bool wid_exiting;
extern bool wid_init_done;
extern bool wid_mouse_two_clicks;
extern bool wid_mouse_visible;

extern const int wid_destroy_delay_ms;

extern ts_t wid_last_mouse_motion;
extern ts_t wid_last_over_event;

extern Widp wid_focus;
extern Widp wid_over;
extern Widp wid_focus_locked;

#endif
