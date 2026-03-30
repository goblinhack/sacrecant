//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_cave.hpp"
#include "my_charmap.hpp"
#include "my_dice_rolls.hpp"
#include "my_game.hpp"
#include "my_globals.hpp"
#include "my_level.hpp"
#include "my_level_ext.hpp"
#include "my_level_inlines.hpp"
#include "my_main.hpp"
#include "my_random.hpp"
#include "my_sprintf.hpp"
#include "my_thing.hpp"
#include "my_thing_inlines.hpp"
#include "my_time.hpp"

#include <array>
#include <functional>
#include <map>
#include <mutex>
#include <print>
#include <thread>
#include <unordered_map>
#include <utility>
#include <vector>

static std::mutex level_gen_mutex;

//
// How many times to try creating a single level
//
static const int MAX_LEVEL_GEN_TRIES_FOR_SAME_SEED = 1000000;

//
// How many times to try adding a room
//
static const int MAX_LEVEL_GEN_TRIES_CREATE_ROOM = 1000;

//
// How many times to try adding the first room
//
static const int MAX_LEVEL_GEN_TRIES_CREATE_FIRST_ROOM = 200;

//
// How many times to try placing a random room next to a door
//
static const int MAX_LEVEL_GEN_ROOM_PLACE_TRIES = 200;

//
// After this length, corridors become bridges
//
static const int MAX_LEVEL_GEN_MIN_BRIDGE_LEN        = 3;
static const int MAX_LEVEL_GEN_MIN_BRIDGE_LEN_CHANCE = 30;

//
// Per level minimums
//
static const int MAX_LEVEL_GEN_MIN_TREASURE_PER_LEVEL = 6;
static const int MAX_LEVEL_GEN_MIN_MONST_PER_LEVEL    = 20;
static const int MAX_LEVEL_GEN_MIN_MONST_PLACE_TRY    = 100;

//
// How many times to try to replace part of the dungeon
//
static const int MAX_LEVEL_GEN_FRAGMENT_TRIES = 100;

//
// When adding missing teleports
//
static const int MAX_LEVEL_GEN_PLACE_ADDITIONAL_TELEPORT_TRIES = 100;

//
// Secret rooms need keys
//
static const int MAX_LEVEL_GEN_PLACE_MISSING_KEY_TRIES = 1000;

//
// The max amount of fragments to create
//
static const int MAX_LEVEL_GEN_FRAGMENTS = 20;

//
// How far away the start and exit should be at a minimum
//
static const int MIN_LEVEL_EXIT_DISTANCE = MAP_WIDTH / 4;

//
// How many rooms qualify as a dungeon. This increases with depth.
//
static const int MIN_LEVEL_ROOM_COUNT = 15;

//
// Cellular auto fill prob
//
static const int LEVEL_BLOB_GEN_FILL_PROB = 1200;

//
// Chance of creating a pool on a level
//
static const int LEVEL_BLOB_GEN_PROB = 80;

//
// Skip tiles near the edges
//
static const int LEVEL_GEN_BORDER_FOR_ITEM_PLACEMENT = 4;

//
// Chance of creating some doors for a room. We always add some doors, and then a chance of more.
//
static const int MAX_LEVEL_GEN_LOOK_FOR_ROOM_TRIES                 = 100;
static const int MAX_LEVEL_GEN_ADD_ROOM_WITH_DOOR_TRIES            = 1;
static const int MAX_LEVEL_GEN_ADD_ADDITIONAL_ROOM_WITH_DOOR_TRIES = 10;

//
// Chance of obscuring things
//
static const int LEVEL_GEN_CHANCE_OF_FOLIAGE_AROUND_TELEPORTS   = 50;
static const int LEVEL_GEN_CHANCE_OF_FOLIAGE_AROUND_ITEMS       = 20;
static const int LEVEL_GEN_CHANCE_OF_FOLIAGE_AROUND_TRAPS       = 75;
static const int LEVEL_GEN_CHANCE_OF_FOLIAGE_AROUND_KEYS        = 100;
static const int LEVEL_GEN_CHANCE_OF_FOLIAGE_AROUND_CHASMS      = 30;
static const int LEVEL_GEN_CHANCE_OF_FOLIAGE_AROUND_DOOR_LOCKED = 50;
static const int LEVEL_GEN_CHANCE_OF_FOLIAGE_AROUND_DOOR_SECRET = 80;
static const int LEVEL_GEN_CHANCE_OF_DOOR_LOCKED                = 30;
static const int LEVEL_GEN_CHANCE_OF_CENTRAL_BRIDGE             = 50;

//
// Chances of creating, tested in the following order, with the default being water
//
static const int LEVEL_BLOB_LAVA_GEN_PROB  = 0;
static const int LEVEL_BLOB_CHASM_GEN_PROB = 50;

class Cell;
class Room;
class LevelFixed;
class LevelGen;

//
// Statistics
//
static int level_create_fail;
static int level_place_first_room_fail;
static int level_place_subsequent_room_fail;
static int level_tried_to_place_existing_room_fail;
static int level_find_door_fail_count;
static int level_not_enough_rooms;
static int level_no_exit_room;

//
// Fixed or proc gen levels
//
static std::array< class LevelGen *, LEVEL_MAX > levels_generated = {};

static void level_gen_dump(class LevelGen *l, const char *msg = nullptr);

class Cell
{
private:
public:
  Cell()  = default;
  ~Cell() = default;

  //
  // Room char
  //
  char c = CHARMAP_EMPTY;

  //
  // The room this cell was sourced from
  //
  class Room *room = {};
};

class LevelGen
{
private:
public:
  LevelGen() { info = {}; }
  ~LevelGen() = default;

  //
  // Dump levels as generating them, enable via -debug
  //
  bool debug {};

  //
  // Level gen stats
  //
  LevelInfo info {};

  //
  // Level number
  //
  LevelNum level_num {};

  //
  // The number of rooms to aim for for a given depth.
  //
  int min_room_count {};
  int max_room_count {};

  //
  // The starting room
  //
  class Room *room_entrance = {};

  //
  // The exitting room
  //
  class Room *room_exit = {};

  //
  // Used to make sure we place each room uniquely per level
  //
  std::unordered_map< class Room *, bool > rooms_placed;

  //
  // Which doors we expanded off of.
  //
  std::map< bpoint, bool > doors_walked;

  //
  // All level doors.
  //
  std::vector< bpoint > doors_not_explored;

  //
  // Rooms that can reach each other via a door.
  //
  std::map< std::pair< class Room *, class Room * >, bool > rooms_connected;

  //
  // Additional adjacent rooms connected from the first pass
  //
  int rooms_adj_connected = {};

  //
  // Only one exit per level
  //
  bool has_placed_ROOM_TYPE_EXIT = {};

  //
  // We've placed a secret room and need a key room
  //
  bool has_placed_ROOM_TYPE_DOOR_LOCKED = {};
  bool has_placed_ROOM_TYPE_HAS_KEY     = {};

  //
  // Temporary for flood fill
  //
  bool walked[ MAP_WIDTH ][ MAP_HEIGHT ] = {};

  //
  // Level tiles and room info
  //
  Cell data[ MAP_WIDTH ][ MAP_HEIGHT ];

  Cave cave = {};
};

using RoomType = enum RoomType_ {
  ROOM_TYPE_ENTRANCE,
#define ROOM_TYPE_FIRST ROOM_TYPE_ENTRANCE
  ROOM_TYPE_NORMAL,
  ROOM_TYPE_EXIT,
  ROOM_TYPE_LOCKED,
  ROOM_TYPE_HAS_KEY,
  ROOM_TYPE_MAX
};

class Room
{
private:
public:
  Room() = default;
  ~Room() { MYFREE(data); }

  //
  // Globally unique
  //
  int id = {};

  //
  // Chance of appearing
  //
  int chance = {CHANCE_NORMAL};

  //
  // Start room?
  //
  RoomType room_type;

  //
  // Room size
  //
  int width  = {};
  int height = {};

  //
  // e.g. ROOM_FLAG_CHECK_EXITS
  //
  uint32_t flags = {};

  //
  // Room data
  //
  char *data = {};

  //
  // For keeping track of source location
  //
  const char *file = {};
  int         line = {};

  //
  // All doors in the room
  //
  // These are room co-ordinates, not map co-ordinates
  //
  std::vector< bpoint > doors;

  //
  // Points adjacent to doors
  //
  // These are room co-ordinates, not map co-ordinates
  //
  std::vector< bpoint > door_adjacent_tile;
};

static int                         room_no;
static std::vector< class Room * > room_all[ ROOM_TYPE_MAX ];

class LevelFixed
{
private:
public:
  LevelFixed() = default;
  ~LevelFixed() { MYFREE(data); }

  //
  // Globally unique
  //
  int id = {};

  //
  // Chance of appearing
  //
  int chance = {CHANCE_NORMAL};

  //
  // Level size before creation
  //
  int width  = {};
  int height = {};

  //
  // Boss level etc...
  //
  LevelType level_type {LEVEL_TYPE_NORMAL};

  //
  // Level data
  //
  char *data = {};

  //
  // For keeping track of source location
  //
  const char *file = {};
  int         line = {};

  Overrides overrides;

  //
  // User alias for level. Must be unique.
  //
  std::string alias;
};

static int                                                   level_fixed_no;
static std::vector< class LevelFixed * >                     level_fixed_all[ LEVEL_TYPE_MAX ];
static std::unordered_map< std::string, class LevelFixed * > level_alias_all;

class FragmentAlt
{
private:
public:
  FragmentAlt() = default;
  ~FragmentAlt() { MYFREE(data); }

  //
  // Globally unique
  //
  int id = {};

  //
  // Chance of appearing
  //
  int chance = {CHANCE_NORMAL};

  //
  // Used to filter the room types this fragment can be appled to
  //
  uint32_t room_flags = {NO_FLAGS};

  //
  // FragmentAlt size
  //
  int width  = {};
  int height = {};

  //
  // FragmentAlt data
  //
  char *data = {};

  //
  // For keeping track of source location
  //
  const char *file = {};
  int         line = {};
};

static int                                fragment_alt_no;
static std::vector< class FragmentAlt * > fragment_alts_all;
static std::vector< class FragmentAlt * > fragment_alts_curr;

class Fragment
{
private:
public:
  Fragment() = default;
  ~Fragment() { MYFREE(data); }

  //
  // Globally unique
  //
  int id = {};

  //
  // Chance of appearing
  //
  int chance = {CHANCE_NORMAL};

  //
  // Fragment size
  //
  int width  = {};
  int height = {};

  //
  // Fragment data
  //
  char *data = {};

  //
  // For keeping track of source location
  //
  const char *file = {};
  int         line = {};

  //
  // Alternatives for this fragment
  //
  std::vector< class FragmentAlt * > fragment_alts;
};

static int                             fragment_no;
static std::vector< class Fragment * > fragments_all;
static std::vector< class Fragment * > fragments_curr;

//
// Read a room char
//
static auto room_char(class Room *r, int x, int y) -> char
{
  if ((x < 0)) [[unlikely]] {
    return CHARMAP_EMPTY;
  }
  if ((y < 0)) [[unlikely]] {
    return CHARMAP_EMPTY;
  }
  if ((x >= r->width)) [[unlikely]] {
    return CHARMAP_EMPTY;
  }
  if ((y >= r->height)) [[unlikely]] {
    return CHARMAP_EMPTY;
  }

  return r->data[ (y * r->width) + x ];
}

//
// Scan the room now it is created, for any things of interest
//
static void room_scan(Gamep g, class Room *r)
{
  TRACE();

  for (int y = 0; y < r->height; y++) {
    for (int x = 0; x < r->width; x++) {
      auto c = room_char(r, x, y);

      if (c == CHARMAP_JOIN) {
        bpoint const p(x, y);
        r->doors.push_back(p);

        //
        // Look for tiles next to the door. We will use these to check if room placement works
        //
        if (room_char(r, x - 1, y) != CHARMAP_EMPTY) {
          r->door_adjacent_tile.push_back(bpoint(x - 1, y));
        }
        if (room_char(r, x + 1, y) != CHARMAP_EMPTY) {
          r->door_adjacent_tile.push_back(bpoint(x + 1, y));
        }
        if (room_char(r, x, y - 1) != CHARMAP_EMPTY) {
          r->door_adjacent_tile.push_back(bpoint(x, y - 1));
        }
        if (room_char(r, x, y + 1) != CHARMAP_EMPTY) {
          r->door_adjacent_tile.push_back(bpoint(x, y + 1));
        }
      }
    }
  }
}

//
// Rotate the current room clockwise and put that into a new room
//
static auto room_rotate(Gamep g, class Room *r) -> class Room *
{
  TRACE();

  auto *n = new Room();
  room_all[ r->room_type ].push_back(n);

  n->id        = room_no++;
  n->chance    = r->chance;
  n->room_type = r->room_type;
  n->file      = r->file;
  n->line      = r->line;
  n->width     = r->height;
  n->height    = r->width;
  n->flags     = r->flags;
  n->data      = static_cast< char * >(MYZALLOC(r->width * r->height, "room data"));

  for (int y = 0; y < r->height; y++) {
    for (int x = 0; x < r->width; x++) {
      int const nx                    = n->width - y - 1;
      int const ny                    = x;
      n->data[ (ny * n->width) + nx ] = r->data[ (y * r->width) + x ];
    }
  }

  room_scan(g, n);

  return n;
}

//
// Flip the current room horizontally and put that into a new room
//
static auto room_flip_horiz(Gamep g, class Room *r) -> class Room *
{
  TRACE();

  auto *n = new Room();
  room_all[ r->room_type ].push_back(n);

  n->id        = room_no++;
  n->chance    = r->chance;
  n->room_type = r->room_type;
  n->file      = r->file;
  n->line      = r->line;
  n->width     = r->width;
  n->height    = r->height;
  n->flags     = r->flags;
  n->data      = static_cast< char * >(MYZALLOC(r->width * r->height, "room data"));

  for (int y = 0; y < r->height; y++) {
    for (int x = 0; x < r->width; x++) {
      int const nx                    = r->width - x - 1;
      int const ny                    = y;
      n->data[ (ny * r->width) + nx ] = r->data[ (y * r->width) + x ];
    }
  }

  room_scan(g, n);

  return n;
}

//
// Add a room and copies with all possible rotations
//
void room_add(Gamep g, int chance, int room_flags, const char *file, int line, ...)
{
  TRACE();

  va_list ap = {};

  int room_width  = 0;
  int room_height = 0;

  //
  // Ensure not too many of certain tiles
  //
  int exit_count     = 0;
  int entrance_count = 0;

  //
  // We scan the room to work out the room type
  //
  RoomType room_type = ROOM_TYPE_NORMAL;

  //
  // First scan, get width and height
  //
  va_start(ap, line);

  for (;;) {
    const char *room_line = va_arg(ap, const char *);
    if (room_line == nullptr) {
      break;
    }

    int const this_line_width = static_cast< int >(strlen(room_line));

    if (room_width == 0) {
      //
      // Get the room width
      //
      room_width = this_line_width;
    } else {
      //
      // Check width is constant
      //
      if (room_width != this_line_width) {
        CROAK("room has inconsistent width in room @ %s:%d", file, line);
        return;
      }
    }

    //
    // Check the room contents are known characters
    //
    for (auto i = 0; i < room_width; i++) {
      switch (room_line[ i ]) {
        case CHARMAP_BARREL :        break;
        case CHARMAP_BRAZIER :       break;
        case CHARMAP_BRIDGE :        break;
        case CHARMAP_CHASM :         break;
        case CHARMAP_CHASM_50 :      break;
        case CHARMAP_CORRIDOR :      break;
        case CHARMAP_DEEP_WATER :    break;
        case CHARMAP_DOOR_LOCKED :   break;
        case CHARMAP_DOOR_SECRET :   break;
        case CHARMAP_DOOR_UNLOCKED : break;
        case CHARMAP_EMPTY :         break;
        case CHARMAP_FLOOR :         break;
        case CHARMAP_FLOOR_50 :      break;
        case CHARMAP_FOLIAGE :       break;
        case CHARMAP_GRASS :         break;
        case CHARMAP_JOIN :          break;
        case CHARMAP_KEY :           break;
        case CHARMAP_LAVA :          break;
        case CHARMAP_MOB1 :          break;
        case CHARMAP_MOB2 :          break;
        case CHARMAP_MONST1 :        break;
        case CHARMAP_MONST2 :        break;
        case CHARMAP_PILLAR :        break;
        case CHARMAP_TELEPORT :      break;
        case CHARMAP_TRAP :          break;
        case CHARMAP_TREASURE :      break;
        case CHARMAP_WALL :          break;
        case CHARMAP_WATER :         break;
        case CHARMAP_FIRE :          break;
        case CHARMAP_EXIT :
          if ((exit_count++) != 0) {
            CROAK("room has too many exits in room @ %s:%d", file, line);
            return;
          }
          room_type = ROOM_TYPE_EXIT;
          break;
        case CHARMAP_ENTRANCE :
          if ((entrance_count++) != 0) {
            CROAK("room has too many entrances in room @ %s:%d", file, line);
            return;
          }
          room_type = ROOM_TYPE_ENTRANCE;
          break;
        default : CROAK("room has unknown char [%c] in room @ %s:%d", room_line[ i ], file, line); return;
      }
    }

    room_height++;
  }

  va_end(ap);

  //
  // Special room types
  //
  if ((room_flags & ROOM_FLAG_LOCKED) != 0) {
    room_type = ROOM_TYPE_LOCKED;
  }

  if ((room_flags & ROOM_FLAG_HAS_KEY) != 0) {
    room_type = ROOM_TYPE_HAS_KEY;
  }

  //
  // Check the room sizes
  //
  if (room_width == 0) {
    CROAK("room has no width in room @ %s:%d", file, line);
    return;
  }
  if (room_height == 0) {
    CROAK("room has no height in room @ %s:%d", file, line);
    return;
  }
  if (room_width > MAP_WIDTH) {
    CROAK("room is too wide in room @ %s:%d", file, line);
    return;
  }
  if (room_height > MAP_HEIGHT) {
    CROAK("room is too tall in room @ %s:%d", file, line);
    return;
  }

  //
  // Allocate space for the room
  //
  auto *r = new Room();
  room_all[ room_type ].push_back(r);

  r->id        = room_no++;
  r->chance    = chance;
  r->room_type = room_type;
  r->file      = file;
  r->line      = line;
  r->width     = room_width;
  r->height    = room_height;
  r->flags     = room_flags;
  r->data      = static_cast< char * >(MYZALLOC(room_width * room_height, "room data"));

  //
  // Keep track of exits so we can sanity check the edges of the room
  //
  bool has_exit_up {};
  bool has_exit_down {};
  bool has_exit_left {};
  bool has_exit_right {};

  //
  // Now read the room again
  //
  va_start(ap, line);

  for (int y = 0; y < r->height; y++) {
    const char *room_line = va_arg(ap, char *);
    for (int x = 0; x < r->width; x++) {
      auto c = room_line[ x ];

      //
      // Keep track of exits
      //
      if (c == CHARMAP_JOIN) {
        if (y == 0) {
          has_exit_up = true;
        }
        if (y == r->height - 1) {
          has_exit_down = true;
        }
        if (x == 0) {
          has_exit_left = true;
        }
        if (x == r->width - 1) {
          has_exit_right = true;
        }
      }

      r->data[ (y * r->width) + x ] = room_line[ x ];
    }
  }

  va_end(ap);

  if (! has_exit_up && ! has_exit_down && ! has_exit_left && ! has_exit_right) {
    CROAK("room has no exits @ %s:%d", file, line);
    return;
  }

  //
  // Sanity check on exits that we have no tiles in the same column or row
  // as an exit; it makes it harder to join rooms together
  //
  if ((room_flags & ROOM_FLAG_CHECK_EXITS) != 0) {
    for (int y = 0; y < r->height; y++) {
      for (int x = 0; x < r->width; x++) {
        auto c = r->data[ (y * r->width) + x ];
        if ((c != CHARMAP_EMPTY) && (c != CHARMAP_JOIN)) {

          //
          // Exclude corners
          //
          if ((x == 0) && (y == 0)) {
            continue;
          }
          if ((x == 0) && (y == r->height - 1)) {
            continue;
          }
          if ((x == r->width - 1) && (y == 0)) {
            continue;
          }
          if ((x == r->width - 1) && (y == r->height - 1)) {
            continue;
          }

          if ((y == 0) && has_exit_up) {
            CROAK("room has exit up and tiles in same row @ %s:%d", file, line);
            return;
          }
          if ((y == r->height - 1) && has_exit_down) {
            CROAK("room has exit down and tiles in same row @ %s:%d", file, line);
            return;
          }
          if ((x == 0) && has_exit_left) {
            CROAK("room has exit left and tiles in same column @ %s:%d", file, line);
            return;
          }
          if ((x == r->width - 1) && has_exit_right) {
            CROAK("room has exit right and tiles in same column @ %s:%d", file, line);
            return;
          }
        }
      }
    }
  }

  //
  // Make alternate rooms
  //
  room_rotate(g, room_rotate(g, room_rotate(g, r)));
  room_rotate(g, room_rotate(g, room_rotate(g, room_flip_horiz(g, r))));

  room_scan(g, r);
}

//
// Get a random room.
//
static auto room_random_get(RoomType room_type) -> class Room *
{
  TRACE();

  if (room_all[ room_type ].empty()) {
    CROAK("no rooms of type %d", room_type);
  }

  return room_all[ room_type ][ PCG_RANDOM_RANGE(0, room_all[ room_type ].size()) ];
}

//
// Dump a room
//
static void room_dump(class Room *r)
{
  TRACE();

  level_gen_mutex.lock();
  LOG("room %d @ %s:%d", r->id, r->file, r->line);
  TRACE_INDENT();

  for (int y = 0; y < r->height; y++) {
    std::string tmp;
    for (int x = 0; x < r->width; x++) {
      tmp += r->data[ (y * r->width) + x ];
    }
    LOG("[%s]", tmp.c_str());
  }

  LOG("-");
  level_gen_mutex.unlock();
}

//
// Dump all rooms
//
// NOLINTBEGIN
void rooms_dump(Gamep g)
// NOLINTEND
{
  TRACE();

  for (auto room_type = static_cast< int >(ROOM_TYPE_FIRST); room_type < static_cast< int >(ROOM_TYPE_MAX); room_type++) {
    for (auto *r : room_all[ room_type ]) {
      room_dump(r);
    }
  }
}

//
// Can we place a room here on the level?
//
[[nodiscard]] static auto room_can_place_at(class LevelGen *l, class Room *r, bpoint at, int rx, int ry) -> bool
{
  //
  // Check we have something to place here.
  //
  char const room_c = r->data[ (ry * r->width) + rx ];
  if ((room_c == CHARMAP_EMPTY)) [[unlikely]] {
    return true;
  }

  //
  // Where we're placing tiles
  //
  bpoint const p(rx + at.x, ry + at.y);

  //
  // We need one tile of edge around rooms.
  //
  if ((p.x <= 0)) [[unlikely]] {
    return false;
  }
  if ((p.x >= MAP_WIDTH - 1)) [[unlikely]] {
    return false;
  }
  if ((p.y <= 0)) [[unlikely]] {
    return false;
  }
  if ((p.y >= MAP_HEIGHT - 1)) [[unlikely]] {
    return false;
  }

  //
  // Special door handling
  //
  if ((room_c == CHARMAP_JOIN)) [[unlikely]] {
    //
    // Doors can overlap.
    //
    if (l->data[ p.x ][ p.y ].c == CHARMAP_JOIN) {
      return true;
    }
  }

  //
  // Check all adjacent tiles for an adjacent room
  //
  for (int dy = -1; dy <= 1; dy++) {
    for (int dx = -1; dx <= 1; dx++) {
      if ((dx == 0) && (dy == 0)) {
        switch (l->data[ p.x + dx ][ p.y + dy ].c) {
          case CHARMAP_JOIN :
          case CHARMAP_EMPTY : break;
          default :            return false;
        }
      } else {
        //
        // Allow certain tiles, like water to be adjacent. This way a room can be created
        // right at the water edge.
        //
        switch (l->data[ p.x + dx ][ p.y + dy ].c) {
          case CHARMAP_WATER :
          case CHARMAP_DEEP_WATER :
          case CHARMAP_LAVA :
          case CHARMAP_CHASM :
          case CHARMAP_JOIN :
          case CHARMAP_EMPTY :      break;
          default :                 return false;
        }
      }
    }
  }

  //
  // Can place here
  //
  return true;
}

//
// Can we place a room here on the level?
//
[[nodiscard]] static auto room_can_place_at(Gamep g, class LevelGen *l, class Room *r, bpoint at) -> bool
{
  //
  // Optimization, check edge tiles first
  //
  for (auto p : r->door_adjacent_tile) {
    if (! room_can_place_at(l, r, at, p.x, p.y)) {
      return false;
    }
  }

  //
  // Check the room is clear to be placed here.
  //
  // Also check 1 adjoining tile
  //
  for (int ry = 0; ry < r->height; ry++) {
    for (int rx = 0; rx < r->width; rx++) {
      if (! room_can_place_at(l, r, at, rx, ry)) {
        return false;
      }
    }
  }

  //
  // Can place here
  //
  return true;
}

//
// Place a room on the level
//
static void room_place_at(class LevelGen *l, class Room *r, bpoint at)
{
  //
  // The room should be clear to place at this point
  //
  for (int ry = 0; ry < r->height; ry++) {
    for (int rx = 0; rx < r->width; rx++) {
      //
      // Check we have something to place here.
      //
      char room_c = r->data[ (ry * r->width) + rx ];
      if (room_c == CHARMAP_EMPTY) {
        continue;
      }

      //
      // Check if we need to modify tiles when placing
      //
      switch (room_c) {
        case CHARMAP_CHASM_50 :
          if (d100() > 50) {
            room_c = CHARMAP_FLOOR;
          } else {
            room_c = CHARMAP_CHASM;
          }
          break;
        case CHARMAP_FLOOR_50 :
          if (d100() > 50) {
            room_c = CHARMAP_EMPTY;
          } else {
            room_c = CHARMAP_FLOOR;
          }
          break;
        default : break;
      }

      if (room_c == CHARMAP_EMPTY) {
        continue;
      }

      //
      // Where we're placing tiles
      //
      bpoint const p(rx + at.x, ry + at.y);

      if (room_c == CHARMAP_ENTRANCE) {
        l->info.entrance_at.x = p.x;
        l->info.entrance_at.y = p.y;
      }
      if (room_c == CHARMAP_EXIT) {
        l->info.exit_at.x = p.x;
        l->info.exit_at.y = p.y;
      }

      class Cell *cell = &l->data[ p.x ][ p.y ];
      cell->c          = room_c;
      cell->room       = r;
    }
  }

  l->rooms_placed[ r ] = true;
  l->info.room_count   = static_cast< int >(l->rooms_placed.size());
}

//
// Clean up rooms
//
void rooms_fini(Gamep g)
{
  TRACE();

  for (auto room_type = static_cast< int >(ROOM_TYPE_FIRST); room_type < static_cast< int >(ROOM_TYPE_MAX); room_type++) {
    for (auto *r : room_all[ room_type ]) {
      delete r;
    }
  }
}

//
// Read a fragment_alt char
//
auto fragment_alt_char(class FragmentAlt *r, int x, int y) -> char
{
  if (x < 0) {
    return CHARMAP_EMPTY;
  }
  if (y < 0) {
    return CHARMAP_EMPTY;
  }
  if (x >= r->width) {
    return CHARMAP_EMPTY;
  }
  if (y >= r->height) {
    return CHARMAP_EMPTY;
  }

  return r->data[ (y * r->width) + x ];
}

//
// Rotate the current fragment_alt clockwise and put that into a new fragment_alt
//
static auto fragment_alt_rotate(class FragmentAlt *r) -> class FragmentAlt *
{
  TRACE();

  auto *n = new FragmentAlt();
  fragment_alts_all.push_back(n);
  fragment_alts_curr.push_back(n);

  n->id         = fragment_no++;
  n->chance     = r->chance;
  n->room_flags = r->room_flags;
  n->file       = r->file;
  n->line       = r->line;
  n->width      = r->height;
  n->height     = r->width;
  n->data       = static_cast< char * >(MYZALLOC(r->width * r->height, "fragment_alt data"));

  for (int y = 0; y < r->height; y++) {
    for (int x = 0; x < r->width; x++) {
      int const nx                    = n->width - y - 1;
      int const ny                    = x;
      n->data[ (ny * n->width) + nx ] = r->data[ (y * r->width) + x ];
    }
  }

  return n;
}

//
// Flip the current fragment_alt horizontally and put that into a new fragment_alt
//
static auto fragment_alt_flip_horiz(class FragmentAlt *r) -> class FragmentAlt *
{
  TRACE();

  auto *n = new FragmentAlt();
  fragment_alts_all.push_back(n);
  fragment_alts_curr.push_back(n);

  n->id         = fragment_alt_no++;
  n->chance     = r->chance;
  n->room_flags = r->room_flags;
  n->file       = r->file;
  n->line       = r->line;
  n->width      = r->width;
  n->height     = r->height;
  n->data       = static_cast< char * >(MYZALLOC(r->width * r->height, "fragment_alt data"));

  for (int y = 0; y < r->height; y++) {
    for (int x = 0; x < r->width; x++) {
      int const nx                    = r->width - x - 1;
      int const ny                    = y;
      n->data[ (ny * r->width) + nx ] = r->data[ (y * r->width) + x ];
    }
  }

  return n;
}

//
// Add a fragment_alt and copies with all possible rotations
//
auto fragment_alt_add(Gamep g, int chance, uint32_t room_flags, const char *file, int line, ...) -> bool
{
  TRACE();

  va_list ap                  = {};
  int     fragment_alt_width  = 0;
  int     fragment_alt_height = 0;

  //
  // First scan, get width and height
  //
  va_start(ap, line);

  for (;;) {
    const char *fragment_alt_line = va_arg(ap, const char *);
    if (fragment_alt_line == nullptr) {
      break;
    }

    int const this_line_width = static_cast< int >(strlen(fragment_alt_line));

    if (fragment_alt_width == 0) {
      //
      // Get the fragment_alt width
      //
      fragment_alt_width = this_line_width;
    } else {
      //
      // Check width is constant
      //
      if (fragment_alt_width != this_line_width) {
        CROAK("fragment_alt has inconsistent width in fragment_alt @ %s:%d", file, line);
        return false;
      }
    }

    //
    // Check the fragment_alt contents are known characters
    //
    for (auto i = 0; i < fragment_alt_width; i++) {
      switch (fragment_alt_line[ i ]) {
        case CHARMAP_BARREL :        break;
        case CHARMAP_BRAZIER :       break;
        case CHARMAP_BRIDGE :        break;
        case CHARMAP_CHASM :         break;
        case CHARMAP_CHASM_50 :      break;
        case CHARMAP_CORRIDOR :      break;
        case CHARMAP_DEEP_WATER :    break;
        case CHARMAP_DOOR_LOCKED :   break;
        case CHARMAP_DOOR_SECRET :   break;
        case CHARMAP_DOOR_UNLOCKED : break;
        case CHARMAP_EMPTY :         break;
        case CHARMAP_ENTRANCE :      break;
        case CHARMAP_EXIT :          break;
        case CHARMAP_FLOOR :         break;
        case CHARMAP_FLOOR_50 :      break;
        case CHARMAP_FOLIAGE :       break;
        case CHARMAP_GRASS :         break;
        case CHARMAP_JOIN :          break;
        case CHARMAP_KEY :           break;
        case CHARMAP_LAVA :          break;
        case CHARMAP_MOB1 :          break;
        case CHARMAP_MOB2 :          break;
        case CHARMAP_MONST1 :        break;
        case CHARMAP_MONST2 :        break;
        case CHARMAP_PILLAR :        break;
        case CHARMAP_TELEPORT :      break;
        case CHARMAP_TRAP :          break;
        case CHARMAP_TREASURE :      break;
        case CHARMAP_WALL :          break;
        case CHARMAP_WATER :         break;
        case CHARMAP_FIRE :          break;
        default :                    CROAK("fragment_alt has unknown char [%c] in fragment_alt @ %s:%d", fragment_alt_line[ i ], file, line); return false;
      }
    }

    fragment_alt_height++;
  }

  va_end(ap);

  //
  // Check the fragment_alt sizes
  //
  if (fragment_alt_width == 0) {
    CROAK("fragment_alt has no width in fragment_alt @ %s:%d", file, line);
    return false;
  }
  if (fragment_alt_height == 0) {
    CROAK("fragment_alt has no height in fragment_alt @ %s:%d", file, line);
    return false;
  }
  if (fragment_alt_width > MAP_WIDTH) {
    CROAK("fragment_alt is too wide in fragment_alt @ %s:%d", file, line);
    return false;
  }
  if (fragment_alt_height > MAP_HEIGHT) {
    CROAK("fragment_alt is too tall in fragment_alt @ %s:%d", file, line);
    return false;
  }

  //
  // Allocate space for the fragment_alt
  //
  auto *f = new FragmentAlt();
  fragment_alts_all.push_back(f);

  fragment_alts_curr.clear();
  fragment_alts_curr.push_back(f);

  f->id         = fragment_alt_no++;
  f->chance     = chance;
  f->room_flags = room_flags;
  f->file       = file;
  f->line       = line;
  f->width      = fragment_alt_width;
  f->height     = fragment_alt_height;
  f->data       = static_cast< char * >(MYZALLOC(fragment_alt_width * fragment_alt_height, "fragment_alt data"));

  //
  // Now read the fragment_alt again
  //
  va_start(ap, line);

  for (int y = 0; y < f->height; y++) {
    const char *fragment_alt_line = va_arg(ap, char *);
    for (int x = 0; x < f->width; x++) {
      auto c = fragment_alt_line[ x ];

      f->data[ (y * f->width) + x ] = c;
    }
  }

  va_end(ap);

  //
  // Make alternate fragment_alts
  //
  fragment_alt_rotate(fragment_alt_rotate(fragment_alt_rotate(f)));
  fragment_alt_rotate(fragment_alt_rotate(fragment_alt_rotate(fragment_alt_flip_horiz(f))));

  //
  // Push the alternatives onto the end of each fragment
  //
  int i = 0;
  for (auto *c : fragments_curr) {
    c->fragment_alts.push_back(fragment_alts_curr[ i++ ]);
  }

  return true;
}

//
// Get a random alt fragment.
//
auto fragment_alt_random_get(Gamep g, Fragment *f, bpoint at) -> class FragmentAlt *
{
  TRACE();

  int tries = 0;
  while (tries++ < MAX_LEVEL_GEN_FRAGMENT_TRIES) {
    auto *a = f->fragment_alts[ PCG_RANDOM_RANGE(0, f->fragment_alts.size()) ];

    if (d10000() < a->chance) {
      return a;
    }
  }
  return nullptr;
}

//
// Get a random alt fragment.
//
static auto fragment_alt_random_get(Gamep g, class LevelGen *l, Fragment *f, bpoint at) -> class FragmentAlt *
{
  TRACE();

  auto *r = l->data[ at.x ][ at.y ].room;
  if (f->fragment_alts.empty()) {
    return nullptr;
  }

  int tries = 0;
  while (tries++ < MAX_LEVEL_GEN_FRAGMENT_TRIES) {
    auto *a = f->fragment_alts[ PCG_RANDOM_RANGE(0, f->fragment_alts.size()) ];

    bool skip = false;

    if (r != nullptr) {
      switch (r->room_type) {
        case ROOM_TYPE_ENTRANCE :
          if ((a->room_flags & ROOM_FLAG_ENTRANCE) == 0U) {
            skip = true;
          }
          break;
        case ROOM_TYPE_NORMAL :
          if ((a->room_flags & ROOM_FLAG_NORMAL) == 0U) {
            skip = true;
          }
          break;
        case ROOM_TYPE_EXIT :
          if ((a->room_flags & ROOM_FLAG_EXIT) == 0U) {
            skip = true;
          }
          break;
        case ROOM_TYPE_LOCKED :
          if ((a->room_flags & ROOM_FLAG_LOCKED) == 0U) {
            skip = true;
          }
          break;
        case ROOM_TYPE_HAS_KEY :
          if ((a->room_flags & ROOM_FLAG_HAS_KEY) == 0U) {
            skip = true;
          }
          break;
        case ROOM_TYPE_MAX : break;
      }
    }

    if (skip) {
      continue;
    }

    if (d10000() < a->chance) {
      return a;
    }
  }
  return nullptr;
}

//
// Dump a fragment_alt
//
static void fragment_alt_dump(class FragmentAlt *f)
{
  TRACE();

  level_gen_mutex.lock();
  LOG("FragmentAlt %d @ %s:%d", f->id, f->file, f->line);
  TRACE_INDENT();

  for (int y = 0; y < f->height; y++) {
    std::string tmp;
    for (int x = 0; x < f->width; x++) {
      tmp += f->data[ (y * f->width) + x ];
    }
    LOG("[%s]", tmp.c_str());
  }

  LOG("-");
  level_gen_mutex.unlock();
}

//
// Dump all fragment_alts
//
// NOLINTBEGIN
void fragment_alts_dump(Gamep g)
// NOLINTEND
{
  TRACE();

  for (auto *f : fragment_alts_all) {
    fragment_alt_dump(f);
  }
}

//
// Clean up fragment_alts
//
void fragment_alts_fini(Gamep g)
{
  TRACE();

  for (auto *f : fragment_alts_all) {
    delete f;
  }
}

auto fragment_width(class Fragment *f) -> int { return f->width; }

auto fragment_height(class Fragment *f) -> int { return f->height; }

//
// Read a fragment char
//
auto fragment_char(class Fragment *f, int x, int y) -> char
{
  if (x < 0) {
    return CHARMAP_EMPTY;
  }
  if (y < 0) {
    return CHARMAP_EMPTY;
  }
  if (x >= f->width) {
    return CHARMAP_EMPTY;
  }
  if (y >= f->height) {
    return CHARMAP_EMPTY;
  }

  return f->data[ (y * f->width) + x ];
}

//
// Rotate the current fragment clockwise and put that into a new fragment
//
static auto fragment_rotate(class Fragment *f) -> class Fragment *
{
  TRACE();

  auto *n = new Fragment();
  fragments_all.push_back(n);
  fragments_curr.push_back(n);

  n->id     = fragment_no++;
  n->chance = f->chance;
  n->file   = f->file;
  n->line   = f->line;
  n->width  = f->height;
  n->height = f->width;
  n->data   = static_cast< char * >(MYZALLOC(f->width * f->height, "fragment data"));

  for (int y = 0; y < f->height; y++) {
    for (int x = 0; x < f->width; x++) {
      int const nx                    = n->width - y - 1;
      int const ny                    = x;
      n->data[ (ny * n->width) + nx ] = f->data[ (y * f->width) + x ];
    }
  }

  return n;
}

//
// Flip the current fragment horizontally and put that into a new fragment
//
static auto fragment_flip_horiz(class Fragment *f) -> class Fragment *
{
  TRACE();

  auto *n = new Fragment();
  fragments_all.push_back(n);
  fragments_curr.push_back(n);

  n->id     = fragment_no++;
  n->chance = f->chance;
  n->file   = f->file;
  n->line   = f->line;
  n->width  = f->width;
  n->height = f->height;
  n->data   = static_cast< char * >(MYZALLOC(f->width * f->height, "fragment data"));

  for (int y = 0; y < f->height; y++) {
    for (int x = 0; x < f->width; x++) {
      int const nx                    = f->width - x - 1;
      int const ny                    = y;
      n->data[ (ny * f->width) + nx ] = f->data[ (y * f->width) + x ];
    }
  }

  return n;
}

//
// Add a fragment and copies with all possible rotations
//
auto fragment_add(Gamep g, int chance, const char *file, int line, ...) -> bool
{
  TRACE();

  va_list ap              = {};
  int     fragment_width  = 0;
  int     fragment_height = 0;

  //
  // First scan, get width and height
  //
  va_start(ap, line);

  for (;;) {
    const char *fragment_line = va_arg(ap, const char *);
    if (fragment_line == nullptr) {
      break;
    }

    int const this_line_width = static_cast< int >(strlen(fragment_line));

    if (fragment_width == 0) {
      //
      // Get the fragment width
      //
      fragment_width = this_line_width;
    } else {
      //
      // Check width is constant
      //
      if (fragment_width != this_line_width) {
        CROAK("fragment has inconsistent width in fragment @ %s:%d", file, line);
        return false;
      }
    }

    //
    // Check the fragment contents are known characters
    //
    for (auto i = 0; i < fragment_width; i++) {
      switch (fragment_line[ i ]) {
        case CHARMAP_BARREL :        break;
        case CHARMAP_BRAZIER :       break;
        case CHARMAP_BRIDGE :        break;
        case CHARMAP_CHASM :         break;
        case CHARMAP_CHASM_50 :      break;
        case CHARMAP_CORRIDOR :      break;
        case CHARMAP_DEEP_WATER :    break;
        case CHARMAP_DOOR_LOCKED :   break;
        case CHARMAP_DOOR_SECRET :   break;
        case CHARMAP_DOOR_UNLOCKED : break;
        case CHARMAP_EMPTY :         break;
        case CHARMAP_ENTRANCE :      break;
        case CHARMAP_EXIT :          break;
        case CHARMAP_FLOOR :         break;
        case CHARMAP_FLOOR_50 :      break;
        case CHARMAP_FOLIAGE :       break;
        case CHARMAP_GRASS :         break;
        case CHARMAP_JOIN :          break;
        case CHARMAP_KEY :           break;
        case CHARMAP_LAVA :          break;
        case CHARMAP_MOB1 :          break;
        case CHARMAP_MOB2 :          break;
        case CHARMAP_MONST1 :        break;
        case CHARMAP_MONST2 :        break;
        case CHARMAP_PILLAR :        break;
        case CHARMAP_TELEPORT :      break;
        case CHARMAP_TRAP :          break;
        case CHARMAP_TREASURE :      break;
        case CHARMAP_WALL :          break;
        case CHARMAP_WATER :         break;
        case CHARMAP_WILDCARD :      break;
        case CHARMAP_FIRE :          break;
        default :                    CROAK("fragment has unknown char [%c] in fragment @ %s:%d", fragment_line[ i ], file, line); return false;
      }
    }

    fragment_height++;
  }

  va_end(ap);

  //
  // Check the fragment sizes
  //
  if (fragment_width == 0) {
    CROAK("fragment has no width in fragment @ %s:%d", file, line);
    return false;
  }
  if (fragment_height == 0) {
    CROAK("fragment has no height in fragment @ %s:%d", file, line);
    return false;
  }
  if (fragment_width > MAP_WIDTH) {
    CROAK("fragment is too wide in fragment @ %s:%d", file, line);
    return false;
  }
  if (fragment_height > MAP_HEIGHT) {
    CROAK("fragment is too tall in fragment @ %s:%d", file, line);
    return false;
  }

  //
  // Allocate space for the fragment
  //
  auto *f = new Fragment();
  fragments_all.push_back(f);

  fragments_curr.clear();
  fragments_curr.push_back(f);

  f->id     = fragment_no++;
  f->chance = chance;
  f->file   = file;
  f->line   = line;
  f->width  = fragment_width;
  f->height = fragment_height;
  f->data   = static_cast< char * >(MYZALLOC(fragment_width * fragment_height, "fragment data"));

  //
  // Now read the fragment again
  //
  va_start(ap, line);

  for (int y = 0; y < f->height; y++) {
    const char *fragment_line = va_arg(ap, char *);
    for (int x = 0; x < f->width; x++) {
      auto c = fragment_line[ x ];

      f->data[ (y * f->width) + x ] = c;
    }
  }

  va_end(ap);

  //
  // Make alternate fragments
  //
  fragment_rotate(fragment_rotate(fragment_rotate(f)));
  fragment_rotate(fragment_rotate(fragment_rotate(fragment_flip_horiz(f))));

  return true;
}

//
// Get a random fragment.
//
auto fragment_random_get(Gamep g) -> class Fragment *
{
  TRACE();

  if (fragments_all.empty()) {
    CROAK("no fragments");
  }

  int tries = 0;
  while (tries++ < MAX_LEVEL_GEN_FRAGMENT_TRIES) {
    auto *f = fragments_all[ PCG_RANDOM_RANGE(0, fragments_all.size()) ];
    if (d10000() < f->chance) {
      return f;
    }
  }
  return nullptr;
}

//
// Dump a fragment
//
static void fragment_dump(class Fragment *f)
{
  TRACE();

  level_gen_mutex.lock();
  LOG("fragment %d @ %s:%d", f->id, f->file, f->line);
  TRACE_INDENT();

  for (int y = 0; y < f->height; y++) {
    std::string tmp;
    for (int x = 0; x < f->width; x++) {
      tmp += f->data[ (y * f->width) + x ];
    }
    LOG("[%s]", tmp.c_str());
  }

  LOG("-");
  level_gen_mutex.unlock();
}

//
// Dump all fragments
//
// NOLINTBEGIN
void fragments_dump(Gamep g)
// NOLINTEND
{
  TRACE();

  for (auto *f : fragments_all) {
    fragment_dump(f);
  }
}

//
// Can we match a fragment against the location
//
[[nodiscard]] static auto fragment_match(Gamep g, class LevelGen *l, class Fragment *f, bpoint at) -> bool
{
  for (int ry = 0; ry < f->height; ry++) {
    for (int rx = 0; rx < f->width; rx++) {

      auto c = fragment_char(f, rx, ry);
      if (c == CHARMAP_WILDCARD) {
        continue;
      }

      bpoint const p(rx + at.x, ry + at.y);

      if ((p.x <= 0)) [[unlikely]] {
        return false;
      }
      if ((p.x >= MAP_WIDTH - 1)) [[unlikely]] {
        return false;
      }
      if ((p.y <= 0)) [[unlikely]] {
        return false;
      }
      if ((p.y >= MAP_HEIGHT - 1)) [[unlikely]] {
        return false;
      }
      if (c != l->data[ p.x ][ p.y ].c) {
        return false;
      }
    }
  }

  //
  // Match
  //
  return true;
}

//
// Place the fragment
//
static auto fragment_put(Gamep g, class LevelGen *l, class Fragment *f, bpoint at) -> FragmentAlt *
{
  TRACE();

  if (f->fragment_alts.empty()) {
    fragment_dump(f);
    CROAK("no alternative fragments for fragment");
    return nullptr;
  }

  auto *a = fragment_alt_random_get(g, l, f, at);
  if (a == nullptr) {
    return nullptr;
  }

  for (int ry = 0; ry < f->height; ry++) {
    for (int rx = 0; rx < f->width; rx++) {

      auto c = fragment_alt_char(a, rx, ry);
      if (c == CHARMAP_EMPTY) {
        continue;
      }

      bpoint const p(rx + at.x, ry + at.y);

      if ((p.x <= 0)) [[unlikely]] {
        continue;
      }
      if ((p.x >= MAP_WIDTH - 1)) [[unlikely]] {
        continue;
      }
      if ((p.y <= 0)) [[unlikely]] {
        continue;
      }
      if ((p.y >= MAP_HEIGHT - 1)) [[unlikely]] {
        continue;
      }

      l->data[ p.x ][ p.y ].c = c;
    }
  }

  return a;
}

//
// Add fragments if we find any matches
//
static void level_gen_add_fragments(Gamep g, class LevelGen *l)
{
  TRACE();

  int tries = 0;
  while (tries++ < MAX_LEVEL_GEN_FRAGMENT_TRIES) {
    auto *f = fragment_random_get(g);
    if (f == nullptr) {
      return;
    }

    std::vector< bpoint > cands;

    for (int y = 0; y < MAP_HEIGHT - f->height; y++) {
      for (int x = 0; x < MAP_WIDTH - f->width; x++) {
        bpoint const at(x, y);
        if (fragment_match(g, l, f, at)) {
          cands.push_back(at);
        }
      }
    }

    if (cands.empty()) {
      continue;
    }

    auto  cand = cands[ PCG_RAND() % cands.size() ];
    auto *alt  = fragment_put(g, l, f, cand);
    if (alt == nullptr) {
      continue;
    }

    if (l->debug) [[unlikely]] {
      auto fragment_name = std::format("placed another fragment {}:{} with {}:{}", f->file, f->line, alt->file, alt->line);
      level_gen_dump(l, fragment_name.c_str());
    }

    if (l->info.fragment_count++ >= MAX_LEVEL_GEN_FRAGMENTS) {
      return;
    }
  }
}

//
// Clean up fragments
//
void fragments_fini(Gamep g)
{
  TRACE();

  for (auto *f : fragments_all) {
    delete f;
  }
}

//
// Add a level
//
void level_fixed_add(Gamep g, int chance, LevelType level_type, const std::string &alias, const char *file, int line,
                     const Overrides &overrides, int dummy, ...)
{
  TRACE();

  va_list ap = {};

  int level_width  = 0;
  int level_height = 0;

  //
  // Ensure not too many of certain tiles
  //
  int exit_count     = 0;
  int entrance_count = 0;

  //
  // First scan, get width and height
  //
  va_start(ap, dummy);

  for (;;) {
    const char *level_line = va_arg(ap, const char *);
    if (level_line == nullptr) {
      break;
    }

    int const this_line_width = static_cast< int >(strlen(level_line));

    if (level_width == 0) {
      //
      // Get the level width
      //
      level_width = this_line_width;
    } else {
      //
      // Check width is constant
      //
      if (level_width != this_line_width) {
        CROAK("level has inconsistent width in level @ %s:%d", file, line);
        return;
      }
    }

    //
    // Check the level contents are known characters
    //
    for (auto i = 0; i < level_width; i++) {
      auto c = level_line[ i ];
      switch (c) {
        case CHARMAP_BARREL :        break;
        case CHARMAP_BRAZIER :       break;
        case CHARMAP_BRIDGE :        break;
        case CHARMAP_CHASM :         break;
        case CHARMAP_CHASM_50 :      break;
        case CHARMAP_CORRIDOR :      break;
        case CHARMAP_DEEP_WATER :    break;
        case CHARMAP_DOOR_SECRET :   break;
        case CHARMAP_DOOR_UNLOCKED : break;
        case CHARMAP_DOOR_LOCKED :   break;
        case CHARMAP_EMPTY :         break;
        case CHARMAP_FLOOR :         break;
        case CHARMAP_FLOOR_50 :      break;
        case CHARMAP_FOLIAGE :       break;
        case CHARMAP_GRASS :         break;
        case CHARMAP_JOIN :          break;
        case CHARMAP_KEY :           break;
        case CHARMAP_LAVA :          break;
        case CHARMAP_MOB1 :          break;
        case CHARMAP_MOB2 :          break;
        case CHARMAP_MONST1 :        break;
        case CHARMAP_MONST2 :        break;
        case CHARMAP_PILLAR :        break;
        case CHARMAP_TELEPORT :      break;
        case CHARMAP_TRAP :          break;
        case CHARMAP_TREASURE :      break;
        case CHARMAP_WALL :          break;
        case CHARMAP_WATER :         break;
        case CHARMAP_ROCK :          break;
        case CHARMAP_BORDER :        break;
        case CHARMAP_FIRE :          break;
        case CHARMAP_EXIT :
          if ((exit_count++) != 0) {
            CROAK("level has too many exits in level @ %s:%d", file, line);
            return;
          }
          break;
        case CHARMAP_ENTRANCE :
          if ((entrance_count++) != 0) {
            CROAK("level has too many entrances in level @ %s:%d", file, line);
            return;
          }
          break;
        default :
          if (! overrides.contains(c)) {
            CROAK("level has unknown char [%c] in level @ %s:%d", level_line[ i ], file, line);
            return;
          }
      }
    }

    level_height++;
  }

  va_end(ap);

  //
  // Check the level sizes
  //
  if (level_width == 0) {
    CROAK("level has no width in level @ %s:%d", file, line);
    return;
  }
  if (level_height == 0) {
    CROAK("level has no height in level @ %s:%d", file, line);
    return;
  }
  if (level_width > MAP_WIDTH) {
    CROAK("level is too wide in level @ %s:%d", file, line);
    return;
  }
  if (level_height > MAP_HEIGHT) {
    CROAK("level is too tall in level @ %s:%d", file, line);
    return;
  }

  //
  // Allocate space for the level
  //
  auto *l = new LevelFixed();
  level_fixed_all[ level_type ].push_back(l);

  if (level_alias_all.contains(alias)) {
    CROAK("level alias \"%s\" is in use already @ %s:%d", alias.c_str(), file, line);
    return;
  }

  level_alias_all[ alias ] = l;

  l->id         = level_fixed_no++;
  l->chance     = chance;
  l->level_type = level_type;
  l->alias      = alias;
  l->file       = file;
  l->line       = line;
  l->width      = level_width;
  l->height     = level_height;
  l->overrides  = overrides;
  l->data       = static_cast< char * >(MYZALLOC(MAP_WIDTH * MAP_HEIGHT, "level data"));

  //
  // Center the level if smaller
  //
  auto pad_w = static_cast< int >(floor((MAP_WIDTH - level_width) / 2));
  auto pad_h = static_cast< int >(floor((MAP_HEIGHT - level_height) / 2));

  //
  // Now read the level again
  //
  va_start(ap, dummy);

  for (int y = 0; y < l->height; y++) {
    const char *level_line = va_arg(ap, char *);
    for (int x = 0; x < l->width; x++) {
      auto c = level_line[ x ];

      auto X = x + pad_w;
      auto Y = y + pad_h;

      l->data[ (Y * MAP_WIDTH) + X ] = c;
    }
  }

  va_end(ap);

  if (exit_count == 0) {
    if (level_type != LEVEL_TYPE_TEST) {
      CROAK("level has no exits @ %s:%d", file, line);
    }
    return;
  }
}

//
// Clean up levels
//
void levels_fini(Gamep g)
{
  TRACE();

  for (auto level_type = static_cast< int >(LEVEL_TYPE_FIRST); level_type < static_cast< int >(LEVEL_TYPE_MAX); level_type++) {
    for (auto *l : level_fixed_all[ level_type ]) {
      delete l;
    }
  }
}

//
// Get a random level.
//
static auto level_random_get(LevelType level_type) -> class LevelFixed *
{
  TRACE();

  if (level_fixed_all[ level_type ].empty()) {
    ERR("no levels of type %d", level_type);
    return nullptr;
  }

  return level_fixed_all[ level_type ][ PCG_RANDOM_RANGE(0, level_fixed_all[ level_type ].size()) ];
}

//
// Get a random level.
//
static auto level_fixed_find_by_name(const std::string &alias, LevelNum level_num) -> class LevelFixed *
{
  TRACE();

  //
  // Add the level number to allow specification of specific depths
  //
  auto alt = alias + "." + std::to_string(static_cast< int >(level_num) + 1);
  if (level_alias_all.contains(alt)) {
    return level_alias_all[ alt ];
  }

  //
  // Fallback to the level name and use the same level of all depths
  //
  if (level_alias_all.contains(alias)) {
    return level_alias_all[ alias ];
  }

  return nullptr;
}

//
// Convert a level into a single string
//
static auto level_gen_string(class LevelGen *o, class LevelFixed *l) -> std::string
{
  TRACE();

  std::string out;

  if (l == nullptr) {
    ERR("no fixed level provided");
    return "";
  }

  if (l->data == nullptr) {
    ERR("no fixed level data provided");
    return "";
  }

  if (o == nullptr) {
    ERR("no destination level provided");
    return "";
  }

  //
  // Useful to have the chars in the common LevelGen structure for things like
  // counting.
  //
  for (int y = 0; y < MAP_HEIGHT; y++) {
    for (int x = 0; x < MAP_WIDTH; x++) {
      auto c = l->data[ (y * MAP_WIDTH) + x ];
      if (c == 0) {
        c = CHARMAP_CHASM;
      }
      o->data[ x ][ y ].c = c;
      out += c;
    }
  }

  return out;
}

//
// Dump a level
//
static void level_gen_dump(class LevelGen *l, const char *msg)
{
  TRACE();

  level_gen_mutex.lock();
  if (msg != nullptr) {
    LOG("level: %u (%s)", l->level_num + 1, msg);
  } else {
    LOG("level: %u", l->level_num + 1);
  }
  TRACE();

  LOG("seed              : %u", l->info.seed_num);
  LOG("room count        : %d", l->info.room_count);
  LOG("fragment count    : %d", l->info.fragment_count);
  LOG("treasure count    : %d", l->info.treasure_count);
  LOG("monst count       : %d (normal:%d enhanced:%d)", l->info.monst_count, l->info.monst_group_easy_count,
      l->info.monst_group_hard_count);
  LOG("teleport count    : %d", l->info.teleport_count);
  LOG("locked door count : %d", l->info.door_locked_count);
  LOG("key count         : %d", l->info.key_count);

  for (int y = 0; y < MAP_HEIGHT; y++) {
    std::string tmp;
    for (auto &x : l->data) {
      auto c = x[ y ].c;
      tmp += c;
    }
    LOG("[%s]", tmp.c_str());
  }

  LOG("-");

  //
  // Sometimes useful to see walkable paths
  //
  if (compiler_unused) {
    for (int y = 0; y < MAP_HEIGHT; y++) {
      std::string tmp;
      for (int x = 0; x < MAP_WIDTH; x++) {
        auto c = l->data[ x ][ y ].c;

        switch (l->data[ x ][ y ].c) {
          case CHARMAP_CHASM :
          case CHARMAP_DOOR_LOCKED :
          case CHARMAP_DOOR_SECRET :
          case CHARMAP_DOOR_UNLOCKED :
          case CHARMAP_ENTRANCE :
          case CHARMAP_EXIT :
          case CHARMAP_JOIN :
          case CHARMAP_KEY :
          case CHARMAP_BRIDGE :
          case CHARMAP_LAVA :
          case CHARMAP_WALL :
          case CHARMAP_EMPTY :         break;
          case CHARMAP_MOB1 :
          case CHARMAP_BARREL :
          case CHARMAP_BRAZIER :
          case CHARMAP_CORRIDOR :
          case CHARMAP_MOB2 :
          case CHARMAP_PILLAR :
          case CHARMAP_TELEPORT :
          case CHARMAP_TRAP :
          case CHARMAP_MONST1 :
          case CHARMAP_MONST2 :
          case CHARMAP_FLOOR :
          case CHARMAP_TREASURE :
          case CHARMAP_FOLIAGE :
          case CHARMAP_GRASS :
            if (l->info.on_path_entrance_to_exit[ x ][ y ] != 0U) {
              c = '_';
            }
            break;
        }

        tmp += c;
      }
      LOG("[%s]", tmp.c_str());
    }

    LOG("-");
  }

  level_gen_mutex.unlock();
}

//
// Convert a level into a single string
//
static auto level_gen_string(class LevelGen *l) -> std::string
{
  TRACE();

  std::string out;

  for (int y = 0; y < MAP_HEIGHT; y++) {
    for (auto &x : l->data) {
      out += x[ y ].c;
    }
  }

  return out;
}

//
// Level stats
//
void level_gen_stats_dump(Gamep g)
{
  TRACE();

  LOG("level generation errors:");
  LOG("- create level fail:               %d", level_create_fail);
  LOG("- place first room fail:           %d", level_place_first_room_fail);
  LOG("- place subsequent room fail:      %d", level_place_subsequent_room_fail);
  LOG("- find door to place room fail:    %d", level_find_door_fail_count);
  LOG("- tried to place a duplicate room: %d", level_tried_to_place_existing_room_fail);
  LOG("- not enough rooms generated:      %d", level_not_enough_rooms);
  LOG("- no exit room generated:          %d", level_no_exit_room);
}

//
// Update the list of all doors, and return a new unwalked door to use
//
[[nodiscard]] static auto level_gen_random_door_get(class LevelGen *l, bpoint *door_out, class Room **room_out) -> bool
{
  TRACE();

  l->doors_not_explored.clear();

  //
  // Build a vector of all doors that have not been walked before
  //
  for (int y = 0; y < MAP_HEIGHT; y++) {
    for (int x = 0; x < MAP_WIDTH; x++) {
      //
      // Filter to doors
      //
      if (l->data[ x ][ y ].c != CHARMAP_JOIN) {
        continue;
      }

      bpoint const p(x, y);

      //
      // If this door is walked already, ignore it
      //
      if (l->doors_walked.contains(p)) {
        continue;
      }

      //
      // Add to the candidates
      //
      l->doors_not_explored.push_back(bpoint(x, y));
    }
  }

  //
  // Did we find any new doors?
  //
  if (l->doors_not_explored.empty()) {
    if (l->debug) [[unlikely]] {
      LOG("no more doors");
    }
    return false;
  }

  //
  // Return a random door
  //
  *door_out = l->doors_not_explored[ PCG_RANDOM_RANGE(0, l->doors_not_explored.size()) ];
  *room_out = l->data[ door_out->x ][ door_out->y ].room;

  //
  // Don't try this door again
  //
  l->doors_walked[ *door_out ] = true;

  if (*room_out == nullptr) {
    ERR("found a door with no room");
  }

  return true;
}

//
// Place a room of the given type at a specific door
//
[[nodiscard]] static auto level_gen_place_room_at_door_intersection(Gamep g, LevelGen *l, const bpoint door_other, const RoomType room_type)
    -> bool
{
  TRACE();

  //
  // Try multiple rooms with this door
  //
  int room_place_tries   = 0;
  int room_width_failed  = 0;
  int room_height_failed = 0;

  while (room_place_tries++ < MAX_LEVEL_GEN_ROOM_PLACE_TRIES) {
    //
    // Get a new room we have not placed before to try to place
    //
    auto *r = room_random_get(room_type);
    if (l->rooms_placed.contains(r)) {
      level_tried_to_place_existing_room_fail++;
      continue;
    }

    //
    // If a certain size failed, do not try that or bigger again
    //
    if (room_width_failed != 0) {
      if (r->width >= room_width_failed) {
        continue;
      }
      if (r->height >= room_height_failed) {
        continue;
      }
    }

    //
    // For each door in the room, can it be placed to connect to the door passed in?
    //
    for (auto d : r->doors) {
      //
      // Place at the D joining rooms e.g.:
      //
      //       ...
      //     D....
      //       ...
      //
      // ...   ...
      // ....D....
      // ...   ...
      //
      bpoint const door_intersection_at = door_other - d;
      if (! room_can_place_at(g, l, r, door_intersection_at)) {
        continue;
      }

      //
      // Place the room
      //
      room_place_at(l, r, door_intersection_at);

      //
      // Have placed an exit?
      //
      if (r->room_type == ROOM_TYPE_EXIT) {
        l->has_placed_ROOM_TYPE_EXIT = true;
        l->room_exit                 = r;
      }

      if (r->room_type == ROOM_TYPE_LOCKED) {
        l->has_placed_ROOM_TYPE_DOOR_LOCKED = true;
      }

      if (r->room_type == ROOM_TYPE_HAS_KEY) {
        l->has_placed_ROOM_TYPE_HAS_KEY = true;
      }

      //
      // Success
      //
      return true;
    }

    level_place_subsequent_room_fail++;
    room_width_failed  = r->width;
    room_height_failed = r->height;
  }

  //
  // Failed
  //
  return false;
}

//
// Place a room of the given type at a random door
//
[[nodiscard]] static auto level_gen_create_another_room(Gamep g, LevelGen *l, RoomType room_type) -> bool
{
  TRACE();

  //
  // Find a random door that we have not walked before
  //
  bpoint      door_other = {};
  class Room *room_other = {}; // NOLINT
  if (! level_gen_random_door_get(l, &door_other, &room_other)) {
    level_find_door_fail_count++;
    return false;
  }

  //
  // If this door is too close, then switch to a normal room
  //
  if (room_type == ROOM_TYPE_EXIT) {
    bpoint const entrance(l->info.entrance_at.x, l->info.entrance_at.y);
    if (distance(door_other, entrance) < MIN_LEVEL_EXIT_DISTANCE) {
      room_type = ROOM_TYPE_NORMAL;
    }
  }

  //
  // Try multiple rooms with this door
  //
  if (level_gen_place_room_at_door_intersection(g, l, door_other, room_type)) {
    return true;
  }

  //
  // Try again
  //
  if (room_type == ROOM_TYPE_EXIT) {
    room_type = ROOM_TYPE_NORMAL;
    return level_gen_place_room_at_door_intersection(g, l, door_other, room_type);
  }

  return false;
}

//
// The first room has been placed. Place all the rest.
//
static void level_gen_create_remaining_rooms(Gamep g, LevelGen *l)
{
  TRACE();

  int        attempts                   = 0;
  bool const need_ROOM_TYPE_DOOR_LOCKED = d100() < LEVEL_GEN_CHANCE_OF_DOOR_LOCKED;

  //
  // Keep placing rooms until we hit the max allowed
  //
  while (std::cmp_less(l->rooms_placed.size(), l->max_room_count)) {
    if (l->debug) [[unlikely]] {
      LOG("rooms placed %d (max %d) attempts %d doors-tried %d doors-not-tried %d", static_cast< int >(l->rooms_placed.size()),
          l->max_room_count, attempts, static_cast< int >(l->doors_walked.size()), static_cast< int >(l->doors_not_explored.size()));
    }

    //
    // Ensure we loop only so many times
    //
    if (attempts++ > MAX_LEVEL_GEN_TRIES_CREATE_ROOM) {
      if (l->debug) [[unlikely]] {
        LOG("have tried enough times to place a room");
      }
      break;
    }

    //
    // Ran out of doors to try?
    //
    if ((static_cast< int >(l->doors_walked.size())) != 0) {
      if (l->doors_not_explored.empty()) {
        if (l->debug) [[unlikely]] {
          LOG("have tried all doors");
        }
        break;
      }
    }

    RoomType room_type = ROOM_TYPE_NORMAL;

    //
    // Special rooms? Keep trying to place until we can.
    //
    if (need_ROOM_TYPE_DOOR_LOCKED) {
      if (! l->has_placed_ROOM_TYPE_DOOR_LOCKED) {
        if (d100() < 50) {
          room_type = ROOM_TYPE_LOCKED;
        }
      }

      if (l->has_placed_ROOM_TYPE_DOOR_LOCKED) {
        if (! l->has_placed_ROOM_TYPE_HAS_KEY) {
          room_type = ROOM_TYPE_HAS_KEY;
        }
      }
    }

    //
    // If we have not yet placed an exit room, should we?
    //
    if (! l->has_placed_ROOM_TYPE_EXIT) {
      if (d100() < 50) {
        room_type = ROOM_TYPE_EXIT;
      } else if (std::cmp_greater(l->rooms_placed.size(), l->min_room_count)) {
        //
        // If we have the minimum rooms, then likely we're far enough away from
        // the start room now to try placing an exit room.
        //
        if (d100() < 50) {
          room_type = ROOM_TYPE_EXIT;
        }
      } else if (attempts > (MAX_LEVEL_GEN_TRIES_CREATE_ROOM / 2)) {
        //
        // If we're running out of room place attempts, try an exit room
        //
        if (d100() < 50) {
          room_type = ROOM_TYPE_EXIT;
        }
      }
    }

    if (l->debug) [[unlikely]] {
      level_gen_dump(l, "placed another room");
    }

    //
    // Create another room if possible
    //
    if (! level_gen_create_another_room(g, l, room_type)) {
      continue;
    }

    //
    // If we are out of doors to walk, no point in continuing.
    //
    if (l->doors_not_explored.empty()) {
      return;
    }
  }
}

//
// Place the first room
//
[[nodiscard]] static auto level_gen_create_first_room(Gamep g, LevelGen *l) -> bool
{
  TRACE();

  //
  // Choose a random start point for the rooms
  //
  int const border = MAP_WIDTH / 4;
  int       x      = 0;
  int       y      = 0;

  //
  // Start somewhere central
  //
  x = PCG_RANDOM_RANGE(border, MAP_WIDTH - border);
  y = PCG_RANDOM_RANGE(border, MAP_HEIGHT - border);

  bpoint const at(x, y);

  //
  // Choose a random first room and place it
  //
  auto *r = l->room_entrance = room_random_get(ROOM_TYPE_ENTRANCE);
  if (! room_can_place_at(g, l, r, at)) {
    return false;
  }

  room_place_at(l, r, at);

  //
  // Placed the first room
  //
  if (l->debug) [[unlikely]] {
    level_gen_dump(l, "placed first room");
  }

  return true;
}

static void cave_dump(class LevelGen *l)
{
  uint8_t x = 0;
  uint8_t y = 0;

  std::print("+");
  for (x = 0; x < MAP_WIDTH; x++) {
    std::print("-");
  }
  std::print("+");
  std::println("");

  for (y = 0; y < MAP_HEIGHT; y++) {
    std::print("|");
    for (x = 0; x < MAP_WIDTH; x++) {
      if (l->cave.curr[ x + MAP_LEVEL_CELLULAR_BORDER ][ y + MAP_LEVEL_CELLULAR_BORDER ] != 0U) {
        std::print("x");
      } else {
        std::print(" ");
      }
    }
    std::print("|");
    std::println("");
  }

  std::print("+");
  for (x = 0; x < MAP_WIDTH; x++) {
    std::print("-");
  }
  std::print("+");
  std::println("");
}

//
// Place a single large pool of water/lava/chasm on the level
//
static void level_gen_single_large_blob_in_center(Gamep g, class LevelGen *l, char c)
{
  uint8_t        x               = 0;
  uint8_t        y               = 0;
  uint32_t const fill_prob       = LEVEL_BLOB_GEN_FILL_PROB;
  int const      r1              = 10; // higher r1 gives a more rounded look
  int const      r2              = 4;  // larger r2 gives a smaller pool
  int const      map_generations = 3;

  //
  // Generate a cave
  //
  cave_create(g, &l->cave, fill_prob, r1, r2, map_generations);

  //
  // Keep a single large blob for the lake
  //
  cave_generation_keep_largest_blob(g, &l->cave);

  //
  // And put in the middle of the level
  //
  cave_generation_center_blob(g, &l->cave);

  if (compiler_unused) {
    cave_dump(l);
    level_gen_dump(l);
  }

  //
  // Add central bridges?
  //
  bool add_bridge_across {};
  bool add_bridge_down {};
  auto r      = d100();
  auto chance = LEVEL_GEN_CHANCE_OF_CENTRAL_BRIDGE / 2;

  if (c == CHARMAP_CHASM) {
    chance *= 2;
  }

  if (r < chance) {
    add_bridge_across = true;
  } else if (r > 100 - chance) {
    add_bridge_down = true;
  }

  for (x = 0; x < MAP_WIDTH; x++) {
    for (y = 0; y < MAP_HEIGHT; y++) {
      if (l->cave.curr[ x + MAP_LEVEL_CELLULAR_BORDER ][ y + MAP_LEVEL_CELLULAR_BORDER ] != 0U) {

        switch (l->data[ x ][ y ].c) {
          case CHARMAP_BARREL :
          case CHARMAP_BRAZIER :
          case CHARMAP_BRIDGE :
          case CHARMAP_CHASM :
          case CHARMAP_CORRIDOR :
          case CHARMAP_DOOR_LOCKED :
          case CHARMAP_DOOR_SECRET :
          case CHARMAP_DOOR_UNLOCKED :
          case CHARMAP_ENTRANCE :
          case CHARMAP_EXIT :
          case CHARMAP_FLOOR :
          case CHARMAP_FOLIAGE :
          case CHARMAP_GRASS :
          case CHARMAP_JOIN :
          case CHARMAP_KEY :
          case CHARMAP_LAVA :
          case CHARMAP_MOB1 :
          case CHARMAP_MOB2 :
          case CHARMAP_MONST1 :
          case CHARMAP_MONST2 :
          case CHARMAP_PILLAR :
          case CHARMAP_TELEPORT :
          case CHARMAP_TRAP :
          case CHARMAP_TREASURE :
          case CHARMAP_WALL :
            //
            // No water / chasm etc...
            //
            break;
          case CHARMAP_EMPTY :
            //
            // Water / chasm / etc...
            //
            if (add_bridge_down && (x == MAP_WIDTH / 2)) {
              l->data[ x ][ y ].c = CHARMAP_BRIDGE;
            } else if (add_bridge_across && (y == MAP_HEIGHT / 2)) {
              l->data[ x ][ y ].c = CHARMAP_BRIDGE;
            } else {
              l->data[ x ][ y ].c = c;
            }
            break;
        }
      }
    }
  }
}

//
// Add grass or other blobby things
//
static void level_gen_blob(Gamep g, class LevelGen *l, char c)
{
  uint8_t        x               = 0;
  uint8_t        y               = 0;
  uint32_t const fill_prob       = LEVEL_BLOB_GEN_FILL_PROB;
  int const      r1              = 4; // higher r1 gives a more rounded look
  int const      r2              = 4; // larger r2 gives a smaller pool
  int const      map_generations = 3;

  //
  // Generate a cave
  //
  cave_create(g, &l->cave, fill_prob, r1, r2, map_generations);

  if (compiler_unused) {
    cave_dump(l);
    level_gen_dump(l);
  }

  for (x = 0; x < MAP_WIDTH; x++) {
    for (y = 0; y < MAP_HEIGHT; y++) {
      if (l->cave.curr[ x + MAP_LEVEL_CELLULAR_BORDER ][ y + MAP_LEVEL_CELLULAR_BORDER ] != 0U) {
        switch (l->data[ x ][ y ].c) {
          case CHARMAP_BARREL :
          case CHARMAP_BRAZIER :
          case CHARMAP_BRIDGE :
          case CHARMAP_CHASM :
          case CHARMAP_CHASM_50 :
          case CHARMAP_CORRIDOR :
          case CHARMAP_DEEP_WATER :
          case CHARMAP_DOOR_LOCKED :
          case CHARMAP_DOOR_SECRET :
          case CHARMAP_DOOR_UNLOCKED :
          case CHARMAP_EMPTY :
          case CHARMAP_ENTRANCE :
          case CHARMAP_EXIT :
          case CHARMAP_FLOOR_50 :
          case CHARMAP_FOLIAGE :
          case CHARMAP_GRASS :
          case CHARMAP_JOIN :
          case CHARMAP_KEY :
          case CHARMAP_LAVA :
          case CHARMAP_MOB1 :
          case CHARMAP_MOB2 :
          case CHARMAP_MONST1 :
          case CHARMAP_MONST2 :
          case CHARMAP_PILLAR :
          case CHARMAP_TELEPORT :
          case CHARMAP_TRAP :
          case CHARMAP_TREASURE :
          case CHARMAP_WALL :
          case CHARMAP_WILDCARD :
            //
            // No item
            //
            break;
          case CHARMAP_FLOOR :
          case CHARMAP_DIRT :
          case CHARMAP_WATER :
            //
            // Place the item
            //
            if (c == CHARMAP_FOLIAGE) {
              if (! level_gen_is_room_entrance(g, l, x, y)) {
                l->data[ x ][ y ].c = c;
              }
            } else {
              l->data[ x ][ y ].c = c;
            }
            break;
        }
      }
    }
  }
}

//
// Create rooms from the current seed
//
static auto level_gen_new_class(Gamep g, LevelNum level_num) -> class LevelGen *
{
  TRACE();

  //
  // Per thread seed that increments each time we fail. Hopefully this avoids dup levels.
  //
  uint32_t seed_num = (game_seed_num_get(g) * 1001) + ((level_num + 1) * LEVEL_MAX);
  PCG_SRAND(seed_num);

  auto *l = new LevelGen();

  //
  // Per thread seed that increments each time we fail. Hopefully this avoids dup levels.
  //
  seed_num = (game_seed_num_get(g) * 1001) + ((level_num + 1) * LEVEL_MAX);
  PCG_SRAND(seed_num);
  l->info.seed_num = seed_num;

  l->level_num      = level_num;
  l->min_room_count = MIN_LEVEL_ROOM_COUNT + (level_num / 10);
  l->max_room_count = l->min_room_count + 10;
  l->debug          = g_opt_debug2;

  return l;
}

//
// Is this a special named level? or some kind of boss level?
//
[[nodiscard]] static auto level_gen_is_special_level(Levelsp v, LevelNum level_num) -> bool
{
  TRACE();

  if (! g_level_opt.level_name.empty()) {
    //
    // Test level
    //
    return true;
  }

  LevelSelect const *s = &v->level_select;
  if (level_num == s->level_count - 1) {
    //
    // Final boss level
    //
    return true;
  }

  return false;
}

//
// Create rooms from the current seed
//
static auto level_proc_gen_create_rooms(Gamep g, LevelNum level_num) -> class LevelGen *
{
  TRACE();

  LevelGen *l = {};

  //
  // Per thread seed that increments each time we fail. Hopefully this avoids dup levels.
  //
  uint32_t seed_num = (game_seed_num_get(g) * 1001) + ((level_num + 1) * LEVEL_MAX);
  PCG_SRAND(seed_num);

  bool const add_blob = d100() < LEVEL_BLOB_GEN_PROB;

  for (int level_gen_tries = 0; level_gen_tries < MAX_LEVEL_GEN_TRIES_FOR_SAME_SEED; level_gen_tries++) {
    //
    // Start with a fresh level each try
    //
    if (l != nullptr) {
      delete l;
      l = nullptr;
    }

    l = level_gen_new_class(g, level_num);

    //
    // Per thread seed that increments each time we fail. Hopefully this avoids dup levels.
    //
    seed_num = (game_seed_num_get(g) * 1001) + ((level_num + 1) * LEVEL_MAX) + level_gen_tries;
    PCG_SRAND(seed_num);
    l->info.seed_num = seed_num;

    //
    // Add a blob of hazard in the center of the level
    //
    if (add_blob) {
      //
      // The blob type
      //
      auto chance    = d100();
      char blob_type = CHARMAP_WATER;

      if (chance < LEVEL_BLOB_LAVA_GEN_PROB + level_num) {
        blob_type = CHARMAP_LAVA;
      } else if (chance < LEVEL_BLOB_CHASM_GEN_PROB) {
        blob_type = CHARMAP_CHASM;
      } else {
        blob_type = CHARMAP_WATER;
      }

      level_gen_single_large_blob_in_center(g, l, blob_type);
    }

    //
    // Place the first room
    //
    bool first_room_placed = false;
    for (auto attempts = 0; attempts < MAX_LEVEL_GEN_TRIES_CREATE_FIRST_ROOM; attempts++) {
      if (level_gen_create_first_room(g, l)) {
        first_room_placed = true;
        break;
      }
    }
    if (! first_room_placed) {
      level_place_first_room_fail++;
      continue;
    }

    //
    // Place the remaining rooms
    //
    level_gen_create_remaining_rooms(g, l);

    //
    // Check we have enough rooms. Add a few more for deeper levels.
    //
    if (std::cmp_less(l->rooms_placed.size(), l->min_room_count)) {
      level_not_enough_rooms++;
      continue;
    }

    //
    // Check we have an exit room
    //
    if (! l->has_placed_ROOM_TYPE_EXIT) {
      level_no_exit_room++;
      continue;
    }

    //
    // Add grass or other blobby features
    //
    level_gen_blob(g, l, CHARMAP_GRASS);
    level_gen_blob(g, l, CHARMAP_FOLIAGE);

    //
    // Success
    //
    return l;
  }

  delete l;
  l = nullptr;

  level_create_fail++;

  if (l->debug) [[unlikely]] {
    LOG("failed to create room with seed: %s", game_seed_name_get(g));
  }

  return nullptr;
}

//
// Has to be a tile you could walk or swim on
//
[[nodiscard]] static auto level_gen_tile_is_traversable(class LevelGen *l, int x, int y) -> bool
{
  switch (l->data[ x ][ y ].c) {
    case CHARMAP_BARREL :        return true;
    case CHARMAP_BRIDGE :        return true;
    case CHARMAP_CORRIDOR :      return true;
    case CHARMAP_DEEP_WATER :    return true;
    case CHARMAP_DOOR_LOCKED :   return true;
    case CHARMAP_DOOR_UNLOCKED : return true;
    case CHARMAP_DOOR_SECRET :   return true; // needed
    case CHARMAP_ENTRANCE :      return true;
    case CHARMAP_EXIT :          return true;
    case CHARMAP_FLOOR :         return true;
    case CHARMAP_FOLIAGE :       return true;
    case CHARMAP_GRASS :         return true;
    case CHARMAP_JOIN :          return true;
    case CHARMAP_KEY :           return true;
    case CHARMAP_MOB1 :          return true;
    case CHARMAP_MOB2 :          return true;
    case CHARMAP_MONST1 :        return true;
    case CHARMAP_MONST2 :        return true;
    case CHARMAP_TRAP :          return true;
    case CHARMAP_TREASURE :      return true;
    case CHARMAP_WATER :         return true;
    default :                    return false;
  }
}

//
// Get rid of tiles that go nowhere
//
[[nodiscard]] static auto level_gen_trim_dead_tiles(Gamep g, class LevelGen *l) -> bool
{
  TRACE();

  bool did_something = false;

  for (int y = 1; y < MAP_HEIGHT - 1; y++) {
    for (int x = 1; x < MAP_WIDTH - 1; x++) {
      switch (l->data[ x ][ y ].c) {
        case CHARMAP_CORRIDOR :
        case CHARMAP_TRAP :
        case CHARMAP_DOOR_UNLOCKED :
        case CHARMAP_DOOR_LOCKED :
        case CHARMAP_JOIN :
          {
            //
            // Check the door/corridor goes from and to somewhere. Diagonals don't count.
            //
            int walkable_tile = 0;

            walkable_tile += level_gen_tile_is_traversable(l, x - 1, y) ? 1 : 0;
            walkable_tile += level_gen_tile_is_traversable(l, x + 1, y) ? 1 : 0;
            walkable_tile += level_gen_tile_is_traversable(l, x, y - 1) ? 1 : 0;
            walkable_tile += level_gen_tile_is_traversable(l, x, y + 1) ? 1 : 0;

            if (walkable_tile <= 1) {
              l->data[ x ][ y ].c    = CHARMAP_EMPTY;
              l->data[ x ][ y ].room = nullptr;
              did_something          = true;
            }
            break;
          }
        case CHARMAP_FLOOR :
          {
            //
            // Choose 3 not 4 nebs to make the rooms have fewer single random tiles
            // on the edges. Makes it look more formed.
            //
            int empty_tile = 0;

            empty_tile += (l->data[ x - 1 ][ y ].c == CHARMAP_EMPTY) ? 1 : 0;
            empty_tile += (l->data[ x + 1 ][ y ].c == CHARMAP_EMPTY) ? 1 : 0;
            empty_tile += (l->data[ x ][ y - 1 ].c == CHARMAP_EMPTY) ? 1 : 0;
            empty_tile += (l->data[ x ][ y + 1 ].c == CHARMAP_EMPTY) ? 1 : 0;

            if (empty_tile >= 3) {
              l->data[ x ][ y ].c    = CHARMAP_EMPTY;
              l->data[ x ][ y ].room = nullptr;
              did_something          = true;
            }
            break;
          }
      }
    }
  }
  return did_something;
}

//
// Keep track of which room is connected to another via a door
//
static void level_gen_scan_connected_rooms(class LevelGen *l)
{
  TRACE();

  for (int y = 1; y < MAP_HEIGHT - 1; y++) {
    for (int x = 1; x < MAP_WIDTH - 1; x++) {
      switch (l->data[ x ][ y ].c) {
        case CHARMAP_JOIN :
          {
            {
              auto *room_a = l->data[ x - 1 ][ y ].room;
              auto *room_b = l->data[ x + 1 ][ y ].room;

              if ((room_a != nullptr) && (room_b != nullptr)) {
                if (room_a < room_b) {
                  std::pair< class Room *, class Room * > const conn(room_a, room_b);
                  l->rooms_connected[ conn ] = true;
                } else {
                  std::pair< class Room *, class Room * > const conn(room_b, room_a);
                  l->rooms_connected[ conn ] = true;
                }
              }
            }

            {
              auto *room_a = l->data[ x ][ y + 1 ].room;
              auto *room_b = l->data[ x ][ y - 1 ].room;

              if ((room_a != nullptr) && (room_b != nullptr)) {
                if (room_a < room_b) {
                  std::pair< class Room *, class Room * > const conn(room_a, room_b);
                  l->rooms_connected[ conn ] = true;
                } else {
                  std::pair< class Room *, class Room * > const conn(room_b, room_a);
                  l->rooms_connected[ conn ] = true;
                }
              }
            }

            break;
          }
      }
    }
  }
}

//
// Add corridors between rooms that are not connected
// Shortest distance is 2, which is ". ."
//
static void level_gen_connect_adjacent_rooms_with_distance_and_chance(class LevelGen *l, int dist, int chance)
{
  TRACE();

  const std::initializer_list< bpoint > directions = {bpoint(-1, 0), bpoint(1, 0), bpoint(0, -1), bpoint(0, 1)};

  for (int y = dist; y < MAP_HEIGHT - dist - 1; y++) {
    for (int x = dist; x < MAP_WIDTH - dist - 1; x++) {
      switch (l->data[ x ][ y ].c) {
        case CHARMAP_JOIN :
          {
            //
            // Decrease the chance of connecting leaf rooms so we don't get too many
            //
            if (d100() > chance) {
              continue;
            }

            for (auto direction : directions) {

              //
              // Bridge or corridor?
              //
              bool bridge_candidate = false;

              //
              // Check there is nothing in the way
              //
              bool has_clear_path = true;
              for (auto d = 1; d < dist; d++) {
                bpoint const adj(x + (direction.x * d), y + (direction.y * d));
                switch (l->data[ adj.x ][ adj.y ].c) {
                  case CHARMAP_WATER :
                  case CHARMAP_DEEP_WATER :
                  case CHARMAP_CHASM :
                  case CHARMAP_LAVA :
                  case CHARMAP_EMPTY :      break;
                  default :                 has_clear_path = false; break;
                }
                if (! has_clear_path) {
                  break;
                }

                //
                // No bridges next to bridges
                //
                // Also only have a bridge if there is some hazard to cross
                //
                for (auto delta : directions) {
                  switch (l->data[ adj.x + delta.x ][ adj.y + delta.y ].c) {
                    case CHARMAP_WATER :
                    case CHARMAP_DEEP_WATER :
                    case CHARMAP_CHASM : //
                      bridge_candidate = true;
                      break;
                    case CHARMAP_BRIDGE : //
                      has_clear_path = false;
                      break;
                    default : break;
                  }
                }
                if (! has_clear_path) {
                  break;
                }
              }

              if (! has_clear_path) {
                continue;
              }

              //
              // Check there is a room at the end of the blank space
              //
              bpoint const dest(x + (direction.x * dist), y + (direction.y * dist));
              if (l->data[ dest.x ][ dest.y ].c != CHARMAP_FLOOR) {
                continue;
              }

              auto *room_a = l->data[ x ][ y ].room;
              auto *room_b = l->data[ dest.x ][ dest.y ].room;

              //
              // Check the two rooms at either end of the corridor are different.
              //
              if (room_a == room_b) {
                continue;
              }

              //
              // Keep connections to the start room minimal
              //
              if (room_a->room_type == ROOM_TYPE_ENTRANCE) {
                continue;
              }
              if (room_b->room_type == ROOM_TYPE_ENTRANCE) {
                continue;
              }

              std::pair< class Room *, class Room * > conn;
              if ((room_a != nullptr) && (room_b != nullptr)) {
                if (room_a < room_b) {
                  conn.first  = room_a;
                  conn.second = room_b;
                } else {
                  conn.first  = room_b;
                  conn.second = room_a;
                }
              }

              //
              // If the rooms are not connected, then join them via a corridor
              //
              if (! l->rooms_connected.contains(conn)) {
                l->rooms_adj_connected++;
                l->rooms_connected[ conn ] = true;

                auto fill_char = CHARMAP_CORRIDOR;
                if (d100() < MAX_LEVEL_GEN_MIN_BRIDGE_LEN_CHANCE) {
                  fill_char = CHARMAP_BRIDGE;
                }

                for (auto d = 1; d < dist; d++) {
                  bpoint const adj(x + (direction.x * d), y + (direction.y * d));

                  if (bridge_candidate) {
                    l->data[ adj.x ][ adj.y ].c = CHARMAP_BRIDGE;
                  } else {
                    if (dist > MAX_LEVEL_GEN_MIN_BRIDGE_LEN) {
                      l->data[ adj.x ][ adj.y ].c = fill_char;
                    } else {
                      l->data[ adj.x ][ adj.y ].c = CHARMAP_CORRIDOR;
                    }
                  }

                  l->data[ adj.x ][ adj.y ].room = room_a;
                }
              }
            }

            break;
          }
      }
    }
  }
}

//
// Add corridors between rooms that are not connected
// Shortest distance is 2, which is ". ."
//
static void level_gen_connect_adjacent_rooms(Gamep g, class LevelGen *l)
{
  TRACE();

  const std::initializer_list< std::pair< int, int > > dists = {
      std::pair(2 /* corridor length */, 80 /* percentage chance of occuring */),
      std::pair(3 /* corridor length */, 80 /* percentage chance of occuring */),
      std::pair(4 /* corridor length */, 80 /* percentage chance of occuring */),
      std::pair(5 /* corridor length */, 80 /* percentage chance of occuring */),
      std::pair(6 /* corridor length */, 80 /* percentage chance of occuring */),
      std::pair(7 /* corridor length */, 80 /* percentage chance of occuring */),
      std::pair(8 /* corridor length */, 80 /* percentage chance of occuring */),
      std::pair(9 /* corridor length */, 80 /* percentage chance of occuring */),
      std::pair(10 /* corridor length */, 40 /* percentage chance of occuring */),
      std::pair(11 /* corridor length */, 40 /* percentage chance of occuring */),
      std::pair(12 /* corridor length */, 40 /* percentage chance of occuring */),
      std::pair(13 /* corridor length */, 40 /* percentage chance of occuring */),
      std::pair(14 /* corridor length */, 40 /* percentage chance of occuring */),
      std::pair(15 /* corridor length */, 40 /* percentage chance of occuring */),
      std::pair(16 /* corridor length */, 40 /* percentage chance of occuring */),
      std::pair(17 /* corridor length */, 40 /* percentage chance of occuring */),
      std::pair(18 /* corridor length */, 40 /* percentage chance of occuring */),
      std::pair(19 /* corridor length */, 40 /* percentage chance of occuring */),
      std::pair(20 /* corridor length */, 40 /* percentage chance of occuring */),
  };
  for (auto d : dists) {
    level_gen_connect_adjacent_rooms_with_distance_and_chance(l, d.first, d.second);
  }
}

//
// Make bridges dramatic by adding chasms around them
//
static void level_gen_add_chasms_around_bridges(class LevelGen *l)
{
  TRACE();

  char const bridge_type = CHARMAP_CHASM;

  //
  // Create the bridge
  //
  for (int y = 1; y < MAP_HEIGHT - 1; y++) {
    for (int x = 1; x < MAP_WIDTH - 1; x++) {
      switch (l->data[ x ][ y ].c) {
        case CHARMAP_BRIDGE :
          {
            if (l->data[ x - 1 ][ y ].c == CHARMAP_EMPTY) {
              l->data[ x - 1 ][ y ].c = bridge_type;
            }
            if (l->data[ x + 1 ][ y ].c == CHARMAP_EMPTY) {
              l->data[ x + 1 ][ y ].c = bridge_type;
            }
            if (l->data[ x ][ y - 1 ].c == CHARMAP_EMPTY) {
              l->data[ x ][ y - 1 ].c = bridge_type;
            }
            if (l->data[ x ][ y + 1 ].c == CHARMAP_EMPTY) {
              l->data[ x ][ y + 1 ].c = bridge_type;
            }
            break;
          }
      }
    }
  }
}

//
// Grow lakes, chasms etc...
//
static void level_gen_grow_hazards(class LevelGen *l)
{
  TRACE();

  char changed[ MAP_WIDTH ][ MAP_HEIGHT ] = {};

  for (int y = 1; y < MAP_HEIGHT - 1; y++) {
    for (int x = 1; x < MAP_WIDTH - 1; x++) {
      auto c = l->data[ x ][ y ].c;

      switch (c) {
        case CHARMAP_CHASM :
        case CHARMAP_LAVA :
        case CHARMAP_WATER :
          if (d100() < 50) {
            continue;
          }

          if (l->data[ x - 1 ][ y - 1 ].c == CHARMAP_EMPTY) {
            if (d100() < 50) {
              changed[ x - 1 ][ y - 1 ] = c;
            }
          }
          if (l->data[ x - 1 ][ y + 1 ].c == CHARMAP_EMPTY) {
            if (d100() < 50) {
              changed[ x - 1 ][ y + 1 ] = c;
            }
          }
          if (l->data[ x + 1 ][ y - 1 ].c == CHARMAP_EMPTY) {
            if (d100() < 50) {
              changed[ x + 1 ][ y - 1 ] = c;
            }
          }
          if (l->data[ x + 1 ][ y + 1 ].c == CHARMAP_EMPTY) {
            if (d100() < 50) {
              changed[ x + 1 ][ y + 1 ] = c;
            }
          }
          if (l->data[ x - 1 ][ y ].c == CHARMAP_EMPTY) {
            if (d100() < 50) {
              changed[ x - 1 ][ y ] = c;
            }
          }
          if (l->data[ x + 1 ][ y ].c == CHARMAP_EMPTY) {
            if (d100() < 50) {
              changed[ x + 1 ][ y ] = c;
            }
          }
          if (l->data[ x ][ y - 1 ].c == CHARMAP_EMPTY) {
            if (d100() < 50) {
              changed[ x ][ y - 1 ] = c;
            }
          }
          if (l->data[ x ][ y + 1 ].c == CHARMAP_EMPTY) {
            if (d100() < 50) {
              changed[ x ][ y + 1 ] = c;
            }
          }
          break;
      }
    }
  }

  for (int y = 0; y < MAP_HEIGHT; y++) {
    for (int x = 0; x < MAP_WIDTH; x++) {
      auto c = changed[ x ][ y ];
      if (c != 0) {
        l->data[ x ][ y ].c = c;
      }
    }
  }
}

//
// Grow island of safety
//
static void level_gen_grow_islands(class LevelGen *l)
{
  TRACE();

  char changed[ MAP_WIDTH ][ MAP_HEIGHT ] = {};

  for (int y = 1; y < MAP_HEIGHT - 1; y++) {
    for (int x = 1; x < MAP_WIDTH - 1; x++) {
      auto c = l->data[ x ][ y ].c;

      switch (c) {
        case CHARMAP_DIRT :

          if (l->data[ x - 1 ][ y - 1 ].c == CHARMAP_WATER) {
            if (d100() < 50) {
              changed[ x - 1 ][ y - 1 ] = c;
            }
          }
          if (l->data[ x - 1 ][ y + 1 ].c == CHARMAP_WATER) {
            if (d100() < 50) {
              changed[ x - 1 ][ y + 1 ] = c;
            }
          }
          if (l->data[ x + 1 ][ y - 1 ].c == CHARMAP_WATER) {
            if (d100() < 50) {
              changed[ x + 1 ][ y - 1 ] = c;
            }
          }
          if (l->data[ x + 1 ][ y + 1 ].c == CHARMAP_WATER) {
            if (d100() < 50) {
              changed[ x + 1 ][ y + 1 ] = c;
            }
          }
          if (l->data[ x - 1 ][ y ].c == CHARMAP_WATER) {
            if (d100() < 50) {
              changed[ x - 1 ][ y ] = c;
            }
          }
          if (l->data[ x + 1 ][ y ].c == CHARMAP_WATER) {
            if (d100() < 50) {
              changed[ x + 1 ][ y ] = c;
            }
          }
          if (l->data[ x ][ y - 1 ].c == CHARMAP_WATER) {
            if (d100() < 50) {
              changed[ x ][ y - 1 ] = c;
            }
          }
          if (l->data[ x ][ y + 1 ].c == CHARMAP_WATER) {
            if (d100() < 50) {
              changed[ x ][ y + 1 ] = c;
            }
          }
          break;
      }
    }
  }

  for (int y = 0; y < MAP_HEIGHT; y++) {
    for (int x = 0; x < MAP_WIDTH; x++) {
      auto c = changed[ x ][ y ];
      if (c != 0) {
        l->data[ x ][ y ].c = c;
      }
    }
  }
}

//
// Add islands of safety
//
static void level_gen_add_islands(class LevelGen *l)
{
  TRACE();

  char changed[ MAP_WIDTH ][ MAP_HEIGHT ] = {};

  for (int y = 1; y < MAP_HEIGHT - 1; y++) {
    for (int x = 1; x < MAP_WIDTH - 1; x++) {
      auto c = l->data[ x ][ y ].c;

      switch (c) {
        case CHARMAP_CHASM :
        case CHARMAP_LAVA :
        case CHARMAP_WATER :
          if (d100() < 95) {
            continue;
          }

          if (/* top left  */ l->data[ x - 1 ][ y - 1 ].c == c &&
              /* bot right */ l->data[ x - 1 ][ y + 1 ].c == c &&
              /* top right */ l->data[ x + 1 ][ y - 1 ].c == c &&
              /* bot right */ l->data[ x + 1 ][ y + 1 ].c == c &&
              /* left      */ l->data[ x - 1 ][ y ].c == c &&
              /* right     */ l->data[ x + 1 ][ y ].c == c &&
              /* top       */ l->data[ x ][ y - 1 ].c == c &&
              /* bot       */ l->data[ x ][ y + 1 ].c == c) {
            changed[ x ][ y ] = CHARMAP_DIRT;
          }
          break;
      }
    }
  }

  for (int y = 1; y < MAP_HEIGHT - 1; y++) {
    for (int x = 1; x < MAP_WIDTH - 1; x++) {
      auto c = changed[ x ][ y ];
      if (c != 0) {
        l->data[ x ][ y ].c = c;
      }
    }
  }
}

//
// Make bridges dramatic by adding chasms around them
//
static void level_gen_add_walls_around_rooms(class LevelGen *l)
{
  TRACE();

  for (int y = 1; y < MAP_HEIGHT - 1; y++) {
    for (int x = 1; x < MAP_WIDTH - 1; x++) {
      auto c = l->data[ x ][ y ].c;
      switch (c) {
        case CHARMAP_WATER :
        case CHARMAP_DEEP_WATER :
        case CHARMAP_LAVA :
        case CHARMAP_CHASM :
        case CHARMAP_CHASM_50 :
        case CHARMAP_EMPTY :
        case CHARMAP_DOOR_LOCKED :
        case CHARMAP_DOOR_SECRET :
        case CHARMAP_DOOR_UNLOCKED :
        case CHARMAP_GRASS :
        case CHARMAP_WALL :
        case CHARMAP_BARREL :
        case CHARMAP_BRAZIER :
        case CHARMAP_BRIDGE :
        case CHARMAP_ENTRANCE :
        case CHARMAP_EXIT :
        case CHARMAP_FOLIAGE :
        case CHARMAP_KEY :
        case CHARMAP_MOB1 :
        case CHARMAP_MOB2 :
        case CHARMAP_MONST1 :
        case CHARMAP_MONST2 :
        case CHARMAP_PILLAR :
        case CHARMAP_TELEPORT :
        case CHARMAP_TRAP :
        case CHARMAP_TREASURE :      break;
        case CHARMAP_CORRIDOR :
        case CHARMAP_FLOOR :
        case CHARMAP_FLOOR_50 :
        case CHARMAP_JOIN :
          if (l->data[ x - 1 ][ y - 1 ].c == CHARMAP_EMPTY) {
            l->data[ x - 1 ][ y - 1 ].c = CHARMAP_WALL;
          }
          if (l->data[ x - 1 ][ y + 1 ].c == CHARMAP_EMPTY) {
            l->data[ x - 1 ][ y + 1 ].c = CHARMAP_WALL;
          }
          if (l->data[ x + 1 ][ y - 1 ].c == CHARMAP_EMPTY) {
            l->data[ x + 1 ][ y - 1 ].c = CHARMAP_WALL;
          }
          if (l->data[ x + 1 ][ y + 1 ].c == CHARMAP_EMPTY) {
            l->data[ x + 1 ][ y + 1 ].c = CHARMAP_WALL;
          }
          if (l->data[ x - 1 ][ y ].c == CHARMAP_EMPTY) {
            l->data[ x - 1 ][ y ].c = CHARMAP_WALL;
          }
          if (l->data[ x + 1 ][ y ].c == CHARMAP_EMPTY) {
            l->data[ x + 1 ][ y ].c = CHARMAP_WALL;
          }
          if (l->data[ x ][ y - 1 ].c == CHARMAP_EMPTY) {
            l->data[ x ][ y - 1 ].c = CHARMAP_WALL;
          }
          if (l->data[ x ][ y + 1 ].c == CHARMAP_EMPTY) {
            l->data[ x ][ y + 1 ].c = CHARMAP_WALL;
          }
          break;
      }
    }
  }
}

//
// Is this tile in the entrance?
//
auto level_gen_is_room_entrance(Gamep g, class LevelGen *l, int x, int y) -> bool
{
  TRACE();

  if (l == nullptr) { // tests
    return false;
  }

  if (is_oob(x, y)) [[unlikely]] {
    return false;
  }

  auto *r = l->data[ x ][ y ].room;
  return (r != nullptr) && (l->room_entrance == r);
}

//
// Is this tile in the entrance?
//
auto level_gen_is_room_entrance(Gamep g, class LevelGen *l, bpoint at) -> bool { return level_gen_is_room_entrance(g, l, at.x, at.y); }

//
// Hide secret doors
//
static void level_gen_add_foliage_around_secret_doors(Gamep g, class LevelGen *l)
{
  TRACE();

  for (int y = 2; y < MAP_HEIGHT - 2; y++) {
    for (int x = 2; x < MAP_WIDTH - 2; x++) {
      auto c = l->data[ x ][ y ].c;
      switch (c) {
        case CHARMAP_WATER :
        case CHARMAP_DEEP_WATER :
        case CHARMAP_LAVA :
        case CHARMAP_CHASM_50 :
        case CHARMAP_EMPTY :
        case CHARMAP_WALL :
        case CHARMAP_BRAZIER :
        case CHARMAP_BRIDGE :
        case CHARMAP_CORRIDOR :
        case CHARMAP_ENTRANCE :
        case CHARMAP_EXIT :
        case CHARMAP_FLOOR :
        case CHARMAP_FLOOR_50 :
        case CHARMAP_FOLIAGE :
        case CHARMAP_GRASS :
        case CHARMAP_JOIN :
        case CHARMAP_MOB1 :
        case CHARMAP_MOB2 :
        case CHARMAP_DOOR_UNLOCKED :
        case CHARMAP_MONST1 :
        case CHARMAP_MONST2 :        break;
        case CHARMAP_DOOR_SECRET :
          for (int dy = -2; dy <= 2; dy++) {
            for (int dx = -2; dx <= 2; dx++) {
              auto d = l->data[ x - dx ][ y - dy ].c;
              if ((d == CHARMAP_EMPTY) || (d == CHARMAP_FLOOR)) {
                if (d100() < LEVEL_GEN_CHANCE_OF_FOLIAGE_AROUND_DOOR_SECRET) {
                  l->data[ x - dx ][ y - dy ].c = CHARMAP_FOLIAGE;
                }
              }
            }
          }
          break;
        case CHARMAP_TRAP :
          for (int dy = -2; dy <= 2; dy++) {
            for (int dx = -2; dx <= 2; dx++) {
              auto d = l->data[ x - dx ][ y - dy ].c;
              if ((d == CHARMAP_EMPTY) || (d == CHARMAP_FLOOR)) {
                if (d100() < LEVEL_GEN_CHANCE_OF_FOLIAGE_AROUND_TRAPS) {
                  l->data[ x - dx ][ y - dy ].c = CHARMAP_FOLIAGE;
                }
              }
            }
          }
          break;
        case CHARMAP_PILLAR :
        case CHARMAP_BARREL :
        case CHARMAP_TREASURE :
          //
          // No foliage in starting rooms. Makes it harder to see
          //
          if (level_gen_is_room_entrance(g, l, x, y)) {
            continue;
          }

          for (int dy = -1; dy <= 1; dy++) {
            for (int dx = -1; dx <= 1; dx++) {
              auto d = l->data[ x - dx ][ y - dy ].c;
              if ((d == CHARMAP_EMPTY) || (d == CHARMAP_FLOOR)) {
                if (d100() < LEVEL_GEN_CHANCE_OF_FOLIAGE_AROUND_ITEMS) {
                  l->data[ x - dx ][ y - dy ].c = CHARMAP_FOLIAGE;
                }
              }
            }
          }
          break;
        case CHARMAP_DOOR_LOCKED :
          //
          // No foliage in starting rooms. Makes it harder to see
          //
          if (level_gen_is_room_entrance(g, l, x, y)) {
            continue;
          }

          for (int dy = -2; dy <= 2; dy++) {
            for (int dx = -2; dx <= 2; dx++) {
              auto d = l->data[ x - dx ][ y - dy ].c;
              if ((d == CHARMAP_EMPTY) || (d == CHARMAP_FLOOR)) {
                if (d100() < LEVEL_GEN_CHANCE_OF_FOLIAGE_AROUND_DOOR_LOCKED) {
                  l->data[ x - dx ][ y - dy ].c = CHARMAP_FOLIAGE;
                }
              }
            }
          }
          break;
        case CHARMAP_CHASM :
          //
          // No foliage in starting rooms. Makes it harder to see
          //
          if (level_gen_is_room_entrance(g, l, x, y)) {
            continue;
          }

          for (int dy = -1; dy <= 1; dy++) {
            for (int dx = -1; dx <= 1; dx++) {
              auto d = l->data[ x - dx ][ y - dy ].c;
              if ((d == CHARMAP_EMPTY) || (d == CHARMAP_FLOOR)) {
                if (d100() < LEVEL_GEN_CHANCE_OF_FOLIAGE_AROUND_CHASMS) {
                  l->data[ x - dx ][ y - dy ].c = CHARMAP_FOLIAGE;
                }
              }
            }
          }
          break;
        case CHARMAP_TELEPORT :
          for (int dy = -1; dy <= 1; dy++) {
            for (int dx = -1; dx <= 1; dx++) {
              auto d = l->data[ x - dx ][ y - dy ].c;
              if (d100() < LEVEL_GEN_CHANCE_OF_FOLIAGE_AROUND_TELEPORTS) {
                if ((d == CHARMAP_EMPTY) || (d == CHARMAP_FLOOR)) {
                  l->data[ x - dx ][ y - dy ].c = CHARMAP_FOLIAGE;
                }
              }
            }
          }
          break;
        case CHARMAP_KEY :
          for (int dy = -2; dy <= 2; dy++) {
            for (int dx = -2; dx <= 2; dx++) {
              auto d = l->data[ x - dx ][ y - dy ].c;
              if (d100() < LEVEL_GEN_CHANCE_OF_FOLIAGE_AROUND_KEYS) {
                if ((d == CHARMAP_EMPTY) || (d == CHARMAP_FLOOR)) {
                  l->data[ x - dx ][ y - dy ].c = CHARMAP_FOLIAGE;
                }
              }
            }
          }
          break;
      }
    }
  }
}

//
// e.g. chasm next to water
//
[[nodiscard]] static auto level_gen_remove_water_conflicts(class LevelGen *l) -> bool
{
  TRACE();

  bool did_something = false;

  for (int y = 1; y < MAP_HEIGHT - 1; y++) {
    for (int x = 1; x < MAP_WIDTH - 1; x++) {
      auto c = l->data[ x ][ y ].c;
      if (c == CHARMAP_WATER) {
        if ((l->data[ x - 1 ][ y - 1 ].c == CHARMAP_LAVA) || l->data[ x - 1 ][ y - 1 ].c == CHARMAP_CHASM) {
          l->data[ x - 1 ][ y - 1 ].c = CHARMAP_WATER;
          did_something               = true;
        }
        if ((l->data[ x - 1 ][ y + 1 ].c == CHARMAP_LAVA) || l->data[ x - 1 ][ y + 1 ].c == CHARMAP_CHASM) {
          l->data[ x - 1 ][ y + 1 ].c = CHARMAP_WATER;
          did_something               = true;
        }
        if ((l->data[ x + 1 ][ y - 1 ].c == CHARMAP_LAVA) || l->data[ x + 1 ][ y - 1 ].c == CHARMAP_CHASM) {
          l->data[ x + 1 ][ y - 1 ].c = CHARMAP_WATER;
          did_something               = true;
        }
        if ((l->data[ x + 1 ][ y + 1 ].c == CHARMAP_LAVA) || l->data[ x + 1 ][ y + 1 ].c == CHARMAP_CHASM) {
          l->data[ x + 1 ][ y + 1 ].c = CHARMAP_WATER;
          did_something               = true;
        }
        if ((l->data[ x - 1 ][ y ].c == CHARMAP_LAVA) || l->data[ x - 1 ][ y ].c == CHARMAP_CHASM) {
          l->data[ x - 1 ][ y ].c = CHARMAP_WATER;
          did_something           = true;
        }
        if ((l->data[ x + 1 ][ y ].c == CHARMAP_LAVA) || l->data[ x + 1 ][ y ].c == CHARMAP_CHASM) {
          l->data[ x + 1 ][ y ].c = CHARMAP_WATER;
          did_something           = true;
        }
        if ((l->data[ x ][ y - 1 ].c == CHARMAP_LAVA) || l->data[ x ][ y - 1 ].c == CHARMAP_CHASM) {
          l->data[ x ][ y - 1 ].c = CHARMAP_WATER;
          did_something           = true;
        }
        if ((l->data[ x ][ y + 1 ].c == CHARMAP_LAVA) || l->data[ x ][ y + 1 ].c == CHARMAP_CHASM) {
          l->data[ x ][ y + 1 ].c = CHARMAP_WATER;
          did_something           = true;
        }
      }
    }
  }
  return did_something;
}

//
// e.g. chasm next to lava
//
[[nodiscard]] static auto level_gen_remove_lava_conflicts(class LevelGen *l) -> bool
{
  TRACE();

  bool did_something = false;

  for (int y = 1; y < MAP_HEIGHT - 1; y++) {
    for (int x = 1; x < MAP_WIDTH - 1; x++) {
      auto c = l->data[ x ][ y ].c;
      if (c == CHARMAP_LAVA) {
        if ((l->data[ x - 1 ][ y - 1 ].c == CHARMAP_WATER) || l->data[ x - 1 ][ y - 1 ].c == CHARMAP_CHASM) {
          l->data[ x - 1 ][ y - 1 ].c = CHARMAP_LAVA;
          did_something               = true;
        }
        if ((l->data[ x - 1 ][ y + 1 ].c == CHARMAP_WATER) || l->data[ x - 1 ][ y + 1 ].c == CHARMAP_CHASM) {
          l->data[ x - 1 ][ y + 1 ].c = CHARMAP_LAVA;
          did_something               = true;
        }
        if ((l->data[ x + 1 ][ y - 1 ].c == CHARMAP_WATER) || l->data[ x + 1 ][ y - 1 ].c == CHARMAP_CHASM) {
          l->data[ x + 1 ][ y - 1 ].c = CHARMAP_LAVA;
          did_something               = true;
        }
        if ((l->data[ x + 1 ][ y + 1 ].c == CHARMAP_WATER) || l->data[ x + 1 ][ y + 1 ].c == CHARMAP_CHASM) {
          l->data[ x + 1 ][ y + 1 ].c = CHARMAP_LAVA;
          did_something               = true;
        }
        if ((l->data[ x - 1 ][ y ].c == CHARMAP_WATER) || l->data[ x - 1 ][ y ].c == CHARMAP_CHASM) {
          l->data[ x - 1 ][ y ].c = CHARMAP_LAVA;
          did_something           = true;
        }
        if ((l->data[ x + 1 ][ y ].c == CHARMAP_WATER) || l->data[ x + 1 ][ y ].c == CHARMAP_CHASM) {
          l->data[ x + 1 ][ y ].c = CHARMAP_LAVA;
          did_something           = true;
        }
        if ((l->data[ x ][ y - 1 ].c == CHARMAP_WATER) || l->data[ x ][ y - 1 ].c == CHARMAP_CHASM) {
          l->data[ x ][ y - 1 ].c = CHARMAP_LAVA;
          did_something           = true;
        }
        if ((l->data[ x ][ y + 1 ].c == CHARMAP_WATER) || l->data[ x ][ y + 1 ].c == CHARMAP_CHASM) {
          l->data[ x ][ y + 1 ].c = CHARMAP_LAVA;
          did_something           = true;
        }
      }
    }
  }
  return did_something;
}

//
// e.g. water next to chasm
//
[[nodiscard]] static auto level_gen_remove_chasm_conflicts(class LevelGen *l) -> bool
{
  TRACE();

  bool did_something = false;

  for (int y = 1; y < MAP_HEIGHT - 1; y++) {
    for (int x = 1; x < MAP_WIDTH - 1; x++) {
      auto c = l->data[ x ][ y ].c;
      if (c == CHARMAP_CHASM) {
        if ((l->data[ x - 1 ][ y - 1 ].c == CHARMAP_WATER) || l->data[ x - 1 ][ y - 1 ].c == CHARMAP_LAVA) {
          l->data[ x - 1 ][ y - 1 ].c = CHARMAP_CHASM;
          did_something               = true;
        }
        if ((l->data[ x - 1 ][ y + 1 ].c == CHARMAP_WATER) || l->data[ x - 1 ][ y + 1 ].c == CHARMAP_LAVA) {
          l->data[ x - 1 ][ y + 1 ].c = CHARMAP_CHASM;
          did_something               = true;
        }
        if ((l->data[ x + 1 ][ y - 1 ].c == CHARMAP_WATER) || l->data[ x + 1 ][ y - 1 ].c == CHARMAP_LAVA) {
          l->data[ x + 1 ][ y - 1 ].c = CHARMAP_CHASM;
          did_something               = true;
        }
        if ((l->data[ x + 1 ][ y + 1 ].c == CHARMAP_WATER) || l->data[ x + 1 ][ y + 1 ].c == CHARMAP_LAVA) {
          l->data[ x + 1 ][ y + 1 ].c = CHARMAP_CHASM;
          did_something               = true;
        }
        if ((l->data[ x - 1 ][ y ].c == CHARMAP_WATER) || l->data[ x - 1 ][ y ].c == CHARMAP_LAVA) {
          l->data[ x - 1 ][ y ].c = CHARMAP_CHASM;
          did_something           = true;
        }
        if ((l->data[ x + 1 ][ y ].c == CHARMAP_WATER) || l->data[ x + 1 ][ y ].c == CHARMAP_LAVA) {
          l->data[ x + 1 ][ y ].c = CHARMAP_CHASM;
          did_something           = true;
        }
        if ((l->data[ x ][ y - 1 ].c == CHARMAP_WATER) || l->data[ x ][ y - 1 ].c == CHARMAP_LAVA) {
          l->data[ x ][ y - 1 ].c = CHARMAP_CHASM;
          did_something           = true;
        }
        if ((l->data[ x ][ y + 1 ].c == CHARMAP_WATER) || l->data[ x ][ y + 1 ].c == CHARMAP_LAVA) {
          l->data[ x ][ y + 1 ].c = CHARMAP_CHASM;
          did_something           = true;
        }
      }
    }
  }
  return did_something;
}

//
// Remove tiles that do not go together, like water next to lava
//
static void level_gen_remove_conflicting_tiles(Gamep g, class LevelGen *l)
{
  TRACE();

  while (level_gen_remove_water_conflicts(l)) {
    ;
  }
  while (level_gen_remove_lava_conflicts(l)) {
    ;
  }
  while (level_gen_remove_chasm_conflicts(l)) {
    ;
  }
}

//
// Change water to deep water if possible
//
static void level_gen_create_deep_water(class LevelGen *l)
{
  TRACE();

  char changed[ MAP_WIDTH ][ MAP_HEIGHT ] = {};

  for (int y = 1; y < MAP_HEIGHT - 1; y++) {
    for (int x = 1; x < MAP_WIDTH - 1; x++) {
      auto c = l->data[ x ][ y ].c;

      switch (c) {
        case CHARMAP_WATER :
          if (d100() < 10) {
            continue;
          }

          if (/* top left  */ l->data[ x - 1 ][ y - 1 ].c == c &&
              /* bot right */ l->data[ x - 1 ][ y + 1 ].c == c &&
              /* top right */ l->data[ x + 1 ][ y - 1 ].c == c &&
              /* bot right */ l->data[ x + 1 ][ y + 1 ].c == c &&
              /* left      */ l->data[ x - 1 ][ y ].c == c &&
              /* right     */ l->data[ x + 1 ][ y ].c == c &&
              /* top       */ l->data[ x ][ y - 1 ].c == c &&
              /* bot       */ l->data[ x ][ y + 1 ].c == c) {
            changed[ x ][ y ] = CHARMAP_DEEP_WATER;
          }
          break;
      }
    }
  }

  for (int y = 1; y < MAP_HEIGHT - 1; y++) {
    for (int x = 1; x < MAP_WIDTH - 1; x++) {
      auto c = changed[ x ][ y ];
      if (c != 0) {
        l->data[ x ][ y ].c = c;
      }
    }
  }
}

//
// See what's on the level
//
static void level_gen_count_items(class LevelGen *l)
{
  TRACE();

  l->info.monst_count            = 0;
  l->info.monst_group_easy_count = 0;
  l->info.monst_group_hard_count = 0;
  l->info.treasure_count         = 0;
  l->info.teleport_count         = 0;
  l->info.door_locked_count      = 0;
  l->info.key_count              = 0;

  for (int y = 1; y < MAP_HEIGHT - 1; y++) {
    for (int x = 1; x < MAP_WIDTH - 1; x++) {
      auto c = l->data[ x ][ y ].c;

      switch (c) {
        case CHARMAP_MONST1 :
          l->info.monst_count++;
          l->info.monst_group_easy_count++;
          break;
        case CHARMAP_MONST2 :
          l->info.monst_count++;
          l->info.monst_group_hard_count++;
          break;
        case CHARMAP_TREASURE : //
          l->info.treasure_count++;
          break;
        case CHARMAP_TELEPORT :
          //
          // Must have > 1 teleport
          //
          l->info.teleport_count++;
          break;
        case CHARMAP_DOOR_LOCKED :
          //
          // Must have > 1 key
          //
          l->info.door_locked_count++;
          break;
        case CHARMAP_KEY :
          //
          // Must have > 1 key
          //
          l->info.key_count++;
          break;
      }
    }
  }
}

//
// Try to add some more content
//
static void level_gen_add_missing_monsts_and_treasure(class LevelGen *l, int nmonst, int ntreasure)
{
  TRACE();

  auto border = LEVEL_GEN_BORDER_FOR_ITEM_PLACEMENT;

  //
  // Place all the monsters we can
  //
  while (nmonst-- > 0) {
    auto x = PCG_RANDOM_RANGE(border, MAP_WIDTH - border);
    auto y = PCG_RANDOM_RANGE(border, MAP_HEIGHT - border);
    auto c = l->data[ x ][ y ].c;

    switch (c) {
      case CHARMAP_FLOOR :
        auto *r = l->data[ x ][ y ].room;
        if ((r != nullptr) && (l->room_entrance == r)) {
          continue;
        }

        if (d100() < 90) {
          l->data[ x ][ y ].c = CHARMAP_MONST1;
        } else {
          l->data[ x ][ y ].c = CHARMAP_MONST2;
        }
        l->info.monst_count++;
    }
  }

  //
  // Place all the treasure we can
  //
  while (ntreasure-- > 0) {
    auto x = PCG_RANDOM_RANGE(border, MAP_WIDTH - border);
    auto y = PCG_RANDOM_RANGE(border, MAP_HEIGHT - border);
    auto c = l->data[ x ][ y ].c;

    switch (c) {
      case CHARMAP_FLOOR :
        auto *r = l->data[ x ][ y ].room;
        if ((r != nullptr) && (l->room_entrance == r)) {
          continue;
        }

        l->data[ x ][ y ].c = CHARMAP_TREASURE;
        l->info.treasure_count++;
    }
  }
}

//
// For secret doors, need to add a corresponding key
//
static void level_gen_add_missing_keys_do(class LevelGen *l)
{
  TRACE();

  for (int y = 1; y < MAP_HEIGHT - 1; y++) {
    for (int x = 1; x < MAP_WIDTH - 1; x++) {
      auto *r = l->data[ x ][ y ].room;

      if ((r != nullptr) && ((r->flags & ROOM_FLAG_LOCKED) != 0U)) {
        continue;
      }
      if ((r != nullptr) && (l->room_entrance == r)) {
        continue;
      }
      if ((r != nullptr) && (l->room_exit == r)) {
        continue;
      }

      auto tries  = MAX_LEVEL_GEN_PLACE_MISSING_KEY_TRIES;
      auto border = LEVEL_GEN_BORDER_FOR_ITEM_PLACEMENT;

      while (tries-- > 0) {
        auto kx = PCG_RANDOM_RANGE(border, MAP_WIDTH - border);
        auto ky = PCG_RANDOM_RANGE(border, MAP_HEIGHT - border);
        auto c  = l->data[ kx ][ ky ].c;
        if (c == CHARMAP_FLOOR) {
          l->data[ kx ][ ky ].c = CHARMAP_KEY;
          return;
        }
      }
    }
  }
}

//
// For secret doors, need to add a corresponding key
//
static void level_gen_add_missing_keys(Gamep g, class LevelGen *l)
{
  TRACE();

  int tries = l->info.door_locked_count;
  tries -= l->info.key_count;

  while (tries-- > 0) {
    level_gen_add_missing_keys_do(l);
  }
}

//
// If too many keys, remove one
//
static void level_gen_remove_additional_keys_do(class LevelGen *l)
{
  TRACE();

  std::vector< bpoint > cands;

  //
  // Find floor tiles with floor space around them, candidates for placing items
  //
  for (int y = 1; y < MAP_HEIGHT - 1; y++) {
    for (int x = 1; x < MAP_WIDTH - 1; x++) {
      auto c = l->data[ x ][ y ].c;

      switch (c) {
        case CHARMAP_KEY : cands.push_back(bpoint(x, y)); break;
      }
    }
  }

  if (cands.empty()) {
    return;
  }

  auto cand                     = cands[ PCG_RAND() % cands.size() ];
  l->data[ cand.x ][ cand.y ].c = CHARMAP_FLOOR;
}

//
// For secret doors, need to add a corresponding key
//
static void level_gen_remove_additional_keys(Gamep g, class LevelGen *l)
{
  TRACE();

  int tries = l->info.key_count;
  tries -= l->info.door_locked_count;

  while (tries-- > 0) {
    level_gen_remove_additional_keys_do(l);
  }
}

//
// Try to add some more content
//
static void level_gen_add_missing_monsts_and_treasure(Gamep g, class LevelGen *l)
{
  TRACE();

  auto tries = MAX_LEVEL_GEN_MIN_MONST_PLACE_TRY;
  while (tries-- > 0) {
    int const need_monsts   = MAX_LEVEL_GEN_MIN_MONST_PER_LEVEL - l->info.monst_count;
    int const need_treasure = MAX_LEVEL_GEN_MIN_TREASURE_PER_LEVEL - l->info.treasure_count;

    if ((need_monsts > 0) || (need_treasure > 0)) {
      level_gen_add_missing_monsts_and_treasure(l, need_monsts, need_treasure);
    }
  }
}

//
// Try to add a telport on the main path
//
[[nodiscard]] static auto level_gen_add_missing_teleport_do(class LevelGen *l, const std::vector< bpoint > &cands) -> bool
{
  TRACE();

  //
  // Place an additional teleport
  //
  auto tries = MAX_LEVEL_GEN_PLACE_ADDITIONAL_TELEPORT_TRIES;
  while (tries-- > 0) {
    auto  cand = cands[ PCG_RAND() % cands.size() ];
    auto  x    = cand.x;
    auto  y    = cand.y;
    auto *r    = l->data[ x ][ y ].room;

    if ((r != nullptr) && (l->room_entrance == r)) {
      continue;
    }
    if ((r != nullptr) && (l->room_exit == r)) {
      continue;
    }
    if (l->info.on_path_entrance_to_exit[ x ][ y ] == 0U) {
      continue;
    }

    l->data[ x ][ y ].c = CHARMAP_TELEPORT;
    return true;
  }

  return false;
}

//
// Try to add some additional teleports if needed
//
static void level_gen_add_missing_teleports(Gamep g, class LevelGen *l)
{
  TRACE();

  if (l->info.teleport_count == 0) {
    return;
  }

  std::vector< bpoint > cands;
  int                   reachable_teleports = 0;

  //
  // Find floor tiles with floor space around them, candidates for placing items
  //
  for (int y = 1; y < MAP_HEIGHT - 1; y++) {
    for (int x = 1; x < MAP_WIDTH - 1; x++) {
      auto c = l->data[ x ][ y ].c;

      //
      // Only place telports on tiles between the entrance and exit
      //
      if (l->info.on_path_entrance_to_exit[ x ][ y ] == 0U) {
        continue;
      }

      switch (c) {
        case CHARMAP_TELEPORT :
          //
          // We need at least one teleport in a reachable room
          //
          reachable_teleports++;
          break;
        case CHARMAP_FLOOR :
          if (/* top left  */ l->data[ x - 1 ][ y - 1 ].c == CHARMAP_FLOOR &&
              /* bot right */ l->data[ x - 1 ][ y + 1 ].c == CHARMAP_FLOOR &&
              /* top right */ l->data[ x + 1 ][ y - 1 ].c == CHARMAP_FLOOR &&
              /* bot right */ l->data[ x + 1 ][ y + 1 ].c == CHARMAP_FLOOR &&
              /* left      */ l->data[ x - 1 ][ y ].c == CHARMAP_FLOOR &&
              /* right     */ l->data[ x + 1 ][ y ].c == CHARMAP_FLOOR &&
              /* top       */ l->data[ x ][ y - 1 ].c == CHARMAP_FLOOR &&
              /* bot       */ l->data[ x ][ y + 1 ].c == CHARMAP_FLOOR) {
            cands.push_back(bpoint(x, y));
          }
          break;
      }
    }
  }

  if (cands.empty()) {
    //
    // Try again, but less restrictive
    //
    for (int y = 1; y < MAP_HEIGHT - 1; y++) {
      for (int x = 1; x < MAP_WIDTH - 1; x++) {
        auto c = l->data[ x ][ y ].c;

        //
        // Only place telports on tiles between the entrance and exit
        //
        if (l->info.on_path_entrance_to_exit[ x ][ y ] == 0U) {
          continue;
        }

        switch (c) {
          case CHARMAP_FLOOR :
            if (/* left      */ l->data[ x - 1 ][ y ].c == CHARMAP_FLOOR &&
                /* right     */ l->data[ x + 1 ][ y ].c == CHARMAP_FLOOR &&
                /* top       */ l->data[ x ][ y - 1 ].c == CHARMAP_FLOOR &&
                /* bot       */ l->data[ x ][ y + 1 ].c == CHARMAP_FLOOR) {
              cands.push_back(bpoint(x, y));
            }
            break;
        }
      }
    }

    if (cands.empty()) {
      //
      // Try again, but even less restrictive
      //
      for (int y = 1; y < MAP_HEIGHT - 1; y++) {
        for (int x = 1; x < MAP_WIDTH - 1; x++) {
          auto c = l->data[ x ][ y ].c;

          //
          // Only place telports on tiles between the entrance and exit
          //
          if (l->info.on_path_entrance_to_exit[ x ][ y ] == 0U) {
            continue;
          }

          switch (c) {
            case CHARMAP_FLOOR : cands.push_back(bpoint(x, y)); break;
          }
        }
      }

      if (cands.empty()) {
        return;
      }
    }
  }

  //
  // Place an additional teleport
  //
  if ((l->info.teleport_count > 0) || (reachable_teleports == 0)) {
    auto tries = MAX_LEVEL_GEN_PLACE_ADDITIONAL_TELEPORT_TRIES;
    while (tries-- > 0) {
      if (level_gen_add_missing_teleport_do(l, cands)) {
        return;
      }
    }
  }

  auto tries = MAX_LEVEL_GEN_PLACE_ADDITIONAL_TELEPORT_TRIES;
  while (tries-- > 0) {
    if (level_gen_add_missing_teleport_do(l, cands)) {
      return;
    }
  }
}

//
// Look for a single room we can add doors at the exits from it
//
static void level_gen_add_doors_do(class LevelGen *l)
{
  TRACE();

  class Room const *r = nullptr;

  //
  // Find a random room.
  //
  auto tries = MAX_LEVEL_GEN_LOOK_FOR_ROOM_TRIES;
  while (tries-- > 0) {
    auto border = LEVEL_GEN_BORDER_FOR_ITEM_PLACEMENT;
    auto x      = PCG_RANDOM_RANGE(border, MAP_WIDTH - border);
    auto y      = PCG_RANDOM_RANGE(border, MAP_HEIGHT - border);
    r           = l->data[ x ][ y ].room;

    if (r == nullptr) {
      continue;
    }
    if ((r->flags & ROOM_FLAG_LOCKED) != 0U) {
      continue;
    }
    if (l->room_entrance == r) {
      continue;
    }
    if (l->room_exit == r) {
      continue;
    }
    break;
  }

  //
  // Look for the tiles for this room.
  //
  for (auto y = 1; y < MAP_HEIGHT - 1; y++) {
    for (auto x = 1; x < MAP_WIDTH - 1; x++) {
      auto *other_room = l->data[ x ][ y ].room;
      if (other_room != r) {
        continue;
      }

      //
      // Only look for join tiles adjacent to some walkable tiles.
      //
      // Rooms which have been extended with corridors are not easily
      // enclosed in doors now.
      //
      switch (l->data[ x ][ y ].c) {
        case CHARMAP_FLOOR :
        case CHARMAP_DIRT :
        case CHARMAP_GRASS :
        case CHARMAP_FOLIAGE :
          for (int dy = -1; dy <= 1; dy++) {
            for (int dx = -1; dx <= 1; dx++) {
              switch (l->data[ x + dx ][ y + dy ].c) {
                case CHARMAP_CORRIDOR :
                case CHARMAP_BRIDGE :   return;
              }
            }
          }
      }
    }
  }

  //
  // Look for the tiles for this room.
  //
  // Only look for join tiles adjacent to some walkable tiles.
  //
  for (auto y = 1; y < MAP_HEIGHT - 1; y++) {
    for (auto x = 1; x < MAP_WIDTH - 1; x++) {
      auto *other_room = l->data[ x ][ y ].room;
      if (other_room != r) {
        continue;
      }

      switch (l->data[ x ][ y ].c) {
        case CHARMAP_FLOOR :
        case CHARMAP_DIRT :
        case CHARMAP_GRASS :
        case CHARMAP_FOLIAGE :
          for (int dy = -1; dy <= 1; dy++) {
            for (int dx = -1; dx <= 1; dx++) {
              switch (l->data[ x + dx ][ y + dy ].c) {
                case CHARMAP_JOIN : l->data[ x + dx ][ y + dy ].c = CHARMAP_DOOR_UNLOCKED; break;
              }
            }
          }
          break;
      }
    }
  }
}

//
// Chance of creating some doors for a room. We always add some doors, and then a chance of more.
//
static void level_gen_add_doors(Gamep g, class LevelGen *l)
{
  TRACE();

  int tries = 0;

  while (tries++ < MAX_LEVEL_GEN_ADD_ROOM_WITH_DOOR_TRIES) {
    level_gen_add_doors_do(l);
  }

  while (d100() < MAX_LEVEL_GEN_ADD_ADDITIONAL_ROOM_WITH_DOOR_TRIES) {
    level_gen_add_doors_do(l);
  }
}

//
// Remove doors that are right next to each other
//
static void level_gen_remove_doors_next_to_each_other(class LevelGen *l)
{
  TRACE();

  for (auto y = 1; y < MAP_HEIGHT - 1; y++) {
    for (auto x = 1; x < MAP_WIDTH - 1; x++) {
      switch (l->data[ x ][ y ].c) {
        case CHARMAP_DOOR_UNLOCKED :
          if (l->data[ x - 1 ][ y ].c == CHARMAP_DOOR_UNLOCKED) {
            l->data[ x - 1 ][ y ].c = CHARMAP_CORRIDOR;
          }
          if (l->data[ x + 1 ][ y ].c == CHARMAP_DOOR_UNLOCKED) {
            l->data[ x + 1 ][ y ].c = CHARMAP_CORRIDOR;
          }
          if (l->data[ x ][ y - 1 ].c == CHARMAP_DOOR_UNLOCKED) {
            l->data[ x ][ y - 1 ].c = CHARMAP_CORRIDOR;
          }
          if (l->data[ x ][ y + 1 ].c == CHARMAP_DOOR_UNLOCKED) {
            l->data[ x ][ y + 1 ].c = CHARMAP_CORRIDOR;
          }
      }
    }
  }
}

//
// Recursive flood fill walkable tiles from the start
//
static void level_gen_mark_tiles_on_path_entrance_to_exit(Gamep g, class LevelGen *l, int x, int y)
{
  //
  // Already walked?
  //
  if (l->walked[ x ][ y ]) {
    return;
  }
  l->walked[ x ][ y ] = true;

  //
  // Mark this tile as on the main path
  //
  l->info.on_path_entrance_to_exit[ x ][ y ] = level_gen_tile_is_traversable(l, x, y) ? 1 : 0;
  if (l->info.on_path_entrance_to_exit[ x ][ y ] == 0U) {
    return;
  }

  if (x > 0) {
    level_gen_mark_tiles_on_path_entrance_to_exit(g, l, x - 1, y);
  }
  if (x < MAP_WIDTH - 1) {
    level_gen_mark_tiles_on_path_entrance_to_exit(g, l, x + 1, y);
  }
  if (y > 0) {
    level_gen_mark_tiles_on_path_entrance_to_exit(g, l, x, y - 1);
  }
  if (y < MAP_HEIGHT - 1) {
    level_gen_mark_tiles_on_path_entrance_to_exit(g, l, x, y + 1);
  }
}

//
// Recursive flood fill walkable tiles from the start
//
static void level_gen_mark_tiles_on_path_entrance_to_exit(Gamep g, class LevelGen *l)
{
  TRACE();

  memset(l->walked, 0, SIZEOF(l->walked));
  memset(l->info.on_path_entrance_to_exit, 0, SIZEOF(l->info.on_path_entrance_to_exit));
  level_gen_mark_tiles_on_path_entrance_to_exit(g, l, l->info.entrance_at.x, l->info.entrance_at.y);
}

//
// Can this bridge extension reach something useful?
//
[[nodiscard]] static auto level_gen_extend_bridge_direction_check(class LevelGen *l, int x, int y, int lr, int ud) -> bool
{
  for (;;) {
    x += lr;
    y += ud;

    if (is_oob(x, y)) [[unlikely]] {
      return false;
    }

    if (l->info.on_path_entrance_to_exit[ x ][ y ] != 0U) {
      return true;
    }
  }
}

//
// Extend a bridge left, right, up down
//
static void level_gen_extend_bridges_do(Gamep g, class LevelGen *l, int x, int y, int lr, int ud)
{
  if (is_oob(x, y)) [[unlikely]] {
    return;
  }

  if (l->debug) [[unlikely]] {
    level_gen_dump(l, "extend bridge");
  }

  //
  // Already walked?
  //
  if (l->walked[ x ][ y ]) {
    return;
  }
  l->walked[ x ][ y ] = true;

  //
  // Keep going until we reach a tile on the main path
  //
  if (l->info.on_path_entrance_to_exit[ x ][ y ] != 0U) {
    return;
  }

  switch (l->data[ x ][ y ].c) {
    case CHARMAP_CHASM_50 :   break;
    case CHARMAP_CORRIDOR :   break;
    case CHARMAP_DEEP_WATER : break;
    case CHARMAP_FLOOR :      break;
    case CHARMAP_FLOOR_50 :   break;
    case CHARMAP_JOIN :       break;
    case CHARMAP_KEY :        break;
    case CHARMAP_MOB1 :       break;
    case CHARMAP_MOB2 :       break;
    case CHARMAP_MONST1 :     break;
    case CHARMAP_MONST2 :     break;
    case CHARMAP_PILLAR :     break;
    case CHARMAP_TELEPORT :   break;
    case CHARMAP_TRAP :       break;
    case CHARMAP_FIRE :       break;
    case CHARMAP_EXIT :       break;
    case CHARMAP_ENTRANCE :   break;
    case CHARMAP_DOOR_LOCKED :
    case CHARMAP_DOOR_SECRET :
    case CHARMAP_DOOR_UNLOCKED :
    case CHARMAP_GRASS :
    case CHARMAP_TREASURE :
    case CHARMAP_FOLIAGE :
    case CHARMAP_BARREL :
    case CHARMAP_BRAZIER :
    case CHARMAP_WALL :
    case CHARMAP_WATER :
    case CHARMAP_LAVA :
    case CHARMAP_BRIDGE :
    case CHARMAP_CHASM :
    case CHARMAP_EMPTY :

      if (l->data[ x ][ y ].c != CHARMAP_BRIDGE) {
        l->data[ x ][ y ].c = CHARMAP_CORRIDOR;
      }

      if (lr > 0) {
        if (x >= 3) {
          level_gen_extend_bridges_do(g, l, x + 1, y, lr, ud);
        }
      } else if (lr < 0) {
        if (x <= MAP_WIDTH - 3) {
          level_gen_extend_bridges_do(g, l, x - 1, y, lr, ud);
        }
      }

      if (ud > 0) {
        if (y >= 3) {
          level_gen_extend_bridges_do(g, l, x, y + 1, lr, ud);
        }
      } else if (ud < 0) {
        if (y <= MAP_HEIGHT - 3) {
          level_gen_extend_bridges_do(g, l, x, y - 1, lr, ud);
        }
      }
      break;
  }
}

//
// Extend bridges that go nowhere - as long as they can go somewhere
//
static void level_gen_extend_bridges(Gamep g, class LevelGen *l, int x, int y, int lr, int ud)
{
  TRACE();

  if (! level_gen_extend_bridge_direction_check(l, x, y, lr, ud)) {
    return;
  }

  level_gen_extend_bridges_do(g, l, x, y, lr, ud);
}

//
// Extend bridges that go nowhere
//
static void level_gen_extend_bridges(Gamep g, class LevelGen *l)
{
  TRACE();

  memset(l->walked, 0, SIZEOF(l->walked));

  //
  // Find bridge tiles not on the main path
  //
  for (int y = 1; y < MAP_HEIGHT - 1; y++) {
    for (int x = 1; x < MAP_WIDTH - 1; x++) {
      if (l->walked[ x ][ y ]) {
        continue;
      }

      if (l->data[ x ][ y ].c != CHARMAP_BRIDGE) {
        continue;
      }

      if (l->info.on_path_entrance_to_exit[ x ][ y ] != 0U) {
        continue;
      }

      //
      // Get the bridge direction
      //
      bool const lr = (l->data[ x - 1 ][ y ].c == CHARMAP_BRIDGE) || //
                      (l->data[ x + 1 ][ y ].c == CHARMAP_BRIDGE);

      bool const ud = (l->data[ x ][ y - 1 ].c == CHARMAP_BRIDGE) || //
                      (l->data[ x ][ y + 1 ].c == CHARMAP_BRIDGE);

      if (lr && ud) {
        continue;
      }

      if (lr) {
        level_gen_extend_bridges(g, l, x, y, 1, 0);
        level_gen_extend_bridges(g, l, x, y, -1, 0);
      }

      if (ud) {
        level_gen_extend_bridges(g, l, x, y, 0, 1);
        level_gen_extend_bridges(g, l, x, y, 0, -1);
      }
    }
  }
}

//
// Convert the level into a string and then populate all the things onto the
// real level, assign tiles etc...
//
[[nodiscard]] static auto level_gen_populate_for_fixed_or_proc_gen_level(Gamep g, class LevelGen *l) -> bool
{
  TRACE();

  auto *v = game_levels_get(g);
  if (v == nullptr) {
    ERR("no levels created");
    return false;
  }

  LevelSelect const *s = &v->level_select;

  auto *level = game_level_get(g, v, l->level_num);
  level_init(g, v, level, l->level_num);

  //
  // Create a string holding all the level chars
  //
  std::string level_string;
  LevelFixed *fixed_level = nullptr;
  Overrides   overrides   = no_overrides;

  if (! g_level_opt.level_name.empty()) {
    //
    // Test level
    //
    fixed_level = level_fixed_find_by_name(g_level_opt.level_name, l->level_num);
    if (fixed_level == nullptr) {
      //
      // Unknown level
      //
      ERR("no fixed level \"%s\" created", g_level_opt.level_name.c_str());
      return false;
    }

    level_string = level_gen_string(l, fixed_level);
    overrides    = fixed_level->overrides;

  } else if (l->level_num == s->level_count - 1) {
    //
    // Final boss level
    //
    fixed_level = level_random_get(LEVEL_TYPE_BOSS);
    if (fixed_level == nullptr) {
      ERR("no fixed boss level \"%u\" created", l->level_num);
      return false;
    }

    level_string = level_gen_string(l, fixed_level);
    overrides    = fixed_level->overrides;
  } else {
    //
    // Procedurally generated level
    //
    level_string = level_gen_string(l);
  }

  if (level_string.empty()) {
    return false;
  }

  //
  // Final count
  //
  level_gen_count_items(l);

  //
  // Copy useful level stats for use in level_populate
  //
  level->info = l->info;

  //
  // Save the fixed level name. Might be useful.
  //
  if (fixed_level != nullptr) {
    level->is_fixed_level = true;
    my_strlcpy(level->info.name, fixed_level->alias.c_str(), sizeof(level->info.name));
  }

  //
  // Create things
  //
  if (! level_populate(g, v, level, l, level_string.c_str(), overrides)) {
    return false;
  }

  //
  // Dump the level to the per thread output log file
  //
  if (g_opt_debug1) {
    level_gen_dump(l);
  }

  //
  // Create joined up tiles
  //
  level_assign_tiles(g, v, level);

  //
  // Create paths to all objects needed
  //
  level_dmap(g, v, level);

  return true;
}

//
// Flood the walkable areas
//
static void level_gen_test_flood(class LevelGen *l)
{
  TRACE();

  //
  // Find floor tiles with floor space around them, candidates for placing items
  //
  for (int y = 1; y < MAP_HEIGHT - 1; y++) {
    for (int x = 1; x < MAP_WIDTH - 1; x++) {
      switch (l->data[ x ][ y ].c) {
        case CHARMAP_ENTRANCE :
        case CHARMAP_EXIT :     break;
        default :
          //
          // Only place telports on tiles between the entrance and exit
          //
          if (l->info.on_path_entrance_to_exit[ x ][ y ] != 0U) {
            l->data[ x ][ y ].c = CHARMAP_CORRIDOR;
          }
          break;
      }
    }
  }
}

//
// Create a level from the current game seed
//
static auto level_gen_create_proc_gen_level(Gamep g, Levelsp v, LevelNum level_num) -> class LevelGen *
{
  TRACE();

  LevelGen *l = level_proc_gen_create_rooms(g, level_num);
  if (l == nullptr) {
    return l;
  }

  //
  // Get rid of tiles that go nowhere
  //
  while (level_gen_trim_dead_tiles(g, l)) {}

  //
  // Keep track of which room is connected to another via a door
  //
  level_gen_scan_connected_rooms(l);

  //
  // Add corridors between rooms that are not connected
  //
  level_gen_connect_adjacent_rooms(g, l);

  //
  // Make bridges dramatic by adding chasms around them
  //
  level_gen_add_chasms_around_bridges(l);
  if (l->debug) [[unlikely]] {
    level_gen_dump(l, "add chasms around bridges");
  }

  //
  // Add islands of safety
  //
  level_gen_add_islands(l);
  if (l->debug) [[unlikely]] {
    level_gen_dump(l, "add islands");
  }

  //
  // Grow lakes, chasms etc...
  //
  level_gen_grow_hazards(l);
  if (l->debug) [[unlikely]] {
    level_gen_dump(l, "grow hazards");
  }

  //
  // Make islands bigger
  //
  level_gen_grow_islands(l);
  if (l->debug) [[unlikely]] {
    level_gen_dump(l, "grown islands");
  }

  //
  // Add some doors
  //
  level_gen_add_doors(g, l);
  if (l->debug) [[unlikely]] {
    level_gen_dump(l, "add doors");
  }

  //
  // Remove some doors!
  //
  level_gen_remove_doors_next_to_each_other(l);
  if (l->debug) [[unlikely]] {
    level_gen_dump(l, "remove doors next to each other");
  }

  //
  // Add fragments before we add walls
  //
  level_gen_add_fragments(g, l);

  //
  // Add walls
  //
  level_gen_add_walls_around_rooms(l);
  if (l->debug) [[unlikely]] {
    level_gen_dump(l, "add walls around rooms");
  }

  //
  // And add again after, in case some can now match
  //
  level_gen_add_fragments(g, l);

  //
  // Add walls again in case a fragment extended the room
  //
  level_gen_add_walls_around_rooms(l);

  //
  // Remove tiles that do not go together, like water next to lava
  //
  level_gen_remove_conflicting_tiles(g, l);

  //
  // Create deep water
  //
  level_gen_create_deep_water(l);

  //
  // Mark walkable tiles prior to adding content; as we want to check teleports are on the main path
  //
  level_gen_mark_tiles_on_path_entrance_to_exit(g, l);

  //
  // Extend dead end bridges
  //
  level_gen_extend_bridges(g, l);

  //
  // See how much we generated
  //
  level_gen_count_items(l);

  //
  // If not enough monsters, add some randomly
  //
  level_gen_add_missing_monsts_and_treasure(g, l);

  //
  // Secret doors need keys
  //
  level_gen_add_missing_keys(g, l);

  //
  // If not enough teleports
  //
  level_gen_add_missing_teleports(g, l);

  //
  // Count the keys
  //
  level_gen_count_items(l);

  //
  // If too many keys, remove some
  //
  level_gen_remove_additional_keys(g, l);

  //
  // Hide doors
  //
  level_gen_add_foliage_around_secret_doors(g, l);

  //
  // Show walkable areas
  //
  if (compiler_unused) {
    level_gen_test_flood(l);
  }

  return l;
}

//
// Create a level (fixed or proc gen) and store in the array of levels
//
static void level_gen_create_fixed_or_proc_gen_level(Gamep g, LevelNum level_num)
{
  TRACE();

  //
  // Per thread stdout name
  //
  g_thread_id = level_num + 1;

  if (level_num >= LEVEL_SELECT_ID) {
    CROAK("this will exceed the number of level bits");
  }

  //
  // We need to create the log files now, even if empty, as if we get a crash we will
  // not be able to call fopen during the segv signal
  //
  redirect_stdout();
  redirect_stderr();

  do {
    auto *v = game_levels_get(g);
    if (v == nullptr) {
      if (! g_level_opt.level_name.empty()) {
        ERR("no levels generate for level %s", g_level_opt.level_name.c_str());
      } else {
        ERR("no levels generate for level num %u", level_num);
      }
      break;
    }

    LevelGen *l = nullptr;

    if (level_gen_is_special_level(v, level_num)) {
      //
      // Fixed level of some kind
      //
      l = level_gen_new_class(g, level_num);
    } else {
      //
      // Procedurally generated levels
      //
      l = level_gen_create_proc_gen_level(g, v, level_num);
    }

    //
    // Check it was created
    //
    if (l == nullptr) {
      if (! g_level_opt.level_name.empty()) {
        ERR("no level generated for level %s", g_level_opt.level_name.c_str());
      } else {
        ERR("no level generated for level num %u", level_num);
      }
      break;
    }

    //
    // Populate the map with things from the level created
    //
    TRACE();
    if (! level_gen_populate_for_fixed_or_proc_gen_level(g, l)) {
      if (! g_level_opt.level_name.empty()) {
        ERR("no level created for level %s", g_level_opt.level_name.c_str());
      } else {
        ERR("no level created for level num %u", level_num);
      }
      break;
    }

    //
    // Final check it worked
    //
    levels_generated[ level_num ] = l;

    TRACE();
    auto *level = game_level_get(g, v, level_num);
    if (level == nullptr) {
      if (! g_level_opt.level_name.empty()) {
        ERR("no level populated for level %s", g_level_opt.level_name.c_str());
      } else {
        ERR("no level populated for level num %u", level_num);
      }
    }
  } while (false);

  close_stdout();
  close_stderr();
}

//
// Create lots of levels
//
void level_gen_create_levels(Gamep g, Levelsp v)
{
  LevelSelect *s           = &v->level_select;
  int const    max_threads = s->level_count;

  //
  // We keep one level free for the grid level
  //
  auto start = time_ms();
  LOG("level generation (max %u)", s->level_count);
  TRACE_INDENT();

  std::vector< std::thread > threads;

  threads.reserve(max_threads);
  for (auto i = 0; i < max_threads; i++) {
    threads.emplace_back(level_gen_create_fixed_or_proc_gen_level, g, i);
  }

  v->is_generating_levels = true;
  for (auto i = 0; i < max_threads; i++) {
    threads[ i ].join();
  }
  v->is_generating_levels = false;

  if (g_opt_debug2) {
    for (auto i = 0; i < max_threads; i++) {
      auto *l = levels_generated[ i ];
      if (l != nullptr) {
        level_gen_dump(l);
      }
    }
  }

  s->create_time = time_ms() - start;
  LOG("level generation completed, took %u ms", s->create_time);
  TRACE_INDENT();

  level_gen_stats_dump(g);
}

void level_gen_test(Gamep g)
{
  TRACE();

  for (auto seed = 0U; seed < LEVEL_TEST_MAX; seed++) {
    game_seed_set(g, seed);

    //
    // Allocate space for the levels
    //
    auto *v = levels_memory_alloc(g);
    (void) game_levels_set(g, v);

    //
    // Generate the maximum number of levels
    //
    LevelSelect *s = &v->level_select;
    s->level_count = LEVEL_SELECT_ID - 1;

    //
    // Create the levels
    //
    level_gen_create_levels(g, v);
    if (AN_ERROR_OCCURRED()) {
      CROAK("failed");
    }

    thing_stats_dump(g, v);

    CON("Created %u levels for dungeon seed %u (took %u ms)", s->level_count, seed, s->create_time);
    LOG("------------------------------------------------------");
    TRACE_INDENT();

    //
    // Free the levels memory
    //
    levels_destroy(g, v);
    if (AN_ERROR_OCCURRED()) {
      CROAK("failed");
    }
  }
}
