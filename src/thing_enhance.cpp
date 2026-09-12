//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_thing.hpp"
#include "my_tp.hpp"
#include "my_tp_inlines.hpp"
#include "my_types.hpp"

//
// Given an existing thing, enhance it with the tp. This way we can apply buffs repeatedly.
//
void thing_enhance(Gamep g, Levelsp v, Levelp l, Thingp t, Tpp tp)
{
  TRACE();

  if (t == nullptr) {
    return;
  }

  if (tp == nullptr) [[unlikely]] {
    return;
  }

  (void) thing_charge_count_incr(g, v, l, t, tp_charge_count_get(tp));
  (void) thing_distance_avoid_target_incr(g, v, l, t, tp_distance_avoid_target_get(tp));
  (void) thing_hearing_threshold_incr(g, v, l, t, tp_hearing_threshold_get(tp));
  (void) thing_distance_jump_incr(g, v, l, t, tp_distance_jump_get(tp));
  (void) thing_distance_throw_incr(g, v, l, t, tp_distance_throw_get(tp));
  (void) thing_distance_vision_incr(g, v, l, t, tp_distance_vision_get(tp));
  (void) thing_health_max_incr(g, v, l, t, thing_health_incr(g, v, l, t, tp_health_max_get(tp)));
  (void) thing_lifespan_initial_set(g, v, l, t, thing_lifespan_incr(g, v, l, t, tp_lifespan_get(tp)));
  (void) thing_ticks_to_stay_dead_initial_set(g, v, l, t, thing_ticks_to_stay_dead_incr(g, v, l, t, tp_ticks_to_stay_dead_get(tp)));
  (void) thing_minion_max_incr(g, v, l, t, tp_minion_max_get(tp));
  (void) thing_missile_count_max_incr(g, v, l, t, tp_missile_count_max_get(tp));
  (void) thing_speed_incr(g, v, l, t, tp_speed_get(tp));
  (void) thing_stamina_max_incr(g, v, l, t, thing_stamina_incr(g, v, l, t, tp_stamina_max_get(tp)));
  (void) thing_temperature_incr(g, v, l, t, tp_temperature_initial_get(tp));
  (void) thing_value1_incr(g, v, l, t, tp_value1_get(tp));
  (void) thing_mana_incr(g, v, l, t, tp_mana_get(tp));
  (void) thing_mana_max_incr(g, v, l, t, tp_mana_max_get(tp));
  (void) thing_sac_points_incr(g, v, l, t, tp_sac_points_get(tp));
  (void) thing_sac_points_max_incr(g, v, l, t, tp_sac_points_max_get(tp));
  (void) thing_jump_attack_pct_chance_incr(g, v, l, t, tp_jump_attack_pct_chance_get(tp));
  (void) thing_items_collected_max_incr(g, v, l, t, tp_items_collected_max_get(tp));
  (void) thing_value2_incr(g, v, l, t, tp_value2_get(tp));
  (void) thing_value3_incr(g, v, l, t, tp_value3_get(tp));
  (void) thing_value4_incr(g, v, l, t, tp_value4_get(tp));
  (void) thing_value5_incr(g, v, l, t, tp_value5_get(tp));
  (void) thing_crit_roll_incr(g, v, l, t, tp_crit_roll_get(tp));
  (void) thing_fumble_roll_incr(g, v, l, t, tp_fumble_roll_get(tp));
  (void) thing_difficulty_incr(g, v, l, t, tp_difficulty_get(tp));
}
