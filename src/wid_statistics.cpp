//
// Copyright goblinhack@gmail.com
//

#include "my_ascii.hpp"
#include "my_callstack.hpp"
#include "my_game.hpp"
#include "my_game_defs.hpp"
#include "my_globals.hpp"
#include "my_main.hpp"
#include "my_sdl_proto.hpp"
#include "my_sound.hpp"
#include "my_spoint.hpp"
#include "my_sprintf.hpp"
#include "my_thing.hpp"
#include "my_thing_inlines.hpp"
#include "my_tp.hpp"
#include "my_tp_inlines.hpp"
#include "my_types.hpp"
#include "my_ui.hpp"
#include "my_wid.hpp"
#include "my_wid_popup.hpp"
#include "my_wid_text_box.hpp"
#include "my_wids.hpp"

#include <SDL_keyboard.h>
#include <SDL_keycode.h>
#include <cstdint>

static WidPopup *wid_statistics_popup;

static const auto COLUMN_WIDTH = 20;

static void wid_statistics_destroy(Gamep g)
{
  TRACE();

  if (wid_statistics_popup != nullptr) {
    delete wid_statistics_popup;
    wid_statistics_popup = nullptr;

    game_state_change(g, STATE_PLAYING, "close defeated");
  }
}

[[nodiscard]] static auto wid_statistics_key_down(Gamep g, Widp w, const struct SDL_Keysym *key) -> bool
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
              case 'q' :
              case 'Q' :
                (void) sound_play(g, "keypress");
                if (g_opt_quick_start) {
                  DIE_CLEAN("Quick quit");
                }
                [[fallthrough]];
              case SDLK_ESCAPE :
                {
                  TRACE();
                  (void) sound_play(g, "keypress");
                  wid_statistics_destroy(g);
                  return true;
                }
            }
          }
      }
  }

  //
  // Allow fallback to the player game input, so they player can do things like change the zoom
  //
  return false;
}

[[nodiscard]] static auto wid_statistics_close(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();
  wid_statistics_destroy(g);

  TRACE();
  game_cleanup(g);

  TRACE();
  game_state_change(g, STATE_MAIN_MENU, "game over");

  TRACE();
  game_state_reset(g, "finished game");

  if (g_opt_quick_start || g_opt_player_select_menu || g_opt_level_select_menu) {
    DIE_CLEAN("Quick quit");
  }

  return true;
}

static void wid_statistics_show_defeated(Gamep g, Levelp l, Thingp player)
{
  TRACE();

  auto *player_struct = thing_player_struct(g);
  if (player_struct == nullptr) {
    return;
  }

  std::string line;
  bool        printed_something = {};

  wid_statistics_popup->log(g, "Defeated:", TEXT_FORMAT_LHS);

  //
  // Monster defeated
  //
  int column_count {};
  for (auto i = 1; i < TP_ID_MAX; i++) {

    auto *it = tp_find(i);
    if (it == nullptr) {
      continue;
    }

    if (! tp_is_monst(it) && ! tp_is_mob(it) && ! tp_is_minion(it)) {
      continue;
    }

    if (player_struct->defeated[ i ] == 0U) {
      continue;
    }

    printed_something = true;

    std::string s;
    s += " %%tp=";
    s += tp_name(it);
    s += "$ ";
    auto n   = tp_name_short(it);
    auto cnt = string_sprintf("%dx ", player_struct->defeated[ i ]);
    s += string_sprintf("%-5s%*s", cnt.c_str(), UI_MAX_THING_NAME_SHORT, n.c_str());
    line += string_sprintf("%*s", COLUMN_WIDTH, s.c_str());

    if (++column_count >= 4) {
      column_count = 0;
      wid_statistics_popup->log(g, line, TEXT_FORMAT_LHS);
      line = "";
    } else {
      line += " %%fg=red$|%%fg=reset$ ";
    }
  }

  if (! line.empty()) {
    wid_statistics_popup->log(g, line, TEXT_FORMAT_LHS);
  }

  if (! printed_something) {
    wid_statistics_popup->log_empty_line(g);
    wid_statistics_popup->log(g, "You did not defeat anything!");
  }
  wid_statistics_popup->log_empty_line(g);
}

static void wid_statistics_show_items(Gamep g, Levelsp v, Levelp l, Thingp player)
{
  TRACE();

  auto *player_struct = thing_player_struct(g);
  if (player_struct == nullptr) {
    return;
  }

  std::string line;
  bool        printed_something = {};

  wid_statistics_popup->log(g, "Carrying:", TEXT_FORMAT_LHS);
  int column_count {};

  FOR_ALL_INVENTORY_SLOTS(g, v, l, player, slot, item)
  {
    auto *it = (item != nullptr) ? thing_tp(item) : nullptr;
    if (it == nullptr) {
      continue;
    }

    printed_something = true;

    std::string s;
    s += " %%tp=";
    s += tp_name(it);
    s += "$ ";

    auto n   = tp_name_short(it);
    auto cnt = string_sprintf("%dx ", slot->count);
    s += string_sprintf("%-5s%*s", cnt.c_str(), UI_MAX_THING_NAME_SHORT, n.c_str());
    line += string_sprintf("%*s", COLUMN_WIDTH, s.c_str());

    if (++column_count >= 4) {
      column_count = 0;
      wid_statistics_popup->log(g, line, TEXT_FORMAT_LHS);
      line = "";
    } else {
      line += " %%fg=red$|%%fg=reset$ ";
    }
  }

  if (! line.empty()) {
    wid_statistics_popup->log(g, line, TEXT_FORMAT_LHS);
  }

  if (! printed_something) {
    wid_statistics_popup->log_empty_line(g);
    wid_statistics_popup->log(g, "You were not carrying anything");
  }
  wid_statistics_popup->log_empty_line(g);
}

void wid_statistics_show(Gamep g, Levelsp v, Levelp l, Thingp player)
{
  TRACE();

  auto *player_struct = thing_player_struct(g);
  if (player_struct == nullptr) {
    return;
  }

  const int defeated_width  = (COLUMN_WIDTH * 5) + 4;
  const int defeated_height = UI_INVENTORY_HEIGHT;

  const int left_half  = defeated_width / 2;
  const int right_half = defeated_width - left_half;
  const int top_half   = defeated_height / 2;
  const int bot_half   = defeated_height - top_half;

  TRACE();

  Widp wid_statistics_window = nullptr;

  {
    spoint const tl((TERM_WIDTH / 2) - left_half, (TERM_HEIGHT / 2) - top_half);
    spoint const br((TERM_WIDTH / 2) + right_half - 1, (TERM_HEIGHT / 2) + bot_half - 1);

    wid_statistics_popup = new WidPopup(g, "statistics", tl, br, nullptr, "", false, true, TP_ID_MAX + 10);

    wid_statistics_window = wid_statistics_popup->wid_popup_container;

    wid_set_on_key_down(wid_statistics_window, wid_statistics_key_down);

    if (thing_is_game_over(player)) {
      wid_set_text(wid_statistics_window, "Ye status of final victory");
    } else {
      wid_set_text(wid_statistics_window, "Ye status of final demise");
    }

    wid_set_text_top(wid_statistics_window, 1u);
  }

  auto *tp = thing_tp(player);

  if (wid_tp_info_icon(g, tp, wid_statistics_popup)) {
    wid_statistics_popup->log_empty_line(g);
  }

  if (wid_thing_info_name(g, v, l, player, wid_statistics_popup)) {
    wid_statistics_popup->log_empty_line(g);
  }

  if (wid_thing_info_score(g, v, l, player, wid_statistics_popup)) {
    wid_statistics_popup->log_empty_line(g);
  }

  if (wid_thing_info_immunity(g, v, l, player, wid_statistics_popup, UI_INVENTORY_WIDTH)) {
    wid_statistics_popup->log_empty_line(g);
  }

  if (wid_thing_info_resistance(g, v, l, player, wid_statistics_popup, UI_INVENTORY_WIDTH)) {
    wid_statistics_popup->log_empty_line(g);
  }

  wid_statistics_show_defeated(g, l, player);

  wid_statistics_show_items(g, v, l, player);

  {
    TRACE();
    auto *w = wid_new_close_button(g, wid_statistics_window, "CLOSE");

    spoint const tl((defeated_width / 2) - 3, defeated_height - 4);
    spoint const br((defeated_width / 2) + 3, defeated_height - 2);
    wid_set_on_mouse_down(w, wid_statistics_close);
    wid_set_pos(w, tl, br);
  }

  wid_update(g, wid_statistics_window);

  game_state_change(g, STATE_INVENTORY_MENU, "defeated");
}
