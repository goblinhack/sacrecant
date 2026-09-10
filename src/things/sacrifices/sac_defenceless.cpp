//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_main.hpp"
#include "my_thing_callbacks.hpp"
#include "my_tp.hpp"
#include "my_tps.hpp"
#include "my_ui.hpp"

static auto tp_sac_defenceless_detail_get(Gamep g, Levelsp v, Levelp l, Thingp me) -> std::string
{
  TRACE();

  return UI_INFO1_FMT_STR                                                                                     //
      "A true warrior has no need of defence. Of course, you are a wizard and not a warrior...\n"             //
      UI_INFO2_FMT_STR                                                                                        //
      "Regardless! Eschew all of your defence points and ensure that monsters never miss you being around!\n" //
      UI_INFO3_FMT_STR                                                                                        //
      "Attacks on you can still fumble, so you're not *completely* defenceless.\n";
}

[[nodiscard]] auto tp_load_sac_defenceless() -> bool
{
  TRACE();

  auto *tp   = tp_load("sac_defenceless"); // keep as string for scripts
  auto  name = tp_name(tp);

  // begin sort marker1 {
  thing_detail_set(tp, tp_sac_defenceless_detail_get);
  tp_flag_set(tp, is_hook);
  tp_flag_set(tp, is_loggable);
  tp_flag_set(tp, is_sacrifice);
  tp_mana_set(tp, 50);
  tp_name_long_set(tp, "naked defence");
  tp_stat_set(tp, THING_STAT_DEF, "0");
  // end sort marker1 }

  return true;
}
