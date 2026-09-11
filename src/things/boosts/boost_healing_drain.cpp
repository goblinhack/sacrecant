//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_dice_rolls.hpp"
#include "my_main.hpp"
#include "my_thing_callbacks.hpp"
#include "my_thing_inlines.hpp"
#include "my_tp.hpp"
#include "my_tps.hpp"
#include "my_ui.hpp"

static auto tp_boost_healing_drain_detail_get(Gamep g, Levelsp v, Levelp l, Thingp me) -> std::string
{
  TRACE();

  return UI_INFO1_FMT_STR                                                                                                 //
      "Convert that useless stamina into health points by stealing from your stamina when in need!\n"                     //
      UI_INFO2_FMT_STR                                                                                                    //
      "Specifically, when your health dips below 25%%%, each move will transfer a d6 dice roll stamina to your health.\n" //
      UI_INFO3_FMT_STR                                                                                                    //
      "Obviously, low stamina has its own problems and will impact your jumping, throwing etc... but who needs that?";    //
}

static void tp_boost_healing_drain_tick_begin(Gamep g, Levelsp v, Levelp l, Thingp me)
{
  TRACE();

  auto owner = thing_buff_owner_get(g, v, l, me);
  if (! owner) {
    return;
  }

  auto old_stamina    = thing_stamina(g, v, l, owner);
  auto old_health     = thing_health(g, v, l, owner);
  auto old_health_max = thing_health_max(g, v, l, owner);

  if (old_health > old_health_max / 4) {
    return;
  }

  if (old_stamina < 10) {
    return;
  }

  auto change = d6();

  THING_DBG(g, v, l, me, "tick");
  THING_DBG(g, v, l, owner, "old health %d, old stamina %d", old_health, old_stamina);

  auto new_health  = thing_health_incr(g, v, l, owner, change);
  auto new_stamina = thing_stamina_decr(g, v, l, owner, change);

  THING_DBG(g, v, l, owner, "new health %d, new stamina %d", new_health, new_stamina);

  if (old_health == new_health) {
    return;
  }

  if (thing_is_player(owner)) {
    topcon(UI_GOOD_FMT_STR "You drain your stamina for health." UI_RESET_FMT);
  }
}

[[nodiscard]] auto tp_load_sac_healing_drain() -> bool
{
  TRACE();

  auto *tp   = tp_load("boost_healing_drain"); // keep as string for scripts
  auto  name = tp_name(tp);

  // begin sort marker1 {
  thing_detail_set(tp, tp_boost_healing_drain_detail_get);
  thing_on_tick_begin_set(tp, tp_boost_healing_drain_tick_begin);
  tp_flag_set(tp, is_boost);
  tp_flag_set(tp, is_hook);
  tp_flag_set(tp, is_loggable);
  tp_mana_set(tp, -5);
  tp_name_long_set(tp, "healing drain");
  // end sort marker1 }

  return true;
}
