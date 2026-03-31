//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_tests.hpp"

auto tests_init() -> bool
{
  TRACE();

  // clang-format off
  /* begin shell marker1 */
  /* shell for i in $(find tests -name "*.cpp" | xargs grep -h "^auto test_load" | sort | awk '{print $2}' | cut -d'(' -f1 ) */
  /* shell do */
  /* shell echo "  if (!$i()) { return false; }" */
  /* shell done */
  if (!test_load_alloc_free_monsts()) { return false; }
  if (!test_load_alloc_free_walls()) { return false; }
  if (!test_load_barrel_explosion()) { return false; }
  if (!test_load_barrel()) { return false; }
  if (!test_load_brazier_shove_chasm_twice()) { return false; }
  if (!test_load_brazier_shove_chasm()) { return false; }
  if (!test_load_brazier_shove_fail()) { return false; }
  if (!test_load_brazier_shove_into_mob()) { return false; }
  if (!test_load_brazier_shove_ok()) { return false; }
  if (!test_load_bridge_on_fire_over_lava()) { return false; }
  if (!test_load_bridge_on_fire()) { return false; }
  if (!test_load_fire_on_water()) { return false; }
  if (!test_load_fire_over_chasm()) { return false; }
  if (!test_load_foliage_on_fire()) { return false; }
  if (!test_load_jump_ok()) { return false; }
  if (!test_load_jump_truncated()) { return false; }
  if (!test_load_large_fire_water()) { return false; }
  if (!test_load_mob_shove_into_lava()) { return false; }
  if (!test_load_mob_water()) { return false; }
  if (!test_load_mob()) { return false; }
  if (!test_load_monst_avoids_chasm()) { return false; }
  if (!test_load_monst_avoids_lava()) { return false; }
  if (!test_load_monst_avoids_water_usually()) { return false; }
  if (!test_load_monst_avoids_water()) { return false; }
  if (!test_load_monst_crosses_chasm()) { return false; }
  if (!test_load_monst_crosses_lava()) { return false; }
  if (!test_load_monst_crosses_water()) { return false; }
  if (!test_load_monst_door_locked()) { return false; }
  if (!test_load_monst_door_unlocked()) { return false; }
  if (!test_load_monst_fast()) { return false; }
  if (!test_load_monst_hiding_behind_bush()) { return false; }
  if (!test_load_monst_hiding_in_bush()) { return false; }
  if (!test_load_monst_jump_over_chasm()) { return false; }
  if (!test_load_monst_maze()) { return false; }
  if (!test_load_monst_mob_leash()) { return false; }
  if (!test_load_monst_mobbing()) { return false; }
  if (!test_load_monst_pass_through_open_door()) { return false; }
  if (!test_load_monst_pass_through_walls()) { return false; }
  if (!test_load_monst_secret_door()) { return false; }
  if (!test_load_monst_short_jump_around_chasm()) { return false; }
  if (!test_load_monst_slow()) { return false; }
  if (!test_load_move_ok()) { return false; }
  if (!test_load_no_change()) { return false; }
  if (!test_load_path_astar()) { return false; }
  if (!test_load_path_dmap_reverse()) { return false; }
  if (!test_load_path_dmap()) { return false; }
  if (!test_load_player_fall_chasm_into_lava()) { return false; }
  if (!test_load_player_fall_chasm_twice_into_lava()) { return false; }
  if (!test_load_player_fall_chasm_twice()) { return false; }
  if (!test_load_player_fall_while_on_fire()) { return false; }
  if (!test_load_player_in_steam()) { return false; }
  if (!test_load_player_lava()) { return false; }
  if (!test_load_player_on_fire_move_into_water()) { return false; }
  if (!test_load_player_on_fire()) { return false; }
  if (!test_load_projectile_barrel_explosion()) { return false; }
  if (!test_load_projectile_border_turns_to_lava()) { return false; }
  if (!test_load_projectile_door_locked()) { return false; }
  if (!test_load_projectile_door_secret()) { return false; }
  if (!test_load_projectile_door_unlocked()) { return false; }
  if (!test_load_projectile_mob()) { return false; }
  if (!test_load_projectile_over_water()) { return false; }
  if (!test_load_projectile_rock_turns_to_lava()) { return false; }
  if (!test_load_projectile_wall_turns_to_lava()) { return false; }
  if (!test_load_projectile()) { return false; }
  if (!test_load_save_load()) { return false; }
  if (!test_load_slots()) { return false; }
  if (!test_load_teleport_chasm()) { return false; }
  if (!test_load_teleport_shove_barrel()) { return false; }
  if (!test_load_teleport_shove_brazier()) { return false; }
  if (!test_load_teleport_shove_mob()) { return false; }
  if (!test_load_teleport()) { return false; }
  if (!test_load_trampled_grass()) { return false; }
  if (!test_load_wall()) { return false; }
  /* end shell marker1 */
  // clang-format on

  return true;
}
