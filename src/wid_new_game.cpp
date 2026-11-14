//
// Copyright goblinhack@gmail.com
//

#include "config.hpp"
#include "my_callstack.hpp"
#include "my_game.hpp"
#include "my_game_defs.hpp"
#include "my_gl.hpp" // NOLINT
#include "my_globals.hpp"
#include "my_level.hpp"
#include "my_main.hpp" // NOLINT
#include "my_types.hpp"
#include "my_wids.hpp"

void wid_new_game(Gamep g)
{
  con("New game: select");
  TRACE_INDENT();

  game_state_reset(g, "new game");
  game_create_levels(g);
  game_map_zoom_in(g);
  game_start_playing(g);

  auto *v = game_levels_get(g);

  if (g_opt_level_select_menu) {
    game_state_change(g, STATE_LEVEL_SELECT_MENU, "select level");
    //
    // So we can quit the level and play normally
    //
    g_opt_level_select_menu = false;

    if (v != nullptr) {
      (void) level_changed_for_player(g, v, LEVEL_ARR_IDX_LEVEL_SELECT);
    }
  } else {
    if (g_opt_quick_start) {
      game_state_change(g, STATE_PLAYING, "new game");
    } else {
      if (v != nullptr) {
        (void) level_changed_for_player(g, v, LEVEL_ARR_IDX_LEVEL_SELECT);
      }
    }
  }
}
