//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_charmap.hpp"
#include "my_dice_rolls.hpp"
#include "my_game.hpp"
#include "my_globals.hpp"
#include "my_level.hpp"
#include "my_level_inlines.hpp"
#include "my_main.hpp"
#include "my_random.hpp"
#include "my_sprintf.hpp"
#include "my_thing_inlines.hpp"
#include "my_ui.hpp"
#include "my_wids.hpp"

#include <algorithm>
#include <print>

auto level_select_is_oob(bpoint p) -> bool
{
  TRACE();

  if (p.x < 0) {
    return true;
  }
  if (p.y < 0) {
    return true;
  }
  if (p.x >= LEVEL_ACROSS) {
    return true;
  }
  if (p.y >= LEVEL_DOWN) {
    return true;
  }
  return false;
}

auto level_select_is_oob(int x, int y) -> bool
{
  TRACE();

  if (x < 0) {
    return true;
  }
  if (y < 0) {
    return true;
  }
  if (x >= LEVEL_ACROSS) {
    return true;
  }
  if (y >= LEVEL_DOWN) {
    return true;
  }
  return false;
}

//
// If in level select mode, enter the chosen level
//
auto level_select_get_level_at_tile_coords(Gamep g, Levelsp v, bpoint p) -> Levelp
{
  TRACE();

  auto  *level_select = game_level_get(g, v, LEVEL_SELECT_ID);
  Levelp level_over   = nullptr;

  auto *tp_is_level_locked_icon = tp_first(is_level_locked_icon);
  auto *tp_is_level_curr        = tp_first(is_level_curr);
  auto *tp_is_level_final_icon  = tp_first(is_level_final_icon);
  auto *tp_is_level_closed_icon = tp_first(is_level_closed_icon);
  auto *tp_is_level_open_icon   = tp_first(is_level_open_icon);
  auto *tp_is_level_next_icon   = tp_first(is_level_next_icon);

  FOR_ALL_THINGS_AT(g, v, level_select, t, v->cursor_at)
  {
    auto *tp = thing_tp(t);

    if ((tp == tp_is_level_locked_icon) || // newline
        (tp == tp_is_level_curr) ||        // newline
        (tp == tp_is_level_final_icon) ||  // newline
        (tp == tp_is_level_closed_icon) || // newline
        (tp == tp_is_level_open_icon) ||   // newline
        (tp == tp_is_level_next_icon)) {
      auto at             = thing_at(t);
      auto level_num_over = v->level_select.tile_to_level[ at.x ][ at.y ];
      level_over          = game_level_get(g, v, level_num_over);
      break;
    }
  }

  return level_over;
}

//
// Given a point in the level select grid, return the corresponding level,
// if one exists there.
//
static auto level_select_get_level_from_grid_coords(Levelsp v, bpoint p) -> Levelp
{
  TRACE();

  if (v == nullptr) {
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

  if (level_num >= LEVEL_SELECT_ID) {
    return nullptr;
  }

  return &v->level[ level_num ];
}

//
// Attempt to find the next level for this thing to fall into
//
auto level_select_get_next_level_down(Gamep g, Levelsp v, Levelp l) -> Levelp
{
  TRACE();

  if (l->level_num_next_set) {
    return game_level_get(g, v, l->level_num_next);
  }

  return nullptr;
}

//
// Attempt to find the next level for this thing to fall into
//
auto level_select_calculate_next_level_down(Gamep g, Levelsp v, Levelp l, bool redo) -> Levelp
{
  TRACE();

  LevelSelect *s = &v->level_select;
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
  // Try right first
  //
  while (tries++ < LEVEL_ACROSS * 2) {
    p.x++;
    if ((p.x >= LEVEL_DOWN) || (p.y >= LEVEL_ACROSS)) {
      //
      // Failed. Try the other direction.
      //
      p = l->level_select_at;
      break;
    }

    if (compiler_unused) {
      con("level %d -> next (look right at %u,%u)", l->level_num, p.x, p.y);
    }

    auto *cand = level_select_get_level_from_grid_coords(v, p);
    if ((cand != nullptr) && (cand != l)) {
      level_out = cand;
      goto got_level;
    }
  }

  //
  // Try left and down
  //
  p.x = 0;
  while (tries++ < LEVEL_ACROSS * 2) {
    p.y++;
    if ((p.x >= LEVEL_DOWN) || (p.y >= LEVEL_ACROSS)) {
      //
      // Failed. Try the other direction.
      //
      p = l->level_select_at;
      break;
    }

    if (compiler_unused) {
      con("level %d -> next (look down and left at %u,%u)", l->level_num, p.x, p.y);
    }

    auto *cand = level_select_get_level_from_grid_coords(v, p);
    if ((cand != nullptr) && (cand != l)) {
      level_out = cand;
      goto got_level;
    }
  }

  //
  // Get the next sequential level, in terms of level number. This is used for tests.
  //
  for (int y = 0; y < LEVEL_DOWN; y++) {
    for (auto &x : s->data) {
      LevelSelectCell const *c = &x[ y ];
      if (c->is_set == 0U) {
        continue;
      }

      auto *cand = game_level_get(g, v, c->level_num);
      if (cand == nullptr) {
        continue;
      }

      if (compiler_unused) {
        con("level %d -> next %d (sequential)", l->level_num, cand->level_num);
      }

      if (cand->level_num == l->level_num + 1) {
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
  while (tries++ < LEVEL_DOWN * LEVEL_ACROSS * 2) {
    if (compiler_unused) {
      con("level %d -> next (random)", l->level_num);
    }
    bpoint const random_p(PCG_RANDOM_RANGE(0, LEVEL_ACROSS), PCG_RANDOM_RANGE(0, LEVEL_DOWN));

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
      con("level %d -> next %d at %u,%u", l->level_num, l->level_num_next, l->level_select_at.x, l->level_select_at.y);
    }
  }

  return level_out;
}

//
// This is the thing that is used to represent the current level. We focus the mouse zoom on this.
//
auto thing_level_select(Gamep g) -> Thingp
{
  TRACE();

  auto *v = game_levels_get(g);
  if (v == nullptr) {
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

  for (int y = 0; y < LEVEL_DOWN; y++) {
    std::string out;
    for (auto &x : s->data) {
      auto is_set = x[ y ].is_set != 0U;

      if (is_set) {
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

  for (int y = 0; y < LEVEL_DOWN; y++) {
    for (int x = 0; x < LEVEL_ACROSS; x++) {
      if (s->data[ x ][ y ].is_set != 0U) {
        auto *l = game_level_get(g, v, n);
        if (l == nullptr) {
          CROAK("ran out of levels to assign to grid, %u", n);
        }

        if (! l->is_initialized) {
          CROAK("trying to use a level that is not initialized, %u", n);
        }

        auto *c            = &s->data[ x ][ y ];
        l->level_select_at = bpoint(x, y);

        c->level_num = l->level_num;
        if ((x == LEVEL_ACROSS - 1) && (y == LEVEL_DOWN - 1)) {
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
auto level_select_get(Gamep g, Levelsp v, bpoint p) -> LevelSelectCell *
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

  for (int y = 0; y < LEVEL_DOWN; y++) {
    for (auto &x : s->data) {
      if (x[ y ].is_set != 0U) {
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
  auto               level_num    = LEVEL_SELECT_ID;
  auto              *level_select = game_level_get(g, v, level_num);

  auto  *player       = thing_player(g);
  Levelp player_level = thing_player_level(g);

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
  auto *tp_is_level_open_icon   = tp_first(is_level_open_icon);
  auto *tp_is_level_next_icon   = tp_first(is_level_next_icon);

  bpoint const map_offset(18, 18);

  for (auto y = 0; y < LEVEL_DOWN; y++) {
    for (auto x = 0; x < LEVEL_ACROSS; x++) {
      LevelSelectCell const *c = &s->data[ x ][ y ];
      if (c->is_set == 0U) {
        continue;
      }

      auto *l = game_level_get(g, v, c->level_num);
      if (l == nullptr) {
        ERR("missing level in select map");
        continue;
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
        if (x > 0) {
          LevelSelectCell const *o = &s->data[ x - 1 ][ y ]; // limit to adjacent levels
          if ((o != nullptr) && (o->is_set != 0U) && (o->level_num == player_level->level_num)) {
            tp                                  = tp_is_level_next_icon;
            l->player_can_enter_this_level_next = true;
          }
        }

        if (x < LEVEL_ACROSS - 2) {
          LevelSelectCell const *o = &s->data[ x + 1 ][ y ]; // limit to adjacent levels
          if ((o != nullptr) && (o->is_set != 0U) && (o->level_num == player_level->level_num)) {
            tp                                  = tp_is_level_next_icon;
            l->player_can_enter_this_level_next = true;
          }
        }

        //
        // Allow transition from boss to next level
        //
        if ((y > 0) && (x == 0)) {
          LevelSelectCell const *o = &s->data[ LEVEL_ACROSS - 1 ][ y - 1 ];
          if ((o != nullptr) && (o->is_set != 0U) && (o->level_num == player_level->level_num)) {
            tp                                  = tp_is_level_next_icon;
            l->player_can_enter_this_level_next = true;
          }
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

        if (x < LEVEL_ACROSS - 1) {
          LevelSelectCell const *o = &s->data[ x + 1 ][ y ]; // limit to adjacent levels
          if ((o != nullptr) && (o->is_set != 0U) && (o->level_num == player_level->level_num)) {
            tp                                  = tp_is_level_next_icon;
            l->player_can_enter_this_level_next = true;
          }
        }
      }

      if (g_opt_level_select_menu) {
        l->player_can_enter_this_level_next = true;
      }

      //
      // Final level
      //
      if ((x == LEVEL_ACROSS - 1) && (y == LEVEL_DOWN - 1)) {
        tp = tp_is_level_final_icon;
      }

      //
      // Completed levels
      //
      if (l->player_completed_level_via_exit || l->player_fell_out_of_level) {
        tp = tp_is_level_closed_icon;

        if (x > 0) {
          LevelSelectCell const *o = &s->data[ x - 1 ][ y ]; // limit to adjacent levels
          if ((o != nullptr) && (o->is_set != 0U) && (o->level_num == player_level->level_num)) {
            tp = tp_is_level_open_icon;
          }
        }

        if (x < LEVEL_ACROSS - 2) {
          LevelSelectCell const *o = &s->data[ x + 1 ][ y ]; // limit to adjacent levels
          if ((o != nullptr) && (o->is_set != 0U) && (o->level_num == player_level->level_num)) {
            tp = tp_is_level_open_icon;
          }
        }
      }

      if (player != nullptr) {
        //
        // Where the player is currently
        //
        if (player->level_num == l->level_num) {
          tp = tp_is_level_curr;
        }
      } else if ((x == 0) && (y == 0)) {
        //
        // Where the player is initially
        //
        tp = tp_is_level_curr;
      }

      if (tp != nullptr) {
        bpoint at(x * 3, y * 4);
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
        if (g_opt_level_select_menu && (tp == tp_is_level_locked_icon)) {
          tp = tp_is_level_next_icon;
        }

        auto t = thing_spawn(g, v, level_select, tp, at);
        if (t == nullptr) {
          return false;
        }

        v->level_select.tile_to_level[ at.x ][ at.y ] = l->level_num;

        if ((x == 2) && (y == 2)) {
          v->level_select_id = t->id;
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

  return true;
}

//
// Create the level grid
//
static void level_select_create(Gamep g, Levelsp v, LevelSelect *s)
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

  for (auto y = 0; y < LEVEL_DOWN; y++) {
    for (auto x = 0; x < LEVEL_ACROSS; x++) {
      LevelSelectCell *c = &s->data[ x ][ y ];
      c->is_set          = 1U;
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

  auto  level_num = LEVEL_SELECT_ID;
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

  level_select_create(g, v, s);
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
// Show a sorted list of vales
//
static void level_select_show_sorted_values(Gamep g, WidPopup *parent, std::map< std::string, int > &map_in, const std::string &map_name)
{
  TRACE();

  if (map_in.empty()) {
    return;
  }

  {
    auto s1 = std::format(" {}:", map_name);
    parent->log(g, UI_INFO_FMT_STR + std::string(s1) + UI_RESET_FMT, TEXT_FORMAT_LHS);
  }

  while (static_cast< uint32_t >(! map_in.empty()) != 0U) {
    std::string highest;
    for (const auto &m : map_in) {
      auto name = m.first;
      auto val  = m.second;
      if (highest.empty()) {
        highest = name;
      } else if (val > map_in[ highest ]) {
        highest = name;
      }
    }

    {
      TRACE();
      auto             *tp   = tp_find_mand(highest);
      std::string const name = tp_name_short(tp);

      auto s2 = std::format("  {} x %tp={}$ {}", map_in[ highest ], highest, name);
      parent->log(g, s2, TEXT_FORMAT_LHS);
    }

    map_in.erase(highest);
  }
}

//
// If in level select mode, update what we're hovering over
//
void level_select_rightbar_show_contents(Gamep g, Levelsp v, Levelp l, WidPopup *parent)
{
  TRACE();

  if (! level_is_level_select(g, v, l)) {
    return;
  }

  Levelp level_over = level_select_get_level_at_tile_coords(g, v, v->cursor_at);
  if (level_over == nullptr) {
    return;
  }

  parent->log_empty_line(g);

  Levelp player_level = thing_player_level(g);

  std::map< std::string, int > mobs;
  std::map< std::string, int > monsts;
  std::map< std::string, int > treasure;

  FOR_ALL_THINGS_ON_LEVEL(g, v, level_over, t)
  {
    auto name = tp_name(thing_tp(t));

    if (thing_is_mob(t)) {
      mobs[ name ]++;
    }

    if (thing_is_monst(t)) {
      monsts[ name ]++;
    }

    if (thing_is_treasure(t)) {
      treasure[ name ]++;
    }
  }

  auto tmp = std::format("Level {}", level_over->level_num + 1);
  parent->log(g, UI_INFO_FMT_STR + std::string(tmp) + UI_RESET_FMT);
  parent->log_empty_line(g);

  if (level_over->player_can_enter_this_level_next) {
    if (player_level->player_completed_level_via_exit) {
      parent->log(g, " You can enter here", TEXT_FORMAT_LHS);
    } else if (player_level->player_fell_out_of_level) {
      parent->log(g, " You can climb back here", TEXT_FORMAT_LHS);
    } else {
      parent->log(g, " You can go back here", TEXT_FORMAT_LHS);
    }
  } else if (level_over != player_level) {
    parent->log(g, " You cannot enter here", TEXT_FORMAT_LHS);
  } else {
    parent->log(g, " You can re-enter here", TEXT_FORMAT_LHS);
  }

  if (level_over->player_completed_level_via_exit) {
    parent->log(g, " You completed this level", TEXT_FORMAT_LHS);
  } else if (level_over->player_fell_out_of_level) {
    parent->log(g, " You fell out of this level", TEXT_FORMAT_LHS);
  }

  {
    parent->log_empty_line(g);
    parent->log(g, " Contents:", TEXT_FORMAT_LHS);
  }

  level_select_show_sorted_values(g, parent, mobs, "Mobs");
  level_select_show_sorted_values(g, parent, monsts, "Monsters");
  level_select_show_sorted_values(g, parent, treasure, "Loot");

  level_minimaps_update(g, v, level_over);
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

  Levelp level_over = level_select_get_level_at_tile_coords(g, v, v->cursor_at);
  if (level_over == nullptr) {
    return;
  }

  game_request_to_remake_ui_set(g);
}

//
// If in level select mode, enter the chosen level
//
void level_select_mouse_down(Gamep g, Levelsp v, Levelp l)
{
  TRACE();

  if (! level_is_level_select(g, v, l)) {
    return;
  }

  Levelp level_over = level_select_get_level_at_tile_coords(g, v, v->cursor_at);
  if (level_over == nullptr) {
    return;
  }

  auto  *player       = thing_player(g);
  Levelp player_level = thing_player_level(g);

  //
  // We're hovering over a level and have pressed the mouse
  //
  Levelp new_level = nullptr;

  auto at = v->cursor_at;

  if (level_is_level_locked_icon(g, v, l, at)) {
    topcon("You cannot enter this level yet. Choose an open door.");
    return;
  }
  if (level_is_level_final_icon(g, v, l, at)) {
    // ok to choose
  }
  if (level_is_level_open_icon(g, v, l, at)) {
    // ok to choose
  }
  if (level_is_level_closed_icon(g, v, l, at)) {
    topcon("This level is closed to you. Choose an open door.");
    return;
  }
  if (level_is_level_next_icon(g, v, l, at)) {
    // ok to choose
  }

  //
  // Switch to the chosen level if possible; allow going back to the old level to clean up if needed
  //
  if ((level_over == player_level) || level_over->player_can_enter_this_level_next) {
    new_level = level_change(g, v, level_over->level_num);
  } else {
    topcon("You cannot enter this level yet. Choose an open door.");
  }

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
}

void level_select_test(Gamep g)
{
  TRACE();

  auto *v = levels_memory_alloc(g);
  (void) game_levels_set(g, v);
  LevelSelect *s = &v->level_select;

  level_select_create(g, v, s);
  level_select_dump(s);
  level_select_count_levels(s);
}
