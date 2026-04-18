//
// Copyright goblinhack@gmail.com
//

#ifndef MY_LEVEL_HPP
#define MY_LEVEL_HPP

#include "my_bpoint.hpp"
#include "my_game_defs.hpp"
#include "my_globals.hpp"
#include "my_main.hpp"
#include "my_ptrcheck.hpp"
#include "my_string.hpp"
#include "my_thing.hpp"
#include "my_tile.hpp"
#include "my_types.hpp"

#include <vector>

class LevelGen;

enum {
  CURSOR_NONE,
  CURSOR_PATH,
  CURSOR_AT,
};

//
// Used for level selection on the command line
//
using LevelOpt = struct LevelOpt {
  std::string level_name;
  LevelNum    level_num {};
  bool        is_set {};
};

extern LevelOpt g_level_opt;

using LevelInfo = struct LevelInfo {
  //////////////////////////////////////////////////////////////
  // No c++ types can be used here, to allow easy level replay
  //
  // Why C types only ? For large data structures it is visibly
  // faster to malloc and memset versus default construction.
  //////////////////////////////////////////////////////////////

  //
  // For fixed levels
  //
  char name[ MAXSHORTSTR ];

  //
  // Seed used to generated this level
  //
  uint32_t seed_num;

  //
  // Where the player start is
  //
  bpoint entrance_at;

  //
  // Where the exit is
  //
  bpoint exit_at;

  //
  // Is this tile on the main flood fill path from start to exit
  //
  uint8_t on_path_entrance_to_exit[ MAP_WIDTH ][ MAP_HEIGHT ];

  //
  // How many rooms?
  //
  int room_count;

  //
  // How many monsters?
  //
  int monst_count;
  int monst_group_mob_count;
  int monst_group_easy_count;
  int monst_group_hard_count;

  //
  // How much treasure?
  //
  int treasure_count;

  //
  // How many teleports. Needs to be > 1
  //
  int teleport_count;

  //
  // How many locked doors. Must have corresponding key counts.
  //
  int door_locked_count;
  int key_count;

  //
  // How many room fragments we've added
  //
  int fragment_count;

  //////////////////////////////////////////////////////////////
  // No c++ types can be used here, to allow easy level replay
  //////////////////////////////////////////////////////////////
};

using LightPixel = struct LightPixel {
  //
  // The total of all lights intersecting here, which is then scaled into the final light
  //
  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint8_t player_r;
  uint8_t player_g;
  uint8_t player_b;
};

using LightPixels = struct LightPixels {
  LightPixel pixel[ LIGHT_PIXEL ][ LIGHT_PIXEL ];
};

using LightTile = struct LightTile {
  LightPixels pixels;
};

//
// Light map for the level
//
using LightMap = struct LightMap {
  LightTile tile[ MAP_WIDTH ][ MAP_HEIGHT ];
};

using Level = struct Level {
  //////////////////////////////////////////////////////////////
  // No c++ types can be used here, to allow easy level replay
  //
  // Why C types only ? For large data structures it is visibly
  // faster to malloc and memset versus default construction.
  //////////////////////////////////////////////////////////////
  //
  // Where this level is in the bigger map
  //
  LevelNum level_num;
  //
  // The biome for this level
  //
  Biome biome;
  //
  // Level generation info
  //
  LevelInfo info;
  //
  // Where on the level select grid is this level
  //
  bpoint level_select_at;
  //
  // Where to being this level
  //
  bpoint entrance;
  //
  // Where to exit this level
  //
  bpoint exit;
  //
  // Animation timestamps
  //
  uint32_t ts;
  uint32_t last_ts;
  //
  // Used to keep track if we have ran the temperature checks yet this tick.
  //
  uint32_t tick_temperature;
  //
  // This is the next level we would fall onto. It's used to show through chasms.
  //
  LevelNum level_num_next;
  uint8_t  level_num_next_set : 1;
  //
  // Is this a non null level?
  //
  uint8_t is_initialized : 1;
  //
  // Is this level one of the set that ticks every turn?
  //
  uint8_t is_tick_required : 1;
  //
  // Sometimes things will fall into a level beyond what we normally see
  // and we need to ensure things don't end up hovering over a chasm.
  //
  uint8_t is_tick_requested : 1;
  //
  // Has it ticked at least once?
  //
  uint8_t is_tick_has_occurred : 1;
  //
  // Booleans that are set whenever something of this type is created on the level
  // and then cleared at end of tick.
  //
  uint8_t is_tick_end_delay : 1;
  //
  // This is the current level that the player is on.
  //
  uint8_t _is_player_level : 1;
  //
  // Is this a pre-generated level and non procedurally generated.
  //
  uint8_t is_fixed_level : 1;
  //
  // Avoid recursion with temperature events.
  //
  uint8_t is_handling_temperature_changes : 1;
  //
  // Player has entered level
  //
  uint8_t player_has_entered_level : 1;
  //
  // Player has completed level on way or another
  //
  uint8_t player_completed_level_via_exit : 1;
  uint8_t player_fell_out_of_level        : 1;
  //
  // Player can enter this level
  //
  uint8_t player_can_enter_this_level_next : 1;
  //
  // Level needs tiles updated
  //
  uint8_t request_to_update_paths : 1;
  //
  // Free up any things at end of life
  //
  uint8_t request_to_cleanup_things : 1;
  //
  // Update lighting during ticks when the player moves a pixel
  //
  uint8_t request_to_update_per_pixel_visibility : 1;
  //
  // Update lighting during ticks when the player moves a tile
  //
  uint8_t request_to_update_per_tile_visibility : 1;
  //
  // We have to interpolate movement and this indicates that is in progress.
  //
  uint8_t tick_in_progress : 1;
  //
  // Some things like explosions, we want to wait for the explosion to finish before
  // moving to the next tick.
  //
  uint8_t tick_wait_on_anim : 1;
  //
  // Ensure things have finished their move before ending the tick
  //
  uint8_t tick_wait_on_things : 1;
  //
  // Player has moved.
  //
  uint8_t tick_begin_requested : 1;
  uint8_t tick_end_requested   : 1;
  uint8_t tick_ended           : 1;
  //
  // When a tile is destroyed, we need to update adjacent tiles. This limits the update to only changed tiles.
  //
  uint8_t is_modified_tile[ MAP_WIDTH ][ MAP_HEIGHT ];
  //
  // What things are where? Each Id points to a thing structure.
  //
  ThingId thing_id[ MAP_WIDTH ][ MAP_HEIGHT ][ MAP_SLOTS ];
  //
  // Has the player been on this tile?
  //
  uint8_t player_has_walked_tile[ MAP_WIDTH ][ MAP_HEIGHT ];
  //
  // Updated per tick what the player has seen
  //
  uint8_t player_has_seen_tile_cache[ MAP_WIDTH ][ MAP_HEIGHT ];
  //
  // Level flag cache
  //
  uint8_t flag[ MAP_WIDTH ][ MAP_HEIGHT ][ THING_FLAG_ENUM_MAX ];
  //
  // Original character map when the level was generated
  //
  char debug[ MAP_WIDTH ][ MAP_HEIGHT ];
  //////////////////////////////////////////////////////////////
  // No c++ types can be used here, to allow easy level replay
  //////////////////////////////////////////////////////////////
};

using LevelSelectCell = struct LevelSelectCell {
  //
  // If there is a level here
  //
  uint8_t is_set;
  //
  // Ultimate boss level
  //
  uint8_t final_level : 1;
  //
  // Back pointer to the level here, if any
  //
  LevelNum level_num;
};

using LevelSelect = struct LevelSelect {
  //
  // Set when we have populated the levels.
  //
  uint8_t is_populated;

  //
  // How many levels generated
  //
  LevelNum level_count;

  //
  // How many ms to create all the lvels
  //
  uint32_t create_time;

  //
  // Level tiles and grid info
  //
  LevelSelectCell data[ LEVEL_ACROSS ][ LEVEL_DOWN ];

  //
  // This is a reverse mapping from on screen tile to level
  //
  LevelNum tile_to_level[ MAP_WIDTH ][ MAP_HEIGHT ];
};

using Levels = struct Levels {
  //////////////////////////////////////////////////////////////
  // No c++ types can be used here, to allow easy level replay
  //
  // Why C types only ? For large data structures it is visibly
  // faster to malloc and memset versus default construction.
  //////////////////////////////////////////////////////////////
  //
  // Tick increases one per move.
  //
  uint32_t tick;
  //
  // Used to only tick things once per loop, even if they move slot
  //
  uint32_t iter[ MY_ITERS_MAX ];
  //
  // To catch iter nesting
  //
  uint8_t in_iter[ MY_ITERS_MAX ];
  //
  // Increments once per event loop.
  //
  uint32_t frame;
  //
  // When the tick began in ms
  //
  uint32_t frame_begin;
  //
  // Ranges from 0 to 1 when a tick is in progress.
  //
  float time_step;
  float last_time_step;
  //
  // For teleporting and movement.
  //
  uint32_t scroll_speed;
  //
  // For keeping track of how many levels are ticking/done.
  //
  uint32_t level_count;
  uint32_t level_tick_done_count;
  uint32_t level_tick_in_progress_count;
  uint32_t level_tick_request_count;
  //
  // How many levels are ticking
  //
  uint32_t level_ticking_count;
  //
  // Current level being played
  //
  LevelNum level_num;
  //
  // Flag array of all levels. The levels equate to one tile in the level grid.
  //
  Level level[ LEVEL_MAX ];
  //
  // Level lighting
  //
  LightMap light_map;
  //
  // Which levels are where
  //
  LevelSelect level_select;
  //
  // All things structure memory.
  //
  Thing thing_body[ THING_ID_MAX ];
#ifdef ENABLE_PER_THING_MEMORY
  Thingp thing_body_debug[ THING_ID_MAX ];
#endif
  //
  // Space for monster memory
  //
  ThingExt thing_ext[ THING_EXT_MAX ];
  int      thing_ext_count;
  //
  // For lighting memory
  //
  ThingLight thing_light[ THING_LIGHT_MAX ];
  int        thing_light_count;
  //
  // Space for player memory
  //
  ThingPlayer thing_player;
  //
  // The current player.
  //
  ThingId player_id;
  //
  // Used in level selection, this is the current level thing
  //
  ThingId level_select_id;
  //
  // Things described in the rightbar.
  //
  ThingId describe[ THING_DESCRIBE_MAX ];
  uint8_t describe_count;
  //
  // The cursor path highlighted.
  //
  uint8_t cursor[ MAP_WIDTH ][ MAP_HEIGHT ];
  //
  // What the player is currently highlighting.
  //
  bpoint cursor_at;
  //
  // Previous cursor. Used to detect changes.
  //
  bpoint old_cursor_at;
  //
  // Map scroll offset.
  //
  spoint pixel_map_at;
  //
  // Value for the previous zoom level
  //
  spoint pixel_map_at_for_zoom[ MAP_ZOOM_MAX ];
  //
  // Maximum pixel bound
  //
  spoint pixel_max;
  //
  // Levels display bounds
  //
  int minx;
  int miny;
  int maxx;
  int maxy;
  //
  // Player movement state
  //
  PlayerState _player_state;
  //
  // Is the cursor over the map?
  //
  uint8_t cursor_at_valid : 1;
  //
  // Can see the cursor currently? It is hidden when moving with the keyboard.
  //
  uint8_t cursor_visible : 1;
  //
  // Mouse moved?
  //
  uint8_t cursor_moved : 1;
  //
  // Player move request.
  //
  uint8_t requested_move_down  : 1;
  uint8_t requested_move_left  : 1;
  uint8_t requested_move_right : 1;
  uint8_t requested_move_up    : 1;
  uint8_t requested_fire       : 1;
  //
  // Set when something modifies the map and we need to update caches.
  //
  uint8_t is_map_changed : 1;
  //
  // If the player has moved, we need to scroll the map
  //
  uint8_t is_generating_levels : 1;
  //
  // If the player has moved, we need to scroll the map
  //
  uint8_t requested_auto_scroll : 1;
  //
  // For a time period e.g. post teleport, we want to ignore mouse moves until the player is
  // centered once more.
  //
  uint32_t requested_forced_auto_scroll;
  //////////////////////////////////////////////////////////////
  // No c++ types can be used here, to allow easy level replay
  //////////////////////////////////////////////////////////////
};

auto level_iter_begin(Gamep, Levelsp v, int *iter, const char *func, int line) -> bool;
auto level_iter_end(Gamep, Levelsp v, int iter, const char *func, int line) -> bool;

struct MyIter {
  Levelsp     v;
  int         iter;
  const char *func;
  int         line;

  MyIter(Gamep g, Levelsp v, int *iter, const char *func, uint16_t line);
  ~MyIter();
};

#define FOR_ALL_LEVELS(_g_, _v_, _l_)                                                                                                      \
  if ((VERIFY(MTYPE_GAME, _g_)) && (VERIFY(MTYPE_LEVELS, _v_)))                                                                            \
    if (Levelp _l_ = nullptr; true)                                                                                                        \
      for (auto _n_ = 0; _n_ < LEVEL_MAX; _n_++)                                                                                           \
        if (((_l_) = &v->level[ _n_ ]))                                                                                                    \
          if ((_l_)->is_initialized)

#define FOR_ALL_TICKING_LEVELS(_g_, _v_, _l_)                                                                                              \
  if ((VERIFY(MTYPE_GAME, _g_)) && (VERIFY(MTYPE_LEVELS, _v_)))                                                                            \
    if (Levelp _l_ = nullptr; true)                                                                                                        \
      for (auto _n_ = 0; _n_ < LEVEL_MAX; _n_++)                                                                                           \
        if (((_l_) = &v->level[ _n_ ]))                                                                                                    \
          if ((_l_)->is_tick_required)

//
// For all things on the map
//
#define FOR_ALL_MAP_POINTS(_g_, _v_, _l_, _x_, _y_)                                                                                        \
  if ((VERIFY(MTYPE_GAME, _g_)) && (VERIFY(MTYPE_LEVELS, _v_)))                                                                            \
    if ((_l_))                                                                                                                             \
      for ((_x_) = 0; (_x_) < MAP_WIDTH; (_x_)++)                                                                                          \
        for ((_y_) = 0; (_y_) < MAP_HEIGHT; (_y_)++)

//
// For all things on this level
//
// Unsafe here means that if things move around during processing, we could process
// them more than once per loop
//
#define FOR_ALL_THINGS_ON_LEVEL_UNSAFE(_g_, _v_, _l_, _t_)                                                                                 \
  if ((_g_) && (_v_) && (_l_))                                                                                                             \
    for (auto _x_ = 0; _x_ < MAP_WIDTH; _x_++)                                                                                             \
      for (auto _y_ = 0; _y_ < MAP_HEIGHT; _y_++)                                                                                          \
        for (auto _slot_ = 0; _slot_ < MAP_SLOTS; _slot_++)                                                                                \
          if (ThingId _id_ = 0; (_id_ = (_l_)->thing_id[ _x_ ][ _y_ ][ _slot_ ]))                                                          \
            if (Thingp _t_ = nullptr; ((_t_) = thing_find(_g_, _v_, _id_)))

#define FOR_ALL_THINGS_ON_LEVEL(_g_, _v_, _l_, _t_)                                                                                        \
  if ((VERIFY(MTYPE_GAME, _g_)) && (VERIFY(MTYPE_LEVELS, _v_)))                                                                            \
    if ((_l_))                                                                                                                             \
      if (int _iter_index_ = 0; true)                                                                                                      \
        if (MyIter _iter_(_g_, _v_, &_iter_index_, __FUNCTION__, __LINE__); true)                                                          \
          for (auto _x_ = 0; _x_ < MAP_WIDTH; _x_++)                                                                                       \
            for (auto _y_ = 0; _y_ < MAP_HEIGHT; _y_++)                                                                                    \
              for (auto _slot_ = 0; _slot_ < MAP_SLOTS; _slot_++)                                                                          \
                if (ThingId _id_ = 0; (_id_ = (_l_)->thing_id[ _x_ ][ _y_ ][ _slot_ ]))                                                    \
                  if (Thingp _t_ = nullptr; ((_t_) = thing_find(_g_, _v_, _id_)))                                                          \
                    if ((_t_)->iter[ _iter_index_ ] != (_v_)->iter[ _iter_index_ ])                                                        \
                      if ((((_t_)->iter[ _iter_index_ ] = (_v_)->iter[ _iter_index_ ])) || 1)

#define FOR_ALL_THINGS_ON_LEVEL_DEBUG(_g_, _v_, _l_, _t_)                                                                                  \
  if ((VERIFY(MTYPE_GAME, _g_)) && (VERIFY(MTYPE_LEVELS, _v_)))                                                                            \
    if ((_l_))                                                                                                                             \
      if (int _iter_index_ = 0; true)                                                                                                      \
        if (MyIter _iter_(_g_, _v_, &_iter_index_, __FUNCTION__, __LINE__); true)                                                          \
          for (auto _x_ = 0; _x_ < MAP_WIDTH; _x_++)                                                                                       \
            for (auto _y_ = 0; _y_ < MAP_HEIGHT; _y_++)                                                                                    \
              for (auto _slot_ = 0; _slot_ < MAP_SLOTS; _slot_++)                                                                          \
                if (ThingId _id_ = 0; (_id_ = (_l_)->thing_id[ _x_ ][ _y_ ][ _slot_ ]))                                                    \
                  if (Thingp _t_ = nullptr; ((_t_) = thing_find(_g_, _v_, _id_)))                                                          \
                    if (thing_debug(_g_, _v_, _l_, _t_, _iter_index_))                                                                     \
                      if ((_t_)->iter[ _iter_index_ ] != (_v_)->iter[ _iter_index_ ])                                                      \
                        if ((((_t_)->iter[ _iter_index_ ] = (_v_)->iter[ _iter_index_ ])) || 1)

#define FOR_ALL_GROUP_THINGS_ON_LEVEL_UNSAFE(_g_, _v_, _l_, _t_, _group_id_)                                                               \
  FOR_ALL_THINGS_ON_LEVEL_UNSAFE(_g_, _v_, _l_, _t_)                                                                                       \
  if ((_t_)->group_id == (_group_id_))

#define FOR_ALL_GROUP_THINGS_ON_LEVEL(_g_, _v_, _l_, _t_, _group_id_)                                                                      \
  FOR_ALL_THINGS_ON_LEVEL(_g_, _v_, _l_, _t_)                                                                                              \
  if ((_t_)->group_id == (_group_id_))

#define FOR_ALL_GROUP_THINGS_ON_LEVEL_DEBUG(_g_, _v_, _l_, _t_, _group_id_)                                                                \
  FOR_ALL_THINGS_ON_LEVEL_DEBUG(_g_, _v_, _l_, _t_)                                                                                        \
  if ((_t_)->group_id == (_group_id_))

#define FOR_ALL_THINGS_AT_UNSAFE(_g_, _v_, _l_, _t_, _p_)                                                                                  \
  if ((_g_) && (_v_) && (_l_))                                                                                                             \
    if (bpoint _at_ = make_bpoint(_p_); true)                                                                                              \
      if (! is_oob(_at_))                                                                                                                  \
        for (auto _slot_ = 0; _slot_ < MAP_SLOTS; _slot_++)                                                                                \
          if (ThingId _id_ = 0; (_id_ = (_l_)->thing_id[ _at_.x ][ _at_.y ][ _slot_ ]))                                                    \
            if (Thingp _t_ = nullptr; ((_t_) = thing_find(_g_, _v_, _id_)))

#define FOR_ALL_THINGS_AT(_g_, _v_, _l_, _t_, _p_)                                                                                         \
  if ((VERIFY(MTYPE_GAME, _g_)) && (VERIFY(MTYPE_LEVELS, _v_)))                                                                            \
    if ((_l_))                                                                                                                             \
      if (bpoint _at_ = make_bpoint(_p_); true)                                                                                            \
        if (! is_oob(_at_))                                                                                                                \
          if (int _iter_index_ = 0; true)                                                                                                  \
            if (MyIter _iter_(_g_, _v_, &_iter_index_, __FUNCTION__, __LINE__); true)                                                      \
              for (auto _slot_ = 0; _slot_ < MAP_SLOTS; _slot_++)                                                                          \
                if (ThingId _id_ = 0; (_id_ = (_l_)->thing_id[ _at_.x ][ _at_.y ][ _slot_ ]))                                              \
                  if (Thingp _t_ = nullptr; ((_t_) = thing_find(_g_, _v_, _id_)))                                                          \
                    if ((_t_)->iter[ _iter_index_ ] != (_v_)->iter[ _iter_index_ ])                                                        \
                      if ((((_t_)->iter[ _iter_index_ ] = (_v_)->iter[ _iter_index_ ])) || 1)

enum {
  CHANCE_VVV_UNLIKELY = 1,
  CHANCE_VV_UNLIKELY  = 10,
  CHANCE_V_UNLIKELY   = 100,
  CHANCE_UNLIKELY     = 1000,
  CHANCE_NORMAL       = 10000,
};

using RoomFlags = enum RoomFlags_ {
  NO_FLAGS = 0,
  //
  // Dungeon entrance room
  //
  ROOM_FLAG_ENTRANCE = 1 << 0,
  //
  // Normal room
  //
  ROOM_FLAG_NORMAL = 1 << 1,
  //
  // Dungeon exit room
  //
  ROOM_FLAG_EXIT = 1 << 2,
  //
  // Secret rooms which need a key to enter
  //
  ROOM_FLAG_LOCKED = 1 << 3,
  //
  // Secret rooms
  //
  ROOM_FLAG_SECRET = 1 << 4,
  //
  // Key room
  //
  ROOM_FLAG_HAS_KEY = 1 << 5,
  //
  // Sanity check on exits that we have no tiles in the same column or row
  // as an exit; it makes it harder to join rooms together
  //
  ROOM_FLAG_CHECK_EXITS = 1 << 6,
  //
  // Room types
  //
};

static auto const ROOM_FLAG_ALL = // newline
    ROOM_FLAG_ENTRANCE |          // newline
    ROOM_FLAG_NORMAL |            // newline
    ROOM_FLAG_EXIT |              // newline
    ROOM_FLAG_LOCKED |            // newline
    ROOM_FLAG_SECRET |            // newline
    ROOM_FLAG_HAS_KEY;            // newline

#define ROOM_TYPE_FIRST ROOM_TYPE_ENTRANCE

enum {
  //
  // A test level
  //
  LEVEL_TEST_FLAG = 1,
};

// begin sort marker1 {
[[nodiscard]] auto      fragment_add(Gamep g, int chance, const char *file, int line, ...) -> bool;
[[nodiscard]] auto      fragment_alt_add(Gamep g, int chance, uint32_t flags, const char *file, int line, ...) -> bool;
[[nodiscard]] auto      level_alive(Gamep g, Levelsp v, Levelp l, ThingFlag f, bpoint p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto      level_alive(Gamep g, Levelsp v, Levelp l, ThingFlag f, Thingp me) -> Thingp;
[[nodiscard]] auto      level_change(Gamep g, Levelsp v, LevelNum level_num) -> Levelp;
[[nodiscard]] auto      level_count(Gamep g, Levelsp v, Levelp l, ThingFlag f, bpoint p) -> int;
[[nodiscard]] auto      level_count(Gamep g, Levelsp v, Levelp l, ThingFlag f, Thingp t) -> int;
[[nodiscard]] auto      level_cursor_describe_add(Gamep g, Levelsp v, Thingp t) -> bool;
[[nodiscard]] auto      level_cursor_describe_remove(Gamep g, Levelsp v, Thingp t) -> bool;
[[nodiscard]] auto      level_cursor_is_valid(Gamep g, Levelsp v) -> bool;
[[nodiscard]] auto      level_cursor_path_size(Gamep g) -> int;
[[nodiscard]] auto      level_find_all(Gamep g, Levelsp v, Levelp l, ThingFlag f, bpoint p) -> std::vector< Thingp >;
[[nodiscard]] auto      level_find_all(Gamep g, Levelsp v, Levelp l, ThingFlag f) -> std::vector< Thingp >;
[[nodiscard]] auto      level_flag_cached(Gamep g, Levelsp v, Levelp l, ThingFlag f, bpoint p) -> bool;
[[nodiscard]] auto      level_flag(Gamep g, Levelsp v, Levelp l, ThingFlag f, bpoint p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto      level_flag(Gamep g, Levelsp v, Levelp l, ThingFlag f, Thingp me) -> Thingp;
[[nodiscard]] auto      level_gen_is_room_entrance(Gamep g, class LevelGen *lg, bpoint at) -> bool;
[[nodiscard]] auto      level_gen_is_room_entrance(Gamep g, class LevelGen *lg, int x, int y) -> bool;
[[nodiscard]] auto      level_gen_is_room_exit(Gamep g, class LevelGen *lg, bpoint at) -> bool;
[[nodiscard]] auto      level_gen_is_room_exit(Gamep g, class LevelGen *lg, int x, int y) -> bool;
[[nodiscard]] auto      level_gen_is_room_has_key(Gamep g, class LevelGen *lg, bpoint at) -> bool;
[[nodiscard]] auto      level_gen_is_room_has_key(Gamep g, class LevelGen *lg, int x, int y) -> bool;
[[nodiscard]] auto      level_gen_is_room_locked(Gamep g, class LevelGen *lg, bpoint at) -> bool;
[[nodiscard]] auto      level_gen_is_room_locked(Gamep g, class LevelGen *lg, int x, int y) -> bool;
[[nodiscard]] auto      level_gen_is_room_secret(Gamep g, class LevelGen *lg, bpoint at) -> bool;
[[nodiscard]] auto      level_gen_is_room_secret(Gamep g, class LevelGen *lg, int x, int y) -> bool;
[[nodiscard]] auto      level_get_thing_id_at(Gamep g, Levelsp v, Levelp l, const bpoint &p, int slot) -> ThingId;
[[nodiscard]] auto      level_is_level_select(Gamep g, Levelsp v, Levelp l) -> bool;
[[nodiscard]] auto      level_is_player_level(Gamep g, Levelsp v, Levelp l) -> bool;
[[nodiscard]] auto      level_is_same_obj_type_at(Gamep g, Levelsp v, Levelp l, const bpoint &p, Tpp tp) -> bool;
[[nodiscard]] auto      level_light_blocker_at(Gamep g, Levelsp v, Levelp l, const bpoint &pov, Thingp me) -> Thingp;
[[nodiscard]] auto      level_match_contents(Gamep g, Levelsp v, Levelp l, Testp t, int w, int h, const char *expected) -> bool;
[[nodiscard]] auto      level_open(Gamep g, Levelsp v, Levelp l, ThingFlag f, bpoint p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto      level_open(Gamep g, Levelsp v, Levelp l, ThingFlag f, Thingp me) -> Thingp;
[[nodiscard]] auto      level_populate_thing_id_at(Gamep g, Levelsp v, Levelp l, const bpoint &p, int slot, ThingId id) -> bool;
[[nodiscard]] auto      level_request_to_cleanup_things(Gamep g, Levelsp v, Levelp l) -> bool;
[[nodiscard]] auto      level_select_calculate_next_level_down(Gamep g, Levelsp v, Levelp l, bool redo = false) -> Levelp;
[[nodiscard]] auto      level_select_get_level_at_tile_coords(Gamep g, Levelsp v, bpoint p) -> Levelp;
[[nodiscard]] auto      level_select_get_level(Gamep, Levelsp, Levelp l, const bpoint &) -> Levelp;
[[nodiscard]] auto      level_select_get_next_level_down(Gamep g, Levelsp v, Levelp l) -> Levelp;
[[nodiscard]] auto      level_select_get(Gamep g, Levelsp v, bpoint p) -> LevelSelectCell *;
[[nodiscard]] auto      level_select_is_oob(bpoint p) -> bool;
[[nodiscard]] auto      level_select_is_oob(int x, int y) -> bool;
[[nodiscard]] auto      level_string(Gamep g, Levelsp v, Levelp l, int w, int h) -> std::string;
[[nodiscard]] auto      level_tick_begin_is_requested_cached(Gamep g, Levelsp v, Levelp l) -> bool;
[[nodiscard]] auto      level_tick_begin_requested_cached(Gamep g, Levelsp v, Levelp l, const char *why) -> bool;
[[nodiscard]] auto      level_tick_is_in_progress(Gamep g, Levelsp v, Levelp l) -> bool;
[[nodiscard]] auto      level_to_biome(Gamep g, Levelsp v, Levelp l) -> Biome;
[[nodiscard]] LevelType level_type(LevelNum level_num);
[[nodiscard]] bool      level_is_boss_level(Gamep g, Levelsp v, Levelp l);
[[nodiscard]] auto      levels_memory_alloc(Gamep g) -> Levelsp;
[[nodiscard]] auto      levels_thing_count(Gamep g, Levelsp v) -> int;
[[nodiscard]] auto      levels_thing_ext_count(Gamep g, Levelsp v) -> int;
[[nodiscard]] auto      player_state_to_string(PlayerState state) -> std::string;
[[nodiscard]] auto      player_state(Gamep g, Levelsp v) -> PlayerState;
[[nodiscard]] auto      thing_level_select(Gamep g) -> Thingp;
[[nodiscard]] auto      to_string(Gamep g, Levelsp v, Levelp l) -> std::string;
// end sort marker1 }

// begin sort marker2 {
void fragment_alts_fini(Gamep g);
void fragments_fini(Gamep g);
void fragments_init(Gamep g);
void game_debug_info(Gamep g);
void level_anim(Gamep g, Levelsp v, Levelp l);
void level_assign_tiles(Gamep g, Levelsp v, Levelp l);
void level_blit(Gamep g);
void level_bounds_set(Gamep g, Levelsp v, Levelp l);
void level_count_items(Gamep g, Levelsp v, Levelp l);
void level_cursor_copy_mouse_path_to_player(Gamep g, Levelsp v, Levelp l);
void level_cursor_copy_path_to_player(Gamep g, Levelsp v, Levelp l, std::vector< bpoint > &move_path);
void level_cursor_describe_clear(Gamep g, Levelsp v);
void level_cursor_describe_update(Gamep g, Levelsp v);
void level_cursor_describe(Gamep g, Levelsp v, Levelp l);
void level_cursor_path_recreate(Gamep g, Levelsp v, Levelp l);
void level_cursor_path_reset(Gamep g, Levelsp v, Levelp l);
void level_cursor_path_reset(Gamep g);
void level_cursor_set(Gamep g, Levelsp v, bpoint p);
void level_debug(Gamep g, Levelsp v, Levelp l);
void level_destroy(Gamep g, Levelsp v, Levelp l);
void level_display_obj(Gamep, Levelsp, Levelp l, const bpoint &, Tpp, Thingp);
void level_display(Gamep g, Levelsp v, Levelp l);
void level_dmap(Gamep g, Levelsp v, Levelp l);
void level_dump(Gamep g, Levelsp v, Levelp l, int w = MAP_WIDTH, int h = MAP_HEIGHT);
void level_dump(Gamep g, Levelsp v, Levelp l, int w, int h, const std::string &s);
void level_enter(Gamep g, Levelsp v, Levelp l, Thingp /*player*/);
void level_finalize(Gamep g, Levelsp v, Levelp l);
void level_forced_auto_scroll(Gamep g, Levelsp v, Levelp l);
void level_gen_create_levels(Gamep g, Levelsp v);
void level_gen_stats_dump(Gamep g);
void level_gen_test(Gamep g);
void level_group_things(Gamep g, Levelsp v, Levelp l, Thingp t);
void level_has_seen_update(Gamep g, Levelsp v, Levelp l);
void level_init(Gamep g, Levelsp v, Levelp l, LevelNum n);
void level_is_completed_by_player_exiting(Gamep g, Levelsp v, Levelp l);
void level_is_completed_by_player_falling(Gamep g, Levelsp v, Levelp l);
void level_is_player_level_set(Gamep g, Levelsp v, Levelp l);
void level_is_player_level_unset(Gamep g, Levelsp v, Levelp l);
void level_light_calculate_all(Gamep g, Levelsp v, Levelp l);
void level_light_per_pixel_lighting(Gamep g, Levelsp v, Levelp l, Thingp t, bpoint pov, bpoint p);
void level_light_precalculate(Gamep g);
void level_lunge_precalculate(Gamep g);
void level_minimaps_update(Gamep g, Levelsp v, Levelp l);
void level_mouse_position_get(Gamep g, Levelsp v, Levelp l);
void level_raycast_fini();
void level_raycast_init(Gamep g);
void level_raycast(Gamep g, Levelsp v, Levelp l, FboEnum fbo);
void level_request_to_cleanup_things_set(Gamep g, Levelsp v, Levelp l);
void level_request_to_cleanup_things_unset(Gamep g, Levelsp v, Levelp l);
void level_scroll_delta(Gamep g, Levelsp v, Levelp l, const spoint &delta);
void level_scroll_to_focus(Gamep g, Levelsp v, Levelp l);
void level_scroll_warp_to_focus(Gamep g, Levelsp v, Levelp l);
void level_select_assign_levels_to_grid(Gamep g, Levelsp v);
void level_select_destroy(Gamep g, Levelsp v, Levelp l);
void level_select_grid_of_empty_levels(Gamep g);
void level_select_mouse_down(Gamep g, Levelsp v, Levelp l);
void level_select_mouse_motion(Gamep g, Levelsp v, Levelp l);
void level_select_rightbar_show_contents(Gamep g, Levelsp v, Levelp l, WidPopup *parent);
void level_select_test(Gamep g);
void level_select_update_grid_tiles(Gamep g, Levelsp v);
void level_thing_pair_temperature_handle(Gamep g, Levelsp v, Levelp l, Thingp a, Thingp b);
void level_tick_begin_temperature(Gamep g, Levelsp v, Levelp l);
void level_tick_chasm(Gamep g, Levelsp v, Levelp l);
void level_tick_end_temperature(Gamep g, Levelsp v, Levelp l);
void level_tick_explosion(Gamep g, Levelsp v, Levelp l);
void level_tick_teleport(Gamep g, Levelsp v, Levelp l);
void level_tick_water(Gamep g, Levelsp v, Levelp l);
void level_update_flags(Gamep g, Levelsp v, Levelp l);
void level_update_paths_set(Gamep g, Levelsp v, Levelp l, const bpoint &p);
void level_update_paths(Gamep g, Levelsp v, Levelp l);
void level_update_tiles(Gamep g, Levelsp v, Levelp l);
void level_update_visibility(Gamep g, Levelsp v, Levelp l);
void level_verify(Gamep g, Levelsp v, Levelp l);
void level_water_display(Gamep, Levelsp, Levelp l, const bpoint &, int, int16_t, int16_t, int16_t, int16_t);
void level_water_tick(Gamep, Levelsp, Levelp);
void level_water_update(Gamep, Levelsp, Levelp);
void levels_destroy(Gamep g, Levelsp v);
void levels_dmap(Gamep g, Levelsp v);
void levels_finalize(Gamep g, Levelsp v);
void levels_fini(Gamep g);
void levels_fixed(Gamep g);
void levels_init(Gamep g);
void levels_test(Gamep g);
void levels_tick(Gamep g, Levelsp v);
void player_state_change(Gamep g, Levelsp v, Levelp l, PlayerState new_state);
void room_add(Gamep g, int chance, int flags, const char *file, int line, ...);
void rooms_blend1(Gamep g);
void rooms_blend2(Gamep g);
void rooms_chunky(Gamep g);
void rooms_circular(Gamep g);
void rooms_cross_sym(Gamep g);
void rooms_cross(Gamep g);
void rooms_fini(Gamep g);
void rooms_init(Gamep g);
void rooms_large(Gamep g);
void rooms_long(Gamep g);
void rooms_medium(Gamep g);
void rooms_prefab_entrance(Gamep g);
void rooms_prefab_exit(Gamep g);
void rooms_prefab_key(Gamep g);
void rooms_prefab_locked(Gamep g);
void rooms_prefab_secret(Gamep g);
void rooms_prefab(Gamep g);
void rooms_small(Gamep g);
void rooms_tall(Gamep g);
void rooms_test(Gamep g);
void thing_lunge_modify_position(Gamep g, Levelsp v, Levelp l, Thingp me, spoint &tl, spoint &br);
// end sort marker2 }

struct FovContext;

using level_fov_can_see_callback_t = void (*)(const struct FovContext &, const bpoint &p);

using FovContext = struct FovContext {
  Gamep                        g                        = {};
  Levelsp                      v                        = {};
  Levelp                       l                        = {};
  Thingp                       me                       = {};
  bpoint                       pov                      = {};
  bool                         light_walls              = false;
  color                        light_color              = {};
  float                        light_strength_in_pixels = {};
  spoint                       thing_at_in_pixels       = {};
  int                          max_radius               = {};
  FovMap                      *can_see_tile             = {};
  AgeMap                      *has_seen_tile            = {};
  level_fov_can_see_callback_t can_see_callback         = {};
};

void level_fov(const FovContext & /*ctx*/);

// begin sort marker3 {
[[nodiscard]] auto level_is_able_to_collect_items(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_able_to_collect_keys(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_able_to_crush_grass(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_able_to_fall_repeatedly(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_able_to_fall_sound(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_able_to_fall(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_able_to_fire_projectiles(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_able_to_jump(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_able_to_lunge(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_able_to_move_diagonally(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_able_to_move_through_walls(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_able_to_open(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_able_to_shove(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_able_to_walk_through_walls(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_animated_can_hflip(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_animated_no_dir(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_animated_sync_first(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_animated(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_attackable_by_monst(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_attackable_by_player(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_barrel(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_biome_bogland(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_biome_dungeon(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_biome_graveyard(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_biome_nethervoid(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_biome_underhell(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_blit_bg(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_blit_centered(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_blit_flush_per_line(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_blit_hit_outline_w_black_inside(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_blit_hit_outline_w_invis_inside(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_blit_if_has_seen(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_blit_obscures(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_blit_on_ground(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_blit_outlined(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_blit_per_pixel_lighting(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_blit_shown_in_chasms(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_blit_shown_in_overlay(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_blit_square_outlined(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_blit_when_obscured_faded(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_blit_when_obscured_outline(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_blitzhound(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_border(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_brazier(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_bridge(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_broken_on_death(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_burnable(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_chasm(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_collectable(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_collision_circle_large(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_collision_circle_small(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_collision_detection_enabled(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_collision_hit_all_on_tile(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_collision_hit_first_on_tile(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_collision_square(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_combustible(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_corpse_on_death(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_corridor(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_crushable(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_cursor_path_hazard(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_cursor_path_none(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_cursor_path_warning(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_cursor_path(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_cursor(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_damage_capped(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_dead_on_collision(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_dead_on_shoving(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_deep_water(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_described_cursor(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_dirt(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_dmap(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_door_locked(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_door_secret(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_door_unlocked(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_dungeon_entrance(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_effect_attack(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_effect_blood(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_effect(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_entrance(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_ethereal(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_exit(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_explosion(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_extinguished_on_death(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_fire(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_fireball(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_flesh(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_floating(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_floor(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_flying(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_foliage(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_gaseous(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_ghost(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_glass(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_gold(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_grass(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_grouped_thing(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_health_visible(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_hit_when_dead(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_indestructible(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_insectoid(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_inventory_item(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_item_droppable(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_item_equipable(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_item_mergeable(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_item(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_key(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_kobalos(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_lava(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_level_closed_icon(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_level_curr(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_level_final_icon(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_level_locked_icon(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_level_next_icon(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_level_open_icon(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_level_select_bg(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_levitating(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_light_source(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_loggable(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_mantisman(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_meltable(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_metal(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_minion(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_mob_kill_minions_on_death(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_mob(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_mob1(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_mob2(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_monst_group_easy(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_monst_group_hard(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_monst_group_mob(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_monst(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_needs_move_confirm(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_obs_to_cursor_path(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_obs_to_explosion(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_obs_to_falling_onto(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_obs_to_fire(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_obs_to_jump_over(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_obs_to_jumping_onto(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_obs_to_jumping_out_of(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_obs_to_movement(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_obs_to_paths(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_obs_to_spawning(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_obs_to_teleporting_onto(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_obs_to_vision(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_obs_when_dead(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_openable(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_physics_explosion(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_physics_temperature(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_physics_water(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_pillar(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_plant(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_player(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_projectile(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_reeds(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_removable_on_err(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_removable_when_dead_on_err(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_rock(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_shovable(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_slime(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_smoke(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_stamina_visible(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_steam(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_stone(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_submergible(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_teleport_blocked(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_teleport(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_tick_end_delay(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_tickable(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_tiled(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_trap(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_treasure(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_undead(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_unused1(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_unused10(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_unused11(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_unused12(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_unused13(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_unused14(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_unused15(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_unused16(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_unused17(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_unused18(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_unused19(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_unused2(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_unused20(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_unused21(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_unused22(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_unused23(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_unused24(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_unused25(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_unused26(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_unused27(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_unused28(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_unused29(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_unused3(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_unused30(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_unused31(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_unused32(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_unused33(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_unused34(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_unused35(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_unused36(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_unused37(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_unused4(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_unused46(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_unused47(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_unused5(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_unused6(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_unused63(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_unused7(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_unused8(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_unused9(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_vault(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_vision_180_degrees(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_vision_360_degrees(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_wait_on_dead_anim(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_wall(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_water(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_is_wood(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me = nullptr) -> Thingp;
[[nodiscard]] auto level_tick_begin_is_requested(Gamep g, Levelsp v, Levelp l) -> bool;
[[nodiscard]] auto level_tick_begin_requested(Gamep g, Levelsp v, Levelp l, const char *why) -> bool;
// end sort marker3 }

// begin sort marker4 {
[[nodiscard]] auto level_is_able_to_collect_items_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_able_to_collect_keys_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_able_to_crush_grass_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_able_to_fall_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_able_to_fall_repeatedly_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_able_to_fall_sound_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_able_to_fire_projectiles_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_able_to_jump_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_able_to_lunge_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_able_to_move_diagonally_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_able_to_move_through_walls_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_able_to_open_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_able_to_shove_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_able_to_walk_through_walls_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_animated_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_animated_can_hflip_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_animated_no_dir_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_animated_sync_first_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_attackable_by_monst_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_attackable_by_player_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_barrel_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_biome_bogland_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_biome_dungeon_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_biome_graveyard_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_biome_nethervoid_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_biome_underhell_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_blit_bg_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_blit_centered_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_blit_flush_per_line_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_blit_hit_outline_w_black_inside_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_blit_hit_outline_w_invis_inside_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_blit_if_has_seen_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_blit_obscures_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_blit_on_ground_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_blit_outlined_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_blit_per_pixel_lighting_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_blit_shown_in_chasms_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_blit_shown_in_overlay_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_blit_square_outlined_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_blit_when_obscured_faded_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_blit_when_obscured_outline_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_blitzhound_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_border_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_brazier_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_bridge_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_broken_on_death_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_burnable_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_chasm_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_collectable_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_collision_circle_large_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_collision_circle_small_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_collision_detection_enabled_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_collision_hit_all_on_tile_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_collision_hit_first_on_tile_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_collision_square_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_combustible_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_corpse_on_death_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_corridor_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_crushable_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_cursor_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_cursor_path_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_cursor_path_hazard_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_cursor_path_none_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_cursor_path_warning_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_damage_capped_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_dead_on_collision_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_dead_on_shoving_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_deep_water_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_described_cursor_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_dirt_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_dmap_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_door_locked_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_door_secret_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_door_unlocked_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_dungeon_entrance_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_effect_attack_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_effect_blood_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_effect_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_entrance_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_ethereal_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_exit_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_explosion_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_extinguished_on_death_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_fire_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_fireball_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_flesh_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_floating_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_floor_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_flying_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_foliage_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_gaseous_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_ghost_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_glass_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_gold_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_grass_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_grouped_thing_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_health_visible_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_hit_when_dead_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_indestructible_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_insectoid_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_inventory_item_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_item_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_item_droppable_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_item_equipable_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_item_mergeable_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_key_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_kobalos_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_lava_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_level_closed_icon_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_level_curr_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_level_final_icon_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_level_locked_icon_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_level_next_icon_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_level_open_icon_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_level_select_bg_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_levitating_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_light_source_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_loggable_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_mantisman_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_meltable_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_metal_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_minion_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_mob_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_mob_kill_minions_on_death_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_mob1_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_mob2_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_monst_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_monst_group_easy_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_monst_group_hard_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_monst_group_mob_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_needs_move_confirm_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_obs_to_cursor_path_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_obs_to_explosion_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_obs_to_falling_onto_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_obs_to_fire_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_obs_to_jump_over_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_obs_to_jumping_onto_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_obs_to_jumping_out_of_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_obs_to_movement_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_obs_to_paths_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_obs_to_spawning_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_obs_to_teleporting_onto_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_obs_to_vision_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_obs_when_dead_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_openable_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_physics_explosion_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_physics_temperature_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_physics_water_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_pillar_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_plant_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_player_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_projectile_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_reeds_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_removable_on_err_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_removable_when_dead_on_err_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_rock_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_shovable_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_slime_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_smoke_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_stamina_visible_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_steam_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_stone_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_submergible_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_teleport_blocked_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_teleport_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_tick_end_delay_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_tickable_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_tiled_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_trap_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_treasure_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_undead_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_unused1_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_unused10_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_unused11_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_unused12_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_unused13_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_unused14_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_unused15_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_unused16_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_unused17_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_unused18_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_unused19_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_unused2_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_unused20_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_unused21_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_unused22_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_unused23_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_unused24_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_unused25_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_unused26_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_unused27_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_unused28_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_unused29_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_unused3_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_unused30_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_unused31_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_unused32_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_unused33_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_unused34_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_unused35_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_unused36_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_unused37_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_unused4_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_unused46_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_unused47_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_unused5_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_unused6_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_unused63_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_unused7_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_unused8_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_unused9_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_vault_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_vision_180_degrees_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_vision_360_degrees_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_wait_on_dead_anim_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_wall_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_water_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_is_wood_cached(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
// end sort marker4 }

// begin sort marker5 {
[[nodiscard]] auto level_alive_is_able_to_collect_items(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_able_to_collect_keys(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_able_to_crush_grass(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_able_to_fall_repeatedly(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_able_to_fall_sound(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_able_to_fall(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_able_to_fire_projectiles(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_able_to_jump(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_able_to_lunge(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_able_to_move_diagonally(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_able_to_move_through_walls(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_able_to_open(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_able_to_shove(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_able_to_walk_through_walls(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_animated_can_hflip(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_animated_no_dir(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_animated_sync_first(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_animated(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_attackable_by_monst(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_attackable_by_player(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_barrel(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_biome_bogland(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_biome_dungeon(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_biome_graveyard(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_biome_nethervoid(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_biome_underhell(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_blit_bg(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_blit_centered(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_blit_flush_per_line(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_blit_hit_outline_w_black_inside(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_blit_hit_outline_w_invis_inside(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_blit_if_has_seen(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_blit_obscures(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_blit_on_ground(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_blit_outlined(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_blit_per_pixel_lighting(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_blit_shown_in_chasms(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_blit_shown_in_overlay(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_blit_square_outlined(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_blit_when_obscured_faded(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_blit_when_obscured_outline(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_blitzhound(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_border(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_brazier(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_bridge(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_broken_on_death(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_burnable(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_chasm(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_collectable(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_collision_circle_large(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_collision_circle_small(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_collision_detection_enabled(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_collision_hit_all_on_tile(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_collision_hit_first_on_tile(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_collision_square(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_combustible(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_corpse_on_death(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_corridor(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_crushable(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_cursor_path_hazard(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_cursor_path_none(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_cursor_path_warning(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_cursor_path(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_cursor(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_damage_capped(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_dead_on_collision(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_dead_on_shoving(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_deep_water(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_described_cursor(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_dirt(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_dmap(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_door_locked(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_door_secret(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_door_unlocked(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_dungeon_entrance(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_effect_attack(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_effect_blood(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_effect(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_entrance(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_ethereal(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_exit(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_explosion(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_extinguished_on_death(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_fire(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_fireball(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_flesh(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_floating(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_floor(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_flying(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_foliage(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_gaseous(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_ghost(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_glass(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_gold(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_grass(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_grouped_thing(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_health_visible(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_hit_when_dead(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_indestructible(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_insectoid(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_inventory_item(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_item_droppable(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_item_equipable(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_item_mergeable(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_item(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_key(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_kobalos(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_lava(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_level_closed_icon(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_level_curr(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_level_final_icon(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_level_locked_icon(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_level_next_icon(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_level_open_icon(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_level_select_bg(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_levitating(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_light_source(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_loggable(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_mantisman(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_meltable(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_metal(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_minion(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_mob_kill_minions_on_death(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_mob(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_mob1(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_mob2(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_monst_group_easy(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_monst_group_hard(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_monst_group_mob(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_monst(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_needs_move_confirm(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_obs_to_cursor_path(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_obs_to_explosion(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_obs_to_falling_onto(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_obs_to_fire(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_obs_to_jump_over(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_obs_to_jumping_onto(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_obs_to_jumping_out_of(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_obs_to_movement(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_obs_to_paths(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_obs_to_spawning(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_obs_to_teleporting_onto(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_obs_to_vision(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_obs_when_dead(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_openable(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_physics_explosion(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_physics_temperature(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_physics_water(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_pillar(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_plant(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_player(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_projectile(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_reeds(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_removable_on_err(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_removable_when_dead_on_err(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_rock(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_shovable(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_slime(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_smoke(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_stamina_visible(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_steam(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_stone(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_submergible(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_teleport_blocked(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_teleport(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_tick_end_delay(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_tickable(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_tiled(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_trap(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_treasure(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_undead(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_unused1(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_unused10(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_unused11(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_unused12(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_unused13(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_unused14(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_unused15(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_unused16(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_unused17(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_unused18(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_unused19(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_unused2(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_unused20(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_unused21(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_unused22(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_unused23(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_unused24(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_unused25(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_unused26(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_unused27(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_unused28(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_unused29(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_unused3(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_unused30(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_unused31(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_unused32(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_unused33(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_unused34(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_unused35(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_unused36(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_unused37(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_unused4(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_unused46(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_unused47(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_unused5(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_unused6(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_unused63(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_unused7(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_unused8(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_unused9(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_vault(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_vision_180_degrees(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_vision_360_degrees(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_wait_on_dead_anim(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_wall(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_water(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_alive_is_wood(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
// end sort marker5 }

// begin sort marker6 {
[[nodiscard]] auto level_count_is_able_to_collect_items(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_able_to_collect_keys(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_able_to_crush_grass(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_able_to_fall_repeatedly(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_able_to_fall_sound(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_able_to_fall(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_able_to_fire_projectiles(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_able_to_jump(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_able_to_lunge(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_able_to_move_diagonally(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_able_to_move_through_walls(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_able_to_open(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_able_to_shove(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_able_to_walk_through_walls(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_animated_can_hflip(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_animated_no_dir(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_animated_sync_first(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_animated(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_attackable_by_monst(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_attackable_by_player(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_barrel(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_biome_bogland(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_biome_dungeon(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_biome_graveyard(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_biome_nethervoid(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_biome_underhell(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_blit_bg(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_blit_centered(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_blit_flush_per_line(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_blit_hit_outline_w_black_inside(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_blit_hit_outline_w_invis_inside(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_blit_if_has_seen(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_blit_obscures(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_blit_on_ground(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_blit_outlined(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_blit_per_pixel_lighting(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_blit_shown_in_chasms(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_blit_shown_in_overlay(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_blit_square_outlined(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_blit_when_obscured_faded(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_blit_when_obscured_outline(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_blitzhound(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_border(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_brazier(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_bridge(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_broken_on_death(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_burnable(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_chasm(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_collectable(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_collision_circle_large(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_collision_circle_small(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_collision_detection_enabled(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_collision_hit_all_on_tile(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_collision_hit_first_on_tile(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_collision_square(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_combustible(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_corpse_on_death(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_corridor(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_crushable(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_cursor_path_hazard(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_cursor_path_none(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_cursor_path_warning(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_cursor_path(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_cursor(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_damage_capped(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_dead_on_collision(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_dead_on_shoving(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_deep_water(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_described_cursor(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_dirt(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_dmap(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_door_locked(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_door_secret(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_door_unlocked(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_dungeon_entrance(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_effect_attack(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_effect_blood(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_effect(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_entrance(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_ethereal(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_exit(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_explosion(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_extinguished_on_death(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_fire(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_fireball(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_flesh(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_floating(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_floor(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_flying(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_foliage(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_gaseous(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_ghost(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_glass(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_gold(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_grass(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_grouped_thing(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_health_visible(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_hit_when_dead(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_indestructible(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_insectoid(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_inventory_item(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_item_droppable(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_item_equipable(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_item_mergeable(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_item(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_key(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_kobalos(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_lava(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_level_closed_icon(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_level_curr(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_level_final_icon(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_level_locked_icon(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_level_next_icon(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_level_open_icon(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_level_select_bg(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_levitating(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_light_source(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_loggable(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_mantisman(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_meltable(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_metal(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_minion(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_mob_kill_minions_on_death(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_mob(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_mob1(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_mob2(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_monst_group_easy(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_monst_group_hard(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_monst_group_mob(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_monst(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_needs_move_confirm(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_obs_to_cursor_path(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_obs_to_explosion(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_obs_to_falling_onto(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_obs_to_fire(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_obs_to_jump_over(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_obs_to_jumping_onto(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_obs_to_jumping_out_of(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_obs_to_movement(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_obs_to_paths(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_obs_to_spawning(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_obs_to_teleporting_onto(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_obs_to_vision(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_obs_when_dead(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_openable(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_physics_explosion(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_physics_temperature(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_physics_water(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_pillar(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_plant(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_player(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_projectile(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_reeds(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_removable_on_err(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_removable_when_dead_on_err(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_rock(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_shovable(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_slime(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_smoke(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_stamina_visible(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_steam(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_stone(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_submergible(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_teleport_blocked(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_teleport(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_tick_end_delay(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_tickable(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_tiled(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_trap(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_treasure(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_undead(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_unused1(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_unused10(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_unused11(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_unused12(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_unused13(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_unused14(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_unused15(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_unused16(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_unused17(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_unused18(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_unused19(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_unused2(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_unused20(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_unused21(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_unused22(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_unused23(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_unused24(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_unused25(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_unused26(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_unused27(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_unused28(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_unused29(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_unused3(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_unused30(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_unused31(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_unused32(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_unused33(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_unused34(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_unused35(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_unused36(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_unused37(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_unused4(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_unused46(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_unused47(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_unused5(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_unused6(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_unused63(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_unused7(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_unused8(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_unused9(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_vault(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_vision_180_degrees(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_vision_360_degrees(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_wait_on_dead_anim(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_wall(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_water(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
[[nodiscard]] auto level_count_is_wood(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> uint32_t;
// end sort marker6 }

// begin sort marker7 {
[[nodiscard]] auto fragment_alt_char(FragmentAltp /*r*/, int x, int y) -> char;
[[nodiscard]] auto fragment_alt_random_get(Gamep g, Fragmentp f, bpoint at) -> FragmentAltp;
[[nodiscard]] auto fragment_char(Fragmentp f, int x, int y) -> char;
[[nodiscard]] auto fragment_height(Fragmentp f) -> int;
[[nodiscard]] auto fragment_random_get(Gamep g) -> Fragmentp;
[[nodiscard]] auto fragment_width(Fragmentp f) -> int;
[[nodiscard]] auto level_has_seen(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> bool;
[[nodiscard]] auto level_open_is_able_to_collect_items(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_able_to_collect_keys(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_able_to_crush_grass(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_able_to_fall_repeatedly(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_able_to_fall_sound(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_able_to_fall(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_able_to_fire_projectiles(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_able_to_jump(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_able_to_lunge(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_able_to_move_diagonally(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_able_to_move_through_walls(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_able_to_open(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_able_to_shove(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_able_to_walk_through_walls(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_animated_can_hflip(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_animated_no_dir(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_animated_sync_first(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_animated(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_attackable_by_monst(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_attackable_by_player(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_barrel(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_biome_bogland(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_biome_dungeon(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_biome_graveyard(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_biome_nethervoid(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_biome_underhell(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_blit_bg(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_blit_centered(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_blit_flush_per_line(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_blit_hit_outline_w_black_inside(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_blit_hit_outline_w_invis_inside(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_blit_if_has_seen(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_blit_obscures(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_blit_on_ground(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_blit_outlined(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_blit_per_pixel_lighting(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_blit_shown_in_chasms(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_blit_shown_in_overlay(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_blit_square_outlined(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_blit_when_obscured_faded(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_blit_when_obscured_outline(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_blitzhound(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_border(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_brazier(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_bridge(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_broken_on_death(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_burnable(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_chasm(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_collectable(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_collision_circle_large(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_collision_circle_small(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_collision_detection_enabled(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_collision_hit_all_on_tile(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_collision_hit_first_on_tile(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_collision_square(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_combustible(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_corpse_on_death(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_corridor(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_crushable(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_cursor_path_hazard(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_cursor_path_none(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_cursor_path_warning(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_cursor_path(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_cursor(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_damage_capped(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_dead_on_collision(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_dead_on_shoving(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_deep_water(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_described_cursor(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_dirt(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_dmap(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_door_locked(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_door_secret(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_door_unlocked(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_dungeon_entrance(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_effect_attack(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_effect_blood(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_effect(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_entrance(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_ethereal(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_exit(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_explosion(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_extinguished_on_death(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_fire(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_fireball(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_flesh(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_floating(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_floor(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_flying(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_foliage(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_gaseous(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_ghost(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_glass(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_gold(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_grass(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_grouped_thing(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_health_visible(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_hit_when_dead(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_indestructible(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_insectoid(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_inventory_item(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_item_droppable(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_item_equipable(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_item_mergeable(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_item(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_key(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_kobalos(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_lava(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_level_closed_icon(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_level_curr(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_level_final_icon(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_level_locked_icon(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_level_next_icon(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_level_open_icon(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_level_select_bg(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_levitating(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_light_source(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_loggable(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_mantisman(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_meltable(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_metal(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_minion(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_mob_kill_minions_on_death(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_mob(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_mob1(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_mob2(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_monst_group_easy(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_monst_group_hard(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_monst_group_mob(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_monst(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_needs_move_confirm(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_obs_to_cursor_path(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_obs_to_explosion(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_obs_to_falling_onto(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_obs_to_fire(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_obs_to_jump_over(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_obs_to_jumping_onto(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_obs_to_jumping_out_of(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_obs_to_movement(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_obs_to_paths(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_obs_to_spawning(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_obs_to_teleporting_onto(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_obs_to_vision(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_obs_when_dead(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_openable(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_physics_explosion(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_physics_temperature(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_physics_water(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_pillar(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_plant(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_player(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_projectile(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_reeds(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_removable_on_err(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_removable_when_dead_on_err(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_rock(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_shovable(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_slime(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_smoke(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_stamina_visible(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_steam(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_stone(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_submergible(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_teleport_blocked(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_teleport(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_tick_end_delay(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_tickable(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_tiled(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_trap(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_treasure(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_undead(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_unused1(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_unused10(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_unused11(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_unused12(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_unused13(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_unused14(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_unused15(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_unused16(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_unused17(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_unused18(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_unused19(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_unused2(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_unused20(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_unused21(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_unused22(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_unused23(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_unused24(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_unused25(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_unused26(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_unused27(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_unused28(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_unused29(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_unused3(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_unused30(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_unused31(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_unused32(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_unused33(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_unused34(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_unused35(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_unused36(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_unused37(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_unused4(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_unused46(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_unused47(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_unused5(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_unused6(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_unused63(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_unused7(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_unused8(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_unused9(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_vault(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_vision_180_degrees(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_vision_360_degrees(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_wait_on_dead_anim(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_wall(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_water(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
[[nodiscard]] auto level_open_is_wood(Gamep g, Levelsp v, Levelp l, const bpoint &p) -> Thingp;
// end sort marker7 }

#endif // MY_LEVEL_HPP
