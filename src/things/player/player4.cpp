//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_dice_rolls.hpp"
#include "my_game_popups.hpp"
#include "my_level.hpp"
#include "my_main.hpp"
#include "my_music.hpp"
#include "my_player.hpp"
#include "my_random.hpp"
#include "my_sound.hpp"
#include "my_thing.hpp"
#include "my_thing_callbacks.hpp"
#include "my_thing_inlines.hpp"
#include "my_tile.hpp"
#include "my_tp.hpp"
#include "my_tps.hpp"
#include "my_types.hpp"
#include "my_ui.hpp"

void tp_player4_on_spawned(Gamep g, Levelsp v, Levelp l, Thingp me, ThingEvent *e_maybe_null)
{
  TRACE();
  THING_DBG(g, v, l, me, "player spawned");

  if (g_opt_tests) {
    return;
  }

  static std::initializer_list< std::string > items = {
      "wand_energy", //
  };

  if (! thing_carry(g, v, l, me, items)) {
    thing_err(g, v, l, me, "failed to carry");
  }
}

auto tp_player4_detail_get(Gamep g, Levelsp v, Levelp l, Thingp me) -> std::string
{
  TRACE();

  return UI_INFO1_FMT_STR "This wizard prefers to sprint and leap through the dungeon, avoiding conflict.\n" //
      UI_INFO2_FMT_STR "With such speed, the odds of running into traps is high.\n"                          //
      UI_INFO3_FMT_STR "It might not seem you move fast, but watch how slow the monsters are!\n"             //
      UI_INFO4_FMT_STR "Fortunately, this wizard is blessed with some luck. Some.\n";
}

[[nodiscard]] auto tp_load_player4() -> bool
{
  auto *tp   = tp_load("player4"); // keep as string for scripts
  auto  name = tp_name(tp);

  tp_load_player_common(tp);

  if (! g_opt_tests) {
    // begin sort marker1 {
    thing_detail_set(tp, tp_player4_detail_get);
    thing_on_spawned_set(tp, tp_player4_on_spawned);
    tp_difficulty_set(tp, 3);
    tp_distance_jump_set(tp, 5);
    tp_health_set(tp, "150");
    tp_mana_set(tp, 200);
    tp_name_long_set(tp, "speed demon wizard");
    tp_speed_set(tp, 200);
    tp_stamina_set(tp, "200");
    tp_stat_set(tp, THING_STAT_DEF, "8");
    tp_stat_set(tp, THING_STAT_LUCK, "1d6+10");
    // end sort marker1 }
  }

  return true;
}
