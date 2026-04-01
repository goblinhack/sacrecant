//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_globals.hpp"
#include "my_main.hpp"
#include "my_thing_inlines.hpp"

auto thing_init(Gamep g, Levelsp v, Levelp l, Tpp tp, const fpoint &real_at) -> Thingp
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
      level_enter(g, v, l);
    }
  }

  //
  // Assign the current level select id. This is only valid when in the select screen.
  //
  if (tp_is_level_curr(tp)) {
    v->level_select_id = t->id;
  }

  (void) thing_distance_minion_from_mob_max_set(g, v, l, t, tp_distance_minion_from_mob_max_get(tp));
  (void) thing_distance_vision_set(g, v, l, t, tp_distance_vision_get(tp));
  (void) thing_health_set(g, v, l, t, tp_health_max_get(tp));
  (void) thing_distance_jump_set(g, v, l, t, tp_distance_jump_get(tp));
  (void) thing_minion_max_set(g, v, l, t, tp_minion_max_get(tp));
  (void) thing_projectile_max_set(g, v, l, t, tp_projectile_max_get(tp));
  (void) thing_priority_set(g, v, l, t, tp_priority_get(tp));
  (void) thing_speed_set(g, v, l, t, tp_speed_get(tp));
  (void) thing_temperature_set(g, v, l, t, tp_temperature_initial_get(tp));
  (void) thing_value1_set(g, v, l, t, tp_value1_get(tp));
  (void) thing_value10_set(g, v, l, t, tp_value10_get(tp));
  (void) thing_value11_set(g, v, l, t, tp_value11_get(tp));
  (void) thing_value12_set(g, v, l, t, tp_value12_get(tp));
  (void) thing_value13_set(g, v, l, t, tp_value13_get(tp));
  (void) thing_value14_set(g, v, l, t, tp_value14_get(tp));
  (void) thing_value15_set(g, v, l, t, tp_value15_get(tp));
  (void) thing_value16_set(g, v, l, t, tp_value16_get(tp));
  (void) thing_value17_set(g, v, l, t, tp_value17_get(tp));
  (void) thing_value18_set(g, v, l, t, tp_value18_get(tp));
  (void) thing_value19_set(g, v, l, t, tp_value19_get(tp));
  (void) thing_value2_set(g, v, l, t, tp_value2_get(tp));
  (void) thing_score_value_set(g, v, l, t, tp_score_value_get(tp));
  (void) thing_value3_set(g, v, l, t, tp_value3_get(tp));
  (void) thing_value4_set(g, v, l, t, tp_value4_get(tp));
  (void) thing_value5_set(g, v, l, t, tp_value5_get(tp));
  (void) thing_value6_set(g, v, l, t, tp_value6_get(tp));
  (void) thing_value7_set(g, v, l, t, tp_value7_get(tp));
  (void) thing_value8_set(g, v, l, t, tp_value8_get(tp));
  (void) thing_value9_set(g, v, l, t, tp_value9_get(tp));
  (void) thing_variant_set(g, v, l, t, tp_variant_get(tp));
  (void) thing_distance_vision_set(g, v, l, t, tp_distance_vision_get(tp));
  (void) thing_weight_set(g, v, l, t, tp_weight_get(tp));
  //
  // More consistent for tests to use max values
  //
  if (g_opt_tests) {
    (void) thing_lifespan_set(g, v, l, t, tp_lifespan_max_get(tp));
  } else {
    (void) thing_lifespan_set(g, v, l, t, tp_lifespan_get(tp));
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
    THING_DBG(t, "init");
  }

  return t;
}

auto thing_init(Gamep g, Levelsp v, Levelp l, Tpp tp, const bpoint &at) -> Thingp { return thing_init(g, v, l, tp, make_fpoint(at)); }
