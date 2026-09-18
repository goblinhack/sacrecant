//
// Copyright goblinhack@gmail.com
//

#include "../../my_callstack.hpp"
#include "../../my_main.hpp"
#include "../../my_thing_callbacks.hpp"
#include "../../my_tp.hpp"
#include "../../my_tps.hpp"
#include "../../my_ui.hpp"

static auto tp_spell_90_get(Gamep g, Levelsp v, Levelp l, Thingp me) -> std::string
{
  TRACE();

  return //
      UI_INFO1_FMT_STR "TODO spell 90\n";
}

[[nodiscard]] auto tp_load_spell_90() -> bool
{
  TRACE();

  auto *tp   = tp_load("spell_90"); // keep as string for scripts
  auto  name = tp_name(tp);

  // begin sort marker1 {
  thing_detail_set(tp, tp_spell_90_get);
  tp_flag_set(tp, is_spell);
  tp_flag_set(tp, is_unused_spell);
  tp_flag_set(tp, is_loggable);
  tp_spell_cost_set(tp, 90);
  tp_name_long_set(tp, "spell 90");
  // end sort marker1 }

  return true;
}
