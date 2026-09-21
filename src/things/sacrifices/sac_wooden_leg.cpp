//
// Copyright goblinhack@gmail.com
//

#include "../../my_callstack.hpp"
#include "../../my_main.hpp"
#include "../../my_thing_callbacks.hpp"
#include "../../my_tp.hpp"
#include "../../my_tps.hpp"
#include "../../my_ui.hpp"

static auto tp_sac_wooden_leg_detail_get(Gamep g, Levelsp v, Levelp l, Thingp me) -> std::string
{
  TRACE();

  return                                                                                                                  //
      UI_INFO1_FMT_STR "In a previous journey you lost touch with one of your legs in an encounter with a Pirnana.\n"     //
      UI_INFO2_FMT_STR "You struggle on with a wooden leg, but you tire easily and are only able to hop around.\n"        //
      UI_INFO3_FMT_STR "On the plus side, all this hopping has been great cardio and your stamina is vastly above par.\n" //
      UI_INFO4_FMT_STR "Even still, frequent rests will be required...\n";                                                //
}

void tp_sac_wooden_leg_on_hook_attached(Gamep g, Levelsp v, Levelp l, Thingp me)
{
  TRACE();

  auto owner = thing_hook_owner_get(g, v, l, me);
  if (! owner) {
    return;
  }

  auto old_stamina_max = thing_stamina_max(g, v, l, owner);
  auto new_stamina_max = old_stamina_max * 2;
  if (! new_stamina_max) {
    return;
  }

  if (old_stamina_max == new_stamina_max) {
    return;
  }

  (void) thing_stamina_max_set(g, v, l, owner, new_stamina_max);
  (void) thing_stamina_set(g, v, l, owner, new_stamina_max);

  THING_DBG(g, v, l, owner, "new stamina max set to %d", new_stamina_max);
}

[[nodiscard]] auto tp_load_sac_wooden_leg() -> bool
{
  TRACE();

  auto *tp   = tp_load("sac_wooden_leg"); // keep as string for scripts
  auto  name = tp_name(tp);

  // begin sort marker1 {
  thing_detail_set(tp, tp_sac_wooden_leg_detail_get);
  thing_on_hook_attached_set(tp, tp_sac_wooden_leg_on_hook_attached);
  tp_flag_set(tp, is_hook);
  tp_flag_set(tp, is_loggable);
  tp_flag_set(tp, is_sacrifice);
  tp_flag_set(tp, is_wooden_leg);
  tp_sac_points_set(tp, 7);
  tp_name_long_set(tp, "wooden leg");
  // end sort marker1 }

  return true;
}
