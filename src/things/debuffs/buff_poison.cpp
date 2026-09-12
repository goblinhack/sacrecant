//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_main.hpp"
#include "my_thing_callbacks.hpp"
#include "my_tp.hpp"
#include "my_tps.hpp"
#include "my_ui.hpp"

static auto tp_buff_poison_detail_get(Gamep g, Levelsp v, Levelp l, Thingp me) -> std::string
{
  TRACE();

  return //
      UI_INFO1_FMT_STR "You are poisoned! Your health is ticking away!";
}

static void tp_poison_on_tick_begin(Gamep g, Levelsp v, Levelp l, Thingp me)
{
  TRACE();

  auto owner = thing_buff_owner_get(g, v, l, me);
  if (owner) {
    (void) thing_attack_poison(g, v, l, owner);
  }
}

[[nodiscard]] auto tp_load_buff_poison() -> bool
{
  TRACE();

  auto *tp   = tp_load("buff_poison"); // keep as string for scripts
  auto  name = tp_name(tp);

  // begin sort marker1 {
  thing_detail_set(tp, tp_buff_poison_detail_get);
  thing_on_tick_begin_set(tp, tp_poison_on_tick_begin);
  tp_flag_set(tp, is_debuff);
  tp_flag_set(tp, is_hook);
  tp_flag_set(tp, is_loggable);
  tp_flag_set(tp, is_tick_on_use);
  tp_flag_set(tp, is_tickable);
  tp_lifespan_set(tp, "1d20");
  tp_name_short_set(tp, "poison");
  tp_stat_set(tp, THING_STAT_DEF, "8");
  // end sort marker1 }

  return true;
}
