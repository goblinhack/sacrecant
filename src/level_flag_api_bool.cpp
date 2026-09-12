//
// Copyright goblinhack@gmail.com
//

#include "my_bpoint.hpp"
#include "my_callstack.hpp"
#include "my_level.hpp"
#include "my_level_inlines.hpp"
#include "my_tp.hpp"
#include "my_types.hpp"

[[nodiscard]] auto level_is_able_to_collect_items_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_able_to_collect_items, p, me) != nullptr;
}

[[nodiscard]] auto level_is_able_to_collect_keys_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_able_to_collect_keys, p, me) != nullptr;
}

[[nodiscard]] auto level_is_able_to_crush_grass_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_able_to_crush_grass, p, me) != nullptr;
}

[[nodiscard]] auto level_is_able_to_fall_repeatedly_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_able_to_fall_repeatedly, p, me) != nullptr;
}

[[nodiscard]] auto level_is_able_to_fall_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_able_to_fall, p, me) != nullptr;
}

[[nodiscard]] auto level_is_able_to_jump_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_able_to_jump, p, me) != nullptr;
}

[[nodiscard]] auto level_is_able_to_move_diagonally_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_able_to_move_diagonally, p, me) != nullptr;
}

[[nodiscard]] auto level_is_able_to_move_through_walls_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_able_to_move_through_walls, p, me) != nullptr;
}

[[nodiscard]] auto level_is_able_to_open_things_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_able_to_open_things, p, me) != nullptr;
}

[[nodiscard]] auto level_is_able_to_shove_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_able_to_shove, p, me) != nullptr;
}

[[nodiscard]] auto level_is_able_to_see_through_walls_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_able_to_see_through_walls, p, me) != nullptr;
}

[[nodiscard]] auto level_is_animated_can_hflip_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_animated_can_hflip, p, me) != nullptr;
}

[[nodiscard]] auto level_is_animated_no_dir_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_animated_no_dir, p, me) != nullptr;
}

[[nodiscard]] auto level_is_animated_sync_first_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_animated_sync_first, p, me) != nullptr;
}

[[nodiscard]] auto level_is_animated_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_animated, p, me) != nullptr;
}

[[nodiscard]] auto level_is_barrel_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_barrel, p, me) != nullptr;
}

[[nodiscard]] auto level_is_blit_centered_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_blit_centered, p, me) != nullptr;
}

[[nodiscard]] auto level_is_blit_flush_per_line_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_blit_flush_per_line, p, me) != nullptr;
}

[[nodiscard]] auto level_is_blit_if_has_seen_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_blit_if_has_seen, p, me) != nullptr;
}

[[nodiscard]] auto level_is_blit_obscures_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_blit_obscures, p, me) != nullptr;
}

[[nodiscard]] auto level_is_blit_on_ground_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_blit_on_ground, p, me) != nullptr;
}

[[nodiscard]] auto level_is_blit_outlined_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_blit_outlined, p, me) != nullptr;
}

[[nodiscard]] auto level_is_blit_per_pixel_lighting_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_blit_per_pixel_lighting, p, me) != nullptr;
}

[[nodiscard]] auto level_is_blit_shown_in_chasms_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_blit_shown_in_chasms, p, me) != nullptr;
}

[[nodiscard]] auto level_is_blit_shown_in_overlay_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_blit_shown_in_overlay, p, me) != nullptr;
}

[[nodiscard]] auto level_is_blit_square_outlined_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_blit_square_outlined, p, me) != nullptr;
}

[[nodiscard]] auto level_is_blit_when_obscured_faded_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_blit_when_obscured_faded, p, me) != nullptr;
}

[[nodiscard]] auto level_is_border_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_border, p, me) != nullptr;
}

[[nodiscard]] auto level_is_brazier_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_brazier, p, me) != nullptr;
}

[[nodiscard]] auto level_is_bridge_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_bridge, p, me) != nullptr;
}

[[nodiscard]] auto level_is_broken_on_death_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_broken_on_death, p, me) != nullptr;
}

[[nodiscard]] auto level_is_burnable_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_burnable, p, me) != nullptr;
}

[[nodiscard]] auto level_is_chasm_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_chasm, p, me) != nullptr;
}

[[nodiscard]] auto level_is_collectable_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_collectable, p, me) != nullptr;
}

[[nodiscard]] auto level_is_collision_circle_large_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_collision_circle_large, p, me) != nullptr;
}

[[nodiscard]] auto level_is_collision_circle_small_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_collision_circle_small, p, me) != nullptr;
}

[[nodiscard]] auto level_is_collision_detection_enabled_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_collision_detection_enabled, p, me) != nullptr;
}

[[nodiscard]] auto level_is_collision_square_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_collision_square, p, me) != nullptr;
}

[[nodiscard]] auto level_is_combustible_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_combustible, p, me) != nullptr;
}

[[nodiscard]] auto level_is_corpse_on_death_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_corpse_on_death, p, me) != nullptr;
}

[[nodiscard]] auto level_is_corridor_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_corridor, p, me) != nullptr;
}

[[nodiscard]] auto level_is_crushable_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_crushable, p, me) != nullptr;
}

[[nodiscard]] auto level_is_cursor_path_hazard_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_cursor_path_hazard, p, me) != nullptr;
}

[[nodiscard]] auto level_is_cursor_path_none_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_cursor_path_none, p, me) != nullptr;
}

[[nodiscard]] auto level_is_cursor_path_warning_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_cursor_path_warning, p, me) != nullptr;
}

[[nodiscard]] auto level_is_cursor_path_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_cursor_path, p, me) != nullptr;
}

[[nodiscard]] auto level_is_cursor_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_cursor, p, me) != nullptr;
}

[[nodiscard]] auto level_is_damage_capped_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_damage_capped, p, me) != nullptr;
}

[[nodiscard]] auto level_is_dead_on_collision_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_dead_on_collision, p, me) != nullptr;
}

[[nodiscard]] auto level_is_dead_on_shoving_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_dead_on_shoving, p, me) != nullptr;
}

[[nodiscard]] auto level_is_water_deep_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_water_deep, p, me) != nullptr;
}

[[nodiscard]] auto level_is_described_cursor_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_described_cursor, p, me) != nullptr;
}

[[nodiscard]] auto level_is_dirt_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_dirt, p, me) != nullptr;
}

[[nodiscard]] auto level_is_dmap_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_dmap, p, me) != nullptr;
}

[[nodiscard]] auto level_is_door_locked_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_door_locked, p, me) != nullptr;
}

[[nodiscard]] auto level_is_door_secret_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_door_secret, p, me) != nullptr;
}

[[nodiscard]] auto level_is_door_unlocked_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_door_unlocked, p, me) != nullptr;
}

[[nodiscard]] auto level_is_dungeon_entrance_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_dungeon_entrance, p, me) != nullptr;
}

[[nodiscard]] auto level_is_entrance_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_entrance, p, me) != nullptr;
}

[[nodiscard]] auto level_is_ethereal_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_ethereal, p, me) != nullptr;
}

[[nodiscard]] auto level_is_exit_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_exit, p, me) != nullptr;
}

[[nodiscard]] auto level_is_explosion_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_explosion, p, me) != nullptr;
}

[[nodiscard]] auto level_is_extinguished_on_death_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_extinguished_on_death, p, me) != nullptr;
}

[[nodiscard]] auto level_is_fire_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_fire, p, me) != nullptr;
}

[[nodiscard]] auto level_is_lucky_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_lucky, p, me) != nullptr;
}

[[nodiscard]] auto level_is_flesh_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_flesh, p, me) != nullptr;
}

[[nodiscard]] auto level_is_levitating_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_levitating, p, me) != nullptr;
}

[[nodiscard]] auto level_is_floor_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_floor, p, me) != nullptr;
}

[[nodiscard]] auto level_is_obs_to_ethereal_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_obs_to_ethereal, p, me) != nullptr;
}

[[nodiscard]] auto level_is_foliage_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_foliage, p, me) != nullptr;
}

[[nodiscard]] auto level_is_gaseous_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_gaseous, p, me) != nullptr;
}

[[nodiscard]] auto level_is_ghost_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_ghost, p, me) != nullptr;
}

[[nodiscard]] auto level_is_glass_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_glass, p, me) != nullptr;
}

[[nodiscard]] auto level_is_gold_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_gold, p, me) != nullptr;
}

[[nodiscard]] auto level_is_grass_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_grass, p, me) != nullptr;
}

[[nodiscard]] auto level_is_shown_health_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_shown_health, p, me) != nullptr;
}

[[nodiscard]] auto level_is_indestructible_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_indestructible, p, me) != nullptr;
}

[[nodiscard]] auto level_is_insectoid_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_insectoid, p, me) != nullptr;
}

[[nodiscard]] auto level_is_inventory_item_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_inventory_item, p, me) != nullptr;
}

[[nodiscard]] auto level_is_droppable_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_droppable, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused183_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused183, p, me) != nullptr;
}

[[nodiscard]] auto level_is_item_mergeable_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_item_mergeable, p, me) != nullptr;
}

[[nodiscard]] auto level_is_item_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_item, p, me) != nullptr;
}

[[nodiscard]] auto level_is_key_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_key, p, me) != nullptr;
}

[[nodiscard]] auto level_is_kobalos_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_kobalos, p, me) != nullptr;
}

[[nodiscard]] auto level_is_lava_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_lava, p, me) != nullptr;
}

[[nodiscard]] auto level_is_usable_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_usable, p, me) != nullptr;
}

[[nodiscard]] auto level_is_level_curr_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_level_curr, p, me) != nullptr;
}

[[nodiscard]] auto level_is_critical_to_dungeon_design_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_critical_to_dungeon_design, p, me) != nullptr;
}

[[nodiscard]] auto level_is_level_final_icon_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_level_final_icon, p, me) != nullptr;
}

[[nodiscard]] auto level_is_level_next_icon_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_level_next_icon, p, me) != nullptr;
}

[[nodiscard]] auto level_is_level_locked_icon_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_level_locked_icon, p, me) != nullptr;
}

[[nodiscard]] auto level_is_level_closed_icon_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_level_closed_icon, p, me) != nullptr;
}

[[nodiscard]] auto level_is_light_source_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_light_source, p, me) != nullptr;
}

[[nodiscard]] auto level_is_loggable_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_loggable, p, me) != nullptr;
}

[[nodiscard]] auto level_is_able_to_eat_items_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_able_to_eat_items, p, me) != nullptr;
}

[[nodiscard]] auto level_is_meltable_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_meltable, p, me) != nullptr;
}

[[nodiscard]] auto level_is_metal_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_metal, p, me) != nullptr;
}

[[nodiscard]] auto level_is_minion_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_minion, p, me) != nullptr;
}

[[nodiscard]] auto level_is_mob_kill_minions_on_death_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_mob_kill_minions_on_death, p, me) != nullptr;
}

[[nodiscard]] auto level_is_mob_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_mob, p, me) != nullptr;
}

[[nodiscard]] auto level_is_mob1_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_mob1, p, me) != nullptr;
}

[[nodiscard]] auto level_is_mob2_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_mob2, p, me) != nullptr;
}

[[nodiscard]] auto level_is_monst1_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_monst1, p, me) != nullptr;
}

[[nodiscard]] auto level_is_monst2_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_monst2, p, me) != nullptr;
}

[[nodiscard]] auto level_is_monst_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_monst, p, me) != nullptr;
}

[[nodiscard]] auto level_is_needs_move_confirm_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_needs_move_confirm, p, me) != nullptr;
}

[[nodiscard]] auto level_is_obs_to_cursor_path_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  if (is_oob(p)) {
    return true;
  }
  return level_flag(g, v, l, is_obs_to_cursor_path, p, me) != nullptr;
}

[[nodiscard]] auto level_is_obs_to_explosion_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  if (is_oob(p)) {
    return true;
  }
  return level_flag(g, v, l, is_obs_to_explosion, p, me) != nullptr;
}

[[nodiscard]] auto level_is_obs_to_falling_onto_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  if (is_oob(p)) {
    return true;
  }
  return level_flag(g, v, l, is_obs_to_falling_onto, p, me) != nullptr;
}

[[nodiscard]] auto level_is_obs_to_fire_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  if (is_oob(p)) {
    return true;
  }
  return level_flag(g, v, l, is_obs_to_fire, p, me) != nullptr;
}

[[nodiscard]] auto level_is_obs_to_jumping_over_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  if (is_oob(p)) {
    return true;
  }
  return level_flag(g, v, l, is_obs_to_jumping_over, p, me) != nullptr;
}

[[nodiscard]] auto level_is_obs_to_jumping_onto_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  if (is_oob(p)) {
    return true;
  }
  return level_flag(g, v, l, is_obs_to_jumping_onto, p, me) != nullptr;
}

[[nodiscard]] auto level_is_obs_to_jumping_out_of_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  if (is_oob(p)) {
    return true;
  }
  return level_flag(g, v, l, is_obs_to_jumping_out_of, p, me) != nullptr;
}

[[nodiscard]] auto level_is_obs_to_movement_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  if (is_oob(p)) {
    return true;
  }
  return level_flag(g, v, l, is_obs_to_movement, p, me) != nullptr;
}

[[nodiscard]] auto level_is_obs_to_spawning_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  if (is_oob(p)) {
    return true;
  }
  return level_flag(g, v, l, is_obs_to_spawning, p, me) != nullptr;
}

[[nodiscard]] auto level_is_obs_to_teleporting_onto_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  if (is_oob(p)) {
    return true;
  }
  return level_flag(g, v, l, is_obs_to_teleporting_onto, p, me) != nullptr;
}

[[nodiscard]] auto level_is_obs_to_vision_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  if (is_oob(p)) {
    return true;
  }
  return level_flag(g, v, l, is_obs_to_vision, p, me) != nullptr;
}

[[nodiscard]] auto level_is_obs_when_dead_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  if (is_oob(p)) {
    return true;
  }
  return level_flag(g, v, l, is_obs_when_dead, p, me) != nullptr;
}

[[nodiscard]] auto level_is_openable_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_openable, p, me) != nullptr;
}

[[nodiscard]] auto level_is_physics_explosion_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_physics_explosion, p, me) != nullptr;
}

[[nodiscard]] auto level_is_physics_temperature_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_physics_temperature, p, me) != nullptr;
}

[[nodiscard]] auto level_is_physics_water_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_physics_water, p, me) != nullptr;
}

[[nodiscard]] auto level_is_pillar_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_pillar, p, me) != nullptr;
}

[[nodiscard]] auto level_is_plant_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_plant, p, me) != nullptr;
}

[[nodiscard]] auto level_is_player_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_player, p, me) != nullptr;
}

[[nodiscard]] auto level_is_proj_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_projectile, p, me) != nullptr;
}

[[nodiscard]] auto level_is_removable_on_err_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_removable_on_err, p, me) != nullptr;
}

[[nodiscard]] auto level_is_removable_when_dead_on_err_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_removable_when_dead_on_err, p, me) != nullptr;
}

[[nodiscard]] auto level_is_rock_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_rock, p, me) != nullptr;
}

[[nodiscard]] auto level_is_shovable_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_shovable, p, me) != nullptr;
}

[[nodiscard]] auto level_is_slime_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_slime, p, me) != nullptr;
}

[[nodiscard]] auto level_is_smoke_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_smoke, p, me) != nullptr;
}

[[nodiscard]] auto level_is_steam_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_steam, p, me) != nullptr;
}

[[nodiscard]] auto level_is_stone_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_stone, p, me) != nullptr;
}

[[nodiscard]] auto level_is_submergible_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_submergible, p, me) != nullptr;
}

[[nodiscard]] auto level_is_able_to_be_teleported_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_able_to_be_teleported, p, me) != nullptr;
}

[[nodiscard]] auto level_is_teleport_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_teleport, p, me) != nullptr;
}

[[nodiscard]] auto level_is_tick_end_delay_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_tick_end_delay, p, me) != nullptr;
}

[[nodiscard]] auto level_is_tickable_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_tickable, p, me) != nullptr;
}

[[nodiscard]] auto level_is_tiled_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_tiled, p, me) != nullptr;
}

[[nodiscard]] auto level_is_trap_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_trap, p, me) != nullptr;
}

[[nodiscard]] auto level_is_treasure_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_treasure, p, me) != nullptr;
}

[[nodiscard]] auto level_is_undead_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_undead, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused1_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused1, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused2_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused2, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused3_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused3, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused4_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused4, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused5_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused5, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused6_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused6, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused7_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused7, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused8_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused8, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused9_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused9, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused10_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused10, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused11_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused11, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused12_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused12, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused13_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused13, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused14_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused14, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused15_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused15, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused16_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused16, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused17_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused17, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused18_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused18, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused19_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused19, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused20_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused20, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused21_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused21, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused22_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused22, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused23_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused23, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused24_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused24, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused25_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused25, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused26_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused26, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused27_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused27, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused28_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused28, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused29_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused29, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused30_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused30, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused31_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused31, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused32_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused32, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused33_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused33, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused34_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused34, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused35_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused35, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused36_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused36, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused37_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused37, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused38_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused38, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused39_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused39, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused40_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused40, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused41_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused41, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused42_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused42, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused43_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused43, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused44_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused44, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused45_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused45, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused46_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused46, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused47_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused47, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused48_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused48, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused49_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused49, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused50_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused50, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused51_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused51, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused52_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused52, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused53_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused53, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused54_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused54, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused55_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused55, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused56_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused56, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused57_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused57, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused58_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused58, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused59_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused59, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused60_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused60, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused61_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused61, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused62_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused62, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused63_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused63, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused64_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused64, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused65_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused65, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused66_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused66, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused67_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused67, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused68_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused68, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused69_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused69, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused70_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused70, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused71_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused71, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused72_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused72, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused73_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused73, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused74_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused74, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused75_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused75, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused76_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused76, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused77_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused77, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused78_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused78, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused79_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused79, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused80_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused80, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused81_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused81, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused82_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused82, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused83_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused83, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused84_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused84, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused85_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused85, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused86_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused86, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused87_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused87, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused88_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused88, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused89_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused89, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused90_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused90, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused91_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused91, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused92_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused92, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused93_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused93, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused94_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused94, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused95_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused95, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused96_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused96, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused97_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused97, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused98_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused98, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused99_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused99, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused100_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused100, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused101_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused101, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused102_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused102, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused103_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused103, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused104_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused104, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused105_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused105, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused106_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused106, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused107_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused107, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused108_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused108, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused109_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused109, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused110_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused110, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused111_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused111, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused112_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused112, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused113_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused113, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused114_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused114, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused115_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused115, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused116_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused116, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused117_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused117, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused118_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused118, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused119_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused119, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused120_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused120, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused121_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused121, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused122_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused122, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused123_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused123, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused124_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused124, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused125_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused125, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused126_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused126, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused127_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused127, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused128_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused128, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused129_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused129, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused130_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused130, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused131_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused131, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused132_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused132, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused133_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused133, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused134_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused134, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused135_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused135, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused136_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused136, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused137_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused137, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused138_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused138, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused139_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused139, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused140_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused140, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused141_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused141, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused142_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused142, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused143_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused143, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused144_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused144, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused145_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused145, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused146_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused146, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused147_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused147, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused148_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused148, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused149_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused149, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused150_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused150, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused151_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused151, p, me) != nullptr;
}

[[nodiscard]] auto level_is_myopic_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_myopic, p, me) != nullptr;
}

[[nodiscard]] auto level_is_boost_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_boost, p, me) != nullptr;
}

[[nodiscard]] auto level_is_weaponless_on_spawn_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_weaponless_on_spawn, p, me) != nullptr;
}

[[nodiscard]] auto level_is_noisy_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_noisy, p, me) != nullptr;
}

[[nodiscard]] auto level_is_hook_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_hook, p, me) != nullptr;
}

[[nodiscard]] auto level_is_sacrifice_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_sacrifice, p, me) != nullptr;
}

[[nodiscard]] auto level_is_debuff_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_debuff, p, me) != nullptr;
}

[[nodiscard]] auto level_is_venomous_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_venomous, p, me) != nullptr;
}

[[nodiscard]] auto level_is_able_to_see_invisible_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_able_to_see_invisible, p, me) != nullptr;
}

[[nodiscard]] auto level_is_able_to_be_invisible_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_able_to_be_invisible, p, me) != nullptr;
}

[[nodiscard]] auto level_is_invisible_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_invisible, p, me) != nullptr;
}

[[nodiscard]] auto level_is_critical_to_gameplay_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_critical_to_gameplay, p, me) != nullptr;
}

[[nodiscard]] auto level_is_able_to_jump_attack_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_able_to_jump_attack, p, me) != nullptr;
}

[[nodiscard]] auto level_is_fish_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_fish, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unlucky_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unlucky, p, me) != nullptr;
}

[[nodiscard]] auto level_is_cursed_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_cursed, p, me) != nullptr;
}

[[nodiscard]] auto level_is_chocolate_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_chocolate, p, me) != nullptr;
}

[[nodiscard]] auto level_is_tick_on_eaten_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_tick_on_eaten, p, me) != nullptr;
}

[[nodiscard]] auto level_is_able_to_eat_food_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_able_to_eat_food, p, me) != nullptr;
}

[[nodiscard]] auto level_is_edible_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_edible, p, me) != nullptr;
}

[[nodiscard]] auto level_is_able_to_be_engulfed_blocked_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_able_to_be_engulfed_blocked, p, me) != nullptr;
}

[[nodiscard]] auto level_is_able_to_be_ensnared_blocked_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_able_to_be_ensnared_blocked, p, me) != nullptr;
}

[[nodiscard]] auto level_is_active_when_worn_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_active_when_worn, p, me) != nullptr;
}

[[nodiscard]] auto level_is_leather_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_leather, p, me) != nullptr;
}

[[nodiscard]] auto level_is_boots_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_boots, p, me) != nullptr;
}

[[nodiscard]] auto level_is_spider_giant_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_spider_giant, p, me) != nullptr;
}

[[nodiscard]] auto level_is_spider_baby_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_spider_baby, p, me) != nullptr;
}

[[nodiscard]] auto level_is_able_to_choose_targets_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_able_to_choose_targets, p, me) != nullptr;
}

[[nodiscard]] auto level_is_flesh_eater_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_flesh_eater, p, me) != nullptr;
}

[[nodiscard]] auto level_is_able_to_move_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_able_to_move, p, me) != nullptr;
}

[[nodiscard]] auto level_is_spider_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_spider, p, me) != nullptr;
}

[[nodiscard]] auto level_is_able_to_be_ensnared_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_able_to_be_ensnared, p, me) != nullptr;
}

[[nodiscard]] auto level_is_able_to_ensnare_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_able_to_ensnare, p, me) != nullptr;
}

[[nodiscard]] auto level_is_spiderweb_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_spiderweb, p, me) != nullptr;
}

[[nodiscard]] auto level_is_rubble_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_rubble, p, me) != nullptr;
}

[[nodiscard]] auto level_is_able_to_jump_land_then_pounce_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_able_to_jump_land_then_pounce, p, me) != nullptr;
}

[[nodiscard]] auto level_is_unused188_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused188, p, me) != nullptr;
}

[[nodiscard]] auto level_is_skeleton_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_skeleton, p, me) != nullptr;
}

[[nodiscard]] auto level_is_water_bound_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_water_bound, p, me) != nullptr;
}

[[nodiscard]] auto level_is_amphibious_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_amphibious, p, me) != nullptr;
}

[[nodiscard]] auto level_is_able_to_resurrect_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_able_to_resurrect, p, me) != nullptr;
}

[[nodiscard]] auto level_is_soft_landing_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_soft_landing, p, me) != nullptr;
}

[[nodiscard]] auto level_is_gelatinous_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_gelatinous, p, me) != nullptr;
}

[[nodiscard]] auto level_is_pot_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_potion, p, me) != nullptr;
}

[[nodiscard]] auto level_is_able_to_be_levitated_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_able_to_be_levitated, p, me) != nullptr;
}

[[nodiscard]] auto level_is_teleport_hazard_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_teleport_hazard, p, me) != nullptr;
}

[[nodiscard]] auto level_is_active_when_carried_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_active_when_carried, p, me) != nullptr;
}

[[nodiscard]] auto level_is_wearable_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_wearable, p, me) != nullptr;
}

[[nodiscard]] auto level_is_ring_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_ring, p, me) != nullptr;
}

[[nodiscard]] auto level_is_obs_to_hearing_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  if (is_oob(p)) {
    return true;
  }
  return level_flag(g, v, l, is_obs_to_hearing, p, me) != nullptr;
}

[[nodiscard]] auto level_is_throwable_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_throwable, p, me) != nullptr;
}

[[nodiscard]] auto level_is_obs_to_throwing_over_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  if (is_oob(p)) {
    return true;
  }
  return level_flag(g, v, l, is_obs_to_throwing_over, p, me) != nullptr;
}

[[nodiscard]] auto level_is_obs_to_throwing_onto_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  if (is_oob(p)) {
    return true;
  }
  return level_flag(g, v, l, is_obs_to_throwing_onto, p, me) != nullptr;
}

[[nodiscard]] auto level_is_physics_trap_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_physics_trap, p, me) != nullptr;
}

[[nodiscard]] auto level_is_tick_on_strip_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_tick_on_strip, p, me) != nullptr;
}

[[nodiscard]] auto level_is_tick_on_worn_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_tick_on_worn, p, me) != nullptr;
}

[[nodiscard]] auto level_is_buff_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_buff, p, me) != nullptr;
}

[[nodiscard]] auto level_is_able_to_be_buffed_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_able_to_be_buffed, p, me) != nullptr;
}

[[nodiscard]] auto level_is_tick_on_drop_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_tick_on_drop, p, me) != nullptr;
}

[[nodiscard]] auto level_is_lava_bg_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_lava_bg, p, me) != nullptr;
}

[[nodiscard]] auto level_is_tick_on_use_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_tick_on_use, p, me) != nullptr;
}

[[nodiscard]] auto level_is_able_to_throw_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_able_to_throw, p, me) != nullptr;
}

[[nodiscard]] auto level_is_described_when_killed_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_described_when_killed, p, me) != nullptr;
}

[[nodiscard]] auto level_is_chest_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_chest, p, me) != nullptr;
}

[[nodiscard]] auto level_is_flammable_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_flammable, p, me) != nullptr;
}

[[nodiscard]] auto level_is_auto_wear_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_auto_wear, p, me) != nullptr;
}

[[nodiscard]] auto level_is_dead_when_discharged_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_dead_when_discharged, p, me) != nullptr;
}

[[nodiscard]] auto level_is_able_to_fire_weapons_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_able_to_fire_weapons, p, me) != nullptr;
}

[[nodiscard]] auto level_is_weapon_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_weapon, p, me) != nullptr;
}

[[nodiscard]] auto level_is_light_flicker_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_light_flicker, p, me) != nullptr;
}

[[nodiscard]] auto level_is_able_to_wear_items_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_able_to_wear_items, p, me) != nullptr;
}

[[nodiscard]] auto level_is_wand_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_wand, p, me) != nullptr;
}

[[nodiscard]] auto level_is_staff_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_staff, p, me) != nullptr;
}

[[nodiscard]] auto level_is_able_to_eat_treasure_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_able_to_eat_treasure, p, me) != nullptr;
}

[[nodiscard]] auto level_is_wait_on_anim_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_wait_on_anim, p, me) != nullptr;
}

[[nodiscard]] auto level_is_flat_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_flat, p, me) != nullptr;
}

[[nodiscard]] auto level_is_obs_to_beam_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  if (is_oob(p)) {
    return true;
  }
  return level_flag(g, v, l, is_obs_to_beam, p, me) != nullptr;
}

[[nodiscard]] auto level_is_beam_weapon_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_beam_weapon, p, me) != nullptr;
}

[[nodiscard]] auto level_is_shown_stamina_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_shown_stamina, p, me) != nullptr;
}

[[nodiscard]] auto level_is_reeds_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_reeds, p, me) != nullptr;
}

[[nodiscard]] auto level_is_able_to_drop_all_items_on_death_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_able_to_drop_all_items_on_death, p, me) != nullptr;
}

[[nodiscard]] auto level_is_vault_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_vault, p, me) != nullptr;
}

[[nodiscard]] auto level_is_biome_underhell_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_biome_underhell, p, me) != nullptr;
}

[[nodiscard]] auto level_is_biome_graveyard_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_biome_graveyard, p, me) != nullptr;
}

[[nodiscard]] auto level_is_biome_nethervoid_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_biome_nethervoid, p, me) != nullptr;
}

[[nodiscard]] auto level_is_biome_bogland_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_biome_bogland, p, me) != nullptr;
}

[[nodiscard]] auto level_is_biome_dungeon_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_biome_dungeon, p, me) != nullptr;
}

[[nodiscard]] auto level_is_level_open_icon_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_level_open_icon, p, me) != nullptr;
}

[[nodiscard]] auto level_is_blit_bg_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_blit_bg, p, me) != nullptr;
}

[[nodiscard]] auto level_is_level_select_bg_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_level_select_bg, p, me) != nullptr;
}

[[nodiscard]] auto level_is_blit_hit_outline_w_black_inside_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_blit_hit_outline_w_black_inside, p, me) != nullptr;
}

[[nodiscard]] auto level_is_able_to_engulf_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_able_to_engulf, p, me) != nullptr;
}

[[nodiscard]] auto level_is_able_to_fall_sound_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_able_to_fall_sound, p, me) != nullptr;
}

[[nodiscard]] auto level_is_collision_hit_first_on_tile_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_collision_hit_first_on_tile, p, me) != nullptr;
}

[[nodiscard]] auto level_is_collision_hit_all_on_tile_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_collision_hit_all_on_tile, p, me) != nullptr;
}

[[nodiscard]] auto level_is_blit_hit_outline_w_invis_inside_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_blit_hit_outline_w_invis_inside, p, me) != nullptr;
}

[[nodiscard]] auto level_is_blit_when_obscured_outline_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_blit_when_obscured_outline, p, me) != nullptr;
}

[[nodiscard]] auto level_is_attackable_by_player_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_attackable_by_player, p, me) != nullptr;
}

[[nodiscard]] auto level_is_attackable_by_monst_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_attackable_by_monst, p, me) != nullptr;
}

[[nodiscard]] auto level_is_effect_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_effect, p, me) != nullptr;
}

[[nodiscard]] auto level_is_effect_attack_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_effect_attack, p, me) != nullptr;
}

[[nodiscard]] auto level_is_effect_blood_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_effect_blood, p, me) != nullptr;
}

[[nodiscard]] auto level_is_effect_ripple_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_effect_ripple, p, me) != nullptr;
}

[[nodiscard]] auto level_is_hit_when_dead_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_hit_when_dead, p, me) != nullptr;
}

[[nodiscard]] auto level_is_able_to_be_engulfed_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_able_to_be_engulfed, p, me) != nullptr;
}

[[nodiscard]] auto level_is_grouped_thing_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_grouped_thing, p, me) != nullptr;
}

[[nodiscard]] auto level_is_vision_360_degrees_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_vision_360_degrees, p, me) != nullptr;
}

[[nodiscard]] auto level_is_vision_180_degrees_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_vision_180_degrees, p, me) != nullptr;
}

[[nodiscard]] auto level_is_able_to_lunge_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_able_to_lunge, p, me) != nullptr;
}

[[nodiscard]] auto level_is_obs_to_paths_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  if (is_oob(p)) {
    return true;
  }
  return level_flag(g, v, l, is_obs_to_paths, p, me) != nullptr;
}

[[nodiscard]] auto level_is_stealthy_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_stealthy, p, me) != nullptr;
}

[[nodiscard]] auto level_is_tireless_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_tireless, p, me) != nullptr;
}

[[nodiscard]] auto level_is_shown_noise_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_shown_noise, p, me) != nullptr;
}

[[nodiscard]] auto level_is_wait_on_dead_anim_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_wait_on_dead_anim, p, me) != nullptr;
}

[[nodiscard]] auto level_is_wall_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_wall, p, me) != nullptr;
}

[[nodiscard]] auto level_is_water_shallow_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_water_shallow, p, me) != nullptr;
}

[[nodiscard]] auto level_is_wood_bool(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> bool
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_wood, p, me) != nullptr;
}
