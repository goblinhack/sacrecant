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

void tp_player1_on_spawned(Gamep g, Levelsp v, Levelp l, Thingp me, ThingEvent *e_maybe_null)
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

  //  (void) thing_buff_add(g, v, l, me, tp_find_mand("sac_poison_blood"));
}

auto tp_player1_detail_get(Gamep g, Levelsp v, Levelp l, Thingp me) -> std::string
{
  TRACE();

  return UI_INFO1_FMT_STR "The choice for the sacrecant that doesn't like to sacrifice much!\n" //
      UI_INFO2_FMT_STR "This sacrecant has generous health and no stat penalties.\n"            //
      UI_INFO3_FMT_STR "It really depends on how you define 'generous'...\n";
}

[[nodiscard]] auto tp_load_player1() -> bool
{
  auto *tp   = tp_load("player1"); // keep as string for scripts
  auto  name = tp_name(tp);

  tp_load_player_common(tp);

  if (! g_opt_tests) {
    // begin sort marker1 {
    thing_detail_set(tp, tp_player1_detail_get);
    thing_on_spawned_set(tp, tp_player1_on_spawned);
    tp_difficulty_set(tp, 0);
    tp_mana_set(tp, 100);
    tp_health_set(tp, "250");
    tp_sac_points_set(tp, 5);
    tp_name_long_set(tp, "cowardly wizard");
    // end sort marker1 }
  }

  return true;
}
