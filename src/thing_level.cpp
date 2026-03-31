//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_game.hpp"
#include "my_globals.hpp"
#include "my_main.hpp"
#include "my_thing_inlines.hpp"

//
// Return the level of the thing
//
auto thing_level(Gamep g, Levelsp v, Thingp t) -> Levelp
{
  TRACE();

  auto *o = thing_find(g, v, t->id);
  if (t != o) {
    CROAK("thing mismatch found for id, %p 08%" PRIX32 "", (void *) t, t->id);
  }

  return game_level_get(g, v, t->level_num);
}

auto thing_on_same_level_as_player(Gamep g, Levelsp v, Thingp t) -> bool
{
  TRACE();

  auto *player = thing_player(g);
  if (player == nullptr) [[unlikely]] {
    return false;
  }

  Levelp player_level = thing_player_level(g);
  if (player_level == nullptr) {
    return false;
  }

  auto *t_level = game_level_get(g, v, t->level_num);
  return t_level == player_level;
}

//
// Move the thing immediately to the new level entrance
//
void thing_level_warp_to_entrance(Gamep g, Levelsp v, Levelp new_level, Thingp t)
{
  TRACE();

  if (t == nullptr) {
    return;
  }

  auto *old_level = game_level_get(g, v, t->level_num);
  if (old_level == new_level) {
    return;
  }

  //
  // Now move to the correct location
  //
  if (! thing_warp_to(g, v, new_level, t, new_level->entrance)) {
    topcon("You seem lost and unable to find the entrance!");
  }
}

//
// Move the thing immediately to the new level exit
//
void thing_level_warp_to_exit(Gamep g, Levelsp v, Levelp new_level, Thingp t)
{
  TRACE();

  if (t == nullptr) {
    return;
  }

  auto *old_level = game_level_get(g, v, t->level_num);
  if (old_level == new_level) {
    return;
  }

  //
  // Now move to the correct location
  //
  if (! thing_warp_to(g, v, new_level, t, new_level->exit)) {
    topcon("You seem lost and unable to find the exit!");
  }
}
