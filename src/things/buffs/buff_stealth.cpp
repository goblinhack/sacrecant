//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_main.hpp"
#include "my_thing_callbacks.hpp"
#include "my_tp.hpp"
#include "my_tps.hpp"
#include "my_ui.hpp"

static auto tp_buff_stealth_detail_get(Gamep g, Levelsp v, Levelp l, Thingp me) -> std::string
{
  TRACE();

  return                                                                                                       //
      UI_INFO1_FMT_STR "With this buff you can creep silently through the noisiest of bushes,"                 //
      UI_INFO2_FMT_STR "or cross water as quietly as a morning breeze.\n"                                      //
      UI_INFO3_FMT_STR "Note: weapon fire and other forms of attack are included in this no noise guarantee, " //
                       "even though this makes no sense. I mean, do you get naked for a potion of invisibiliy?\n";
}

[[nodiscard]] auto tp_load_buff_stealth() -> bool
{
  TRACE();

  auto *tp   = tp_load("buff_stealth"); // keep as string for scripts
  auto  name = tp_name(tp);

  // begin sort marker1 {
  thing_detail_set(tp, tp_buff_stealth_detail_get);
  tp_flag_set(tp, is_buff);
  tp_flag_set(tp, is_loggable);
  tp_flag_set(tp, is_stealthy);
  tp_flag_set(tp, is_tick_on_use);
  tp_flag_set(tp, is_tickable);
  tp_lifespan_set(tp, "1d100+1000");
  tp_name_short_set(tp, "stealthy");
  tp_stat_set(tp, THING_STAT_DEF, "12");
  tp_flag_set(tp, is_hook);
  // end sort marker1 }

  return true;
}
