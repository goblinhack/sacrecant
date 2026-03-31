//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_game.hpp"
#include "my_globals.hpp"
#include "my_main.hpp"
#include "my_ui.hpp"
#include "my_wid_keyboard.hpp"
#include "my_wids.hpp"

static Widp wid_choose_seed;

static void selected(Gamep g, Widp w, const std::string &text)
{
  TRACE();

  con("Manually set seed to '%s'", text.c_str());

  //
  // If you set a seed manually, forget what the cli said
  //
  g_opt_seed_name = "";

  if (text.empty()) {
    game_seed_clear(g);
  }

  game_seed_set(g, text.c_str());

  con("Save config for seed");
  game_save_config(g);

  wid_destroy(g, &wid_choose_seed);
  wid_options_menu_select(g);
}

void wid_seed_select(Gamep g)
{
  TRACE();

  con("Choose seed menu");

  wid_choose_seed
      = wid_keyboard(g, "", "Enter a name or number for the dungeon seed, max " + std::to_string(UI_MAX_SEED_NAME_LEN) + " characters",
                     selected, selected, UI_MAX_SEED_NAME_LEN);
}
