//
// Copyright goblinhack@gmail.com
//

#ifndef MY_TPS_HPP
#define MY_TPS_HPP

[[nodiscard]] auto templates_init() -> bool;

// clang-format off
/* begin shell marker1 */
/* shell for i in $(find things -name "*.cpp" | xargs grep -h "^..nodiscard.. auto tp_load" | sort | awk '{print $3}' | cut -d'(' -f1 ) */
/* shell do */
/* shell echo "auto $i() -> bool;" */
/* shell done */
auto tp_load_argusul() -> bool;
auto tp_load_barrel() -> bool;
auto tp_load_beam_of_energy() -> bool;
auto tp_load_beam_of_fire() -> bool;
auto tp_load_blitzhound() -> bool;
auto tp_load_boost_devoted_thrust() -> bool;
auto tp_load_boots_fire() -> bool;
auto tp_load_boots_spider() -> bool;
auto tp_load_border() -> bool;
auto tp_load_brazier() -> bool;
auto tp_load_bridge() -> bool;
auto tp_load_buff_bad_luck() -> bool;
auto tp_load_buff_good_luck() -> bool;
auto tp_load_buff_immune_fire() -> bool;
auto tp_load_buff_invis() -> bool;
auto tp_load_buff_levit() -> bool;
auto tp_load_buff_poison() -> bool;
auto tp_load_buff_prot() -> bool;
auto tp_load_buff_resistant_fire() -> bool;
auto tp_load_buff_stealth() -> bool;
auto tp_load_buff_tireless() -> bool;
auto tp_load_chasm() -> bool;
auto tp_load_chest() -> bool;
auto tp_load_chocolate_frog() -> bool;
auto tp_load_cleaner() -> bool;
auto tp_load_clover() -> bool;
auto tp_load_clown_meat() -> bool;
auto tp_load_coil_eel() -> bool;
auto tp_load_copious_chest() -> bool;
auto tp_load_corridor() -> bool;
auto tp_load_cursor_at() -> bool;
auto tp_load_cursor_path() -> bool;
auto tp_load_dirt() -> bool;
auto tp_load_door_locked() -> bool;
auto tp_load_door_secret() -> bool;
auto tp_load_door_unlocked() -> bool;
auto tp_load_effect_attack() -> bool;
auto tp_load_effect_blood() -> bool;
auto tp_load_effect_ripple() -> bool;
auto tp_load_entrance() -> bool;
auto tp_load_exit() -> bool;
auto tp_load_explosion() -> bool;
auto tp_load_fire() -> bool;
auto tp_load_floor() -> bool;
auto tp_load_foliage() -> bool;
auto tp_load_ghost_mob() -> bool;
auto tp_load_ghost() -> bool;
auto tp_load_glorp() -> bool;
auto tp_load_grass() -> bool;
auto tp_load_horseshoe() -> bool;
auto tp_load_key() -> bool;
auto tp_load_kobalos_mob() -> bool;
auto tp_load_kobalos() -> bool;
auto tp_load_lava_bg() -> bool;
auto tp_load_lava() -> bool;
auto tp_load_level_closed() -> bool;
auto tp_load_level_curr() -> bool;
auto tp_load_level_final() -> bool;
auto tp_load_level_locked() -> bool;
auto tp_load_level_next() -> bool;
auto tp_load_level_open() -> bool;
auto tp_load_level_select_bg() -> bool;
auto tp_load_mantisman() -> bool;
auto tp_load_mummy() -> bool;
auto tp_load_ogrik() -> bool;
auto tp_load_pale_eel() -> bool;
auto tp_load_pillar() -> bool;
auto tp_load_pirnana() -> bool;
auto tp_load_player1() -> bool;
auto tp_load_player2() -> bool;
auto tp_load_player3() -> bool;
auto tp_load_player4() -> bool;
auto tp_load_player5() -> bool;
auto tp_load_pot_disloc() -> bool;
auto tp_load_pot_healing() -> bool;
auto tp_load_pot_incin() -> bool;
auto tp_load_pot_invis() -> bool;
auto tp_load_pot_levit() -> bool;
auto tp_load_pot_prot() -> bool;
auto tp_load_pot_stealth() -> bool;
auto tp_load_pot_tireless() -> bool;
auto tp_load_proj_energy() -> bool;
auto tp_load_proj_fire() -> bool;
auto tp_load_reeds() -> bool;
auto tp_load_ring_life() -> bool;
auto tp_load_ring_war() -> bool;
auto tp_load_rock() -> bool;
auto tp_load_rubble() -> bool;
auto tp_load_sac_blood_pact() -> bool;
auto tp_load_sac_clumsy() -> bool;
auto tp_load_sac_defenceless() -> bool;
auto tp_load_sac_fumble_more() -> bool;
auto tp_load_sac_healing_drain() -> bool;
auto tp_load_sac_noisy() -> bool;
auto tp_load_sac_poison_blood() -> bool;
auto tp_load_sac_poor_vision() -> bool;
auto tp_load_sac_sickly_health() -> bool;
auto tp_load_sac_tunnel_vision() -> bool;
auto tp_load_sac_unlucky() -> bool;
auto tp_load_sac_weaponless() -> bool;
auto tp_load_skeleton_mob() -> bool;
auto tp_load_skeleton() -> bool;
auto tp_load_skullferno() -> bool;
auto tp_load_smoke() -> bool;
auto tp_load_spider_baby() -> bool;
auto tp_load_spider() -> bool;
auto tp_load_spiderweb() -> bool;
auto tp_load_staff_energy() -> bool;
auto tp_load_staff_fire() -> bool;
auto tp_load_steam() -> bool;
auto tp_load_teleport() -> bool;
auto tp_load_trap() -> bool;
auto tp_load_vault() -> bool;
auto tp_load_voider() -> bool;
auto tp_load_wall() -> bool;
auto tp_load_wand_energy() -> bool;
auto tp_load_wand_fire() -> bool;
auto tp_load_water_deep() -> bool;
auto tp_load_water() -> bool;
  /* end shell marker1 */

#endif
