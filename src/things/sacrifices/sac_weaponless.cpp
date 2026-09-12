//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_main.hpp"
#include "my_thing_callbacks.hpp"
#include "my_tp.hpp"
#include "my_tps.hpp"
#include "my_ui.hpp"

static auto tp_sac_weaponless_detail_get(Gamep g, Levelsp v, Levelp l, Thingp me) -> std::string
{
  TRACE();

  return UI_INFO1_FMT_STR                                                              //
      "Who needs weapons? Instead just rely on melee by bashing into your opponent!\n" //
      UI_INFO2_FMT_STR                                                                 //
      "This sacrifice does not block you from collecting a weapon later.\n"            //
      UI_INFO3_FMT_STR                                                                 //
      "But why would you want to with such damage bonuses?\n";                         //
}

[[nodiscard]] auto tp_load_sac_weaponless() -> bool
{
  TRACE();

  auto *tp   = tp_load("sac_weaponless"); // keep as string for scripts
  auto  name = tp_name(tp);

  // begin sort marker1 {
  thing_detail_set(tp, tp_sac_weaponless_detail_get);
  tp_flag_set(tp, is_hook);
  tp_flag_set(tp, is_loggable);
  tp_flag_set(tp, is_sacrifice);
  tp_flag_set(tp, is_weaponless_on_spawn);
  tp_sac_points_set(tp, 10);
  tp_name_long_set(tp, "weapons disdain");
  tp_stat_set(tp, THING_STAT_DMG, "16");
  tp_stat_set(tp, THING_STAT_STR, "12");
  // end sort marker1 }

  return true;
}
