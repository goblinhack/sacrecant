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
#include "my_sdl_proto.hpp"
#include "my_sound.hpp"
#include "my_spoint.hpp"
#include "my_sprintf.hpp"
#include "my_string.hpp"
#include "my_thing.hpp"
#include "my_thing_inlines.hpp"
#include "my_tile.hpp"
#include "my_tp.hpp"
#include "my_tp_inlines.hpp"
#include "my_types.hpp"
#include "my_ui.hpp"
#include "my_wid.hpp"
#include "my_wids.hpp"

#include <SDL_keyboard.h>
#include <SDL_keycode.h>
#include <cstdint>
#include <cstring>
#include <string>

static Widp wid_player_select_window;

static Widp wid_player_shortcut[ THING_INVENTORY_MAX ];
static Widp wid_player[ THING_INVENTORY_MAX ];
static Widp wid_player_icon[ THING_INVENTORY_MAX ];

static Widp wid_sacrifice_shortcut[ THING_INVENTORY_MAX ];
static Widp wid_sacrifice[ THING_INVENTORY_MAX ];

static void wid_player_select_destroy(Gamep g)
{
  TRACE();

  memset(wid_player_shortcut, 0, sizeof(wid_player_shortcut));
  memset(wid_player_icon, 0, sizeof(wid_player_icon));
  memset(wid_player, 0, sizeof(wid_player));

  memset(wid_sacrifice_shortcut, 0, sizeof(wid_sacrifice_shortcut));
  memset(wid_sacrifice, 0, sizeof(wid_sacrifice));

  game_mouse_over_player_set(g, nullptr);
  game_mouse_over_sacrifice_set(g, nullptr);

  game_mouse_down_player_set(g, nullptr);
  game_mouse_down_sacrifice_set(g, nullptr);

  if (wid_player_select_window != nullptr) {
    wid_destroy(g, &wid_player_select_window);
  }
}

static void wid_player_select_check_if_done(Gamep g)
{
  TRACE();

  if ((game_mouse_down_player_get(g) == nullptr) || (game_mouse_down_sacrifice_get(g) == nullptr)) {
    return;
  }

  auto *tp = thing_tp(game_mouse_down_player_get(g));
  game_chosen_player_set(g, tp);
  game_chosen_sacrifice_set(g, thing_tp(game_mouse_down_sacrifice_get(g)));

  wid_player_select_destroy(g);

  game_difficulty_set(g, tp_difficulty_get(tp));

  wid_progress_bar(g, "Generating...", 0.0F);
  wid_new_game(g);
  wid_progress_bar(g, "Generating...", 1.0F);
  wid_progress_bar_destroy(g);

  game_chosen_player_set(g, nullptr);
  game_chosen_sacrifice_set(g, nullptr);
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
      if (t == game_mouse_down_player_get(g)) {
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
      if (t == game_mouse_down_sacrifice_get(g)) {
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

  if (game_mouse_down_player_get(g) == t) {
    game_mouse_down_player_set(g, nullptr);
  } else {
    game_mouse_down_player_set(g, t);
  }

  (void) sound_play(g, "select");

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

  if (game_mouse_down_sacrifice_get(g) == t) {
    game_mouse_down_sacrifice_set(g, nullptr);
  } else {
    game_mouse_down_sacrifice_set(g, t);
  }

  wid_player_select_check_if_done(g);
  wid_player_update_selections(g);
  game_request_to_remake_ui_set(g);

  (void) sound_play(g, "select");

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
                w = wid_player[ 0 ];
                if (w != nullptr) {
                  (void) wid_player_select_player_via_mouse_down(g, w, -1, -1, 0);
                }
                w = wid_sacrifice[ 0 ];
                if (w != nullptr) {
                  (void) wid_player_select_sacrifice_via_mouse_down(g, w, -1, -1, 0);
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
                w = wid_sacrifice[ c - 'a' ];
                if (w != nullptr) {
                  (void) wid_player_select_sacrifice_via_mouse_down(g, w, -1, -1, 0);
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
    if (v->tick != 0u) {
      wid_set_text(w, UI_FMT_STR "Choose your next sacrifice");
    } else {
      wid_set_text(w, UI_FMT_STR "Choose a sacrecant and your first sacrifice");
    }
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
    wid_set_text(w, UI_INFO_FMT_STR "Your Sacrecant                                 Mana");
    y_at++;
  }

  memset(wid_player_shortcut, 0, sizeof(wid_player_shortcut));
  memset(wid_player_icon, 0, sizeof(wid_player_icon));
  memset(wid_player, 0, sizeof(wid_player));

  int y_index = 0;

  for (auto &tp : tp_vec) {
    if (! tp_is_player(tp)) {
      continue;
    }

    //
    // Create a temporary thing on the level select map
    //
    auto   at             = bpoint(0, y_index);
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

      wid_player_icon[ y_index ] = w;
    }

    //
    // Key shortcut
    //
    {
      TRACE();
      auto *w = wid_new_square_button(g, wid_player_select_window, "Key");

      std::string s;
      s += static_cast< char >('0' + y_index);
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

      wid_player_shortcut[ y_index ] = w;
    }

    //
    // Sacrecant name
    //
    {
      //
      // Append mana to the name
      //
      std::string line;

      line = capitalize(tp_name_long(tp));

      auto mana = tp_mana_get(tp);
      if (mana > 0) {
        line = string_sprintf("%-40s +%d", line.c_str(), mana);
      } else if (mana < 0) {
        line = string_sprintf("%-40s -%d", line.c_str(), mana);
      } else {
        line = string_sprintf("%-40s -", line.c_str());
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

      wid_player[ y_index ] = w;
    }

    y_at += button_step;
    y_index++;
  }

  y_at++;

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
    wid_set_text(w, UI_INFO_FMT_STR "Sacrifices                                     Mana");
    y_at++;
  }

  memset(wid_sacrifice_shortcut, 0, sizeof(wid_sacrifice_shortcut));
  memset(wid_sacrifice, 0, sizeof(wid_sacrifice));

  y_index = 0;

  for (auto &tp : tp_vec) {
    if (! tp_is_sacrifice(tp)) {
      continue;
    }

    //
    // Create a temporary thing on the level select map
    //
    auto   at             = bpoint(1, y_index);
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
      s += static_cast< char >('a' + y_index);
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

      wid_sacrifice_shortcut[ y_index ] = w;
    }

    //
    // Sacrifice name
    //
    {
      //
      // Append mana to the name
      //
      std::string line;

      line = capitalize_first(tp_name_long(tp));

      auto mana = tp_mana_get(tp);
      if (mana > 0) {
        line = string_sprintf("%-40s +%d", line.c_str(), mana);
      } else if (mana < 0) {
        line = string_sprintf("%-40s -%d", line.c_str(), mana);
      } else {
        line = string_sprintf("%-40s -", line.c_str());
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

      wid_sacrifice[ y_index ] = w;
    }

    y_at += button_step;
    y_index++;
  }

  wid_update(g, wid_player_select_window);

  game_state_change(g, STATE_PLAYER_SELECT_MENU, "player_select");
}
