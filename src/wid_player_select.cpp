//
// Copyright goblinhack@gmail.com
//

#include "my_ascii.hpp"
#include "my_bpoint.hpp"
#include "my_callstack.hpp"
#include "my_color_defs.hpp"
#include "my_game.hpp"
#include "my_game_defs.hpp"
#include "my_game_inlines.hpp"
#include "my_level.hpp"
#include "my_level_inlines.hpp" // NOLINT
#include "my_main.hpp"
#include "my_random.hpp"
#include "my_sdl_proto.hpp"
#include "my_sound.hpp"
#include "my_spoint.hpp"
#include "my_sprintf.hpp"
#include "my_string.hpp"
#include "my_thing.hpp"
#include "my_thing_inlines.hpp" // NOLINT
#include "my_tile.hpp"
#include "my_tp.hpp"
#include "my_tp_inlines.hpp"
#include "my_types.hpp"
#include "my_ui.hpp"
#include "my_wid.hpp"
#include "my_wids.hpp"

#include <SDL_keyboard.h>
#include <SDL_keycode.h>
#include <algorithm>
#include <cstdint>
#include <cstring>
#include <string>
#include <vector>

static Widp      wid_total;
static Widp      wid_player_select_window;
static WidPopup *wid_player_select_continue_window;

static int  wid_player_index = 0;
static Widp wid_player_shortcut[ THING_INVENTORY_MAX ];
static Widp wid_player[ THING_INVENTORY_MAX ];
static Widp wid_player_icon[ THING_INVENTORY_MAX ];

static int  wid_sacrifice_index = 0;
static Widp wid_sacrifice_shortcut[ THING_INVENTORY_MAX ];
static Widp wid_sacrifice[ THING_INVENTORY_MAX ];

static int  wid_boost_index = 0;
static Widp wid_boost_shortcut[ THING_INVENTORY_MAX ];
static Widp wid_boost[ THING_INVENTORY_MAX ];

static void wid_player_select_destroy(Gamep g)
{
  TRACE();

  memset(wid_player_shortcut, 0, sizeof(wid_player_shortcut));
  memset(wid_player_icon, 0, sizeof(wid_player_icon));
  memset(wid_player, 0, sizeof(wid_player));

  memset(wid_sacrifice_shortcut, 0, sizeof(wid_sacrifice_shortcut));
  memset(wid_sacrifice, 0, sizeof(wid_sacrifice));

  memset(wid_boost_shortcut, 0, sizeof(wid_boost_shortcut));
  memset(wid_boost, 0, sizeof(wid_boost));

  game_mouse_over_player_set(g, nullptr);
  game_mouse_over_sacrifice_set(g, nullptr);
  game_mouse_over_boost_set(g, nullptr);

  game_cand_player_set(g, nullptr);
  game_cand_sacrifice_set(g, nullptr);
  game_cand_boost_set(g, nullptr);

  if (wid_player_select_window != nullptr) {
    wid_destroy(g, &wid_player_select_window);
  }

  delete wid_player_select_continue_window;
  wid_player_select_continue_window = nullptr;
}

static void wid_player_select_all_done(Gamep g)
{
  TRACE();

  auto *tp = game_cand_player_get(g);
  game_chosen_player_set(g, tp);
  game_chosen_sacrifice_set(g, game_cand_sacrifice_get(g));
  game_chosen_boost_set(g, game_cand_boost_get(g));

  wid_player_select_destroy(g);

  game_difficulty_set(g, tp_difficulty_get(tp));

  wid_progress_bar(g, "Generating...", 0.0F);
  wid_new_game(g);
  wid_progress_bar(g, "Generating...", 1.0F);
  wid_progress_bar_destroy(g);

  game_player_clear(g);
  game_sacrifice_clear(g);
  game_boost_clear(g);
}

[[nodiscard]] static auto wid_continue_mouse_down(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();

  wid_player_select_all_done(g);

  return true;
}

static void wid_player_select_check_if_done(Gamep g)
{
  TRACE();

  if ((game_cand_player_get(g) == nullptr) || (game_cand_sacrifice_get(g).empty())) {
    return;
  }

  if (wid_player_select_continue_window == nullptr) {
    auto         m = TERM_WIDTH / 2;
    auto         n = TERM_HEIGHT - 5;
    spoint const outer_tl(m - (UI_WID_POPUP_WIDTH_NORMAL / 2), n - 3);
    spoint const outer_br(m + (UI_WID_POPUP_WIDTH_NORMAL / 2), n + 1);
    auto         width = outer_br.x - outer_tl.x;

    wid_player_select_continue_window = new WidPopup(g, "game continue", outer_tl, outer_br, nullptr, "", false, false);
    auto *p                           = wid_player_select_continue_window->wid_text_area->wid_text_area;
    auto *w                           = wid_new_continue_button(g, p, "continue");

    spoint const tl(0, 0);
    spoint const br(width - 2, 2);
    wid_set_pos(w, tl, br);
    wid_set_text(w, "Continue?");
    wid_set_on_mouse_down(w, wid_continue_mouse_down);
  }
}

static int wid_player_total_sac_points(Gamep g)
{
  TRACE();

  auto *v = levels_memory_alloc(g);
  if (v == nullptr) {
    return 0;
  }

  Widp w = nullptr;
  int  total_sac_points {};

  for (auto &n : wid_player) {
    w = n;
    if (w != nullptr) {
      auto *t = wid_get_thing_context(g, v, w, 0);
      if (t == game_cand_player_get_thing(g)) {
        total_sac_points += tp_sac_points_get(thing_tp(t));
      }
    }
  }

  for (auto &n : wid_sacrifice) {
    w = n;
    if (w != nullptr) {
      auto *t = wid_get_thing_context(g, v, w, 0);
      if (game_cand_sacrifice_find(g, t)) {
        total_sac_points += tp_sac_points_get(thing_tp(t));
      }
    }
  }

  for (auto &n : wid_boost) {
    w = n;
    if (w != nullptr) {
      auto *t = wid_get_thing_context(g, v, w, 0);
      if (game_cand_boost_find(g, t)) {
        total_sac_points += tp_sac_points_get(thing_tp(t));
      }
    }
  }

  return total_sac_points;
}

static void wid_player_update_selections(Gamep g)
{
  TRACE();

  auto *v = levels_memory_alloc(g);
  if (v == nullptr) {
    return;
  }

  Widp w = nullptr;

  wid_unset_focus(g);
  wid_mouse_over_end(g);

  for (auto &n : wid_player) {
    w = n;
    if (w != nullptr) {
      wid_set_mode(w, WID_MODE_OVER);
      wid_set_style(w, UI_WID_STYLE_BUTTON_BAR);
      wid_set_color(w, WID_COLOR_BG, GREEN);
      wid_set_color(w, WID_COLOR_TEXT_FG, WHITE);
      wid_set_mode(w, WID_MODE_NORMAL);
      wid_set_style(w, UI_WID_STYLE_BUTTON_BAR);
      wid_set_color(w, WID_COLOR_BG, GRAY10);

      auto *t = wid_get_thing_context(g, v, w, 0);
      if (t == game_cand_player_get_thing(g)) {
        wid_set_mode(w, WID_MODE_OVER);
        wid_set_style(w, UI_WID_STYLE_BUTTON_BAR);
        wid_set_color(w, WID_COLOR_BG, RED);
        wid_set_color(w, WID_COLOR_TEXT_FG, WHITE);
        wid_set_mode(w, WID_MODE_NORMAL);
        wid_set_style(w, UI_WID_STYLE_BUTTON_BAR);
        wid_set_color(w, WID_COLOR_BG, RED);
      }
    }
  }

  for (auto &n : wid_sacrifice) {
    w = n;
    if (w != nullptr) {
      wid_set_mode(w, WID_MODE_OVER);
      wid_set_style(w, UI_WID_STYLE_BUTTON_BAR);
      wid_set_color(w, WID_COLOR_BG, GREEN);
      wid_set_color(w, WID_COLOR_TEXT_FG, WHITE);
      wid_set_mode(w, WID_MODE_NORMAL);
      wid_set_style(w, UI_WID_STYLE_BUTTON_BAR);
      wid_set_color(w, WID_COLOR_BG, GRAY10);

      auto *t = wid_get_thing_context(g, v, w, 0);
      if (game_cand_sacrifice_find(g, t)) {
        wid_set_mode(w, WID_MODE_OVER);
        wid_set_style(w, UI_WID_STYLE_BUTTON_BAR);
        wid_set_color(w, WID_COLOR_BG, RED);
        wid_set_color(w, WID_COLOR_TEXT_FG, WHITE);
        wid_set_mode(w, WID_MODE_NORMAL);
        wid_set_style(w, UI_WID_STYLE_BUTTON_BAR);
        wid_set_color(w, WID_COLOR_BG, RED);
      }
    }
  }

  for (auto &n : wid_boost) {
    w = n;
    if (w != nullptr) {
      wid_set_mode(w, WID_MODE_OVER);
      wid_set_style(w, UI_WID_STYLE_BUTTON_BAR);
      wid_set_color(w, WID_COLOR_BG, GREEN);
      wid_set_color(w, WID_COLOR_TEXT_FG, WHITE);
      wid_set_mode(w, WID_MODE_NORMAL);
      wid_set_style(w, UI_WID_STYLE_BUTTON_BAR);
      wid_set_color(w, WID_COLOR_BG, GRAY10);

      auto *t = wid_get_thing_context(g, v, w, 0);

      if (game_cand_boost_find(g, t)) {
        wid_set_mode(w, WID_MODE_OVER);
        wid_set_style(w, UI_WID_STYLE_BUTTON_BAR);
        wid_set_color(w, WID_COLOR_BG, RED);
        wid_set_color(w, WID_COLOR_TEXT_FG, WHITE);
        wid_set_mode(w, WID_MODE_NORMAL);
        wid_set_style(w, UI_WID_STYLE_BUTTON_BAR);
        wid_set_color(w, WID_COLOR_BG, RED);
      } else {
        auto total_sac_points = wid_player_total_sac_points(g);
        auto boost_sac_points = tp_sac_points_get(thing_tp(t));
        if (total_sac_points < -boost_sac_points) {
          wid_set_color(w, WID_COLOR_TEXT_FG, ORANGE);
        } else {
          wid_set_color(w, WID_COLOR_TEXT_FG, WHITE);
        }
      }
    }
  }

  {
    auto total_sac_points = wid_player_total_sac_points(g);
    auto line             = string_sprintf("Sacrifical points (SPs) for spell casting       %d", total_sac_points);
    wid_set_text_lhs(wid_total, 1u);
    wid_set_text(wid_total, line);
    wid_update(g, wid_total);
  }
}

static void wid_player_select_player_via_mouse_over_begin(Gamep g, Widp w, int /*relx*/, int /*rely*/, int /*wheelx*/, int /*wheely*/)
{
  TRACE();

  auto *v = levels_memory_alloc(g);
  if (v == nullptr) {
    return;
  }

  auto *t = wid_get_thing_context(g, v, w, 0);
  if (t == nullptr) {
    return;
  }

  game_mouse_over_player_set(g, t);

  level_cursor_describe_clear(g, v);

  if (level_cursor_describe_add(g, v, t)) {
    game_request_to_remake_ui_set(g);
  }
}

static void wid_player_select_player_via_mouse_over_end(Gamep g, Widp w)
{
  TRACE();

  auto *v = levels_memory_alloc(g);
  if (v == nullptr) {
    return;
  }

  auto *t = wid_get_thing_context(g, v, w, 0);
  if (t == nullptr) {
    return;
  }

  game_mouse_over_player_set(g, nullptr);

  if (level_cursor_describe_remove(g, v, t)) {
    game_request_to_remake_ui_set(g);
  }
}

[[nodiscard]] static auto wid_player_select_player_via_mouse_down(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();

  auto *v = levels_memory_alloc(g);
  if (v == nullptr) {
    return false;
  }

  auto *t = wid_get_thing_context(g, v, w, 0);
  if (t == nullptr) {
    return false;
  }

  if (game_cand_player_get_thing(g) == t) {
    game_cand_player_unset(g);
  } else {
    game_cand_player_set(g, t);
  }

  if (game_state(g) == STATE_PLAYER_SELECT_MENU) {
    (void) sound_play(g, "select");
  }

  wid_player_select_check_if_done(g);
  wid_player_update_selections(g);
  game_request_to_remake_ui_set(g);

  return true;
}

static void wid_player_select_sacrifice_via_mouse_over_begin(Gamep g, Widp w, int /*relx*/, int /*rely*/, int /*wheelx*/, int /*wheely*/)
{
  TRACE();

  auto *v = levels_memory_alloc(g);
  if (v == nullptr) {
    return;
  }

  auto *t = wid_get_thing_context(g, v, w, 0);
  if (t == nullptr) {
    return;
  }

  game_mouse_over_sacrifice_set(g, t);

  level_cursor_describe_clear(g, v);

  if (level_cursor_describe_add(g, v, t)) {
    game_request_to_remake_ui_set(g);
  }
}

static void wid_player_select_sacrifice_via_mouse_over_end(Gamep g, Widp w)
{
  TRACE();

  auto *v = levels_memory_alloc(g);
  if (v == nullptr) {
    return;
  }

  auto *t = wid_get_thing_context(g, v, w, 0);
  if (t == nullptr) {
    return;
  }

  game_mouse_over_sacrifice_set(g, nullptr);

  if (level_cursor_describe_remove(g, v, t)) {
    game_request_to_remake_ui_set(g);
  }
}

[[nodiscard]] static auto wid_player_select_sacrifice_via_mouse_down(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();

  auto *v = levels_memory_alloc(g);
  if (v == nullptr) {
    return false;
  }

  auto *t = wid_get_thing_context(g, v, w, 0);
  if (t == nullptr) {
    return false;
  }

  if (game_cand_sacrifice_find(g, t)) {
    game_cand_sacrifice_unset(g, t);
  } else {
    game_cand_sacrifice_set(g, t);
  }

  wid_player_select_check_if_done(g);
  wid_player_update_selections(g);
  game_request_to_remake_ui_set(g);

  if (game_state(g) == STATE_PLAYER_SELECT_MENU) {
    (void) sound_play(g, "select");
  }

  return true;
}

static void wid_player_select_boost_via_mouse_over_begin(Gamep g, Widp w, int /*relx*/, int /*rely*/, int /*wheelx*/, int /*wheely*/)
{
  TRACE();

  auto *v = levels_memory_alloc(g);
  if (v == nullptr) {
    return;
  }

  auto *t = wid_get_thing_context(g, v, w, 0);
  if (t == nullptr) {
    return;
  }

  game_mouse_over_boost_set(g, t);

  level_cursor_describe_clear(g, v);

  if (level_cursor_describe_add(g, v, t)) {
    game_request_to_remake_ui_set(g);
  }
}

static void wid_player_select_boost_via_mouse_over_end(Gamep g, Widp w)
{
  TRACE();

  auto *v = levels_memory_alloc(g);
  if (v == nullptr) {
    return;
  }

  auto *t = wid_get_thing_context(g, v, w, 0);
  if (t == nullptr) {
    return;
  }

  game_mouse_over_boost_set(g, nullptr);

  if (level_cursor_describe_remove(g, v, t)) {
    game_request_to_remake_ui_set(g);
  }
}

[[nodiscard]] static auto wid_player_select_boost_via_mouse_down(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();

  auto *v = levels_memory_alloc(g);
  if (v == nullptr) {
    return false;
  }

  auto *t = wid_get_thing_context(g, v, w, 0);
  if (t == nullptr) {
    return false;
  }

  if (game_cand_boost_find(g, t)) {
    game_cand_boost_unset(g, t);
  } else {
    //
    // Check we have enough sac_points for this
    //
    auto total_sac_points  = wid_player_total_sac_points(g);
    auto sac_points_change = tp_sac_points_get(thing_tp(t));
    if (total_sac_points + sac_points_change < 0) {
      (void) sound_play(g, "error");
      topcon("Not enough sac_points to buy this boost.\n");
      return true;
    }

    game_cand_boost_set(g, t);
  }

  wid_player_select_check_if_done(g);
  wid_player_update_selections(g);
  game_request_to_remake_ui_set(g);

  if (game_state(g) == STATE_PLAYER_SELECT_MENU) {
    (void) sound_play(g, "select");
  }

  return true;
}

[[nodiscard]] static auto wid_player_select_key_down(Gamep g, Widp w, const struct SDL_Keysym *key) -> bool
{
  TRACE();

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
              case ' ' :
                if (wid_player_select_continue_window != nullptr) {
                  //
                  // All done
                  //
                  wid_player_select_all_done(g);
                } else {
                  //
                  // Choose some random player
                  //
                  w = wid_player[ PCG_RANDOM_RANGE(0, wid_player_index) ];
                  if (w != nullptr) {
                    (void) wid_player_select_player_via_mouse_down(g, w, -1, -1, 0);
                  }
                  w = wid_sacrifice[ PCG_RANDOM_RANGE(0, wid_sacrifice_index) ];
                  if (w != nullptr) {
                    (void) wid_player_select_sacrifice_via_mouse_down(g, w, -1, -1, 0);
                  }
                  w = wid_boost[ PCG_RANDOM_RANGE(0, wid_boost_index) ];
                  if (w != nullptr) {
                    (void) wid_player_select_boost_via_mouse_down(g, w, -1, -1, 0);
                  }
                }
                break;

              case '0' :
              case '1' :
              case '2' :
              case '3' :
              case '4' :
              case '5' :
              case '6' :
              case '7' :
              case '8' :
              case '9' :
                game_mouse_over_player_set(g, nullptr);
                w = wid_player[ c - '0' ];
                if (w != nullptr) {
                  (void) wid_player_select_player_via_mouse_down(g, w, -1, -1, 0);
                }
                break;

              case 'a' :
              case 'b' :
              case 'c' :
              case 'd' :
              case 'e' :
              case 'f' :
              case 'g' :
              case 'h' :
              case 'i' :
              case 'j' :
              case 'k' :
              case 'l' :
              case 'm' :
              case 'n' :
              case 'o' :
              case 'p' :
              case 'q' :
              case 'r' :
              case 's' :
              case 't' :
              case 'u' :
              case 'v' :
              case 'w' :
              case 'x' :
              case 'y' :
              case 'z' :
                game_mouse_over_sacrifice_set(g, nullptr);
                w = wid_sacrifice[ c - 'a' ];
                if (w != nullptr) {
                  (void) wid_player_select_sacrifice_via_mouse_down(g, w, -1, -1, 0);
                }
                break;

              case 'A' :
              case 'B' :
              case 'C' :
              case 'D' :
              case 'E' :
              case 'F' :
              case 'G' :
              case 'H' :
              case 'I' :
              case 'J' :
              case 'K' :
              case 'L' :
              case 'M' :
              case 'N' :
              case 'O' :
              case 'P' :
              case 'Q' :
              case 'R' :
              case 'S' :
              case 'T' :
              case 'U' :
              case 'V' :
              case 'W' :
              case 'X' :
              case 'Y' :
              case 'Z' :
                game_mouse_over_boost_set(g, nullptr);
                w = wid_boost[ c - 'A' ];
                if (w != nullptr) {
                  (void) wid_player_select_boost_via_mouse_down(g, w, -1, -1, 0);
                }
                break;
            }
          }
      }
  }

  //
  // So screenshots can work
  //
  return false;
}

void wid_player_select(Gamep g)
{
  con("Player select menu: create");
  TRACE_INDENT();

  auto *v = levels_memory_alloc(g);
  if (v == nullptr) {
    return;
  }

  (void) game_levels_set(g, v);

  auto *level_select = game_level_get(g, v, LEVEL_ARR_IDX_LEVEL_SELECT);
  if (level_select == nullptr) {
    return;
  }

  level_init(g, v, level_select, LEVEL_ARR_IDX_LEVEL_SELECT);

  if (wid_player_select_window != nullptr) {
    wid_player_select_destroy(g);
  }

  const int player_select_width  = UI_INVENTORY_WIDTH;
  const int player_select_height = UI_INVENTORY_HEIGHT;

  const auto button_width  = player_select_width - 2;
  const auto button_height = 0;
  const auto button_step   = 1;
  const auto button_style  = UI_WID_STYLE_SPARSE_NONE;

  auto y_at = 2;

  const int left_half  = player_select_width / 2;
  const int right_half = player_select_width - left_half;
  const int top_half   = player_select_height / 2;
  const int bot_half   = player_select_height - top_half;

  {
    TRACE();
    spoint const tl((TERM_WIDTH / 2) - left_half, (TERM_HEIGHT / 2) - top_half);
    spoint const br((TERM_WIDTH / 2) + right_half - 1, (TERM_HEIGHT / 2) + bot_half - 1);

    wid_player_select_window = wid_new_window(g, "widget player_select");
    wid_set_pos(wid_player_select_window, tl, br);
    wid_set_style(wid_player_select_window, UI_WID_STYLE_BUTTON_OUTLINE);
    wid_set_on_key_down(wid_player_select_window, wid_player_select_key_down);
    wid_set_text_top(wid_player_select_window, 1u);
    wid_raise(g, wid_player_select_window);
  }

  {
    TRACE();
    auto        *w = wid_new_square_button(g, wid_player_select_window, "text");
    spoint const tl(0, y_at);
    spoint const br(player_select_width, y_at);
    wid_set_pos(w, tl, br);
    if (v->tick != 0U) {
      wid_set_text(w, UI_INFO_FMT_STR "Choose your next sacrifice");
    } else {
      wid_set_text(w, UI_INFO_FMT_STR "Choose a sacrecant and at least one sacrifice.");
    }
    wid_set_style(w, UI_WID_STYLE_BUTTON_OUTLINE);
    wid_set_shape_none(w);
    wid_set_text_centerx(w, 1u);
    y_at += 1;
  }

  if (v->tick == 0U) {
    TRACE();
    auto        *w = wid_new_square_button(g, wid_player_select_window, "text");
    spoint const tl(0, y_at);
    spoint const br(player_select_width, y_at);
    wid_set_pos(w, tl, br);
    wid_set_text(w, UI_INFO1_FMT_STR "Or press 'SPACE' for unlucky dip!");
    wid_set_style(w, UI_WID_STYLE_BUTTON_OUTLINE);
    wid_set_shape_none(w);
    wid_set_text_centerx(w, 1u);
    y_at += 2;
  }

  //
  // Sacrecant
  //
  {
    TRACE();
    auto *w = wid_new_bar_button(g, wid_player_select_window, "Sacrecant");

    spoint const tl(1, y_at);
    spoint const br(button_width, y_at + button_height);
    wid_set_text_lhs(w, 1u);
    wid_set_pos(w, tl, br);
    wid_set_text(w, UI_INFO_FMT_STR "Sacrecant                                      SPs ");
    y_at++;
  }

  memset(wid_player_shortcut, 0, sizeof(wid_player_shortcut));
  memset(wid_player_icon, 0, sizeof(wid_player_icon));
  memset(wid_player, 0, sizeof(wid_player));

  wid_player_index = 0;

  std::vector< Tpp > wid_player_tps;

  for (auto &tp : tp_vec) {
    if (! tp_is_player(tp)) {
      continue;
    }
    wid_player_tps.push_back(tp);
  }

  //
  // Sort by sac_points
  //
  std::ranges::sort(wid_player_tps, [](const Tpp &a, const Tpp &b) -> bool { return tp_sac_points_get(a) < tp_sac_points_get(b); });

  for (auto &tp : wid_player_tps) {
    //
    // Check for overflow
    //
    if (wid_player_index >= ARRAY_SIZE(wid_player)) {
      break;
    }

    //
    // Create a temporary thing on the level select map
    //
    auto   at             = bpoint(0, wid_player_index);
    Thingp existing_thing = nullptr;
    FOR_ALL_THINGS_AT(g, v, level_select, t, at)
    {
      if (t != nullptr) {
        existing_thing = t;
        break;
      }
    }

    if (existing_thing == nullptr) {
      existing_thing = thing_spawn(g, v, level_select, tp, at);
      if (existing_thing == nullptr) {
        continue;
      }
    }

    if (game_mouse_over_player_get(g) == nullptr) {
      game_mouse_over_player_set(g, existing_thing);
    }

    //
    // Sacrecant icon
    //
    Tilep tile = tp_tiles_get(tp, THING_ANIM_IDLE, 0);
    if (tile != nullptr) {
      TRACE();
      auto        *w = wid_new_square_button(g, wid_player_select_window, "Icon");
      spoint const tl(1, y_at);
      spoint const br(1, y_at);
      wid_set_tile(TILE_LAYER_TEXT_FG, w, tile);
      wid_set_style(w, button_style);
      wid_set_pos(w, tl, br);

      wid_set_thing_context(g, v, w, existing_thing);
      wid_set_on_mouse_down(w, wid_player_select_player_via_mouse_down);

      wid_set_on_mouse_over_begin(w, wid_player_select_player_via_mouse_over_begin);
      wid_set_on_mouse_over_end(w, wid_player_select_player_via_mouse_over_end);

      wid_player_icon[ wid_player_index ] = w;
    }

    //
    // Key shortcut
    //
    {
      TRACE();
      auto *w = wid_new_square_button(g, wid_player_select_window, "Key");

      std::string s;
      s += static_cast< char >('0' + wid_player_index);
      s += ')';

      spoint const tl(3, y_at);
      spoint const br(6, y_at + button_height);
      wid_set_text_lhs(w, 1u);

      wid_set_mode(w, WID_MODE_NORMAL);
      wid_set_color(w, WID_COLOR_TEXT_FG, GRAY50);
      wid_set_style(w, button_style);
      wid_set_pos(w, tl, br);
      wid_set_text(w, s);

      wid_set_thing_context(g, v, w, existing_thing);
      wid_set_on_mouse_down(w, wid_player_select_player_via_mouse_down);

      wid_set_on_mouse_over_begin(w, wid_player_select_player_via_mouse_over_begin);
      wid_set_on_mouse_over_end(w, wid_player_select_player_via_mouse_over_end);

      wid_player_shortcut[ wid_player_index ] = w;
    }

    //
    // Sacrecant name
    //
    {
      //
      // Append sac_points to the name
      //
      std::string line;

      line = capitalize(tp_name_long(tp));

      auto sac_points = tp_sac_points_get(tp);
      if (sac_points > 0) {
        line = string_sprintf("%-41s +%d", line.c_str(), sac_points);
      } else if (sac_points < 0) {
        line = string_sprintf("%-41s %d", line.c_str(), sac_points);
      } else {
        line = string_sprintf("%-41s -", line.c_str());
      }

      auto *w = wid_new_bar_button(g, wid_player_select_window, "Sacrecant");

      spoint const tl(6, y_at);
      spoint const br(button_width, y_at + button_height);
      wid_set_text_lhs(w, 1u);
      wid_set_pos(w, tl, br);
      wid_set_text(w, line);

      wid_set_thing_context(g, v, w, existing_thing);
      wid_set_on_mouse_down(w, wid_player_select_player_via_mouse_down);

      wid_set_on_mouse_over_begin(w, wid_player_select_player_via_mouse_over_begin);
      wid_set_on_mouse_over_end(w, wid_player_select_player_via_mouse_over_end);

      wid_player[ wid_player_index ] = w;
    }

    y_at += button_step;
    wid_player_index++;
  }

  //
  // Sacrifices
  //
  {
    TRACE();
    auto *w = wid_new_bar_button(g, wid_player_select_window, "Sacrifices");

    spoint const tl(1, y_at);
    spoint const br(button_width, y_at + button_height);
    wid_set_text_lhs(w, 1u);
    wid_set_pos(w, tl, br);
    wid_set_text(w, UI_INFO_FMT_STR "Sacrifices");
    y_at++;
  }

  memset(wid_sacrifice_shortcut, 0, sizeof(wid_sacrifice_shortcut));
  memset(wid_sacrifice, 0, sizeof(wid_sacrifice));

  wid_sacrifice_index = 0;

  std::vector< Tpp > wid_sacrifice_tps;

  for (auto &tp : tp_vec) {
    if (! tp_is_sacrifice(tp)) {
      continue;
    }
    wid_sacrifice_tps.push_back(tp);
  }

  //
  // Sort by sac_points
  //
  std::ranges::sort(wid_sacrifice_tps, [](const Tpp &a, const Tpp &b) -> bool { return tp_sac_points_get(a) < tp_sac_points_get(b); });

  for (auto &tp : wid_sacrifice_tps) {
    //
    // Check for overflow
    //
    if (wid_sacrifice_index >= ARRAY_SIZE(wid_sacrifice)) {
      break;
    }

    //
    // Create a temporary thing on the level select map
    //
    auto   at             = bpoint(1, wid_sacrifice_index);
    Thingp existing_thing = nullptr;
    FOR_ALL_THINGS_AT(g, v, level_select, t, at)
    {
      if (t != nullptr) {
        existing_thing = t;
        break;
      }
    }

    if (existing_thing == nullptr) {
      existing_thing = thing_spawn(g, v, level_select, tp, at);
      if (existing_thing == nullptr) {
        continue;
      }
    }

    //
    // Key shortcut
    //
    {
      TRACE();
      auto *w = wid_new_square_button(g, wid_player_select_window, "Key");

      std::string s;
      s += static_cast< char >('a' + wid_sacrifice_index);
      s += ')';

      spoint const tl(3, y_at);
      spoint const br(6, y_at + button_height);
      wid_set_text_lhs(w, 1u);

      wid_set_mode(w, WID_MODE_NORMAL);
      wid_set_color(w, WID_COLOR_TEXT_FG, GRAY50);
      wid_set_style(w, button_style);
      wid_set_pos(w, tl, br);
      wid_set_text(w, s);

      wid_set_thing_context(g, v, w, existing_thing);
      wid_set_on_mouse_down(w, wid_player_select_sacrifice_via_mouse_down);

      wid_set_on_mouse_over_begin(w, wid_player_select_sacrifice_via_mouse_over_begin);
      wid_set_on_mouse_over_end(w, wid_player_select_sacrifice_via_mouse_over_end);

      wid_sacrifice_shortcut[ wid_sacrifice_index ] = w;
    }

    //
    // Sacrifice name
    //
    {
      //
      // Append sac_points to the name
      //
      std::string line;

      line = capitalize_first(tp_name_long(tp));

      auto sac_points = tp_sac_points_get(tp);
      if (sac_points > 0) {
        line = string_sprintf("%-41s +%d", line.c_str(), sac_points);
      } else if (sac_points < 0) {
        line = string_sprintf("%-41s %d", line.c_str(), sac_points);
      } else {
        line = string_sprintf("%-41s -", line.c_str());
      }

      TRACE();
      auto *w = wid_new_bar_button(g, wid_player_select_window, "Sacrifice");

      spoint const tl(6, y_at);
      spoint const br(button_width, y_at + button_height);
      wid_set_text_lhs(w, 1u);
      wid_set_pos(w, tl, br);
      wid_set_text(w, line);

      wid_set_thing_context(g, v, w, existing_thing);
      wid_set_on_mouse_down(w, wid_player_select_sacrifice_via_mouse_down);

      wid_set_on_mouse_over_begin(w, wid_player_select_sacrifice_via_mouse_over_begin);
      wid_set_on_mouse_over_end(w, wid_player_select_sacrifice_via_mouse_over_end);

      wid_sacrifice[ wid_sacrifice_index ] = w;
    }

    y_at += button_step;
    wid_sacrifice_index++;
  }

  //
  // Boosts
  //
  // No for now
  //
  if (compiler_unused) {
    {
      TRACE();
      auto *w = wid_new_bar_button(g, wid_player_select_window, "Boosts");

      spoint const tl(1, y_at);
      spoint const br(button_width, y_at + button_height);
      wid_set_text_lhs(w, 1u);
      wid_set_pos(w, tl, br);
      wid_set_text(w, UI_INFO_FMT_STR "Boosts");
      y_at++;
    }

    memset(wid_boost_shortcut, 0, sizeof(wid_boost_shortcut));
    memset(wid_boost, 0, sizeof(wid_boost));

    wid_boost_index = 0;

    std::vector< Tpp > wid_boost_tps;

    for (auto &tp : tp_vec) {
      if (! tp_is_boost(tp)) {
        continue;
      }
      wid_boost_tps.push_back(tp);
    }

    //
    // Sort by sac_points
    //
    std::ranges::sort(wid_boost_tps, [](const Tpp &a, const Tpp &b) -> bool { return tp_sac_points_get(a) > tp_sac_points_get(b); });

    for (auto &tp : wid_boost_tps) {
      //
      // Check for overflow
      //
      if (wid_boost_index >= ARRAY_SIZE(wid_boost)) {
        break;
      }

      //
      // Create a temporary thing on the level select map
      //
      auto   at             = bpoint(2, wid_boost_index);
      Thingp existing_thing = nullptr;
      FOR_ALL_THINGS_AT(g, v, level_select, t, at)
      {
        if (t != nullptr) {
          existing_thing = t;
          break;
        }
      }

      if (existing_thing == nullptr) {
        existing_thing = thing_spawn(g, v, level_select, tp, at);
        if (existing_thing == nullptr) {
          continue;
        }
      }

      //
      // Key shortcut
      //
      {
        TRACE();
        auto *w = wid_new_square_button(g, wid_player_select_window, "Key");

        std::string s;
        s += static_cast< char >('A' + wid_boost_index);
        s += ')';

        spoint const tl(3, y_at);
        spoint const br(6, y_at + button_height);
        wid_set_text_lhs(w, 1u);

        wid_set_mode(w, WID_MODE_NORMAL);
        wid_set_color(w, WID_COLOR_TEXT_FG, GRAY50);
        wid_set_style(w, button_style);
        wid_set_pos(w, tl, br);
        wid_set_text(w, s);

        wid_set_thing_context(g, v, w, existing_thing);
        wid_set_on_mouse_down(w, wid_player_select_boost_via_mouse_down);

        wid_set_on_mouse_over_begin(w, wid_player_select_boost_via_mouse_over_begin);
        wid_set_on_mouse_over_end(w, wid_player_select_boost_via_mouse_over_end);

        wid_boost_shortcut[ wid_boost_index ] = w;
      }

      //
      // Boost name
      //
      {
        //
        // Append sac_points to the name
        //
        std::string line;

        line = capitalize_first(tp_name_long(tp));

        auto sac_points = tp_sac_points_get(tp);
        if (sac_points > 0) {
          line = string_sprintf("%-41s +%d", line.c_str(), sac_points);
        } else if (sac_points < 0) {
          line = string_sprintf("%-41s %d", line.c_str(), sac_points);
        } else {
          line = string_sprintf("%-41s -", line.c_str());
        }

        TRACE();
        auto *w = wid_new_bar_button(g, wid_player_select_window, "Boost");

        spoint const tl(6, y_at);
        spoint const br(button_width, y_at + button_height);
        wid_set_text_lhs(w, 1u);
        wid_set_pos(w, tl, br);
        wid_set_text(w, line);

        wid_set_thing_context(g, v, w, existing_thing);
        wid_set_on_mouse_down(w, wid_player_select_boost_via_mouse_down);

        wid_set_on_mouse_over_begin(w, wid_player_select_boost_via_mouse_over_begin);
        wid_set_on_mouse_over_end(w, wid_player_select_boost_via_mouse_over_end);

        wid_boost[ wid_boost_index ] = w;
      }

      y_at += button_step;
      wid_boost_index++;
    }
  }

  y_at++;

  //
  // Total sac_points
  //
  {
    TRACE();
    auto *w = wid_new_bar_button(g, wid_player_select_window, "Boost");

    spoint const tl(1, y_at);
    spoint const br(button_width, y_at + button_height);
    wid_set_pos(w, tl, br);

    wid_total = w;
  }

  wid_update(g, wid_player_select_window);

  if (v->tick == 0u) {
    auto *w = wid_player[ 0 ];
    if (w != nullptr) {
      (void) wid_player_select_player_via_mouse_down(g, w, -1, -1, 0);
    }
  }

  game_state_change(g, STATE_PLAYER_SELECT_MENU, "player_select");
}
