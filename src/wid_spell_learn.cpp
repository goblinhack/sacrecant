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
static Widp      wid_spell_learn_window;
static WidPopup *wid_spell_learn_list;
static WidPopup *wid_spell_learn_learn_window;

static int  wid_spell_index = 0;
static Widp wid_spell[ TP_ID_MAX ];

static void wid_spell_learn_destroy(Gamep g)
{
  TRACE();

  memset(wid_spell, 0, sizeof(wid_spell));

  game_cand_spell_unset(g, nullptr);

  delete wid_spell_learn_list;
  wid_spell_learn_list = nullptr;

  delete wid_spell_learn_learn_window;
  wid_spell_learn_learn_window = nullptr;

  if (wid_spell_learn_window != nullptr) {
    wid_destroy(g, &wid_spell_learn_window);
  }
}

static void wid_spell_learn_all_done(Gamep g)
{
  TRACE();

  game_chosen_spell_set(g, game_cand_spell_get(g));

  wid_spell_learn_destroy(g);

  game_spell_clear(g);
}

[[nodiscard]] static auto wid_learn_mouse_down(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();

  wid_spell_learn_all_done(g);

  return true;
}

static void wid_spell_learn_check_if_done(Gamep g)
{
  TRACE();

  if (game_cand_spell_get(g).empty()) {
    return;
  }

  if (wid_spell_learn_learn_window == nullptr) {
    auto         m = TERM_WIDTH / 2;
    auto         n = TERM_HEIGHT - 5;
    spoint const outer_tl(m - (UI_WID_POPUP_WIDTH_NORMAL / 2), n - 3);
    spoint const outer_br(m + (UI_WID_POPUP_WIDTH_NORMAL / 2), n + 1);
    auto         width = outer_br.x - outer_tl.x;

    wid_spell_learn_learn_window = new WidPopup(g, "learn", outer_tl, outer_br, nullptr, "", false, false);
    auto *p                      = wid_spell_learn_learn_window->wid_text_area->wid_text_area;
    auto *w                      = wid_new_continue_button(g, p, "learn");

    spoint const tl(0, 0);
    spoint const br(width - 2, 2);
    wid_set_pos(w, tl, br);
    wid_set_text(w, "Learn?");
    wid_set_on_mouse_down(w, wid_learn_mouse_down);
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

  for (auto &n : wid_spell) {
    w = n;
    if (w != nullptr) {
      auto *t = wid_get_thing_context(g, v, w, 0);
      if (game_cand_spell_find(g, t)) {
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

  for (auto &n : wid_spell) {
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
      if (game_cand_spell_find(g, t)) {
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

  {
    auto total_sac_points = wid_player_total_sac_points(g);
    auto line             = string_sprintf("Sacrificial points (SPs) for spell casting       %d", total_sac_points);
    wid_set_text_lhs(wid_total, 1u);
    wid_set_text(wid_total, line);
    wid_update(g, wid_total);
  }
}

static void wid_spell_learn_spell_via_mouse_over_begin(Gamep g, Widp w, int /*relx*/, int /*rely*/, int /*wheelx*/, int /*wheely*/)
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

  game_spell_mouse_over_currently_set(g, t);

  level_cursor_describe_clear(g, v);

  if (level_cursor_describe_add(g, v, t)) {
    game_request_to_remake_ui_set(g);
  }
}

static void wid_spell_learn_spell_via_mouse_over_end(Gamep g, Widp w)
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

  game_spell_mouse_over_currently_set(g, nullptr);

  if (level_cursor_describe_remove(g, v, t)) {
    game_request_to_remake_ui_set(g);
  }
}

[[nodiscard]] static auto wid_spell_learn_spell_via_mouse_down(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
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

  if (game_cand_spell_find(g, t)) {
    game_cand_spell_unset(g, t);
  } else {
    game_cand_spell_set(g, t);
  }

  wid_spell_learn_check_if_done(g);
  wid_player_update_selections(g);
  game_request_to_remake_ui_set(g);

  (void) sound_play(g, "select");

  return true;
}

[[nodiscard]] static auto wid_spell_learn_key_down(Gamep g, Widp w, const struct SDL_Keysym *key) -> bool
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
                if (wid_spell_learn_learn_window != nullptr) {
                  //
                  // All done
                  //
                  wid_spell_learn_all_done(g);
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
                game_spell_mouse_over_currently_set(g, nullptr);
                w = wid_spell[ c - 'a' ];
                if (w != nullptr) {
                  (void) wid_spell_learn_spell_via_mouse_down(g, w, -1, -1, 0);
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
                game_spell_mouse_over_currently_set(g, nullptr);
                w = wid_spell[ c - 'A' + 26 ];
                if (w != nullptr) {
                  (void) wid_spell_learn_spell_via_mouse_down(g, w, -1, -1, 0);
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

void wid_spell_learn(Gamep g, Levelsp v, Levelp l, Thingp player)
{
  con("Player select menu: create");
  TRACE_INDENT();

  auto *level_select = game_level_get(g, v, LEVEL_ARR_IDX_LEVEL_SELECT);
  if (level_select == nullptr) {
    return;
  }

  if (wid_spell_learn_window != nullptr) {
    wid_spell_learn_destroy(g);
  }

  std::vector< Tpp > wid_spell_tps;

  for (auto &tp : tp_vec) {
    if (! tp_is_spell(tp)) {
      continue;
    }
    wid_spell_tps.push_back(tp);
  }

  const int player_select_width  = UI_INVENTORY_WIDTH + 2;
  const int player_select_height = TERM_HEIGHT;

  const auto button_width  = player_select_width - 2;
  const auto button_height = 0;
  const auto button_step   = 1;
  const auto button_style  = UI_WID_STYLE_SPARSE_NONE;

  auto y_at = 1;

  const int left_half  = player_select_width / 2;
  const int right_half = player_select_width - left_half;

  {
    TRACE();
    spoint const tl((TERM_WIDTH / 2) - left_half, 0);
    spoint const br((TERM_WIDTH / 2) + right_half - 1, TERM_HEIGHT - 1);

    wid_spell_learn_window = wid_new_window(g, "widget spell_learn");
    wid_set_pos(wid_spell_learn_window, tl, br);
    wid_set_style(wid_spell_learn_window, UI_WID_STYLE_BUTTON_OUTLINE);
    wid_set_on_key_down(wid_spell_learn_window, wid_spell_learn_key_down);
    wid_set_text_top(wid_spell_learn_window, 1u);
    wid_raise(g, wid_spell_learn_window);
  }

  {
    TRACE();
    auto        *w = wid_new_square_button(g, wid_spell_learn_window, "text");
    spoint const tl(0, y_at);
    spoint const br(player_select_width, y_at);
    wid_set_pos(w, tl, br);
    wid_set_text(w, UI_INFO_FMT_STR "Choose a spell to learn.");
    wid_set_style(w, UI_WID_STYLE_BUTTON_OUTLINE);
    wid_set_shape_none(w);
    wid_set_text_centerx(w, 1u);
    y_at += 2;
  }

  //
  // Spells
  //
  {
    TRACE();
    auto *w = wid_new_bar_button(g, wid_spell_learn_window, "Spells");

    spoint const tl(1, y_at);
    spoint const br(button_width, y_at + button_height);
    wid_set_text_lhs(w, 1u);
    wid_set_pos(w, tl, br);
    wid_set_text(w, UI_INFO_FMT_STR "Spell name                                Arcana  SP");
    y_at++;
  }

  {
    spoint inner_tl(1, 5);
    spoint inner_br(player_select_width - 2, player_select_height - 4);

    wid_spell_learn_list
        = new WidPopup(g, wid_spell_learn_window, "spell list", inner_tl, inner_br, nullptr, "", false, true, wid_spell_tps.size());
  }

  memset(wid_spell, 0, sizeof(wid_spell));

  wid_spell_index = 0;

  //
  // Sort by sac_points
  //
  std::ranges::sort(wid_spell_tps, [](const Tpp &a, const Tpp &b) -> bool { return tp_sac_points_get(a) < tp_sac_points_get(b); });

  for (auto &tp : wid_spell_tps) {
    //
    // Check for overflow
    //
    if (wid_spell_index >= ARRAY_SIZE(wid_spell)) {
      break;
    }

    //
    // Massive hack. Use hidden space on level select level for the things.
    //
    // This is one column to the right of player selection things
    //
    // Create a temporary thing on the level select map
    //
    auto   at             = bpoint(4 + wid_spell_index / MAP_WIDTH, wid_spell_index % MAP_HEIGHT);
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
    // Spell shortcut and name
    //
    if (wid_spell_index <= ('z' - 'a') * 2 + 1) {
      TRACE();

      auto spell_cost = tp_spell_cost_get(tp);

      std::string s;

      if (spell_cost <= thing_sac_points(g, v, l, player)) {
        s += "%%fg=gray90$";
      } else {
        s += "%%fg=gray50$";
      }

      if (wid_spell_index >= 26) {
        s += static_cast< char >('A' + wid_spell_index - 26);
      } else {
        s += static_cast< char >('a' + wid_spell_index);
      }
      s += ") ";

      s += capitalize_first(tp_name_long(tp));
      s = string_sprintf("%-53s", s.c_str());
      s += "%%fg=reset$";
      s += "%%fg=red$Fire%%fg=reset$    ";
      s += string_sprintf("%2d", spell_cost);

      auto w = wid_spell_learn_list->log(g, s, TEXT_FORMAT_LHS);

      wid_set_mode(w, WID_MODE_NORMAL);
      wid_set_style(w, button_style);

      wid_set_thing_context(g, v, w, existing_thing);
      wid_set_on_mouse_down(w, wid_spell_learn_spell_via_mouse_down);

      wid_set_on_mouse_over_begin(w, wid_spell_learn_spell_via_mouse_over_begin);
      wid_set_on_mouse_over_end(w, wid_spell_learn_spell_via_mouse_over_end);

      wid_spell[ wid_spell_index ] = w;
    }

    y_at += button_step;
    wid_spell_index++;
  }

  y_at = player_select_height - 2;

  //
  // Total sac_points
  //
  {
    TRACE();
    wid_total = wid_new_bar_button(g, wid_spell_learn_window, "available SP");

    spoint const tl(1, y_at);
    spoint const br(button_width, y_at + button_height);
    wid_set_pos(wid_total, tl, br);

    auto total_sac_points = thing_sac_points(g, v, l, player);
    auto line             = string_sprintf("SPs available to learning                          %2d", total_sac_points);

    wid_set_text_lhs(wid_total, 1u);
    wid_set_text(wid_total, line);
    wid_update(g, wid_total);
  }

  wid_update(g, wid_spell_learn_window);

  game_state_change(g, STATE_SPELL_LEARN_MENU, "spell_learn");
}
