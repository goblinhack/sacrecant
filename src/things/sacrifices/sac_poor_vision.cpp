//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_main.hpp"
#include "my_thing_callbacks.hpp"
#include "my_tp.hpp"
#include "my_tps.hpp"
#include "my_ui.hpp"

static auto tp_sac_poor_vision_detail_get(Gamep g, Levelsp v, Levelp l, Thingp me) -> std::string
{
  TRACE();

  return UI_INFO1_FMT_STR //
      "The amount you can see ahead is limited to a fraction of what it is normally.\n";
}

[[nodiscard]] auto tp_load_sac_poor_vision() -> bool
{
  TRACE();

  auto *tp   = tp_load("sac_poor_vision"); // keep as string for scripts
  auto  name = tp_name(tp);

  // begin sort marker1 {
  thing_detail_set(tp, tp_sac_poor_vision_detail_get);
  tp_flag_set(tp, is_hook);
  tp_flag_set(tp, is_loggable);
  tp_flag_set(tp, is_sacrifice);
  tp_flag_set(tp, is_myopic);
  tp_sac_points_set(tp, 5);
  tp_name_long_set(tp, "myopic mania");
  // end sort marker1 }

  return true;
}
