//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_cpp_template.hpp"
#include "my_game.hpp"
#include "my_main.hpp"
#include "my_music.hpp"
#include "my_sdl_proto.hpp"
#include "my_sound.hpp"
#include "my_string.hpp"
#include "my_ui.hpp"
#include "my_wids.hpp"

static WidPopup *wid_dead_window;

void wid_dead_fini(Gamep g)
{
  TRACE();

  delete wid_dead_window;
  wid_dead_window = nullptr;
}

static void wid_dead_selected(Gamep g)
{
  TRACE();
  wid_dead_fini(g);

  TRACE();
  game_cleanup(g);

  TRACE();
  game_state_reset(g, "finished game");
}

[[nodiscard]] static auto wid_dead_key_down(Gamep g, Widp w, const struct SDL_Keysym *key) -> bool
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
              case SDLK_ESCAPE :
                {
                  TRACE();
                  sound_play(g, "keypress");
                  wid_dead_selected(g);
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

[[nodiscard]] static auto wid_dead_mouse_up(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();
  wid_dead_selected(g);

  return true;
}

void wid_dead_select(Gamep g, const std::string &reason)
{
  TRACE();

  sound_fade_out(g);
  music_play(g, "dead");

  log("open dead select: %s", reason.c_str());

  game_state_change(g, STATE_DEAD_MENU, "player is dead");

  auto         h = TERM_HEIGHT / 2;
  spoint const tl(0, h + 1);
  spoint const br(UI_LEFTBAR_WIDTH - 1, TERM_HEIGHT - 1);
  auto         width = br.x - tl.x - 1;

  wid_dead_window = new WidPopup(g, "game dead", tl, br, nullptr, "", false, false);

  wid_set_on_key_down(wid_dead_window->wid_popup_container, wid_dead_key_down);
  wid_raise(g, wid_dead_window->wid_popup_container);

  auto grave_text_width = 18;

  wid_set_do_not_lower(wid_dead_window->wid_popup_container);

  wid_dead_window->log_empty_line(g);
  wid_dead_window->log_empty_line(g);
  wid_dead_window->log_empty_line(g);
  wid_dead_window->log_empty_line(g);
  wid_dead_window->log(g, " .------------.   ");
  wid_dead_window->log(g, "/     Rest     \\  ");
  wid_dead_window->log(g, " /       In       \\ ");
  wid_dead_window->log(g, "/      Pieces      \\");
  wid_dead_window->log(g, "|                  |");
  wid_dead_window->log(g, "|                  |");
  wid_dead_window->log(g, "|                  |");

  //
  // Center the player name
  //
  {
    std::string const text    = capitalize(game_player_name_get(g));
    int const         pad     = grave_text_width - text.size();
    int               lhs_pad = pad / 2;
    int               rhs_pad = pad - lhs_pad;

    std::string grave_name = "|";
    while ((lhs_pad--) != 0) {
      grave_name += " ";
    }
    grave_name += text;
    while ((rhs_pad--) != 0) {
      grave_name += " ";
    }
    grave_name += "|";

    wid_dead_window->log(g, grave_name);
  }

  wid_dead_window->log(g, "|                  |");

  const std::initializer_list< std::string > epitaphs = {
      "A brave end.",
      "A noble end.",
      "A pointless end.",
      "As lifeless as a Norweigian blue.",
      "At rest, sort of.",
      "Bade farewell to life.",
      "Bit the dust.",
      "Bought the farm.",
      "Came to a sticky end.",
      "Came. Tried. Failed.",
      "Croaked it.",
      "Crossed the great divide.",
      "Dead, what? When?",
      "Died horribly.",
      "Fell off the perch.",
      "Fought bravely...",
      "Gave up the ghost.",
      "Gave up too soon",
      "Gone and dearly missed",
      "Goodbye cruel world.",
      "Had your final summons.",
      "Has gone to a better place.",
      "Had such high hopes",
      "Is a better place.",
      "Kicked the bucket.",
      "Left this mortal coil.",
      "Life was hard. Taking it easy now.",
      "Me, dead? never",
      "Not sure how that happened.",
      "Off to the happy hunting ground.",
      "Paid the piper.",
      "Passed on.",
      "Profoundly dead.",
      "Pushing up the daisies now.",
      "Sadly departed.",
      "Six feet under.",
      "Snuffed it.",
      "Suddenly expired.",
      "Well, could have gone a lot worse...",
      "The Gods are disappointed in you...",
      "Their number was up.",
      "Try 3D Monster Maze instead.",
      "Was a noble pursuit.",
      "Was the G.O.A.T.",
      "Welcomed to the afterlife!",
      "Well, that could have been worse.",
      "You tried.",
  };

  std::vector< std::string > epitaphs_arr(epitaphs);
  auto                       epitaph = rand_one_of(epitaphs_arr);

  //
  // Center the death mockery
  //
  {
    std::vector< std::string > const d = split(capitalize(epitaph), grave_text_width);

    for (const auto &text : d) {
      int const pad     = grave_text_width - text.size();
      int       lhs_pad = pad / 2;
      int       rhs_pad = pad - lhs_pad;

      std::string death_reason = " |";
      while ((lhs_pad--) != 0) {
        death_reason += " ";
      }
      death_reason += text;
      while ((rhs_pad--) != 0) {
        death_reason += " ";
      }
      death_reason += "|";

      wid_dead_window->log(g, death_reason);
    }
  }

  wid_dead_window->log(g, "|                  |");
  wid_dead_window->log(g, "|                  |");

  //
  // Center the death reason
  //
  {
    std::vector< std::string > const d = split(capitalize(reason), grave_text_width);

    for (const auto &text : d) {
      int const pad     = grave_text_width - text.size();
      int       lhs_pad = pad / 2;
      int       rhs_pad = pad - lhs_pad;

      std::string death_reason = " |";
      while ((lhs_pad--) != 0) {
        death_reason += " ";
      }
      death_reason += text;
      while ((rhs_pad--) != 0) {
        death_reason += " ";
      }
      death_reason += "|";

      wid_dead_window->log(g, death_reason);
    }
  }

  wid_dead_window->log(g, "|                  |");
  wid_dead_window->log(g, "|     " UI_YELLOW_FMT_STR "*" UI_RESET_FMT "            |");
  wid_dead_window->log(g, "|     " UI_GREEN_FMT_STR "|" UI_RESET_FMT "        " UI_YELLOW_FMT_STR "*" UI_RESET_FMT "   |");
  wid_dead_window->log(g, UI_GREEN_FMT_STR ".,." UI_RESET_FMT "|" UI_GREEN_FMT_STR "'" UI_RESET_FMT "__" UI_GREEN_FMT_STR
                                           "'\"|" UI_RESET_FMT "___" UI_GREEN_FMT_STR "'\"'" UI_RESET_FMT "_" UI_GREEN_FMT_STR
                                           "\"|'" UI_RESET_FMT "__|" UI_GREEN_FMT_STR ",,." UI_RESET_FMT);
  wid_dead_window->log_empty_line(g);

  auto *p = wid_dead_window->wid_text_area->wid_text_area;
  auto *w = wid_new_menu_button(g, p, "dead");

  {
    spoint const tl2(9, h - 7);
    spoint const br2(width - 11, h - 5);

    wid_set_pos(w, tl2, br2);
    wid_set_on_mouse_up(w, wid_dead_mouse_up);
    wid_set_text(w, "Continue");
  }

  wid_update(g, wid_dead_window->wid_text_area->wid_text_area);
}
