//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_game_inlines.hpp"
#include "my_globals.hpp"
#include "my_level.hpp"
#include "my_sound.hpp"
#include "my_thing.hpp"
#include "my_types.hpp"

#include <string>

void thing_sound_play(Gamep g, Levelsp v, Levelp l, Thingp t, const std::string &alias)
{
  TRACE();

  if (g_opt_tests) {
    return;
  }

  auto *player = thing_player(g);
  if (player == nullptr) [[unlikely]] {
    return;
  }

  Levelp player_level = thing_player_level(g);
  if (player_level == nullptr) {
    return;
  }

  auto *next_level_down = level_select_get_next_level_down(g, v, player_level);

  float scale = 1.0;

  //
  // Where did the thing play the sound? Might not be on the same level as the player.
  //
  auto *t_level = game_level_get(g, v, t->level_num);
  if (level_is_boss_level(g, v, t_level)) {
    //
    // Full volume
    //
  } else if (t_level == next_level_down) {
    //
    // It's the level below. Dampen the sound.
    //
    scale = 0.01F;
  } else if ((t_level != nullptr) && (t_level != player_level)) {
    //
    // It's too far away. Ignore the sound.
    //
    return;
  }

  //
  // Dampen sounds on the same level.
  //
  if (t_level == player_level) {
    auto at = thing_at(g, v, l, t);

    if (! thing_vision_can_see_tile(g, v, l, player, at)) {
      return;
    }
  }

  if (scale <= 0) {
    return;
  }

  THING_DBG(g, v, l, t, "play sound %s scale %f", alias.c_str(), scale);

  ((void) sound_play(g, v, l, alias, scale, 0 /* loops */, t));
}
