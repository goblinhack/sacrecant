//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_main.hpp"
#include "my_thing_callbacks.hpp"
#include "my_tp.hpp"
#include "my_tps.hpp"
#include "my_ui.hpp"

static auto tp_sac_noisy_detail_get(Gamep g, Levelsp v, Levelp l, Thingp me) -> std::string
{
  TRACE();

  return UI_INFO1_FMT_STR                                                                //
      "Your very footsteps echo noisily as you clod around the dungeon "                 //
      "and no attempt to stay quiet seems to help!\n"                                    //
      UI_INFO2_FMT_STR                                                                   //
      "In detail, this sacrifice doubles the amount of noise you make. "                 //
      "From crashing through bushes, crushing grass underfoot, or firing weapons, "      //
      "all will be doubled in noise output.\n"                                           //
      UI_INFO3_FMT_STR                                                                   //
      "The noise you generate impacts your stealth and hence ability to stay hidden.\n"; //
}

[[nodiscard]] auto tp_load_sac_noisy() -> bool
{
  TRACE();

  auto *tp   = tp_load("sac_noisy"); // keep as string for scripts
  auto  name = tp_name(tp);

  // begin sort marker1 {
  thing_detail_set(tp, tp_sac_noisy_detail_get);
  tp_flag_set(tp, is_hook);
  tp_flag_set(tp, is_loggable);
  tp_flag_set(tp, is_noisy);
  tp_flag_set(tp, is_sacrifice);
  tp_mana_set(tp, 10);
  tp_name_long_set(tp, "squeaky boots");
  // end sort marker1 }

  return true;
}
