//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_game.hpp"
#include "my_game_inlines.hpp"
#include "my_globals.hpp"
#include "my_level.hpp"
#include "my_level_inlines.hpp"
#include "my_main.hpp"
#include "my_sdl_event.hpp"
#include "my_sdl_proto.hpp"
#include "my_thing.hpp"
#include "my_thing_inlines.hpp"
#include "my_types.hpp"

#include <SDL_events.h>
#include <set>

enum {
  GOAL_PRIO_HIGHEST   = 0,
  GOAL_PRIO_VERY_HIGH = 1,
  GOAL_PRIO_HIGHER    = 2,
  GOAL_PRIO_HIGH      = 3,
  GOAL_PRIO_MED       = 4,
  GOAL_PRIO_LOW       = 5,
  GOAL_PRIO_VERY_LOW  = 6,
};

class Robot
{
public:
  Robot() {}
};

class Goal
{
public:
  int         prio  = {};
  int         score = {};
  bpoint      at;
  std::string what;
  Thingp      what_it = {};

  Goal(int _prio, int _score, bpoint _at, const std::string &_what, Thingp _what_it)
      : //
        prio(_prio), score(_score), at(_at), what(_what), what_it(_what_it)
  {
  }
};

static bool operator<(const class Goal &lhs, const class Goal &rhs)
{
  // Lower priorities at the head
  if (lhs.prio < rhs.prio) {
    return true;
  } else if (lhs.prio > rhs.prio) {
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

  con("Robot: handler");
  TRACE_INDENT();

  SDL_Delay(10);

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

  if (thing_is_moving(player)) {
    return;
  }

  int  x, y;
  auto at = thing_at(g, v, l, player);

  FOR_ALL_MAP_POINTS(g, v, l, x, y)
  {
    bpoint p(x, y);

    if (! thing_vision_can_see_tile(g, v, l, player, p)) {
      continue;
    }

    int score = 0;

    int dist = (int) (distance(p, at) * 10);

    score += dist;

    if (level_has_seen(g, v, l, p)) {
      score -= 10;
    }

    if (l->player_has_walked_tile[ x ][ y ]) {
      score -= 10;
    }

    FOR_ALL_THINGS_AT_UNSAFE(g, v, l, t, p)
    {
      if (thing_is_exit(t)) {
        goals.insert(Goal(GOAL_PRIO_HIGHEST, score, p, "exit", t));
      }

      if (thing_is_monst(t)) {
        if (! thing_is_dead(t)) {
          score = -dist;
          goals.insert(Goal(GOAL_PRIO_VERY_HIGH, score, p, "monst", t));
        }
      }

      if (thing_is_treasure(t)) {
        goals.insert(Goal(GOAL_PRIO_HIGHER, score, p, "exit", t));
      }

      if (! level_is_obs_to_movement(g, v, l, p)) {
        if (thing_is_floor(t) || thing_is_dirt(t)) {
          goals.insert(Goal(GOAL_PRIO_MED, score, p, "floor", t));
        }

        if (thing_is_water_shallow(t)) {
          goals.insert(Goal(GOAL_PRIO_LOW, score, p, "shallow water", t));
        }

        if (thing_is_water_deep(t)) {
          goals.insert(Goal(GOAL_PRIO_VERY_LOW, score, p, "deep water", t));
        }
      }
    }
  }

  if (1 || compiler_unused) {
    con("Goals:");
    for (auto goal : goals) {
      thing_con(g, v, l, player, "goal: prio %d score %d -- @%d,%d, %s", goal.prio, goal.score, goal.at.x, goal.at.y, goal.what.c_str());
    }
    con("-");
  }

  int visible_map_tl_x = 0;
  int visible_map_tl_y = 0;
  int visible_map_br_x = 0;
  int visible_map_br_y = 0;
  game_visible_map_pix_get(g, &visible_map_tl_x, &visible_map_tl_y, &visible_map_br_x, &visible_map_br_y);

  con("bounds: %d,%d -> %d,%d", visible_map_tl_x, visible_map_tl_y, visible_map_br_x, visible_map_br_y);

  for (auto goal : goals) {
    if (goal.what_it) {

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

      thing_topcon(g, v, l, player, "goal: @%d,%d, pix %d,%d %s", goal.at.x, goal.at.y, pixel.x, pixel.y, goal.what.c_str());

      SDL_WarpMouseInWindow(sdl.window, pixel.x, pixel.y);
      e.type = SDL_MOUSEMOTION;
      SDL_PushEvent(&e);
      e = {};

      if (v->cursor_at != goal.at) {
        con("wait on mouse: goal %d,%d cursor %d,%d", goal.at.x, goal.at.y, v->cursor_at.x, v->cursor_at.y);
        return;
      }

      if (thing_is_monst(goal.what_it)) {
        e.type = SDL_KEYDOWN;
        *key   = game_key_fire_get(g);
        SDL_PushEvent(&e);
        con("fire!");
        return;
      } else {
        e.type          = SDL_MOUSEBUTTONDOWN;
        e.button.button = 0;
        SDL_PushEvent(&e);
        con("mouse down");
        return;
      }
    }
  }

  CROAK("Robot is out of things to do");
}

//
// Main loop
//
void robot_mode_handler(Gamep g)
{
  con("Robot: handler");
  TRACE_INDENT();

  SDL_Event   e   = {};
  SDL_Keysym *key = &e.key.keysym;

  switch (game_state(g)) {
    case STATE_INIT : break;
    case STATE_MAIN_MENU :
      con("Robot: main menu: send SPACE key");
      if (g_robot) {
        delete g_robot;
      }
      g_robot  = new Robot();
      e.type   = SDL_KEYDOWN;
      key->sym = SDLK_SPACE;
      SDL_PushEvent(&e);
      break;
    case STATE_PLAYER_SELECT_MENU :
      con("Robot: player select menu: send SPACE key x 2");
      e.type   = SDL_KEYDOWN;
      key->sym = SDLK_SPACE;
      SDL_PushEvent(&e);
      SDL_PushEvent(&e);
      break;
    case STATE_LEVEL_SELECT_MENU :
      con("Robot: level select menu: send SPACE key");
      e.type   = SDL_KEYDOWN;
      key->sym = SDLK_SPACE;
      SDL_PushEvent(&e);
      break;
    case STATE_PLAYING :
      con("Robot: playing");
      robot_mode_handler_playing(g, g_robot);
      break;
    case STATE_QUITTING :            break;
    case STATE_GAME_OVER_MENU :      break;
    case STATE_CHOOSE_THROW_TARGET : break;
    case STATE_CHOOSE_SPELL_TARGET : break;
    case STATE_STATISTICS_MENU :
      con("Robot: player statistic menu: send ESCAPE key");
      e.type   = SDL_KEYDOWN;
      key->sym = SDLK_ESCAPE;
      SDL_PushEvent(&e);
      break;
    case STATE_DEAD_MENU :
      con("Robot: player dead menu: send ESCAPE key");
      e.type   = SDL_KEYDOWN;
      key->sym = SDLK_ESCAPE;
      SDL_PushEvent(&e);
      break;
    case STATE_MOVE_WARNING_MENU : break;
    case STATE_KEYBOARD_MENU :     break;
    case STATE_LOAD_MENU :         break;
    case STATE_LOADED :            break;
    case STATE_SAVE_MENU :         break;
    case STATE_THROW_MENU :        break;
    case STATE_QUIT_MENU :         break;
    case STATE_INVENTORY_MENU :    break;
    case STATE_SPELL_LEARN_MENU :  break;
    case STATE_SPELLBOOK_MENU :    break;
    case STATE_COLLECT_MENU :      break;
    case STATE_ITEM_MENU :         break;
    case STATE_GENERATING :        break;
    case STATE_GENERATED :         break;
    case GAME_STATE_ENUM_MAX :     break;
  }
}
