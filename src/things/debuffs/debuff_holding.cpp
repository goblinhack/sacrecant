//
// Copyright goblinhack@gmail.com
//

#include "../../my_callstack.hpp"
#include "../../my_main.hpp"
#include "../../my_thing_callbacks.hpp"
#include "../../my_tp.hpp"
#include "../../my_tps.hpp"
#include "../../my_ui.hpp"

static auto tp_debuff_holding_detail_get(Gamep g, Levelsp v, Levelp l, Thingp me) -> std::string
{
  TRACE();

  return UI_INFO1_FMT_STR "You are held tight!";
}

void tp_buff_holding_on_hook_attached(Gamep g, Levelsp v, Levelp l, Thingp me)
{
  TRACE();

  THING_DBG(g, v, l, me, "holding debuff attached");

  (void) thing_leader_spawn_a_follower_at(g, v, l, me, tp_find_random(g, v, l, is_effect_holding), thing_at(g, v, l, me));
}

[[nodiscard]] auto tp_load_debuff_holding() -> bool
{
  TRACE();

  auto *tp   = tp_load("debuff_holding"); // keep as string for scripts
  auto  name = tp_name(tp);

  // begin sort marker1 {
  thing_detail_set(tp, tp_debuff_holding_detail_get);
  thing_on_hook_attached_set(tp, tp_buff_holding_on_hook_attached);
  tp_flag_set(tp, is_blocker_engulfed);
  tp_flag_set(tp, is_blocker_ensnared);
  tp_flag_set(tp, is_blocker_falling);
  tp_flag_set(tp, is_blocker_movement);
  tp_flag_set(tp, is_blocker_teleport);
  tp_flag_set(tp, is_debuff);
  tp_flag_set(tp, is_hook);
  tp_flag_set(tp, is_leader_kill_followers_on_death);
  tp_flag_set(tp, is_leader);
  tp_flag_set(tp, is_loggable);
  tp_flag_set(tp, is_memory_ext);
  tp_flag_set(tp, is_tick_on_use);
  tp_flag_set(tp, is_tickable);
  tp_follower_count_max_set(tp, 1);
  tp_lifespan_set(tp, "1d100+100");
  tp_name_short_set(tp, "holding");
  tp_stat_set(tp, THING_STAT_DEF, "12");
  // end sort marker1 }

  return true;
}
