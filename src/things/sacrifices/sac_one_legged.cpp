//
// Copyright goblinhack@gmail.com
//

#include "../../my_callstack.hpp"
#include "../../my_main.hpp"
#include "../../my_thing_callbacks.hpp"
#include "../../my_tp.hpp"
#include "../../my_tps.hpp"
#include "../../my_ui.hpp"

static auto tp_sac_one_legged_detail_get(Gamep g, Levelsp v, Levelp l, Thingp me) -> std::string
{
  TRACE();

  return                                                                                                           //
      UI_INFO1_FMT_STR "With one leg, life is a challenge. Jumping over chasms doubly so.\n"                       //
      UI_INFO2_FMT_STR "With this sacrifice, you tire easily, but are gifted with higher stamina to compensate.\n" //
      UI_INFO3_FMT_STR "It helps a bit, but frequent rests will be required...\n";                                 //
}

void tp_sac_one_legged_hook_on_attached(Gamep g, Levelsp v, Levelp l, Thingp me)
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

[[nodiscard]] auto tp_load_sac_one_legged() -> bool
{
  TRACE();

  auto *tp   = tp_load("sac_one_legged"); // keep as string for scripts
  auto  name = tp_name(tp);

  // begin sort marker1 {
  thing_detail_set(tp, tp_sac_one_legged_detail_get);
  thing_hook_on_attached_set(tp, tp_sac_one_legged_hook_on_attached);
  tp_flag_set(tp, is_hook);
  tp_flag_set(tp, is_loggable);
  tp_flag_set(tp, is_sacrifice);
  tp_flag_set(tp, is_one_legged);
  tp_sac_points_set(tp, 7);
  tp_name_long_set(tp, "one legged");
  // end sort marker1 }

  return true;
}
