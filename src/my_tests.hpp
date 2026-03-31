//
// Copyright goblinhack@gmail.com
//

#ifndef MY_TESTS_HPP
#define MY_TESTS_HPP

auto tests_init() -> bool;

// clang-format off
/* begin shell marker1 */
/* shell for i in $(find tests -name "*.cpp" | xargs grep -h "^auto test_load" | sort | awk '{print $2}' | cut -d'(' -f1 ) */
/* shell do */
/* shell echo "auto $i() -> bool;" */
/* shell done */
auto test_load_alloc_free_monsts() -> bool;
auto test_load_alloc_free_walls() -> bool;
auto test_load_barrel_explosion() -> bool;
auto test_load_barrel() -> bool;
auto test_load_brazier_shove_chasm_twice() -> bool;
auto test_load_brazier_shove_chasm() -> bool;
auto test_load_brazier_shove_fail() -> bool;
auto test_load_brazier_shove_into_mob() -> bool;
auto test_load_brazier_shove_ok() -> bool;
auto test_load_bridge_on_fire_over_lava() -> bool;
auto test_load_bridge_on_fire() -> bool;
auto test_load_fire_on_water() -> bool;
auto test_load_fire_over_chasm() -> bool;
auto test_load_foliage_on_fire() -> bool;
auto test_load_jump_ok() -> bool;
auto test_load_jump_truncated() -> bool;
auto test_load_large_fire_water() -> bool;
auto test_load_mob_shove_into_lava() -> bool;
auto test_load_mob_water() -> bool;
auto test_load_mob() -> bool;
auto test_load_monst_avoids_chasm() -> bool;
auto test_load_monst_avoids_lava() -> bool;
auto test_load_monst_avoids_water_usually() -> bool;
auto test_load_monst_avoids_water() -> bool;
auto test_load_monst_crosses_chasm() -> bool;
auto test_load_monst_crosses_lava() -> bool;
auto test_load_monst_crosses_water() -> bool;
auto test_load_monst_door_locked() -> bool;
auto test_load_monst_door_unlocked() -> bool;
auto test_load_monst_fast() -> bool;
auto test_load_monst_hiding_behind_bush() -> bool;
auto test_load_monst_hiding_in_bush() -> bool;
auto test_load_monst_jump_over_chasm() -> bool;
auto test_load_monst_maze() -> bool;
auto test_load_monst_mob_leash() -> bool;
auto test_load_monst_mobbing() -> bool;
auto test_load_monst_pass_through_open_door() -> bool;
auto test_load_monst_pass_through_walls() -> bool;
auto test_load_monst_secret_door() -> bool;
auto test_load_monst_short_jump_around_chasm() -> bool;
auto test_load_monst_slow() -> bool;
auto test_load_move_ok() -> bool;
auto test_load_no_change() -> bool;
auto test_load_path_astar() -> bool;
auto test_load_path_dmap_reverse() -> bool;
auto test_load_path_dmap() -> bool;
auto test_load_player_fall_chasm_into_lava() -> bool;
auto test_load_player_fall_chasm_twice_into_lava() -> bool;
auto test_load_player_fall_chasm_twice() -> bool;
auto test_load_player_fall_while_on_fire() -> bool;
auto test_load_player_in_steam() -> bool;
auto test_load_player_lava() -> bool;
auto test_load_player_on_fire_move_into_water() -> bool;
auto test_load_player_on_fire() -> bool;
auto test_load_projectile_barrel_explosion() -> bool;
auto test_load_projectile_border_turns_to_lava() -> bool;
auto test_load_projectile_door_locked() -> bool;
auto test_load_projectile_door_secret() -> bool;
auto test_load_projectile_door_unlocked() -> bool;
auto test_load_projectile_mob() -> bool;
auto test_load_projectile_over_water() -> bool;
auto test_load_projectile_rock_turns_to_lava() -> bool;
auto test_load_projectile_wall_turns_to_lava() -> bool;
auto test_load_projectile() -> bool;
auto test_load_save_load() -> bool;
auto test_load_slots() -> bool;
auto test_load_teleport_chasm() -> bool;
auto test_load_teleport_shove_barrel() -> bool;
auto test_load_teleport_shove_brazier() -> bool;
auto test_load_teleport_shove_mob() -> bool;
auto test_load_teleport() -> bool;
auto test_load_trampled_grass() -> bool;
auto test_load_wall() -> bool;
  /* end shell marker1 */
#endif // MY_TESTS_HPP
