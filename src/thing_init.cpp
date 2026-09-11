//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_fpoint.hpp"
#include "my_globals.hpp"
#include "my_level.hpp"
#include "my_main.hpp"
#include "my_thing.hpp"
#include "my_thing_inlines.hpp"
#include "my_tp.hpp"
#include "my_tp_inlines.hpp"
#include "my_types.hpp"

[[nodiscard]] auto thing_init(Gamep g, Levelsp v, Levelp l, Tpp tp, const fpoint &real_at) -> Thingp
{
  TRACE();

  auto at = make_bpoint(real_at);

  auto *t = thing_alloc(g, v, l, tp, at);
  if (t == nullptr) {
    return nullptr;
  }

  //
  // Used so often, we cache it
  //
  if (tp_flag(thing_tp(t), is_player) != 0) {
    t->_is_player = true;
  }

  thing_at_set(g, v, l, t, real_at); // INTENTIONAL
  thing_at_set(g, v, l, t, real_at); // INTENTIONAL Doing it twice sets old_at too
  thing_moving_from_set(t, at);

  t->level_num = l->level_num;

  //
  // Assign an initial tile
  //
  thing_anim_init(g, v, l, t, THING_ANIM_IDLE);

  //
  // Assign the player
  //
  if (tp_is_player(tp)) {
    //
    // But only if this is not a temporary thing being created during player selection
    //
    if (! level_is_level_select(g, v, l)) {
      //
      // MUST come before player_state_change
      //
      v->player_id = t->id;

      //
      // Reset the player state
      //
      player_state_change(g, v, l, PLAYER_STATE_NORMAL);

      //
      // First time entering this level?
      //
      if (! l->player_has_entered_level) {
        level_enter(g, v, l, t);
      }
    }
  }

  // *******************************************************************
  // * consider thing_enhance if changing code here
  // *******************************************************************
  FOR_ALL_THING_STAT(stat) { (void) thing_stat_set(g, v, l, t, stat, tp_stat(tp, stat)); }

  (void) thing_charge_count_set(g, v, l, t, tp_charge_count_get(tp));
  (void) thing_distance_avoid_target_set(g, v, l, t, tp_distance_avoid_target_get(tp));
  (void) thing_distance_jump_set(g, v, l, t, tp_distance_jump_get(tp));
  (void) thing_distance_minion_from_mob_max_set(g, v, l, t, tp_distance_minion_from_mob_max_get(tp));
  (void) thing_distance_throw_set(g, v, l, t, tp_distance_throw_get(tp));
  (void) thing_distance_vision_set(g, v, l, t, tp_distance_vision_get(tp));
  (void) thing_health_max_set(g, v, l, t, thing_health_set(g, v, l, t, tp_health_max_get(tp)));
  (void) thing_hearing_threshold_set(g, v, l, t, tp_hearing_threshold_get(tp));
  (void) thing_items_collected_max_set(g, v, l, t, tp_items_collected_max_get(tp));
  (void) thing_minion_max_set(g, v, l, t, tp_minion_max_get(tp));
  (void) thing_missile_count_max_set(g, v, l, t, tp_missile_count_max_get(tp));
  (void) thing_priority_set(g, v, l, t, tp_priority_get(tp));
  (void) thing_score_value_set(g, v, l, t, tp_score_value_get(tp));
  (void) thing_speed_set(g, v, l, t, tp_speed_get(tp));
  (void) thing_stamina_max_set(g, v, l, t, thing_stamina_set(g, v, l, t, tp_stamina_max_get(tp)));
  (void) thing_temperature_set(g, v, l, t, tp_temperature_initial_get(tp));
  (void) thing_value1_set(g, v, l, t, tp_value1_get(tp));
  (void) thing_jump_attack_pct_chance_set(g, v, l, t, tp_jump_attack_pct_chance_get(tp));
  (void) thing_value2_set(g, v, l, t, tp_value2_get(tp));
  (void) thing_value3_set(g, v, l, t, tp_value3_get(tp));
  (void) thing_value4_set(g, v, l, t, tp_value4_get(tp));
  (void) thing_value5_set(g, v, l, t, tp_value5_get(tp));
  (void) thing_crit_roll_set(g, v, l, t, tp_crit_roll_get(tp));
  (void) thing_fumble_roll_set(g, v, l, t, tp_fumble_roll_get(tp));
  (void) thing_difficulty_set(g, v, l, t, tp_difficulty_get(tp));
  (void) thing_mana_set(g, v, l, t, tp_mana_get(tp));
  (void) thing_mana_max_set(g, v, l, t, tp_mana_get(tp));
  (void) thing_variant_set(g, v, l, t, tp_variant_get(tp));
  (void) thing_weight_set(g, v, l, t, tp_weight_get(tp));
  // *******************************************************************
  // * consider thing_enhance if changing code here
  // *******************************************************************

  //
  // More consistent for tests to use max values
  //
  if (g_opt_tests) {
    (void) thing_lifespan_initial_set(g, v, l, t, thing_lifespan_set(g, v, l, t, tp_lifespan_max_get(tp)));
    (void) thing_ticks_to_stay_dead_initial_set(g, v, l, t, thing_ticks_to_stay_dead_set(g, v, l, t, tp_ticks_to_stay_dead_max_get(tp)));
  } else {
    (void) thing_lifespan_initial_set(g, v, l, t, thing_lifespan_set(g, v, l, t, tp_lifespan_get(tp)));
    (void) thing_ticks_to_stay_dead_initial_set(g, v, l, t, thing_ticks_to_stay_dead_set(g, v, l, t, tp_ticks_to_stay_dead_get(tp)));
  }

  //
  // Need to update with the new pixel position
  //
  thing_update_pos(g, v, l, t);

  //
  // Do adjacent tiles need updating due to the destruction of this tiled thing?
  //
  if (thing_is_tiled(t)) {
    level_update_paths_set(g, v, l, at);
  }

  if (compiler_unused) {
    THING_DBG(g, v, l, t, "init");
  }

  return t;
}

[[nodiscard]] auto thing_init(Gamep g, Levelsp v, Levelp l, Tpp tp, const bpoint &at) -> Thingp
{
  return thing_init(g, v, l, tp, make_fpoint(at));
}
