//
// Copyright goblinhack@gmail.com
//

#include "../../my_callstack.hpp"
#include "../../my_main.hpp"
#include "../../my_thing_callbacks.hpp"
#include "../../my_tp.hpp"
#include "../../my_tps.hpp"
#include "../../my_ui.hpp"

static auto tp_sac_crit_more_detail_get(Gamep g, Levelsp v, Levelp l, Thingp me) -> std::string
{
  TRACE();

  return                                                                                                                            //
      UI_INFO1_FMT_STR "Dice rolls are the core of this game and most are done on d20s. 20 means a critical hit. 1 means fumble.\n" //
      UI_INFO2_FMT_STR "With this sacrifice, 18, 19 and 20 result in a crit, which will increase your chances of double damage.\n"  //
      UI_INFO3_FMT_STR "The downside? Most of your stats are bad. I lie. They're all bad!\n";                                       //
}

[[nodiscard]] auto tp_load_sac_crit_more() -> bool
{
  TRACE();

  auto *tp   = tp_load("sac_crit_more"); // keep as string for scripts
  auto  name = tp_name(tp);

  // begin sort marker1 {
  thing_detail_set(tp, tp_sac_crit_more_detail_get);
  tp_flag_set(tp, is_hook);
  tp_flag_set(tp, is_loggable);
  tp_flag_set(tp, is_sacrifice);
  tp_crit_roll_set(tp, 18);
  tp_sac_points_set(tp, 1);
  tp_stat_set(tp, THING_STAT_ATT, "1d8");
  tp_stat_set(tp, THING_STAT_DEF, "1d8");
  tp_stat_set(tp, THING_STAT_STR, "1d8");
  tp_stat_set(tp, THING_STAT_CON, "1d8");
  tp_stat_set(tp, THING_STAT_INT, "1d8");
  tp_stat_set(tp, THING_STAT_DEX, "1d8");
  tp_stat_set(tp, THING_STAT_PSI, "1d8");
  tp_stat_set(tp, THING_STAT_LUCK, "1d8");
  tp_name_long_set(tp, "improved crit");
  // end sort marker1 }

  return true;
}
