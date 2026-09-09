//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_main.hpp"
#include "my_thing_callbacks.hpp"
#include "my_tp.hpp"
#include "my_tps.hpp"
#include "my_ui.hpp"

static auto tp_buff_bad_luck_detail_get(Gamep g, Levelsp v, Levelp l, Thingp me) -> std::string
{
  TRACE();

  return                                                                        //
      UI_INFO1_FMT_STR "Feel the heavy burden of bad luck with this debuff.\n"; //
}

[[nodiscard]] auto tp_load_buff_bad_luck() -> bool
{
  TRACE();

  auto *tp   = tp_load("buff_bad_luck"); // keep as string for scripts
  auto  name = tp_name(tp);

  // begin sort marker1 {
  thing_detail_set(tp, tp_buff_bad_luck_detail_get);
  tp_flag_set(tp, is_buff);
  tp_flag_set(tp, is_loggable);
  tp_flag_set(tp, is_tick_on_use);
  tp_flag_set(tp, is_tickable);
  tp_lifespan_set(tp, "1d100+1000");
  tp_name_short_set(tp, "bad luck");
  tp_stat_set(tp, THING_STAT_LUCK, "9");
  tp_flag_set(tp, is_hook);
  // end sort marker1 }

  return true;
}
