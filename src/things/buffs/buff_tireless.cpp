//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_main.hpp"
#include "my_thing_callbacks.hpp"
#include "my_tp.hpp"
#include "my_tps.hpp"
#include "my_ui.hpp"

static auto tp_buff_tireless_detail_get(Gamep g, Levelsp v, Levelp l, Thingp me) -> std::string
{
  TRACE();

  return                                                                                  //
      UI_INFO1_FMT_STR "With this buff your tireesome issues are a thing of the past.\n"; //
}

[[nodiscard]] auto tp_load_buff_tireless() -> bool
{
  TRACE();

  auto *tp   = tp_load("buff_tireless"); // keep as string for scripts
  auto  name = tp_name(tp);

  // begin sort marker1 {
  thing_detail_set(tp, tp_buff_tireless_detail_get);
  tp_flag_set(tp, is_buff);
  tp_flag_set(tp, is_loggable);
  tp_flag_set(tp, is_tick_on_use);
  tp_flag_set(tp, is_tickable);
  tp_flag_set(tp, is_tireless);
  tp_lifespan_set(tp, "1d100+1000");
  tp_name_short_set(tp, "tireless");
  tp_stat_set(tp, THING_STAT_CON, "12");
  tp_stat_set(tp, THING_STAT_DEF, "12");
  tp_stat_set(tp, THING_STAT_STR, "12");
  tp_flag_set(tp, is_hook);
  // end sort marker1 }

  return true;
}
