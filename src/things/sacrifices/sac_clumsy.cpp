//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_main.hpp"
#include "my_thing_callbacks.hpp"
#include "my_tp.hpp"
#include "my_tps.hpp"
#include "my_ui.hpp"

static auto tp_sac_clumsy_detail_get(Gamep g, Levelsp v, Levelp l, Thingp me) -> std::string
{
  TRACE();

  return UI_INFO1_FMT_STR "This sacrifice doubles any noise that you make.";
}

[[nodiscard]] auto tp_load_sac_clumsy() -> bool
{
  TRACE();

  auto *tp   = tp_load("sac_clumsy"); // keep as string for scripts
  auto  name = tp_name(tp);

  // begin sort marker1 {
  thing_detail_set(tp, tp_sac_clumsy_detail_get);
  tp_flag_set(tp, is_noisy);
  tp_flag_set(tp, is_loggable);
  tp_flag_set(tp, is_sacrifice);
  tp_flag_set(tp, is_tick_on_use);
  tp_flag_set(tp, is_tickable);
  tp_flag_set(tp, is_hook);
  tp_name_long_set(tp, "clumsy");
  // end sort marker1 }

  return true;
}
