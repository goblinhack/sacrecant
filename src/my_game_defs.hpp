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
  // L Level        (enough for LEVEL_MAX - 1)
  //
  THING_LEVEL_ID_BITS           = 6,
  THING_PER_LEVEL_THING_ID_BITS = 15,
  THING_ENTROPY_BITS            = 10,

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

  THING_FALL_TILE_HEIGHT_MAX = 4,
  THING_FALL_TIME_MS         = 250,
  THING_IS_HOT_PULSE_MS      = 1500,
  THING_IS_HIDDEN_PULSE_MS   = 1500,
  THING_LUNGE_TIME_MS        = 150,
  THING_HIT_FLASH_TIME_MS    = 250,
  THING_CORPSES_PER_TILE     = 3,
  THING_JUMP_HEIGHT_TILES    = 1,

  POPUP_DURATION_MS          = 1000,
  POPUP_DURATION_TILE_HEIGHT = 3,
  POPUP_TEXT_SCALE_ACROSS    = 3,

  //
  // Number of light rays the player casts
  //
  LIGHT_MAX_RAYS_MAX = (360 * 4),

  LEVEL_TEST_MAX  = 1000,
  LEVEL_ACROSS    = (MAP_WIDTH / LEVEL_SCALE),
  LEVEL_DOWN      = (MAP_HEIGHT / LEVEL_SCALE),
  LEVEL_MAX       = ((LEVEL_ACROSS * LEVEL_DOWN) + 1),
  LEVEL_SELECT_ID = (LEVEL_MAX - 1), // The level select level

  THING_ARR_INDEX_BITS = (THING_LEVEL_ID_BITS + THING_PER_LEVEL_THING_ID_BITS),
  THING_ID_MAX         = (1 << THING_ARR_INDEX_BITS),

  THING_ID_PER_LEVEL_REQ = (MAP_SLOTS * MAP_WIDTH * MAP_HEIGHT),
  THING_ID_PER_LEVEL_MAX = (1 << THING_PER_LEVEL_THING_ID_BITS),

  LEVEL_ID_REQ = (LEVEL_ACROSS * LEVEL_DOWN),
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
  WALL_VARIANTS  = 12, // newline
  FLOOR_VARIANTS = 10  // newline
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
  MAP_SCROLL_EDGE_OUTER_PIXEL = 5  // max pixels to scroll jump
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
