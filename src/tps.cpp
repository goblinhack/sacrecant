//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_tps.hpp"

[[nodiscard]] auto templates_init() -> bool
{
  TRACE();

  // clang-format off
  /* begin shell marker1 */
  /* shell for i in $(find things -name "*.cpp" | xargs grep -h "^..nodiscard.. auto tp_load" | sort | awk '{print $3}' | cut -d'(' -f1 ) */
  /* shell do */
  /* shell echo "  if (!$i()) { return false; }" */
  /* shell done */
  if (!tp_load_argusul()) { return false; }
  if (!tp_load_barrel()) { return false; }
  if (!tp_load_beam_of_energy()) { return false; }
  if (!tp_load_beam_of_fire()) { return false; }
  if (!tp_load_blitzhound()) { return false; }
  if (!tp_load_boost_devoted_thrust()) { return false; }
  if (!tp_load_boots_fire()) { return false; }
  if (!tp_load_boots_spider()) { return false; }
  if (!tp_load_border()) { return false; }
  if (!tp_load_brazier()) { return false; }
  if (!tp_load_bridge()) { return false; }
  if (!tp_load_buff_bad_luck()) { return false; }
  if (!tp_load_buff_good_luck()) { return false; }
  if (!tp_load_buff_immune_fire()) { return false; }
  if (!tp_load_buff_invis()) { return false; }
  if (!tp_load_buff_levit()) { return false; }
  if (!tp_load_buff_poison()) { return false; }
  if (!tp_load_buff_prot()) { return false; }
  if (!tp_load_buff_resistant_fire()) { return false; }
  if (!tp_load_buff_stealth()) { return false; }
  if (!tp_load_buff_tireless()) { return false; }
  if (!tp_load_chasm()) { return false; }
  if (!tp_load_chest()) { return false; }
  if (!tp_load_chocolate_frog()) { return false; }
  if (!tp_load_cleaner()) { return false; }
  if (!tp_load_clover()) { return false; }
  if (!tp_load_clown_meat()) { return false; }
  if (!tp_load_coil_eel()) { return false; }
  if (!tp_load_copious_chest()) { return false; }
  if (!tp_load_corridor()) { return false; }
  if (!tp_load_cursor_at()) { return false; }
  if (!tp_load_cursor_path()) { return false; }
  if (!tp_load_dirt()) { return false; }
  if (!tp_load_door_locked()) { return false; }
  if (!tp_load_door_secret()) { return false; }
  if (!tp_load_door_unlocked()) { return false; }
  if (!tp_load_effect_attack()) { return false; }
  if (!tp_load_effect_blood()) { return false; }
  if (!tp_load_effect_ripple()) { return false; }
  if (!tp_load_entrance()) { return false; }
  if (!tp_load_exit()) { return false; }
  if (!tp_load_explosion_major()) { return false; }
  if (!tp_load_explosion_minor()) { return false; }
  if (!tp_load_fire()) { return false; }
  if (!tp_load_floor()) { return false; }
  if (!tp_load_foliage()) { return false; }
  if (!tp_load_ghost_mob()) { return false; }
  if (!tp_load_ghost()) { return false; }
  if (!tp_load_glorp()) { return false; }
  if (!tp_load_grass()) { return false; }
  if (!tp_load_horseshoe()) { return false; }
  if (!tp_load_key()) { return false; }
  if (!tp_load_kobalos_mob()) { return false; }
  if (!tp_load_kobalos()) { return false; }
  if (!tp_load_lava_bg()) { return false; }
  if (!tp_load_lava()) { return false; }
  if (!tp_load_level_closed()) { return false; }
  if (!tp_load_level_curr()) { return false; }
  if (!tp_load_level_final()) { return false; }
  if (!tp_load_level_locked()) { return false; }
  if (!tp_load_level_next()) { return false; }
  if (!tp_load_level_open()) { return false; }
  if (!tp_load_level_select_bg()) { return false; }
  if (!tp_load_mantisman()) { return false; }
  if (!tp_load_mummy()) { return false; }
  if (!tp_load_ogrik()) { return false; }
  if (!tp_load_pale_eel()) { return false; }
  if (!tp_load_pillar()) { return false; }
  if (!tp_load_pirnana()) { return false; }
  if (!tp_load_player1()) { return false; }
  if (!tp_load_player2()) { return false; }
  if (!tp_load_player3()) { return false; }
  if (!tp_load_player4()) { return false; }
  if (!tp_load_player5()) { return false; }
  if (!tp_load_pot_disloc()) { return false; }
  if (!tp_load_pot_healing()) { return false; }
  if (!tp_load_pot_incin()) { return false; }
  if (!tp_load_pot_invis()) { return false; }
  if (!tp_load_pot_levit()) { return false; }
  if (!tp_load_pot_prot()) { return false; }
  if (!tp_load_pot_stealth()) { return false; }
  if (!tp_load_pot_tireless()) { return false; }
  if (!tp_load_proj_energy()) { return false; }
  if (!tp_load_proj_fire()) { return false; }
  if (!tp_load_reeds()) { return false; }
  if (!tp_load_ring_life()) { return false; }
  if (!tp_load_ring_war()) { return false; }
  if (!tp_load_rock()) { return false; }
  if (!tp_load_rubble()) { return false; }
  if (!tp_load_sac_blood_pact()) { return false; }
  if (!tp_load_sac_clumsy()) { return false; }
  if (!tp_load_sac_crit_more()) { return false; }
  if (!tp_load_sac_defenceless()) { return false; }
  if (!tp_load_sac_fumble_more()) { return false; }
  if (!tp_load_sac_glass_bones()) { return false; }
  if (!tp_load_sac_healing_drain()) { return false; }
  if (!tp_load_sac_hollow_bones()) { return false; }
  if (!tp_load_sac_noisy()) { return false; }
  if (!tp_load_sac_poison_blood()) { return false; }
  if (!tp_load_sac_poor_vision()) { return false; }
  if (!tp_load_sac_power_crazed()) { return false; }
  if (!tp_load_sac_sickly_health()) { return false; }
  if (!tp_load_sac_soul_feast()) { return false; }
  if (!tp_load_sac_tunnel_vision()) { return false; }
  if (!tp_load_sac_unlucky()) { return false; }
  if (!tp_load_sac_wall_walker()) { return false; }
  if (!tp_load_sac_weaponless()) { return false; }
  if (!tp_load_sac_wooden_leg()) { return false; }
  if (!tp_load_skeleton_mob()) { return false; }
  if (!tp_load_skeleton()) { return false; }
  if (!tp_load_skullferno()) { return false; }
  if (!tp_load_smoke()) { return false; }
  if (!tp_load_spell_10()) { return false; }
  if (!tp_load_spell_11()) { return false; }
  if (!tp_load_spell_12()) { return false; }
  if (!tp_load_spell_13()) { return false; }
  if (!tp_load_spell_14()) { return false; }
  if (!tp_load_spell_15()) { return false; }
  if (!tp_load_spell_16()) { return false; }
  if (!tp_load_spell_17()) { return false; }
  if (!tp_load_spell_18()) { return false; }
  if (!tp_load_spell_19()) { return false; }
  if (!tp_load_spell_2()) { return false; }
  if (!tp_load_spell_20()) { return false; }
  if (!tp_load_spell_21()) { return false; }
  if (!tp_load_spell_22()) { return false; }
  if (!tp_load_spell_23()) { return false; }
  if (!tp_load_spell_24()) { return false; }
  if (!tp_load_spell_25()) { return false; }
  if (!tp_load_spell_26()) { return false; }
  if (!tp_load_spell_27()) { return false; }
  if (!tp_load_spell_28()) { return false; }
  if (!tp_load_spell_29()) { return false; }
  if (!tp_load_spell_3()) { return false; }
  if (!tp_load_spell_30()) { return false; }
  if (!tp_load_spell_31()) { return false; }
  if (!tp_load_spell_32()) { return false; }
  if (!tp_load_spell_33()) { return false; }
  if (!tp_load_spell_34()) { return false; }
  if (!tp_load_spell_35()) { return false; }
  if (!tp_load_spell_36()) { return false; }
  if (!tp_load_spell_37()) { return false; }
  if (!tp_load_spell_38()) { return false; }
  if (!tp_load_spell_39()) { return false; }
  if (!tp_load_spell_4()) { return false; }
  if (!tp_load_spell_40()) { return false; }
  if (!tp_load_spell_41()) { return false; }
  if (!tp_load_spell_42()) { return false; }
  if (!tp_load_spell_43()) { return false; }
  if (!tp_load_spell_44()) { return false; }
  if (!tp_load_spell_45()) { return false; }
  if (!tp_load_spell_46()) { return false; }
  if (!tp_load_spell_47()) { return false; }
  if (!tp_load_spell_48()) { return false; }
  if (!tp_load_spell_49()) { return false; }
  if (!tp_load_spell_5()) { return false; }
  if (!tp_load_spell_50()) { return false; }
  if (!tp_load_spell_51()) { return false; }
  if (!tp_load_spell_52()) { return false; }
  if (!tp_load_spell_6()) { return false; }
  if (!tp_load_spell_7()) { return false; }
  if (!tp_load_spell_8()) { return false; }
  if (!tp_load_spell_9()) { return false; }
  if (!tp_load_spell_effect()) { return false; }
  if (!tp_load_spell_firestorm()) { return false; }
  if (!tp_load_spider_baby()) { return false; }
  if (!tp_load_spider()) { return false; }
  if (!tp_load_spiderweb()) { return false; }
  if (!tp_load_staff_energy()) { return false; }
  if (!tp_load_staff_fire()) { return false; }
  if (!tp_load_steam()) { return false; }
  if (!tp_load_teleport()) { return false; }
  if (!tp_load_trap()) { return false; }
  if (!tp_load_vault()) { return false; }
  if (!tp_load_voider()) { return false; }
  if (!tp_load_wall()) { return false; }
  if (!tp_load_wand_energy()) { return false; }
  if (!tp_load_wand_fire()) { return false; }
  if (!tp_load_water_deep()) { return false; }
  if (!tp_load_water()) { return false; }
  /* end shell marker1 */
  // clang-format on

  return true;
}
