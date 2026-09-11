//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_tests.hpp"

[[nodiscard]] auto tests_init() -> bool
{
  TRACE();

  // clang-format off
  /* begin shell marker1 */
  /* shell for i in $(find tests -name "*.cpp" | xargs grep -h "^..nodiscard.. auto test_load" | sort | awk '{print $3}' | cut -d'(' -f1 ) */
  /* shell do */
  /* shell echo "  if (!$i()) { return false; }" */
  /* shell done */
  if (!test_load_alloc_free_monsts()) { return false; }
  if (!test_load_alloc_free_walls()) { return false; }
  if (!test_load_barrel_explosion()) { return false; }
  if (!test_load_barrel()) { return false; }
  if (!test_load_beam_weapon_barrel_explosion()) { return false; }
  if (!test_load_beam_weapon_border_turns_to_lava()) { return false; }
  if (!test_load_beam_weapon_door_locked()) { return false; }
  if (!test_load_beam_weapon_door_secret()) { return false; }
  if (!test_load_beam_weapon_door_unlocked()) { return false; }
  if (!test_load_beam_weapon_mob()) { return false; }
  if (!test_load_beam_weapon_monst_chain()) { return false; }
  if (!test_load_beam_weapon_over_grass()) { return false; }
  if (!test_load_beam_weapon_over_water()) { return false; }
  if (!test_load_beam_weapon_rock_turns_to_lava()) { return false; }
  if (!test_load_beam_weapon_rubble()) { return false; }
  if (!test_load_beam_weapon_teleport()) { return false; }
  if (!test_load_beam_weapon_wall_turns_to_lava()) { return false; }
  if (!test_load_beam_weapon()) { return false; }
  if (!test_load_boost_steal_heal()) { return false; }
  if (!test_load_boots_fire_player_lava()) { return false; }
  if (!test_load_boots_spider_player_spiderweb()) { return false; }
  if (!test_load_brazier_shove_chasm_twice()) { return false; }
  if (!test_load_brazier_shove_chasm()) { return false; }
  if (!test_load_brazier_shove_fail()) { return false; }
  if (!test_load_brazier_shove_into_mob()) { return false; }
  if (!test_load_brazier_shove_ok()) { return false; }
  if (!test_load_bridge_on_fire_over_lava()) { return false; }
  if (!test_load_bridge_on_fire()) { return false; }
  if (!test_load_chocolate_frog()) { return false; }
  if (!test_load_clown_meat()) { return false; }
  if (!test_load_drop_all_items()) { return false; }
  if (!test_load_drop_cursed_item()) { return false; }
  if (!test_load_drop_items()) { return false; }
  if (!test_load_drop_then_recollect_items()) { return false; }
  if (!test_load_fire_on_water()) { return false; }
  if (!test_load_fire_over_chasm()) { return false; }
  if (!test_load_foliage_on_fire()) { return false; }
  if (!test_load_items_drift_away()) { return false; }
  if (!test_load_jump_ok()) { return false; }
  if (!test_load_jump_truncated()) { return false; }
  if (!test_load_large_fire_water()) { return false; }
  if (!test_load_mob_shove_into_lava()) { return false; }
  if (!test_load_mob_shove_ok()) { return false; }
  if (!test_load_mob_water()) { return false; }
  if (!test_load_monst_amphibian()) { return false; }
  if (!test_load_monst_avoids_chasm()) { return false; }
  if (!test_load_monst_avoids_lava()) { return false; }
  if (!test_load_monst_avoids_player()) { return false; }
  if (!test_load_monst_avoids_water_usually()) { return false; }
  if (!test_load_monst_avoids_water()) { return false; }
  if (!test_load_monst_blasts_player()) { return false; }
  if (!test_load_monst_crosses_chasm()) { return false; }
  if (!test_load_monst_crosses_lava()) { return false; }
  if (!test_load_monst_crosses_rubble()) { return false; }
  if (!test_load_monst_crosses_water()) { return false; }
  if (!test_load_monst_door_locked()) { return false; }
  if (!test_load_monst_door_slam_attack()) { return false; }
  if (!test_load_monst_door_unlocked()) { return false; }
  if (!test_load_monst_fast()) { return false; }
  if (!test_load_monst_fish_jump()) { return false; }
  if (!test_load_monst_fish()) { return false; }
  if (!test_load_monst_jump_land_then_pounce_over_water()) { return false; }
  if (!test_load_monst_jump_over_chasm()) { return false; }
  if (!test_load_monst_jump_over_lava()) { return false; }
  if (!test_load_monst_jumper()) { return false; }
  if (!test_load_monst_massacre()) { return false; }
  if (!test_load_monst_maze()) { return false; }
  if (!test_load_monst_mob_leash()) { return false; }
  if (!test_load_monst_mobbing()) { return false; }
  if (!test_load_monst_move_diagonal_barrel()) { return false; }
  if (!test_load_monst_move_diagonal_border()) { return false; }
  if (!test_load_monst_move_diagonal_brazier()) { return false; }
  if (!test_load_monst_move_diagonal_pillar()) { return false; }
  if (!test_load_monst_move_diagonal_rock()) { return false; }
  if (!test_load_monst_move_diagonal_rubble()) { return false; }
  if (!test_load_monst_move_diagonal_vault()) { return false; }
  if (!test_load_monst_move_diagonal_wall()) { return false; }
  if (!test_load_monst_pass_through_open_door()) { return false; }
  if (!test_load_monst_pass_through_walls()) { return false; }
  if (!test_load_monst_poison()) { return false; }
  if (!test_load_monst_resurrect_but_burns()) { return false; }
  if (!test_load_monst_resurrect_fire()) { return false; }
  if (!test_load_monst_resurrect()) { return false; }
  if (!test_load_monst_secret_door()) { return false; }
  if (!test_load_monst_short_jump_around_chasm()) { return false; }
  if (!test_load_monst_slow_attack_multi()) { return false; }
  if (!test_load_monst_slow_attack_single()) { return false; }
  if (!test_load_monst_slow()) { return false; }
  if (!test_load_monst_vision_direction()) { return false; }
  if (!test_load_monst_voider()) { return false; }
  if (!test_load_no_change()) { return false; }
  if (!test_load_path_astar()) { return false; }
  if (!test_load_path_dmap_reverse()) { return false; }
  if (!test_load_path_dmap()) { return false; }
  if (!test_load_player_engulfed_if_levitating()) { return false; }
  if (!test_load_player_engulfed_max_items()) { return false; }
  if (!test_load_player_engulfed_multiple()) { return false; }
  if (!test_load_player_engulfed_same_items()) { return false; }
  if (!test_load_player_engulfed_then_falls()) { return false; }
  if (!test_load_player_engulfed_then_jumps()) { return false; }
  if (!test_load_player_engulfed_then_teleports()) { return false; }
  if (!test_load_player_engulfed_treasure_chest()) { return false; }
  if (!test_load_player_engulfed()) { return false; }
  if (!test_load_player_fall_chasm_into_lava()) { return false; }
  if (!test_load_player_fall_chasm_twice_into_lava()) { return false; }
  if (!test_load_player_fall_chasm_twice()) { return false; }
  if (!test_load_player_fall_while_on_fire()) { return false; }
  if (!test_load_player_hiding_behind_foliage()) { return false; }
  if (!test_load_player_hiding_in_foliage()) { return false; }
  if (!test_load_player_in_steam()) { return false; }
  if (!test_load_player_lava()) { return false; }
  if (!test_load_player_melee()) { return false; }
  if (!test_load_player_move_diagonal_barrel()) { return false; }
  if (!test_load_player_move_diagonal_border()) { return false; }
  if (!test_load_player_move_diagonal_brazier()) { return false; }
  if (!test_load_player_move_diagonal_pillar()) { return false; }
  if (!test_load_player_move_diagonal_rock()) { return false; }
  if (!test_load_player_move_diagonal_vault()) { return false; }
  if (!test_load_player_move_diagonal_wall()) { return false; }
  if (!test_load_player_move_ok()) { return false; }
  if (!test_load_player_moving_in_foliage()) { return false; }
  if (!test_load_player_on_fire_move_into_water()) { return false; }
  if (!test_load_player_on_fire()) { return false; }
  if (!test_load_player_trap_did_not_trigger_due_to_clover()) { return false; }
  if (!test_load_player_trap_did_not_trigger_due_to_horseshoe()) { return false; }
  if (!test_load_player_trap_triggered()) { return false; }
  if (!test_load_pot_disloc_bad_luck()) { return false; }
  if (!test_load_pot_disloc_good_luck()) { return false; }
  if (!test_load_pot_healing()) { return false; }
  if (!test_load_pot_invis_player_monst_can_see()) { return false; }
  if (!test_load_pot_invis_player_monst_cannot_see_but_made_sound()) { return false; }
  if (!test_load_pot_invis_player_monst_cannot_see()) { return false; }
  if (!test_load_pot_levit_player_chasm()) { return false; }
  if (!test_load_pot_levit_player_grass()) { return false; }
  if (!test_load_pot_levit_player_in_steam()) { return false; }
  if (!test_load_pot_levit_player_lava()) { return false; }
  if (!test_load_pot_levit_player_move_fail()) { return false; }
  if (!test_load_pot_levit_player_move_fail2()) { return false; }
  if (!test_load_pot_levit_player_on_fire()) { return false; }
  if (!test_load_pot_levit_player_rubble()) { return false; }
  if (!test_load_pot_levit_player_spiderweb()) { return false; }
  if (!test_load_pot_levit_player_water()) { return false; }
  if (!test_load_pot_levit_trampled_grass()) { return false; }
  if (!test_load_pot_prot()) { return false; }
  if (!test_load_pot_stealth()) { return false; }
  if (!test_load_pot_tireless()) { return false; }
  if (!test_load_proj_barrel_explosion()) { return false; }
  if (!test_load_proj_border_turns_to_lava()) { return false; }
  if (!test_load_proj_door_locked()) { return false; }
  if (!test_load_proj_door_secret()) { return false; }
  if (!test_load_proj_door_unlocked()) { return false; }
  if (!test_load_proj_mob()) { return false; }
  if (!test_load_proj_over_grass()) { return false; }
  if (!test_load_proj_over_water()) { return false; }
  if (!test_load_proj_rock_turns_to_lava()) { return false; }
  if (!test_load_proj_spiderweb()) { return false; }
  if (!test_load_proj_teleport()) { return false; }
  if (!test_load_proj_wall_turns_to_lava()) { return false; }
  if (!test_load_projectile()) { return false; }
  if (!test_load_ring_life()) { return false; }
  if (!test_load_ring_war_beam_weapon()) { return false; }
  if (!test_load_ring_war_defence()) { return false; }
  if (!test_load_ring_war_melee()) { return false; }
  if (!test_load_ring_war_projectile()) { return false; }
  if (!test_load_sac_clumsy()) { return false; }
  if (!test_load_sac_noisy_pot_stealth()) { return false; }
  if (!test_load_sac_noisy()) { return false; }
  if (!test_load_sac_weaponless_attack_mob()) { return false; }
  if (!test_load_save_load()) { return false; }
  if (!test_load_slots()) { return false; }
  if (!test_load_spiderweb_monst_not_trapped_due_to_ethereal()) { return false; }
  if (!test_load_spiderweb_monst_not_trapped_due_to_levitation()) { return false; }
  if (!test_load_spiderweb_monst_trapped()) { return false; }
  if (!test_load_spiderweb_player_trapped()) { return false; }
  if (!test_load_teleport_chasm_bad_luck()) { return false; }
  if (!test_load_teleport_chasm()) { return false; }
  if (!test_load_teleport_lava()) { return false; }
  if (!test_load_teleport_shove_barrel()) { return false; }
  if (!test_load_teleport_shove_brazier()) { return false; }
  if (!test_load_teleport_shove_mob()) { return false; }
  if (!test_load_teleport()) { return false; }
  if (!test_load_throw_item_at_monst()) { return false; }
  if (!test_load_throw_item_chasm()) { return false; }
  if (!test_load_throw_item_pot_disloc()) { return false; }
  if (!test_load_throw_item_pot_healing_into_lava()) { return false; }
  if (!test_load_throw_item_pot_healing()) { return false; }
  if (!test_load_throw_item_pot_incin_chain_explosion()) { return false; }
  if (!test_load_throw_item_pot_incin_chasm()) { return false; }
  if (!test_load_throw_item_pot_incin_into_water()) { return false; }
  if (!test_load_throw_item_pot_incin()) { return false; }
  if (!test_load_throw_item_pot_invis_monst()) { return false; }
  if (!test_load_throw_item_pot_invis()) { return false; }
  if (!test_load_throw_item_pot_levit_hard_landing()) { return false; }
  if (!test_load_throw_item_pot_levit_monst_can_move()) { return false; }
  if (!test_load_throw_item_pot_levit_monst_cannot_move()) { return false; }
  if (!test_load_throw_item_pot_levit_monst_death()) { return false; }
  if (!test_load_throw_item_pot_levit_soft_landing()) { return false; }
  if (!test_load_throw_item_pot_levit()) { return false; }
  if (!test_load_throw_items()) { return false; }
  if (!test_load_trampled_grass()) { return false; }
  if (!test_load_wall()) { return false; }
  if (!test_load_weapon_discharge()) { return false; }
  if (!test_load_weapon_worn_then_stripped()) { return false; }
  /* end shell marker1 */
  // clang-format on

  return true;
}
