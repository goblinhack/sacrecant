//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_game.hpp"
#include "my_main.hpp"
#include "my_ui.hpp"
#include "my_wid_keyboard.hpp"
#include "my_wids.hpp"

static Widp wid_choose_player_name;

static void selected(Gamep g, Widp w, const std::string &text)
{
  TRACE();

  if (! text.empty()) {
    game_player_name_set(g, text.c_str());
    con("Save config for player name");
    game_save_config(g);
  }

  wid_destroy(g, &wid_choose_player_name);
  wid_options_menu_select(g);
}

void wid_player_name_select(Gamep g)
{
  TRACE();

  con("Choose player name menu");

  wid_choose_player_name = wid_keyboard(g, game_player_name_get(g),
                                        "Enter a name for the player, max " + std::to_string(UI_MAX_PLAYER_NAME_LEN) + " characters",
                                        selected, selected, UI_MAX_PLAYER_NAME_LEN);
}
