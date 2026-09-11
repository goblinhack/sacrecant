//
// Copyright goblinhack@gmail.com
//

#include "my_bpoint.hpp"
#include "my_callstack.hpp"
#include "my_level.hpp"
#include "my_tp.hpp"
#include "my_types.hpp"

[[nodiscard]] auto level_open_is_able_to_collect_items(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_able_to_collect_items, p);
}

[[nodiscard]] auto level_open_is_able_to_collect_keys(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_able_to_collect_keys, p);
}

[[nodiscard]] auto level_open_is_able_to_crush_grass(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_able_to_crush_grass, p);
}

[[nodiscard]] auto level_open_is_able_to_fall_repeatedly(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_able_to_fall_repeatedly, p);
}

[[nodiscard]] auto level_open_is_able_to_fall(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_able_to_fall, p);
}

[[nodiscard]] auto level_open_is_able_to_jump(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_able_to_jump, p);
}

[[nodiscard]] auto level_open_is_able_to_move_diagonally(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_able_to_move_diagonally, p);
}

[[nodiscard]] auto level_open_is_able_to_move_through_walls(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_able_to_move_through_walls, p);
}

[[nodiscard]] auto level_open_is_able_to_open_things(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_able_to_open_things, p);
}

[[nodiscard]] auto level_open_is_able_to_shove(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_able_to_shove, p);
}

[[nodiscard]] auto level_open_is_able_to_see_through_walls(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_able_to_see_through_walls, p);
}

[[nodiscard]] auto level_open_is_animated_can_hflip(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_animated_can_hflip, p);
}

[[nodiscard]] auto level_open_is_animated_no_dir(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_animated_no_dir, p);
}

[[nodiscard]] auto level_open_is_animated_sync_first(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_animated_sync_first, p);
}

[[nodiscard]] auto level_open_is_animated(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_animated, p);
}

[[nodiscard]] auto level_open_is_barrel(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_barrel, p);
}

[[nodiscard]] auto level_open_is_blit_centered(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_blit_centered, p);
}

[[nodiscard]] auto level_open_is_blit_flush_per_line(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_blit_flush_per_line, p);
}

[[nodiscard]] auto level_open_is_blit_if_has_seen(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_blit_if_has_seen, p);
}

[[nodiscard]] auto level_open_is_blit_obscures(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_blit_obscures, p);
}

[[nodiscard]] auto level_open_is_blit_on_ground(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_blit_on_ground, p);
}

[[nodiscard]] auto level_open_is_blit_outlined(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_blit_outlined, p);
}

[[nodiscard]] auto level_open_is_blit_per_pixel_lighting(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_blit_per_pixel_lighting, p);
}

[[nodiscard]] auto level_open_is_blit_shown_in_chasms(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_blit_shown_in_chasms, p);
}

[[nodiscard]] auto level_open_is_blit_shown_in_overlay(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_blit_shown_in_overlay, p);
}

[[nodiscard]] auto level_open_is_blit_square_outlined(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_blit_square_outlined, p);
}

[[nodiscard]] auto level_open_is_blit_when_obscured_faded(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_blit_when_obscured_faded, p);
}

[[nodiscard]] auto level_open_is_border(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_border, p);
}

[[nodiscard]] auto level_open_is_brazier(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_brazier, p);
}

[[nodiscard]] auto level_open_is_bridge(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_bridge, p);
}

[[nodiscard]] auto level_open_is_broken_on_death(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_broken_on_death, p);
}

[[nodiscard]] auto level_open_is_burnable(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_burnable, p);
}

[[nodiscard]] auto level_open_is_chasm(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_chasm, p);
}

[[nodiscard]] auto level_open_is_collectable(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_collectable, p);
}

[[nodiscard]] auto level_open_is_collision_circle_large(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_collision_circle_large, p);
}

[[nodiscard]] auto level_open_is_collision_circle_small(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_collision_circle_small, p);
}

[[nodiscard]] auto level_open_is_collision_detection_enabled(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_collision_detection_enabled, p);
}

[[nodiscard]] auto level_open_is_collision_square(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_collision_square, p);
}

[[nodiscard]] auto level_open_is_combustible(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_combustible, p);
}

[[nodiscard]] auto level_open_is_corpse_on_death(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_corpse_on_death, p);
}

[[nodiscard]] auto level_open_is_corridor(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_corridor, p);
}

[[nodiscard]] auto level_open_is_crushable(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_crushable, p);
}

[[nodiscard]] auto level_open_is_cursor_path_hazard(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_cursor_path_hazard, p);
}

[[nodiscard]] auto level_open_is_cursor_path_none(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_cursor_path_none, p);
}

[[nodiscard]] auto level_open_is_cursor_path_warning(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_cursor_path_warning, p);
}

[[nodiscard]] auto level_open_is_cursor_path(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_cursor_path, p);
}

[[nodiscard]] auto level_open_is_cursor(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_cursor, p);
}

[[nodiscard]] auto level_open_is_damage_capped(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_damage_capped, p);
}

[[nodiscard]] auto level_open_is_dead_on_collision(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_dead_on_collision, p);
}

[[nodiscard]] auto level_open_is_dead_on_shoving(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_dead_on_shoving, p);
}

[[nodiscard]] auto level_open_is_water_deep(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_water_deep, p);
}

[[nodiscard]] auto level_open_is_described_cursor(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_described_cursor, p);
}

[[nodiscard]] auto level_open_is_dirt(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_dirt, p);
}

[[nodiscard]] auto level_open_is_dmap(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_dmap, p);
}

[[nodiscard]] auto level_open_is_door_locked(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_door_locked, p);
}

[[nodiscard]] auto level_open_is_door_secret(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_door_secret, p);
}

[[nodiscard]] auto level_open_is_door_unlocked(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_door_unlocked, p);
}

[[nodiscard]] auto level_open_is_dungeon_entrance(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_dungeon_entrance, p);
}

[[nodiscard]] auto level_open_is_entrance(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_entrance, p);
}

[[nodiscard]] auto level_open_is_ethereal(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_ethereal, p);
}

[[nodiscard]] auto level_open_is_exit(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_exit, p);
}

[[nodiscard]] auto level_open_is_explosion(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_explosion, p);
}

[[nodiscard]] auto level_open_is_extinguished_on_death(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_extinguished_on_death, p);
}

[[nodiscard]] auto level_open_is_fire(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_fire, p);
}

[[nodiscard]] auto level_open_is_lucky(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_lucky, p);
}

[[nodiscard]] auto level_open_is_flesh(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_flesh, p);
}

[[nodiscard]] auto level_open_is_levitating(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_levitating, p);
}

[[nodiscard]] auto level_open_is_floor(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_floor, p);
}

[[nodiscard]] auto level_open_is_obs_to_ethereal(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_obs_to_ethereal, p);
}

[[nodiscard]] auto level_open_is_foliage(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_foliage, p);
}

[[nodiscard]] auto level_open_is_gaseous(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_gaseous, p);
}

[[nodiscard]] auto level_open_is_ghost(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_ghost, p);
}

[[nodiscard]] auto level_open_is_glass(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_glass, p);
}

[[nodiscard]] auto level_open_is_gold(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_gold, p);
}

[[nodiscard]] auto level_open_is_grass(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_grass, p);
}

[[nodiscard]] auto level_open_is_shown_health(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_shown_health, p);
}

[[nodiscard]] auto level_open_is_indestructible(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_indestructible, p);
}

[[nodiscard]] auto level_open_is_insectoid(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_insectoid, p);
}

[[nodiscard]] auto level_open_is_inventory_item(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_inventory_item, p);
}

[[nodiscard]] auto level_open_is_droppable(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_droppable, p);
}

[[nodiscard]] auto level_open_is_unused183(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused183, p);
}

[[nodiscard]] auto level_open_is_item_mergeable(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_item_mergeable, p);
}

[[nodiscard]] auto level_open_is_item(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_item, p);
}

[[nodiscard]] auto level_open_is_key(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_key, p);
}

[[nodiscard]] auto level_open_is_kobalos(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_kobalos, p);
}

[[nodiscard]] auto level_open_is_lava(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_lava, p);
}

[[nodiscard]] auto level_open_is_usable(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_usable, p);
}

[[nodiscard]] auto level_open_is_level_curr(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_level_curr, p);
}

[[nodiscard]] auto level_open_is_critical_to_dungeon_design(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_critical_to_dungeon_design, p);
}

[[nodiscard]] auto level_open_is_level_final_icon(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_level_final_icon, p);
}

[[nodiscard]] auto level_open_is_level_next_icon(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_level_next_icon, p);
}

[[nodiscard]] auto level_open_is_level_locked_icon(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_level_locked_icon, p);
}

[[nodiscard]] auto level_open_is_level_closed_icon(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_level_closed_icon, p);
}

[[nodiscard]] auto level_open_is_light_source(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_light_source, p);
}

[[nodiscard]] auto level_open_is_loggable(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_loggable, p);
}

[[nodiscard]] auto level_open_is_able_to_eat_items(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_able_to_eat_items, p);
}

[[nodiscard]] auto level_open_is_meltable(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_meltable, p);
}

[[nodiscard]] auto level_open_is_metal(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_metal, p);
}

[[nodiscard]] auto level_open_is_minion(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_minion, p);
}

[[nodiscard]] auto level_open_is_mob_kill_minions_on_death(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_mob_kill_minions_on_death, p);
}

[[nodiscard]] auto level_open_is_mob(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_mob, p);
}

[[nodiscard]] auto level_open_is_mob1(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_mob1, p);
}

[[nodiscard]] auto level_open_is_mob2(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_mob2, p);
}

[[nodiscard]] auto level_open_is_monst1(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_monst1, p);
}

[[nodiscard]] auto level_open_is_monst2(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_monst2, p);
}

[[nodiscard]] auto level_open_is_monst(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_monst, p);
}

[[nodiscard]] auto level_open_is_needs_move_confirm(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_needs_move_confirm, p);
}

[[nodiscard]] auto level_open_is_obs_to_cursor_path(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_obs_to_cursor_path, p);
}

[[nodiscard]] auto level_open_is_obs_to_explosion(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_obs_to_explosion, p);
}

[[nodiscard]] auto level_open_is_obs_to_falling_onto(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_obs_to_falling_onto, p);
}

[[nodiscard]] auto level_open_is_obs_to_fire(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_obs_to_fire, p);
}

[[nodiscard]] auto level_open_is_obs_to_jumping_over(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_obs_to_jumping_over, p);
}

[[nodiscard]] auto level_open_is_obs_to_jumping_onto(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_obs_to_jumping_onto, p);
}

[[nodiscard]] auto level_open_is_obs_to_jumping_out_of(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_obs_to_jumping_out_of, p);
}

[[nodiscard]] auto level_open_is_obs_to_movement(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_obs_to_movement, p);
}

[[nodiscard]] auto level_open_is_obs_to_spawning(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_obs_to_spawning, p);
}

[[nodiscard]] auto level_open_is_obs_to_teleporting_onto(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_obs_to_teleporting_onto, p);
}

[[nodiscard]] auto level_open_is_obs_to_vision(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_obs_to_vision, p);
}

[[nodiscard]] auto level_open_is_obs_when_dead(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_obs_when_dead, p);
}

[[nodiscard]] auto level_open_is_openable(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_openable, p);
}

[[nodiscard]] auto level_open_is_physics_explosion(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_physics_explosion, p);
}

[[nodiscard]] auto level_open_is_physics_temperature(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_physics_temperature, p);
}

[[nodiscard]] auto level_open_is_physics_water(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_physics_water, p);
}

[[nodiscard]] auto level_open_is_pillar(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_pillar, p);
}

[[nodiscard]] auto level_open_is_plant(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_plant, p);
}

[[nodiscard]] auto level_open_is_player(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_player, p);
}

[[nodiscard]] auto level_open_is_projectile(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_projectile, p);
}

[[nodiscard]] auto level_open_is_removable_on_err(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_removable_on_err, p);
}

[[nodiscard]] auto level_open_is_removable_when_dead_on_err(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_removable_when_dead_on_err, p);
}

[[nodiscard]] auto level_open_is_rock(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_rock, p);
}

[[nodiscard]] auto level_open_is_shovable(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_shovable, p);
}

[[nodiscard]] auto level_open_is_slime(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_slime, p);
}

[[nodiscard]] auto level_open_is_smoke(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_smoke, p);
}

[[nodiscard]] auto level_open_is_steam(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_steam, p);
}

[[nodiscard]] auto level_open_is_stone(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_stone, p);
}

[[nodiscard]] auto level_open_is_submergible(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_submergible, p);
}

[[nodiscard]] auto level_open_is_able_to_be_teleported(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_able_to_be_teleported, p);
}

[[nodiscard]] auto level_open_is_teleport(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_teleport, p);
}

[[nodiscard]] auto level_open_is_tick_end_delay(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_tick_end_delay, p);
}

[[nodiscard]] auto level_open_is_tickable(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_tickable, p);
}

[[nodiscard]] auto level_open_is_tiled(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_tiled, p);
}

[[nodiscard]] auto level_open_is_trap(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_trap, p);
}

[[nodiscard]] auto level_open_is_treasure(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_treasure, p);
}

[[nodiscard]] auto level_open_is_undead(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_undead, p);
}

[[nodiscard]] auto level_open_is_unused1(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused1, p);
}

[[nodiscard]] auto level_open_is_unused2(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused2, p);
}

[[nodiscard]] auto level_open_is_unused3(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused3, p);
}

[[nodiscard]] auto level_open_is_unused4(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused4, p);
}

[[nodiscard]] auto level_open_is_unused5(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused5, p);
}

[[nodiscard]] auto level_open_is_unused6(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused6, p);
}

[[nodiscard]] auto level_open_is_unused7(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused7, p);
}

[[nodiscard]] auto level_open_is_unused8(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused8, p);
}

[[nodiscard]] auto level_open_is_unused9(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused9, p);
}

[[nodiscard]] auto level_open_is_unused10(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused10, p);
}

[[nodiscard]] auto level_open_is_unused11(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused11, p);
}

[[nodiscard]] auto level_open_is_unused12(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused12, p);
}

[[nodiscard]] auto level_open_is_unused13(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused13, p);
}

[[nodiscard]] auto level_open_is_unused14(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused14, p);
}

[[nodiscard]] auto level_open_is_unused15(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused15, p);
}

[[nodiscard]] auto level_open_is_unused16(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused16, p);
}

[[nodiscard]] auto level_open_is_unused17(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused17, p);
}

[[nodiscard]] auto level_open_is_unused18(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused18, p);
}

[[nodiscard]] auto level_open_is_unused19(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused19, p);
}

[[nodiscard]] auto level_open_is_unused20(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused20, p);
}

[[nodiscard]] auto level_open_is_unused21(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused21, p);
}

[[nodiscard]] auto level_open_is_unused22(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused22, p);
}

[[nodiscard]] auto level_open_is_unused23(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused23, p);
}

[[nodiscard]] auto level_open_is_unused24(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused24, p);
}

[[nodiscard]] auto level_open_is_unused25(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused25, p);
}

[[nodiscard]] auto level_open_is_unused26(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused26, p);
}

[[nodiscard]] auto level_open_is_unused27(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused27, p);
}

[[nodiscard]] auto level_open_is_unused28(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused28, p);
}

[[nodiscard]] auto level_open_is_unused29(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused29, p);
}

[[nodiscard]] auto level_open_is_unused30(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused30, p);
}

[[nodiscard]] auto level_open_is_unused31(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused31, p);
}

[[nodiscard]] auto level_open_is_unused32(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused32, p);
}

[[nodiscard]] auto level_open_is_unused33(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused33, p);
}

[[nodiscard]] auto level_open_is_unused34(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused34, p);
}

[[nodiscard]] auto level_open_is_unused35(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused35, p);
}

[[nodiscard]] auto level_open_is_unused36(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused36, p);
}

[[nodiscard]] auto level_open_is_unused37(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused37, p);
}

[[nodiscard]] auto level_open_is_unused38(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused38, p);
}

[[nodiscard]] auto level_open_is_unused39(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused39, p);
}

[[nodiscard]] auto level_open_is_unused40(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused40, p);
}

[[nodiscard]] auto level_open_is_unused41(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused41, p);
}

[[nodiscard]] auto level_open_is_unused42(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused42, p);
}

[[nodiscard]] auto level_open_is_unused43(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused43, p);
}

[[nodiscard]] auto level_open_is_unused44(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused44, p);
}

[[nodiscard]] auto level_open_is_unused45(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused45, p);
}

[[nodiscard]] auto level_open_is_unused46(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused46, p);
}

[[nodiscard]] auto level_open_is_unused47(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused47, p);
}

[[nodiscard]] auto level_open_is_unused48(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused48, p);
}

[[nodiscard]] auto level_open_is_unused49(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused49, p);
}

[[nodiscard]] auto level_open_is_unused50(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused50, p);
}

[[nodiscard]] auto level_open_is_unused51(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused51, p);
}

[[nodiscard]] auto level_open_is_unused52(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused52, p);
}

[[nodiscard]] auto level_open_is_unused53(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused53, p);
}

[[nodiscard]] auto level_open_is_unused54(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused54, p);
}

[[nodiscard]] auto level_open_is_unused55(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused55, p);
}

[[nodiscard]] auto level_open_is_unused56(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused56, p);
}

[[nodiscard]] auto level_open_is_unused57(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused57, p);
}

[[nodiscard]] auto level_open_is_unused58(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused58, p);
}

[[nodiscard]] auto level_open_is_unused59(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused59, p);
}

[[nodiscard]] auto level_open_is_unused60(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused60, p);
}

[[nodiscard]] auto level_open_is_unused61(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused61, p);
}

[[nodiscard]] auto level_open_is_unused62(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused62, p);
}

[[nodiscard]] auto level_open_is_unused63(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused63, p);
}

[[nodiscard]] auto level_open_is_unused64(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused64, p);
}

[[nodiscard]] auto level_open_is_unused65(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused65, p);
}

[[nodiscard]] auto level_open_is_unused66(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused66, p);
}

[[nodiscard]] auto level_open_is_unused67(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused67, p);
}

[[nodiscard]] auto level_open_is_unused68(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused68, p);
}

[[nodiscard]] auto level_open_is_unused69(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused69, p);
}

[[nodiscard]] auto level_open_is_unused70(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused70, p);
}

[[nodiscard]] auto level_open_is_unused71(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused71, p);
}

[[nodiscard]] auto level_open_is_unused72(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused72, p);
}

[[nodiscard]] auto level_open_is_unused73(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused73, p);
}

[[nodiscard]] auto level_open_is_unused74(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused74, p);
}

[[nodiscard]] auto level_open_is_unused75(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused75, p);
}

[[nodiscard]] auto level_open_is_unused76(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused76, p);
}

[[nodiscard]] auto level_open_is_unused77(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused77, p);
}

[[nodiscard]] auto level_open_is_unused78(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused78, p);
}

[[nodiscard]] auto level_open_is_unused79(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused79, p);
}

[[nodiscard]] auto level_open_is_unused80(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused80, p);
}

[[nodiscard]] auto level_open_is_unused81(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused81, p);
}

[[nodiscard]] auto level_open_is_unused82(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused82, p);
}

[[nodiscard]] auto level_open_is_unused83(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused83, p);
}

[[nodiscard]] auto level_open_is_unused84(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused84, p);
}

[[nodiscard]] auto level_open_is_unused85(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused85, p);
}

[[nodiscard]] auto level_open_is_unused86(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused86, p);
}

[[nodiscard]] auto level_open_is_unused87(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused87, p);
}

[[nodiscard]] auto level_open_is_unused88(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused88, p);
}

[[nodiscard]] auto level_open_is_unused89(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused89, p);
}

[[nodiscard]] auto level_open_is_unused90(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused90, p);
}

[[nodiscard]] auto level_open_is_unused91(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused91, p);
}

[[nodiscard]] auto level_open_is_unused92(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused92, p);
}

[[nodiscard]] auto level_open_is_unused93(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused93, p);
}

[[nodiscard]] auto level_open_is_unused94(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused94, p);
}

[[nodiscard]] auto level_open_is_unused95(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused95, p);
}

[[nodiscard]] auto level_open_is_unused96(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused96, p);
}

[[nodiscard]] auto level_open_is_unused97(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused97, p);
}

[[nodiscard]] auto level_open_is_unused98(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused98, p);
}

[[nodiscard]] auto level_open_is_unused99(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused99, p);
}

[[nodiscard]] auto level_open_is_unused100(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused100, p);
}

[[nodiscard]] auto level_open_is_unused101(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused101, p);
}

[[nodiscard]] auto level_open_is_unused102(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused102, p);
}

[[nodiscard]] auto level_open_is_unused103(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused103, p);
}

[[nodiscard]] auto level_open_is_unused104(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused104, p);
}

[[nodiscard]] auto level_open_is_unused105(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused105, p);
}

[[nodiscard]] auto level_open_is_unused106(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused106, p);
}

[[nodiscard]] auto level_open_is_unused107(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused107, p);
}

[[nodiscard]] auto level_open_is_unused108(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused108, p);
}

[[nodiscard]] auto level_open_is_unused109(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused109, p);
}

[[nodiscard]] auto level_open_is_unused110(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused110, p);
}

[[nodiscard]] auto level_open_is_unused111(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused111, p);
}

[[nodiscard]] auto level_open_is_unused112(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused112, p);
}

[[nodiscard]] auto level_open_is_unused113(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused113, p);
}

[[nodiscard]] auto level_open_is_unused114(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused114, p);
}

[[nodiscard]] auto level_open_is_unused115(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused115, p);
}

[[nodiscard]] auto level_open_is_unused116(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused116, p);
}

[[nodiscard]] auto level_open_is_unused117(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused117, p);
}

[[nodiscard]] auto level_open_is_unused118(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused118, p);
}

[[nodiscard]] auto level_open_is_unused119(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused119, p);
}

[[nodiscard]] auto level_open_is_unused120(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused120, p);
}

[[nodiscard]] auto level_open_is_unused121(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused121, p);
}

[[nodiscard]] auto level_open_is_unused122(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused122, p);
}

[[nodiscard]] auto level_open_is_unused123(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused123, p);
}

[[nodiscard]] auto level_open_is_unused124(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused124, p);
}

[[nodiscard]] auto level_open_is_unused125(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused125, p);
}

[[nodiscard]] auto level_open_is_unused126(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused126, p);
}

[[nodiscard]] auto level_open_is_unused127(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused127, p);
}

[[nodiscard]] auto level_open_is_unused128(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused128, p);
}

[[nodiscard]] auto level_open_is_unused129(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused129, p);
}

[[nodiscard]] auto level_open_is_unused130(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused130, p);
}

[[nodiscard]] auto level_open_is_unused131(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused131, p);
}

[[nodiscard]] auto level_open_is_unused132(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused132, p);
}

[[nodiscard]] auto level_open_is_unused133(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused133, p);
}

[[nodiscard]] auto level_open_is_unused134(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused134, p);
}

[[nodiscard]] auto level_open_is_unused135(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused135, p);
}

[[nodiscard]] auto level_open_is_unused136(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused136, p);
}

[[nodiscard]] auto level_open_is_unused137(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused137, p);
}

[[nodiscard]] auto level_open_is_unused138(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused138, p);
}

[[nodiscard]] auto level_open_is_unused139(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused139, p);
}

[[nodiscard]] auto level_open_is_unused140(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused140, p);
}

[[nodiscard]] auto level_open_is_unused141(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused141, p);
}

[[nodiscard]] auto level_open_is_unused142(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused142, p);
}

[[nodiscard]] auto level_open_is_unused143(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused143, p);
}

[[nodiscard]] auto level_open_is_unused144(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused144, p);
}

[[nodiscard]] auto level_open_is_unused145(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused145, p);
}

[[nodiscard]] auto level_open_is_unused146(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused146, p);
}

[[nodiscard]] auto level_open_is_unused147(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused147, p);
}

[[nodiscard]] auto level_open_is_unused148(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused148, p);
}

[[nodiscard]] auto level_open_is_unused149(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused149, p);
}

[[nodiscard]] auto level_open_is_unused150(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused150, p);
}

[[nodiscard]] auto level_open_is_unused151(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused151, p);
}

[[nodiscard]] auto level_open_is_unused152(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused152, p);
}

[[nodiscard]] auto level_open_is_boost(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_boost, p);
}

[[nodiscard]] auto level_open_is_weaponless_on_spawn(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_weaponless_on_spawn, p);
}

[[nodiscard]] auto level_open_is_noisy(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_noisy, p);
}

[[nodiscard]] auto level_open_is_hook(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_hook, p);
}

[[nodiscard]] auto level_open_is_sacrifice(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_sacrifice, p);
}

[[nodiscard]] auto level_open_is_debuff(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_debuff, p);
}

[[nodiscard]] auto level_open_is_venomous(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_venomous, p);
}

[[nodiscard]] auto level_open_is_able_to_see_invisible(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_able_to_see_invisible, p);
}

[[nodiscard]] auto level_open_is_able_to_be_invisible(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_able_to_be_invisible, p);
}

[[nodiscard]] auto level_open_is_invisible(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_invisible, p);
}

[[nodiscard]] auto level_open_is_critical_to_gameplay(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_critical_to_gameplay, p);
}

[[nodiscard]] auto level_open_is_able_to_jump_attack(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_able_to_jump_attack, p);
}

[[nodiscard]] auto level_open_is_fish(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_fish, p);
}

[[nodiscard]] auto level_open_is_unlucky(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unlucky, p);
}

[[nodiscard]] auto level_open_is_cursed(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_cursed, p);
}

[[nodiscard]] auto level_open_is_chocolate(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_chocolate, p);
}

[[nodiscard]] auto level_open_is_tick_on_eaten(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_tick_on_eaten, p);
}

[[nodiscard]] auto level_open_is_able_to_eat_food(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_able_to_eat_food, p);
}

[[nodiscard]] auto level_open_is_edible(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_edible, p);
}

[[nodiscard]] auto level_open_is_able_to_be_engulfed_blocked(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_able_to_be_engulfed_blocked, p);
}

[[nodiscard]] auto level_open_is_able_to_be_ensnared_blocked(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_able_to_be_ensnared_blocked, p);
}

[[nodiscard]] auto level_open_is_active_when_worn(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_active_when_worn, p);
}

[[nodiscard]] auto level_open_is_leather(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_leather, p);
}

[[nodiscard]] auto level_open_is_boots(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_boots, p);
}

[[nodiscard]] auto level_open_is_spider_giant(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_spider_giant, p);
}

[[nodiscard]] auto level_open_is_spider_baby(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_spider_baby, p);
}

[[nodiscard]] auto level_open_is_able_to_choose_targets(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_able_to_choose_targets, p);
}

[[nodiscard]] auto level_open_is_flesh_eater(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_flesh_eater, p);
}

[[nodiscard]] auto level_open_is_able_to_move(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_able_to_move, p);
}

[[nodiscard]] auto level_open_is_spider(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_spider, p);
}

[[nodiscard]] auto level_open_is_able_to_be_ensnared(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_able_to_be_ensnared, p);
}

[[nodiscard]] auto level_open_is_able_to_ensnare(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_able_to_ensnare, p);
}

[[nodiscard]] auto level_open_is_spiderweb(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_spiderweb, p);
}

[[nodiscard]] auto level_open_is_rubble(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_rubble, p);
}

[[nodiscard]] auto level_open_is_able_to_jump_land_then_pounce(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_able_to_jump_land_then_pounce, p);
}

[[nodiscard]] auto level_open_is_unused188(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_unused188, p);
}

[[nodiscard]] auto level_open_is_skeleton(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_skeleton, p);
}

[[nodiscard]] auto level_open_is_water_bound(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_water_bound, p);
}

[[nodiscard]] auto level_open_is_amphibious(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_amphibious, p);
}

[[nodiscard]] auto level_open_is_able_to_resurrect(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_able_to_resurrect, p);
}

[[nodiscard]] auto level_open_is_soft_landing(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_soft_landing, p);
}

[[nodiscard]] auto level_open_is_gelatinous(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_gelatinous, p);
}

[[nodiscard]] auto level_open_is_potion(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_potion, p);
}

[[nodiscard]] auto level_open_is_able_to_be_levitated(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_able_to_be_levitated, p);
}

[[nodiscard]] auto level_open_is_teleport_hazard(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_teleport_hazard, p);
}

[[nodiscard]] auto level_open_is_active_when_carried(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_active_when_carried, p);
}

[[nodiscard]] auto level_open_is_wearable(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_wearable, p);
}

[[nodiscard]] auto level_open_is_ring(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_ring, p);
}

[[nodiscard]] auto level_open_is_obs_to_hearing(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_obs_to_hearing, p);
}

[[nodiscard]] auto level_open_is_throwable(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_throwable, p);
}

[[nodiscard]] auto level_open_is_obs_to_throwing_over(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_obs_to_throwing_over, p);
}

[[nodiscard]] auto level_open_is_obs_to_throwing_onto(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_obs_to_throwing_onto, p);
}

[[nodiscard]] auto level_open_is_physics_trap(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_physics_trap, p);
}

[[nodiscard]] auto level_open_is_tick_on_strip(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_tick_on_strip, p);
}

[[nodiscard]] auto level_open_is_tick_on_worn(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_tick_on_worn, p);
}

[[nodiscard]] auto level_open_is_buff(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_buff, p);
}

[[nodiscard]] auto level_open_is_able_to_be_buffed(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_able_to_be_buffed, p);
}

[[nodiscard]] auto level_open_is_tick_on_drop(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_tick_on_drop, p);
}

[[nodiscard]] auto level_open_is_lava_bg(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_lava_bg, p);
}

[[nodiscard]] auto level_open_is_tick_on_use(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_tick_on_use, p);
}

[[nodiscard]] auto level_open_is_able_to_throw(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_able_to_throw, p);
}

[[nodiscard]] auto level_open_is_described_when_killed(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_described_when_killed, p);
}

[[nodiscard]] auto level_open_is_chest(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_chest, p);
}

[[nodiscard]] auto level_open_is_flammable(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_flammable, p);
}

[[nodiscard]] auto level_open_is_auto_wear(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_auto_wear, p);
}

[[nodiscard]] auto level_open_is_dead_when_discharged(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_dead_when_discharged, p);
}

[[nodiscard]] auto level_open_is_able_to_fire_weapons(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_able_to_fire_weapons, p);
}

[[nodiscard]] auto level_open_is_weapon(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_weapon, p);
}

[[nodiscard]] auto level_open_is_light_flicker(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_light_flicker, p);
}

[[nodiscard]] auto level_open_is_able_to_wear_items(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_able_to_wear_items, p);
}

[[nodiscard]] auto level_open_is_wand(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_wand, p);
}

[[nodiscard]] auto level_open_is_staff(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_staff, p);
}

[[nodiscard]] auto level_open_is_able_to_eat_treasure(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_able_to_eat_treasure, p);
}

[[nodiscard]] auto level_open_is_wait_on_anim(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_wait_on_anim, p);
}

[[nodiscard]] auto level_open_is_flat(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_flat, p);
}

[[nodiscard]] auto level_open_is_obs_to_beam(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_obs_to_beam, p);
}

[[nodiscard]] auto level_open_is_beam_weapon(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_beam_weapon, p);
}

[[nodiscard]] auto level_open_is_shown_stamina(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_shown_stamina, p);
}

[[nodiscard]] auto level_open_is_reeds(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_reeds, p);
}

[[nodiscard]] auto level_open_is_able_to_drop_all_items_on_death(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_able_to_drop_all_items_on_death, p);
}

[[nodiscard]] auto level_open_is_vault(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_vault, p);
}

[[nodiscard]] auto level_open_is_biome_underhell(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_biome_underhell, p);
}

[[nodiscard]] auto level_open_is_biome_graveyard(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_biome_graveyard, p);
}

[[nodiscard]] auto level_open_is_biome_nethervoid(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_biome_nethervoid, p);
}

[[nodiscard]] auto level_open_is_biome_bogland(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_biome_bogland, p);
}

[[nodiscard]] auto level_open_is_biome_dungeon(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_biome_dungeon, p);
}

[[nodiscard]] auto level_open_is_level_open_icon(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_level_open_icon, p);
}

[[nodiscard]] auto level_open_is_blit_bg(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_blit_bg, p);
}

[[nodiscard]] auto level_open_is_level_select_bg(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_level_select_bg, p);
}

[[nodiscard]] auto level_open_is_blit_hit_outline_w_black_inside(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_blit_hit_outline_w_black_inside, p);
}

[[nodiscard]] auto level_open_is_able_to_engulf(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_able_to_engulf, p);
}

[[nodiscard]] auto level_open_is_able_to_fall_sound(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_able_to_fall_sound, p);
}

[[nodiscard]] auto level_open_is_collision_hit_first_on_tile(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_collision_hit_first_on_tile, p);
}

[[nodiscard]] auto level_open_is_collision_hit_all_on_tile(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_collision_hit_all_on_tile, p);
}

[[nodiscard]] auto level_open_is_blit_hit_outline_w_invis_inside(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_blit_hit_outline_w_invis_inside, p);
}

[[nodiscard]] auto level_open_is_blit_when_obscured_outline(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_blit_when_obscured_outline, p);
}

[[nodiscard]] auto level_open_is_attackable_by_player(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_attackable_by_player, p);
}

[[nodiscard]] auto level_open_is_attackable_by_monst(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_attackable_by_monst, p);
}

[[nodiscard]] auto level_open_is_effect(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_effect, p);
}

[[nodiscard]] auto level_open_is_effect_attack(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_effect_attack, p);
}

[[nodiscard]] auto level_open_is_effect_blood(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_effect_blood, p);
}

[[nodiscard]] auto level_open_is_effect_ripple(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_effect_ripple, p);
}

[[nodiscard]] auto level_open_is_hit_when_dead(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_hit_when_dead, p);
}

[[nodiscard]] auto level_open_is_able_to_be_engulfed(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_able_to_be_engulfed, p);
}

[[nodiscard]] auto level_open_is_grouped_thing(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_grouped_thing, p);
}

[[nodiscard]] auto level_open_is_vision_360_degrees(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_vision_360_degrees, p);
}

[[nodiscard]] auto level_open_is_vision_180_degrees(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_vision_180_degrees, p);
}

[[nodiscard]] auto level_open_is_able_to_lunge(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_able_to_lunge, p);
}

[[nodiscard]] auto level_open_is_obs_to_paths(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_obs_to_paths, p);
}

[[nodiscard]] auto level_open_is_stealthy(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_stealthy, p);
}

[[nodiscard]] auto level_open_is_tireless(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_tireless, p);
}

[[nodiscard]] auto level_open_is_shown_noise(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_shown_noise, p);
}

[[nodiscard]] auto level_open_is_wait_on_dead_anim(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_wait_on_dead_anim, p);
}

[[nodiscard]] auto level_open_is_wall(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_wall, p);
}

[[nodiscard]] auto level_open_is_water_shallow(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_water_shallow, p);
}

[[nodiscard]] auto level_open_is_wood(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp
{
  TRACE_DEBUG();
  return level_open(g, v, l, is_wood, p);
}
