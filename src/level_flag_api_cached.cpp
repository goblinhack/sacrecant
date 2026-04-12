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

auto level_is_able_to_collect_items_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_able_to_collect_items, p);
}

auto level_is_able_to_collect_keys_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_able_to_collect_keys, p);
}

auto level_is_able_to_crush_grass_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_able_to_crush_grass, p);
}

auto level_is_able_to_fall_repeatedly_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_able_to_fall_repeatedly, p);
}

auto level_is_able_to_fall_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_able_to_fall, p);
}

auto level_is_able_to_jump_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_able_to_jump, p);
}

auto level_is_able_to_move_diagonally_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_able_to_move_diagonally, p);
}

auto level_is_able_to_move_through_walls_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_able_to_move_through_walls, p);
}

auto level_is_able_to_open_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_able_to_open, p);
}

auto level_is_able_to_shove_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_able_to_shove, p);
}

auto level_is_able_to_walk_through_walls_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_able_to_walk_through_walls, p);
}

auto level_is_animated_can_hflip_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_animated_can_hflip, p);
}

auto level_is_animated_no_dir_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_animated_no_dir, p);
}

auto level_is_animated_sync_first_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_animated_sync_first, p);
}

auto level_is_animated_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_animated, p);
}

auto level_is_barrel_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_barrel, p);
}

auto level_is_blit_centered_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_blit_centered, p);
}

auto level_is_blit_flush_per_line_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_blit_flush_per_line, p);
}

auto level_is_blit_if_has_seen_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_blit_if_has_seen, p);
}

auto level_is_blit_obscures_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_blit_obscures, p);
}

auto level_is_blit_on_ground_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_blit_on_ground, p);
}

auto level_is_blit_outlined_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_blit_outlined, p);
}

auto level_is_blit_per_pixel_lighting_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_blit_per_pixel_lighting, p);
}

auto level_is_blit_shown_in_chasms_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_blit_shown_in_chasms, p);
}

auto level_is_blit_shown_in_overlay_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_blit_shown_in_overlay, p);
}

auto level_is_blit_square_outlined_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_blit_square_outlined, p);
}

auto level_is_blit_when_obscured_faded_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_blit_when_obscured_faded, p);
}

auto level_is_border_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_border, p);
}

auto level_is_brazier_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_brazier, p);
}

auto level_is_bridge_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_bridge, p);
}

auto level_is_broken_on_death_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_broken_on_death, p);
}

auto level_is_burnable_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_burnable, p);
}

auto level_is_chasm_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_chasm, p);
}

auto level_is_collectable_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_collectable, p);
}

auto level_is_collision_circle_large_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_collision_circle_large, p);
}

auto level_is_collision_circle_small_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_collision_circle_small, p);
}

auto level_is_collision_detection_enabled_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_collision_detection_enabled, p);
}

auto level_is_collision_square_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_collision_square, p);
}

auto level_is_combustible_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_combustible, p);
}

auto level_is_corpse_on_death_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_corpse_on_death, p);
}

auto level_is_corridor_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_corridor, p);
}

auto level_is_crushable_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_crushable, p);
}

auto level_is_cursor_path_hazard_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_cursor_path_hazard, p);
}

auto level_is_cursor_path_none_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_cursor_path_none, p);
}

auto level_is_cursor_path_warning_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_cursor_path_warning, p);
}

auto level_is_cursor_path_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_cursor_path, p);
}

auto level_is_cursor_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_cursor, p);
}

auto level_is_damage_capped_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_damage_capped, p);
}

auto level_is_dead_on_collision_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_dead_on_collision, p);
}

auto level_is_dead_on_shoving_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_dead_on_shoving, p);
}

auto level_is_deep_water_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_deep_water, p);
}

auto level_is_described_cursor_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_described_cursor, p);
}

auto level_is_dirt_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_dirt, p);
}

auto level_is_dmap_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_dmap, p);
}

auto level_is_door_locked_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_door_locked, p);
}

auto level_is_door_secret_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_door_secret, p);
}

auto level_is_door_unlocked_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_door_unlocked, p);
}

auto level_is_dungeon_entrance_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_dungeon_entrance, p);
}

auto level_is_entrance_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_entrance, p);
}

auto level_is_ethereal_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_ethereal, p);
}

auto level_is_exit_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_exit, p);
}

auto level_is_explosion_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_explosion, p);
}

auto level_is_extinguished_on_death_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_extinguished_on_death, p);
}

auto level_is_fire_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_fire, p);
}

auto level_is_fireball_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_fireball, p);
}

auto level_is_flesh_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_flesh, p);
}

auto level_is_floating_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_floating, p);
}

auto level_is_floor_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_floor, p);
}

auto level_is_flying_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_flying, p);
}

auto level_is_foliage_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_foliage, p);
}

auto level_is_gaseous_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_gaseous, p);
}

auto level_is_ghost_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_ghost, p);
}

auto level_is_glass_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_glass, p);
}

auto level_is_gold_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_gold, p);
}

auto level_is_grass_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_grass, p);
}

auto level_is_health_bar_shown_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_health_bar_shown, p);
}

auto level_is_indestructible_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_indestructible, p);
}

auto level_is_insectoid_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_insectoid, p);
}

auto level_is_inventory_item_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_inventory_item, p);
}

auto level_is_item_droppable_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_item_droppable, p);
}

auto level_is_item_equipable_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_item_equipable, p);
}

auto level_is_item_mergeable_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_item_mergeable, p);
}

auto level_is_item_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_item, p);
}

auto level_is_key_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_key, p);
}

auto level_is_kobalos_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_kobalos, p);
}

auto level_is_lava_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_lava, p);
}

auto level_is_unused46_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused46, p);
}

auto level_is_level_curr_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_level_curr, p);
}

auto level_is_unused47_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused47, p);
}

auto level_is_level_final_icon_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_level_final_icon, p);
}

auto level_is_level_next_icon_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_level_next_icon, p);
}

auto level_is_level_locked_icon_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_level_locked_icon, p);
}

auto level_is_level_closed_icon_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_level_closed_icon, p);
}

auto level_is_levitating_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_levitating, p);
}

auto level_is_light_source_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_light_source, p);
}

auto level_is_loggable_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_loggable, p);
}

auto level_is_mantisman_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_mantisman, p);
}

auto level_is_meltable_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_meltable, p);
}

auto level_is_metal_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_metal, p);
}

auto level_is_minion_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_minion, p);
}

auto level_is_mob_kill_minions_on_death_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_mob_kill_minions_on_death, p);
}

auto level_is_mob_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_mob, p);
}

auto level_is_mob1_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_mob1, p);
}

auto level_is_mob2_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_mob2, p);
}

auto level_is_monst_group_easy_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_monst_group_easy, p);
}

auto level_is_monst_group_hard_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_monst_group_hard, p);
}

auto level_is_monst_group_mob_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_monst_group_mob, p);
}

auto level_is_monst_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_monst, p);
}

auto level_is_needs_move_confirm_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_needs_move_confirm, p);
}

auto level_is_obs_to_cursor_path_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_obs_to_cursor_path, p);
}

auto level_is_obs_to_explosion_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_obs_to_explosion, p);
}

auto level_is_obs_to_falling_onto_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_obs_to_falling_onto, p);
}

auto level_is_obs_to_fire_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_obs_to_fire, p);
}

auto level_is_obs_to_jump_over_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_obs_to_jump_over, p);
}

auto level_is_obs_to_jumping_onto_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_obs_to_jumping_onto, p);
}

auto level_is_obs_to_jumping_out_of_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_obs_to_jumping_out_of, p);
}

auto level_is_obs_to_movement_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_obs_to_movement, p);
}

auto level_is_obs_to_spawning_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_obs_to_spawning, p);
}

auto level_is_obs_to_teleporting_onto_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_obs_to_teleporting_onto, p);
}

auto level_is_obs_to_vision_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_obs_to_vision, p);
}

auto level_is_obs_when_dead_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_obs_when_dead, p);
}

auto level_is_openable_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_openable, p);
}

auto level_is_physics_explosion_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_physics_explosion, p);
}

auto level_is_physics_temperature_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_physics_temperature, p);
}

auto level_is_physics_water_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_physics_water, p);
}

auto level_is_pillar_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_pillar, p);
}

auto level_is_plant_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_plant, p);
}

auto level_is_player_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_player, p);
}

auto level_is_projectile_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_projectile, p);
}

auto level_is_removable_on_err_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_removable_on_err, p);
}

auto level_is_removable_when_dead_on_err_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_removable_when_dead_on_err, p);
}

auto level_is_rock_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_rock, p);
}

auto level_is_shovable_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_shovable, p);
}

auto level_is_slime_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_slime, p);
}

auto level_is_smoke_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_smoke, p);
}

auto level_is_steam_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_steam, p);
}

auto level_is_stone_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_stone, p);
}

auto level_is_submergible_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_submergible, p);
}

auto level_is_teleport_blocked_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_teleport_blocked, p);
}

auto level_is_teleport_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_teleport, p);
}

auto level_is_tick_end_delay_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_tick_end_delay, p);
}

auto level_is_tickable_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_tickable, p);
}

auto level_is_tiled_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_tiled, p);
}

auto level_is_trap_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_trap, p);
}

auto level_is_treasure_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_treasure, p);
}

auto level_is_undead_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_undead, p);
}

auto level_is_unused1_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused1, p);
}

auto level_is_unused10_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused10, p);
}

auto level_is_unused11_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused11, p);
}

auto level_is_unused12_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused12, p);
}

auto level_is_unused13_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused13, p);
}

auto level_is_unused14_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused14, p);
}

auto level_is_unused15_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused15, p);
}

auto level_is_unused16_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused16, p);
}

auto level_is_unused17_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused17, p);
}

auto level_is_unused18_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused18, p);
}

auto level_is_unused19_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused19, p);
}

auto level_is_unused2_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused2, p);
}

auto level_is_unused20_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused20, p);
}

auto level_is_unused21_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused21, p);
}

auto level_is_unused22_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused22, p);
}

auto level_is_unused23_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused23, p);
}

auto level_is_unused24_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused24, p);
}

auto level_is_unused25_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused25, p);
}

auto level_is_unused26_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused26, p);
}

auto level_is_unused27_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused27, p);
}

auto level_is_unused28_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused28, p);
}

auto level_is_unused29_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused29, p);
}

auto level_is_unused3_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused3, p);
}

auto level_is_unused30_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused30, p);
}

auto level_is_unused31_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused31, p);
}

auto level_is_unused32_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused32, p);
}

auto level_is_unused33_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused33, p);
}

auto level_is_unused34_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused34, p);
}

auto level_is_unused35_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused35, p);
}

auto level_is_unused36_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused36, p);
}

auto level_is_unused37_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused37, p);
}

auto level_is_unused38_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused38, p);
}

auto level_is_reeds_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_reeds, p);
}

auto level_is_unused4_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused4, p);
}

auto level_is_vault_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_vault, p);
}

auto level_is_biome_underhell_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_biome_underhell, p);
}

auto level_is_biome_graveyard_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_biome_graveyard, p);
}

auto level_is_biome_nethervoid_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_biome_nethervoid, p);
}

auto level_is_biome_bogland_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_biome_bogland, p);
}

auto level_is_biome_dungeon_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_biome_dungeon, p);
}

auto level_is_level_open_icon_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_level_open_icon, p);
}

auto level_is_blit_bg_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_blit_bg, p);
}

auto level_is_level_select_bg_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_level_select_bg, p);
}

auto level_is_blit_hit_outline_w_black_inside_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_blit_hit_outline_w_black_inside, p);
}

auto level_is_unused5_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused5, p);
}

auto level_is_able_to_fall_sound_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_able_to_fall_sound, p);
}

auto level_is_collision_hit_first_on_tile_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_collision_hit_first_on_tile, p);
}

auto level_is_collision_hit_all_on_tile_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_collision_hit_all_on_tile, p);
}

auto level_is_blit_hit_outline_w_invis_inside_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_blit_hit_outline_w_invis_inside, p);
}

auto level_is_blit_when_obscured_outline_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_blit_when_obscured_outline, p);
}

auto level_is_attackable_by_player_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_attackable_by_player, p);
}

auto level_is_attackable_by_monst_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_attackable_by_monst, p);
}

auto level_is_effect_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_effect, p);
}

auto level_is_effect_attack_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_effect_attack, p);
}

auto level_is_effect_blood_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_effect_blood, p);
}

auto level_is_unused6_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused6, p);
}

auto level_is_hit_when_dead_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_hit_when_dead, p);
}

auto level_is_able_to_fire_projectiles_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_able_to_fire_projectiles, p);
}

auto level_is_blitzhound_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_blitzhound, p);
}

auto level_is_unused63_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused63, p);
}

auto level_is_grouped_thing_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_grouped_thing, p);
}

auto level_is_vision_360_degrees_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_vision_360_degrees, p);
}

auto level_is_vision_180_degrees_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_vision_180_degrees, p);
}

auto level_is_able_to_lunge_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_able_to_lunge, p);
}

auto level_is_obs_to_paths_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_obs_to_paths, p);
}

auto level_is_unused7_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused7, p);
}

auto level_is_unused8_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused8, p);
}

auto level_is_unused9_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_unused9, p);
}

auto level_is_wait_on_dead_anim_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_wait_on_dead_anim, p);
}

auto level_is_wall_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_wall, p);
}

auto level_is_water_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_water, p);
}

auto level_is_wood_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool
{
  TRACE_DEBUG();
  return level_flag_cached(g, v, l, is_wood, p);
}
