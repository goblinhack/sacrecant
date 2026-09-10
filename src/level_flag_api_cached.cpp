//
// Copyright goblinhack@gmail.com
//

#include "my_bpoint.hpp"
#include "my_callstack.hpp"
#include "my_level.hpp"
#include "my_level_inlines.hpp"
#include "my_tp.hpp"
#include "my_types.hpp"

[[nodiscard]] auto level_is_able_to_collect_items_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_able_to_collect_items, p);
}

[[nodiscard]] auto level_is_able_to_collect_keys_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_able_to_collect_keys, p);
}

[[nodiscard]] auto level_is_able_to_crush_grass_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_able_to_crush_grass, p);
}

[[nodiscard]] auto level_is_able_to_fall_repeatedly_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_able_to_fall_repeatedly, p);
}

[[nodiscard]] auto level_is_able_to_fall_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_able_to_fall, p);
}

[[nodiscard]] auto level_is_able_to_jump_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_able_to_jump, p);
}

[[nodiscard]] auto level_is_able_to_move_diagonally_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_able_to_move_diagonally, p);
}

[[nodiscard]] auto level_is_able_to_move_through_walls_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_able_to_move_through_walls, p);
}

[[nodiscard]] auto level_is_able_to_open_things_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_able_to_open_things, p);
}

[[nodiscard]] auto level_is_able_to_shove_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_able_to_shove, p);
}

[[nodiscard]] auto level_is_able_to_see_through_walls_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_able_to_see_through_walls, p);
}

[[nodiscard]] auto level_is_animated_can_hflip_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_animated_can_hflip, p);
}

[[nodiscard]] auto level_is_animated_no_dir_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_animated_no_dir, p);
}

[[nodiscard]] auto level_is_animated_sync_first_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_animated_sync_first, p);
}

[[nodiscard]] auto level_is_animated_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_animated, p);
}

[[nodiscard]] auto level_is_barrel_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_barrel, p);
}

[[nodiscard]] auto level_is_blit_centered_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_blit_centered, p);
}

[[nodiscard]] auto level_is_blit_flush_per_line_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_blit_flush_per_line, p);
}

[[nodiscard]] auto level_is_blit_if_has_seen_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_blit_if_has_seen, p);
}

[[nodiscard]] auto level_is_blit_obscures_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_blit_obscures, p);
}

[[nodiscard]] auto level_is_blit_on_ground_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_blit_on_ground, p);
}

[[nodiscard]] auto level_is_blit_outlined_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_blit_outlined, p);
}

[[nodiscard]] auto level_is_blit_per_pixel_lighting_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_blit_per_pixel_lighting, p);
}

[[nodiscard]] auto level_is_blit_shown_in_chasms_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_blit_shown_in_chasms, p);
}

[[nodiscard]] auto level_is_blit_shown_in_overlay_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_blit_shown_in_overlay, p);
}

[[nodiscard]] auto level_is_blit_square_outlined_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_blit_square_outlined, p);
}

[[nodiscard]] auto level_is_blit_when_obscured_faded_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_blit_when_obscured_faded, p);
}

[[nodiscard]] auto level_is_border_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_border, p);
}

[[nodiscard]] auto level_is_brazier_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_brazier, p);
}

[[nodiscard]] auto level_is_bridge_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_bridge, p);
}

[[nodiscard]] auto level_is_broken_on_death_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_broken_on_death, p);
}

[[nodiscard]] auto level_is_burnable_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_burnable, p);
}

[[nodiscard]] auto level_is_chasm_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_chasm, p);
}

[[nodiscard]] auto level_is_collectable_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_collectable, p);
}

[[nodiscard]] auto level_is_collision_circle_large_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_collision_circle_large, p);
}

[[nodiscard]] auto level_is_collision_circle_small_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_collision_circle_small, p);
}

[[nodiscard]] auto level_is_collision_detection_enabled_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_collision_detection_enabled, p);
}

[[nodiscard]] auto level_is_collision_square_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_collision_square, p);
}

[[nodiscard]] auto level_is_combustible_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_combustible, p);
}

[[nodiscard]] auto level_is_corpse_on_death_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_corpse_on_death, p);
}

[[nodiscard]] auto level_is_corridor_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_corridor, p);
}

[[nodiscard]] auto level_is_crushable_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_crushable, p);
}

[[nodiscard]] auto level_is_cursor_path_hazard_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_cursor_path_hazard, p);
}

[[nodiscard]] auto level_is_cursor_path_none_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_cursor_path_none, p);
}

[[nodiscard]] auto level_is_cursor_path_warning_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_cursor_path_warning, p);
}

[[nodiscard]] auto level_is_cursor_path_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_cursor_path, p);
}

[[nodiscard]] auto level_is_cursor_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_cursor, p);
}

[[nodiscard]] auto level_is_damage_capped_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_damage_capped, p);
}

[[nodiscard]] auto level_is_dead_on_collision_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_dead_on_collision, p);
}

[[nodiscard]] auto level_is_dead_on_shoving_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_dead_on_shoving, p);
}

[[nodiscard]] auto level_is_water_deep_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_water_deep, p);
}

[[nodiscard]] auto level_is_described_cursor_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_described_cursor, p);
}

[[nodiscard]] auto level_is_dirt_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_dirt, p);
}

[[nodiscard]] auto level_is_dmap_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_dmap, p);
}

[[nodiscard]] auto level_is_door_locked_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_door_locked, p);
}

[[nodiscard]] auto level_is_door_secret_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_door_secret, p);
}

[[nodiscard]] auto level_is_door_unlocked_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_door_unlocked, p);
}

[[nodiscard]] auto level_is_dungeon_entrance_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_dungeon_entrance, p);
}

[[nodiscard]] auto level_is_entrance_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_entrance, p);
}

[[nodiscard]] auto level_is_ethereal_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_ethereal, p);
}

[[nodiscard]] auto level_is_exit_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_exit, p);
}

[[nodiscard]] auto level_is_explosion_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_explosion, p);
}

[[nodiscard]] auto level_is_extinguished_on_death_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_extinguished_on_death, p);
}

[[nodiscard]] auto level_is_fire_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_fire, p);
}

[[nodiscard]] auto level_is_lucky_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_lucky, p);
}

[[nodiscard]] auto level_is_flesh_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_flesh, p);
}

[[nodiscard]] auto level_is_levitating_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_levitating, p);
}

[[nodiscard]] auto level_is_floor_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_floor, p);
}

[[nodiscard]] auto level_is_obs_to_ethereal_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_obs_to_ethereal, p);
}

[[nodiscard]] auto level_is_foliage_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_foliage, p);
}

[[nodiscard]] auto level_is_gaseous_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_gaseous, p);
}

[[nodiscard]] auto level_is_ghost_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_ghost, p);
}

[[nodiscard]] auto level_is_glass_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_glass, p);
}

[[nodiscard]] auto level_is_gold_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_gold, p);
}

[[nodiscard]] auto level_is_grass_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_grass, p);
}

[[nodiscard]] auto level_is_shown_health_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_shown_health, p);
}

[[nodiscard]] auto level_is_indestructible_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_indestructible, p);
}

[[nodiscard]] auto level_is_insectoid_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_insectoid, p);
}

[[nodiscard]] auto level_is_inventory_item_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_inventory_item, p);
}

[[nodiscard]] auto level_is_droppable_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_droppable, p);
}

[[nodiscard]] auto level_is_unused183_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused183, p);
}

[[nodiscard]] auto level_is_item_mergeable_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_item_mergeable, p);
}

[[nodiscard]] auto level_is_item_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_item, p);
}

[[nodiscard]] auto level_is_key_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_key, p);
}

[[nodiscard]] auto level_is_kobalos_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_kobalos, p);
}

[[nodiscard]] auto level_is_lava_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_lava, p);
}

[[nodiscard]] auto level_is_usable_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_usable, p);
}

[[nodiscard]] auto level_is_level_curr_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_level_curr, p);
}

[[nodiscard]] auto level_is_critical_to_dungeon_design_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_critical_to_dungeon_design, p);
}

[[nodiscard]] auto level_is_level_final_icon_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_level_final_icon, p);
}

[[nodiscard]] auto level_is_level_next_icon_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_level_next_icon, p);
}

[[nodiscard]] auto level_is_level_locked_icon_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_level_locked_icon, p);
}

[[nodiscard]] auto level_is_level_closed_icon_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_level_closed_icon, p);
}

[[nodiscard]] auto level_is_light_source_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_light_source, p);
}

[[nodiscard]] auto level_is_loggable_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_loggable, p);
}

[[nodiscard]] auto level_is_able_to_eat_items_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_able_to_eat_items, p);
}

[[nodiscard]] auto level_is_meltable_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_meltable, p);
}

[[nodiscard]] auto level_is_metal_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_metal, p);
}

[[nodiscard]] auto level_is_minion_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_minion, p);
}

[[nodiscard]] auto level_is_mob_kill_minions_on_death_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_mob_kill_minions_on_death, p);
}

[[nodiscard]] auto level_is_mob_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_mob, p);
}

[[nodiscard]] auto level_is_mob1_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_mob1, p);
}

[[nodiscard]] auto level_is_mob2_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_mob2, p);
}

[[nodiscard]] auto level_is_monst1_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_monst1, p);
}

[[nodiscard]] auto level_is_monst2_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_monst2, p);
}

[[nodiscard]] auto level_is_monst_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_monst, p);
}

[[nodiscard]] auto level_is_needs_move_confirm_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_needs_move_confirm, p);
}

[[nodiscard]] auto level_is_obs_to_cursor_path_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  if (is_oob(p)) {
    return true;
  }
  return level_flag_cached(g, v, l, is_obs_to_cursor_path, p);
}

[[nodiscard]] auto level_is_obs_to_explosion_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  if (is_oob(p)) {
    return true;
  }
  return level_flag_cached(g, v, l, is_obs_to_explosion, p);
}

[[nodiscard]] auto level_is_obs_to_falling_onto_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  if (is_oob(p)) {
    return true;
  }
  return level_flag_cached(g, v, l, is_obs_to_falling_onto, p);
}

[[nodiscard]] auto level_is_obs_to_fire_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  if (is_oob(p)) {
    return true;
  }
  return level_flag_cached(g, v, l, is_obs_to_fire, p);
}

[[nodiscard]] auto level_is_obs_to_jumping_over_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  if (is_oob(p)) {
    return true;
  }
  return level_flag_cached(g, v, l, is_obs_to_jumping_over, p);
}

[[nodiscard]] auto level_is_obs_to_jumping_onto_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  if (is_oob(p)) {
    return true;
  }
  return level_flag_cached(g, v, l, is_obs_to_jumping_onto, p);
}

[[nodiscard]] auto level_is_obs_to_jumping_out_of_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  if (is_oob(p)) {
    return true;
  }
  return level_flag_cached(g, v, l, is_obs_to_jumping_out_of, p);
}

[[nodiscard]] auto level_is_obs_to_movement_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  if (is_oob(p)) {
    return true;
  }
  return level_flag_cached(g, v, l, is_obs_to_movement, p);
}

[[nodiscard]] auto level_is_obs_to_spawning_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  if (is_oob(p)) {
    return true;
  }
  return level_flag_cached(g, v, l, is_obs_to_spawning, p);
}

[[nodiscard]] auto level_is_obs_to_teleporting_onto_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  if (is_oob(p)) {
    return true;
  }
  return level_flag_cached(g, v, l, is_obs_to_teleporting_onto, p);
}

[[nodiscard]] auto level_is_obs_to_vision_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  if (is_oob(p)) {
    return true;
  }
  return level_flag_cached(g, v, l, is_obs_to_vision, p);
}

[[nodiscard]] auto level_is_obs_when_dead_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  if (is_oob(p)) {
    return true;
  }
  return level_flag_cached(g, v, l, is_obs_when_dead, p);
}

[[nodiscard]] auto level_is_openable_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_openable, p);
}

[[nodiscard]] auto level_is_physics_explosion_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_physics_explosion, p);
}

[[nodiscard]] auto level_is_physics_temperature_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_physics_temperature, p);
}

[[nodiscard]] auto level_is_physics_water_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_physics_water, p);
}

[[nodiscard]] auto level_is_pillar_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_pillar, p);
}

[[nodiscard]] auto level_is_plant_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_plant, p);
}

[[nodiscard]] auto level_is_player_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_player, p);
}

[[nodiscard]] auto level_is_proj_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_projectile, p);
}

[[nodiscard]] auto level_is_removable_on_err_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_removable_on_err, p);
}

[[nodiscard]] auto level_is_removable_when_dead_on_err_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_removable_when_dead_on_err, p);
}

[[nodiscard]] auto level_is_rock_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_rock, p);
}

[[nodiscard]] auto level_is_shovable_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_shovable, p);
}

[[nodiscard]] auto level_is_slime_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_slime, p);
}

[[nodiscard]] auto level_is_smoke_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_smoke, p);
}

[[nodiscard]] auto level_is_steam_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_steam, p);
}

[[nodiscard]] auto level_is_stone_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_stone, p);
}

[[nodiscard]] auto level_is_submergible_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_submergible, p);
}

[[nodiscard]] auto level_is_able_to_be_teleported_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_able_to_be_teleported, p);
}

[[nodiscard]] auto level_is_teleport_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_teleport, p);
}

[[nodiscard]] auto level_is_tick_end_delay_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_tick_end_delay, p);
}

[[nodiscard]] auto level_is_tickable_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_tickable, p);
}

[[nodiscard]] auto level_is_tiled_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_tiled, p);
}

[[nodiscard]] auto level_is_trap_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_trap, p);
}

[[nodiscard]] auto level_is_treasure_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_treasure, p);
}

[[nodiscard]] auto level_is_undead_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_undead, p);
}

[[nodiscard]] auto level_is_unused1_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused1, p);
}

[[nodiscard]] auto level_is_unused2_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused2, p);
}

[[nodiscard]] auto level_is_unused3_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused3, p);
}

[[nodiscard]] auto level_is_unused4_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused4, p);
}

[[nodiscard]] auto level_is_unused5_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused5, p);
}

[[nodiscard]] auto level_is_unused6_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused6, p);
}

[[nodiscard]] auto level_is_unused7_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused7, p);
}

[[nodiscard]] auto level_is_unused8_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused8, p);
}

[[nodiscard]] auto level_is_unused9_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused9, p);
}

[[nodiscard]] auto level_is_unused10_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused10, p);
}

[[nodiscard]] auto level_is_unused11_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused11, p);
}

[[nodiscard]] auto level_is_unused12_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused12, p);
}

[[nodiscard]] auto level_is_unused13_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused13, p);
}

[[nodiscard]] auto level_is_unused14_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused14, p);
}

[[nodiscard]] auto level_is_unused15_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused15, p);
}

[[nodiscard]] auto level_is_unused16_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused16, p);
}

[[nodiscard]] auto level_is_unused17_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused17, p);
}

[[nodiscard]] auto level_is_unused18_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused18, p);
}

[[nodiscard]] auto level_is_unused19_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused19, p);
}

[[nodiscard]] auto level_is_unused20_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused20, p);
}

[[nodiscard]] auto level_is_unused21_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused21, p);
}

[[nodiscard]] auto level_is_unused22_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused22, p);
}

[[nodiscard]] auto level_is_unused23_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused23, p);
}

[[nodiscard]] auto level_is_unused24_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused24, p);
}

[[nodiscard]] auto level_is_unused25_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused25, p);
}

[[nodiscard]] auto level_is_unused26_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused26, p);
}

[[nodiscard]] auto level_is_unused27_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused27, p);
}

[[nodiscard]] auto level_is_unused28_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused28, p);
}

[[nodiscard]] auto level_is_unused29_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused29, p);
}

[[nodiscard]] auto level_is_unused30_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused30, p);
}

[[nodiscard]] auto level_is_unused31_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused31, p);
}

[[nodiscard]] auto level_is_unused32_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused32, p);
}

[[nodiscard]] auto level_is_unused33_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused33, p);
}

[[nodiscard]] auto level_is_unused34_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused34, p);
}

[[nodiscard]] auto level_is_unused35_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused35, p);
}

[[nodiscard]] auto level_is_unused36_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused36, p);
}

[[nodiscard]] auto level_is_unused37_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused37, p);
}

[[nodiscard]] auto level_is_unused38_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused38, p);
}

[[nodiscard]] auto level_is_unused39_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused39, p);
}

[[nodiscard]] auto level_is_unused40_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused40, p);
}

[[nodiscard]] auto level_is_unused41_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused41, p);
}

[[nodiscard]] auto level_is_unused42_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused42, p);
}

[[nodiscard]] auto level_is_unused43_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused43, p);
}

[[nodiscard]] auto level_is_unused44_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused44, p);
}

[[nodiscard]] auto level_is_unused45_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused45, p);
}

[[nodiscard]] auto level_is_unused46_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused46, p);
}

[[nodiscard]] auto level_is_unused47_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused47, p);
}

[[nodiscard]] auto level_is_unused48_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused48, p);
}

[[nodiscard]] auto level_is_unused49_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused49, p);
}

[[nodiscard]] auto level_is_unused50_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused50, p);
}

[[nodiscard]] auto level_is_unused51_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused51, p);
}

[[nodiscard]] auto level_is_unused52_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused52, p);
}

[[nodiscard]] auto level_is_unused53_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused53, p);
}

[[nodiscard]] auto level_is_unused54_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused54, p);
}

[[nodiscard]] auto level_is_unused55_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused55, p);
}

[[nodiscard]] auto level_is_unused56_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused56, p);
}

[[nodiscard]] auto level_is_unused57_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused57, p);
}

[[nodiscard]] auto level_is_unused58_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused58, p);
}

[[nodiscard]] auto level_is_unused59_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused59, p);
}

[[nodiscard]] auto level_is_unused60_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused60, p);
}

[[nodiscard]] auto level_is_unused61_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused61, p);
}

[[nodiscard]] auto level_is_unused62_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused62, p);
}

[[nodiscard]] auto level_is_unused63_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused63, p);
}

[[nodiscard]] auto level_is_unused64_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused64, p);
}

[[nodiscard]] auto level_is_unused65_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused65, p);
}

[[nodiscard]] auto level_is_unused66_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused66, p);
}

[[nodiscard]] auto level_is_unused67_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused67, p);
}

[[nodiscard]] auto level_is_unused68_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused68, p);
}

[[nodiscard]] auto level_is_unused69_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused69, p);
}

[[nodiscard]] auto level_is_unused70_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused70, p);
}

[[nodiscard]] auto level_is_unused71_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused71, p);
}

[[nodiscard]] auto level_is_unused72_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused72, p);
}

[[nodiscard]] auto level_is_unused73_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused73, p);
}

[[nodiscard]] auto level_is_unused74_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused74, p);
}

[[nodiscard]] auto level_is_unused75_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused75, p);
}

[[nodiscard]] auto level_is_unused76_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused76, p);
}

[[nodiscard]] auto level_is_unused77_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused77, p);
}

[[nodiscard]] auto level_is_unused78_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused78, p);
}

[[nodiscard]] auto level_is_unused79_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused79, p);
}

[[nodiscard]] auto level_is_unused80_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused80, p);
}

[[nodiscard]] auto level_is_unused81_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused81, p);
}

[[nodiscard]] auto level_is_unused82_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused82, p);
}

[[nodiscard]] auto level_is_unused83_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused83, p);
}

[[nodiscard]] auto level_is_unused84_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused84, p);
}

[[nodiscard]] auto level_is_unused85_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused85, p);
}

[[nodiscard]] auto level_is_unused86_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused86, p);
}

[[nodiscard]] auto level_is_unused87_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused87, p);
}

[[nodiscard]] auto level_is_unused88_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused88, p);
}

[[nodiscard]] auto level_is_unused89_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused89, p);
}

[[nodiscard]] auto level_is_unused90_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused90, p);
}

[[nodiscard]] auto level_is_unused91_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused91, p);
}

[[nodiscard]] auto level_is_unused92_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused92, p);
}

[[nodiscard]] auto level_is_unused93_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused93, p);
}

[[nodiscard]] auto level_is_unused94_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused94, p);
}

[[nodiscard]] auto level_is_unused95_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused95, p);
}

[[nodiscard]] auto level_is_unused96_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused96, p);
}

[[nodiscard]] auto level_is_unused97_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused97, p);
}

[[nodiscard]] auto level_is_unused98_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused98, p);
}

[[nodiscard]] auto level_is_unused99_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused99, p);
}

[[nodiscard]] auto level_is_unused100_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused100, p);
}

[[nodiscard]] auto level_is_unused101_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused101, p);
}

[[nodiscard]] auto level_is_unused102_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused102, p);
}

[[nodiscard]] auto level_is_unused103_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused103, p);
}

[[nodiscard]] auto level_is_unused104_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused104, p);
}

[[nodiscard]] auto level_is_unused105_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused105, p);
}

[[nodiscard]] auto level_is_unused106_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused106, p);
}

[[nodiscard]] auto level_is_unused107_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused107, p);
}

[[nodiscard]] auto level_is_unused108_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused108, p);
}

[[nodiscard]] auto level_is_unused109_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused109, p);
}

[[nodiscard]] auto level_is_unused110_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused110, p);
}

[[nodiscard]] auto level_is_unused111_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused111, p);
}

[[nodiscard]] auto level_is_unused112_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused112, p);
}

[[nodiscard]] auto level_is_unused113_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused113, p);
}

[[nodiscard]] auto level_is_unused114_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused114, p);
}

[[nodiscard]] auto level_is_unused115_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused115, p);
}

[[nodiscard]] auto level_is_unused116_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused116, p);
}

[[nodiscard]] auto level_is_unused117_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused117, p);
}

[[nodiscard]] auto level_is_unused118_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused118, p);
}

[[nodiscard]] auto level_is_unused119_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused119, p);
}

[[nodiscard]] auto level_is_unused120_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused120, p);
}

[[nodiscard]] auto level_is_unused121_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused121, p);
}

[[nodiscard]] auto level_is_unused122_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused122, p);
}

[[nodiscard]] auto level_is_unused123_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused123, p);
}

[[nodiscard]] auto level_is_unused124_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused124, p);
}

[[nodiscard]] auto level_is_unused125_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused125, p);
}

[[nodiscard]] auto level_is_unused126_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused126, p);
}

[[nodiscard]] auto level_is_unused127_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused127, p);
}

[[nodiscard]] auto level_is_unused128_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused128, p);
}

[[nodiscard]] auto level_is_unused129_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused129, p);
}

[[nodiscard]] auto level_is_unused130_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused130, p);
}

[[nodiscard]] auto level_is_unused131_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused131, p);
}

[[nodiscard]] auto level_is_unused132_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused132, p);
}

[[nodiscard]] auto level_is_unused133_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused133, p);
}

[[nodiscard]] auto level_is_unused134_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused134, p);
}

[[nodiscard]] auto level_is_unused135_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused135, p);
}

[[nodiscard]] auto level_is_unused136_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused136, p);
}

[[nodiscard]] auto level_is_unused137_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused137, p);
}

[[nodiscard]] auto level_is_unused138_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused138, p);
}

[[nodiscard]] auto level_is_unused139_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused139, p);
}

[[nodiscard]] auto level_is_unused140_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused140, p);
}

[[nodiscard]] auto level_is_unused141_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused141, p);
}

[[nodiscard]] auto level_is_unused142_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused142, p);
}

[[nodiscard]] auto level_is_unused143_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused143, p);
}

[[nodiscard]] auto level_is_unused144_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused144, p);
}

[[nodiscard]] auto level_is_unused145_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused145, p);
}

[[nodiscard]] auto level_is_unused146_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused146, p);
}

[[nodiscard]] auto level_is_unused147_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused147, p);
}

[[nodiscard]] auto level_is_unused148_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused148, p);
}

[[nodiscard]] auto level_is_unused149_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused149, p);
}

[[nodiscard]] auto level_is_unused150_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused150, p);
}

[[nodiscard]] auto level_is_unused151_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused151, p);
}

[[nodiscard]] auto level_is_unused152_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused152, p);
}

[[nodiscard]] auto level_is_unused153_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused153, p);
}

[[nodiscard]] auto level_is_weaponless_on_spawn_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_weaponless_on_spawn, p);
}

[[nodiscard]] auto level_is_noisy_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_noisy, p);
}

[[nodiscard]] auto level_is_hook_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_hook, p);
}

[[nodiscard]] auto level_is_sacrifice_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_sacrifice, p);
}

[[nodiscard]] auto level_is_debuff_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_debuff, p);
}

[[nodiscard]] auto level_is_venomous_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_venomous, p);
}

[[nodiscard]] auto level_is_able_to_see_invisible_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_able_to_see_invisible, p);
}

[[nodiscard]] auto level_is_able_to_be_invisible_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_able_to_be_invisible, p);
}

[[nodiscard]] auto level_is_invisible_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_invisible, p);
}

[[nodiscard]] auto level_is_critical_to_gameplay_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_critical_to_gameplay, p);
}

[[nodiscard]] auto level_is_able_to_jump_attack_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_able_to_jump_attack, p);
}

[[nodiscard]] auto level_is_fish_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_fish, p);
}

[[nodiscard]] auto level_is_unlucky_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unlucky, p);
}

[[nodiscard]] auto level_is_cursed_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_cursed, p);
}

[[nodiscard]] auto level_is_chocolate_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_chocolate, p);
}

[[nodiscard]] auto level_is_tick_on_eaten_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_tick_on_eaten, p);
}

[[nodiscard]] auto level_is_able_to_eat_food_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_able_to_eat_food, p);
}

[[nodiscard]] auto level_is_edible_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_edible, p);
}

[[nodiscard]] auto level_is_able_to_be_engulfed_blocked_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_able_to_be_engulfed_blocked, p);
}

[[nodiscard]] auto level_is_able_to_be_ensnared_blocked_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_able_to_be_ensnared_blocked, p);
}

[[nodiscard]] auto level_is_active_when_worn_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_active_when_worn, p);
}

[[nodiscard]] auto level_is_leather_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_leather, p);
}

[[nodiscard]] auto level_is_boots_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_boots, p);
}

[[nodiscard]] auto level_is_spider_giant_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_spider_giant, p);
}

[[nodiscard]] auto level_is_spider_baby_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_spider_baby, p);
}

[[nodiscard]] auto level_is_able_to_choose_targets_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_able_to_choose_targets, p);
}

[[nodiscard]] auto level_is_flesh_eater_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_flesh_eater, p);
}

[[nodiscard]] auto level_is_able_to_move_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_able_to_move, p);
}

[[nodiscard]] auto level_is_spider_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_spider, p);
}

[[nodiscard]] auto level_is_able_to_be_ensnared_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_able_to_be_ensnared, p);
}

[[nodiscard]] auto level_is_able_to_ensnare_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_able_to_ensnare, p);
}

[[nodiscard]] auto level_is_spiderweb_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_spiderweb, p);
}

[[nodiscard]] auto level_is_rubble_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_rubble, p);
}

[[nodiscard]] auto level_is_able_to_jump_land_then_pounce_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_able_to_jump_land_then_pounce, p);
}

[[nodiscard]] auto level_is_unused188_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused188, p);
}

[[nodiscard]] auto level_is_skeleton_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_skeleton, p);
}

[[nodiscard]] auto level_is_water_bound_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_water_bound, p);
}

[[nodiscard]] auto level_is_amphibious_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_amphibious, p);
}

[[nodiscard]] auto level_is_able_to_resurrect_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_able_to_resurrect, p);
}

[[nodiscard]] auto level_is_soft_landing_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_soft_landing, p);
}

[[nodiscard]] auto level_is_gelatinous_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_gelatinous, p);
}

[[nodiscard]] auto level_is_pot_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_potion, p);
}

[[nodiscard]] auto level_is_able_to_be_levitated_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_able_to_be_levitated, p);
}

[[nodiscard]] auto level_is_teleport_hazard_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_teleport_hazard, p);
}

[[nodiscard]] auto level_is_active_when_carried_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_active_when_carried, p);
}

[[nodiscard]] auto level_is_wearable_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_wearable, p);
}

[[nodiscard]] auto level_is_ring_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_ring, p);
}

[[nodiscard]] auto level_is_obs_to_hearing_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  if (is_oob(p)) {
    return true;
  }
  return level_flag_cached(g, v, l, is_obs_to_hearing, p);
}

[[nodiscard]] auto level_is_throwable_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_throwable, p);
}

[[nodiscard]] auto level_is_obs_to_throwing_over_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  if (is_oob(p)) {
    return true;
  }
  return level_flag_cached(g, v, l, is_obs_to_throwing_over, p);
}

[[nodiscard]] auto level_is_obs_to_throwing_onto_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  if (is_oob(p)) {
    return true;
  }
  return level_flag_cached(g, v, l, is_obs_to_throwing_onto, p);
}

[[nodiscard]] auto level_is_physics_trap_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_physics_trap, p);
}

[[nodiscard]] auto level_is_tick_on_strip_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_tick_on_strip, p);
}

[[nodiscard]] auto level_is_tick_on_worn_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_tick_on_worn, p);
}

[[nodiscard]] auto level_is_buff_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_buff, p);
}

[[nodiscard]] auto level_is_able_to_be_buffed_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_able_to_be_buffed, p);
}

[[nodiscard]] auto level_is_tick_on_drop_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_tick_on_drop, p);
}

[[nodiscard]] auto level_is_lava_bg_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_lava_bg, p);
}

[[nodiscard]] auto level_is_tick_on_use_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_tick_on_use, p);
}

[[nodiscard]] auto level_is_able_to_throw_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_able_to_throw, p);
}

[[nodiscard]] auto level_is_described_when_killed_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_described_when_killed, p);
}

[[nodiscard]] auto level_is_chest_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_chest, p);
}

[[nodiscard]] auto level_is_flammable_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_flammable, p);
}

[[nodiscard]] auto level_is_auto_wear_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_auto_wear, p);
}

[[nodiscard]] auto level_is_dead_when_discharged_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_dead_when_discharged, p);
}

[[nodiscard]] auto level_is_able_to_fire_weapons_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_able_to_fire_weapons, p);
}

[[nodiscard]] auto level_is_weapon_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_weapon, p);
}

[[nodiscard]] auto level_is_light_flicker_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_light_flicker, p);
}

[[nodiscard]] auto level_is_able_to_wear_items_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_able_to_wear_items, p);
}

[[nodiscard]] auto level_is_wand_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_wand, p);
}

[[nodiscard]] auto level_is_staff_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_staff, p);
}

[[nodiscard]] auto level_is_able_to_eat_treasure_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_able_to_eat_treasure, p);
}

[[nodiscard]] auto level_is_wait_on_anim_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_wait_on_anim, p);
}

[[nodiscard]] auto level_is_flat_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_flat, p);
}

[[nodiscard]] auto level_is_obs_to_beam_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  if (is_oob(p)) {
    return true;
  }
  return level_flag_cached(g, v, l, is_obs_to_beam, p);
}

[[nodiscard]] auto level_is_beam_weapon_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_beam_weapon, p);
}

[[nodiscard]] auto level_is_shown_stamina_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_shown_stamina, p);
}

[[nodiscard]] auto level_is_reeds_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_reeds, p);
}

[[nodiscard]] auto level_is_able_to_drop_all_items_on_death_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_able_to_drop_all_items_on_death, p);
}

[[nodiscard]] auto level_is_vault_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_vault, p);
}

[[nodiscard]] auto level_is_biome_underhell_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_biome_underhell, p);
}

[[nodiscard]] auto level_is_biome_graveyard_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_biome_graveyard, p);
}

[[nodiscard]] auto level_is_biome_nethervoid_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_biome_nethervoid, p);
}

[[nodiscard]] auto level_is_biome_bogland_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_biome_bogland, p);
}

[[nodiscard]] auto level_is_biome_dungeon_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_biome_dungeon, p);
}

[[nodiscard]] auto level_is_level_open_icon_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_level_open_icon, p);
}

[[nodiscard]] auto level_is_blit_bg_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_blit_bg, p);
}

[[nodiscard]] auto level_is_level_select_bg_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_level_select_bg, p);
}

[[nodiscard]] auto level_is_blit_hit_outline_w_black_inside_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_blit_hit_outline_w_black_inside, p);
}

[[nodiscard]] auto level_is_able_to_engulf_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_able_to_engulf, p);
}

[[nodiscard]] auto level_is_able_to_fall_sound_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_able_to_fall_sound, p);
}

[[nodiscard]] auto level_is_collision_hit_first_on_tile_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_collision_hit_first_on_tile, p);
}

[[nodiscard]] auto level_is_collision_hit_all_on_tile_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_collision_hit_all_on_tile, p);
}

[[nodiscard]] auto level_is_blit_hit_outline_w_invis_inside_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_blit_hit_outline_w_invis_inside, p);
}

[[nodiscard]] auto level_is_blit_when_obscured_outline_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_blit_when_obscured_outline, p);
}

[[nodiscard]] auto level_is_attackable_by_player_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_attackable_by_player, p);
}

[[nodiscard]] auto level_is_attackable_by_monst_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_attackable_by_monst, p);
}

[[nodiscard]] auto level_is_effect_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_effect, p);
}

[[nodiscard]] auto level_is_effect_attack_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_effect_attack, p);
}

[[nodiscard]] auto level_is_effect_blood_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_effect_blood, p);
}

[[nodiscard]] auto level_is_effect_ripple_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_effect_ripple, p);
}

[[nodiscard]] auto level_is_hit_when_dead_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_hit_when_dead, p);
}

[[nodiscard]] auto level_is_able_to_be_engulfed_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_able_to_be_engulfed, p);
}

[[nodiscard]] auto level_is_grouped_thing_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_grouped_thing, p);
}

[[nodiscard]] auto level_is_vision_360_degrees_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_vision_360_degrees, p);
}

[[nodiscard]] auto level_is_vision_180_degrees_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_vision_180_degrees, p);
}

[[nodiscard]] auto level_is_able_to_lunge_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_able_to_lunge, p);
}

[[nodiscard]] auto level_is_obs_to_paths_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  if (is_oob(p)) {
    return true;
  }
  return level_flag_cached(g, v, l, is_obs_to_paths, p);
}

[[nodiscard]] auto level_is_stealthy_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_stealthy, p);
}

[[nodiscard]] auto level_is_tireless_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_tireless, p);
}

[[nodiscard]] auto level_is_shown_noise_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_shown_noise, p);
}

[[nodiscard]] auto level_is_wait_on_dead_anim_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_wait_on_dead_anim, p);
}

[[nodiscard]] auto level_is_wall_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_wall, p);
}

[[nodiscard]] auto level_is_water_shallow_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_water_shallow, p);
}

[[nodiscard]] auto level_is_wood_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_wood, p);
}
