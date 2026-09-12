//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_main.hpp"
#include "my_thing_callbacks.hpp"
#include "my_tp.hpp"
#include "my_tps.hpp"
#include "my_ui.hpp"

#include <math.h>

static auto tp_sac_blood_pact_detail_get(Gamep g, Levelsp v, Levelp l, Thingp me) -> std::string
{
  TRACE();

  return UI_INFO1_FMT_STR                                                                                      //
      "How much do you want to defeat that monster?\n"                                                         //
      UI_INFO2_FMT_STR                                                                                         //
      "Are you willing to deliver double damage and have the extra subtracted from your health and stamina?\n" //
      UI_INFO3_FMT_STR                                                                                         //
      "In detail, if you hit for 5 damage, this is doubled to 10 damage.\n"                                    //
      UI_INFO4_FMT_STR                                                                                         //
      "In turn (5 / 2) (2 rounded down) is subtracted from both your health and stamina.\n"                    //
      UI_INFO5_FMT_STR                                                                                         //
      "Minimum damage subtracted is 1.\n";                                                                     //
}

static bool tp_sac_blood_pact_on_attacking(Gamep g, Levelsp v, Levelp l, Thingp me, Thingp target, ThingEvent &e)
{
  TRACE();

  auto owner = thing_buff_owner_get(g, v, l, me);
  if (! owner) {
    return true;
  }

  auto to_subtract = (int) floor(e.damage / 2);

  if (! to_subtract) {
    to_subtract = 1;
  }

  e.damage *= 2;

  THING_DBG(g, v, l, owner, "damage doubled to %d", e.damage);

  if (thing_health(g, v, l, owner) > to_subtract + 1) {
    (void) thing_health_decr(g, v, l, owner, to_subtract);
  }

  if (thing_stamina(g, v, l, owner) > to_subtract + 1) {
    (void) thing_stamina_decr(g, v, l, owner, to_subtract);
  }

  return true;
}

[[nodiscard]] auto tp_load_sac_blood_pact() -> bool
{
  TRACE();

  auto *tp   = tp_load("sac_blood_pact"); // keep as string for scripts
  auto  name = tp_name(tp);

  // begin sort marker1 {
  thing_detail_set(tp, tp_sac_blood_pact_detail_get);
  thing_on_attacking_set(tp, tp_sac_blood_pact_on_attacking);
  tp_flag_set(tp, is_hook);
  tp_flag_set(tp, is_loggable);
  tp_flag_set(tp, is_sacrifice);
  tp_sac_points_set(tp, 10);
  tp_name_long_set(tp, "blood pact");
  // end sort marker1 }

  return true;
}
