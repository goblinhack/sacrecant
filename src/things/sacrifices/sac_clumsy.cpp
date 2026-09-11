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

  return UI_INFO1_FMT_STR                                                                                                     //
      "You'd trip over your own feet if you hadn't already stumbled into a chasm, with this sacrifice.\n"                     //
      UI_INFO2_FMT_STR                                                                                                        //
      "In detail, your dexterity and defence is heavily decreased, which has an impact on many things, like the chance of:\n" //
      UI_INFO3_FMT_STR                                                                                                        //
      "- avoiding traps\n"                                                                                                    //
      UI_INFO4_FMT_STR                                                                                                        //
      "- escaping spiderwebs\n"                                                                                               //
      UI_INFO5_FMT_STR                                                                                                        //
      "- and more...\n";                                                                                                      //
}

[[nodiscard]] auto tp_load_sac_clumsy() -> bool
{
  TRACE();

  auto *tp   = tp_load("sac_clumsy"); // keep as string for scripts
  auto  name = tp_name(tp);

  // begin sort marker1 {
  thing_detail_set(tp, tp_sac_clumsy_detail_get);
  tp_flag_set(tp, is_hook);
  tp_flag_set(tp, is_loggable);
  tp_flag_set(tp, is_sacrifice);
  tp_mana_set(tp, 3);
  tp_name_long_set(tp, "clumsy clodhopper");
  tp_stat_set(tp, THING_STAT_DEF, "8");
  tp_stat_set(tp, THING_STAT_DEX, "8");
  // end sort marker1 }

  return true;
}
