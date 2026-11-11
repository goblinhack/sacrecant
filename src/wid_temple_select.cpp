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
#include "my_globals.hpp"
#include "my_level.hpp"
#include "my_level_inlines.hpp" // NOLINT
#include "my_main.hpp"
#include "my_random.hpp"
#include "my_sdl_proto.hpp"
#include "my_sound.hpp"
#include "my_spoint.hpp"
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
#include <format>
#include <string>
#include <vector>

static Widp      wid_total;
static Widp      wid_temple_sacrifice_select_window;
static WidPopup *wid_temple_sacrifice_select_continue_window;

static int  wid_sacrifice_index = 0;
static Widp wid_sacrifice_shortcut[ THING_INVENTORY_MAX ];
static Widp wid_sacrifice[ THING_INVENTORY_MAX ];

static int  wid_boost_index = 0;
static Widp wid_boost_shortcut[ THING_INVENTORY_MAX ];
static Widp wid_boost[ THING_INVENTORY_MAX ];

static void wid_temple_sacrifice_select_destroy(Gamep g)
{
  TRACE();

  memset(wid_sacrifice_shortcut, 0, sizeof(wid_sacrifice_shortcut));
  memset(wid_sacrifice, 0, sizeof(wid_sacrifice));

  memset(wid_boost_shortcut, 0, sizeof(wid_boost_shortcut));
  memset(wid_boost, 0, sizeof(wid_boost));

  game_cand_sacrifice_unset(g, nullptr);
  game_cand_boost_unset(g, nullptr);

  if (wid_temple_sacrifice_select_window != nullptr) {
    wid_destroy(g, &wid_temple_sacrifice_select_window);
  }

  delete wid_temple_sacrifice_select_continue_window;
  wid_temple_sacrifice_select_continue_window = nullptr;
}

static void wid_temple_sacrifice_select_all_done(Gamep g)
{
  TRACE();

  auto *v = levels_memory_alloc(g);
  if (v == nullptr) {
    return;
  }

  auto *l = game_level_get(g, v);
  if (l == nullptr) {
    return;
  }

  auto *player = thing_player(g);
  if (player == nullptr) {
    return;
  }

  auto *tp = game_cand_player_get(g);
  game_chosen_player_set(g, tp);
  game_chosen_sacrifice_set(g, game_cand_sacrifice_get(g));
  game_chosen_boost_set(g, game_cand_boost_get(g));

  //
  // Add the chosen sacrifice
  //
  for (auto *chosen_sacrifice : game_chosen_sacrifice_get(g)) {
    auto sacrifice = thing_hook_add(g, v, l, player, chosen_sacrifice);
    if (! sacrifice) {
      tp_err(chosen_sacrifice, "failed to add sacrifice");
    } else {
      (void) thing_sac_points_incr(g, v, l, player, thing_sac_points(g, v, l, sacrifice));
    }
  }

  //
  // Add the chosen boost
  //
  for (auto *chosen_boost : game_chosen_boost_get(g)) {
    auto boost = thing_hook_add(g, v, l, player, chosen_boost);
    if (! boost) {
      tp_err(chosen_boost, "failed to add boost");
    } else {
      (void) thing_sac_points_incr(g, v, l, player, thing_sac_points(g, v, l, boost));
    }
  }

  wid_temple_sacrifice_select_destroy(g);

  game_sacrifice_clear(g);
  game_boost_clear(g);

  game_state_reset(g, "close temple door");
}

[[nodiscard]] static auto wid_continue_mouse_down(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();

  (void) sound_play(g, "select");
  wid_temple_sacrifice_select_all_done(g);

  return true;
}

static void wid_temple_sacrifice_select_check_if_done(Gamep g)
{
  TRACE();

  if (game_cand_sacrifice_get(g).empty()) {
    return;
  }

  if (wid_temple_sacrifice_select_continue_window == nullptr) {
    auto         m = TERM_WIDTH / 2;
    auto         n = TERM_HEIGHT - 15;
    spoint const outer_tl(m - (UI_WID_POPUP_WIDTH_NORMAL), n - 3);
    spoint const outer_br(m + (UI_WID_POPUP_WIDTH_NORMAL), n + 1);
    auto         width = outer_br.x - outer_tl.x;

    wid_temple_sacrifice_select_continue_window = new WidPopup(g, "game continue", outer_tl, outer_br, nullptr, "", false, false);
    auto *p                                     = wid_temple_sacrifice_select_continue_window->wid_text_area->wid_text_area;
    auto *w                                     = wid_new_continue_button(g, p, "continue");

    spoint const tl(0, 0);
    spoint const br(width - 2, 2);
    wid_set_pos(w, tl, br);
    wid_set_text(w, "Leave the temple?");
    wid_set_on_mouse_down(w, wid_continue_mouse_down);
  }
}

static auto wid_player_total_sac_points(Gamep g) -> int
{
  TRACE();

  auto *v = levels_memory_alloc(g);
  if (v == nullptr) {
    return 0;
  }

  Widp w = nullptr;
  int  total_sac_points {};

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

static void wid_player_update_selections(Gamep g, Levelsp v, Levelp l, Thingp player)
{
  TRACE();

  Widp w = nullptr;

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

      bool previously_chosen_sarcrifice {};

      FOR_ALL_SACRIFICES(g, v, l, player, sacrifice)
      {
        if (thing_tp(sacrifice) == thing_tp(t)) {
          wid_set_mode(w, WID_MODE_OVER);
          wid_set_style(w, UI_WID_STYLE_BUTTON_BAR);
          wid_set_color(w, WID_COLOR_BG, GRAY10);
          wid_set_color(w, WID_COLOR_TEXT_FG, GRAY20);
          wid_set_mode(w, WID_MODE_NORMAL);
          wid_set_style(w, UI_WID_STYLE_BUTTON_BAR);
          wid_set_color(w, WID_COLOR_TEXT_FG, GRAY20);
          wid_set_color(w, WID_COLOR_BG, GRAY10);
          previously_chosen_sarcrifice = true;
        }
      }

      if (! previously_chosen_sarcrifice && game_cand_sacrifice_find(g, t)) {
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
    auto line             = std::format("SPs gained for spell casting       {}", total_sac_points);
    wid_set_text_lhs(wid_total, 1u);
    wid_set_text(wid_total, line);
    wid_update(g, wid_total);
  }
}

static void wid_temple_sacrifice_select_sacrifice_via_mouse_over_begin(Gamep g, Widp w, int /*relx*/, int /*rely*/, int /*wheelx*/,
                                                                       int /*wheely*/)
{
  TRACE();

  if (wid_temple_sacrifice_select_window == nullptr) {
    return;
  }

  auto *v = levels_memory_alloc(g);
  if (v == nullptr) {
    return;
  }

  auto *t = wid_get_thing_context(g, v, w, 0);
  if (t == nullptr) {
    return;
  }

  game_sacrifice_mouse_over_currently_set(g, t);
  game_request_to_remake_ui_set(g);
}

static void wid_temple_sacrifice_select_sacrifice_via_mouse_over_end(Gamep g, Widp w)
{
  TRACE();

  if (wid_temple_sacrifice_select_window == nullptr) {
    return;
  }

  auto *v = levels_memory_alloc(g);
  if (v == nullptr) {
    return;
  }

  auto *t = wid_get_thing_context(g, v, w, 0);
  if (t == nullptr) {
    return;
  }

  game_sacrifice_mouse_over_currently_set(g, nullptr);
  game_request_to_remake_ui_set(g);
}

[[nodiscard]] static auto wid_temple_sacrifice_select_sacrifice_via_mouse_down(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();

  if (wid_temple_sacrifice_select_window == nullptr) {
    return false;
  }

  auto *v = levels_memory_alloc(g);
  if (v == nullptr) {
    return false;
  }

  auto *l = game_level_get(g, v);
  if (l == nullptr) {
    return false;
  }

  auto *t = wid_get_thing_context(g, v, w, 0);
  if (t == nullptr) {
    return false;
  }

  auto *player = thing_player(g);
  if (player == nullptr) {
    return false;
  }

  if (game_cand_sacrifice_find(g, t)) {
    game_cand_sacrifice_unset(g, t);
  } else {
    game_cand_sacrifice_set(g, t);
  }

  game_sacrifice_mouse_over_currently_set(g, nullptr);
  wid_temple_sacrifice_select_check_if_done(g);
  wid_player_update_selections(g, v, l, player);
  game_request_to_remake_ui_set(g);

  if (game_state(g) == STATE_TEMPLE_SELECT_MENU) {
    (void) sound_play(g, "select");
  }

  return true;
}

static void wid_temple_sacrifice_select_boost_via_mouse_over_begin(Gamep g, Widp w, int /*relx*/, int /*rely*/, int /*wheelx*/, int /*wheely*/)
{
  TRACE();

  if (wid_temple_sacrifice_select_window == nullptr) {
    return;
  }

  auto *v = levels_memory_alloc(g);
  if (v == nullptr) {
    return;
  }

  auto *t = wid_get_thing_context(g, v, w, 0);
  if (t == nullptr) {
    return;
  }

  game_boost_mouse_over_currently_set(g, t);
  game_request_to_remake_ui_set(g);
}

static void wid_temple_sacrifice_select_boost_via_mouse_over_end(Gamep g, Widp w)
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

  game_boost_mouse_over_currently_set(g, nullptr);
  game_request_to_remake_ui_set(g);
}

[[nodiscard]] static auto wid_temple_sacrifice_select_boost_via_mouse_down(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();

  if (wid_temple_sacrifice_select_window == nullptr) {
    return false;
  }

  auto *v = levels_memory_alloc(g);
  if (v == nullptr) {
    return false;
  }

  auto *l = game_level_get(g, v);
  if (l == nullptr) {
    return false;
  }

  auto *t = wid_get_thing_context(g, v, w, 0);
  if (t == nullptr) {
    return false;
  }

  auto *player = thing_player(g);
  if (player == nullptr) {
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
      topcon("Not enough SPs to buy this boost.\n");
      return true;
    }

    game_cand_boost_set(g, t);
  }

  wid_temple_sacrifice_select_check_if_done(g);
  wid_player_update_selections(g, v, l, player);
  game_request_to_remake_ui_set(g);

  if (game_state(g) == STATE_TEMPLE_SELECT_MENU) {
    (void) sound_play(g, "select");
  }

  return true;
}

[[nodiscard]] static auto wid_temple_sacrifice_select_key_down(Gamep g, Widp w, const struct SDL_Keysym *key) -> bool
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
                (void) sound_play(g, "keypress");
                w = wid_sacrifice[ c - 'a' ];
                if (w != nullptr) {
                  (void) wid_temple_sacrifice_select_sacrifice_via_mouse_down(g, w, -1, -1, 0);
                  return true;
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
                (void) sound_play(g, "keypress");
                game_boost_mouse_over_currently_set(g, nullptr);
                w = wid_boost[ c - 'A' ];
                if (w != nullptr) {
                  (void) wid_temple_sacrifice_select_boost_via_mouse_down(g, w, -1, -1, 0);
                  return true;
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

void wid_temple_sacrifice_select(Gamep g, Levelsp v, Levelp l, Thingp player)
{
  con("Temple select menu: create");
  TRACE_INDENT();

  auto *level_select = game_level_get(g, v, LEVEL_ARR_IDX_LEVEL_SELECT);
  if (level_select == nullptr) {
    return;
  }

  if (wid_temple_sacrifice_select_window != nullptr) {
    wid_temple_sacrifice_select_destroy(g);
  }

  const int menu_width  = UI_INVENTORY_WIDTH;
  const int menu_height = UI_INVENTORY_HEIGHT;

  const auto button_width  = menu_width - 2;
  const auto button_height = 0;
  const auto button_step   = 1;
  const auto button_style  = UI_WID_STYLE_SPARSE_NONE;

  auto y_at = 2;

  const int left_half  = menu_width / 2;
  const int right_half = menu_width - left_half;
  const int top_half   = menu_height / 2;
  const int bot_half   = menu_height - top_half;

  {
    TRACE();
    spoint const tl((TERM_WIDTH / 2) - left_half, (TERM_HEIGHT / 2) - top_half);
    spoint const br((TERM_WIDTH / 2) + right_half - 1, (TERM_HEIGHT / 2) + bot_half - 10);

    wid_temple_sacrifice_select_window = wid_new_window(g, "widget temple_select");
    wid_set_pos(wid_temple_sacrifice_select_window, tl, br);
    wid_set_style(wid_temple_sacrifice_select_window, UI_WID_STYLE_BUTTON_OUTLINE);
    wid_set_on_key_down(wid_temple_sacrifice_select_window, wid_temple_sacrifice_select_key_down);
    wid_set_text_top(wid_temple_sacrifice_select_window, 1u);
    wid_raise(g, wid_temple_sacrifice_select_window);
  }

  {
    TRACE();
    auto        *w = wid_new_square_button(g, wid_temple_sacrifice_select_window, "text");
    spoint const tl(0, y_at);
    spoint const br(menu_width, y_at);
    wid_set_pos(w, tl, br);
    wid_set_text(w, UI_INFO_FMT_STR "Choose your next sacrifice, mortal!");
    wid_set_style(w, UI_WID_STYLE_BUTTON_OUTLINE);
    wid_set_shape_none(w);
    wid_set_text_centerx(w, 1u);
    y_at += 1;
  }

  //
  // Title
  //
  {
    TRACE();
    auto *w = wid_new_bar_button(g, wid_temple_sacrifice_select_window, "Sacrecant");

    spoint const tl(1, y_at);
    spoint const br(button_width, y_at + button_height);
    wid_set_text_lhs(w, 1u);
    wid_set_pos(w, tl, br);
    wid_set_text(w, UI_INFO_FMT_STR "Sacrifices                                 SPs ");
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

  bool added_a_sacrifice = {};

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

    bool previously_chosen_sarcrifice {};

    FOR_ALL_SACRIFICES(g, v, l, player, sacrifice)
    {
      if (thing_tp(sacrifice) == thing_tp(existing_thing)) {
        previously_chosen_sarcrifice = true;
      }
    }

    if (! previously_chosen_sarcrifice) {
      added_a_sacrifice = true;
    }

    //
    // Key shortcut
    //
    {
      TRACE();
      auto *w = wid_new_square_button(g, wid_temple_sacrifice_select_window, "Key");

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
      if (! previously_chosen_sarcrifice) {
        wid_set_on_mouse_down(w, wid_temple_sacrifice_select_sacrifice_via_mouse_down);
      }

      wid_set_on_mouse_over_begin(w, wid_temple_sacrifice_select_sacrifice_via_mouse_over_begin);
      wid_set_on_mouse_over_end(w, wid_temple_sacrifice_select_sacrifice_via_mouse_over_end);

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

      line = string_capitalize_first(tp_name_long(tp));

      auto sac_points = tp_sac_points_get(tp);
      if (sac_points > 0) {
        line = std::format("{:<37} +{}", line, sac_points);
      } else if (sac_points < 0) {
        line = std::format("{:<37} {}", line, sac_points);
      } else {
        line = std::format("{:<37} -", line);
      }

      TRACE();
      auto *w = wid_new_bar_button(g, wid_temple_sacrifice_select_window, "Sacrifice");

      spoint const tl(6, y_at);
      spoint const br(button_width, y_at + button_height);
      wid_set_text_lhs(w, 1u);
      wid_set_pos(w, tl, br);
      wid_set_text(w, line);

      wid_set_thing_context(g, v, w, existing_thing);
      if (previously_chosen_sarcrifice) {
        wid_set_mode(w, WID_MODE_OVER);
        wid_set_style(w, UI_WID_STYLE_BUTTON_BAR);
        wid_set_color(w, WID_COLOR_BG, GRAY10);
        wid_set_color(w, WID_COLOR_TEXT_FG, GRAY20);
        wid_set_mode(w, WID_MODE_NORMAL);
        wid_set_style(w, UI_WID_STYLE_BUTTON_BAR);
        wid_set_color(w, WID_COLOR_TEXT_FG, GRAY20);
        wid_set_color(w, WID_COLOR_BG, GRAY10);
      } else {
        wid_set_on_mouse_down(w, wid_temple_sacrifice_select_sacrifice_via_mouse_down);
      }

      wid_set_on_mouse_over_begin(w, wid_temple_sacrifice_select_sacrifice_via_mouse_over_begin);
      wid_set_on_mouse_over_end(w, wid_temple_sacrifice_select_sacrifice_via_mouse_over_end);

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
      auto *w = wid_new_bar_button(g, wid_temple_sacrifice_select_window, "Boosts");

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
        auto *w = wid_new_square_button(g, wid_temple_sacrifice_select_window, "Key");

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
        wid_set_on_mouse_down(w, wid_temple_sacrifice_select_boost_via_mouse_down);

        wid_set_on_mouse_over_begin(w, wid_temple_sacrifice_select_boost_via_mouse_over_begin);
        wid_set_on_mouse_over_end(w, wid_temple_sacrifice_select_boost_via_mouse_over_end);

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

        line = string_capitalize_first(tp_name_long(tp));

        auto sac_points = tp_sac_points_get(tp);
        if (sac_points > 0) {
          line = std::format("{:<37} +{}", line, sac_points);
        } else if (sac_points < 0) {
          line = std::format("{:<37} {}", line, sac_points);
        } else {
          line = std::format("{:<37} -", line);
        }

        TRACE();
        auto *w = wid_new_bar_button(g, wid_temple_sacrifice_select_window, "Boost");

        spoint const tl(6, y_at);
        spoint const br(button_width, y_at + button_height);
        wid_set_text_lhs(w, 1u);
        wid_set_pos(w, tl, br);
        wid_set_text(w, line);

        wid_set_thing_context(g, v, w, existing_thing);
        wid_set_on_mouse_down(w, wid_temple_sacrifice_select_boost_via_mouse_down);

        wid_set_on_mouse_over_begin(w, wid_temple_sacrifice_select_boost_via_mouse_over_begin);
        wid_set_on_mouse_over_end(w, wid_temple_sacrifice_select_boost_via_mouse_over_end);

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
    auto *w = wid_new_bar_button(g, wid_temple_sacrifice_select_window, "total");

    spoint const tl(1, y_at);
    spoint const br(button_width, y_at + button_height);
    wid_set_pos(w, tl, br);

    wid_total = w;
  }

  wid_update(g, wid_temple_sacrifice_select_window);

  game_state_change(g, STATE_TEMPLE_SELECT_MENU, "temple_select");

  if (! added_a_sacrifice) {
    topcon(UI_IMPORTANT_FMT_STR "The temple is closed to you mortal. You can sacrifice no more!" UI_RESET_FMT);
    wid_temple_sacrifice_select_destroy(g);
    game_state_reset(g, "close temple door");
  }
}
