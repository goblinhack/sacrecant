//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_game.hpp"
#include "my_main.hpp"
#include "my_thing.hpp"
#include "my_tp.hpp"
#include "my_types.hpp"

#include <string>

[[nodiscard]] auto thing_is_game_over(Thingp player) -> bool
{
#ifdef DEBUG_BUILD
  TRACE_DEBUG();
  if (player == nullptr) [[unlikely]] {
    no_thing_pointer();
    return false;
  }
#endif

  return player->_is_game_over;
}

void thing_is_game_over_set(Gamep g, Levelsp v, Levelp l, Thingp player, bool val)
{
  TRACE_DEBUG();

  if (player == nullptr) [[unlikely]] {
    no_thing_pointer();
    return;
  }

  if (player->_is_game_over == static_cast< int >(val)) {
    return;
  }
  player->_is_game_over = val;

  if (val) {
    THING_DBG(g, v, l, player, "reached the final exit");
  }

  ThingEvent e {
      .reason     = "escaped the dungeon", //
      .event_type = THING_EVENT_GAME_OVER, //
  };

  (void) thing_score_incr(g, v, l, player, 10000);

  thing_is_game_over_set(g, v, l, player);
  thing_dead(g, v, l, player, e);
  thing_anim_init(g, v, l, player, THING_ANIM_IDLE);

  game_request_to_end_game_set(g);
  game_request_to_end_game_reason_set(g, "game over");
}

void thing_is_game_over_unset(Gamep g, Levelsp v, Levelp l, Thingp player)
{
  TRACE_DEBUG();

  thing_is_game_over_set(g, v, l, player, false);
}
