//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_main.hpp"
#include "my_thing_callbacks.hpp"
#include "my_tp.hpp"
#include "my_tps.hpp"
#include "my_ui.hpp"

static auto tp_sac_sickly_health_detail_get(Gamep g, Levelsp v, Levelp l, Thingp me) -> std::string
{
  TRACE();

  return UI_INFO1_FMT_STR //
      "With this sacrifice, your maximum health is reduced by 10%%%\n";
}

void tp_sac_sickly_health_on_attached(Gamep g, Levelsp v, Levelp l, Thingp me)
{
  TRACE();

  auto owner = thing_buff_owner_get(g, v, l, me);
  if (! owner) {
    return;
  }

  auto old_health_max = thing_health_max(g, v, l, owner);
  auto new_health_max = old_health_max - (old_health_max / 10);
  if (! new_health_max) {
    return;
  }

  if (old_health_max == new_health_max) {
    return;
  }

  (void) thing_health_max_set(g, v, l, owner, new_health_max);

  THING_DBG(g, v, l, owner, "new health max set to %d", new_health_max);
}

[[nodiscard]] auto tp_load_sac_sickly_health() -> bool
{
  TRACE();

  auto *tp   = tp_load("sac_sickly_health"); // keep as string for scripts
  auto  name = tp_name(tp);

  // begin sort marker1 {
  thing_detail_set(tp, tp_sac_sickly_health_detail_get);
  thing_on_attached_set(tp, tp_sac_sickly_health_on_attached);
  tp_flag_set(tp, is_hook);
  tp_flag_set(tp, is_loggable);
  tp_flag_set(tp, is_sacrifice);
  tp_sac_points_set(tp, 10);
  tp_name_long_set(tp, "sickly health");
  // end sort marker1 }

  return true;
}
