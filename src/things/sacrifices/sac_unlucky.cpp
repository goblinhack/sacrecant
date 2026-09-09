//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_main.hpp"
#include "my_thing_callbacks.hpp"
#include "my_tp.hpp"
#include "my_tps.hpp"
#include "my_ui.hpp"

static auto tp_sac_unlucky_detail_get(Gamep g, Levelsp v, Levelp l, Thingp me) -> std::string
{
  TRACE();

  return UI_INFO1_FMT_STR                                                                                    //
      "It's just not your day, ever, with this sacrifice.\n"                                                 //
      UI_INFO2_FMT_STR                                                                                       //
      "In detail, your luck is heavily decreased, which has an impact on many things, like the chance of:\n" //
      UI_INFO1_FMT_STR                                                                                       //
      "- volatile treasure chests\n"                                                                         //
      UI_INFO2_FMT_STR                                                                                       //
      "- escaping engulfment\n"                                                                              //
      UI_INFO1_FMT_STR                                                                                       //
      "- landing in lava \n"                                                                                 //
      UI_INFO2_FMT_STR                                                                                       //
      "- teleporting into danger\n"                                                                          //
      UI_INFO1_FMT_STR                                                                                       //
      "- and more...\n";                                                                                     //
}

[[nodiscard]] auto tp_load_sac_unlucky() -> bool
{
  TRACE();

  auto *tp   = tp_load("sac_unlucky"); // keep as string for scripts
  auto  name = tp_name(tp);

  // begin sort marker1 {
  thing_detail_set(tp, tp_sac_unlucky_detail_get);
  tp_flag_set(tp, is_hook);
  tp_flag_set(tp, is_loggable);
  tp_flag_set(tp, is_sacrifice);
  tp_flag_set(tp, is_unlucky);
  tp_mana_set(tp, 20);
  tp_name_long_set(tp, "dashed unlucky");
  tp_stat_set(tp, THING_STAT_LUCK, "6");
  // end sort marker1 }

  return true;
}
