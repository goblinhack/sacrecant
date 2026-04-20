//
// Copyright goblinhack@gmail.com
//

#include "my_ascii.hpp"
#include "my_callstack.hpp"
#include "my_game.hpp"
#include "my_level.hpp"
#include "my_sdl_proto.hpp"
#include "my_sound.hpp"
#include "my_thing_inlines.hpp"
#include "my_wids.hpp"

#include <format>

static WidPopup *wid_statistics_popup;

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
    sound_play(g, "keypress");
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
                if (g_opt_quick_start) {
                  DIE_CLEAN("Quick quit");
                }
                [[fallthrough]];
              case SDLK_ESCAPE :
                {
                  TRACE();
                  sound_play(g, "keypress");
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

[[nodiscard]] static auto wid_statistics_back(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();
  wid_statistics_destroy(g);
  return true;
}

void wid_statistics_show(Gamep g, Levelsp v, Levelp l, Thingp player)
{
  TRACE();

  auto *player_struct = thing_player_struct(g);
  if (player_struct == nullptr) {
    return;
  }

  const int defeated_width  = UI_INVENTORY_WIDTH;
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
    wid_set_text(wid_statistics_window, "Statistics");
    wid_set_text_top(wid_statistics_window, 1u);
  }

  auto *tp = thing_tp(player);

  if (wid_thing_info_icon(g, tp, wid_statistics_popup)) {
    wid_statistics_popup->log_empty_line(g);
  }

  if (wid_thing_info_name(g, v, l, player, tp, wid_statistics_popup)) {
    wid_statistics_popup->log_empty_line(g);
  }

  if (0) {
    if (wid_thing_info_detail(g, v, l, player, wid_statistics_popup)) {
      wid_statistics_popup->log_empty_line(g);
    }
  }

  if (wid_thing_info_score(g, player, tp, wid_statistics_popup)) {
    wid_statistics_popup->log_empty_line(g);
  }

  if (wid_thing_info_immunities(g, player, wid_statistics_popup, UI_INVENTORY_WIDTH)) {
    wid_statistics_popup->log_empty_line(g);
  }

  if (wid_thing_info_special_damage(g, player, wid_statistics_popup)) {
    wid_statistics_popup->log_empty_line(g);
  }

  wid_statistics_popup->log(g, "Defeateds:", TEXT_FORMAT_LHS);

  std::string line;

  //
  // Monster defeateds
  //
  for (auto x = 0; x < 5; x++) {
    for (auto i = 1; i < TP_ID_MAX; i++) {

      auto *monst_tp = tp_find(i);
      if (monst_tp == nullptr) {
        continue;
      }

      if (! tp_is_monst(monst_tp)) {
        continue;
      }

      auto s = std::format("{:<3}", player_struct->defeated[ i ]);

      s += " %%tp=";
      s += tp_name(monst_tp);
      s += "$";
      s += " ";
      s += std::format("{:<20}", tp_name(monst_tp));

      if (line.empty()) {
        line = s;
      } else {
        line = " - " + line + "  " + s;
        wid_statistics_popup->log(g, line, TEXT_FORMAT_LHS);
        line = "";
      }
    }
  }

  if (! line.empty()) {
    line = " - " + line;
    wid_statistics_popup->log(g, line, TEXT_FORMAT_LHS);
    line = "";
  }

  {
    TRACE();
    auto *w = wid_new_back_button(g, wid_statistics_window, "BACK");

    spoint const tl((defeated_width / 2) - 4, defeated_height - 4);
    spoint const br((defeated_width / 2) + 3, defeated_height - 2);
    wid_set_on_mouse_up(w, wid_statistics_back);
    wid_set_pos(w, tl, br);
  }

  wid_update(g, wid_statistics_window);

  game_state_change(g, STATE_INVENTORY_MENU, "defeated");
}
