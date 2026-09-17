//
// Copyright goblinhack@gmail.com
//

#include "../../my_callstack.hpp"
#include "../../my_main.hpp"
#include "../../my_thing_callbacks.hpp"
#include "../../my_tp.hpp"
#include "../../my_tps.hpp"
#include "../../my_ui.hpp"

static auto tp_sac_wall_walker_detail_get(Gamep g, Levelsp v, Levelp l, Thingp me) -> std::string
{
  TRACE();

  return                                                                                                                  //
      UI_INFO1_FMT_STR "Glide through walls like a ghost and avoid your enemies with ease.\n"                             //
      UI_INFO2_FMT_STR "However, just like a ghost, expect to be hunger forever. Food will give you no health bonuses.\n" //
      UI_INFO3_FMT_STR "Note, you are not fully ethereal and lava and fire can cause damage.\n"                           //
      UI_INFO4_FMT_STR "Vaults and locked doors will also block your movement.\n";                                        //
}

[[nodiscard]] auto tp_load_sac_wall_walker() -> bool
{
  TRACE();

  auto *tp   = tp_load("sac_wall_walker"); // keep as string for scripts
  auto  name = tp_name(tp);

  // begin sort marker1 {
  thing_detail_set(tp, tp_sac_wall_walker_detail_get);
  tp_flag_set(tp, is_hook);
  tp_flag_set(tp, is_loggable);
  tp_flag_set(tp, is_sacrifice);
  tp_flag_set(tp, is_wall_walker);
  tp_sac_points_set(tp, 1);
  tp_name_long_set(tp, "wraith like");
  tp_stat_set(tp, THING_STAT_DEF, "14");
  // end sort marker1 }

  return true;
}
