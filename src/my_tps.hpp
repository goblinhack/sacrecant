//
// Copyright goblinhack@gmail.com
//

#ifndef MY_TPS_HPP
#define MY_TPS_HPP

auto templates_init() -> bool;

// clang-format off
/* begin shell marker1 */
/* shell for i in $(find things -name "*.cpp" | xargs grep -h "^auto tp_load" | sort | awk '{print $2}' | cut -d'(' -f1 ) */
/* shell do */
/* shell echo "auto $i() -> bool;" */
/* shell done */
auto tp_load_barrel() -> bool;
auto tp_load_blitzhound() -> bool;
auto tp_load_border() -> bool;
auto tp_load_brazier() -> bool;
auto tp_load_bridge() -> bool;
auto tp_load_chasm() -> bool;
auto tp_load_chest() -> bool;
auto tp_load_corridor() -> bool;
auto tp_load_cursor_at() -> bool;
auto tp_load_cursor_path() -> bool;
auto tp_load_deep_water() -> bool;
auto tp_load_dirt() -> bool;
auto tp_load_door_locked() -> bool;
auto tp_load_door_secret() -> bool;
auto tp_load_door_unlocked() -> bool;
auto tp_load_effect_attack() -> bool;
auto tp_load_effect_blood() -> bool;
auto tp_load_entrance() -> bool;
auto tp_load_exit() -> bool;
auto tp_load_explosion() -> bool;
auto tp_load_fire() -> bool;
auto tp_load_fireball() -> bool;
auto tp_load_floor() -> bool;
auto tp_load_foliage() -> bool;
auto tp_load_ghost_mob() -> bool;
auto tp_load_ghost() -> bool;
auto tp_load_glorp() -> bool;
auto tp_load_grass() -> bool;
auto tp_load_key() -> bool;
auto tp_load_kobalos_mob() -> bool;
auto tp_load_kobalos() -> bool;
auto tp_load_lava() -> bool;
auto tp_load_level_closed() -> bool;
auto tp_load_level_curr() -> bool;
auto tp_load_level_final() -> bool;
auto tp_load_level_locked() -> bool;
auto tp_load_level_next() -> bool;
auto tp_load_level_open() -> bool;
auto tp_load_level_select_bg() -> bool;
auto tp_load_mantisman() -> bool;
auto tp_load_pillar() -> bool;
auto tp_load_player() -> bool;
auto tp_load_potion() -> bool;
auto tp_load_rock() -> bool;
auto tp_load_smoke() -> bool;
auto tp_load_steam() -> bool;
auto tp_load_teleport() -> bool;
auto tp_load_trap() -> bool;
auto tp_load_wall() -> bool;
auto tp_load_water() -> bool;
  /* end shell marker1 */

#endif
