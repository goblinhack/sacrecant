//
// Copyright goblinhack@gmail.com
//

#include "my_charmap.hpp"
#include "my_globals.hpp"
#include "my_level.hpp"
#include "my_main.hpp"
#include "my_test.hpp"
#include "my_thing_inlines.hpp"
#include "my_ui.hpp"
#include "my_wids.hpp"

auto tp_is_able_to_collect_items(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_able_to_collect_items) != 0;
}

auto tp_is_able_to_collect_keys(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_able_to_collect_keys) != 0;
}

auto tp_is_able_to_crush_grass(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_able_to_crush_grass) != 0;
}

auto tp_is_able_to_fall_repeatedly(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_able_to_fall_repeatedly) != 0;
}

auto tp_is_able_to_fall(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_able_to_fall) != 0;
}

auto tp_is_able_to_jump(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_able_to_jump) != 0;
}

auto tp_is_able_to_move_diagonally(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_able_to_move_diagonally) != 0;
}

auto tp_is_able_to_move_through_walls(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_able_to_move_through_walls) != 0;
}

auto tp_is_able_to_open(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_able_to_open) != 0;
}

auto tp_is_able_to_shove(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_able_to_shove) != 0;
}

auto tp_is_able_to_walk_through_walls(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_able_to_walk_through_walls) != 0;
}

auto tp_is_animated_can_hflip(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_animated_can_hflip) != 0;
}

auto tp_is_animated_no_dir(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_animated_no_dir) != 0;
}

auto tp_is_animated_sync_first(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_animated_sync_first) != 0;
}

auto tp_is_animated(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_animated) != 0;
}

auto tp_is_barrel(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_barrel) != 0;
}

auto tp_is_blit_centered(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_blit_centered) != 0;
}

auto tp_is_blit_flush_per_line(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_blit_flush_per_line) != 0;
}

auto tp_is_blit_if_has_seen(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_blit_if_has_seen) != 0;
}

auto tp_is_blit_obscures(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_blit_obscures) != 0;
}

auto tp_is_blit_on_ground(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_blit_on_ground) != 0;
}

auto tp_is_blit_outlined(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_blit_outlined) != 0;
}

auto tp_is_blit_per_pixel_lighting(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_blit_per_pixel_lighting) != 0;
}

auto tp_is_blit_shown_in_chasms(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_blit_shown_in_chasms) != 0;
}

auto tp_is_blit_shown_in_overlay(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_blit_shown_in_overlay) != 0;
}

auto tp_is_blit_square_outlined(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_blit_square_outlined) != 0;
}

auto tp_is_blit_when_obscured_faded(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_blit_when_obscured_faded) != 0;
}

auto tp_is_border(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_border) != 0;
}

auto tp_is_brazier(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_brazier) != 0;
}

auto tp_is_bridge(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_bridge) != 0;
}

auto tp_is_broken_on_death(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_broken_on_death) != 0;
}

auto tp_is_burnable(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_burnable) != 0;
}

auto tp_is_chasm(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_chasm) != 0;
}

auto tp_is_collectable(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_collectable) != 0;
}

auto tp_is_collision_circle_large(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_collision_circle_large) != 0;
}

auto tp_is_collision_circle_small(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_collision_circle_small) != 0;
}

auto tp_is_collision_detection_enabled(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_collision_detection_enabled) != 0;
}

auto tp_is_collision_square(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_collision_square) != 0;
}

auto tp_is_combustible(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_combustible) != 0;
}

auto tp_is_corpse_on_death(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_corpse_on_death) != 0;
}

auto tp_is_corridor(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_corridor) != 0;
}

auto tp_is_crushable(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_crushable) != 0;
}

auto tp_is_cursor_path_hazard(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_cursor_path_hazard) != 0;
}

auto tp_is_cursor_path_none(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_cursor_path_none) != 0;
}

auto tp_is_cursor_path_warning(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_cursor_path_warning) != 0;
}

auto tp_is_cursor_path(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_cursor_path) != 0;
}

auto tp_is_cursor(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_cursor) != 0;
}

auto tp_is_damage_capped(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_damage_capped) != 0;
}

auto tp_is_dead_on_collision(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_dead_on_collision) != 0;
}

auto tp_is_dead_on_shoving(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_dead_on_shoving) != 0;
}

auto tp_is_deep_water(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_deep_water) != 0;
}

auto tp_is_described_cursor(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_described_cursor) != 0;
}

auto tp_is_dirt(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_dirt) != 0;
}

auto tp_is_dmap(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_dmap) != 0;
}

auto tp_is_door_locked(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_door_locked) != 0;
}

auto tp_is_door_secret(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_door_secret) != 0;
}

auto tp_is_door_unlocked(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_door_unlocked) != 0;
}

auto tp_is_dungeon_entrance(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_dungeon_entrance) != 0;
}

auto tp_is_entrance(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_entrance) != 0;
}

auto tp_is_ethereal(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_ethereal) != 0;
}

auto tp_is_exit(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_exit) != 0;
}

auto tp_is_explosion(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_explosion) != 0;
}

auto tp_is_extinguished_on_death(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_extinguished_on_death) != 0;
}

auto tp_is_fire(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_fire) != 0;
}

auto tp_is_fireball(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_fireball) != 0;
}

auto tp_is_flesh(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_flesh) != 0;
}

auto tp_is_floating(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_floating) != 0;
}

auto tp_is_floor(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_floor) != 0;
}

auto tp_is_flying(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_flying) != 0;
}

auto tp_is_foliage(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_foliage) != 0;
}

auto tp_is_gaseous(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_gaseous) != 0;
}

auto tp_is_ghost(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_ghost) != 0;
}

auto tp_is_glass(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_glass) != 0;
}

auto tp_is_gold(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_gold) != 0;
}

auto tp_is_grass(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_grass) != 0;
}

auto tp_is_health_bar_shown(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_health_bar_shown) != 0;
}

auto tp_is_indestructible(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_indestructible) != 0;
}

auto tp_is_insectoid(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_insectoid) != 0;
}

auto tp_is_inventory_item(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_inventory_item) != 0;
}

auto tp_is_item_droppable(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_item_droppable) != 0;
}

auto tp_is_item_equipable(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_item_equipable) != 0;
}

auto tp_is_item_mergeable(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_item_mergeable) != 0;
}

auto tp_is_item(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_item) != 0;
}

auto tp_is_key(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_key) != 0;
}

auto tp_is_kobalos(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_kobalos) != 0;
}

auto tp_is_lava(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_lava) != 0;
}

auto tp_is_unused46(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused46) != 0;
}

auto tp_is_level_curr(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_level_curr) != 0;
}

auto tp_is_unused47(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused47) != 0;
}

auto tp_is_level_final_icon(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_level_final_icon) != 0;
}

auto tp_is_level_next_icon(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_level_next_icon) != 0;
}

auto tp_is_level_locked_icon(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_level_locked_icon) != 0;
}

auto tp_is_level_closed_icon(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_level_closed_icon) != 0;
}

auto tp_is_levitating(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_levitating) != 0;
}

auto tp_is_light_source(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_light_source) != 0;
}

auto tp_is_loggable(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_loggable) != 0;
}

auto tp_is_mantisman(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_mantisman) != 0;
}

auto tp_is_meltable(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_meltable) != 0;
}

auto tp_is_metal(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_metal) != 0;
}

auto tp_is_minion(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_minion) != 0;
}

auto tp_is_mob_kill_minions_on_death(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_mob_kill_minions_on_death) != 0;
}

auto tp_is_mob(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_mob) != 0;
}

auto tp_is_mob1(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_mob1) != 0;
}

auto tp_is_mob2(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_mob2) != 0;
}

auto tp_is_monst_group_easy(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_monst_group_easy) != 0;
}

auto tp_is_monst_group_hard(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_monst_group_hard) != 0;
}

auto tp_is_monst_group_mob(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_monst_group_mob) != 0;
}

auto tp_is_monst(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_monst) != 0;
}

auto tp_is_needs_move_confirm(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_needs_move_confirm) != 0;
}

auto tp_is_obs_to_cursor_path(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_obs_to_cursor_path) != 0;
}

auto tp_is_obs_to_explosion(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_obs_to_explosion) != 0;
}

auto tp_is_obs_to_falling_onto(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_obs_to_falling_onto) != 0;
}

auto tp_is_obs_to_fire(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_obs_to_fire) != 0;
}

auto tp_is_obs_to_jump_over(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_obs_to_jump_over) != 0;
}

auto tp_is_obs_to_jumping_onto(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_obs_to_jumping_onto) != 0;
}

auto tp_is_obs_to_jumping_out_of(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_obs_to_jumping_out_of) != 0;
}

auto tp_is_obs_to_movement(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_obs_to_movement) != 0;
}

auto tp_is_obs_to_spawning(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_obs_to_spawning) != 0;
}

auto tp_is_obs_to_teleporting_onto(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_obs_to_teleporting_onto) != 0;
}

auto tp_is_obs_to_vision(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_obs_to_vision) != 0;
}

auto tp_is_obs_when_dead(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_obs_when_dead) != 0;
}

auto tp_is_openable(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_openable) != 0;
}

auto tp_is_physics_explosion(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_physics_explosion) != 0;
}

auto tp_is_physics_temperature(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_physics_temperature) != 0;
}

auto tp_is_physics_water(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_physics_water) != 0;
}

auto tp_is_pillar(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_pillar) != 0;
}

auto tp_is_plant(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_plant) != 0;
}

auto tp_is_player(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_player) != 0;
}

auto tp_is_projectile(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_projectile) != 0;
}

auto tp_is_removable_on_err(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_removable_on_err) != 0;
}

auto tp_is_removable_when_dead_on_err(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_removable_when_dead_on_err) != 0;
}

auto tp_is_rock(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_rock) != 0;
}

auto tp_is_shovable(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_shovable) != 0;
}

auto tp_is_slime(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_slime) != 0;
}

auto tp_is_smoke(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_smoke) != 0;
}

auto tp_is_steam(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_steam) != 0;
}

auto tp_is_stone(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_stone) != 0;
}

auto tp_is_submergible(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_submergible) != 0;
}

auto tp_is_teleport_blocked(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_teleport_blocked) != 0;
}

auto tp_is_teleport(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_teleport) != 0;
}

auto tp_is_tick_end_delay(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_tick_end_delay) != 0;
}

auto tp_is_tickable(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_tickable) != 0;
}

auto tp_is_tiled(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_tiled) != 0;
}

auto tp_is_trap(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_trap) != 0;
}

auto tp_is_treasure(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_treasure) != 0;
}

auto tp_is_undead(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_undead) != 0;
}

auto tp_is_unused1(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused1) != 0;
}

auto tp_is_unused10(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused10) != 0;
}

auto tp_is_unused11(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused11) != 0;
}

auto tp_is_unused12(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused12) != 0;
}

auto tp_is_unused13(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused13) != 0;
}

auto tp_is_unused14(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused14) != 0;
}

auto tp_is_unused15(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused15) != 0;
}

auto tp_is_unused16(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused16) != 0;
}

auto tp_is_unused17(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused17) != 0;
}

auto tp_is_unused18(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused18) != 0;
}

auto tp_is_unused19(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused19) != 0;
}

auto tp_is_unused2(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused2) != 0;
}

auto tp_is_unused20(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused20) != 0;
}

auto tp_is_unused21(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused21) != 0;
}

auto tp_is_unused22(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused22) != 0;
}

auto tp_is_unused23(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused23) != 0;
}

auto tp_is_unused24(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused24) != 0;
}

auto tp_is_unused25(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused25) != 0;
}

auto tp_is_unused26(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused26) != 0;
}

auto tp_is_unused27(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused27) != 0;
}

auto tp_is_unused28(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused28) != 0;
}

auto tp_is_unused29(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused29) != 0;
}

auto tp_is_unused3(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused3) != 0;
}

auto tp_is_unused30(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused30) != 0;
}

auto tp_is_unused31(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused31) != 0;
}

auto tp_is_unused32(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused32) != 0;
}

auto tp_is_unused33(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused33) != 0;
}

auto tp_is_unused34(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused34) != 0;
}

auto tp_is_unused35(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused35) != 0;
}

auto tp_is_unused36(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused36) != 0;
}

auto tp_is_unused37(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused37) != 0;
}

auto tp_is_unused38(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused38) != 0;
}

auto tp_is_unused39(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused39) != 0;
}

auto tp_is_unused4(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused4) != 0;
}

auto tp_is_unused40(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused40) != 0;
}

auto tp_is_unused41(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused41) != 0;
}

auto tp_is_unused42(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused42) != 0;
}

auto tp_is_unused43(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused43) != 0;
}

auto tp_is_unused44(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused44) != 0;
}

auto tp_is_unused45(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused45) != 0;
}

auto tp_is_level_open_icon(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_level_open_icon) != 0;
}

auto tp_is_blit_bg(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_blit_bg) != 0;
}

auto tp_is_level_select_bg(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_level_select_bg) != 0;
}

auto tp_is_blit_hit_outline_w_black_inside(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_blit_hit_outline_w_black_inside) != 0;
}

auto tp_is_unused5(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused5) != 0;
}

auto tp_is_able_to_fall_sound(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_able_to_fall_sound) != 0;
}

auto tp_is_collision_hit_first_on_tile(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_collision_hit_first_on_tile) != 0;
}

auto tp_is_collision_hit_all_on_tile(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_collision_hit_all_on_tile) != 0;
}

auto tp_is_blit_hit_outline_w_invis_inside(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_blit_hit_outline_w_invis_inside) != 0;
}

auto tp_is_blit_when_obscured_outline(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_blit_when_obscured_outline) != 0;
}

auto tp_is_attackable_by_player(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_attackable_by_player) != 0;
}

auto tp_is_attackable_by_monst(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_attackable_by_monst) != 0;
}

auto tp_is_effect(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_effect) != 0;
}

auto tp_is_effect_attack(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_effect_attack) != 0;
}

auto tp_is_effect_blood(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_effect_blood) != 0;
}

auto tp_is_unused6(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused6) != 0;
}

auto tp_is_hit_when_dead(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_hit_when_dead) != 0;
}

auto tp_is_able_to_fire_projectiles(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_able_to_fire_projectiles) != 0;
}

auto tp_is_blitzhound(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_blitzhound) != 0;
}

auto tp_is_unused63(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused63) != 0;
}

auto tp_is_grouped_thing(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_grouped_thing) != 0;
}

auto tp_is_vision_360_degrees(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_vision_360_degrees) != 0;
}

auto tp_is_vision_180_degrees(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_vision_180_degrees) != 0;
}

auto tp_is_able_to_lunge(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_able_to_lunge) != 0;
}

auto tp_is_obs_to_paths(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_obs_to_paths) != 0;
}

auto tp_is_unused7(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused7) != 0;
}

auto tp_is_unused8(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused8) != 0;
}

auto tp_is_unused9(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused9) != 0;
}

auto tp_is_wait_on_dead_anim(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_wait_on_dead_anim) != 0;
}

auto tp_is_wall(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_wall) != 0;
}

auto tp_is_water(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_water) != 0;
}

auto tp_is_wood(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_wood) != 0;
}
