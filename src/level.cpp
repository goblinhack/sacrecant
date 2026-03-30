//
// Copyright goblinhack@gmail.com
//

#include "my_charmap.hpp"
#include "my_globals.hpp"
#include "my_level.hpp"
#include "my_level_inlines.hpp"
#include "my_main.hpp"
#include "my_test.hpp"
#include "my_thing_inlines.hpp"
#include "my_tp_class.hpp"
#include "my_ui.hpp"
#include "my_wids.hpp"

#include <algorithm>
#include <format>

LevelOpt g_level_opt;

void level_verify(Gamep g, Levelsp v, Levelp l)
{
  TRACE();

  VERIFY(MTYPE_GAME, g);
  VERIFY(MTYPE_LEVELS, v);

  FOR_ALL_LEVELS(g, v, o)
  {
    if (o == l) {
      return;
    }
  }

  CROAK("unknown level pointer %p", (void *) l);
}

//
// Are we on the level selection level?
//
auto level_is_level_select(Gamep g, Levelsp v, Levelp l) -> bool
{
  TRACE();

  if ((g == nullptr) || (v == nullptr) || (l == nullptr)) {
    return false;
  }

  return l->level_num == LEVEL_SELECT_ID;
}

//
// Print a level string
//
static void level_dump(Gamep g, Levelp l, int w, int h, std::string s)
{
  TRACE();

  CON("Level: %u", l->level_num + 1);
  for (int y = 0; y < h; y++) {
    std::string line;

    for (int x = 0; x < w; x++) {
      auto offset = (w * y) + x;
      auto c      = s[ offset ];
      if (c != 0) {
        line += c;
      }
    }

    if (! line.empty()) {
      CON("[%s]", line.c_str());
    }
  }
}

//
// Print a level string
//
void level_dump(Gamep g, Levelsp v, Levelp l, int w, int h)
{
  TRACE();

  std::string const s = level_string(g, v, l, w, h);

  level_dump(g, l, w, h, s);
}

//
// Compare level contents
//
auto level_match_contents(Gamep g, Levelsp v, Levelp l, Testp t, int w, int h, const char *expected) -> bool
{
  TRACE();

  level_log(g, v, l, "match");
  TRACE_INDENT();

  std::string found = level_string(g, v, l, w, h);

  for (int y = 0; y < h; y++) {
    std::string const line;

    for (int x = 0; x < w; x++) {
      auto offset = (w * y) + x;
      auto c      = found[ offset ];
      auto e      = expected[ offset ];
      if (c != e) {
        CON_NEW_LINE();
        CON("Expected:");
        level_dump(g, l, w, h, expected);
        CON_NEW_LINE();
        CON("Found:");
        level_dump(g, v, l, w, h);
        CON_NEW_LINE();
        TEST_CON(t, "Level contents not as expected");
        return false;
      }
    }
  }

  //
  // Check for stuff outside the level
  //
  for (int y = 0; y < MAP_HEIGHT; y++) {
    std::string const line;

    if (y < h) {
      continue;
    }

    for (int x = 0; x < MAP_WIDTH; x++) {
      if (x < w) {
        continue;
      }
      bpoint p(x, y);

      FOR_ALL_THINGS_AT(g, v, l, unused, p)
      {
        level_dump(g, v, l, MAP_WIDTH, MAP_HEIGHT);
        CON_NEW_LINE();
        TEST_CON(t, "Level contents found outside of test area");
        return false;
      }
    }
  }

  return true;
}

void level_init(Gamep g, Levelsp v, Levelp l, LevelNum n)
{
  TRACE();

  memset(l, 0, SIZEOF(*l));

  l->is_initialized = true;
  l->level_num      = n;
}

auto levels_memory_alloc(Gamep g) -> Levelsp
{
  LOG("levels alloc memory");
  TRACE_INDENT();

  auto *v = game_levels_get(g);
  if (v != nullptr) {
    return v;
  }

  //
  // Allocate the level as a flat C structure to allow history rewind
  //
  // NOTE: if we use "new" here, the initialization is visibly slower.
  // DO NOT USE C++ classes or types
  //
  v = static_cast< Levelsp >(MYZALLOC(SIZEOF(*v), "v"));
  if (v == nullptr) {
    return nullptr;
  }
  NEWPTR(MTYPE_LEVELS, v, "levels");

  return v;
}

static void levels_memory_free(Gamep g, Levelsp v)
{
  LOG("levels free memory");
  TRACE_INDENT();

  VERIFY(MTYPE_LEVELS, v);
  MYFREE(v);
  OLDPTR(MTYPE_LEVELS, v);

  (void) game_levels_set(g, nullptr);
}

void levels_destroy(Gamep g, Levelsp v)
{
  LOG("levels destroy");
  TRACE_INDENT();

  FOR_ALL_LEVELS(g, v, iter)
  {
    TRACE();
    level_destroy(g, v, iter);
  }

  levels_memory_free(g, v);
}

void level_finalize(Gamep g, Levelsp v, Levelp l)
{
  TRACE();

  (void) level_select_calculate_next_level_down(g, v, l, true);
  v->level_count++;
}

void levels_finalize(Gamep g, Levelsp v)
{
  LOG("levels finalize");
  TRACE_INDENT();

  if (g == nullptr) {
    CROAK("no game pointer");
  }

  if (v == nullptr) {
    CROAK("no levels pointer");
  }

  v->level_count = 0;
  FOR_ALL_LEVELS(g, v, iter)
  {
    TRACE();
    level_finalize(g, v, iter);
  }
}

void level_enter(Gamep g, Levelsp v, Levelp l)
{
  level_log(g, v, l, "Level entered");
  TRACE_INDENT();

  l->player_has_entered_level = true;
}

void level_is_completed_by_player_exiting(Gamep g, Levelsp v, Levelp l)
{
  //
  // Clear signs of previous failure
  //
  l->player_fell_out_of_level = false;

  if (l->player_completed_level_via_exit) {
    return;
  }
  l->player_completed_level_via_exit = true;

  level_log(g, v, l, "Level completed");
  TRACE_INDENT();

  auto *player_struct = thing_player_struct(g);
  if (player_struct != nullptr) {
    player_struct->levels_completed++;
  }
}

void level_is_completed_by_player_falling(Gamep g, Levelsp v, Levelp l)
{
  l->player_fell_out_of_level = true;

  TOPCON(UI_IMPORTANT_FMT_STR "You tumble into the void." UI_RESET_FMT);
}

//
// Change the current level
//
auto level_change(Gamep g, Levelsp v, LevelNum level_num) -> Levelp
{
  LOG("level change to %u", level_num);
  TRACE_INDENT();

  VERIFY(MTYPE_LEVELS, v);

  //
  // Check we're not trying to jump off the end of the levels
  //
  LevelSelect const *s = &v->level_select;
  if (s == nullptr) {
    ERR("no level selection created");
    return nullptr;
  }

  if (level_num == LEVEL_SELECT_ID) {
    //
    // Enter level selectionm
    //
  } else if ((level_num > s->level_count) && (static_cast< bool >(s->level_count))) {
    //
    // Jump tot the last real level
    //
    level_num = s->level_count - 1;
    LOG("level change to %u (max level)", level_num);
  }

  Level *old_level = game_level_get(g, v); // NOLINT
  if (game_level_populate(g, v, level_num) == nullptr) {
    TOPCON("The dungeon is still under construction and cannot be entered (BUG)");
    return old_level;
  }

  VERIFY(MTYPE_LEVELS, v);

  Level *new_level = game_level_get(g, v);

  if (old_level == new_level) {
    if (new_level->level_num == 0) {
      TOPCON("Welcome to " UI_IMPORTANT_FMT_STR "Gorget" UI_RESET_FMT ". A literal work in progress...");
    }
    return new_level;
  }

  level_scroll_warp_to_focus(g, v, new_level);
  level_debug(g, v, new_level);

  if (level_is_level_select(g, v, new_level)) {
    BOTCON_NEW_LINE();
    TOPCON_NEW_LINE();
    TOPCON("Choose your next level.");
    TOPCON("Mouse over levels for monster/treasure info.");
  } else if (new_level->player_completed_level_via_exit) {
    TOPCON_NEW_LINE();
    TOPCON("You re-enter level %u of dungeon %s.", new_level->level_num + 1, game_seed_name_get(g));
  } else if (new_level->player_fell_out_of_level) {
    TOPCON_NEW_LINE();
    TOPCON("You stumble back into level %u of dungeon %s.", new_level->level_num + 1, game_seed_name_get(g));
  } else if (level_num > 0) {
    TOPCON_NEW_LINE();
    TOPCON("You enter level %u of dungeon %s.", new_level->level_num + 1, game_seed_name_get(g));
  }

  level_update_visibility(g, v, new_level);
  game_request_to_remake_ui_set(g);

  return new_level;
}

void level_destroy(Gamep g, Levelsp v, Levelp l)
{
  TRACE();

  if ((l == nullptr) || ! l->is_initialized) {
    return;
  }

  IF_DEBUG2
  {
    DBG("Level destroy %u dump all", l->level_num);
    FOR_ALL_THINGS_ON_LEVEL(g, v, l, t)
    {
      //
      THING_DBG(t, "dump");
    }
  }

  //
  // Remove all things
  //
  DBG("Level destroy %u", l->level_num);
  FOR_ALL_THINGS_ON_LEVEL(g, v, l, t)
  {
    TRACE();
    thing_fini(g, v, l, t);
  }

  //
  // Check all things are gone
  //
  FOR_ALL_THINGS_ON_LEVEL(g, v, l, t)
  {
    //
    thing_err(t, "thing still on level after destroying it");
  }

  //
  // Clean up the level select snake walk
  //
  if (level_is_level_select(g, v, l)) {
    level_select_destroy(g, v, l);
  }

  memset(l, 0, SIZEOF(*l));
}

auto level_populate_thing_id_at(Gamep g, Levelsp v, Levelp l, const bpoint &p, int slot, ThingId id) -> bool
{
  TRACE();

  if (is_oob(p)) [[unlikely]] {
    return false;
  }

  l->thing_id[ p.x ][ p.y ][ slot ] = id;
  return true;
}

auto level_get_thing_id_at(Gamep g, Levelsp v, Levelp l, const bpoint &p, int slot) -> ThingId
{
  TRACE();

  if (is_oob(p)) [[unlikely]] {
    return 0;
  }

  return l->thing_id[ p.x ][ p.y ][ slot ];
}

//
// Additional level flag filters e.g. open doors are not obstacles
//
[[nodiscard]] static auto level_flag_filter(Gamep g, Levelsp v, Levelp l, ThingFlag f, Thingp me, Thingp it) -> bool
{
  TRACE_DEBUG();

  if (it == nullptr) {
    return true;
  }

  switch (f) {
    case is_obs_to_vision :
      if (! thing_vision_blocker(g, v, l, me, it)) {
        return true; // filter out i.e. ignore
      }
      return false;

    case is_obs_to_jumping_onto :
      //
      // Allow players to land on chasms intentionally. Monsters don't want to.
      //
      if (thing_is_chasm(it)) {
        if ((me != nullptr) && thing_is_player(me)) {
          return true; // filter out i.e. ignore
        }
      }

      [[fallthrough]];

    case is_obs_to_cursor_path :
    case is_obs_to_explosion :
    case is_obs_to_falling_onto :
    case is_obs_to_fire :
    case is_obs_to_jump_over :
    case is_obs_to_teleporting_onto :
    case is_obs_to_movement :
      //
      // Should be able to move onto dead things
      //
      if (thing_is_dead(it)) {
        if (! thing_is_obs_when_dead(it)) {
          return true; // filter out i.e. ignore
        }
      }

      //
      // Should be able to move onto open things like doors
      //
      if (thing_is_open(it)) {
        return true; // filter out i.e. ignore
      }
      break;

    case is_able_to_fall :
      if (thing_is_falling(it) != 0) {
        return true; // filter out i.e. ignore
      }
      break;
    default : break;
  }

  return false;
}

//
// Additional level flag filters e.g. open doors are not obstacles
//
[[nodiscard]] static auto level_flag_filter_needed(Gamep g, Levelsp v, Levelp l, ThingFlag f, Thingp me) -> bool
{
  Thingp it = nullptr;

  return level_flag_filter(g, v, l, f, me, it);
}

auto level_find_all(Gamep g, Levelsp v, Levelp l, ThingFlag f) -> std::vector< Thingp >
{
  TRACE();

  Thingp                me = nullptr;
  std::vector< Thingp > out;

  const auto filter_needed = level_flag_filter_needed(g, v, l, f, me);
  if (filter_needed) {
    FOR_ALL_THINGS_ON_LEVEL_UNSAFE(g, v, l, it)
    {
      if (level_flag_filter(g, v, l, f, me, it)) {
        continue;
      }

      if (tp_flag(thing_tp(it), f) != 0) {
        out.push_back(it);
      }
    }
  } else {
    FOR_ALL_THINGS_ON_LEVEL_UNSAFE(g, v, l, it)
    {
      if (tp_flag(thing_tp(it), f) != 0) {
        out.push_back(it);
      }
    }
  }

  return out;
}

auto level_find_all(Gamep g, Levelsp v, Levelp l, ThingFlag f, bpoint p) -> std::vector< Thingp >
{
  TRACE();

  std::vector< Thingp > out;
  Thingp                me = nullptr;

  const auto filter_needed = level_flag_filter_needed(g, v, l, f, me);
  if (filter_needed) {
    FOR_ALL_THINGS_AT_UNSAFE(g, v, l, it, p)
    {
      if (level_flag_filter(g, v, l, f, me, it)) {
        continue;
      }

      if (tp_flag(thing_tp(it), f) != 0) {
        out.push_back(it);
      }
    }
  } else {
    FOR_ALL_THINGS_AT_UNSAFE(g, v, l, it, p)
    {
      if (tp_flag(thing_tp(it), f) != 0) {
        out.push_back(it);
      }
    }
  }

  return out;
}

void level_update_flags(Gamep g, Levelsp v, Levelp l)
{
  TRACE();

  memset(l->flag, 0, sizeof(l->flag));

  FOR_ALL_THINGS_ON_LEVEL(g, v, l, t)
  {
    auto *tp = thing_tp(t);
    auto  at = thing_at(t);

    for (auto f = 0; f < THING_FLAG_ENUM_MAX; f++) {
      l->flag[ at.x ][ at.y ][ f ] |= tp->flag[ f ];
    }
  }
}

auto level_flag_cached(Gamep g, Levelsp v, Levelp l, ThingFlag f, bpoint p) -> bool
{
  TRACE_DEBUG();

  return l->flag[ p.x ][ p.y ][ f ] != 0;
}

auto level_flag(Gamep g, Levelsp v, Levelp l, ThingFlag f, bpoint p, Thingp me) -> Thingp
{
  TRACE_DEBUG();

  const auto filter_needed = level_flag_filter_needed(g, v, l, f, me);

  if (filter_needed) {
    FOR_ALL_THINGS_AT_UNSAFE(g, v, l, it, p)
    {
      if (level_flag_filter(g, v, l, f, me, it)) {
        continue;
      }

      if (tp_flag(thing_tp(it), f) != 0) {
        return it;
      }
    }
  } else {
    FOR_ALL_THINGS_AT_UNSAFE(g, v, l, it, p)
    {
      if (tp_flag(thing_tp(it), f) != 0) {
        return it;
      }
    }
  }

  return nullptr;
}

auto level_flag(Gamep g, Levelsp v, Levelp l, ThingFlag f, Thingp me) -> Thingp
{
  TRACE();

  if (me == nullptr) {
    ERR("no thing pointer");
    return nullptr;
  }

  return level_flag(g, v, l, f, thing_at(me), me);
}

//
// Filter to only alive things
//
auto level_alive(Gamep g, Levelsp v, Levelp l, ThingFlag f, bpoint p, Thingp me) -> Thingp
{
  TRACE();

  const auto filter_needed = level_flag_filter_needed(g, v, l, f, me);
  if (filter_needed) {
    FOR_ALL_THINGS_AT_UNSAFE(g, v, l, it, p)
    {
      if (level_flag_filter(g, v, l, f, me, it)) {
        continue;
      }

      if (thing_is_dead(it)) {
        continue;
      }

      if (tp_flag(thing_tp(it), f) != 0) {
        return it;
      }
    }
  } else {
    FOR_ALL_THINGS_AT_UNSAFE(g, v, l, it, p)
    {
      if (thing_is_dead(it)) {
        continue;
      }

      if (tp_flag(thing_tp(it), f) != 0) {
        return it;
      }
    }
  }

  return nullptr;
}

auto level_alive(Gamep g, Levelsp v, Levelp l, ThingFlag f, Thingp me) -> Thingp
{
  TRACE();

  if (me == nullptr) {
    ERR("no thing pointer");
    return nullptr;
  }

  return level_alive(g, v, l, f, thing_at(me), me);
}

//
// Flag + is open
//
auto level_open(Gamep g, Levelsp v, Levelp l, ThingFlag f, bpoint p, Thingp me) -> Thingp
{
  TRACE();

  const auto filter_needed = level_flag_filter_needed(g, v, l, f, me);
  if (filter_needed) {
    FOR_ALL_THINGS_AT_UNSAFE(g, v, l, it, p)
    {
      if (level_flag_filter(g, v, l, f, me, it)) {
        continue;
      }

      if (! thing_is_open(it)) {
        continue;
      }

      if (tp_flag(thing_tp(it), f) != 0) {
        return it;
      }
    }
  } else {
    FOR_ALL_THINGS_AT_UNSAFE(g, v, l, it, p)
    {
      if (! thing_is_open(it)) {
        continue;
      }

      if (tp_flag(thing_tp(it), f) != 0) {
        return it;
      }
    }
  }

  return nullptr;
}

auto level_open(Gamep g, Levelsp v, Levelp l, ThingFlag f, Thingp me) -> Thingp
{
  TRACE();

  if (me == nullptr) {
    ERR("no thing pointer");
    return nullptr;
  }

  return level_open(g, v, l, f, thing_at(me), me);
}

//
// Count things
//
auto level_count(Gamep g, Levelsp v, Levelp l, ThingFlag f, bpoint p) -> int
{
  TRACE();

  int count = 0;

  Thingp     me            = nullptr;
  const auto filter_needed = level_flag_filter_needed(g, v, l, f, me);

  if (filter_needed) {
    FOR_ALL_THINGS_AT_UNSAFE(g, v, l, it, p)
    {
      if (level_flag_filter(g, v, l, f, me, it)) {
        continue;
      }

      if (thing_is_dead(it)) {
        continue;
      }

      if (tp_flag(thing_tp(it), f) != 0) {
        count++;
      }
    }
  } else {
    FOR_ALL_THINGS_AT_UNSAFE(g, v, l, it, p)
    {
      if (thing_is_dead(it)) {
        continue;
      }

      if (tp_flag(thing_tp(it), f) != 0) {
        count++;
      }
    }
  }

  return count;
}

auto level_count(Gamep g, Levelsp v, Levelp l, ThingFlag f, Thingp t) -> int
{
  TRACE();

  return level_count(g, v, l, f, thing_at(t));
}

auto level_is_same_obj_type_at(Gamep g, Levelsp v, Levelp l, const bpoint &p, Tpp tp) -> bool
{
  TRACE();

  if (l == nullptr) {
    return false;
  }

  if (is_oob(p)) [[unlikely]] {
    return false;
  }

  for (auto slot = 0; slot < MAP_SLOTS; slot++) {
    Tpp    it_tp = nullptr;
    Thingp it    = thing_and_tp_get_at(g, v, l, p, slot, &it_tp);
    if (it == nullptr) {
      continue;
    }

    //
    // This check is needed to handle walls that are destroyed, but not yet popped.
    //
    if (thing_is_dead(it)) {
      continue;
    }

    if (it_tp == tp) {
      return true;
    }
  }
  return false;
}

void level_bounds_set(Gamep g, Levelsp v, Levelp l)
{
  TRACE();

  VERIFY(MTYPE_LEVELS, v);

  int const zoom = game_map_zoom_get(g);
  auto      dw   = TILE_WIDTH * zoom;
  auto      dh   = TILE_HEIGHT * zoom;

  //
  // The number of tiles additionally to draw to avoid clipping
  //
  auto clipping_border = 2;

  //
  // Set the scroll bounds
  //
  v->pixel_map_at.x = std::max< short >(v->pixel_map_at.x, 0);
  v->pixel_map_at.y = std::max< short >(v->pixel_map_at.y, 0);

  v->pixel_max.x = (MAP_WIDTH * dw) - game_map_fbo_width_get(g);
  v->pixel_max.y = (MAP_HEIGHT * dh) - game_map_fbo_height_get(g);

  //
  // Overflow?
  //
  v->pixel_map_at.x = std::min(v->pixel_map_at.x, v->pixel_max.x);

  v->pixel_map_at.y = std::min(v->pixel_map_at.y, v->pixel_max.y);

  //
  // Set the tile bounds
  //
  int tmp_minx = v->pixel_map_at.x / dw;
  int tmp_miny = v->pixel_map_at.y / dh;
  tmp_minx -= clipping_border;
  tmp_minx -= clipping_border;

  tmp_minx = std::max(tmp_minx, 0);
  tmp_miny = std::max(tmp_miny, 0);

  int tmp_maxx = (v->pixel_map_at.x + game_map_fbo_width_get(g)) / dw;
  int tmp_maxy = (v->pixel_map_at.y + game_map_fbo_height_get(g)) / dh;

  tmp_maxx += clipping_border;
  tmp_maxy += clipping_border;

  tmp_maxx = std::min(tmp_maxx, static_cast< int >(MAP_WIDTH));
  tmp_maxy = std::min(tmp_maxy, static_cast< int >(MAP_HEIGHT));

  v->minx = tmp_minx;
  v->miny = tmp_miny;
  v->maxx = tmp_maxx;
  v->maxy = tmp_maxy;

  //
  // We need to animate all the level tiles in sync, so might as well draw the whole level
  //
  if (level_is_level_select(g, v, l)) {
    v->minx = 0;
    v->miny = 0;
    v->maxx = MAP_WIDTH;
    v->maxy = MAP_HEIGHT;
  }
}
