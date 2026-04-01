//
// Copyright goblinhack@gmail.com
//

#ifndef MY_THING_HPP
#define MY_THING_HPP

#include "my_age_map.hpp"
#include "my_bpoint.hpp"
#include "my_color.hpp"
#include "my_dmap.hpp"
#include "my_fov_map.hpp"
#include "my_fpoint.hpp"
#include "my_game_defs.hpp"
#include "my_gl.hpp"
#include "my_spoint.hpp"
#include "my_tp.hpp"
#include "my_types.hpp"

#include <string>
#include <vector>

enum { MY_ITERS_MAX = 4 };

using ThingIdPacked = union {
  struct {
    uint32_t val : 32;
  } __attribute__((__packed__)) a;
  struct {
    uint32_t entropy      : THING_ENTROPY_BITS;
    uint32_t per_level_id : THING_PER_LEVEL_THING_ID_BITS;
    uint32_t level_num    : THING_LEVEL_ID_BITS;
  } __attribute__((__packed__)) b;
  struct {
    uint32_t entropy   : THING_ENTROPY_BITS;
    uint32_t arr_index : THING_ARR_INDEX_BITS;
  } __attribute__((__packed__)) c;
};

//
// Essentially equates to the max number of monsters+light sources per level
//
#define THING_EXT_MAX   (LEVEL_MAX * 500) // The size of thing_ext
#define THING_LIGHT_MAX (LEVEL_MAX * 200) // The size of thing_light
enum {
  THING_DESCRIBE_MAX = 10 // The number of things we can show in the rightbar
};
#define THING_MOVE_PATH_MAX  (MAP_WIDTH + MAP_HEIGHT) // Player/monster move paths (max size uint8_t)
#define THING_INVENTORY_MAX  26
#define THING_MINION_MAX     10
#define THING_PROJECTILE_MAX 10

enum {
  TEXT_INCLUDE_OWNER = 1,
  TEXT_EXCLUDE_DEATH = 2,
  TEXT_A_OR_AN       = 4,
  TEXT_APOSTROPHIZE  = 8,
  TEXT_PLURALIZE     = 16,
};

using ThingTextFlags = uint8_t;

//
// Player state
//
#define PLAYER_STATE_ENUM(list_macro)                                                                                                      \
  CLANG_FORMAT_INDENT()                                                    /* dummy line for clang indentation fixup */                    \
  list_macro(PLAYER_STATE_INIT, "INIT"),                                   /* newline */                                                   \
      list_macro(PLAYER_STATE_DEAD, "DEAD"),                               /* newline */                                                   \
      list_macro(PLAYER_STATE_NORMAL, "NORMAL"),                           /* newline */                                                   \
      list_macro(PLAYER_STATE_PATH_REQUESTED, "PATH-REQUESTED"),           /* newline */                                                   \
      list_macro(PLAYER_STATE_MOVE_CONFIRM_REQUESTED, "MOVE-CONFIRM-REQ"), /* newline */                                                   \
      list_macro(PLAYER_STATE_FOLLOWING_PATH, "FOLLOWING-PATH"),           /* newline */

ENUM_DEF_H(PLAYER_STATE_ENUM, PlayerState)

//
// Monst state
//
#define MONST_STATE_ENUM(list_macro)                                                                                                       \
  CLANG_FORMAT_INDENT()                           /* dummy line for clang indentation fixup */                                             \
  list_macro(MONST_STATE_INIT, "INIT"),           /* newline */                                                                            \
      list_macro(MONST_STATE_DEAD, "DEAD"),       /* newline */                                                                            \
      list_macro(MONST_STATE_NORMAL, "NORMAL"),   /* newline */                                                                            \
      list_macro(MONST_STATE_CHASING, "CHASING"), /* newline */                                                                            \
      list_macro(MONST_STATE_WANDER, "WANDER"),   /* newline */

ENUM_DEF_H(MONST_STATE_ENUM, MonstState)

//
// Some kind of event that befalls a thing. Usually an attack
//
using ThingEvent = struct ThingEvent {
  //
  // What happened?
  //
  std::string reason;
  //
  // What type of event/attack?
  //
  ThingEventType event_type = THING_EVENT_NONE;
  //
  // How much damage, if this is an attack?
  //
  int damage = 0;
  //
  // Whodunnit?
  //
  Thingp source = nullptr;
};

//
// Minions
//
using ThingMinion = struct ThingMinion {
  ThingId minion_id;
};

//
// Per mob minions
//
using ThingMinions = struct ThingMinions {
  ThingMinion minion[ THING_MINION_MAX ];
  int8_t      count;
};

//
// Projectiles
//
using ThingProjectile = struct ThingProjectile {
  ThingId projectile_id;
};

//
// Per monster/player projectiles fired
//
using ThingProjectiles = struct ThingProjectiles {
  ThingProjectile projectile[ THING_PROJECTILE_MAX ];
  int8_t          count;
};

//
// Per thing extended memory
//
using ThingExt = struct ThingExt {
  uint8_t in_use : 1;
  //
  // All minions for this mob
  //
  ThingMinions minions;
  //
  // All projectiles currently en-route
  //
  ThingProjectiles projectiles;
  //
  // Can be per monster or shared per mob memory of the preferred target,
  // usually the player.
  //
  Dmap dmap;
  //
  // What we have ever seen
  //
  AgeMap has_seen;
  //
  // What we can currently see
  //
  FovMap can_see;
  //
  // Holds the path as we or the monster walk it
  //
  struct {
    bpoint  points[ THING_MOVE_PATH_MAX ];
    uint8_t size      : 7;
    uint8_t confirmed : 1;
  } move_path;
};

//
// Per thing light source lighting memory
//
using ThingLight = struct ThingLight {
  uint8_t in_use : 1;
  //
  // Used to avoid lighting the same tile multiple times.
  //
  FovMap is_lit;
};

//
// Inventory items
//
using ThingSlot = struct ThingSlot {
  ThingId item_id;
  //
  // How many of this identical item are there?
  //
  int8_t count;
};

//
// Per thing inventory memory
//
using ThingInventory = struct ThingInventory {
  //
  // This is the max any player or monster can carry
  //
  ThingSlot slots[ THING_INVENTORY_MAX ];
};

//
// Player specific memory
//
using ThingPlayer = struct ThingPlayer {
  //
  // Player score
  //
  uint32_t score;
  //
  // What we're carrying
  //
  ThingInventory inventory;
  //
  // For hiscores
  //
  int8_t levels_completed;
};

using Thing = struct Thing {
  //////////////////////////////////////////////////////////////
  // No c++ types can be used here, to allow easy level replay
  //
  // Why C types only ? For large data structures it is visibly
  // faster to malloc and memset versus default construction.
  //////////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////////////
  // Template ID. MUST BE FIRST AS WE memset the Thing after this.
  uint16_t tp_id;
  // Template ID. MUST BE FIRST AS WE memset the Thing after this.
  //////////////////////////////////////////////////////////////

  //
  // Direction
  //
  ThingDir dir;
  //
  // The current tiles[] index for this object
  //
  uint8_t anim_index;
  //
  // Unused for player.
  //
  MonstState _monst_state;
  //
  // For event processing. Lower is better. Player is 0
  //
  ThingPriorityType _priority;
  //
  // Vision in tiles.
  //
  int8_t _distance_vision;
  //
  // Distance from mob
  //
  int8_t _distance_minion_from_mob_max;
  //
  // Jump distance in tiles.
  //
  int8_t _distance_jump;
  //
  // Decrements each frame. Increments if hit.
  //
  uint8_t _is_hit;
  //
  // Decrements each frame. Increments if hidden.
  //
  uint8_t _is_hidden;
  //
  // Decrements each frame. Resets if still too hot when we reach zero.
  //
  uint8_t _is_hot;
  //
  // Used so often, we cache it
  //
  uint8_t _is_player : 1;
  //
  // Snuffed it.
  //
  uint8_t _is_dead : 1;
  //
  // Was set fire to.
  //
  uint8_t _is_burning : 1;
  //
  // Snuffed it and is a corpse.
  //
  uint8_t _is_corpse : 1;
  //
  // If a thing has died and leaves no corpse, schedule it to be removed from the game.
  //
  uint8_t _is_scheduled_for_cleanup : 1;
  //
  // If you've fallen through a chasm twice, more damage.
  //
  uint8_t _is_falling_continues : 1;
  //
  // Zzz
  //
  uint8_t _is_sleeping : 1;
  //
  // For doors, so we do not need keys forever
  //
  uint8_t _is_unlocked : 1;
  //
  // For doors, chests etc...
  //
  uint8_t _is_open : 1;
  //
  // Is being carried
  //
  uint8_t _is_carried : 1;
  //
  // Pushed onto the map?
  //
  uint8_t _is_on_map : 1;
  //
  // Currently moving between tiles. The thing is already at the destination.
  //
  uint8_t _is_moving : 1;
  //
  // Temporary flag set when a thing is newly spawned.
  //
  uint8_t _is_spawned : 1;
  //
  // Currently teleporting between tiles. The thing is already at the destination.
  //
  uint8_t _is_teleporting : 1;
  //
  // Currently jumping between tiles. The thing is already at the destination.
  //
  uint8_t _is_jumping : 1;
  //
  // Idle etc...
  //
  ThingAnim anim_type;
  //
  // Which level am I on?
  //
  LevelNum level_num;
  //
  // Current tile.
  //
  uint16_t tile_index;
  //
  // Move speed, with 100 being player.
  //
  uint16_t _speed;
  //
  // This is the amount move moving (the speed above) we can do per tick.
  //
  // If we are 200 and the player is 100, then we get 2 moves per tick.
  //
  // If we are 50 then we get to move every other tick.
  //
  // Each tick, this amount is incremented by the speed value until it
  // is >= the player's speed. At which point the monst can move.
  //
  int16_t _move_remaining; // can be decremented, so signed is safer
  //
  // Count down until the next animation frame should start
  //
  int16_t anim_ms_remaining; // can be decremented, so signed is safer
  //
  // Temperature in celsius.
  //
  int16_t _temperature; // can be decremented, so signed is safer
  //
  // Damage can be capped per tick to a limit so we don't kill a player in one go
  //
  int16_t _damage_this_tick;
  //
  // How many keys are carried
  //
  int16_t _keys_carried;
  int16_t _value1;
  int16_t _value2;
  int16_t _value3;
  int16_t _value4;
  int16_t _value5;
  int16_t _value6;
  int16_t _value7;
  int16_t _value8;
  int16_t _value9;
  int16_t _value10;
  int16_t _value11;
  int16_t _value12;
  int16_t _value13;
  int16_t _value14;
  int16_t _value15;
  int16_t _value16;
  int16_t _value17;
  int16_t _value18;
  int16_t _value19;
  int16_t _score_value;
  //
  // How many minions this mob can spawn
  //
  uint8_t _minion_max;
  //
  // How many projectiles this thing can spawn
  //
  uint8_t _projectile_max;
  //
  // The type of wall
  //
  uint8_t _variant;
  //
  // Used in lava, water etc...
  //
  uint8_t _submerged_pct;
  //
  // Lifespan remaining in ticks
  //
  int16_t _lifespan; // can be decremented, so signed is safer
  //
  // How long this thing has lived
  //
  int16_t _age; // can be decremented, so signed is safer
  //
  // Health of the item.
  //
  int16_t _health;
  //
  // Accumulates and holds the amount of time we've been falling.
  //
  uint16_t _fall_ms;
  //
  // Accumulates and holds the amount of time we've been lunging.
  //
  uint16_t _lunge_ms;
  //
  // Weight in grams. Impacts things like grass being crushed.
  //
  uint32_t _weight;
  //
  // Current game tick this thing has completed
  //
  uint32_t tick;
  //
  // Used to keep track if we have ran the temperature checks yet this tick.
  //
  uint32_t tick_temperature;
  uint32_t tick_temperature_last_change;
  //
  // Used to only tick things once per loop, even if they move slot
  //
  uint32_t iter[ MY_ITERS_MAX ];
  //
  // When this poor thing died.
  //
  uint32_t tick_dead;
  //
  // Avoid fast teleport loops
  //
  uint32_t tick_teleport;
  //
  // Unique ID with some entropy built in
  //
  ThingId id;
  //
  // If owned, by whom
  //
  ThingId owner_id;
  //
  // If a mob spawned me, whom
  //
  ThingId mob_id;
  //
  // If fired, by whom
  //
  ThingId fired_by_id;
  //
  // If this is a thing that is tied to other things, e.g. a bridge tile then
  // they all share this id; which is the id of the first thing in the group
  //
  ThingId group_id;
  //
  // For player and monsters
  //
  ThingExtId ext_id;
  //
  // For light sources and player and monsters
  //
  ThingLightId light_id;
  //
  // Interpolated co-ords in pixels
  //
  spoint _prev_pix_at;
  spoint _curr_pix_at;
  //
  // Previous map co-ords used for interpolation when moving. Changes when
  // the move finishes.
  //
  bpoint _moving_from;
  //
  // Last location we were pushed onto the map.
  //
  bpoint last_pushed_at;
  //
  // Where we're lunging
  //
  bpoint lunging_to;
  //
  // Increases per tick and when it reaches 1, allows the thing to move
  //
  float thing_dt;
  //
  // Angle of movement.
  //
  f16 angle;
  //
  // Previous map co-ords. Does not change when the move finishes.
  //
  fpoint _old_at;
  //
  // Map co-ords. This is the slot the thing is at when rounded.
  //
  // However some things like missiles can be at fractional positions.
  //
  fpoint _at;
  //
  // What we're chasing currently. Might be the player or some random tile.
  //
  bpoint _target;
};

// begin sort marker1 {
[[nodiscard]] auto immediate_owner(Gamep g, Levelsp v, Levelp l, Thingp t) -> Thingp;
[[nodiscard]] auto level_vision_blocker_at(Gamep g, Levelsp v, Levelp l, Thingp me, const bpoint &at) -> bool;
[[nodiscard]] auto monst_state_to_string(MonstState state) -> std::string;
[[nodiscard]] auto monst_state(Gamep g, Levelsp v, Levelp l, Thingp me) -> MonstState;
[[nodiscard]] auto player_check_if_target_needs_move_confirm(Gamep g, Levelsp v, Levelp l, const bpoint &to) -> bool;
[[nodiscard]] auto player_fire(Gamep g, Levelsp v, Levelp l, int dx, int dy, Tpp what = nullptr) -> bool;
[[nodiscard]] auto player_jump(Gamep g, Levelsp v, Levelp l, Thingp me, bpoint to) -> bool;
[[nodiscard]] auto player_mouse_down(Gamep, Levelsp, Levelp, int x, int y, uint32_t button) -> bool;
[[nodiscard]] auto player_move_request(Gamep g, bool up, bool down, bool left, bool right, bool fire) -> bool;
[[nodiscard]] auto player_move_to_next(Gamep g, Levelsp v, Levelp l, Thingp me) -> bool;
[[nodiscard]] auto thing_age_decr(Gamep g, Levelsp v, Levelp l, Thingp t, int val = 1) -> int;
[[nodiscard]] auto thing_age_incr(Gamep g, Levelsp v, Levelp l, Thingp t, int val = 1) -> int;
[[nodiscard]] auto thing_age_set(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int;
[[nodiscard]] auto thing_age(Thingp t) -> int;
[[nodiscard]] auto thing_alloc(Gamep g, Levelsp v, Levelp l, Tpp tp, bpoint p) -> Thingp;
[[nodiscard]] auto thing_and_tp_get_at_safe(Gamep g, Levelsp v, Levelp l, const bpoint &p, int slot, Tpp *out) -> Thingp;
[[nodiscard]] auto thing_and_tp_get_at(Gamep g, Levelsp v, Levelp l, const bpoint &p, int slot, Tpp *out) -> Thingp;
[[nodiscard]] auto thing_at(Thingp t) -> bpoint;
[[nodiscard]] auto thing_attack_at(Gamep g, Levelsp v, Levelp l, Thingp me, const bpoint &attack_at) -> bool;
[[nodiscard]] auto thing_can_move_to_ai(Gamep g, Levelsp v, Levelp l, Thingp me, bpoint to) -> bool;
[[nodiscard]] auto thing_can_move_to_attempt_by_opening(Gamep g, Levelsp v, Levelp l, Thingp me, bpoint to) -> bool;
[[nodiscard]] auto thing_can_move_to_attempt_by_shoving(Gamep g, Levelsp v, Levelp l, Thingp me, bpoint to) -> bool;
[[nodiscard]] auto thing_can_move_to_attempt(Gamep g, Levelsp v, Levelp l, Thingp me, bpoint to) -> bool;
[[nodiscard]] auto thing_can_move_to_possible(Gamep g, Levelsp v, Levelp l, Thingp me, bpoint to) -> bool;
[[nodiscard]] auto thing_carry_item(Gamep g, Levelsp v, Levelp l, Thingp item, Thingp carrier) -> bool;
[[nodiscard]] auto thing_close(Gamep g, Levelsp v, Levelp l, Thingp me, Thingp closer) -> bool;
[[nodiscard]] auto thing_collect_key(Gamep g, Levelsp v, Levelp l, Thingp it, Thingp carrier) -> bool;
[[nodiscard]] auto thing_collision_radius(Thingp t) -> float;
[[nodiscard]] auto thing_corpse_allowed(Gamep g, Levelsp v, Levelp l, Thingp t) -> bool;
[[nodiscard]] auto thing_crush(Gamep g, Levelsp v, Levelp l, Thingp me, Thingp crusher) -> bool;
[[nodiscard]] auto thing_damage_this_tick_decr(Gamep g, Levelsp v, Levelp l, Thingp t, int val = 1) -> int;
[[nodiscard]] auto thing_damage_this_tick_incr(Gamep g, Levelsp v, Levelp l, Thingp t, int val = 1) -> int;
[[nodiscard]] auto thing_damage_this_tick_set(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int;
[[nodiscard]] auto thing_damage_this_tick(Thingp t) -> int;
[[nodiscard]] auto thing_debug(Gamep g, Levelsp v, Levelp l, Thingp t, uint32_t iter_index) -> bool;
[[nodiscard]] auto thing_distance_jump_decr(Gamep g, Levelsp v, Levelp l, Thingp t, int val = 1) -> int;
[[nodiscard]] auto thing_distance_jump_incr(Gamep g, Levelsp v, Levelp l, Thingp t, int val = 1) -> int;
[[nodiscard]] auto thing_distance_jump_set(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int;
[[nodiscard]] auto thing_distance_jump(Thingp t) -> int;
[[nodiscard]] auto thing_distance_minion_from_mob_max_set(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int;
[[nodiscard]] auto thing_distance_minion_from_mob_max(Thingp t) -> int;
[[nodiscard]] auto thing_distance_vision_decr(Gamep g, Levelsp v, Levelp l, Thingp t, int val = 1) -> int;
[[nodiscard]] auto thing_distance_vision_incr(Gamep g, Levelsp v, Levelp l, Thingp t, int val = 1) -> int;
[[nodiscard]] auto thing_distance_vision_set(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int;
[[nodiscard]] auto thing_distance_vision(Thingp t) -> int;
[[nodiscard]] auto thing_drop_item(Gamep g, Levelsp v, Levelp l, Thingp item, Thingp dropper) -> bool;
[[nodiscard]] auto thing_ext_struct(Gamep g, Thingp t) -> ThingExtp;
[[nodiscard]] auto thing_find_non_inline(Gamep g, Levelsp v, ThingId id) -> Thingp;
[[nodiscard]] auto thing_get_at_safe(Gamep g, Levelsp v, Levelp l, const bpoint &p, int slot) -> Thingp;
[[nodiscard]] auto thing_get_direction_grid(Gamep g, Levelsp v, Levelp l, Thingp me) -> bpoint;
[[nodiscard]] auto thing_get_direction(Gamep g, Levelsp v, Levelp l, Thingp me) -> fpoint;
[[nodiscard]] auto thing_get_dmap(Gamep g, Levelsp v, Levelp l, Thingp me) -> Dmap *;
[[nodiscard]] auto thing_get(Gamep g, Levelsp v, Levelp l, const bpoint &p, int slot) -> Thingp;
[[nodiscard]] auto thing_health_decr(Gamep g, Levelsp v, Levelp l, Thingp t, int val = 1) -> int;
[[nodiscard]] auto thing_health_incr(Gamep g, Levelsp v, Levelp l, Thingp t, int val = 1) -> int;
[[nodiscard]] auto thing_health_set(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int;
[[nodiscard]] auto thing_health(Thingp t) -> int;
[[nodiscard]] auto thing_init(Gamep g, Levelsp v, Levelp l, Tpp tp, const bpoint &at) -> Thingp;
[[nodiscard]] auto thing_init(Gamep g, Levelsp v, Levelp l, Tpp tp, const fpoint &real_at) -> Thingp;
[[nodiscard]] auto thing_inventory_add(Gamep g, Levelsp v, Levelp l, Thingp new_item, Thingp carrier) -> bool;
[[nodiscard]] auto thing_inventory_is_empty(Gamep g, Levelsp v, Levelp l, Thingp carrier) -> bool;
[[nodiscard]] auto thing_inventory_item_mergeable(Gamep g, Levelsp v, Levelp l, Thingp a, Thingp b) -> bool;
[[nodiscard]] auto thing_inventory_remove(Gamep g, Levelsp v, Levelp l, Thingp drop_item, Thingp carrier) -> bool;
[[nodiscard]] auto thing_is_able_to_collect_items(Thingp t) -> bool;
[[nodiscard]] auto thing_is_able_to_collect_keys(Thingp t) -> bool;
[[nodiscard]] auto thing_is_able_to_crush_grass(Thingp t) -> bool;
[[nodiscard]] auto thing_is_able_to_fall_repeatedly(Thingp t) -> bool;
[[nodiscard]] auto thing_is_able_to_fall_sound(Thingp t) -> bool;
[[nodiscard]] auto thing_is_able_to_fall(Thingp t) -> bool;
[[nodiscard]] auto thing_is_able_to_fire_projectiles(Thingp t) -> bool;
[[nodiscard]] auto thing_is_able_to_jump(Thingp t) -> bool;
[[nodiscard]] auto thing_is_able_to_lunge(Thingp t) -> bool;
[[nodiscard]] auto thing_is_able_to_move_diagonally(Thingp t) -> bool;
[[nodiscard]] auto thing_is_able_to_move_through_walls(Thingp t) -> bool;
[[nodiscard]] auto thing_is_able_to_open(Thingp t) -> bool;
[[nodiscard]] auto thing_is_able_to_shove(Thingp t) -> bool;
[[nodiscard]] auto thing_is_able_to_walk_through_walls(Thingp t) -> bool;
[[nodiscard]] auto thing_is_always_hot(Thingp me) -> bool;
[[nodiscard]] auto thing_is_animated_can_hflip(Thingp t) -> bool;
[[nodiscard]] auto thing_is_animated_no_dir(Thingp t) -> bool;
[[nodiscard]] auto thing_is_animated_sync_first(Thingp t) -> bool;
[[nodiscard]] auto thing_is_animated(Thingp t) -> bool;
[[nodiscard]] auto thing_is_attackable_by_monst(Thingp t) -> bool;
[[nodiscard]] auto thing_is_attackable_by_player(Thingp t) -> bool;
[[nodiscard]] auto thing_is_barrel(Thingp t) -> bool;
[[nodiscard]] auto thing_is_blit_centered(Thingp t) -> bool;
[[nodiscard]] auto thing_is_blit_flush_per_line(Thingp t) -> bool;
[[nodiscard]] auto thing_is_blit_hit_outline_w_black_inside(Thingp t) -> bool;
[[nodiscard]] auto thing_is_blit_hit_outline_w_invis_inside(Thingp t) -> bool;
[[nodiscard]] auto thing_is_blit_if_has_seen(Thingp t) -> bool;
[[nodiscard]] auto thing_is_blit_obscures(Thingp t) -> bool;
[[nodiscard]] auto thing_is_blit_on_ground(Thingp t) -> bool;
[[nodiscard]] auto thing_is_blit_outlined(Thingp t) -> bool;
[[nodiscard]] auto thing_is_blit_per_pixel_lighting(Thingp t) -> bool;
[[nodiscard]] auto thing_is_blit_shown_in_chasms(Thingp t) -> bool;
[[nodiscard]] auto thing_is_blit_shown_in_overlay(Thingp t) -> bool;
[[nodiscard]] auto thing_is_blit_square_outlined(Thingp t) -> bool;
[[nodiscard]] auto thing_is_blit_when_obscured_faded(Thingp t) -> bool;
[[nodiscard]] auto thing_is_blit_when_obscured_outline(Thingp t) -> bool;
[[nodiscard]] auto thing_is_blitzhound(Thingp t) -> bool;
[[nodiscard]] auto thing_is_border(Thingp t) -> bool;
[[nodiscard]] auto thing_is_brazier(Thingp t) -> bool;
[[nodiscard]] auto thing_is_bridge(Thingp t) -> bool;
[[nodiscard]] auto thing_is_broken_on_death(Thingp t) -> bool;
[[nodiscard]] auto thing_is_burnable(Thingp t) -> bool;
[[nodiscard]] auto thing_is_burning(Thingp t) -> bool;
[[nodiscard]] auto thing_is_carried_try_set(Gamep g, Levelsp v, Levelp l, Thingp item, Thingp carrier, bool val = true) -> bool;
[[nodiscard]] auto thing_is_carried_try_unset(Gamep g, Levelsp v, Levelp l, Thingp item, Thingp carrier) -> bool;
[[nodiscard]] auto thing_is_carried(Thingp t) -> bool;
[[nodiscard]] auto thing_is_chasm(Thingp t) -> bool;
[[nodiscard]] auto thing_is_collectable(Thingp t) -> bool;
[[nodiscard]] auto thing_is_collision_circle_large(Thingp t) -> bool;
[[nodiscard]] auto thing_is_collision_circle_small(Thingp t) -> bool;
[[nodiscard]] auto thing_is_collision_detection_enabled(Thingp t) -> bool;
[[nodiscard]] auto thing_is_collision_hit_all_on_tile(Thingp t) -> bool;
[[nodiscard]] auto thing_is_collision_hit_first_on_tile(Thingp t) -> bool;
[[nodiscard]] auto thing_is_collision_square(Thingp t) -> bool;
[[nodiscard]] auto thing_is_combustible(Thingp t) -> bool;
[[nodiscard]] auto thing_is_corpse_on_death(Thingp t) -> bool;
[[nodiscard]] auto thing_is_corpse(Thingp t) -> bool;
[[nodiscard]] auto thing_is_corridor(Thingp t) -> bool;
[[nodiscard]] auto thing_is_crushable(Thingp t) -> bool;
[[nodiscard]] auto thing_is_cursor_path_hazard(Thingp t) -> bool;
[[nodiscard]] auto thing_is_cursor_path_none(Thingp t) -> bool;
[[nodiscard]] auto thing_is_cursor_path_warning(Thingp t) -> bool;
[[nodiscard]] auto thing_is_cursor_path(Thingp t) -> bool;
[[nodiscard]] auto thing_is_cursor(Thingp t) -> bool;
[[nodiscard]] auto thing_is_damage_capped(Thingp t) -> bool;
[[nodiscard]] auto thing_is_dead_on_collision(Thingp t) -> bool;
[[nodiscard]] auto thing_is_dead_on_shoving(Thingp t) -> bool;
[[nodiscard]] auto thing_is_deep_water(Thingp t) -> bool;
[[nodiscard]] auto thing_is_described_cursor(Thingp t) -> bool;
[[nodiscard]] auto thing_is_dir_bl(Thingp me) -> bool;
[[nodiscard]] auto thing_is_dir_br(Thingp me) -> bool;
[[nodiscard]] auto thing_is_dir_down(Thingp me) -> bool;
[[nodiscard]] auto thing_is_dir_left(Thingp me) -> bool;
[[nodiscard]] auto thing_is_dir_right(Thingp me) -> bool;
[[nodiscard]] auto thing_is_dir_tl(Thingp me) -> bool;
[[nodiscard]] auto thing_is_dir_tr(Thingp me) -> bool;
[[nodiscard]] auto thing_is_dir_up(Thingp me) -> bool;
[[nodiscard]] auto thing_is_dirt(Thingp t) -> bool;
[[nodiscard]] auto thing_is_dmap(Thingp t) -> bool;
[[nodiscard]] auto thing_is_door_locked(Thingp t) -> bool;
[[nodiscard]] auto thing_is_door_secret(Thingp t) -> bool;
[[nodiscard]] auto thing_is_door_unlocked(Thingp t) -> bool;
[[nodiscard]] auto thing_is_dungeon_entrance(Thingp t) -> bool;
[[nodiscard]] auto thing_is_effect_attack(Thingp t) -> bool;
[[nodiscard]] auto thing_is_effect_blood(Thingp t) -> bool;
[[nodiscard]] auto thing_is_effect(Thingp t) -> bool;
[[nodiscard]] auto thing_is_entrance(Thingp t) -> bool;
[[nodiscard]] auto thing_is_ethereal(Thingp t) -> bool;
[[nodiscard]] auto thing_is_exit(Thingp t) -> bool;
[[nodiscard]] auto thing_is_explosion(Thingp t) -> bool;
[[nodiscard]] auto thing_is_extinguished_on_death(Thingp t) -> bool;
[[nodiscard]] auto thing_is_falling_continues(Thingp t) -> bool;
[[nodiscard]] auto thing_is_falling_incr(Gamep g, Levelsp v, Levelp l, Thingp t, int val = 1) -> int;
[[nodiscard]] auto thing_is_fire(Thingp t) -> bool;
[[nodiscard]] auto thing_is_fireball(Thingp t) -> bool;
[[nodiscard]] auto thing_is_flesh(Thingp t) -> bool;
[[nodiscard]] auto thing_is_floating(Thingp t) -> bool;
[[nodiscard]] auto thing_is_floor(Thingp t) -> bool;
[[nodiscard]] auto thing_is_flying(Thingp t) -> bool;
[[nodiscard]] auto thing_is_foliage(Thingp t) -> bool;
[[nodiscard]] auto thing_is_gaseous(Thingp t) -> bool;
[[nodiscard]] auto thing_is_ghost(Thingp t) -> bool;
[[nodiscard]] auto thing_is_glass(Thingp t) -> bool;
[[nodiscard]] auto thing_is_gold(Thingp t) -> bool;
[[nodiscard]] auto thing_is_grass(Thingp t) -> bool;
[[nodiscard]] auto thing_is_grouped_thing(Thingp t) -> bool;
[[nodiscard]] auto thing_is_health_bar_shown(Thingp t) -> bool;
[[nodiscard]] auto thing_is_hit_decr(Gamep g, Levelsp v, Levelp l, Thingp t, int val = 1) -> int;
[[nodiscard]] auto thing_is_hit_incr(Gamep g, Levelsp v, Levelp l, Thingp t, int val = 1) -> int;
[[nodiscard]] auto thing_is_hit_when_dead(Thingp t) -> bool;
[[nodiscard]] auto thing_is_hot_check(Gamep g, Levelsp v, Levelp l, Thingp me) -> bool;
[[nodiscard]] auto thing_is_immune_to(Thingp t, ThingEventType val) -> bool;
[[nodiscard]] auto thing_is_indestructible(Thingp t) -> bool;
[[nodiscard]] auto thing_is_insectoid(Thingp t) -> bool;
[[nodiscard]] auto thing_is_inventory_item(Thingp t) -> bool;
[[nodiscard]] auto thing_is_item_droppable(Thingp t) -> bool;
[[nodiscard]] auto thing_is_item_equipable(Thingp t) -> bool;
[[nodiscard]] auto thing_is_item_mergeable(Thingp t) -> bool;
[[nodiscard]] auto thing_is_item(Thingp t) -> bool;
[[nodiscard]] auto thing_is_key(Thingp t) -> bool;
[[nodiscard]] auto thing_is_kobalos(Thingp t) -> bool;
[[nodiscard]] auto thing_is_lava(Thingp t) -> bool;
[[nodiscard]] auto thing_is_level_across_icon(Thingp t) -> bool;
[[nodiscard]] auto thing_is_level_curr(Thingp t) -> bool;
[[nodiscard]] auto thing_is_level_down_icon(Thingp t) -> bool;
[[nodiscard]] auto thing_is_level_final_icon(Thingp t) -> bool;
[[nodiscard]] auto thing_is_level_next_icon(Thingp t) -> bool;
[[nodiscard]] auto thing_is_level_not_visited_icon(Thingp t) -> bool;
[[nodiscard]] auto thing_is_level_visited_icon(Thingp t) -> bool;
[[nodiscard]] auto thing_is_levitating(Thingp t) -> bool;
[[nodiscard]] auto thing_is_loggable(Thingp t) -> bool;
[[nodiscard]] auto thing_is_mantisman(Thingp t) -> bool;
[[nodiscard]] auto thing_is_meltable(Thingp t) -> bool;
[[nodiscard]] auto thing_is_metal(Thingp t) -> bool;
[[nodiscard]] auto thing_is_minion(Thingp t) -> bool;
[[nodiscard]] auto thing_is_mob_kill_minions_on_death(Thingp t) -> bool;
[[nodiscard]] auto thing_is_mob(Thingp t) -> bool;
[[nodiscard]] auto thing_is_mob1(Thingp t) -> bool;
[[nodiscard]] auto thing_is_mob2(Thingp t) -> bool;
[[nodiscard]] auto thing_is_monst_group_easy(Thingp t) -> bool;
[[nodiscard]] auto thing_is_monst_group_hard(Thingp t) -> bool;
[[nodiscard]] auto thing_is_monst_group_mob(Thingp t) -> bool;
[[nodiscard]] auto thing_is_monst(Thingp t) -> bool;
[[nodiscard]] auto thing_is_needs_move_confirm(Thingp t) -> bool;
[[nodiscard]] auto thing_is_obs_to_cursor_path(Thingp t) -> bool;
[[nodiscard]] auto thing_is_obs_to_explosion(Thingp t) -> bool;
[[nodiscard]] auto thing_is_obs_to_falling_onto(Thingp t) -> bool;
[[nodiscard]] auto thing_is_obs_to_fire(Thingp t) -> bool;
[[nodiscard]] auto thing_is_obs_to_jump_over(Thingp t) -> bool;
[[nodiscard]] auto thing_is_obs_to_jumping_onto(Thingp t) -> bool;
[[nodiscard]] auto thing_is_obs_to_jumping_out_of(Thingp t) -> bool;
[[nodiscard]] auto thing_is_obs_to_movement(Thingp t) -> bool;
[[nodiscard]] auto thing_is_obs_to_paths(Thingp t) -> bool;
[[nodiscard]] auto thing_is_obs_to_spawning(Thingp t) -> bool;
[[nodiscard]] auto thing_is_obs_to_teleporting_onto(Thingp t) -> bool;
[[nodiscard]] auto thing_is_obs_when_dead(Thingp t) -> bool;
[[nodiscard]] auto thing_is_on_map(Thingp t) -> bool;
[[nodiscard]] auto thing_is_open_try_set(Gamep g, Levelsp v, Levelp l, Thingp t, Thingp opener, bool val = true) -> bool;
[[nodiscard]] auto thing_is_open_try_unset(Gamep g, Levelsp v, Levelp l, Thingp t, Thingp closer) -> bool;
[[nodiscard]] auto thing_is_openable(Thingp t) -> bool;
[[nodiscard]] auto thing_is_physics_explosion(Thingp t) -> bool;
[[nodiscard]] auto thing_is_physics_water(Thingp t) -> bool;
[[nodiscard]] auto thing_is_pillar(Thingp t) -> bool;
[[nodiscard]] auto thing_is_plant(Thingp t) -> bool;
[[nodiscard]] auto thing_is_projectile(Thingp t) -> bool;
[[nodiscard]] auto thing_is_removable_on_err(Thingp t) -> bool;
[[nodiscard]] auto thing_is_removable_when_dead_on_err(Thingp t) -> bool;
[[nodiscard]] auto thing_is_rock(Thingp t) -> bool;
[[nodiscard]] auto thing_is_scheduled_for_cleanup(Thingp t) -> bool;
[[nodiscard]] auto thing_is_shovable(Thingp t) -> bool;
[[nodiscard]] auto thing_is_sleeping(Thingp me) -> bool;
[[nodiscard]] auto thing_is_slime(Thingp t) -> bool;
[[nodiscard]] auto thing_is_smoke(Thingp t) -> bool;
[[nodiscard]] auto thing_is_steam(Thingp t) -> bool;
[[nodiscard]] auto thing_is_stone(Thingp t) -> bool;
[[nodiscard]] auto thing_is_submergible(Thingp t) -> bool;
[[nodiscard]] auto thing_is_teleport_blocked(Thingp t) -> bool;
[[nodiscard]] auto thing_is_teleport(Thingp t) -> bool;
[[nodiscard]] auto thing_is_teleporting(Thingp me) -> bool;
[[nodiscard]] auto thing_is_tick_end_delay(Thingp t) -> bool;
[[nodiscard]] auto thing_is_tickable(Thingp t) -> bool;
[[nodiscard]] auto thing_is_tiled(Thingp t) -> bool;
[[nodiscard]] auto thing_is_trap(Thingp t) -> bool;
[[nodiscard]] auto thing_is_treasure(Thingp t) -> bool;
[[nodiscard]] auto thing_is_undead(Thingp t) -> bool;
[[nodiscard]] auto thing_is_unlocked(Thingp t) -> bool;
[[nodiscard]] auto thing_is_unused1(Thingp t) -> bool;
[[nodiscard]] auto thing_is_unused10(Thingp t) -> bool;
[[nodiscard]] auto thing_is_unused11(Thingp t) -> bool;
[[nodiscard]] auto thing_is_unused12(Thingp t) -> bool;
[[nodiscard]] auto thing_is_unused13(Thingp t) -> bool;
[[nodiscard]] auto thing_is_unused14(Thingp t) -> bool;
[[nodiscard]] auto thing_is_unused15(Thingp t) -> bool;
[[nodiscard]] auto thing_is_unused16(Thingp t) -> bool;
[[nodiscard]] auto thing_is_unused17(Thingp t) -> bool;
[[nodiscard]] auto thing_is_unused18(Thingp t) -> bool;
[[nodiscard]] auto thing_is_unused19(Thingp t) -> bool;
[[nodiscard]] auto thing_is_unused2(Thingp t) -> bool;
[[nodiscard]] auto thing_is_unused20(Thingp t) -> bool;
[[nodiscard]] auto thing_is_unused21(Thingp t) -> bool;
[[nodiscard]] auto thing_is_unused22(Thingp t) -> bool;
[[nodiscard]] auto thing_is_unused23(Thingp t) -> bool;
[[nodiscard]] auto thing_is_unused24(Thingp t) -> bool;
[[nodiscard]] auto thing_is_unused25(Thingp t) -> bool;
[[nodiscard]] auto thing_is_unused26(Thingp t) -> bool;
[[nodiscard]] auto thing_is_unused27(Thingp t) -> bool;
[[nodiscard]] auto thing_is_unused28(Thingp t) -> bool;
[[nodiscard]] auto thing_is_unused29(Thingp t) -> bool;
[[nodiscard]] auto thing_is_unused3(Thingp t) -> bool;
[[nodiscard]] auto thing_is_unused30(Thingp t) -> bool;
[[nodiscard]] auto thing_is_unused31(Thingp t) -> bool;
[[nodiscard]] auto thing_is_unused32(Thingp t) -> bool;
[[nodiscard]] auto thing_is_unused33(Thingp t) -> bool;
[[nodiscard]] auto thing_is_unused34(Thingp t) -> bool;
[[nodiscard]] auto thing_is_unused35(Thingp t) -> bool;
[[nodiscard]] auto thing_is_unused36(Thingp t) -> bool;
[[nodiscard]] auto thing_is_unused37(Thingp t) -> bool;
[[nodiscard]] auto thing_is_unused38(Thingp t) -> bool;
[[nodiscard]] auto thing_is_unused39(Thingp t) -> bool;
[[nodiscard]] auto thing_is_unused4(Thingp t) -> bool;
[[nodiscard]] auto thing_is_unused40(Thingp t) -> bool;
[[nodiscard]] auto thing_is_unused41(Thingp t) -> bool;
[[nodiscard]] auto thing_is_unused42(Thingp t) -> bool;
[[nodiscard]] auto thing_is_unused43(Thingp t) -> bool;
[[nodiscard]] auto thing_is_unused44(Thingp t) -> bool;
[[nodiscard]] auto thing_is_unused45(Thingp t) -> bool;
[[nodiscard]] auto thing_is_unused46(Thingp t) -> bool;
[[nodiscard]] auto thing_is_unused47(Thingp t) -> bool;
[[nodiscard]] auto thing_is_unused48(Thingp t) -> bool;
[[nodiscard]] auto thing_is_unused5(Thingp t) -> bool;
[[nodiscard]] auto thing_is_unused6(Thingp t) -> bool;
[[nodiscard]] auto thing_is_unused63(Thingp t) -> bool;
[[nodiscard]] auto thing_is_unused7(Thingp t) -> bool;
[[nodiscard]] auto thing_is_unused8(Thingp t) -> bool;
[[nodiscard]] auto thing_is_unused9(Thingp t) -> bool;
[[nodiscard]] auto thing_is_vision_180_degrees(Thingp t) -> bool;
[[nodiscard]] auto thing_is_vision_360_degrees(Thingp t) -> bool;
[[nodiscard]] auto thing_is_wait_on_dead_anim(Thingp t) -> bool;
[[nodiscard]] auto thing_is_wall(Thingp t) -> bool;
[[nodiscard]] auto thing_is_water(Thingp t) -> bool;
[[nodiscard]] auto thing_is_wood(Thingp t) -> bool;
[[nodiscard]] auto thing_jump_to(Gamep g, Levelsp v, Levelp l, Thingp me, bpoint to, bool warn = true) -> bool;
[[nodiscard]] auto thing_keys_carried_decr(Gamep g, Levelsp v, Levelp l, Thingp t, int val = 1) -> int;
[[nodiscard]] auto thing_keys_carried_incr(Gamep g, Levelsp v, Levelp l, Thingp t, int val = 1) -> int;
[[nodiscard]] auto thing_keys_carried_set(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int;
[[nodiscard]] auto thing_keys_carried(Thingp t) -> int;
[[nodiscard]] auto thing_level(Gamep g, Levelsp v, Thingp t) -> Levelp;
[[nodiscard]] auto thing_lifespan_decr(Gamep g, Levelsp v, Levelp l, Thingp t, int val = 1) -> int;
[[nodiscard]] auto thing_lifespan_incr(Gamep g, Levelsp v, Levelp l, Thingp t, int val = 1) -> int;
[[nodiscard]] auto thing_lifespan_set(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int;
[[nodiscard]] auto thing_lifespan(Thingp t) -> int;
[[nodiscard]] auto thing_light_struct(Gamep g, Thingp t) -> ThingLightp;
[[nodiscard]] auto thing_lunge(Gamep g, Levelsp v, Levelp l, Thingp me, const bpoint &to) -> bool;
[[nodiscard]] auto thing_minion_can_move_to_possible(Gamep g, Levelsp v, Levelp l, Thingp me, const bpoint &to) -> bool;
[[nodiscard]] auto thing_minion_choose_target_near_mob(Gamep g, Levelsp v, Levelp l, Thingp me) -> bool;
[[nodiscard]] auto thing_minion_detach_me_from_mob(Gamep g, Levelsp v, Levelp l, Thingp me) -> bool;
[[nodiscard]] auto thing_minion_get_mob_dmap(Gamep g, Levelsp v, Levelp l, Thingp me) -> Dmap *;
[[nodiscard]] auto thing_minion_max_decr(Gamep g, Levelsp v, Levelp l, Thingp t, int val = 1) -> int;
[[nodiscard]] auto thing_minion_max_incr(Gamep g, Levelsp v, Levelp l, Thingp t, int val = 1) -> int;
[[nodiscard]] auto thing_minion_max_set(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int;
[[nodiscard]] auto thing_minion_max(Thingp t) -> int;
[[nodiscard]] auto thing_minion_mob_get(Gamep g, Levelsp v, Levelp l, Thingp me) -> Thingp;
[[nodiscard]] auto thing_mob_detach_all_minions(Gamep g, Levelsp v, Levelp l, Thingp mob) -> bool;
[[nodiscard]] auto thing_mob_detach_minion(Gamep g, Levelsp v, Levelp l, Thingp mob, Thingp minion) -> bool;
[[nodiscard]] auto thing_mob_kill_all_minions(Gamep g, Levelsp v, Levelp l, Thingp mob, ThingEvent &e) -> bool;
[[nodiscard]] auto thing_mob_minion_count_get(Gamep g, Levelsp v, Levelp l, Thingp mob) -> int;
[[nodiscard]] auto thing_mob_spawn_a_minion(Gamep g, Levelsp v, Levelp l, Thingp mob, Tpp tp_minion) -> Thingp;
[[nodiscard]] auto thing_move_path_apply(Gamep g, Levelsp v, Levelp l, Thingp me, const std::vector< bpoint > &move_path) -> bool;
[[nodiscard]] auto thing_move_path_confirmed(Gamep g, Levelsp v, Levelp l, Thingp me, const std::vector< bpoint > &move_path) -> bool;
[[nodiscard]] auto thing_move_path_pop(Gamep g, Levelsp v, Levelp l, Thingp me, bool &move_confirmed, bpoint &out) -> bool;
[[nodiscard]] auto thing_move_path_pop(Gamep g, Levelsp v, Levelp l, Thingp me, bpoint &out) -> bool;
[[nodiscard]] auto thing_move_path_size(Gamep g, Levelsp v, Levelp l, Thingp me) -> int;
[[nodiscard]] auto thing_move_path_target(Gamep g, Levelsp v, Levelp l, Thingp me, bpoint &out) -> bool;
[[nodiscard]] auto thing_move_remaining_decr(Gamep g, Levelsp v, Levelp l, Thingp t, int val = 1) -> int;
[[nodiscard]] auto thing_move_remaining_incr(Gamep g, Levelsp v, Levelp l, Thingp t, int val = 1) -> int;
[[nodiscard]] auto thing_move_remaining_set(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int;
[[nodiscard]] auto thing_move_remaining(Thingp t) -> int;
[[nodiscard]] auto thing_move_to_next(Gamep, Levelsp, Levelp, Thingp) -> bool;
[[nodiscard]] auto thing_move_to(Gamep g, Levelsp v, Levelp l, Thingp me, bpoint to) -> bool;
[[nodiscard]] auto thing_moving_from(Thingp t) -> bpoint;
[[nodiscard]] auto thing_name_a_or_an(Thingp t) -> std::string;
[[nodiscard]] auto thing_name_a_or_an(Thingp, ThingTextFlags) -> std::string;
[[nodiscard]] auto thing_name_apostrophize(Thingp t) -> std::string;
[[nodiscard]] auto thing_name_apostrophize(Thingp, ThingTextFlags) -> std::string;
[[nodiscard]] auto thing_name_long_the(Gamep g, Levelsp v, Levelp l, Thingp t, ThingTextFlags flags = 0) -> std::string;
[[nodiscard]] auto thing_name_long(Gamep g, Levelsp v, Levelp l, Thingp t, ThingTextFlags flags = 0) -> std::string;
[[nodiscard]] auto thing_name_pluralize(Thingp t) -> std::string;
[[nodiscard]] auto thing_name_pluralize(Thingp, ThingTextFlags) -> std::string;
[[nodiscard]] auto thing_name_short_the(Gamep g, Levelsp v, Levelp l, Thingp t, ThingTextFlags flags = 0) -> std::string;
[[nodiscard]] auto thing_name_short(Gamep g, Levelsp v, Levelp l, Thingp t, ThingTextFlags flags = 0) -> std::string;
[[nodiscard]] auto thing_old_at(Thingp t) -> bpoint;
[[nodiscard]] auto thing_on_same_level_as_player(Gamep g, Levelsp v, Thingp t) -> bool;
[[nodiscard]] auto thing_open(Gamep g, Levelsp v, Levelp l, Thingp me, Thingp opener) -> bool;
[[nodiscard]] auto thing_path_cost(Gamep g, Levelsp v, Levelp l, Thingp me, const std::vector< bpoint > &path) -> int;
[[nodiscard]] auto thing_player_level(Gamep g) -> Levelp;
[[nodiscard]] auto thing_player_mouse_down(Gamep g, Levelsp v, Levelp l, int x, int y, uint32_t button) -> bool;
[[nodiscard]] auto thing_player_struct(Gamep g) -> ThingPlayerp;
[[nodiscard]] auto thing_player(Gamep g) -> Thingp;
[[nodiscard]] auto thing_pop(Gamep g, Levelsp v, Thingp t) -> bool;
[[nodiscard]] auto thing_prev_pix_at(Thingp t) -> spoint;
[[nodiscard]] auto thing_priority_set(Gamep g, Levelsp v, Levelp l, Thingp t, ThingPriorityType val) -> ThingPriorityType;
[[nodiscard]] auto thing_priority(Thingp t) -> ThingPriorityType;
[[nodiscard]] auto thing_projectile_count_get(Gamep g, Levelsp v, Levelp l, Thingp me) -> int;
[[nodiscard]] auto thing_projectile_detach_all_fired(Gamep g, Levelsp v, Levelp l, Thingp me) -> bool;
[[nodiscard]] auto thing_projectile_detach_me_from_firer(Gamep g, Levelsp v, Levelp l, Thingp me) -> bool;
[[nodiscard]] auto thing_projectile_fire_at(Gamep g, Levelsp v, Levelp l, Thingp me, Tpp what, bpoint target) -> bool;
[[nodiscard]] auto thing_projectile_fire_at(Gamep g, Levelsp v, Levelp l, Thingp me, Tpp what, fpoint target) -> bool;
[[nodiscard]] auto thing_projectile_fired_by_get(Gamep g, Levelsp v, Levelp l, Thingp me) -> Thingp;
[[nodiscard]] auto thing_projectile_get_direction(Gamep g, Levelsp v, Levelp l, Thingp t) -> fpoint;
[[nodiscard]] auto thing_projectile_kill_all_fired(Gamep g, Levelsp v, Levelp l, Thingp me, ThingEvent &e) -> bool;
[[nodiscard]] auto thing_projectile_max_decr(Gamep g, Levelsp v, Levelp l, Thingp t, int val = 1) -> int;
[[nodiscard]] auto thing_projectile_max_incr(Gamep g, Levelsp v, Levelp l, Thingp t, int val = 1) -> int;
[[nodiscard]] auto thing_projectile_max_set(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int;
[[nodiscard]] auto thing_projectile_max(Thingp t) -> int;
[[nodiscard]] auto thing_push(Gamep g, Levelsp v, Levelp l, Thingp t) -> bool;
[[nodiscard]] auto thing_real_at(Thingp t) -> fpoint;
[[nodiscard]] auto thing_score_decr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int;
[[nodiscard]] auto thing_score_incr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int;
[[nodiscard]] auto thing_score_set(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int;
[[nodiscard]] auto thing_score_value_decr(Gamep g, Levelsp v, Levelp l, Thingp t, int val = 1) -> int;
[[nodiscard]] auto thing_score_value_incr(Gamep g, Levelsp v, Levelp l, Thingp t, int val = 1) -> int;
[[nodiscard]] auto thing_score_value_set(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int;
[[nodiscard]] auto thing_score_value(Thingp t) -> int;
[[nodiscard]] auto thing_score(Gamep g, Thingp t) -> int;
[[nodiscard]] auto thing_shove_handle(Gamep g, Levelsp v, Levelp l, Thingp shover, bpoint at) -> bool;
[[nodiscard]] auto thing_shove_to(Gamep g, Levelsp v, Levelp l, Thingp me, bpoint to) -> bool;
[[nodiscard]] auto thing_spawn_a_projectile(Gamep g, Levelsp v, Levelp l, Thingp me, Tpp tp_projectile) -> Thingp;
[[nodiscard]] auto thing_spawn(Gamep g, Levelsp v, Levelp l, Tpp tp, const bpoint &at) -> Thingp;
[[nodiscard]] auto thing_spawn(Gamep g, Levelsp v, Levelp l, Tpp tp, const fpoint &at) -> Thingp;
[[nodiscard]] auto thing_spawn(Gamep g, Levelsp v, Levelp l, Tpp tp, Thingp spawner) -> Thingp;
[[nodiscard]] auto thing_speed_set(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int;
[[nodiscard]] auto thing_speed(Thingp t) -> int;
[[nodiscard]] auto thing_submerged_pct_decr(Gamep g, Levelsp v, Levelp l, Thingp t, int val = 1) -> int;
[[nodiscard]] auto thing_submerged_pct_incr(Gamep g, Levelsp v, Levelp l, Thingp t, int val = 1) -> int;
[[nodiscard]] auto thing_submerged_pct_set(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int;
[[nodiscard]] auto thing_target(Thingp t) -> bpoint;
[[nodiscard]] auto thing_teleport_handle(Gamep g, Levelsp v, Levelp l, Thingp me) -> bool;
[[nodiscard]] auto thing_temperature_decr(Gamep g, Levelsp v, Levelp l, Thingp t, int val = 1) -> int;
[[nodiscard]] auto thing_temperature_incr(Gamep g, Levelsp v, Levelp l, Thingp t, int val = 1) -> int;
[[nodiscard]] auto thing_temperature_set(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int;
[[nodiscard]] auto thing_temperature(Thingp t) -> int;
[[nodiscard]] auto thing_value1_decr(Gamep g, Levelsp v, Levelp l, Thingp t, int val = 1) -> int;
[[nodiscard]] auto thing_value1_incr(Gamep g, Levelsp v, Levelp l, Thingp t, int val = 1) -> int;
[[nodiscard]] auto thing_value1_set(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int;
[[nodiscard]] auto thing_value1(Thingp t) -> int;
[[nodiscard]] auto thing_value10_decr(Gamep g, Levelsp v, Levelp l, Thingp t, int val = 1) -> int;
[[nodiscard]] auto thing_value10_incr(Gamep g, Levelsp v, Levelp l, Thingp t, int val = 1) -> int;
[[nodiscard]] auto thing_value10_set(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int;
[[nodiscard]] auto thing_value10(Thingp t) -> int;
[[nodiscard]] auto thing_value11_decr(Gamep g, Levelsp v, Levelp l, Thingp t, int val = 1) -> int;
[[nodiscard]] auto thing_value11_incr(Gamep g, Levelsp v, Levelp l, Thingp t, int val = 1) -> int;
[[nodiscard]] auto thing_value11_set(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int;
[[nodiscard]] auto thing_value11(Thingp t) -> int;
[[nodiscard]] auto thing_value12_decr(Gamep g, Levelsp v, Levelp l, Thingp t, int val = 1) -> int;
[[nodiscard]] auto thing_value12_incr(Gamep g, Levelsp v, Levelp l, Thingp t, int val = 1) -> int;
[[nodiscard]] auto thing_value12_set(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int;
[[nodiscard]] auto thing_value12(Thingp t) -> int;
[[nodiscard]] auto thing_value13_decr(Gamep g, Levelsp v, Levelp l, Thingp t, int val = 1) -> int;
[[nodiscard]] auto thing_value13_incr(Gamep g, Levelsp v, Levelp l, Thingp t, int val = 1) -> int;
[[nodiscard]] auto thing_value13_set(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int;
[[nodiscard]] auto thing_value13(Thingp t) -> int;
[[nodiscard]] auto thing_value14_decr(Gamep g, Levelsp v, Levelp l, Thingp t, int val = 1) -> int;
[[nodiscard]] auto thing_value14_incr(Gamep g, Levelsp v, Levelp l, Thingp t, int val = 1) -> int;
[[nodiscard]] auto thing_value14_set(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int;
[[nodiscard]] auto thing_value14(Thingp t) -> int;
[[nodiscard]] auto thing_value15_decr(Gamep g, Levelsp v, Levelp l, Thingp t, int val = 1) -> int;
[[nodiscard]] auto thing_value15_incr(Gamep g, Levelsp v, Levelp l, Thingp t, int val = 1) -> int;
[[nodiscard]] auto thing_value15_set(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int;
[[nodiscard]] auto thing_value15(Thingp t) -> int;
[[nodiscard]] auto thing_value16_decr(Gamep g, Levelsp v, Levelp l, Thingp t, int val = 1) -> int;
[[nodiscard]] auto thing_value16_incr(Gamep g, Levelsp v, Levelp l, Thingp t, int val = 1) -> int;
[[nodiscard]] auto thing_value16_set(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int;
[[nodiscard]] auto thing_value16(Thingp t) -> int;
[[nodiscard]] auto thing_value17_decr(Gamep g, Levelsp v, Levelp l, Thingp t, int val = 1) -> int;
[[nodiscard]] auto thing_value17_incr(Gamep g, Levelsp v, Levelp l, Thingp t, int val = 1) -> int;
[[nodiscard]] auto thing_value17_set(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int;
[[nodiscard]] auto thing_value17(Thingp t) -> int;
[[nodiscard]] auto thing_value18_decr(Gamep g, Levelsp v, Levelp l, Thingp t, int val = 1) -> int;
[[nodiscard]] auto thing_value18_incr(Gamep g, Levelsp v, Levelp l, Thingp t, int val = 1) -> int;
[[nodiscard]] auto thing_value18_set(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int;
[[nodiscard]] auto thing_value18(Thingp t) -> int;
[[nodiscard]] auto thing_value19_decr(Gamep g, Levelsp v, Levelp l, Thingp t, int val = 1) -> int;
[[nodiscard]] auto thing_value19_incr(Gamep g, Levelsp v, Levelp l, Thingp t, int val = 1) -> int;
[[nodiscard]] auto thing_value19_set(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int;
[[nodiscard]] auto thing_value19(Thingp t) -> int;
[[nodiscard]] auto thing_value2_decr(Gamep g, Levelsp v, Levelp l, Thingp t, int val = 1) -> int;
[[nodiscard]] auto thing_value2_incr(Gamep g, Levelsp v, Levelp l, Thingp t, int val = 1) -> int;
[[nodiscard]] auto thing_value2_set(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int;
[[nodiscard]] auto thing_value2(Thingp t) -> int;
[[nodiscard]] auto thing_value3_decr(Gamep g, Levelsp v, Levelp l, Thingp t, int val = 1) -> int;
[[nodiscard]] auto thing_value3_incr(Gamep g, Levelsp v, Levelp l, Thingp t, int val = 1) -> int;
[[nodiscard]] auto thing_value3_set(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int;
[[nodiscard]] auto thing_value3(Thingp t) -> int;
[[nodiscard]] auto thing_value4_decr(Gamep g, Levelsp v, Levelp l, Thingp t, int val = 1) -> int;
[[nodiscard]] auto thing_value4_incr(Gamep g, Levelsp v, Levelp l, Thingp t, int val = 1) -> int;
[[nodiscard]] auto thing_value4_set(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int;
[[nodiscard]] auto thing_value4(Thingp t) -> int;
[[nodiscard]] auto thing_value5_decr(Gamep g, Levelsp v, Levelp l, Thingp t, int val = 1) -> int;
[[nodiscard]] auto thing_value5_incr(Gamep g, Levelsp v, Levelp l, Thingp t, int val = 1) -> int;
[[nodiscard]] auto thing_value5_set(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int;
[[nodiscard]] auto thing_value5(Thingp t) -> int;
[[nodiscard]] auto thing_value6_decr(Gamep g, Levelsp v, Levelp l, Thingp t, int val = 1) -> int;
[[nodiscard]] auto thing_value6_incr(Gamep g, Levelsp v, Levelp l, Thingp t, int val = 1) -> int;
[[nodiscard]] auto thing_value6_set(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int;
[[nodiscard]] auto thing_value6(Thingp t) -> int;
[[nodiscard]] auto thing_value7_decr(Gamep g, Levelsp v, Levelp l, Thingp t, int val = 1) -> int;
[[nodiscard]] auto thing_value7_incr(Gamep g, Levelsp v, Levelp l, Thingp t, int val = 1) -> int;
[[nodiscard]] auto thing_value7_set(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int;
[[nodiscard]] auto thing_value7(Thingp t) -> int;
[[nodiscard]] auto thing_value8_decr(Gamep g, Levelsp v, Levelp l, Thingp t, int val = 1) -> int;
[[nodiscard]] auto thing_value8_incr(Gamep g, Levelsp v, Levelp l, Thingp t, int val = 1) -> int;
[[nodiscard]] auto thing_value8_set(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int;
[[nodiscard]] auto thing_value8(Thingp t) -> int;
[[nodiscard]] auto thing_value9_decr(Gamep g, Levelsp v, Levelp l, Thingp t, int val = 1) -> int;
[[nodiscard]] auto thing_value9_incr(Gamep g, Levelsp v, Levelp l, Thingp t, int val = 1) -> int;
[[nodiscard]] auto thing_value9_set(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int;
[[nodiscard]] auto thing_value9(Thingp t) -> int;
[[nodiscard]] auto thing_variant_set(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int;
[[nodiscard]] auto thing_variant(Thingp t) -> int;
[[nodiscard]] auto thing_vision_blocker(Gamep g, Levelsp v, Levelp l, Thingp me, Thingp it) -> bool;
[[nodiscard]] auto thing_vision_can_see_tile(Gamep g, Levelsp v, Levelp l, Thingp t, bpoint p) -> bool;
[[nodiscard]] auto thing_warp_to(Gamep g, Levelsp v, Levelp new_level, Thingp me, bpoint to) -> bool;
[[nodiscard]] auto thing_weight_set(Gamep g, Levelsp v, Levelp l, Thingp t, uint32_t val) -> int;
[[nodiscard]] auto thing_weight(Thingp t) -> int;
[[nodiscard]] auto to_death_reason_string(Gamep g, Levelsp v, Levelp l, Thingp t, ThingEvent &e) -> std::string;
[[nodiscard]] auto to_string(Gamep g, Levelsp v, Levelp l, ThingEvent &e) -> std::string;
[[nodiscard]] auto to_string(Gamep g, Levelsp v, Levelp l, Thingp t) -> std::string;
[[nodiscard]] auto top_owner(Gamep g, Levelsp v, Levelp l, Thingp t) -> Thingp;
[[nodiscard]] auto wid_get_thing_context(Gamep g, Levelsp v, Widp w, int which) -> Thingp;
// end sort marker1 }

// begin sort marker2 {
auto astar_solve(Gamep g, Levelsp v, Levelp l, Thingp me, bpoint src, bpoint dst) -> std::vector< bpoint >;
void level_botcon(Gamep g, Levelsp v, Levelp l, const char *fmt, ...) CHECK_FORMAT_STR(printf, 4, 5);
void level_con(Gamep g, Levelsp v, Levelp l, const char *fmt, ...) CHECK_FORMAT_STR(printf, 4, 5);
void level_dbg(Gamep g, Levelsp v, Levelp l, const char *fmt, ...) CHECK_FORMAT_STR(printf, 4, 5);
void level_err(Gamep g, Levelsp v, Levelp l, const char *fmt, ...) CHECK_FORMAT_STR(printf, 4, 5);
void level_log(Gamep g, Levelsp v, Levelp l, const char *fmt, ...) CHECK_FORMAT_STR(printf, 4, 5);
void level_topcon(Gamep g, Levelsp v, Levelp l, const char *fmt, ...) CHECK_FORMAT_STR(printf, 4, 5);
void level_vision_calculate_all(Gamep g, Levelsp v, Levelp l);
void level_warn(Gamep g, Levelsp v, Levelp l, const char *fmt, ...) CHECK_FORMAT_STR(printf, 4, 5);
void monst_state_change(Gamep g, Levelsp v, Levelp l, Thingp me, MonstState new_state);
void player_collision_handle(Gamep g, Levelsp v, Levelp l, Thingp me);
void player_fell(Gamep g, Levelsp v, Levelp l, Levelp next_level, Thingp me);
void player_map_center(Gamep, Levelsp, Levelp);
void player_move_accum(Gamep g, Levelsp v, Levelp l, bool up, bool down, bool left, bool right, bool fire);
void player_move_requests_reset(Gamep g, Levelsp v);
void player_reached_entrance(Gamep g, Levelsp v, Levelp l);
void player_reached_exit(Gamep g, Levelsp v, Levelp l);
void player_warp_to_specific_level(Gamep g, Levelsp v, LevelNum level_num);
void thing_anim_init(Gamep g, Levelsp v, Levelp l, Thingp t, ThingAnim anim_type);
void thing_anim_time_step(Gamep g, Levelsp v, Levelp l, Thingp t, Tpp tp, int time_step);
void thing_at_set(Gamep g, Levelsp v, Levelp l, Thingp t, const bpoint &val);
void thing_at_set(Gamep g, Levelsp v, Levelp l, Thingp t, const fpoint &val);
void thing_blit_text(Gamep g, Levelsp v, Levelp l, spoint tl, spoint br, std::string const &text, color fg, bool outline);
void thing_botcon(Thingp t, const char *fmt, ...) CHECK_FORMAT_STR(printf, 2, 3);
void thing_can_see_dump(Gamep g, Levelsp v, Levelp l, Thingp t);
void thing_chasm_handle(Gamep g, Levelsp v, Levelp l, Thingp t);
void thing_collision_handle_interpolated(Gamep g, Levelsp v, Levelp l, Thingp me, fpoint old_at);
void thing_collision_handle(Gamep g, Levelsp v, Levelp l, Thingp me);
void thing_con(Thingp t, const char *fmt, ...) CHECK_FORMAT_STR(printf, 2, 3);
void thing_continue_to_burn_check(Gamep g, Levelsp v, Levelp l, Thingp t);
void thing_croak(Thingp t, const char *fmt, ...) CHECK_FORMAT_STR(printf, 2, 3);
void thing_damage(Gamep g, Levelsp v, Levelp l, Thingp t, ThingEvent &e);
void thing_dbg(Thingp t, const char *fmt, ...) CHECK_FORMAT_STR(printf, 2, 3);
void thing_dead(Gamep g, Levelsp v, Levelp l, Thingp me, ThingEvent &e);
void thing_degroup(Gamep g, Levelsp v, Levelp l, Thingp t);
void thing_dir_bl_set(Thingp, uint8_t);
void thing_dir_br_set(Thingp, uint8_t);
void thing_dir_down_set(Thingp, uint8_t);
void thing_dir_left_set(Thingp, uint8_t);
void thing_dir_right_set(Thingp, uint8_t);
void thing_dir_tl_set(Thingp, uint8_t);
void thing_dir_tr_set(Thingp, uint8_t);
void thing_dir_up_set(Thingp, uint8_t);
void thing_dmap(Gamep g, Levelsp v, Levelp l, Thingp me, bool reverse = false);
void thing_dump_projectiles(Gamep g, Levelsp v, Levelp l, Thingp me);
void thing_err(Thingp t, const char *fmt, ...) CHECK_FORMAT_STR(printf, 2, 3);
void thing_explosion_handle(Gamep g, Levelsp v, Levelp l, Thingp t);
void thing_fall_end_check(Gamep g, Levelsp v, Levelp l, Thingp t);
void thing_fall_time_step(Gamep g, Levelsp v, Levelp l, Thingp t, int time_step);
void thing_fall(Gamep g, Levelsp v, Levelp l, Thingp t);
void thing_fini(Gamep g, Levelsp v, Levelp l, Thingp t);
void thing_free(Gamep g, Levelsp v, Levelp l, Thingp t);
void thing_group_join(Gamep g, Levelsp v, Levelp l, Thingp t, Thingp group);
void thing_group_leave(Gamep g, Levelsp v, Levelp l, Thingp t);
void thing_group_member_leave(Gamep g, Levelsp v, Levelp l, Thingp t);
void thing_has_seen_dump(Gamep g, Levelsp v, Levelp l, Thingp t);
void thing_hidden_time_step(Gamep g, Levelsp v, Levelp l, Thingp me, int time_step);
void thing_hit_time_step(Gamep g, Levelsp v, Levelp l, Thingp t, int time_step);
void thing_hot_time_step(Gamep g, Levelsp v, Levelp l, Thingp me, int time_step);
void thing_interpolate(Gamep g, Levelsp v, Levelp l, Thingp t, float dt);
void thing_inventory_dump(Gamep g, Levelsp v, Levelp l, Thingp carrier);
void thing_is_burning_handle(Gamep g, Levelsp v, Levelp l, Thingp t);
void thing_is_burning_set(Gamep g, Levelsp v, Levelp l, Thingp t, bool val = true);
void thing_is_burning_unset(Gamep g, Levelsp v, Levelp l, Thingp t);
void thing_is_corpse_set(Gamep g, Levelsp v, Levelp l, Thingp t, bool val = true);
void thing_is_corpse_unset(Gamep g, Levelsp v, Levelp l, Thingp t);
void thing_is_dead_set(Gamep g, Levelsp v, Levelp l, Thingp t, bool val = true);
void thing_is_dead_unset(Gamep g, Levelsp v, Levelp l, Thingp t);
void thing_is_falling_continues_set(Gamep g, Levelsp v, Levelp l, Thingp t, bool val = true);
void thing_is_falling_continues_unset(Gamep g, Levelsp v, Levelp l, Thingp t);
void thing_is_falling_set(Gamep g, Levelsp v, Levelp l, Thingp t, bool val);
void thing_is_hidden_set(Gamep g, Levelsp v, Levelp l, Thingp me, bool val);
void thing_is_hit_set(Gamep g, Levelsp v, Levelp l, Thingp t, int val);
void thing_is_hot_set(Gamep g, Levelsp v, Levelp l, Thingp me, bool val);
void thing_is_jumping_set(Gamep g, Levelsp v, Levelp l, Thingp me, bool val = true);
void thing_is_jumping_unset(Gamep g, Levelsp v, Levelp l, Thingp me);
void thing_is_lunging_set(Gamep g, Levelsp v, Levelp l, Thingp me, bool val);
void thing_is_moving_set(Gamep g, Levelsp v, Levelp l, Thingp t, bool val = true);
void thing_is_moving_unset(Gamep g, Levelsp v, Levelp l, Thingp t);
void thing_is_on_map_set(Gamep g, Levelsp v, Levelp l, Thingp t, bool val = true);
void thing_is_on_map_unset(Gamep g, Levelsp v, Levelp l, Thingp t);
void thing_is_scheduled_for_cleanup_set(Gamep g, Levelsp v, Levelp l, Thingp t, bool val = true);
void thing_is_scheduled_for_cleanup_unset(Gamep g, Levelsp v, Levelp l, Thingp t);
void thing_is_sleeping_set(Gamep g, Levelsp v, Levelp l, Thingp me, bool val = true);
void thing_is_sleeping_unset(Gamep g, Levelsp v, Levelp l, Thingp me);
void thing_is_spawned_set(Gamep g, Levelsp v, Levelp l, Thingp t, bool val = true);
void thing_is_spawned_unset(Gamep g, Levelsp v, Levelp l, Thingp t);
void thing_is_teleporting_set(Gamep g, Levelsp v, Levelp l, Thingp me, bool val = true);
void thing_is_teleporting_unset(Gamep g, Levelsp v, Levelp l, Thingp me);
void thing_is_unlocked_set(Gamep g, Levelsp v, Levelp l, Thingp t, bool val = true);
void thing_is_unlocked_unset(Gamep g, Levelsp v, Levelp l, Thingp t);
void thing_level_warp_to_entrance(Gamep g, Levelsp v, Levelp new_level, Thingp t);
void thing_level_warp_to_exit(Gamep g, Levelsp v, Levelp new_level, Thingp t);
void thing_log(Thingp t, const char *fmt, ...) CHECK_FORMAT_STR(printf, 2, 3);
void thing_lunge_end_check(Gamep g, Levelsp v, Levelp l, Thingp me);
void thing_lunge_time_step(Gamep g, Levelsp v, Levelp l, Thingp me, int time_step);
void thing_melt(Gamep g, Levelsp v, Levelp l, Thingp t);
void thing_mob_dump_minions(Gamep g, Levelsp v, Levelp l, Thingp mob);
void thing_monst_event_loop(Gamep g, Levelsp v, Levelp l, Thingp me);
void thing_monst_tick(Gamep g, Levelsp v, Levelp l, Thingp me);
void thing_move_or_jump_finish(Gamep g, Levelsp v, Levelp l, Thingp me);
void thing_move_path_confirm(Gamep g, Levelsp v, Levelp l, Thingp me);
void thing_move_path_reset(Gamep g, Levelsp v, Levelp l, Thingp me);
void thing_moving_from_set(Thingp t, const bpoint &val);
void thing_path_shorten(Gamep g, Levelsp v, Levelp l, Thingp t, std::vector< bpoint > &path);
void thing_pix_at_set(Gamep g, Levelsp v, Levelp l, Thingp t, const spoint &val);
void thing_pix_at_set(Gamep g, Levelsp v, Levelp l, Thingp t, short x, short y);
void thing_player_event_loop(Gamep g, Levelsp v, Levelp l);
void thing_player_init(Gamep g);
void thing_prev_pix_at_set(Gamep g, Levelsp v, Levelp l, Thingp t, const spoint &val);
void thing_projectile_move(Gamep g, Levelsp v, Levelp l, Thingp t, float dt);
void thing_set_dir_from_delta(Thingp me, const bpoint &p);
void thing_set_dir_from_delta(Thingp me, int dx, int dy);
void thing_sound_play(Gamep g, Levelsp v, Levelp l, Thingp t, const std::string &alias);
void thing_stats_dump(Gamep g, Levelsp v);
void thing_submerged_update(Gamep g, Levelsp v, Levelp l, Thingp t);
void thing_target_set(Gamep g, Levelsp v, Levelp l, Thingp t, const bpoint &val);
void thing_temperature_damage_handle(Gamep g, Levelsp v, Levelp l, Thingp source, Thingp t, int n);
void thing_temperature_handle(Gamep g, Levelsp v, Levelp l, Thingp source, Thingp t, int n);
void thing_tick_begin(Gamep g, Levelsp v, Levelp l, Thingp t);
void thing_tick_end(Gamep g, Levelsp v, Levelp l, Thingp t);
void thing_tick_idle(Gamep g, Levelsp v, Levelp l, Thingp t);
void thing_topcon(Thingp t, const char *fmt, ...) CHECK_FORMAT_STR(printf, 2, 3);
void thing_update_pos(Gamep g, Levelsp v, Levelp l, Thingp me);
void thing_vision_calculate(Gamep g, Levelsp v, Levelp l, Thingp me);
void thing_vision_reset(Gamep g, Levelsp v, Levelp l, Thingp t);
void thing_warn(Thingp t, const char *fmt, ...) CHECK_FORMAT_STR(printf, 2, 3);
void thing_water_handle(Gamep g, Levelsp v, Levelp l, Thingp t);
void tp_collision_init(Tpp tp);
void tp_display_init(Tpp tp);
void tp_temperature_init(Tpp tp);
void wid_set_thing_context(Gamep g, Levelsp v, Widp w, Thingp t);
void wid_thing_info(Gamep g, Levelsp v, Levelp l, Thingp t, WidPopup *parent, int width);
void wid_unset_thing_context(Gamep g, Levelsp v, Widp w, Thingp t);
// end sort marker2 }

void thing_display_get_tile_info(Gamep g, Levelsp v, Levelp l, const bpoint &p, Tpp tp_maybe_null, Thingp t_maybe_null, spoint &tl,
                                 spoint &br, uint16_t *tile_index);

void thing_display(Gamep g, Levelsp v, Levelp l, const bpoint &p, Tpp tp, Thingp t_maybe_null, spoint tl, spoint br, uint16_t tile_index,
                   FboEnum fbo);

#define FOR_ALL_MINION_SLOTS(_g_, _v_, _l_, _mob_, _slot_, _minion_)                                                                       \
  if ((_g_) && (_v_) && (_l_))                                                                                                             \
    for (auto _ext_ = thing_ext_struct(_g_, _mob_); _ext_; _ext_ = nullptr)                                                                \
      for (auto _n_ = 0; _n_ < THING_MINION_MAX; _n_++)                                                                                    \
        for (AUTO(_slot_) = &_ext_->minions.minion[ _n_ ]; _slot_; (_slot_) = nullptr)                                                     \
          for (AUTO(_minion_) = thing_find_optional(g, v, (_slot_)->minion_id), loop2 = (Thingp) 1; loop2 == (Thingp) 1; loop2 = (Thingp) 0)

#define FOR_ALL_MINIONS(_g_, _v_, _l_, _mob_, _minion_)                                                                                    \
  if ((_g_) && (_v_) && (_l_))                                                                                                             \
    for (auto _ext_ = thing_ext_struct(_g_, _mob_); _ext_; _ext_ = nullptr)                                                                \
      for (auto _n_ = 0; _n_ < THING_MINION_MAX; _n_++)                                                                                    \
        for (auto _slot_ = &_ext_->minions.minion[ _n_ ]; _slot_; _slot_ = nullptr)                                                        \
          for (Thingp _minion_ = thing_find_optional(g, v, _slot_->minion_id); _minion_; (_minion_) = nullptr)

#define FOR_ALL_PROJECTILE_SLOTS(_g_, _v_, _l_, _mob_, _slot_, _projectile_)                                                               \
  if ((_g_) && (_v_) && (_l_))                                                                                                             \
    for (auto _ext_ = thing_ext_struct(_g_, _mob_); _ext_; _ext_ = nullptr)                                                                \
      for (auto _n_ = 0; _n_ < THING_PROJECTILE_MAX; _n_++)                                                                                \
        for (AUTO(_slot_) = &_ext_->projectiles.projectile[ _n_ ]; _slot_; (_slot_) = nullptr)                                             \
          for (AUTO(_projectile_) = thing_find_optional(g, v, (_slot_)->projectile_id), loop2 = (Thingp) 1; loop2 == (Thingp) 1;           \
               loop2 = (Thingp) 0)

#define FOR_ALL_PROJECTILES(_g_, _v_, _l_, _mob_, _projectile_)                                                                            \
  if ((_g_) && (_v_) && (_l_))                                                                                                             \
    for (auto _ext_ = thing_ext_struct(_g_, _mob_); _ext_; _ext_ = nullptr)                                                                \
      for (auto _n_ = 0; _n_ < THING_PROJECTILE_MAX; _n_++)                                                                                \
        for (auto _slot_ = &_ext_->projectiles.projectile[ _n_ ]; _slot_; _slot_ = nullptr)                                                \
          for (Thingp _projectile_ = thing_find_optional(g, v, _slot_->projectile_id); _projectile_; (_projectile_) = nullptr)

#define FOR_ALL_INVENTORY_SLOTS(_g_, _v_, _l_, _owner_, _slot_, _item_)                                                                    \
  if ((_g_) && (_v_) && (_l_))                                                                                                             \
    for (auto _ext_ = thing_player_struct(_g_); _ext_; _ext_ = nullptr)                                                                    \
      for (auto _n_ = 0; _n_ < THING_INVENTORY_MAX; _n_++)                                                                                 \
        for (AUTO(_slot_) = &_ext_->inventory.slots[ _n_ ]; _slot_; (_slot_) = nullptr)                                                    \
          for (AUTO(_item_) = thing_find_optional(g, v, (_slot_)->item_id), loop2 = (Thingp) 1; loop2 == (Thingp) 1; loop2 = (Thingp) 0)

#define FOR_ALL_INVENTORY_ITEMS(_g_, _v_, _l_, _owner_, _item_)                                                                            \
  if ((_g_) && (_v_) && (_l_))                                                                                                             \
    for (auto _ext_ = thing_player_struct(_g_); _ext_; _ext_ = nullptr)                                                                    \
      for (auto _n_ = 0; _n_ < THING_INVENTORY_MAX; _n_++)                                                                                 \
        for (auto _slot_ = &_ext_->inventory.slots[ _n_ ]; _slot_; _slot_ = nullptr)                                                       \
          for (AUTO(_item_) = thing_find_optional(g, v, _slot_->item_id); _item_; (_item_) = nullptr)

#define THING_DBG IF_DEBUG thing_dbg
#define LEVEL_DBG IF_DEBUG level_dbg

#endif
