//
// Copyright goblinhack@gmail.com
//

#ifndef MY_THING_TEMPLATE_HPP
#define MY_THING_TEMPLATE_HPP

#include "my_color.hpp"
#include "my_enum.hpp"
#include "my_types.hpp"

#include <stdarg.h>
#include <string>

//
// Thing flags enum
//
#define THING_FLAG_ENUM(list_macro)                                                                                                        \
  CLANG_FORMAT_INDENT()                                                                     /* dummy line for clang indentation fixup */   \
  list_macro(is_able_to_collect_items, "is_able_to_collect_items"),                         /* newline */                                  \
      list_macro(is_able_to_collect_keys, "is_able_to_collect_keys"),                       /* newline */                                  \
      list_macro(is_able_to_crush_grass, "is_able_to_crush_grass"),                         /* newline */                                  \
      list_macro(is_able_to_fall, "is_able_to_fall"),                                       /* newline */                                  \
      list_macro(is_able_to_jump, "is_able_to_jump"),                                       /* newline */                                  \
      list_macro(is_able_to_open, "is_able_to_open"),                                       /* newline */                                  \
      list_macro(is_able_to_shove, "is_able_to_shove"),                                     /* newline */                                  \
      list_macro(is_animated_can_hflip, "is_animated_can_hflip"),                           /* newline */                                  \
      list_macro(is_animated_no_dir, "is_animated_no_dir"),                                 /* newline */                                  \
      list_macro(is_animated_sync_first, "is_animated_sync_first"),                         /* newline */                                  \
      list_macro(is_animated, "is_animated"),                                               /* newline */                                  \
      list_macro(is_barrel, "is_barrel"),                                                   /* newline */                                  \
      list_macro(is_blit_centered, "is_blit_centered"),                                     /* newline */                                  \
      list_macro(is_blit_if_has_seen, "is_blit_if_has_seen"),                               /* newline */                                  \
      list_macro(is_blit_obscures, "is_blit_obscures"),                                     /* newline */                                  \
      list_macro(is_blit_flush_per_line, "is_blit_flush_per_line"),                         /* newline */                                  \
      list_macro(is_blit_outlined, "is_blit_outlined"),                                     /* newline */                                  \
      list_macro(is_blit_shown_in_chasms, "is_blit_shown_in_chasms"),                       /* newline */                                  \
      list_macro(is_blit_shown_in_overlay, "is_blit_shown_in_overlay"),                     /* newline */                                  \
      list_macro(is_blit_square_outlined, "is_blit_square_outlined"),                       /* newline */                                  \
      list_macro(is_blit_when_obscured_faded, "is_blit_when_obscured_faded"),               /* newline */                                  \
      list_macro(is_brazier, "is_brazier"),                                                 /* newline */                                  \
      list_macro(is_bridge, "is_bridge"),                                                   /* newline */                                  \
      list_macro(is_broken_on_death, "is_broken_on_death"),                                 /* newline */                                  \
      list_macro(is_burnable, "is_burnable"),                                               /* newline */                                  \
      list_macro(is_chasm, "is_chasm"),                                                     /* newline */                                  \
      list_macro(is_collectable, "is_collectable"),                                         /* newline */                                  \
      list_macro(is_combustible, "is_combustible"),                                         /* newline */                                  \
      list_macro(is_corpse_on_death, "is_corpse_on_death"),                                 /* newline */                                  \
      list_macro(is_corridor, "is_corridor"),                                               /* newline */                                  \
      list_macro(is_crushable, "is_crushable"),                                             /* newline */                                  \
      list_macro(is_cursor_path_hazard, "is_cursor_path_hazard"),                           /* newline */                                  \
      list_macro(is_cursor_path_none, "is_cursor_path_none"),                               /* newline */                                  \
      list_macro(is_cursor_path, "is_cursor_path"),                                         /* newline */                                  \
      list_macro(is_cursor, "is_cursor"),                                                   /* newline */                                  \
      list_macro(is_damage_capped, "is_damage_capped"),                                     /* newline */                                  \
      list_macro(is_dead_on_shoving, "is_dead_on_shoving"),                                 /* newline */                                  \
      list_macro(is_deep_water, "is_deep_water"),                                           /* newline */                                  \
      list_macro(is_described_cursor, "is_described_cursor"),                               /* newline */                                  \
      list_macro(is_dirt, "is_dirt"),                                                       /* newline */                                  \
      list_macro(is_door_locked, "is_door_locked"),                                         /* newline */                                  \
      list_macro(is_door_secret, "is_door_secret"),                                         /* newline */                                  \
      list_macro(is_door_unlocked, "is_door_unlocked"),                                     /* newline */                                  \
      list_macro(is_dungeon_entrance, "is_dungeon_entrance"),                               /* newline */                                  \
      list_macro(is_entrance, "is_entrance"),                                               /* newline */                                  \
      list_macro(is_ethereal, "is_ethereal"),                                               /* newline */                                  \
      list_macro(is_exit, "is_exit"),                                                       /* newline */                                  \
      list_macro(is_explosion, "is_explosion"),                                             /* newline */                                  \
      list_macro(is_extinguished_on_death, "is_extinguished_on_death"),                     /* newline */                                  \
      list_macro(is_fire, "is_fire"),                                                       /* newline */                                  \
      list_macro(is_floating, "is_floating"),                                               /* newline */                                  \
      list_macro(is_floor, "is_floor"),                                                     /* newline */                                  \
      list_macro(is_flying, "is_flying"),                                                   /* newline */                                  \
      list_macro(is_foliage, "is_foliage"),                                                 /* newline */                                  \
      list_macro(is_gaseous, "is_gaseous"),                                                 /* newline */                                  \
      list_macro(is_ghost, "is_ghost"),                                                     /* newline */                                  \
      list_macro(is_grass, "is_grass"),                                                     /* newline */                                  \
      list_macro(is_health_visible, "is_health_visible"),                                   /* newline */                                  \
      list_macro(is_indestructible, "is_indestructible"),                                   /* newline */                                  \
      list_macro(is_inventory_item, "is_inventory_item"),                                   /* newline */                                  \
      list_macro(is_item_droppable, "is_item_droppable"),                                   /* newline */                                  \
      list_macro(is_item_equipable, "is_item_equipable"),                                   /* newline */                                  \
      list_macro(is_item_mergeable, "is_item_mergeable"),                                   /* newline */                                  \
      list_macro(is_item, "is_item"),                                                       /* newline */                                  \
      list_macro(is_key, "is_key"),                                                         /* newline */                                  \
      list_macro(is_kobalos, "is_kobalos"),                                                 /* newline */                                  \
      list_macro(is_lava, "is_lava"),                                                       /* newline */                                  \
      list_macro(is_unused46, "is_unused46"),                                               /* newline */                                  \
      list_macro(is_level_curr, "is_level_curr"),                                           /* newline */                                  \
      list_macro(is_unused47, "is_unused47"),                                               /* newline */                                  \
      list_macro(is_level_final_icon, "is_level_final_icon"),                               /* newline */                                  \
      list_macro(is_level_next_icon, "is_level_next_icon"),                                 /* newline */                                  \
      list_macro(is_level_locked_icon, "is_level_locked_icon"),                             /* newline */                                  \
      list_macro(is_level_closed_icon, "is_level_closed_icon"),                             /* newline */                                  \
      list_macro(is_levitating, "is_levitating"),                                           /* newline */                                  \
      list_macro(is_light_source, "is_light_source"),                                       /* newline */                                  \
      list_macro(is_loggable, "is_loggable"),                                               /* newline */                                  \
      list_macro(is_minion, "is_minion"),                                                   /* newline */                                  \
      list_macro(is_mob, "is_mob"),                                                         /* newline */                                  \
      list_macro(is_mob1, "is_mob1"),                                                       /* newline */                                  \
      list_macro(is_mob2, "is_mob2"),                                                       /* newline */                                  \
      list_macro(is_monst_group_easy, "is_monst_group_easy"),                               /* newline */                                  \
      list_macro(is_monst_group_hard, "is_monst_group_hard"),                               /* newline */                                  \
      list_macro(is_monst_group_mob, "is_monst_group_mob"),                                 /* newline */                                  \
      list_macro(is_monst, "is_monst"),                                                     /* newline */                                  \
      list_macro(is_needs_move_confirm, "is_needs_move_confirm"),                           /* newline */                                  \
      list_macro(is_obs_to_cursor_path, "is_obs_to_cursor_path"),                           /* newline */                                  \
      list_macro(is_obs_to_explosion, "is_obs_to_explosion"),                               /* newline */                                  \
      list_macro(is_obs_to_falling_onto, "is_obs_to_falling_onto"),                         /* newline */                                  \
      list_macro(is_obs_to_fire, "is_obs_to_fire"),                                         /* newline */                                  \
      list_macro(is_obs_to_jump_over, "is_obs_to_jump_over"),                               /* newline */                                  \
      list_macro(is_obs_to_jumping_onto, "is_obs_to_jumping_onto"),                         /* newline */                                  \
      list_macro(is_obs_to_teleporting_onto, "is_obs_to_teleporting_onto"),                 /* newline */                                  \
      list_macro(is_obs_to_movement, "is_obs_to_movement"),                                 /* newline */                                  \
      list_macro(is_obs_to_vision, "is_obs_to_vision"),                                     /* newline */                                  \
      list_macro(is_openable, "is_openable"),                                               /* newline */                                  \
      list_macro(is_physics_explosion, "is_physics_explosion"),                             /* newline */                                  \
      list_macro(is_physics_temperature, "is_physics_temperature"),                         /* newline */                                  \
      list_macro(is_physics_water, "is_physics_water"),                                     /* newline */                                  \
      list_macro(is_pillar, "is_pillar"),                                                   /* newline */                                  \
      list_macro(is_player, "is_player"),                                                   /* newline */                                  \
      list_macro(is_rock, "is_rock"),                                                       /* newline */                                  \
      list_macro(is_shovable, "is_shovable"),                                               /* newline */                                  \
      list_macro(is_slime, "is_slime"),                                                     /* newline */                                  \
      list_macro(is_smoke, "is_smoke"),                                                     /* newline */                                  \
      list_macro(is_steam, "is_steam"),                                                     /* newline */                                  \
      list_macro(is_submergible, "is_submergible"),                                         /* newline */                                  \
      list_macro(is_teleport_blocked, "is_teleport_blocked"),                               /* newline */                                  \
      list_macro(is_teleport, "is_teleport"),                                               /* newline */                                  \
      list_macro(is_tick_end_delay, "is_tick_end_delay"),                                   /* newline */                                  \
      list_macro(is_tickable, "is_tickable"),                                               /* newline */                                  \
      list_macro(is_tiled, "is_tiled"),                                                     /* newline */                                  \
      list_macro(is_trap, "is_trap"),                                                       /* newline */                                  \
      list_macro(is_treasure, "is_treasure"),                                               /* newline */                                  \
      list_macro(is_undead, "is_undead"),                                                   /* newline */                                  \
      list_macro(is_unused1, "is_unused1"),                                                 /* newline */                                  \
      list_macro(is_unused2, "is_unused2"),                                                 /* newline */                                  \
      list_macro(is_unused3, "is_unused3"),                                                 /* newline */                                  \
      list_macro(is_unused4, "is_unused4"),                                                 /* newline */                                  \
      list_macro(is_unused5, "is_unused5"),                                                 /* newline */                                  \
      list_macro(is_unused6, "is_unused6"),                                                 /* newline */                                  \
      list_macro(is_unused7, "is_unused7"),                                                 /* newline */                                  \
      list_macro(is_unused8, "is_unused8"),                                                 /* newline */                                  \
      list_macro(is_unused9, "is_unused9"),                                                 /* newline */                                  \
      list_macro(is_unused10, "is_unused10"),                                               /* newline */                                  \
      list_macro(is_unused11, "is_unused11"),                                               /* newline */                                  \
      list_macro(is_unused12, "is_unused12"),                                               /* newline */                                  \
      list_macro(is_unused13, "is_unused13"),                                               /* newline */                                  \
      list_macro(is_unused14, "is_unused14"),                                               /* newline */                                  \
      list_macro(is_unused15, "is_unused15"),                                               /* newline */                                  \
      list_macro(is_unused16, "is_unused16"),                                               /* newline */                                  \
      list_macro(is_unused17, "is_unused17"),                                               /* newline */                                  \
      list_macro(is_unused18, "is_unused18"),                                               /* newline */                                  \
      list_macro(is_unused19, "is_unused19"),                                               /* newline */                                  \
      list_macro(is_unused20, "is_unused20"),                                               /* newline */                                  \
      list_macro(is_unused21, "is_unused21"),                                               /* newline */                                  \
      list_macro(is_unused22, "is_unused22"),                                               /* newline */                                  \
      list_macro(is_unused23, "is_unused23"),                                               /* newline */                                  \
      list_macro(is_unused24, "is_unused24"),                                               /* newline */                                  \
      list_macro(is_unused25, "is_unused25"),                                               /* newline */                                  \
      list_macro(is_unused26, "is_unused26"),                                               /* newline */                                  \
      list_macro(is_unused27, "is_unused27"),                                               /* newline */                                  \
      list_macro(is_unused28, "is_unused28"),                                               /* newline */                                  \
      list_macro(is_unused29, "is_unused29"),                                               /* newline */                                  \
      list_macro(is_unused30, "is_unused30"),                                               /* newline */                                  \
      list_macro(is_unused31, "is_unused31"),                                               /* newline */                                  \
      list_macro(is_unused32, "is_unused32"),                                               /* newline */                                  \
      list_macro(is_unused33, "is_unused33"),                                               /* newline */                                  \
      list_macro(is_unused34, "is_unused34"),                                               /* newline */                                  \
      list_macro(is_unused35, "is_unused35"),                                               /* newline */                                  \
      list_macro(is_unused36, "is_unused36"),                                               /* newline */                                  \
      list_macro(is_unused37, "is_unused37"),                                               /* newline */                                  \
      list_macro(is_stamina_visible, "is_stamina_visible"),                                 /* newline */                                  \
      list_macro(is_reeds, "is_reeds"),                                                     /* newline */                                  \
      list_macro(is_vault, "is_vault"),                                                     /* newline */                                  \
      list_macro(is_biome_underhell, "is_biome_underhell"),                                 /* newline */                                  \
      list_macro(is_biome_graveyard, "is_biome_graveyard"),                                 /* newline */                                  \
      list_macro(is_biome_nethervoid, "is_biome_nethervoid"),                               /* newline */                                  \
      list_macro(is_biome_bogland, "is_biome_bogland"),                                     /* newline */                                  \
      list_macro(is_biome_dungeon, "is_biome_dungeon"),                                     /* newline */                                  \
      list_macro(is_level_open_icon, "is_level_open_icon"),                                 /* newline */                                  \
      list_macro(is_blit_bg, "is_blit_bg"),                                                 /* newline */                                  \
      list_macro(is_level_select_bg, "is_level_select_bg"),                                 /* newline */                                  \
      list_macro(is_blit_hit_outline_w_black_inside, "is_blit_hit_outline_w_black_inside"), /* newline */                                  \
      list_macro(is_able_to_fall_sound, "is_able_to_fall_sound"),                           /* newline */                                  \
      list_macro(is_collision_hit_first_on_tile, "is_collision_hit_first_on_tile"),         /* newline */                                  \
      list_macro(is_collision_hit_all_on_tile, "is_collision_hit_all_on_tile"),             /* newline */                                  \
      list_macro(is_blit_hit_outline_w_invis_inside, "is_blit_hit_outline_w_invis_inside"), /* newline */                                  \
      list_macro(is_blit_when_obscured_outline, "is_blit_when_obscured_outline"),           /* newline */                                  \
      list_macro(is_attackable_by_player, "is_attackable_by_player"),                       /* newline */                                  \
      list_macro(is_attackable_by_monst, "is_attackable_by_monst"),                         /* newline */                                  \
      list_macro(is_effect, "is_effect"),                                                   /* newline */                                  \
      list_macro(is_effect_attack, "is_effect_attack"),                                     /* newline */                                  \
      list_macro(is_effect_blood, "is_effect_blood"),                                       /* newline */                                  \
      list_macro(is_hit_when_dead, "is_hit_when_dead"),                                     /* newline */                                  \
      list_macro(is_able_to_fire_projectiles, "is_able_to_fire_projectiles"),               /* newline */                                  \
      list_macro(is_blitzhound, "is_blitzhound"),                                           /* newline */                                  \
      list_macro(is_unused63, "is_unused63"),                                               /* newline */                                  \
      list_macro(is_grouped_thing, "is_grouped_thing"),                                     /* newline */                                  \
      list_macro(is_vision_360_degrees, "is_vision_360_degrees"),                           /* newline */                                  \
      list_macro(is_vision_180_degrees, "is_vision_180_degrees"),                           /* newline */                                  \
      list_macro(is_able_to_lunge, "is_able_to_lunge"),                                     /* newline */                                  \
      list_macro(is_obs_to_paths, "is_obs_to_paths"),                                       /* newline */                                  \
      list_macro(is_removable_when_dead_on_err, "is_removable_when_dead_on_err"),           /* newline */                                  \
      list_macro(is_removable_on_err, "is_removable_on_err"),                               /* newline */                                  \
      list_macro(is_obs_when_dead, "is_obs_when_dead"),                                     /* newline */                                  \
      list_macro(is_able_to_fall_repeatedly, "is_able_to_fall_repeatedly"),                 /* newline */                                  \
      list_macro(is_able_to_move_through_walls, "is_able_to_move_through_walls"),           /* newline */                                  \
      list_macro(is_able_to_move_diagonally, "is_able_to_move_diagonally"),                 /* newline */                                  \
      list_macro(is_blit_on_ground, "is_blit_on_ground"),                                   /* newline */                                  \
      list_macro(is_insectoid, "is_insectoid"),                                             /* newline */                                  \
      list_macro(is_mantisman, "is_mantisman"),                                             /* newline */                                  \
      list_macro(is_dmap, "is_dmap"),                                                       /* newline */                                  \
      list_macro(is_obs_to_spawning, "is_obs_to_spawning"),                                 /* newline */                                  \
      list_macro(is_mob_kill_minions_on_death, "is_mob_kill_minions_on_death"),             /* newline */                                  \
      list_macro(is_border, "is_border"),                                                   /* newline */                                  \
      list_macro(is_collision_detection_enabled, "is_collision_detection_enabled"),         /* newline */                                  \
      list_macro(is_gold, "is_gold"),                                                       /* newline */                                  \
      list_macro(is_glass, "is_glass"),                                                     /* newline */                                  \
      list_macro(is_plant, "is_plant"),                                                     /* newline */                                  \
      list_macro(is_wood, "is_wood"),                                                       /* newline */                                  \
      list_macro(is_metal, "is_metal"),                                                     /* newline */                                  \
      list_macro(is_flesh, "is_flesh"),                                                     /* newline */                                  \
      list_macro(is_stone, "is_stone"),                                                     /* newline */                                  \
      list_macro(is_meltable, "is_meltable"),                                               /* newline */                                  \
      list_macro(is_blit_per_pixel_lighting, "is_blit_per_pixel_lighting"),                 /* newline */                                  \
      list_macro(is_dead_on_collision, "is_dead_on_collision"),                             /* newline */                                  \
      list_macro(is_obs_to_jumping_out_of, "is_obs_to_jumping_out_of"),                     /* newline */                                  \
      list_macro(is_cursor_path_warning, "is_cursor_path_warning"),                         /* newline */                                  \
      list_macro(is_collision_circle_large, "is_collision_circle_large"),                   /* newline */                                  \
      list_macro(is_collision_circle_small, "is_collision_circle_small"),                   /* newline */                                  \
      list_macro(is_collision_square, "is_collision_square"),                               /* newline */                                  \
      list_macro(is_fireball, "is_fireball"),                                               /* newline */                                  \
      list_macro(is_projectile, "is_projectile"),                                           /* newline */                                  \
      list_macro(is_wait_on_dead_anim, "is_wait_on_dead_anim"),                             /* newline */                                  \
      list_macro(is_able_to_walk_through_walls, "is_able_to_walk_through_walls"),           /* newline */                                  \
      list_macro(is_wall, "is_wall"),                                                       /* newline */                                  \
      list_macro(is_water, "is_water"),                                                     /* newline */

ENUM_DEF_H(THING_FLAG_ENUM, ThingFlag)

//
// Things all at the same z layer that can be drawn at the same x,y
//
#define MAP_Z_DEPTH_ENUM(list_macro)                                                                                                       \
  CLANG_FORMAT_INDENT()                                    /* dummy line for clang indentation fixup */                                    \
  list_macro(MAP_Z_DEPTH_FLOOR, "floor"),                  /* newline */                                                                   \
      list_macro(MAP_Z_DEPTH_LIQUID, "e.g. water"),        /* newline */                                                                   \
      list_macro(MAP_Z_DEPTH_LIQUID2, "e.g. deep water"),  /* newline */                                                                   \
      list_macro(MAP_Z_DEPTH_GRASS, "grass"),              /* newline */                                                                   \
      list_macro(MAP_Z_DEPTH_OBJ, "monsts"),               /* newline */                                                                   \
      list_macro(MAP_Z_DEPTH_WALL, "wall"),                /* newline */                                                                   \
      list_macro(MAP_Z_DEPTH_PLAYER, "monsts"),            /* newline */                                                                   \
      list_macro(MAP_Z_DEPTH_FOLIAGE, "obscuring plants"), /* newline */                                                                   \
      list_macro(MAP_Z_DEPTH_EFFECT, "effect"),            /* newline */                                                                   \
      list_macro(MAP_Z_DEPTH_PROJECTILE, "projectile"),    /* newline */                                                                   \
      list_macro(MAP_Z_DEPTH_GAS, "fire, smoke"),          /* newline */

ENUM_DEF_H(MAP_Z_DEPTH_ENUM, MapZDepth)

#define MAP_Z_DEPTH_ENUM_FIRST ((MapZDepth) 0)

#define FOR_ALL_MAP_Z_DEPTH(_iter_)                                                                                                        \
  for (MapZDepth _iter_ = MAP_Z_DEPTH_ENUM_FIRST; /* newline */                                                                            \
       (_iter_) < MAP_Z_DEPTH_ENUM_MAX;           /* newline */                                                                            \
       (_iter_) = static_cast< MapZDepth >(static_cast< int >(_iter_) + 1))

//
// Monst challenge level
//
#define MONST_GROUP_ENUM(list_macro)                                                                                                       \
  CLANG_FORMAT_INDENT()                  /* dummy line for clang indentation fixup */                                                      \
  list_macro(MONST_GROUP_MOB, "0"),      /* Used for monsters that are only generated by mobs */                                           \
      list_macro(MONST_GROUP_EASY, "1"), /* newline */                                                                                     \
      list_macro(MONST_GROUP_HARD, "2"), /* newline */

ENUM_DEF_H(MONST_GROUP_ENUM, ThingMonstGroup)

//
// Level Type
//
#define LEVEL_TYPE_ENUM(list_macro)                                                                                                        \
  CLANG_FORMAT_INDENT()                        /* dummy line for clang indentation fixup */                                                \
  list_macro(LEVEL_TYPE_NONE, "none"),         /* newline */                                                                               \
      list_macro(LEVEL_TYPE_NORMAL, "normal"), /* newline */                                                                               \
      list_macro(LEVEL_TYPE_TEST, "test"),     /* newline */                                                                               \
      list_macro(LEVEL_TYPE_BOSS1, "boss1"),   /* newline */                                                                               \
      list_macro(LEVEL_TYPE_BOSS2, "boss2"),   /* newline */                                                                               \
      list_macro(LEVEL_TYPE_BOSS3, "boss3"),   /* newline */                                                                               \
      list_macro(LEVEL_TYPE_BOSS4, "boss4"),   /* newline */                                                                               \
      list_macro(LEVEL_TYPE_BOSS5, "boss5"),   /* newline */

ENUM_DEF_H(LEVEL_TYPE_ENUM, LevelType)

#define LEVEL_TYPE_FIRST LEVEL_TYPE_NORMAL

//
// Biome
//
#define BIOME_ENUM(list_macro)                                                                                                             \
  CLANG_FORMAT_INDENT()                           /* dummy line for clang indentation fixup */                                             \
  list_macro(BIOME_NONE, "NONE"),                 /* newline */                                                                            \
      list_macro(BIOME_DUNGEON, "Dungeon"),       /* newline */                                                                            \
      list_macro(BIOME_BOGLAND, "Bogland"),       /* newline */                                                                            \
      list_macro(BIOME_NETHERVOID, "Nethervoid"), /* newline */                                                                            \
      list_macro(BIOME_GRAVEYARD, "Graveyard"),   /* newline */                                                                            \
      list_macro(BIOME_UNDERHELL, "Underhell"),   /* newline */

ENUM_DEF_H(BIOME_ENUM, Biome)

//
// Thing anim enum
//
#define THING_ANIM_ENUM(list_macro)                                                                                                        \
  CLANG_FORMAT_INDENT()                                           /* dummy line for clang indentation fixup */                             \
  list_macro(THING_ANIM_JOIN_BL, "IS_JOIN_BL"),                   /* newline */                                                            \
      list_macro(THING_ANIM_JOIN_BL2, "IS_JOIN_BL2"),             /* newline */                                                            \
      list_macro(THING_ANIM_JOIN_BLOCK, "IS_JOIN_BLOCK"),         /* newline */                                                            \
      list_macro(THING_ANIM_JOIN_BOT, "IS_JOIN_BOT"),             /* newline */                                                            \
      list_macro(THING_ANIM_JOIN_BR, "IS_JOIN_BR"),               /* newline */                                                            \
      list_macro(THING_ANIM_JOIN_BR2, "IS_JOIN_BR2"),             /* newline */                                                            \
      list_macro(THING_ANIM_JOIN_HORIZ, "IS_JOIN_HORIZ"),         /* newline */                                                            \
      list_macro(THING_ANIM_JOIN_HORIZ2, "IS_JOIN_HORIZ"),        /* newline */                                                            \
      list_macro(THING_ANIM_JOIN_LEFT, "IS_JOIN_LEFT"),           /* newline */                                                            \
      list_macro(THING_ANIM_JOIN_NODE, "IS_JOIN_NODE"),           /* newline */                                                            \
      list_macro(THING_ANIM_JOIN_RIGHT, "IS_JOIN_RIGHT"),         /* newline */                                                            \
      list_macro(THING_ANIM_JOIN_T_1, "IS_JOIN_T_1"),             /* newline */                                                            \
      list_macro(THING_ANIM_JOIN_T_2, "IS_JOIN_T_2"),             /* newline */                                                            \
      list_macro(THING_ANIM_JOIN_T_3, "IS_JOIN_T_3"),             /* newline */                                                            \
      list_macro(THING_ANIM_JOIN_T, "IS_JOIN_T"),                 /* newline */                                                            \
      list_macro(THING_ANIM_JOIN_T180_1, "IS_JOIN_T180_"),        /* newline */                                                            \
      list_macro(THING_ANIM_JOIN_T180_2, "IS_JOIN_T180_"),        /* newline */                                                            \
      list_macro(THING_ANIM_JOIN_T180_3, "IS_JOIN_T180_"),        /* newline */                                                            \
      list_macro(THING_ANIM_JOIN_T180, "IS_JOIN_T180"),           /* newline */                                                            \
      list_macro(THING_ANIM_JOIN_T270_1, "IS_JOIN_T270_"),        /* newline */                                                            \
      list_macro(THING_ANIM_JOIN_T270_2, "IS_JOIN_T270_"),        /* newline */                                                            \
      list_macro(THING_ANIM_JOIN_T270_3, "IS_JOIN_T270_"),        /* newline */                                                            \
      list_macro(THING_ANIM_JOIN_T270, "IS_JOIN_T270"),           /* newline */                                                            \
      list_macro(THING_ANIM_JOIN_T90_1, "IS_JOIN_T90_1"),         /* newline */                                                            \
      list_macro(THING_ANIM_JOIN_T90_2, "IS_JOIN_T90_2"),         /* newline */                                                            \
      list_macro(THING_ANIM_JOIN_T90_3, "IS_JOIN_T90_3"),         /* newline */                                                            \
      list_macro(THING_ANIM_JOIN_T90, "IS_JOIN_T90"),             /* newline */                                                            \
      list_macro(THING_ANIM_JOIN_TL, "IS_JOIN_TL"),               /* newline */                                                            \
      list_macro(THING_ANIM_JOIN_TL2, "IS_JOIN_TL2"),             /* newline */                                                            \
      list_macro(THING_ANIM_JOIN_TOP, "IS_JOIN_TOP"),             /* newline */                                                            \
      list_macro(THING_ANIM_JOIN_TR, "IS_JOIN_TR"),               /* newline */                                                            \
      list_macro(THING_ANIM_JOIN_TR2, "IS_JOIN_TR2"),             /* newline */                                                            \
      list_macro(THING_ANIM_JOIN_VERT, "IS_JOIN_VERT"),           /* newline */                                                            \
      list_macro(THING_ANIM_JOIN_VERT2, "IS_JOIN_VERT2"),         /* newline */                                                            \
      list_macro(THING_ANIM_JOIN_X, "IS_JOIN_X"),                 /* newline */                                                            \
      list_macro(THING_ANIM_JOIN_X1_180, "IS_JOIN_X1_18"),        /* newline */                                                            \
      list_macro(THING_ANIM_JOIN_X1_270, "IS_JOIN_X1_27"),        /* newline */                                                            \
      list_macro(THING_ANIM_JOIN_X1_90, "IS_JOIN_X1_90"),         /* newline */                                                            \
      list_macro(THING_ANIM_JOIN_X1, "IS_JOIN_X1"),               /* newline */                                                            \
      list_macro(THING_ANIM_JOIN_X2_180, "IS_JOIN_X2_18"),        /* newline */                                                            \
      list_macro(THING_ANIM_JOIN_X2_270, "IS_JOIN_X2_27"),        /* newline */                                                            \
      list_macro(THING_ANIM_JOIN_X2_90, "IS_JOIN_X2_90"),         /* newline */                                                            \
      list_macro(THING_ANIM_JOIN_X2, "IS_JOIN_X2"),               /* newline */                                                            \
      list_macro(THING_ANIM_JOIN_X3_180, "IS_JOIN_X3_18"),        /* newline */                                                            \
      list_macro(THING_ANIM_JOIN_X3, "IS_JOIN_X3"),               /* newline */                                                            \
      list_macro(THING_ANIM_JOIN_X4_180, "IS_JOIN_X4_18"),        /* newline */                                                            \
      list_macro(THING_ANIM_JOIN_X4_270, "IS_JOIN_X4_27"),        /* newline */                                                            \
      list_macro(THING_ANIM_JOIN_X4_90, "IS_JOIN_X4_90"),         /* newline */                                                            \
      list_macro(THING_ANIM_JOIN_X4, "IS_JOIN_X4"),               /* newline */                                                            \
      list_macro(THING_ANIM_IDLE, "idle"),                        /* newline */                                                            \
      list_macro(THING_ANIM_DEAD, "dead"),                        /* newline */                                                            \
      list_macro(THING_ANIM_OPEN, "open"),                        /* newline */                                                            \
      list_macro(THING_ANIM_BURNT, "burnt"),                      /* newline */                                                            \
      list_macro(THING_ANIM_CURSOR_NORMAL, "cursor-at-normal"),   /* newline */                                                            \
      list_macro(THING_ANIM_CURSOR_NOPATH, "cursor-at-nopath"),   /* newline */                                                            \
      list_macro(THING_ANIM_CURSOR_HAZARD, "cursor-at-hazard"),   /* newline */                                                            \
      list_macro(THING_ANIM_CURSOR_WARNING, "cursor-at-warning"), /* newline */

ENUM_DEF_H(THING_ANIM_ENUM, ThingAnim)

//
// Thing event enum
//
#define THING_EVENT_ENUM(list_macro)                                                                                                       \
  CLANG_FORMAT_INDENT()                                            /* dummy line for clang indentation fixup */                            \
  list_macro(THING_EVENT_CRUSH, "crushing"),                       /* newline */                                                           \
      list_macro(THING_EVENT_FALL, "falling"),                     /* newline */                                                           \
      list_macro(THING_EVENT_FIRE_DAMAGE, "fire"),                 /* newline */                                                           \
      list_macro(THING_EVENT_HEAT_DAMAGE, "heat"),                 /* newline */                                                           \
      list_macro(THING_EVENT_LIFESPAN_EXPIRED, "life"),            /* newline */                                                           \
      list_macro(THING_EVENT_MELEE_DAMAGE, "melee"),               /* newline */                                                           \
      list_macro(THING_EVENT_MELT, "melt"),                        /* newline */                                                           \
      list_macro(THING_EVENT_OPEN, "open"),                        /* newline */                                                           \
      list_macro(THING_EVENT_NONE, "none"),                        /* newline */                                                           \
      list_macro(THING_EVENT_THE_END, "became the new dark lord"), /* newline */                                                           \
      list_macro(THING_EVENT_SHOVED, "shove"),                     /* newline */                                                           \
      list_macro(THING_EVENT_WATER_DAMAGE, "water"),               /* newline */                                                           \
      list_macro(THING_EVENT_EXPLOSION_DAMAGE, "explosion"),       /* newline */                                                           \
      list_macro(THING_EVENT_CARRIED, "carried"),                  /* newline */                                                           \
      list_macro(THING_EVENT_CARRIED_MERGED, "carried-merged"),    /* newline */

ENUM_DEF_H(THING_EVENT_ENUM, ThingEventType)

#define THING_EVENT_ENUM_FIRST ((ThingEventType) 0)

#define FOR_ALL_THING_EVENT(_iter_)                                                                                                        \
  for (ThingEventType _iter_ = THING_EVENT_ENUM_FIRST; /* newline */                                                                       \
       (_iter_) < THING_EVENT_ENUM_MAX;                /* newline */                                                                       \
       (_iter_) = static_cast< ThingEventType >(static_cast< int >(_iter_) + 1))

//
// Thing environ enum
//
#define THING_ENVIRON_ENUM(list_macro)                                                                                                     \
  CLANG_FORMAT_INDENT()                             /* dummy line for clang indentation fixup */                                           \
  list_macro(THING_ENVIRON_HATES, "hates"),         /* newline */                                                                          \
      list_macro(THING_ENVIRON_DISLIKES, "hates"),  /* newline */                                                                          \
      list_macro(THING_ENVIRON_NEUTRAL, "neutral"), /* newline */                                                                          \
      list_macro(THING_ENVIRON_LIKES, "likes"),     /* newline */

ENUM_DEF_H(THING_ENVIRON_ENUM, ThingEnviron)

#define THING_ENVIRON_ENUM_FIRST ((ThingEnviron) 0)

#define FOR_ALL_THING_ENVIRON(_iter_)                                                                                                      \
  for (ThingEnviron _iter_ = THING_ENVIRON_ENUM_FIRST; /* newline */                                                                       \
       (_iter_) < THING_ENVIRON_ENUM_MAX;              /* newline */                                                                       \
       (_iter_) = static_cast< ThingEnviron >(static_cast< int >(_iter_) + 1))

//
// Thing priority enum
//
#define THING_PRIORITY_ENUM(list_macro)                                                                                                    \
  CLANG_FORMAT_INDENT()                                    /* dummy line for clang indentation fixup */                                    \
  list_macro(THING_PRIORITY_NONE, "none"),                 /* newline */                                                                   \
      list_macro(THING_PRIORITY_EXPLOSION, "explosion"),   /* newline */                                                                   \
      list_macro(THING_PRIORITY_PROJECTILE, "projectile"), /* newline */                                                                   \
      list_macro(THING_PRIORITY_LAVA, "lava"),             /* newline */                                                                   \
      list_macro(THING_PRIORITY_FIRE, "fire"),             /* newline */                                                                   \
      list_macro(THING_PRIORITY_STEAM, "steam"),           /* newline */                                                                   \
      list_macro(THING_PRIORITY_PLAYER, "player"),         /* newline */                                                                   \
      list_macro(THING_PRIORITY_MONST, "monst"),           /* newline */                                                                   \
      list_macro(THING_PRIORITY_MOB, "mob"),               /* newline */                                                                   \
      list_macro(THING_PRIORITY_WATER, "water"),           /* newline */                                                                   \
      list_macro(THING_PRIORITY_SMOKE, "smoke"),           /* newline */                                                                   \
      list_macro(THING_PRIORITY_OBJECT, "object"),         /* newline */                                                                   \
      list_macro(THING_PRIORITY_FOLIAGE, "low"),           /* newline */                                                                   \
      list_macro(THING_PRIORITY_DEAD, "dead"),             /* newline */                                                                   \
      list_macro(THING_PRIORITY_BRAZIER, "low"),           /* newline */                                                                   \
      list_macro(THING_PRIORITY_TELEPORT, "teleport"),     /* newline */                                                                   \
      list_macro(THING_PRIORITY_WALL, "wall"),             /* newline */                                                                   \
      list_macro(THING_PRIORITY_LOWEST, "lowest"),         /* newline */

ENUM_DEF_H(THING_PRIORITY_ENUM, ThingPriorityType)

//
// Thing chance enum
//
#define THING_CHANCE_ENUM(list_macro)                                                                                                      \
  CLANG_FORMAT_INDENT()                                           /* dummy line for clang indentation fixup */                             \
  list_macro(THING_CHANCE_START_BURNING, "start-burn"),           /* newline */                                                            \
      list_macro(THING_CHANCE_CONTINUE_TO_BURN, "continue-burn"), /* newline */

ENUM_DEF_H(THING_CHANCE_ENUM, ThingChanceType)

//
// Thing rarity enum
//
#define THING_RARITY_ENUM(list_macro)                                                                                                      \
  CLANG_FORMAT_INDENT()                                /* dummy line for clang indentation fixup */                                        \
  list_macro(THING_RARITY_COMMON, "common"),           /* newline */                                                                       \
      list_macro(THING_RARITY_UNCOMMON, "uncommon"),   /* newline */                                                                       \
      list_macro(THING_RARITY_RARE, "rare"),           /* newline */                                                                       \
      list_macro(THING_RARITY_VERY_RARE, "very_rare"), /* newline */                                                                       \
      list_macro(THING_RARITY_UNIQUE, "unique"),       /* newline */

ENUM_DEF_H(THING_RARITY_ENUM, ThingRarity)

//
// Thing dir enum
//
#define THING_DIR_ENUM(list_macro)                                                                                                         \
  CLANG_FORMAT_INDENT()                               /* dummy line for clang indentation fixup */                                         \
  list_macro(THING_DIR_NONE, "THING_DIR_NONE"),       /* newline */                                                                        \
      list_macro(THING_DIR_DOWN, "THING_DIR_DOWN"),   /* newline */                                                                        \
      list_macro(THING_DIR_UP, "THING_DIR_UP"),       /* newline */                                                                        \
      list_macro(THING_DIR_LEFT, "THING_DIR_LEFT"),   /* newline */                                                                        \
      list_macro(THING_DIR_RIGHT, "THING_DIR_RIGHT"), /* newline */                                                                        \
      list_macro(THING_DIR_TL, "THING_DIR_TL"),       /* newline */                                                                        \
      list_macro(THING_DIR_BL, "THING_DIR_BL"),       /* newline */                                                                        \
      list_macro(THING_DIR_TR, "THING_DIR_TR"),       /* newline */                                                                        \
      list_macro(THING_DIR_BR, "THING_DIR_BR"),       /* newline */

ENUM_DEF_H(THING_DIR_ENUM, ThingDir)

class Tp;

// begin sort marker1 {
[[nodiscard]] auto string2tp(const char **s, int *len = nullptr) -> Tpp;
[[nodiscard]] auto tp_chance_fail(Tpp tp, ThingChanceType val) -> bool;
[[nodiscard]] auto tp_chance_success(Tpp tp, ThingChanceType val) -> bool;
[[nodiscard]] auto tp_chance(Tpp tp, ThingChanceType val) -> int;
[[nodiscard]] auto tp_collision_radius(Tpp t) -> float;
[[nodiscard]] auto tp_damage(Tpp tp, ThingEventType val) -> int;
[[nodiscard]] auto tp_distance_jump_get(Tpp tp) -> int;
[[nodiscard]] auto tp_distance_minion_from_mob_max_get(Tpp tp) -> int;
[[nodiscard]] auto tp_distance_vision_get(Tpp tp) -> int;
[[nodiscard]] auto tp_find_mand(const std::string &val) -> Tpp;
[[nodiscard]] auto tp_find_opt(const std::string &val) -> Tpp;
[[nodiscard]] auto tp_find(TpId id) -> Tpp;
[[nodiscard]] auto tp_first_tile(class Tp *tp, ThingAnim val) -> Tilep;
[[nodiscard]] auto tp_first(ThingFlag f) -> Tpp;
[[nodiscard]] auto tp_health_get(Tpp tp) -> int;
[[nodiscard]] auto tp_health_max_get(Tpp tp) -> int;
[[nodiscard]] auto tp_id_get(Tpp tp) -> TpId;
[[nodiscard]] auto tp_init() -> bool;
[[nodiscard]] auto tp_is_able_to_collect_items(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_able_to_collect_keys(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_able_to_crush_grass(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_able_to_fall_repeatedly(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_able_to_fall_sound(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_able_to_fall(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_able_to_fire_projectiles(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_able_to_jump(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_able_to_lunge(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_able_to_move_diagonally(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_able_to_move_through_walls(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_able_to_open(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_able_to_shove(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_able_to_walk_through_walls(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_animated_can_hflip(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_animated_no_dir(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_animated_sync_first(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_animated(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_attackable_by_monst(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_attackable_by_player(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_barrel(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_biome_bogland(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_biome_dungeon(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_biome_graveyard(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_biome_nethervoid(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_biome_underhell(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_blit_bg(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_blit_centered(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_blit_flush_per_line(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_blit_hit_outline_w_black_inside(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_blit_hit_outline_w_invis_inside(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_blit_if_has_seen(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_blit_obscures(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_blit_on_ground(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_blit_outlined(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_blit_per_pixel_lighting(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_blit_shown_in_chasms(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_blit_shown_in_overlay(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_blit_square_outlined(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_blit_when_obscured_faded(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_blit_when_obscured_outline(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_blitzhound(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_border(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_brazier(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_bridge(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_broken_on_death(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_burnable(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_chasm(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_collectable(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_collision_circle_large(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_collision_circle_small(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_collision_detection_enabled(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_collision_hit_all_on_tile(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_collision_hit_first_on_tile(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_collision_square(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_combustible(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_corpse_on_death(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_corridor(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_crushable(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_cursor_path_hazard(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_cursor_path_none(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_cursor_path_warning(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_cursor_path(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_cursor(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_damage_capped(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_dead_on_collision(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_dead_on_shoving(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_deep_water(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_described_cursor(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_dirt(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_dmap(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_door_locked(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_door_secret(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_door_unlocked(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_dungeon_entrance(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_effect_attack(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_effect_blood(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_effect(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_entrance(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_ethereal(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_exit(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_explosion(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_extinguished_on_death(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_fire(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_fireball(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_flesh(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_floating(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_floor(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_flying(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_foliage(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_gaseous(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_ghost(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_glass(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_gold(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_grass(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_grouped_thing(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_health_visible(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_hit_when_dead(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_immune_to(Tpp tp, ThingEventType val) -> bool;
[[nodiscard]] auto tp_is_indestructible(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_insectoid(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_inventory_item(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_item_droppable(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_item_equipable(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_item_mergeable(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_item(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_key(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_kobalos(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_lava(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_level_closed_icon(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_level_curr(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_level_final_icon(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_level_locked_icon(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_level_next_icon(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_level_open_icon(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_level_select_bg(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_levitating(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_light_source(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_loggable(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_mantisman(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_meltable(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_metal(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_minion(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_mob_kill_minions_on_death(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_mob(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_mob1(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_mob2(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_monst_group_easy(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_monst_group_hard(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_monst_group_mob(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_monst(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_needs_move_confirm(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_obs_to_cursor_path(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_obs_to_explosion(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_obs_to_falling_onto(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_obs_to_fire(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_obs_to_jump_over(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_obs_to_jumping_onto(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_obs_to_jumping_out_of(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_obs_to_movement(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_obs_to_paths(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_obs_to_spawning(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_obs_to_teleporting_onto(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_obs_to_vision(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_obs_when_dead(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_openable(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_physics_explosion(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_physics_temperature(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_physics_water(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_pillar(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_plant(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_player(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_projectile(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_reeds(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_removable_on_err(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_removable_when_dead_on_err(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_rock(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_shovable(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_slime(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_smoke(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_stamina_visible(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_steam(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_stone(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_submergible(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_teleport_blocked(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_teleport(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_tick_end_delay(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_tickable(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_tiled(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_trap(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_treasure(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_undead(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_unused1(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_unused10(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_unused11(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_unused12(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_unused13(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_unused14(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_unused15(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_unused16(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_unused17(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_unused18(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_unused19(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_unused2(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_unused20(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_unused21(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_unused22(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_unused23(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_unused24(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_unused25(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_unused26(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_unused27(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_unused28(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_unused29(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_unused3(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_unused30(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_unused31(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_unused32(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_unused33(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_unused34(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_unused35(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_unused36(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_unused37(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_unused4(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_unused46(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_unused47(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_unused5(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_unused6(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_unused63(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_unused7(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_unused8(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_unused9(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_vault(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_vision_180_degrees(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_vision_360_degrees(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_wait_on_dead_anim(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_wall(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_water(Tpp tp) -> bool;
[[nodiscard]] auto tp_is_wood(Tpp tp) -> bool;
[[nodiscard]] auto tp_lifespan_get(Tpp tp) -> int;
[[nodiscard]] auto tp_lifespan_max_get(Tpp tp) -> int;
[[nodiscard]] auto tp_light_color(Tpp tp) -> color;
[[nodiscard]] auto tp_load(const std::string &val) -> Tpp;
[[nodiscard]] auto tp_minion_max_get(Tpp tp) -> int;
[[nodiscard]] auto tp_name_a_or_an(Tpp tp) -> std::string;
[[nodiscard]] auto tp_name_apostrophize(Tpp tp) -> std::string;
[[nodiscard]] auto tp_name_long(Tpp tp) -> std::string;
[[nodiscard]] auto tp_name_pluralize(Tpp tp) -> std::string;
[[nodiscard]] auto tp_name_real(Tpp tp) -> std::string;
[[nodiscard]] auto tp_name_short(Tpp tp) -> std::string;
[[nodiscard]] auto tp_name(Tpp tp) -> std::string;
[[nodiscard]] auto tp_projectile_max_get(Tpp tp) -> int;
[[nodiscard]] auto tp_random_dungeon_entrance() -> Tpp;
[[nodiscard]] auto tp_random_exit() -> Tpp;
[[nodiscard]] auto tp_random_key() -> Tpp;
[[nodiscard]] auto tp_random_monst(Gamep g, Levelsp v, Levelp l, int c) -> Tpp;
[[nodiscard]] auto tp_random_player() -> Tpp;
[[nodiscard]] auto tp_random_wall() -> Tpp;
[[nodiscard]] auto tp_random(Gamep g, Levelsp v, Levelp l, ThingFlag f) -> Tpp;
[[nodiscard]] auto tp_score_value_get(Tpp tp) -> int;
[[nodiscard]] auto tp_speed_get(Tpp tp) -> int;
[[nodiscard]] auto tp_stamina_get(Tpp tp) -> int;
[[nodiscard]] auto tp_stamina_max_get(Tpp tp) -> int;
[[nodiscard]] auto tp_temperature_burns_at_get(Tpp tp) -> int;
[[nodiscard]] auto tp_temperature_damage_at_get(Tpp tp) -> int;
[[nodiscard]] auto tp_temperature_heat_capacity_get(Tpp tp) -> float;
[[nodiscard]] auto tp_temperature_initial_get(Tpp tp) -> int;
[[nodiscard]] auto tp_temperature_melts_at_get(Tpp tp) -> int;
[[nodiscard]] auto tp_temperature_thermal_conductivity_get(Tpp tp) -> float;
[[nodiscard]] auto tp_tile_name(Tpp tp) -> std::string;
[[nodiscard]] auto tp_tiles_get(Tpp tp, ThingAnim val, int index) -> Tilep;
[[nodiscard]] auto tp_tiles_size(Tpp tp, ThingAnim val) -> int;
[[nodiscard]] auto tp_value1_get(Tpp tp) -> int;
[[nodiscard]] auto tp_value10_get(Tpp tp) -> int;
[[nodiscard]] auto tp_value11_get(Tpp tp) -> int;
[[nodiscard]] auto tp_value12_get(Tpp tp) -> int;
[[nodiscard]] auto tp_value13_get(Tpp tp) -> int;
[[nodiscard]] auto tp_value14_get(Tpp tp) -> int;
[[nodiscard]] auto tp_value15_get(Tpp tp) -> int;
[[nodiscard]] auto tp_value16_get(Tpp tp) -> int;
[[nodiscard]] auto tp_value17_get(Tpp tp) -> int;
[[nodiscard]] auto tp_value18_get(Tpp tp) -> int;
[[nodiscard]] auto tp_value2_get(Tpp tp) -> int;
[[nodiscard]] auto tp_value3_get(Tpp tp) -> int;
[[nodiscard]] auto tp_value4_get(Tpp tp) -> int;
[[nodiscard]] auto tp_value5_get(Tpp tp) -> int;
[[nodiscard]] auto tp_value6_get(Tpp tp) -> int;
[[nodiscard]] auto tp_value7_get(Tpp tp) -> int;
[[nodiscard]] auto tp_value8_get(Tpp tp) -> int;
[[nodiscard]] auto tp_value9_get(Tpp tp) -> int;
[[nodiscard]] auto tp_variant_get(Tpp tp) -> int;
[[nodiscard]] auto tp_variant(ThingFlag f, int /*variant*/) -> Tpp;
[[nodiscard]] auto tp_weight_get(Tpp tp) -> uint32_t;
[[nodiscard]] auto tp_z_depth_get(Tpp tp) -> MapZDepth;
// end sort marker1 }

// begin sort marker2 {
auto tp_priority_get(Tpp tp) -> ThingPriorityType;
void tp_chance_set(Tpp tp, ThingChanceType e, const std::string &val);
void tp_con_(Tpp tp, const char *fmt, va_list args); // compile error without
void tp_con(Tpp tp, const char *fmt, ...) CHECK_FORMAT_STR(printf, 2, 3);
void tp_damage_set(Tpp tp, ThingEventType e, const std::string &val);
void tp_dbg_(Tpp tp, const char *fmt, ...) CHECK_FORMAT_STR(printf, 2, 3);
void tp_die_(Tpp tp, const char *fmt, va_list args); // compile error without
void tp_die(Tpp tp, const char *fmt, ...) CHECK_FORMAT_STR(printf, 2, 3);
void tp_distance_jump_set(Tpp tp, int val);
void tp_distance_minion_from_mob_max_set(Tpp tp, int val);
void tp_distance_vision_set(Tpp tp, int val);
void tp_err_(Tpp tp, const char *fmt, va_list args); // compile error without
void tp_err(Tpp tp, const char *fmt, ...) CHECK_FORMAT_STR(printf, 2, 3);
void tp_fini();
void tp_flag_set(Tpp tp, ThingFlag f, int val = 1);
void tp_get_id(const char *, int *id);
void tp_health_set(Tpp tp, const std::string &val);
void tp_is_immunity_add(Tpp tp, ThingEventType val);
void tp_lifespan_set(Tpp tp, const std::string &val);
void tp_light_color_apply(Tpp tp);
void tp_light_color_set(Tpp tp, const std::string &val);
void tp_log_(Tpp tp, const char *fmt, va_list args); // compile error without
void tp_log(Tpp tp, const char *fmt, ...) CHECK_FORMAT_STR(printf, 2, 3);
void tp_minion_max_set(Tpp tp, int val);
void tp_monst_group_add(Tpp tp, ThingMonstGroup val);
void tp_name_a_or_an_set(Tpp tp, const std::string &val);
void tp_name_apostrophize_set(Tpp tp, const std::string &val);
void tp_name_long_set(Tpp tp, const std::string &val);
void tp_name_pluralize_set(Tpp tp, const std::string &val);
void tp_name_real_set(Tpp tp, const std::string &val);
void tp_name_short_set(Tpp tp, const std::string &val);
void tp_priority_set(Tpp tp, ThingPriorityType val);
void tp_projectile_max_set(Tpp tp, int val);
void tp_score_value_set(Tpp tp, int val);
void tp_speed_set(Tpp tp, int val);
void tp_stamina_set(Tpp tp, const std::string &val);
void tp_temperature_burns_at_set(Tpp tp, int val);
void tp_temperature_damage_at_set(Tpp tp, int val);
void tp_temperature_heat_capacity_set(Tpp tp, float val);
void tp_temperature_initial_set(Tpp tp, int val);
void tp_temperature_melts_at_set(Tpp tp, int val);
void tp_temperature_thermal_conductivity_set(Tpp tp, float val);
void tp_tile_name_set(Tpp tp, const std::string &val);
void tp_tiles_push_back(Tpp tp, ThingAnim val, Tilep tile_p);
void tp_value1_set(Tpp tp, int val);
void tp_value10_set(Tpp tp, int val);
void tp_value11_set(Tpp tp, int val);
void tp_value12_set(Tpp tp, int val);
void tp_value13_set(Tpp tp, int val);
void tp_value14_set(Tpp tp, int val);
void tp_value15_set(Tpp tp, int val);
void tp_value16_set(Tpp tp, int val);
void tp_value17_set(Tpp tp, int val);
void tp_value18_set(Tpp tp, int val);
void tp_value2_set(Tpp tp, int val);
void tp_value3_set(Tpp tp, int val);
void tp_value4_set(Tpp tp, int val);
void tp_value5_set(Tpp tp, int val);
void tp_value6_set(Tpp tp, int val);
void tp_value7_set(Tpp tp, int val);
void tp_value8_set(Tpp tp, int val);
void tp_value9_set(Tpp tp, int val);
void tp_variant_set(Tpp tp, int val);
void tp_weight_set(Tpp tp, uint32_t val);
void tp_z_depth_set(Tpp tp, MapZDepth val);
// end sort marker2 }

//
// Heat capacity indicates the amount of heat energy required to
// change a material's temperature.
//
// Thermal conductivity measures how well a material can conduct heat.
//
// Both properties are essential for understanding heat transfer in materials.
//

//
// Unit is in joules per kelvin per gram
//
#define HEAT_CAPACITY_AIR   1.0f
#define HEAT_CAPACITY_FLESH 3.0f
#define HEAT_CAPACITY_GAS   14.0f
#define HEAT_CAPACITY_GEL   0.02f
#define HEAT_CAPACITY_GLASS 0.8f
#define HEAT_CAPACITY_GOLD  0.1f
#define HEAT_CAPACITY_STEEL 0.5f
#define HEAT_CAPACITY_STONE 0.8f
#define HEAT_CAPACITY_WALL  0.8f
#define HEAT_CAPACITY_WATER 4.0f
#define HEAT_CAPACITY_WOOD  2.0f
#define HEAT_CAPACITY_MAX   15.0f

//
// Unit is weight per mass per kelvin
//
#define THERMAL_CONDUCTIVITY_AIR   0.025f
#define THERMAL_CONDUCTIVITY_FLESH 10.0f
#define THERMAL_CONDUCTIVITY_GAS   0.01f
#define THERMAL_CONDUCTIVITY_GEL   0.2f
#define THERMAL_CONDUCTIVITY_GLASS 1.1f
#define THERMAL_CONDUCTIVITY_STEEL 32.0f
#define THERMAL_CONDUCTIVITY_STONE 70.0f
#define THERMAL_CONDUCTIVITY_WALL  70.0f
#define THERMAL_CONDUCTIVITY_WATER 0.6f
#define THERMAL_CONDUCTIVITY_WOOD  0.1f
#define THERMAL_CONDUCTIVITY_GOLD  310.0f
#define THERMAL_CONDUCTIVITY_HIGH  70.0f

//
// Unit is in grams
//
#define ONE_KG          1000
#define WEIGHT_VVVHEAVY (ONE_KG * 500)
#define WEIGHT_VVHEAVY  (ONE_KG * 200)
#define WEIGHT_VHEAVY   (ONE_KG * 100)
#define WEIGHT_HEAVY    (ONE_KG * 50)
#define WEIGHT_HUMAN    (ONE_KG * 50)
#define WEIGHT_MEDIUM   (ONE_KG * 10)
#define WEIGHT_LIGHT    (ONE_KG * 1)
#define WEIGHT_FEATHER  (1)
#define WEIGHT_NONE     (0)

#endif // MY_THING_TEMPLATE_HPP
