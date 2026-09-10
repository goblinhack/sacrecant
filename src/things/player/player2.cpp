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

void tp_player2_on_spawned(Gamep g, Levelsp v, Levelp l, Thingp me, ThingEvent *e_maybe_null)
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
      "wand_fire", //
  };

  if (! thing_carry(g, v, l, me, items)) {
    thing_err(g, v, l, me, "failed to carry");
  }
}

auto tp_player2_detail_get(Gamep g, Levelsp v, Levelp l, Thingp me) -> std::string
{
  TRACE();

  return UI_INFO1_FMT_STR "This wizard scoffed at the need for health and instead trained hard to increase their physical traits.\n" //
      UI_INFO2_FMT_STR "Although you still have the much needed fireball weapon, it receives a needed damage boost.\n"               //
      UI_INFO3_FMT_STR "Should melee be more your thing, your fists are now furious weapons of pain and delivery.\n"                 //
      UI_INFO4_FMT_STR "For fist only operation, simply unwield your fireball and bump into enemies instead.\n"                      //
      UI_INFO5_FMT_STR "Combine this with attack bonuses, this wizard will fight well, while they last.\n";
}

[[nodiscard]] auto tp_load_player2() -> bool
{
  auto *tp   = tp_load("player2"); // keep as string for scripts
  auto  name = tp_name(tp);

  tp_load_player_common(tp);

  if (! g_opt_tests) {
    // begin sort marker1 {
    thing_detail_set(tp, tp_player2_detail_get);
    thing_on_spawned_set(tp, tp_player2_on_spawned);
    tp_damage_set(tp, THING_EVENT_MELEE_DAMAGE, "2d4");
    tp_difficulty_set(tp, 1);
    tp_health_set(tp, "150");
    tp_mana_set(tp, 100);
    tp_name_long_set(tp, "fisticuffs wizard");
    tp_stat_set(tp, THING_STAT_ATT, "1d4+10");
    tp_stat_set(tp, THING_STAT_DEF, "8");
    tp_stat_set(tp, THING_STAT_DMG, "12");
    // end sort marker1 }
  }

  return true;
}
