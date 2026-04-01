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

auto level_is_able_to_collect_items(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_able_to_collect_items, p, me);
}

auto level_is_able_to_collect_keys(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_able_to_collect_keys, p, me);
}

auto level_is_able_to_crush_grass(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_able_to_crush_grass, p, me);
}

auto level_is_able_to_fall_repeatedly(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_able_to_fall_repeatedly, p, me);
}

auto level_is_able_to_fall(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_able_to_fall, p, me);
}

auto level_is_able_to_jump(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_able_to_jump, p, me);
}

auto level_is_able_to_move_diagonally(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_able_to_move_diagonally, p, me);
}

auto level_is_able_to_move_through_walls(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_able_to_move_through_walls, p, me);
}

auto level_is_able_to_open(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_able_to_open, p, me);
}

auto level_is_able_to_shove(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_able_to_shove, p, me);
}

auto level_is_able_to_walk_through_walls(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_able_to_walk_through_walls, p, me);
}

auto level_is_animated_can_hflip(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_animated_can_hflip, p, me);
}

auto level_is_animated_no_dir(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_animated_no_dir, p, me);
}

auto level_is_animated_sync_first(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_animated_sync_first, p, me);
}

auto level_is_animated(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_animated, p, me);
}

auto level_is_barrel(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_barrel, p, me);
}

auto level_is_blit_centered(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_blit_centered, p, me);
}

auto level_is_blit_flush_per_line(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_blit_flush_per_line, p, me);
}

auto level_is_blit_if_has_seen(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_blit_if_has_seen, p, me);
}

auto level_is_blit_obscures(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_blit_obscures, p, me);
}

auto level_is_blit_on_ground(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_blit_on_ground, p, me);
}

auto level_is_blit_outlined(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_blit_outlined, p, me);
}

auto level_is_blit_per_pixel_lighting(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_blit_per_pixel_lighting, p, me);
}

auto level_is_blit_shown_in_chasms(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_blit_shown_in_chasms, p, me);
}

auto level_is_blit_shown_in_overlay(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_blit_shown_in_overlay, p, me);
}

auto level_is_blit_square_outlined(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_blit_square_outlined, p, me);
}

auto level_is_blit_when_obscured_faded(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_blit_when_obscured_faded, p, me);
}

auto level_is_border(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_border, p, me);
}

auto level_is_brazier(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_brazier, p, me);
}

auto level_is_bridge(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_bridge, p, me);
}

auto level_is_broken_on_death(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_broken_on_death, p, me);
}

auto level_is_burnable(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_burnable, p, me);
}

auto level_is_chasm(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_chasm, p, me);
}

auto level_is_collectable(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_collectable, p, me);
}

auto level_is_collision_circle_large(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_collision_circle_large, p, me);
}

auto level_is_collision_circle_small(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_collision_circle_small, p, me);
}

auto level_is_collision_detection_enabled(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_collision_detection_enabled, p, me);
}

auto level_is_collision_square(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_collision_square, p, me);
}

auto level_is_combustible(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_combustible, p, me);
}

auto level_is_corpse_on_death(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_corpse_on_death, p, me);
}

auto level_is_corridor(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_corridor, p, me);
}

auto level_is_crushable(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_crushable, p, me);
}

auto level_is_cursor_path_hazard(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_cursor_path_hazard, p, me);
}

auto level_is_cursor_path_none(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_cursor_path_none, p, me);
}

auto level_is_cursor_path_warning(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_cursor_path_warning, p, me);
}

auto level_is_cursor_path(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_cursor_path, p, me);
}

auto level_is_cursor(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_cursor, p, me);
}

auto level_is_damage_capped(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_damage_capped, p, me);
}

auto level_is_dead_on_collision(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_dead_on_collision, p, me);
}

auto level_is_dead_on_shoving(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_dead_on_shoving, p, me);
}

auto level_is_deep_water(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_deep_water, p, me);
}

auto level_is_described_cursor(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_described_cursor, p, me);
}

auto level_is_dirt(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_dirt, p, me);
}

auto level_is_dmap(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_dmap, p, me);
}

auto level_is_door_locked(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_door_locked, p, me);
}

auto level_is_door_secret(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_door_secret, p, me);
}

auto level_is_door_unlocked(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_door_unlocked, p, me);
}

auto level_is_dungeon_entrance(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_dungeon_entrance, p, me);
}

auto level_is_entrance(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_entrance, p, me);
}

auto level_is_ethereal(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_ethereal, p, me);
}

auto level_is_exit(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_exit, p, me);
}

auto level_is_explosion(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_explosion, p, me);
}

auto level_is_extinguished_on_death(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_extinguished_on_death, p, me);
}

auto level_is_fire(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_fire, p, me);
}

auto level_is_fireball(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_fireball, p, me);
}

auto level_is_flesh(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_flesh, p, me);
}

auto level_is_floating(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_floating, p, me);
}

auto level_is_floor(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_floor, p, me);
}

auto level_is_flying(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_flying, p, me);
}

auto level_is_foliage(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_foliage, p, me);
}

auto level_is_gaseous(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_gaseous, p, me);
}

auto level_is_ghost(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_ghost, p, me);
}

auto level_is_glass(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_glass, p, me);
}

auto level_is_gold(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_gold, p, me);
}

auto level_is_grass(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_grass, p, me);
}

auto level_is_health_bar_shown(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_health_bar_shown, p, me);
}

auto level_is_indestructible(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_indestructible, p, me);
}

auto level_is_insectoid(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_insectoid, p, me);
}

auto level_is_inventory_item(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_inventory_item, p, me);
}

auto level_is_item_droppable(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_item_droppable, p, me);
}

auto level_is_item_equipable(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_item_equipable, p, me);
}

auto level_is_item_mergeable(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_item_mergeable, p, me);
}

auto level_is_item(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_item, p, me);
}

auto level_is_key(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_key, p, me);
}

auto level_is_kobalos(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_kobalos, p, me);
}

auto level_is_lava(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_lava, p, me);
}

auto level_is_level_across_icon(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_level_across_icon, p, me);
}

auto level_is_level_curr(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_level_curr, p, me);
}

auto level_is_level_down_icon(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_level_down_icon, p, me);
}

auto level_is_level_final_icon(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_level_final_icon, p, me);
}

auto level_is_level_next_icon(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_level_next_icon, p, me);
}

auto level_is_level_not_visited_icon(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_level_not_visited_icon, p, me);
}

auto level_is_level_visited_icon(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_level_visited_icon, p, me);
}

auto level_is_levitating(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_levitating, p, me);
}

auto level_is_light_source(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_light_source, p, me);
}

auto level_is_loggable(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_loggable, p, me);
}

auto level_is_mantisman(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_mantisman, p, me);
}

auto level_is_meltable(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_meltable, p, me);
}

auto level_is_metal(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_metal, p, me);
}

auto level_is_minion(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_minion, p, me);
}

auto level_is_mob_kill_minions_on_death(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_mob_kill_minions_on_death, p, me);
}

auto level_is_mob(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_mob, p, me);
}

auto level_is_mob1(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_mob1, p, me);
}

auto level_is_mob2(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_mob2, p, me);
}

auto level_is_monst_group_easy(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_monst_group_easy, p, me);
}

auto level_is_monst_group_hard(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_monst_group_hard, p, me);
}

auto level_is_monst_group_mob(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_monst_group_mob, p, me);
}

auto level_is_monst(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_monst, p, me);
}

auto level_is_needs_move_confirm(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_needs_move_confirm, p, me);
}

auto level_is_obs_to_cursor_path(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_obs_to_cursor_path, p, me);
}

auto level_is_obs_to_explosion(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_obs_to_explosion, p, me);
}

auto level_is_obs_to_falling_onto(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_obs_to_falling_onto, p, me);
}

auto level_is_obs_to_fire(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_obs_to_fire, p, me);
}

auto level_is_obs_to_jump_over(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_obs_to_jump_over, p, me);
}

auto level_is_obs_to_jumping_onto(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_obs_to_jumping_onto, p, me);
}

auto level_is_obs_to_jumping_out_of(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_obs_to_jumping_out_of, p, me);
}

auto level_is_obs_to_movement(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_obs_to_movement, p, me);
}

auto level_is_obs_to_spawning(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_obs_to_spawning, p, me);
}

auto level_is_obs_to_teleporting_onto(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_obs_to_teleporting_onto, p, me);
}

auto level_is_obs_to_vision(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_obs_to_vision, p, me);
}

auto level_is_obs_when_dead(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_obs_when_dead, p, me);
}

auto level_is_openable(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_openable, p, me);
}

auto level_is_physics_explosion(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_physics_explosion, p, me);
}

auto level_is_physics_temperature(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_physics_temperature, p, me);
}

auto level_is_physics_water(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_physics_water, p, me);
}

auto level_is_pillar(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_pillar, p, me);
}

auto level_is_plant(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_plant, p, me);
}

auto level_is_player(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_player, p, me);
}

auto level_is_projectile(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_projectile, p, me);
}

auto level_is_removable_on_err(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_removable_on_err, p, me);
}

auto level_is_removable_when_dead_on_err(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_removable_when_dead_on_err, p, me);
}

auto level_is_rock(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_rock, p, me);
}

auto level_is_shovable(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_shovable, p, me);
}

auto level_is_slime(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_slime, p, me);
}

auto level_is_smoke(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_smoke, p, me);
}

auto level_is_steam(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_steam, p, me);
}

auto level_is_stone(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_stone, p, me);
}

auto level_is_submergible(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_submergible, p, me);
}

auto level_is_teleport_blocked(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_teleport_blocked, p, me);
}

auto level_is_teleport(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_teleport, p, me);
}

auto level_is_tick_end_delay(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_tick_end_delay, p, me);
}

auto level_is_tickable(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_tickable, p, me);
}

auto level_is_tiled(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_tiled, p, me);
}

auto level_is_trap(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_trap, p, me);
}

auto level_is_treasure(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_treasure, p, me);
}

auto level_is_undead(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_undead, p, me);
}

auto level_is_unused1(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused1, p, me);
}

auto level_is_unused10(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused10, p, me);
}

auto level_is_unused11(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused11, p, me);
}

auto level_is_unused12(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused12, p, me);
}

auto level_is_unused13(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused13, p, me);
}

auto level_is_unused14(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused14, p, me);
}

auto level_is_unused15(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused15, p, me);
}

auto level_is_unused16(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused16, p, me);
}

auto level_is_unused17(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused17, p, me);
}

auto level_is_unused18(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused18, p, me);
}

auto level_is_unused19(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused19, p, me);
}

auto level_is_unused2(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused2, p, me);
}

auto level_is_unused20(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused20, p, me);
}

auto level_is_unused21(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused21, p, me);
}

auto level_is_unused22(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused22, p, me);
}

auto level_is_unused23(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused23, p, me);
}

auto level_is_unused24(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused24, p, me);
}

auto level_is_unused25(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused25, p, me);
}

auto level_is_unused26(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused26, p, me);
}

auto level_is_unused27(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused27, p, me);
}

auto level_is_unused28(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused28, p, me);
}

auto level_is_unused29(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused29, p, me);
}

auto level_is_unused3(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused3, p, me);
}

auto level_is_unused30(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused30, p, me);
}

auto level_is_unused31(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused31, p, me);
}

auto level_is_unused32(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused32, p, me);
}

auto level_is_unused33(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused33, p, me);
}

auto level_is_unused34(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused34, p, me);
}

auto level_is_unused35(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused35, p, me);
}

auto level_is_unused36(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused36, p, me);
}

auto level_is_unused37(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused37, p, me);
}

auto level_is_unused38(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused38, p, me);
}

auto level_is_unused39(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused39, p, me);
}

auto level_is_unused4(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused4, p, me);
}

auto level_is_unused40(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused40, p, me);
}

auto level_is_unused41(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused41, p, me);
}

auto level_is_unused42(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused42, p, me);
}

auto level_is_unused43(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused43, p, me);
}

auto level_is_unused44(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused44, p, me);
}

auto level_is_unused45(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused45, p, me);
}

auto level_is_unused46(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused46, p, me);
}

auto level_is_unused47(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused47, p, me);
}

auto level_is_unused48(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused48, p, me);
}

auto level_is_blit_hit_effect2(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_blit_hit_effect2, p, me);
}

auto level_is_unused5(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused5, p, me);
}

auto level_is_able_to_fall_sound(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_able_to_fall_sound, p, me);
}

auto level_is_collision_hit_first_on_tile(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_collision_hit_first_on_tile, p, me);
}

auto level_is_collision_hit_all_on_tile(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_collision_hit_all_on_tile, p, me);
}

auto level_is_blit_hit_effect1(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_blit_hit_effect1, p, me);
}

auto level_is_blit_when_obscured_outline(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_blit_when_obscured_outline, p, me);
}

auto level_is_attackable_by_player(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_attackable_by_player, p, me);
}

auto level_is_attackable_by_monst(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_attackable_by_monst, p, me);
}

auto level_is_effect(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_effect, p, me);
}

auto level_is_effect_attack(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_effect_attack, p, me);
}

auto level_is_effect_blood(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_effect_blood, p, me);
}

auto level_is_unused6(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused6, p, me);
}

auto level_is_hit_when_dead(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_hit_when_dead, p, me);
}

auto level_is_able_to_fire_projectiles(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_able_to_fire_projectiles, p, me);
}

auto level_is_blitzhound(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_blitzhound, p, me);
}

auto level_is_unused63(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused63, p, me);
}

auto level_is_grouped_thing(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_grouped_thing, p, me);
}

auto level_is_vision_360_degrees(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_vision_360_degrees, p, me);
}

auto level_is_vision_180_degrees(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_vision_180_degrees, p, me);
}

auto level_is_able_to_lunge(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_able_to_lunge, p, me);
}

auto level_is_obs_to_paths(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_obs_to_paths, p, me);
}

auto level_is_unused7(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused7, p, me);
}

auto level_is_unused8(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused8, p, me);
}

auto level_is_unused9(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_unused9, p, me);
}

auto level_is_wait_on_dead_anim(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_wait_on_dead_anim, p, me);
}

auto level_is_wall(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_wall, p, me);
}

auto level_is_water(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_water, p, me);
}

auto level_is_wood(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> Thingp
{
  TRACE_DEBUG();
  return level_flag(g, v, l, is_wood, p, me);
}
