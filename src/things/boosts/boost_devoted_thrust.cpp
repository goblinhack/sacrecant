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

static auto tp_boost_devoted_thrust_detail_get(Gamep g, Levelsp v, Levelp l, Thingp me) -> std::string
{
  TRACE();

  return UI_INFO1_FMT_STR                                                                                                                 //
      "Prove your devotion by adding extra damage to attacks when you are in low health!\n"                                               //
      UI_INFO2_FMT_STR                                                                                                                    //
      "Specifically, when your health dips below 25%%%, each melee attack will add a d6 dice roll, draining your stamina for the same.\n" //
      UI_INFO3_FMT_STR                                                                                                                    //
      "It makes no sense that this works for wands and staffs also, but the gods are feeling generous.\n";
}

static bool tp_boost_devoted_thrust_on_attacking(Gamep g, Levelsp v, Levelp l, Thingp me, Thingp target, ThingEvent &e)
{
  TRACE();

  auto attacker = thing_buff_owner_get(g, v, l, me);
  if (! attacker) {
    return true;
  }

  auto old_stamina    = thing_stamina(g, v, l, attacker);
  auto old_health     = thing_health(g, v, l, attacker);
  auto old_health_max = thing_health_max(g, v, l, attacker);

  if (old_health > old_health_max / 4) {
    return true;
  }

  if (old_stamina < 10) {
    return true;
  }

  auto change = d6();

  e.damage += change;

  THING_DBG(g, v, l, attacker, "old stamina %d", old_stamina);

  auto new_stamina = thing_stamina_decr(g, v, l, attacker, change);

  THING_DBG(g, v, l, attacker, "new stamina %d", new_stamina);

  if (thing_is_player(attacker)) {
    topcon(UI_GOOD_FMT_STR "You strike with devotion for extra damage!" UI_RESET_FMT);
  }

  return true;
}

[[nodiscard]] auto tp_load_boost_devoted_thrust() -> bool
{
  TRACE();

  auto *tp   = tp_load("boost_devoted_thrust"); // keep as string for scripts
  auto  name = tp_name(tp);

  // begin sort marker1 {
  thing_detail_set(tp, tp_boost_devoted_thrust_detail_get);
  thing_on_attacking_set(tp, tp_boost_devoted_thrust_on_attacking);
  tp_flag_set(tp, is_boost);
  tp_flag_set(tp, is_hook);
  tp_flag_set(tp, is_loggable);
  tp_mana_set(tp, -100);
  tp_name_long_set(tp, "devoted thrust");
  // end sort marker1 }

  return true;
}
