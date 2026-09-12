//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_main.hpp"
#include "my_thing_callbacks.hpp"
#include "my_tp.hpp"
#include "my_tps.hpp"
#include "my_ui.hpp"

static auto tp_sac_fumble_more_detail_get(Gamep g, Levelsp v, Levelp l, Thingp me) -> std::string
{
  TRACE();

  return UI_INFO1_FMT_STR                                                                                                           //
      "Dice rolls are the core of this game and most are done on d20s. 20 means a critical hit. 1 means fumble.\n" UI_INFO2_FMT_STR //
      "With this sacrifice, 1 and 2 both result in a fumble, which could impact:\n"                                                 //
      UI_INFO1_FMT_STR                                                                                                              //
      "- combat chance of success\n"                                                                                                //
      UI_INFO2_FMT_STR                                                                                                              //
      "- trap activation\n"                                                                                                         //
      UI_INFO3_FMT_STR                                                                                                              //
      "- luck rolls \n"                                                                                                             //
      UI_INFO4_FMT_STR                                                                                                              //
      "- basically all dice rolls...\n";                                                                                            //
}

[[nodiscard]] auto tp_load_sac_fumble_more() -> bool
{
  TRACE();

  auto *tp   = tp_load("sac_fumble_more"); // keep as string for scripts
  auto  name = tp_name(tp);

  // begin sort marker1 {
  thing_detail_set(tp, tp_sac_fumble_more_detail_get);
  tp_flag_set(tp, is_hook);
  tp_flag_set(tp, is_loggable);
  tp_flag_set(tp, is_sacrifice);
  tp_fumble_roll_set(tp, 2);
  tp_sac_points_set(tp, 10);
  tp_name_long_set(tp, "fumble more");
  // end sort marker1 }

  return true;
}
