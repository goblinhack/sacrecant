//
// Copyright goblinhack@gmail.com
//

#include "my_bpoint.hpp"
#include "my_callstack.hpp"
#include "my_charmap.hpp"
#include "my_game.hpp"
#include "my_game_defs.hpp"
#include "my_game_inlines.hpp"
#include "my_globals.hpp"
#include "my_level.hpp"
#include "my_level_inlines.hpp"
#include "my_main.hpp"
#include "my_random.hpp"
#include "my_sound.hpp"
#include "my_thing.hpp"
#include "my_thing_inlines.hpp"
#include "my_tp.hpp"
#include "my_types.hpp"
#include "my_wids.hpp"

#include <cmath>
#include <cstdint>
#include <cstring>
#include <string>

[[nodiscard]] auto level_select_is_oob(bpoint p) -> bool
{
  TRACE();

  if (p.x < 0) {
    return true;
  }
  if (p.y < 0) {
    return true;
  }
  if (p.x >= LEVEL_GRID_ACROSS) {
    return true;
  }
  if (p.y >= LEVEL_GRID_DOWN) {
    return true;
  }
  return false;
}

[[nodiscard]] auto level_select_is_oob(int x, int y) -> bool
{
  TRACE();

  if (x < 0) {
    return true;
  }
  if (y < 0) {
    return true;
  }
  if (x >= LEVEL_GRID_ACROSS) {
    return true;
  }
  if (y >= LEVEL_GRID_DOWN) {
    return true;
  }
  return false;
}

//
// What level thing is at the cursor
//
[[nodiscard]] auto level_select_get_level_at_tile_coords(Gamep g, Levelsp v, bpoint p) -> Levelp
{
  TRACE();

  auto  *level_select = game_level_get(g, v, LEVEL_ARR_IDX_LEVEL_SELECT);
  Levelp level_over   = nullptr;

  auto *tp_is_level_locked_icon = tp_first(is_level_locked_icon);
  auto *tp_is_level_curr        = tp_first(is_level_curr);
  auto *tp_is_level_final_icon  = tp_first(is_level_final_icon);
  auto *tp_is_level_closed_icon = tp_first(is_level_closed_icon);
  auto *tp_is_level_open_icon   = tp_first(is_level_open_icon);
  auto *tp_is_level_next_icon   = tp_first(is_level_next_icon);

  FOR_ALL_THINGS_AT(g, v, level_select, t, p)
  {
    auto *tp = thing_tp(t);

    if ((tp == tp_is_level_locked_icon) || // newline
        (tp == tp_is_level_curr) ||        // newline
        (tp == tp_is_level_final_icon) ||  // newline
        (tp == tp_is_level_closed_icon) || // newline
        (tp == tp_is_level_open_icon) ||   // newline
        (tp == tp_is_level_next_icon)) {
      auto at             = thing_at(g, v, level_select, t);
      auto level_num_over = v->level_select.tile_to_level[ at.x ][ at.y ];
      level_over          = game_level_get(g, v, level_num_over);
      break;
    }
  }

  return level_over;
}

//
// What thing is at the cursor
//
[[nodiscard]] static auto level_select_get_thing_at_tile_coords(Gamep g, Levelsp v, bpoint p) -> Thingp
{
  TRACE();

  auto *level_select = game_level_get(g, v, LEVEL_ARR_IDX_LEVEL_SELECT);

  FOR_ALL_THINGS_AT(g, v, level_select, t, p) { return t; }

  return nullptr;
}

//
// Given a point in the level select grid, return the corresponding level,
// if one exists there.
//
static auto level_select_get_level_from_grid_coords(Levelsp v, bpoint p) -> Levelp
{
  TRACE();

  if (v == nullptr) [[unlikely]] {
    return nullptr;
  }

  if (level_select_is_oob(p)) {
    return nullptr;
  }

  auto *s = &v->level_select.data[ p.x ][ p.y ];
  if (s->is_set == 0U) {
    return nullptr;
  }

  auto level_num = s->level_num;

  if (level_num >= LEVEL_ARR_IDX_LEVEL_SELECT) {
    return nullptr;
  }

  return &v->level[ level_num ];
}

//
// Attempt to find the next level for this thing to fall into
//
[[nodiscard]] auto level_select_get_next_level_down(Gamep g, Levelsp v, Levelp l) -> Levelp
{
  TRACE();

  if (l->level_num_next_set) {
    return game_level_get(g, v, l->level_num_next);
  }

  return nullptr;
}

//
// Attempt to find the next level for play
//
[[nodiscard]] auto level_select_get_next_level(Gamep g, Levelsp v, Levelp l) -> Levelp
{
  TRACE();

  if (v->tick == 0U) {
    auto *player = thing_player(g);
    if (player != nullptr) {
      return thing_player_level(g);
    }
  }

  if (l->level_num_next_set) {
    return game_level_get(g, v, l->level_num + 1);
  }

  return nullptr;
}

//
// Attempt to find the next level for this thing to fall into
//
[[nodiscard]] auto level_select_calculate_next_level_down(Gamep g, Levelsp v, Levelp l, bool redo) -> Levelp
{
  TRACE();

  LevelSelect const *s = &v->level_select;
  if (s == nullptr) {
    CROAK("missing level select pointer");
  }

  auto   p         = l->level_select_at;
  Levelp level_out = nullptr;
  int    tries     = 0;

  //
  // Tests need to re-reun this each time a new level is added.
  //
  if (! redo) {
    if (l->level_num_next_set) {
      return game_level_get(g, v, l->level_num_next);
    }
  }

  if (compiler_unused) {
    con("-");
    con("level %d at %u,%u", l->level_num, l->level_select_at.x, l->level_select_at.y);
  }

  //
  // Cannot fall out of a boss level
  //
  if (level_is_boss_level(g, v, l)) {
    level_out = l;
    goto got_level;
  }

  //
  // Try to fall down
  //
  {
    p.y++;
    if (p.y >= LEVEL_GRID_DOWN) {
      //
      // Failed.
      //
      level_out = l;
      goto got_level;
    }

    if (compiler_unused) {
      con("level %d -> next (look at %u,%u)", l->level_num, p.x, p.y);
    }

    auto *cand = level_select_get_level_from_grid_coords(v, p);
    if ((cand != nullptr) && (cand != l)) {
      level_out = cand;
      goto got_level;
    }
  }

  //
  // Try to fall down into the boss level
  //
  switch (p.y) {
    case 3 :
    case 7 :
    case 11 :
    case 15 :
    case 19 :
      {
        p.x = LEVEL_ARR_IDX_BOSS_OFFSET;

        if (compiler_unused) {
          con("level %d -> next (look at boss level %u,%u)", l->level_num, p.x, p.y);
        }

        auto *cand = level_select_get_level_from_grid_coords(v, p);
        if ((cand != nullptr) && (cand != l)) {
          level_out = cand;
          goto got_level;
        }
      }
  }

  if (l->level_num == v->level_count) {
    auto *cand = game_level_get(g, v, 0);
    if (cand != nullptr) {
      if (compiler_unused) {
        con("level %d -> next (first level)", l->level_num);
      }
      level_out = cand;
      goto got_level;
    }
  }

  //
  // Nothing to fall onto. Try a random level.
  //
  tries = 0;
  while (tries++ < LEVEL_GRID_DOWN * LEVEL_GRID_ACROSS * 2) {
    if (compiler_unused) {
      con("level %d -> next (random)", l->level_num);
    }
    bpoint const random_p(PCG_RANDOM_RANGE(0, LEVEL_GRID_ACROSS), PCG_RANDOM_RANGE(0, LEVEL_GRID_DOWN));

    auto *cand = level_select_get_level_from_grid_coords(v, random_p);
    if ((cand != nullptr) && (cand != l)) {
      level_out = cand;
      goto got_level;
    }
  }

  //
  // Some tests have only 1 level
  //
  if (! g_opt_tests) {
    CROAK("failed to find a next level down");
  }

got_level:
  if (level_out != nullptr) {
    l->level_num_next_set = true;
    l->level_num_next     = level_out->level_num;
    if (compiler_unused) {
      con("level %d -> got next %d at %u,%u", l->level_num, l->level_num_next, l->level_select_at.x, l->level_select_at.y);
    }
  }

  return level_out;
}

//
// This is the thing that is used to represent the current level. We focus the mouse zoom on this.
//
[[nodiscard]] auto thing_level_select(Gamep g) -> Thingp
{
  TRACE();

  auto *v = game_levels_get(g);
  if (v == nullptr) [[unlikely]] {
    return nullptr;
  }

  if (v->level_select_id == 0U) {
    return nullptr;
  }

  return thing_find(g, v, v->level_select_id);
}

//
// Dump a level
//
static void level_select_dump(LevelSelect *s)
{
  TRACE();

  IF_NODEBUG { return; }

  log("levelSelect, level count %d", s->level_count);
  TRACE_INDENT();

  for (auto y = 0; y < LEVEL_GRID_DOWN; y++) {
    std::string out;
    for (auto &x : s->data) {
      LevelSelectCell const *c = &x[ y ];

      if (c->is_set != 0U) {
        out += std::to_string(CHARMAP_FLOOR);
      } else {
        out += std::to_string(CHARMAP_EMPTY);
      }
    }
    log("%s", out.c_str());
  }
  log("-");
}

//
// Assign levels to their position in the grid
//
void level_select_assign_levels_to_grid(Gamep g, Levelsp v)
{
  TRACE();

  LevelSelect *s = &v->level_select;

  auto n = 0;

  for (int y = 0; y < LEVEL_GRID_DOWN; y++) {
    for (int x = 0; x < LEVEL_GRID_ACROSS; x++) {
      if (s->data[ x ][ y ].is_set != 0U) {
        auto *l = game_level_get(g, v, n);
        if (l == nullptr) [[unlikely]] {
          CROAK("ran out of levels to assign to grid, %u", n);
        }

        if (! l->is_initialized) {
          CROAK("trying to use a level that is not initialized, %u", n);
        }

        auto *c            = &s->data[ x ][ y ];
        l->level_select_at = bpoint(x, y);

        c->level_num = l->level_num;
        if ((x == LEVEL_ARR_IDX_BOSS_OFFSET) && (y == LEVEL_GRID_DOWN - 1)) {
          c->final_level = true;
        }

        n++;
      }
    }
  }
}

//
// Return the level
//
[[nodiscard]] auto level_select_get(Gamep g, Levelsp v, bpoint p) -> LevelSelectCell *
{
  TRACE();

  LevelSelect *s = &v->level_select;
  if (s == nullptr) {
    ERR("null level select");
    return nullptr;
  }

  if (level_select_is_oob(p)) {
    ERR("level select out of range");
    return nullptr;
  }

  return &s->data[ p.x ][ p.y ];
}

//
// Count the levels created
//
static auto level_select_count_levels(LevelSelect *s) -> int
{
  TRACE();

  s->level_count = 0;

  for (auto y = 0; y < LEVEL_GRID_DOWN; y++) {
    for (auto &x : s->data) {
      LevelSelectCell const *c = &x[ y ];
      if (c->is_set != 0U) {
        s->level_count++;
      }
    }
  }
  return s->level_count;
}

//
// Create a Thing for each level
//
[[nodiscard]] static auto level_select_map_set(Gamep g, Levelsp v) -> bool
{
  log("level select map");
  TRACE_INDENT();

  LevelSelect const *s            = &v->level_select;
  auto               level_num    = LEVEL_ARR_IDX_LEVEL_SELECT;
  auto              *level_select = game_level_get(g, v, level_num);

  auto  *player       = thing_player(g);
  Levelp player_level = thing_player_level(g);

  IF_DEBUG
  {
    if (player_level != nullptr) {
      player_level->player_completed_level_via_exit = true;
    }
  }

  //
  // Clean up all previous things
  //
  FOR_ALL_THINGS_ON_LEVEL(g, v, level_select, t)
  {
    //
    thing_fini(g, v, level_select, t);
  }

  memset(level_select->debug, ' ', SIZEOF(level_select->debug));

  auto *tp_is_level_locked_icon = tp_first(is_level_locked_icon);
  auto *tp_is_level_curr        = tp_first(is_level_curr);
  auto *tp_is_level_final_icon  = tp_first(is_level_final_icon);
  auto *tp_is_level_closed_icon = tp_first(is_level_closed_icon);
  auto *tp_is_level_next_icon   = tp_first(is_level_next_icon);

  bpoint const map_offset(18, 5);

  for (auto y = 0; y < LEVEL_GRID_DOWN; y++) {
    for (auto x = 0; x < LEVEL_GRID_ACROSS; x++) {

      //
      // Skip fake levels
      //
      if (! level_is_valid_grid_coord(x, y)) {
        continue;
      }

      LevelSelectCell const *c = &s->data[ x ][ y ];
      if (c->is_set == 0U) {
        ERR("valid level, but not set");
        continue;
      }

      auto *l = game_level_get(g, v, c->level_num);
      if (l == nullptr) [[unlikely]] {
        ERR("missing level in select map");
        continue;
      }

      //
      // Boss level?
      //
      if ((y % 4) == 3) {
        if (x == LEVEL_ARR_IDX_BOSS_OFFSET) {
          level_is_boss_level_set(g, v, l);
        }
      }

      //
      // Default
      //
      Tpp tp = tp_is_level_locked_icon;

      //
      // Can we enter this level?
      //
      l->player_can_enter_this_level_next = false;

      //
      // If not visited, is it a next level for the current level?
      //
      if ((player_level != nullptr) && (player_level->player_completed_level_via_exit || player_level->player_fell_out_of_level)) {

        if (x < LEVEL_GRID_ACROSS - 1) {
          LevelSelectCell const *o = &s->data[ x + 1 ][ y ]; // left levels
          if ((o != nullptr) && (o->is_set != 0U) && (o->level_num == player_level->level_num)) {
            tp                                  = tp_is_level_next_icon;
            l->player_can_enter_this_level_next = true;
          }
        }

        if (x > 0) {
          LevelSelectCell const *o = &s->data[ x - 1 ][ y ]; // right levels
          if ((o != nullptr) && (o->is_set != 0U) && (o->level_num == player_level->level_num)) {
            tp                                  = tp_is_level_next_icon;
            l->player_can_enter_this_level_next = true;
          }
        }

        if (y < LEVEL_GRID_DOWN - 1) {
          LevelSelectCell const *o = &s->data[ x ][ y + 1 ]; // left levels
          if ((o != nullptr) && (o->is_set != 0U) && (o->level_num == player_level->level_num)) {
            tp                                  = tp_is_level_next_icon;
            l->player_can_enter_this_level_next = true;
          }
        }

        if (y > 0) {
          LevelSelectCell const *o = &s->data[ x ][ y - 1 ]; // right levels
          if ((o != nullptr) && (o->is_set != 0U) && (o->level_num == player_level->level_num)) {
            tp                                  = tp_is_level_next_icon;
            l->player_can_enter_this_level_next = true;
          }
        }

        //
        // Allow transition from boss to next level
        //
        switch (y) {
          case 3 :
          case 7 :
          case 11 :
          case 15 :
          case 19 :
            for (const auto &px : s->data) {
              LevelSelectCell const *o = &px[ y - 1 ];
              if ((o != nullptr) && (o->is_set != 0U) && (o->level_num == player_level->level_num)) {
                tp                                  = tp_is_level_next_icon;
                l->player_can_enter_this_level_next = true;
              }
            }
            break;
        }
      }

      //
      // Allow backwards moves to completed levels
      //
      if ((player_level != nullptr) && (l->player_completed_level_via_exit || l->player_fell_out_of_level)) {
        if (x > 0) {
          LevelSelectCell const *o = &s->data[ x - 1 ][ y ]; // limit to adjacent levels
          if ((o != nullptr) && (o->is_set != 0U) && (o->level_num == player_level->level_num)) {
            tp                                  = tp_is_level_next_icon;
            l->player_can_enter_this_level_next = true;
          }
        }

        if (x < LEVEL_GRID_ACROSS - 2) {
          LevelSelectCell const *o = &s->data[ x + 1 ][ y ]; // limit to adjacent levels
          if ((o != nullptr) && (o->is_set != 0U) && (o->level_num == player_level->level_num)) {
            tp                                  = tp_is_level_next_icon;
            l->player_can_enter_this_level_next = true;
          }
        }
      }

      IF_DEBUG2
      {
        if (g_opt_level_select_menu) {
          l->player_can_enter_this_level_next = true;
        }
      }

      //
      // Final level
      //
      if ((x == LEVEL_ARR_IDX_BOSS_OFFSET) && (y == LEVEL_GRID_DOWN - 1)) {
        if (tp != tp_is_level_next_icon) {
          tp = tp_is_level_final_icon;
          level_is_boss_final_level_set(g, v, l);
        }
      }

      //
      // Completed levels
      //
      if (l->player_completed_level_via_exit || l->player_fell_out_of_level) {
        tp = tp_is_level_closed_icon;
      }

      if ((v->tick == 0U) && (player->level_num == 0U)) {
        //
        // Where the player is initially
        //
        if (y == 0) {
          tp = tp_is_level_next_icon;
        }
      } else if (player != nullptr) {
        //
        // Where the player is currently
        //
        if (player->level_num == l->level_num) {
          tp = tp_is_level_curr;
        }
      }

      if (tp != nullptr) {
        bpoint at(x * 3, y * 2);
        at += map_offset;
        if (is_oob(at)) [[unlikely]] {
          continue;
        }

        //
        // Save debugging
        //
        level_select->debug[ at.x ][ at.y ] = '#';

        if (tp == tp_is_level_locked_icon) {
          level_select->debug[ at.x ][ at.y ] = '?';
        }
        if (tp == tp_is_level_curr) {
          level_select->debug[ at.x ][ at.y ] = '@';
        }
        if (tp == tp_is_level_final_icon) {
          level_select->debug[ at.x ][ at.y ] = 'E';
        }
        if (tp == tp_is_level_next_icon) {
          level_select->debug[ at.x ][ at.y ] = '?';
        }

        //
        // Show all levels as next when debugging
        //
        IF_DEBUG2
        {
          if (g_opt_level_select_menu && (tp == tp_is_level_locked_icon)) {
            tp = tp_is_level_next_icon;
          }
        }

        auto *t = thing_spawn(g, v, level_select, tp, at);
        if (t == nullptr) {
          return false;
        }

        v->level_select.tile_to_level[ at.x ][ at.y ] = l->level_num;

        if (v->level_select_id == 0U) {
          if ((x == LEVEL_ARR_IDX_BOSS_OFFSET) && (y == 0)) {
            v->level_select_id = t->id;
            level_scroll_to_focus(g, v, level_select);
          }
        }

        if (x == LEVEL_ARR_IDX_BOSS_OFFSET) {
          for (const auto &px : s->data) {
            LevelSelectCell const *o = &px[ y ];
            if ((o != nullptr) && (o->is_set != 0U) && (o->level_num == player_level->level_num)) {
              v->level_select_id = t->id;
            }
          }
        }
      }
    }
  }

  //
  // Add the worlds background
  //
  if (thing_spawn(g, v, level_select, tp_first(is_level_select_bg), bpoint(0, 0)) == nullptr) {
    return false;
  }

  //
  // Create joined up tiles (for the rocks)
  //
  level_assign_tiles(g, v, level_select);

  level_scroll_to_focus(g, v, level_select);

  return true;
}

//
// Create the level grid
//
static void level_select_create(Gamep g, LevelSelect *s)
{
  TRACE();

  if (s->is_populated != 0U) {
    return;
  }

  //
  // Use a consistent seed
  //
  uint32_t const seed_num = game_seed_num_get(g);
  PCG_SRAND(seed_num);

  for (auto y = 0; y < LEVEL_GRID_DOWN; y++) {
    for (auto x = 0; x < LEVEL_GRID_ACROSS; x++) {
      if (level_is_valid_grid_coord(x, y)) {
        LevelSelectCell *c = &s->data[ x ][ y ];
        c->is_set          = 1U;
      }
    }
  }

  s->is_populated = 1U;
}

//
// Create the things that are used to represent levels
//
void level_select_update_grid_tiles(Gamep g, Levelsp v)
{
  TRACE();

  auto  level_num = LEVEL_ARR_IDX_LEVEL_SELECT;
  auto *l         = game_level_get(g, v, level_num);

  level_init(g, v, l, level_num);
  l->info.seed_num = game_seed_num_get(g);

  (void) level_select_map_set(g, v);
}

//
// Create the special level used to walk other levels
//
void level_select_grid_of_empty_levels(Gamep g)
{
  log("level select generate");
  TRACE_INDENT();

  auto *v = levels_memory_alloc(g);
  (void) game_levels_set(g, v);
  LevelSelect *s = &v->level_select;

  level_select_create(g, s);
  level_select_dump(s);
  level_select_count_levels(s);
}

//
// Clean up the level select snake walk
//
void level_select_destroy(Gamep g, Levelsp v, Levelp l)
{
  log("level select destroy");
  TRACE_INDENT();

  if ((l == nullptr) || ! l->is_initialized) {
    return;
  }

  LevelSelect *s = &v->level_select;
  memset(s, 0, SIZEOF(*s));
}

//
// If in level select mode, update what we're hovering over
//
void level_select_mouse_motion(Gamep g, Levelsp v, Levelp l)
{
  TRACE();

  if (! level_is_level_select(g, v, l)) {
    return;
  }

  FOR_ALL_THINGS_ON_LEVEL(g, v, l, t)
  {
    if (t->anim_type != THING_ANIM_IDLE) {
      thing_anim_init(g, v, l, t, THING_ANIM_IDLE);
    }
  }

  Levelp level_over = level_select_get_level_at_tile_coords(g, v, v->cursor_at);
  if (level_over == nullptr) {
    return;
  }

  auto *t = level_select_get_thing_at_tile_coords(g, v, v->cursor_at);
  if (t != nullptr) {
    if (t->anim_type != THING_ANIM_MOUSE_OVER) {
      thing_anim_init(g, v, l, t, THING_ANIM_MOUSE_OVER);
    }
  }

  wid_level_select(g, v, level_over);

  game_request_to_remake_ui_set(g);
}

//
// If in level select mode, enter the chosen level
//
static auto level_select_next(Gamep g, Levelsp v, Levelp l, Levelp level_over) -> bool
{
  TRACE();
  level_con(g, v, l, "over");
  level_con(g, v, level_over, "over");

  auto  *player       = thing_player(g);
  Levelp player_level = thing_player_level(g);

  //
  // We're hovering over a level and have pressed the mouse
  //
  Levelp new_level = nullptr;

  auto at = v->cursor_at;

  if (level_is_level_locked_icon(g, v, l, at) != nullptr) {
    topcon("You cannot enter this level yet. Choose an open door.");
    (void) sound_play(g, "error");
    return false;
  }

  bool ok_to_choose = {};
  if (level_is_level_final_icon(g, v, l, at) != nullptr) {
    // ok to choose
    ok_to_choose = true;
  }
  if (level_is_level_open_icon(g, v, l, at) != nullptr) {
    // ok to choose
    ok_to_choose = true;
  }
  if (level_is_level_closed_icon(g, v, l, at) != nullptr) {
    // ok to choose
    ok_to_choose = true;
  }
  if (level_is_level_next_icon(g, v, l, at) != nullptr) {
    // ok to choose
    ok_to_choose = true;
  }

  //
  // Switch to the chosen level if possible; allow going back to the old level to clean up if needed
  //
  if ((level_over == player_level) || ok_to_choose) {
    new_level = level_change(g, v, level_over->level_num);
  } else {
    topcon("You cannot enter level %u yet. Choose an open door.", level_over->level_num + 1);
    (void) sound_play(g, "error");
    return false;
  }

  level_update(g, v, new_level);

  wid_level_select_destroy();

  //
  // Move the player also
  //
  if (new_level != nullptr) {
    if (new_level->player_completed_level_via_exit) {
      thing_level_warp_to_exit(g, v, new_level, player);
    } else if (new_level->player_fell_out_of_level) {
      thing_level_warp_to_exit(g, v, new_level, player);
    } else {
      thing_level_warp_to_entrance(g, v, new_level, player);
    }
    level_scroll_warp_to_focus(g, v, l);

    //
    // Disable load and save buttons
    //
    game_request_to_remake_ui_set(g);
  }

  return true;
}

//
// If in level select mode, enter the chosen level
//
auto level_select_mouse_down(Gamep g) -> bool
{
  TRACE();

  auto *v = game_levels_get(g);
  if (v == nullptr) [[unlikely]] {
    (void) sound_play(g, "error");
    return false;
  }

  Levelp l = thing_player_level(g);
  if (l == nullptr) [[unlikely]] {
    (void) sound_play(g, "error");
    return false;
  }

  auto *level_over = level_select_get_next_level(g, v, l);
  if (level_over == nullptr) {
    (void) sound_play(g, "error");
    return false;
  }

  if (v->tick == 0u) {
    (void) sound_play(g, "start");
  }

  return level_select_next(g, v, l, level_over);
}

//
// If in level select mode, enter the chosen level
//
auto level_select_mouse_down(Gamep g, Levelsp v, Levelp l) -> bool
{
  TRACE();

  if (! level_is_level_select(g, v, l)) {
    (void) sound_play(g, "error");
    return false;
  }

  Levelp level_over = level_select_get_level_at_tile_coords(g, v, v->cursor_at);
  if (level_over == nullptr) {
    (void) sound_play(g, "error");
    return false;
  }

  if (v->tick == 0u) {
    (void) sound_play(g, "start");
  }

  return level_select_next(g, v, l, level_over);
}

void level_select_test(Gamep g)
{
  TRACE();

  auto *v = levels_memory_alloc(g);
  (void) game_levels_set(g, v);
  LevelSelect *s = &v->level_select;

  level_select_create(g, s);
  level_select_dump(s);
  level_select_count_levels(s);
}
