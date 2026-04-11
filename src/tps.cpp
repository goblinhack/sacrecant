//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_tps.hpp"

auto templates_init() -> bool
{
  TRACE();

  // clang-format off
  /* begin shell marker1 */
  /* shell for i in $(find things -name "*.cpp" | xargs grep -h "^auto tp_load" | sort | awk '{print $2}' | cut -d'(' -f1 ) */
  /* shell do */
  /* shell echo "  if (!$i()) { return false; }" */
  /* shell done */
  if (!tp_load_barrel()) { return false; }
  if (!tp_load_blitzhound()) { return false; }
  if (!tp_load_border()) { return false; }
  if (!tp_load_brazier()) { return false; }
  if (!tp_load_bridge()) { return false; }
  if (!tp_load_chasm()) { return false; }
  if (!tp_load_chest()) { return false; }
  if (!tp_load_corridor()) { return false; }
  if (!tp_load_cursor_at()) { return false; }
  if (!tp_load_cursor_path()) { return false; }
  if (!tp_load_deep_water()) { return false; }
  if (!tp_load_dirt()) { return false; }
  if (!tp_load_door_locked()) { return false; }
  if (!tp_load_door_secret()) { return false; }
  if (!tp_load_door_unlocked()) { return false; }
  if (!tp_load_effect_attack()) { return false; }
  if (!tp_load_effect_blood()) { return false; }
  if (!tp_load_entrance()) { return false; }
  if (!tp_load_exit()) { return false; }
  if (!tp_load_explosion()) { return false; }
  if (!tp_load_fire()) { return false; }
  if (!tp_load_fireball()) { return false; }
  if (!tp_load_floor()) { return false; }
  if (!tp_load_foliage()) { return false; }
  if (!tp_load_ghost_mob()) { return false; }
  if (!tp_load_ghost()) { return false; }
  if (!tp_load_glorp()) { return false; }
  if (!tp_load_grass()) { return false; }
  if (!tp_load_key()) { return false; }
  if (!tp_load_kobalos_mob()) { return false; }
  if (!tp_load_kobalos()) { return false; }
  if (!tp_load_lava()) { return false; }
  if (!tp_load_level_closed()) { return false; }
  if (!tp_load_level_curr()) { return false; }
  if (!tp_load_level_final()) { return false; }
  if (!tp_load_level_locked()) { return false; }
  if (!tp_load_level_next()) { return false; }
  if (!tp_load_level_open()) { return false; }
  if (!tp_load_level_select_bg()) { return false; }
  if (!tp_load_mantisman()) { return false; }
  if (!tp_load_pillar()) { return false; }
  if (!tp_load_player()) { return false; }
  if (!tp_load_potion()) { return false; }
  if (!tp_load_rock()) { return false; }
  if (!tp_load_smoke()) { return false; }
  if (!tp_load_steam()) { return false; }
  if (!tp_load_teleport()) { return false; }
  if (!tp_load_trap()) { return false; }
  if (!tp_load_vault()) { return false; }
  if (!tp_load_wall()) { return false; }
  if (!tp_load_water()) { return false; }
  /* end shell marker1 */
  // clang-format on

  return true;
}
