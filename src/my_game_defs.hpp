//
// Copyright goblinhack@gmail.com
//

#ifndef MY_GAME_DEFS_HPP
#define MY_GAME_DEFS_HPP

//
// The level map dimensions
//
enum {
  MAP_HEIGHT = 48,
  MAP_WIDTH  = 48,
  MAP_BORDER = 1,
  MAP_SLOTS  = 14,

  //
  // The world map is smaller than the level map by a factor.
  //
  LEVEL_SCALE = 9,

  //
  // Entropy is always > 0 for Thing IDs to distinguish them
  // A thing ID is composed as: [ Entropy bits ] [ ID bits ]
  //
  //                       31      |       |       |      0
  //                       +-------------------------------
  //                       0LLLLLLIIIIIIIIIIIIIIIEEEEEEEEEE
  //
  // E Entropy
  // I Per level ID (enough for THING_ID_PER_LEVEL_REQ)
  // L Level        (enough for LEVEL_ARR_IDX_MAX - 1)
  //
  THING_LEVEL_ID_BITS           = 7,
  THING_PER_LEVEL_THING_ID_BITS = 15,
  THING_ENTROPY_BITS            = 9,

  //
  // In the world minimap, this is how large each level tile is
  //
  MAP_WORLD_MAP_PIXEL_SIZE_PER_LEVEL = 4,

  MAP_SCROLL_SPEED          = 50,
  MAP_SCROLL_TELEPORT_SPEED = 200,

  //
  // How many on screen tiles we aim to have
  //
  MAP_TILES_ACROSS_DEF = 16,
  MAP_TILES_DOWN_DEF   = 16,

  //
  // Visible map zoom level
  //
  MAP_ZOOM_FULL_MAP = 1,
  MAP_ZOOM_DEF      = 2,
  MAP_ZOOM_MAX      = 16,

  //
  // How long we allow the fall animation
  //
  THING_FALL_ANIM_MS = 250,

  //
  // How fast lights flicker. Does increase processing cost as the number gets smaller.
  //
  THING_LIGHT_FLICKER_ANIM_MS = 10,

  //
  // How fast lava moves
  //
  THING_LAVA_SCROLL_ANIM_MS = 100,

  //
  // If things are too hot, this is the animation pulse time
  //
  THING_IS_HOT_PULSE_ANIM_MS = 1500,

  //
  // If things are hidden, this is the animation pulse time
  //
  THING_IS_HIDDEN_PULSE_ANIM_MS = 1500,

  //
  // If things are hit, this is the animation pulse time
  //
  THING_HIT_FLASH_ANIM_MS = 250,

  //
  // Lunge duration
  //
  THING_LUNGE_ANIM_MS = 150,

  //
  // Don't let corpses pile up as each corpse take up a lot
  //
  THING_CORPSES_PER_TILE = 3,

  //
  // The number of tiles noise can spread at most
  //
  THING_NOISE_MAX = 20,

  //
  // Various sounds
  //
  THING_NOISE_STAFF           = 20,
  THING_NOISE_WAND            = 20,
  THING_NOISE_SHATTER         = 10,
  THING_NOISE_DOOR_SLAM       = 10,
  THING_NOISE_PLAYER_FALL     = 10,
  THING_NOISE_PLAYER_HIT      = 10,
  THING_NOISE_DOOR_SECRET     = 10,
  THING_NOISE_SPLASH          = 10,
  THING_NOISE_PLAYER_JUMP     = 5,
  THING_NOISE_CHEST_OPEN      = 4,
  THING_NOISE_FOLIAGE         = 4,
  THING_NOISE_FOOTSTEP_BRIDGE = 4,
  THING_NOISE_DOOR_OPEN       = 3,
  THING_NOISE_GRASS           = 3,
  THING_NOISE_FOOTSTEP        = 2,

  //
  // Target rolls. Higher targets are harder to exceed.
  //
  TARGET_ROLL                 = 10,
  TARGET_ROLL_TELEPORT_HAZARD = 12,
  TARGET_ROLL_POISONED        = 12,
  TARGET_ROLL_FALL            = 12,
  TARGET_ROLL_CHEST_EXPLODE   = 12,
  TARGET_ROLL_TRAP            = 15,
  TARGET_ROLL_ENGULFED        = 15,
  TARGET_ROLL_ENSNARED        = 19,

  //
  // Jump animation height in tiles
  //
  THING_JUMP_HEIGHT_ANIM_TILES  = 1,
  THING_THROW_HEIGHT_ANIM_TILES = 1,

  //
  // The number of things we can show in the rightbar
  //
  THING_DESCRIBE_MAX = 10,

  //
  // The beam weapon weapon is fixed length due to the animation.
  //
  THING_BEAM_WEAPON_TILES_MAX = 16,

  POPUP_DURATION_MS          = 1000,
  POPUP_DURATION_TILE_HEIGHT = 3,
  POPUP_TEXT_SCALE_ACROSS    = 3,

  //
  // Number of light rays the player casts
  //
  LIGHT_MAX_RAYS_MAX = (360 * 4),

  LEVEL_TEST_MAX = 1000,

  //
  // This is the level selection grid
  //
  LEVEL_GRID_ACROSS = 5,
  LEVEL_GRID_DOWN   = 20,

  //
  // This is the maximum number of levels we need to generate.
  // We can skip some due to gaps due to the boss levels
  //
  BIOME_COUNT       = 5,
  EMPTY_LEVEL_COUNT = ((LEVEL_GRID_ACROSS - 1) * 5),

  //
  // 0 x x x x
  // x x x x x
  // x x x x x
  //    16
  // x x x x x
  // x x x x x
  // x x x x x
  //    32
  // x x x x x
  // x x x x x
  // x x x x x
  //    48
  // x x x x x
  // x x x x x
  // x x x x x
  //    64
  // x x x x x
  // x x x x x
  // x x x x x
  //    80
  //    81 level select
  //    82 player select
  //    83 LEVEL_ARR_IDX_MAX
  //
  LEVEL_ARR_IDX_MAX          = (((LEVEL_GRID_ACROSS * LEVEL_GRID_DOWN) + 3) - EMPTY_LEVEL_COUNT), // 83
  LEVEL_ARR_IDX_LEVEL_SECRET = (LEVEL_ARR_IDX_MAX - 1),                                           // 82
  LEVEL_ARR_IDX_LEVEL_SELECT = (LEVEL_ARR_IDX_MAX - 2),                                           // 81
  LEVEL_ARR_IDX_BOSS_5       = 80,
  LEVEL_ARR_IDX_BOSS_4       = 64,
  LEVEL_ARR_IDX_BOSS_3       = 48,
  LEVEL_ARR_IDX_BOSS_2       = 32,
  LEVEL_ARR_IDX_BOSS_1       = 16,
  LEVEL_ARR_IDX_BOSS_OFFSET  = 2, // Boss levels are in the middle of the grid

  THING_ARR_INDEX_BITS = (THING_LEVEL_ID_BITS + THING_PER_LEVEL_THING_ID_BITS),
  THING_ID_MAX         = (1 << THING_ARR_INDEX_BITS),

  THING_ID_PER_LEVEL_REQ = (MAP_SLOTS * MAP_WIDTH * MAP_HEIGHT),
  THING_ID_PER_LEVEL_MAX = (1 << THING_PER_LEVEL_THING_ID_BITS),

  //
  // Maximum number of things
  //
  TP_ID_MAX = 255,

  LEVEL_ID_REQ = (LEVEL_GRID_ACROSS * LEVEL_GRID_DOWN),
  LEVEL_ID_MAX = (1 << THING_LEVEL_ID_BITS),
};

#define POPUP_TEXT_SCALE_DOWN 1.5
#define TEST_SEED             "test-seed"

enum {
  TEST_ITERATIONS           = 100, // newline
  TICK_DURATION_MS          = 125, // newline
  TICK_FAR_OFF_MONST_CHANCE = 10,  // newline
};

enum {
  WALL_VARIANTS       = 26, // newline
  WALL_TILED_VARIANTS = 24, // newline
  VAULT_VARIANTS      = 1,  // newline
  FLOOR_VARIANTS      = 10  // newline
};

//
// 'x' is the edge of the visible map
// 'o' is the outer scroll barrier that kicks in if the player moved the mouse
//     after the character started to move (and disabled auto scrolling).
//     hitting this will re-enable auto scrolling to attempt to center the
//     player.
// 'i' inner is normally where we try and keep scrolling.
//
// xxxxxxxxxxxxxxxxxxxxxxxxxxxx
// x..........................x
// x.oooooooooooooooooooooooo.x
// x.o......................o.x
// x.o......................o.x
// x.o........iiiiii........o.x
// x.o........i....i........o.x
// x.o........i.@..i........o.x
// x.o........i....i........o.x
// x.o........iiiiii........o.x
// x.o......................o.x
// x.o......................o.x
// x.oooooooooooooooooooooooo.x
// x..........................x
// xxxxxxxxxxxxxxxxxxxxxxxxxxxx
//
#define MAP_SCROLL_EDGE_INNER 0.45f // slower scroll when far from edge
#define MAP_SCROLL_EDGE_OUTER 0.25f // faster scroll as closer to edge

enum {
  MAP_SCROLL_EDGE_INNER_PIXEL = 1, // max pixels to scroll jump
  MAP_SCROLL_EDGE_OUTER_PIXEL = 10 // max pixels to scroll jump
};

//
// Collision detection radii
//
#define THING_COLLISION_CIRCLE_SMALL_RADIUS 0.025f
#define THING_COLLISION_CIRCLE_LARGE_RADIUS 0.5f

//
// Need a small fraction to account for comparisons of very similar floats where
// we end up shooting the player upon firing
//
#define THING_COLLISION_FIRING_OFFSET 0.01f

#endif
