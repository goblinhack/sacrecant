//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_thing_inlines.hpp"
#include "my_tp.hpp"
#include "my_types.hpp"

[[nodiscard]] auto tp_is_able_to_collect_items(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_able_to_collect_items) != 0;
}

[[nodiscard]] auto tp_is_able_to_collect_keys(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_able_to_collect_keys) != 0;
}

[[nodiscard]] auto tp_is_able_to_crush_grass(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_able_to_crush_grass) != 0;
}

[[nodiscard]] auto tp_is_able_to_fall_repeatedly(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_able_to_fall_repeatedly) != 0;
}

[[nodiscard]] auto tp_is_able_to_fall(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_able_to_fall) != 0;
}

[[nodiscard]] auto tp_is_able_to_jump(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_able_to_jump) != 0;
}

[[nodiscard]] auto tp_is_able_to_move_diagonally(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_able_to_move_diagonally) != 0;
}

[[nodiscard]] auto tp_is_able_to_move_through_walls(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_able_to_move_through_walls) != 0;
}

[[nodiscard]] auto tp_is_able_to_open_things(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_able_to_open_things) != 0;
}

[[nodiscard]] auto tp_is_able_to_shove(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_able_to_shove) != 0;
}

[[nodiscard]] auto tp_is_able_to_see_through_walls(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_able_to_see_through_walls) != 0;
}

[[nodiscard]] auto tp_is_animated_can_hflip(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_animated_can_hflip) != 0;
}

[[nodiscard]] auto tp_is_animated_no_dir(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_animated_no_dir) != 0;
}

[[nodiscard]] auto tp_is_animated_sync_first(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_animated_sync_first) != 0;
}

[[nodiscard]] auto tp_is_animated(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_animated) != 0;
}

[[nodiscard]] auto tp_is_barrel(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_barrel) != 0;
}

[[nodiscard]] auto tp_is_blit_centered(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_blit_centered) != 0;
}

[[nodiscard]] auto tp_is_blit_flush_per_line(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_blit_flush_per_line) != 0;
}

[[nodiscard]] auto tp_is_blit_if_has_seen(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_blit_if_has_seen) != 0;
}

[[nodiscard]] auto tp_is_blit_obscures(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_blit_obscures) != 0;
}

[[nodiscard]] auto tp_is_blit_on_ground(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_blit_on_ground) != 0;
}

[[nodiscard]] auto tp_is_blit_outlined(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_blit_outlined) != 0;
}

[[nodiscard]] auto tp_is_blit_per_pixel_lighting(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_blit_per_pixel_lighting) != 0;
}

[[nodiscard]] auto tp_is_blit_shown_in_chasms(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_blit_shown_in_chasms) != 0;
}

[[nodiscard]] auto tp_is_blit_shown_in_overlay(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_blit_shown_in_overlay) != 0;
}

[[nodiscard]] auto tp_is_blit_square_outlined(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_blit_square_outlined) != 0;
}

[[nodiscard]] auto tp_is_blit_when_obscured_faded(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_blit_when_obscured_faded) != 0;
}

[[nodiscard]] auto tp_is_border(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_border) != 0;
}

[[nodiscard]] auto tp_is_brazier(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_brazier) != 0;
}

[[nodiscard]] auto tp_is_bridge(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_bridge) != 0;
}

[[nodiscard]] auto tp_is_broken_on_death(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_broken_on_death) != 0;
}

[[nodiscard]] auto tp_is_burnable(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_burnable) != 0;
}

[[nodiscard]] auto tp_is_chasm(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_chasm) != 0;
}

[[nodiscard]] auto tp_is_collectable(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_collectable) != 0;
}

[[nodiscard]] auto tp_is_collision_circle_large(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_collision_circle_large) != 0;
}

[[nodiscard]] auto tp_is_collision_circle_small(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_collision_circle_small) != 0;
}

[[nodiscard]] auto tp_is_collision_detection_enabled(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_collision_detection_enabled) != 0;
}

[[nodiscard]] auto tp_is_collision_square(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_collision_square) != 0;
}

[[nodiscard]] auto tp_is_combustible(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_combustible) != 0;
}

[[nodiscard]] auto tp_is_corpse_on_death(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_corpse_on_death) != 0;
}

[[nodiscard]] auto tp_is_corridor(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_corridor) != 0;
}

[[nodiscard]] auto tp_is_crushable(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_crushable) != 0;
}

[[nodiscard]] auto tp_is_cursor_path_hazard(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_cursor_path_hazard) != 0;
}

[[nodiscard]] auto tp_is_cursor_path_none(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_cursor_path_none) != 0;
}

[[nodiscard]] auto tp_is_cursor_path_warning(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_cursor_path_warning) != 0;
}

[[nodiscard]] auto tp_is_cursor_path(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_cursor_path) != 0;
}

[[nodiscard]] auto tp_is_cursor(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_cursor) != 0;
}

[[nodiscard]] auto tp_is_damage_capped(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_damage_capped) != 0;
}

[[nodiscard]] auto tp_is_dead_on_collision(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_dead_on_collision) != 0;
}

[[nodiscard]] auto tp_is_dead_on_shoving(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_dead_on_shoving) != 0;
}

[[nodiscard]] auto tp_is_water_deep(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_water_deep) != 0;
}

[[nodiscard]] auto tp_is_described_cursor(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_described_cursor) != 0;
}

[[nodiscard]] auto tp_is_dirt(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_dirt) != 0;
}

[[nodiscard]] auto tp_is_dmap(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_dmap) != 0;
}

[[nodiscard]] auto tp_is_door_locked(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_door_locked) != 0;
}

[[nodiscard]] auto tp_is_door_secret(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_door_secret) != 0;
}

[[nodiscard]] auto tp_is_door_unlocked(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_door_unlocked) != 0;
}

[[nodiscard]] auto tp_is_dungeon_entrance(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_dungeon_entrance) != 0;
}

[[nodiscard]] auto tp_is_entrance(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_entrance) != 0;
}

[[nodiscard]] auto tp_is_ethereal(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_ethereal) != 0;
}

[[nodiscard]] auto tp_is_exit(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_exit) != 0;
}

[[nodiscard]] auto tp_is_explosion(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_explosion) != 0;
}

[[nodiscard]] auto tp_is_extinguished_on_death(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_extinguished_on_death) != 0;
}

[[nodiscard]] auto tp_is_fire(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_fire) != 0;
}

[[nodiscard]] auto tp_is_lucky(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_lucky) != 0;
}

[[nodiscard]] auto tp_is_flesh(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_flesh) != 0;
}

[[nodiscard]] auto tp_is_levitating(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_levitating) != 0;
}

[[nodiscard]] auto tp_is_floor(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_floor) != 0;
}

[[nodiscard]] auto tp_is_obs_to_ethereal(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_obs_to_ethereal) != 0;
}

[[nodiscard]] auto tp_is_foliage(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_foliage) != 0;
}

[[nodiscard]] auto tp_is_gaseous(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_gaseous) != 0;
}

[[nodiscard]] auto tp_is_ghost(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_ghost) != 0;
}

[[nodiscard]] auto tp_is_glass(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_glass) != 0;
}

[[nodiscard]] auto tp_is_gold(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_gold) != 0;
}

[[nodiscard]] auto tp_is_grass(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_grass) != 0;
}

[[nodiscard]] auto tp_is_shown_health(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_shown_health) != 0;
}

[[nodiscard]] auto tp_is_indestructible(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_indestructible) != 0;
}

[[nodiscard]] auto tp_is_insectoid(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_insectoid) != 0;
}

[[nodiscard]] auto tp_is_inventory_item(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_inventory_item) != 0;
}

[[nodiscard]] auto tp_is_droppable(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_droppable) != 0;
}

[[nodiscard]] auto tp_is_unused183(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused183) != 0;
}

[[nodiscard]] auto tp_is_item_mergeable(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_item_mergeable) != 0;
}

[[nodiscard]] auto tp_is_item(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_item) != 0;
}

[[nodiscard]] auto tp_is_key(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_key) != 0;
}

[[nodiscard]] auto tp_is_kobalos(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_kobalos) != 0;
}

[[nodiscard]] auto tp_is_lava(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_lava) != 0;
}

[[nodiscard]] auto tp_is_usable(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_usable) != 0;
}

[[nodiscard]] auto tp_is_level_curr(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_level_curr) != 0;
}

[[nodiscard]] auto tp_is_critical_to_dungeon_design(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_critical_to_dungeon_design) != 0;
}

[[nodiscard]] auto tp_is_level_final_icon(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_level_final_icon) != 0;
}

[[nodiscard]] auto tp_is_level_next_icon(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_level_next_icon) != 0;
}

[[nodiscard]] auto tp_is_level_locked_icon(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_level_locked_icon) != 0;
}

[[nodiscard]] auto tp_is_level_closed_icon(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_level_closed_icon) != 0;
}

[[nodiscard]] auto tp_is_light_source(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_light_source) != 0;
}

[[nodiscard]] auto tp_is_loggable(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_loggable) != 0;
}

[[nodiscard]] auto tp_is_able_to_eat_items(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_able_to_eat_items) != 0;
}

[[nodiscard]] auto tp_is_meltable(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_meltable) != 0;
}

[[nodiscard]] auto tp_is_metal(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_metal) != 0;
}

[[nodiscard]] auto tp_is_minion(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_minion) != 0;
}

[[nodiscard]] auto tp_is_mob_kill_minions_on_death(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_mob_kill_minions_on_death) != 0;
}

[[nodiscard]] auto tp_is_mob(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_mob) != 0;
}

[[nodiscard]] auto tp_is_mob1(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_mob1) != 0;
}

[[nodiscard]] auto tp_is_mob2(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_mob2) != 0;
}

[[nodiscard]] auto tp_is_monst1(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_monst1) != 0;
}

[[nodiscard]] auto tp_is_monst2(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_monst2) != 0;
}

[[nodiscard]] auto tp_is_monst(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_monst) != 0;
}

[[nodiscard]] auto tp_is_needs_move_confirm(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_needs_move_confirm) != 0;
}

[[nodiscard]] auto tp_is_obs_to_cursor_path(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_obs_to_cursor_path) != 0;
}

[[nodiscard]] auto tp_is_obs_to_explosion(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_obs_to_explosion) != 0;
}

[[nodiscard]] auto tp_is_obs_to_falling_onto(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_obs_to_falling_onto) != 0;
}

[[nodiscard]] auto tp_is_obs_to_fire(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_obs_to_fire) != 0;
}

[[nodiscard]] auto tp_is_obs_to_jumping_over(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_obs_to_jumping_over) != 0;
}

[[nodiscard]] auto tp_is_obs_to_jumping_onto(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_obs_to_jumping_onto) != 0;
}

[[nodiscard]] auto tp_is_obs_to_jumping_out_of(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_obs_to_jumping_out_of) != 0;
}

[[nodiscard]] auto tp_is_obs_to_movement(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_obs_to_movement) != 0;
}

[[nodiscard]] auto tp_is_obs_to_spawning(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_obs_to_spawning) != 0;
}

[[nodiscard]] auto tp_is_obs_to_teleporting_onto(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_obs_to_teleporting_onto) != 0;
}

[[nodiscard]] auto tp_is_obs_to_vision(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_obs_to_vision) != 0;
}

[[nodiscard]] auto tp_is_obs_when_dead(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_obs_when_dead) != 0;
}

[[nodiscard]] auto tp_is_openable(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_openable) != 0;
}

[[nodiscard]] auto tp_is_physics_explosion(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_physics_explosion) != 0;
}

[[nodiscard]] auto tp_is_physics_temperature(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_physics_temperature) != 0;
}

[[nodiscard]] auto tp_is_physics_water(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_physics_water) != 0;
}

[[nodiscard]] auto tp_is_pillar(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_pillar) != 0;
}

[[nodiscard]] auto tp_is_plant(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_plant) != 0;
}

[[nodiscard]] auto tp_is_player(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_player) != 0;
}

[[nodiscard]] auto tp_is_projectile(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_projectile) != 0;
}

[[nodiscard]] auto tp_is_removable_on_err(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_removable_on_err) != 0;
}

[[nodiscard]] auto tp_is_removable_when_dead_on_err(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_removable_when_dead_on_err) != 0;
}

[[nodiscard]] auto tp_is_rock(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_rock) != 0;
}

[[nodiscard]] auto tp_is_shovable(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_shovable) != 0;
}

[[nodiscard]] auto tp_is_slime(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_slime) != 0;
}

[[nodiscard]] auto tp_is_smoke(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_smoke) != 0;
}

[[nodiscard]] auto tp_is_steam(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_steam) != 0;
}

[[nodiscard]] auto tp_is_stone(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_stone) != 0;
}

[[nodiscard]] auto tp_is_submergible(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_submergible) != 0;
}

[[nodiscard]] auto tp_is_able_to_be_teleported(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_able_to_be_teleported) != 0;
}

[[nodiscard]] auto tp_is_teleport(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_teleport) != 0;
}

[[nodiscard]] auto tp_is_tick_end_delay(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_tick_end_delay) != 0;
}

[[nodiscard]] auto tp_is_tickable(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_tickable) != 0;
}

[[nodiscard]] auto tp_is_tiled(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_tiled) != 0;
}

[[nodiscard]] auto tp_is_trap(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_trap) != 0;
}

[[nodiscard]] auto tp_is_treasure(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_treasure) != 0;
}

[[nodiscard]] auto tp_is_undead(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_undead) != 0;
}

[[nodiscard]] auto tp_is_unused1(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused1) != 0;
}

[[nodiscard]] auto tp_is_unused2(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused2) != 0;
}

[[nodiscard]] auto tp_is_unused3(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused3) != 0;
}

[[nodiscard]] auto tp_is_unused4(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused4) != 0;
}

[[nodiscard]] auto tp_is_unused5(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused5) != 0;
}

[[nodiscard]] auto tp_is_unused6(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused6) != 0;
}

[[nodiscard]] auto tp_is_unused7(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused7) != 0;
}

[[nodiscard]] auto tp_is_unused8(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused8) != 0;
}

[[nodiscard]] auto tp_is_unused9(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused9) != 0;
}

[[nodiscard]] auto tp_is_unused10(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused10) != 0;
}

[[nodiscard]] auto tp_is_unused11(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused11) != 0;
}

[[nodiscard]] auto tp_is_unused12(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused12) != 0;
}

[[nodiscard]] auto tp_is_unused13(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused13) != 0;
}

[[nodiscard]] auto tp_is_unused14(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused14) != 0;
}

[[nodiscard]] auto tp_is_unused15(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused15) != 0;
}

[[nodiscard]] auto tp_is_unused16(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused16) != 0;
}

[[nodiscard]] auto tp_is_unused17(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused17) != 0;
}

[[nodiscard]] auto tp_is_unused18(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused18) != 0;
}

[[nodiscard]] auto tp_is_unused19(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused19) != 0;
}

[[nodiscard]] auto tp_is_unused20(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused20) != 0;
}

[[nodiscard]] auto tp_is_unused21(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused21) != 0;
}

[[nodiscard]] auto tp_is_unused22(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused22) != 0;
}

[[nodiscard]] auto tp_is_unused23(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused23) != 0;
}

[[nodiscard]] auto tp_is_unused24(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused24) != 0;
}

[[nodiscard]] auto tp_is_unused25(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused25) != 0;
}

[[nodiscard]] auto tp_is_unused26(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused26) != 0;
}

[[nodiscard]] auto tp_is_unused27(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused27) != 0;
}

[[nodiscard]] auto tp_is_unused28(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused28) != 0;
}

[[nodiscard]] auto tp_is_unused29(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused29) != 0;
}

[[nodiscard]] auto tp_is_unused30(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused30) != 0;
}

[[nodiscard]] auto tp_is_unused31(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused31) != 0;
}

[[nodiscard]] auto tp_is_unused32(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused32) != 0;
}

[[nodiscard]] auto tp_is_unused33(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused33) != 0;
}

[[nodiscard]] auto tp_is_unused34(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused34) != 0;
}

[[nodiscard]] auto tp_is_unused35(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused35) != 0;
}

[[nodiscard]] auto tp_is_unused36(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused36) != 0;
}

[[nodiscard]] auto tp_is_unused37(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused37) != 0;
}

[[nodiscard]] auto tp_is_unused38(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused38) != 0;
}

[[nodiscard]] auto tp_is_unused39(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused39) != 0;
}

[[nodiscard]] auto tp_is_unused40(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused40) != 0;
}

[[nodiscard]] auto tp_is_unused41(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused41) != 0;
}

[[nodiscard]] auto tp_is_unused42(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused42) != 0;
}

[[nodiscard]] auto tp_is_unused43(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused43) != 0;
}

[[nodiscard]] auto tp_is_unused44(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused44) != 0;
}

[[nodiscard]] auto tp_is_unused45(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused45) != 0;
}

[[nodiscard]] auto tp_is_unused46(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused46) != 0;
}

[[nodiscard]] auto tp_is_unused47(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused47) != 0;
}

[[nodiscard]] auto tp_is_unused48(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused48) != 0;
}

[[nodiscard]] auto tp_is_unused49(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused49) != 0;
}

[[nodiscard]] auto tp_is_unused50(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused50) != 0;
}

[[nodiscard]] auto tp_is_unused51(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused51) != 0;
}

[[nodiscard]] auto tp_is_unused52(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused52) != 0;
}

[[nodiscard]] auto tp_is_unused53(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused53) != 0;
}

[[nodiscard]] auto tp_is_unused54(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused54) != 0;
}

[[nodiscard]] auto tp_is_unused55(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused55) != 0;
}

[[nodiscard]] auto tp_is_unused56(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused56) != 0;
}

[[nodiscard]] auto tp_is_unused57(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused57) != 0;
}

[[nodiscard]] auto tp_is_unused58(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused58) != 0;
}

[[nodiscard]] auto tp_is_unused59(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused59) != 0;
}

[[nodiscard]] auto tp_is_unused60(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused60) != 0;
}

[[nodiscard]] auto tp_is_unused61(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused61) != 0;
}

[[nodiscard]] auto tp_is_unused62(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused62) != 0;
}

[[nodiscard]] auto tp_is_unused63(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused63) != 0;
}

[[nodiscard]] auto tp_is_unused64(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused64) != 0;
}

[[nodiscard]] auto tp_is_unused65(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused65) != 0;
}

[[nodiscard]] auto tp_is_unused66(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused66) != 0;
}

[[nodiscard]] auto tp_is_unused67(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused67) != 0;
}

[[nodiscard]] auto tp_is_unused68(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused68) != 0;
}

[[nodiscard]] auto tp_is_unused69(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused69) != 0;
}

[[nodiscard]] auto tp_is_unused70(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused70) != 0;
}

[[nodiscard]] auto tp_is_unused71(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused71) != 0;
}

[[nodiscard]] auto tp_is_unused72(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused72) != 0;
}

[[nodiscard]] auto tp_is_unused73(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused73) != 0;
}

[[nodiscard]] auto tp_is_unused74(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused74) != 0;
}

[[nodiscard]] auto tp_is_unused75(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused75) != 0;
}

[[nodiscard]] auto tp_is_unused76(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused76) != 0;
}

[[nodiscard]] auto tp_is_unused77(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused77) != 0;
}

[[nodiscard]] auto tp_is_unused78(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused78) != 0;
}

[[nodiscard]] auto tp_is_unused79(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused79) != 0;
}

[[nodiscard]] auto tp_is_unused80(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused80) != 0;
}

[[nodiscard]] auto tp_is_unused81(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused81) != 0;
}

[[nodiscard]] auto tp_is_unused82(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused82) != 0;
}

[[nodiscard]] auto tp_is_unused83(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused83) != 0;
}

[[nodiscard]] auto tp_is_unused84(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused84) != 0;
}

[[nodiscard]] auto tp_is_unused85(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused85) != 0;
}

[[nodiscard]] auto tp_is_unused86(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused86) != 0;
}

[[nodiscard]] auto tp_is_unused87(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused87) != 0;
}

[[nodiscard]] auto tp_is_unused88(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused88) != 0;
}

[[nodiscard]] auto tp_is_unused89(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused89) != 0;
}

[[nodiscard]] auto tp_is_unused90(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused90) != 0;
}

[[nodiscard]] auto tp_is_unused91(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused91) != 0;
}

[[nodiscard]] auto tp_is_unused92(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused92) != 0;
}

[[nodiscard]] auto tp_is_unused93(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused93) != 0;
}

[[nodiscard]] auto tp_is_unused94(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused94) != 0;
}

[[nodiscard]] auto tp_is_unused95(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused95) != 0;
}

[[nodiscard]] auto tp_is_unused96(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused96) != 0;
}

[[nodiscard]] auto tp_is_unused97(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused97) != 0;
}

[[nodiscard]] auto tp_is_unused98(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused98) != 0;
}

[[nodiscard]] auto tp_is_unused99(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused99) != 0;
}

[[nodiscard]] auto tp_is_unused100(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused100) != 0;
}

[[nodiscard]] auto tp_is_unused101(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused101) != 0;
}

[[nodiscard]] auto tp_is_unused102(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused102) != 0;
}

[[nodiscard]] auto tp_is_unused103(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused103) != 0;
}

[[nodiscard]] auto tp_is_unused104(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused104) != 0;
}

[[nodiscard]] auto tp_is_unused105(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused105) != 0;
}

[[nodiscard]] auto tp_is_unused106(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused106) != 0;
}

[[nodiscard]] auto tp_is_unused107(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused107) != 0;
}

[[nodiscard]] auto tp_is_unused108(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused108) != 0;
}

[[nodiscard]] auto tp_is_unused109(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused109) != 0;
}

[[nodiscard]] auto tp_is_unused110(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused110) != 0;
}

[[nodiscard]] auto tp_is_unused111(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused111) != 0;
}

[[nodiscard]] auto tp_is_unused112(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused112) != 0;
}

[[nodiscard]] auto tp_is_unused113(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused113) != 0;
}

[[nodiscard]] auto tp_is_unused114(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused114) != 0;
}

[[nodiscard]] auto tp_is_unused115(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused115) != 0;
}

[[nodiscard]] auto tp_is_unused116(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused116) != 0;
}

[[nodiscard]] auto tp_is_unused117(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused117) != 0;
}

[[nodiscard]] auto tp_is_unused118(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused118) != 0;
}

[[nodiscard]] auto tp_is_unused119(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused119) != 0;
}

[[nodiscard]] auto tp_is_unused120(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused120) != 0;
}

[[nodiscard]] auto tp_is_unused121(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused121) != 0;
}

[[nodiscard]] auto tp_is_unused122(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused122) != 0;
}

[[nodiscard]] auto tp_is_unused123(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused123) != 0;
}

[[nodiscard]] auto tp_is_unused124(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused124) != 0;
}

[[nodiscard]] auto tp_is_unused125(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused125) != 0;
}

[[nodiscard]] auto tp_is_unused126(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused126) != 0;
}

[[nodiscard]] auto tp_is_unused127(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused127) != 0;
}

[[nodiscard]] auto tp_is_unused128(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused128) != 0;
}

[[nodiscard]] auto tp_is_unused129(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused129) != 0;
}

[[nodiscard]] auto tp_is_unused130(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused130) != 0;
}

[[nodiscard]] auto tp_is_unused131(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused131) != 0;
}

[[nodiscard]] auto tp_is_unused132(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused132) != 0;
}

[[nodiscard]] auto tp_is_unused133(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused133) != 0;
}

[[nodiscard]] auto tp_is_unused134(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused134) != 0;
}

[[nodiscard]] auto tp_is_unused135(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused135) != 0;
}

[[nodiscard]] auto tp_is_unused136(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused136) != 0;
}

[[nodiscard]] auto tp_is_unused137(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused137) != 0;
}

[[nodiscard]] auto tp_is_unused138(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused138) != 0;
}

[[nodiscard]] auto tp_is_unused139(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused139) != 0;
}

[[nodiscard]] auto tp_is_unused140(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused140) != 0;
}

[[nodiscard]] auto tp_is_unused141(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused141) != 0;
}

[[nodiscard]] auto tp_is_unused142(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused142) != 0;
}

[[nodiscard]] auto tp_is_unused143(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused143) != 0;
}

[[nodiscard]] auto tp_is_unused144(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused144) != 0;
}

[[nodiscard]] auto tp_is_unused145(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused145) != 0;
}

[[nodiscard]] auto tp_is_unused146(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused146) != 0;
}

[[nodiscard]] auto tp_is_unused147(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused147) != 0;
}

[[nodiscard]] auto tp_is_unused148(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused148) != 0;
}

[[nodiscard]] auto tp_is_unused149(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused149) != 0;
}

[[nodiscard]] auto tp_is_unused150(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused150) != 0;
}

[[nodiscard]] auto tp_is_unused151(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused151) != 0;
}

[[nodiscard]] auto tp_is_unused152(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused152) != 0;
}

[[nodiscard]] auto tp_is_unused153(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused153) != 0;
}

[[nodiscard]] auto tp_is_unused154(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused154) != 0;
}

[[nodiscard]] auto tp_is_noisy(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_noisy) != 0;
}

[[nodiscard]] auto tp_is_hook(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_hook) != 0;
}

[[nodiscard]] auto tp_is_sacrifice(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_sacrifice) != 0;
}

[[nodiscard]] auto tp_is_debuff(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_debuff) != 0;
}

[[nodiscard]] auto tp_is_venomous(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_venomous) != 0;
}

[[nodiscard]] auto tp_is_able_to_see_invisible(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_able_to_see_invisible) != 0;
}

[[nodiscard]] auto tp_is_able_to_be_invisible(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_able_to_be_invisible) != 0;
}

[[nodiscard]] auto tp_is_invisible(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_invisible) != 0;
}

[[nodiscard]] auto tp_is_critical_to_gameplay(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_critical_to_gameplay) != 0;
}

[[nodiscard]] auto tp_is_able_to_jump_attack(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_able_to_jump_attack) != 0;
}

[[nodiscard]] auto tp_is_fish(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_fish) != 0;
}

[[nodiscard]] auto tp_is_unlucky(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unlucky) != 0;
}

[[nodiscard]] auto tp_is_cursed(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_cursed) != 0;
}

[[nodiscard]] auto tp_is_chocolate(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_chocolate) != 0;
}

[[nodiscard]] auto tp_is_tick_on_eaten(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_tick_on_eaten) != 0;
}

[[nodiscard]] auto tp_is_able_to_eat_food(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_able_to_eat_food) != 0;
}

[[nodiscard]] auto tp_is_edible(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_edible) != 0;
}

[[nodiscard]] auto tp_is_able_to_be_engulfed_blocked(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_able_to_be_engulfed_blocked) != 0;
}

[[nodiscard]] auto tp_is_able_to_be_ensnared_blocked(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_able_to_be_ensnared_blocked) != 0;
}

[[nodiscard]] auto tp_is_active_when_worn(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_active_when_worn) != 0;
}

[[nodiscard]] auto tp_is_leather(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_leather) != 0;
}

[[nodiscard]] auto tp_is_boots(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_boots) != 0;
}

[[nodiscard]] auto tp_is_spider_giant(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_spider_giant) != 0;
}

[[nodiscard]] auto tp_is_spider_baby(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_spider_baby) != 0;
}

[[nodiscard]] auto tp_is_able_to_choose_targets(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_able_to_choose_targets) != 0;
}

[[nodiscard]] auto tp_is_flesh_eater(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_flesh_eater) != 0;
}

[[nodiscard]] auto tp_is_able_to_move(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_able_to_move) != 0;
}

[[nodiscard]] auto tp_is_spider(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_spider) != 0;
}

[[nodiscard]] auto tp_is_able_to_be_ensnared(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_able_to_be_ensnared) != 0;
}

[[nodiscard]] auto tp_is_able_to_ensnare(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_able_to_ensnare) != 0;
}

[[nodiscard]] auto tp_is_spiderweb(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_spiderweb) != 0;
}

[[nodiscard]] auto tp_is_rubble(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_rubble) != 0;
}

[[nodiscard]] auto tp_is_able_to_jump_land_then_pounce(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_able_to_jump_land_then_pounce) != 0;
}

[[nodiscard]] auto tp_is_unused188(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_unused188) != 0;
}

[[nodiscard]] auto tp_is_skeleton(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_skeleton) != 0;
}

[[nodiscard]] auto tp_is_water_bound(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_water_bound) != 0;
}

[[nodiscard]] auto tp_is_amphibious(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_amphibious) != 0;
}

[[nodiscard]] auto tp_is_able_to_resurrect(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_able_to_resurrect) != 0;
}

[[nodiscard]] auto tp_is_soft_landing(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_soft_landing) != 0;
}

[[nodiscard]] auto tp_is_gelatinous(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_gelatinous) != 0;
}

[[nodiscard]] auto tp_is_potion(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_potion) != 0;
}

[[nodiscard]] auto tp_is_able_to_be_levitated(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_able_to_be_levitated) != 0;
}

[[nodiscard]] auto tp_is_teleport_hazard(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_teleport_hazard) != 0;
}

[[nodiscard]] auto tp_is_active_when_carried(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_active_when_carried) != 0;
}

[[nodiscard]] auto tp_is_wearable(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_wearable) != 0;
}

[[nodiscard]] auto tp_is_ring(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_ring) != 0;
}

[[nodiscard]] auto tp_is_obs_to_hearing(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_obs_to_hearing) != 0;
}

[[nodiscard]] auto tp_is_throwable(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_throwable) != 0;
}

[[nodiscard]] auto tp_is_obs_to_throwing_over(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_obs_to_throwing_over) != 0;
}

[[nodiscard]] auto tp_is_obs_to_throwing_onto(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_obs_to_throwing_onto) != 0;
}

[[nodiscard]] auto tp_is_physics_trap(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_physics_trap) != 0;
}

[[nodiscard]] auto tp_is_tick_on_strip(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_tick_on_strip) != 0;
}

[[nodiscard]] auto tp_is_tick_on_worn(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_tick_on_worn) != 0;
}

[[nodiscard]] auto tp_is_buff(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_buff) != 0;
}

[[nodiscard]] auto tp_is_able_to_be_buffed(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_able_to_be_buffed) != 0;
}

[[nodiscard]] auto tp_is_tick_on_drop(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_tick_on_drop) != 0;
}

[[nodiscard]] auto tp_is_lava_bg(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_lava_bg) != 0;
}

[[nodiscard]] auto tp_is_tick_on_use(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_tick_on_use) != 0;
}

[[nodiscard]] auto tp_is_able_to_throw(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_able_to_throw) != 0;
}

[[nodiscard]] auto tp_is_described_when_killed(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_described_when_killed) != 0;
}

[[nodiscard]] auto tp_is_chest(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_chest) != 0;
}

[[nodiscard]] auto tp_is_flammable(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_flammable) != 0;
}

[[nodiscard]] auto tp_is_auto_wear(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_auto_wear) != 0;
}

[[nodiscard]] auto tp_is_dead_when_discharged(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_dead_when_discharged) != 0;
}

[[nodiscard]] auto tp_is_able_to_fire_weapons(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_able_to_fire_weapons) != 0;
}

[[nodiscard]] auto tp_is_weapon(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_weapon) != 0;
}

[[nodiscard]] auto tp_is_light_flicker(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_light_flicker) != 0;
}

[[nodiscard]] auto tp_is_able_to_wear_items(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_able_to_wear_items) != 0;
}

[[nodiscard]] auto tp_is_wand(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_wand) != 0;
}

[[nodiscard]] auto tp_is_staff(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_staff) != 0;
}

[[nodiscard]] auto tp_is_able_to_eat_treasure(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_able_to_eat_treasure) != 0;
}

[[nodiscard]] auto tp_is_wait_on_anim(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_wait_on_anim) != 0;
}

[[nodiscard]] auto tp_is_flat(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_flat) != 0;
}

[[nodiscard]] auto tp_is_obs_to_beam(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_obs_to_beam) != 0;
}

[[nodiscard]] auto tp_is_beam_weapon(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_beam_weapon) != 0;
}

[[nodiscard]] auto tp_is_shown_stamina(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_shown_stamina) != 0;
}

[[nodiscard]] auto tp_is_reeds(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_reeds) != 0;
}

[[nodiscard]] auto tp_is_able_to_drop_all_items_on_death(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_able_to_drop_all_items_on_death) != 0;
}

[[nodiscard]] auto tp_is_vault(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_vault) != 0;
}

[[nodiscard]] auto tp_is_biome_underhell(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_biome_underhell) != 0;
}

[[nodiscard]] auto tp_is_biome_graveyard(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_biome_graveyard) != 0;
}

[[nodiscard]] auto tp_is_biome_nethervoid(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_biome_nethervoid) != 0;
}

[[nodiscard]] auto tp_is_biome_bogland(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_biome_bogland) != 0;
}

[[nodiscard]] auto tp_is_biome_dungeon(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_biome_dungeon) != 0;
}

[[nodiscard]] auto tp_is_level_open_icon(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_level_open_icon) != 0;
}

[[nodiscard]] auto tp_is_blit_bg(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_blit_bg) != 0;
}

[[nodiscard]] auto tp_is_level_select_bg(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_level_select_bg) != 0;
}

[[nodiscard]] auto tp_is_blit_hit_outline_w_black_inside(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_blit_hit_outline_w_black_inside) != 0;
}

[[nodiscard]] auto tp_is_able_to_engulf(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_able_to_engulf) != 0;
}

[[nodiscard]] auto tp_is_able_to_fall_sound(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_able_to_fall_sound) != 0;
}

[[nodiscard]] auto tp_is_collision_hit_first_on_tile(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_collision_hit_first_on_tile) != 0;
}

[[nodiscard]] auto tp_is_collision_hit_all_on_tile(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_collision_hit_all_on_tile) != 0;
}

[[nodiscard]] auto tp_is_blit_hit_outline_w_invis_inside(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_blit_hit_outline_w_invis_inside) != 0;
}

[[nodiscard]] auto tp_is_blit_when_obscured_outline(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_blit_when_obscured_outline) != 0;
}

[[nodiscard]] auto tp_is_attackable_by_player(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_attackable_by_player) != 0;
}

[[nodiscard]] auto tp_is_attackable_by_monst(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_attackable_by_monst) != 0;
}

[[nodiscard]] auto tp_is_effect(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_effect) != 0;
}

[[nodiscard]] auto tp_is_effect_attack(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_effect_attack) != 0;
}

[[nodiscard]] auto tp_is_effect_blood(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_effect_blood) != 0;
}

[[nodiscard]] auto tp_is_effect_ripple(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_effect_ripple) != 0;
}

[[nodiscard]] auto tp_is_hit_when_dead(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_hit_when_dead) != 0;
}

[[nodiscard]] auto tp_is_able_to_be_engulfed(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_able_to_be_engulfed) != 0;
}

[[nodiscard]] auto tp_is_grouped_thing(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_grouped_thing) != 0;
}

[[nodiscard]] auto tp_is_vision_360_degrees(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_vision_360_degrees) != 0;
}

[[nodiscard]] auto tp_is_vision_180_degrees(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_vision_180_degrees) != 0;
}

[[nodiscard]] auto tp_is_able_to_lunge(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_able_to_lunge) != 0;
}

[[nodiscard]] auto tp_is_obs_to_paths(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_obs_to_paths) != 0;
}

[[nodiscard]] auto tp_is_stealthy(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_stealthy) != 0;
}

[[nodiscard]] auto tp_is_tireless(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_tireless) != 0;
}

[[nodiscard]] auto tp_is_shown_noise(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_shown_noise) != 0;
}

[[nodiscard]] auto tp_is_wait_on_dead_anim(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_wait_on_dead_anim) != 0;
}

[[nodiscard]] auto tp_is_wall(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_wall) != 0;
}

[[nodiscard]] auto tp_is_water_shallow(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_water_shallow) != 0;
}

[[nodiscard]] auto tp_is_wood(Tpp tp) -> bool
{
  TRACE_DEBUG();
  return tp_flag(tp, is_wood) != 0;
}
