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

void tp_player3_on_spawned(Gamep g, Levelsp v, Levelp l, Thingp me, ThingEvent *e_maybe_null)
{
  TRACE();
  THING_DBG(g, v, l, me, "player spawned");

  if (g_opt_tests) {
    return;
  }

  //
  // Look ahead to the sacrifices we're going to add
  //
  for (auto chosen_sac : game_chosen_sacrifice_get(g)) {
    if (tp_is_weaponless_on_spawn(chosen_sac)) {
      return;
    }
  }
  static std::initializer_list< std::string > items = {
      "staff_fire", //
  };

  if (! thing_carry(g, v, l, me, items)) {
    thing_err(g, v, l, me, "failed to carry");
  }
}

auto tp_player3_detail_get(Gamep g, Levelsp v, Levelp l, Thingp me) -> std::string
{
  TRACE();

  return UI_INFO1_FMT_STR "This wizard prefers to bathe in the flames. Handy for a dungeon almost always on fire.\n" //
      UI_INFO2_FMT_STR "Note, fire resistant, not fire proof... Half damage from all fire related activities.\n"     //
      UI_INFO3_FMT_STR "For weaponry, the fireball is replaced with a more powerful beam of fire weapon.\n"          //
      UI_INFO4_FMT_STR "For maximum effect, line up enemies to strike them all in one go with this staff.\n"         //
      UI_INFO5_FMT_STR "With such great power comes... limited health and lower defences.\n";
}

[[nodiscard]] auto tp_load_player3() -> bool
{
  auto *tp   = tp_load("player3"); // keep as string for scripts
  auto  name = tp_name(tp);

  tp_load_player_common(tp);

  if (! g_opt_tests) {
    // begin sort marker1 {
    thing_detail_set(tp, tp_player3_detail_get);
    thing_on_spawned_set(tp, tp_player3_on_spawned);
    tp_chance_set(tp, THING_CHANCE_CONTINUE_TO_BURN, "1d20"); // fumble => intensify / keep burning / crit => stop burning
    tp_chance_set(tp, THING_CHANCE_START_BURNING, "1d1000");  // fumble => flames spread to you
    tp_difficulty_set(tp, 2);
    tp_health_set(tp, "100");
    tp_is_resistant_to_add(tp, THING_EVENT_FIRE_DAMAGE);
    tp_mana_set(tp, 150);
    tp_name_long_set(tp, "firestarter wizard");
    tp_stat_set(tp, THING_STAT_ATT, "1d4+10");
    tp_stat_set(tp, THING_STAT_DEF, "12");
    // end sort marker1 }
  }

  return true;
}
