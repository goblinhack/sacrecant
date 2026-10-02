//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_game_defs.hpp"
#include "my_level.hpp"
#include "my_main.hpp"
#include "my_thing.hpp"
#include "my_thing_callbacks.hpp"
#include "my_thing_inlines.hpp"
#include "my_tp.hpp"
#include "my_types.hpp"
#include "my_ui.hpp"

void thing_trap_handle(Gamep g, Levelsp v, Levelp l, Thingp me)
{
  TRACE();

  if (me->tick_trap == v->tick) {
    return;
  }
  me->tick_trap = v->tick;

  THING_DBG(g, v, l, me, "over trap");

  auto   at   = thing_at(g, v, l, me);
  Thingp trap = level_is_trap(g, v, l, at);

  if (trap == nullptr) {
    thing_err(g, v, l, me, "no trap of trap found to handle");
    return;
  }

  thing_sound_play(g, v, l, me, "click");

  if (thing_stat_success(g, v, l, me, THING_STAT_LUCK, TARGET_ROLL_TRAP)) {
    //
    // Lucky
    //
    if (thing_is_player(me)) {
      topcon(UI_IMPORTANT_FMT_STR "You feel a loose floor tile." UI_RESET_FMT);
    }
  } else if (thing_stat_success(g, v, l, me, THING_STAT_DEX, TARGET_ROLL_TRAP)) {
    //
    // Skillful
    //
    if (thing_is_player(me)) {
      topcon(UI_IMPORTANT_FMT_STR "You skillfully skip over a trap!" UI_RESET_FMT);
    }
  } else if (thing_on_activated(g, v, l, trap, me)) {
    //
    // The trap springs
    //
    if (thing_is_player(me)) {
      topcon(UI_IMPORTANT_FMT_STR "You heard a click..." UI_RESET_FMT);
    }
  } else {
    //
    // The trap fails
    //
    if (thing_is_player(me)) {
      topcon(UI_IMPORTANT_FMT_STR "You heard a click, but nothing happens..." UI_RESET_FMT);
    }
  }
}
