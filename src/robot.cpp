//
// Copyright goblinhack@gmail.com
//

#include "my_bpoint.hpp"
#include "my_callstack.hpp"
#include "my_game.hpp"
#include "my_game_inlines.hpp"
#include "my_level.hpp"
#include "my_level_inlines.hpp"
#include "my_main.hpp"
#include "my_random.hpp"
#include "my_random_name.hpp"
#include "my_robot.hpp"
#include "my_sdl_event.hpp"
#include "my_thing.hpp"
#include "my_thing_inlines.hpp"
#include "my_types.hpp"
#include "my_wids.hpp"

#include <SDL_events.h>
#include <SDL_keyboard.h>
#include <SDL_keycode.h>
#include <SDL_mouse.h>
#include <initializer_list>
#include <set>
#include <string>
#include <utility>

enum {
  GOAL_PRIO_MOB                              = 10,
  GOAL_PRIO_TREASURE                         = 20,
  GOAL_PRIO_MONST                            = 30,
  GOAL_PRIO_EXPLORE_EDGE_OF_VISION           = 40,
  GOAL_PRIO_EXPLORE_ANY_VISIBLE_TILES        = 50,
  GOAL_PRIO_EXPLORE_PREVIOUSLY_VISITED_TILES = 60,
  GOAL_PRIO_EXIT                             = 70,
};

class Robot
{
public:
  Robot() = default;
};

class Goal
{
public:
  int         prio  = {};
  int         score = {};
  bpoint      at;
  std::string what;
  Thingp      what_it = {};

  Goal(int _prio, int _score, bpoint _at, std::string _what, Thingp _what_it)
      : //
        prio(_prio), score(_score), at(_at), what(std::move(_what)), what_it(_what_it)
  {
  }
};

static auto operator<(const class Goal &lhs, const class Goal &rhs) -> bool
{
  // Lower priorities at the head
  if (lhs.prio < rhs.prio) {
    return true;
  }
  if (lhs.prio > rhs.prio) {
    return false;
  }
  return lhs.score > rhs.score; // Higher scores at the head
}

static Robot *g_robot;

//
// Main loop
//
static void robot_mode_handler_playing(Gamep g, Robot *robot)
{
  SDL_Event   e   = {};
  SDL_Keysym *key = &e.key.keysym;

  TRACE();

  std::multiset< Goal > goals;

  auto *v = game_levels_get(g);
  if (v == nullptr) [[unlikely]] {
    CROAK("no levels");
    return;
  }

  auto *l = game_level_get(g, v);
  if (l == nullptr) {
    CROAK("no level");
    return;
  }

  auto *player = thing_player(g);
  if (player == nullptr) {
    CROAK("no player");
    return;
  }

  if (robot == nullptr) [[unlikely]] {
    CROAK("no robot");
    return;
  }

  if (level_tick_is_in_progress(g, v, l)) {
    return;
  }

  if (thing_is_moving(player)) {
    return;
  }

  if (v->tick) {
    if ((v->tick % 20) == 0u) {
      //          game_request_to_save_game_set(g);
    }

    if ((v->tick % 200) == 0u) {
      ThingEvent ev = {};
      ev.reason     = "robot is out of time ";
      thing_dead(g, v, l, player, ev);
      return;
    }
  }

  int  x;
  int  y;
  auto at = thing_at(g, v, l, player);

  //
  // Explore at the edge of vision
  //
  FOR_ALL_MAP_POINTS_NO_BREAK(g, v, l, x, y)
  {
    bpoint p(x, y);

    //
    // Avoid lava
    //
    if (level_is_cursor_path_hazard_bool(g, v, l, p)) {
      continue;
    }

    //
    // Look for tiles at the edge of vision
    //
    if (! thing_vision_can_see_tile(g, v, l, player, p)) {
      const std::initializer_list< bpoint > points = {
          bpoint(-1, -1), bpoint(1, -1), bpoint(0, -1), bpoint(-1, 0), bpoint(1, 0), bpoint(0, 0), bpoint(-1, 1), bpoint(1, 1), bpoint(0, 1),
      };

      bool cand = {};
      for (auto delta : points) {
        auto n = p + delta;
        if (level_is_obs_to_movement(g, v, l, n) != nullptr) {
          continue;
        }

        if (! is_oob(n)) {
          if (thing_vision_can_see_tile(g, v, l, player, n)) {
            cand = true;
            break;
          }
        }
      }

      if (! cand) {
        continue;
      }
    } else {
      continue;
    }

    auto path = level_cursor_path_draw_line(g, v, l, at, p);
    if (path.empty()) {
      continue;
    }

    int score = -static_cast< int >(path.size());

    if (level_has_seen(g, v, l, p)) {
      score -= 10;
    }

    if (l->player_has_walked_tile[ x ][ y ] != 0u) {
      score -= 10;
    }

    FOR_ALL_THINGS_AT_UNSAFE(g, v, l, t, p)
    {
      //
      // Avoid lava
      //
      if (level_is_cursor_path_hazard_bool(g, v, l, p)) {
        continue;
      }

      //
      // Obstacles
      //
      if (level_is_obs_to_movement_bool(g, v, l, p)) {
        continue;
      }

      if (thing_is_floor(t) || thing_is_dirt(t)) {
        goals.insert(Goal(GOAL_PRIO_EXPLORE_EDGE_OF_VISION, score, p, "floor", t));
      }

      if (thing_is_water_shallow(t)) {
        goals.insert(Goal(GOAL_PRIO_EXPLORE_EDGE_OF_VISION, score, p, "shallow water", t));
      }

      if (thing_is_water_deep(t)) {
        goals.insert(Goal(GOAL_PRIO_EXPLORE_EDGE_OF_VISION, score, p, "deep water", t));
      }
    }
  }

  if (goals.empty())
    FOR_ALL_MAP_POINTS_NO_BREAK(g, v, l, x, y)
    {
      bpoint p(x, y);

      //
      // Avoid lava
      //
      if (level_is_cursor_path_hazard_bool(g, v, l, p)) {
        continue;
      }

      //
      // Don't try to shoot ghosts in walls
      //
      if (level_is_obs_to_movement_bool(g, v, l, p)) {
        continue;
      }

      //
      // Look for tiles at the edge of vision
      //
      if (! thing_vision_can_see_tile(g, v, l, player, p)) {
        continue;
      }

      int       score = 0;
      int const dist  = static_cast< int >(distance(p, at) * 10);

      FOR_ALL_THINGS_AT_UNSAFE(g, v, l, t, p)
      {
        if (thing_is_exit(t)) {
          score *= 2;
          goals.insert(Goal(GOAL_PRIO_EXIT, score, p, "exit", t));
        }

        if (thing_is_mob(t)) {
          score *= 2;
          goals.insert(Goal(GOAL_PRIO_MOB, score, p, "monst", t));
        }

        if (thing_is_treasure(t)) {
          score *= 2;
          goals.insert(Goal(GOAL_PRIO_TREASURE, score, p, "exit", t));
        }

        if (thing_is_monst(t)) {
          if (! thing_is_dead(t) && ! thing_is_corpse(t)) {
            score = -dist;
            goals.insert(Goal(GOAL_PRIO_MONST, score, p, "monst", t));
          }
        }
      }
    }

  //
  // Lower priority. Explore known tiles.
  //
  if (goals.empty())
    FOR_ALL_MAP_POINTS_NO_BREAK(g, v, l, x, y)
    {
      bpoint p(x, y);

      //
      // Look for tiles at the edge of vision
      //
      if (! thing_vision_can_see_tile(g, v, l, player, p)) {
        continue;
      }

      auto path = level_cursor_path_draw_line(g, v, l, at, p);
      if (path.empty()) {
        continue;
      }

      int score = -static_cast< int >(path.size());

      if (level_has_seen(g, v, l, p)) {
        score -= 10;
      }

      if (l->player_has_walked_tile[ x ][ y ] != 0u) {
        score -= 10;
      }

      FOR_ALL_THINGS_AT_UNSAFE(g, v, l, t, p)
      {
        //
        // Avoid lava
        //
        if (level_is_cursor_path_hazard_bool(g, v, l, p)) {
          continue;
        }

        //
        // Obstacles
        //
        if (level_is_obs_to_movement_bool(g, v, l, p)) {
          continue;
        }

        goals.insert(Goal(GOAL_PRIO_EXPLORE_ANY_VISIBLE_TILES, score, p, "explore", t));
      }
    }

  //
  // Lowest priority. Explore known tiles.
  //
  if (goals.empty())
    FOR_ALL_MAP_POINTS_NO_BREAK(g, v, l, x, y)
    {
      bpoint p(x, y);

      if (! l->player_has_walked_tile[ x ][ y ]) {
        continue;
      }

      auto path = level_cursor_path_draw_line(g, v, l, at, p);
      if (path.empty()) {
        continue;
      }

      int score = -static_cast< int >(path.size());

      if (level_has_seen(g, v, l, p)) {
        score -= 10;
      }

      FOR_ALL_THINGS_AT_UNSAFE(g, v, l, t, p)
      {
        //
        // Avoid lava
        //
        if (level_is_cursor_path_hazard_bool(g, v, l, p)) {
          continue;
        }

        //
        // Obstacles
        //
        if (level_is_obs_to_movement_bool(g, v, l, p)) {
          continue;
        }

        goals.insert(Goal(GOAL_PRIO_EXPLORE_PREVIOUSLY_VISITED_TILES, score, p, "explore", t));
      }
    }

  if (compiler_unused) {
    con("Goals:");
    for (const auto &goal : goals) {
      thing_con(g, v, l, player, "goal: prio %d score %d -- @%d,%d, %s", goal.prio, goal.score, goal.at.x, goal.at.y, goal.what.c_str());
    }
    con("-");
  }

  int visible_map_tl_x = 0;
  int visible_map_tl_y = 0;
  int visible_map_br_x = 0;
  int visible_map_br_y = 0;
  game_visible_map_pix_get(g, &visible_map_tl_x, &visible_map_tl_y, &visible_map_br_x, &visible_map_br_y);

  if (compiler_unused) {
    con("bounds: %d,%d -> %d,%d", visible_map_tl_x, visible_map_tl_y, visible_map_br_x, visible_map_br_y);
  }

  for (const auto &goal : goals) {
    if (goal.what_it != nullptr) {

      auto pixel = thing_to_pixel(g, v, l, goal.what_it);
      if (pixel.x <= visible_map_tl_x) {
        continue;
      }
      if (pixel.x >= visible_map_br_x) {
        continue;
      }
      if (pixel.y <= visible_map_tl_y) {
        continue;
      }
      if (pixel.y >= visible_map_br_y) {
        continue;
      }

      if (compiler_unused) {
        thing_topcon(g, v, l, player, "goal: @%d,%d, pix %d,%d %s", goal.at.x, goal.at.y, pixel.x, pixel.y, goal.what.c_str());
      }

      SDL_WarpMouseInWindow(sdl.window, pixel.x, pixel.y);
      e.type = SDL_MOUSEMOTION;
      SDL_PushEvent(&e);
      e = {};

      if (v->cursor_at != goal.at) {
        if (compiler_unused) {
          con("wait on mouse: goal %d,%d cursor %d,%d", goal.at.x, goal.at.y, v->cursor_at.x, v->cursor_at.y);
        }
        return;
      }

      if (thing_is_monst(goal.what_it)) {
        e.type = SDL_KEYDOWN;
        *key   = game_key_fire_get(g);
        SDL_PushEvent(&e);
        log("Robot: fire!");
        return;
      }
      e.type          = SDL_MOUSEBUTTONDOWN;
      e.button.button = 0;
      SDL_PushEvent(&e);
      log("Robot: mouse down");
      return;
    }
  }

  {
    ThingEvent ev = {};
    ev.reason     = "robot is out of things to do";
    thing_dead(g, v, l, player, ev);
  }
}

//
// Main loop
//
void robot_mode_handler(Gamep g)
{
  TRACE();

  SDL_Event   e   = {};
  SDL_Keysym *key = &e.key.keysym;

  switch (game_state(g)) {
    case STATE_INIT : break;
    case STATE_MAIN_MENU :
      {
        con("Robot: main menu: send SPACE key");
        delete g_robot;
        g_robot  = new Robot();
        e.type   = SDL_KEYDOWN;
        key->sym = SDLK_SPACE;
        SDL_PushEvent(&e);
      }
      break;
    case STATE_PLAYER_SELECT_MENU :
      {
        con("Robot: player select menu: send SPACE key x 2");
        e.type   = SDL_KEYDOWN;
        key->sym = SDLK_SPACE;
        SDL_PushEvent(&e);
        SDL_PushEvent(&e);
      }
      break;
    case STATE_LEVEL_SELECT_MENU :
      {
        auto *v = game_levels_get(g);
        con("Robot: level select menu");
        if (v && v->tick) {
          wid_dead_select(g, "level select");
        } else {
          e.type   = SDL_KEYDOWN;
          key->sym = SDLK_SPACE;
          SDL_PushEvent(&e);
        }
      }
      break;
    case STATE_PLAYING :
      {
        auto *v = game_levels_get(g);
        if (v == nullptr) [[unlikely]] {
          CROAK("no levels");
          return;
        }

        robot_mode_handler_playing(g, g_robot);
      }
      break;
    case STATE_QUITTING :            break;
    case STATE_GAME_OVER_MENU :      break;
    case STATE_CHOOSE_THROW_TARGET : break;
    case STATE_CHOOSE_SPELL_TARGET : break;
    case STATE_STATISTICS_MENU :
      {
        con("Robot: player statistic menu: send ESCAPE key");
        e.type   = SDL_KEYDOWN;
        key->sym = SDLK_ESCAPE;
        SDL_PushEvent(&e);
      }
      break;
    case STATE_DEAD_MENU :
      {
        con("Robot: player dead menu: send ESCAPE key");
        e.type   = SDL_KEYDOWN;
        key->sym = SDLK_ESCAPE;
        SDL_PushEvent(&e);

        //
        // New seed
        //
        game_seed_clear(g);
        g_opt_seed_name = "";
        auto seed_name  = os_random_name(SIZEOF("4294967295") - 1);
        game_seed_set(g, seed_name.c_str());
      }
      break;
    case STATE_MOVE_WARNING_MENU :
      {
        con("Robot: warning menu, send 'y'");
        e.type   = SDL_KEYDOWN;
        key->sym = SDLK_y;
        SDL_PushEvent(&e);
      }
      break;
    case STATE_COLLECT_MENU :
      {
        con("Robot: collect menu, send 'a'");
        e.type   = SDL_KEYDOWN;
        key->sym = SDLK_a;
        SDL_PushEvent(&e);
      }
      break;
    case STATE_KEYBOARD_MENU :    break;
    case STATE_LOAD_MENU :        break;
    case STATE_LOADED :           break;
    case STATE_SAVE_MENU :        break;
    case STATE_THROW_MENU :       break;
    case STATE_QUIT_MENU :        break;
    case STATE_INVENTORY_MENU :   break;
    case STATE_SPELL_LEARN_MENU : break;
    case STATE_SPELLBOOK_MENU :   break;
    case STATE_ITEM_MENU :        break;
    case STATE_GENERATING :       break;
    case STATE_GENERATED :        break;
    case GAME_STATE_ENUM_MAX :    break;
  }
}
