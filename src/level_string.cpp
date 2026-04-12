//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_charmap.hpp"
#include "my_game.hpp"
#include "my_globals.hpp"
#include "my_level.hpp"
#include "my_main.hpp"
#include "my_sprintf.hpp"

#include <print>

auto to_string(Gamep g, Levelsp v, Levelp l) -> std::string
{
  TRACE();

  if (l == nullptr) {
    ERR("no level pointer");
    return "<no level>";
  }

  return (std::format("L{}{}",
                      /* newline */ l->level_num + 1,
                      /* newline */ level_is_player_level(g, v, l) ? "/curr" : ""));
}

//
// Convert a level into a single string
//
auto level_string(Gamep g, Levelsp v, Levelp l, int w, int h) -> std::string
{
  std::string out;

  TRACE_INDENT();

  for (int y = 0; y < h; y++) {
    for (int x = 0; x < w; x++) {
      char         c = CHARMAP_EMPTY;
      bpoint const p(x, y);

      if (level_is_dirt(g, v, l, p) != nullptr) {
        c = CHARMAP_DIRT;
      }
      if (level_is_floor(g, v, l, p) != nullptr) {
        c = CHARMAP_FLOOR;
      }
      if (level_is_corridor(g, v, l, p) != nullptr) {
        c = CHARMAP_CORRIDOR;
      }
      if (level_alive_is_foliage(g, v, l, p) != nullptr) {
        c = CHARMAP_FOLIAGE;
      }
      if (level_alive_is_reeds(g, v, l, p) != nullptr) {
        c = CHARMAP_REEDS;
      }
      if (level_alive_is_grass(g, v, l, p) != nullptr) {
        c = CHARMAP_GRASS;
      }
      if (level_is_vault(g, v, l, p) != nullptr) {
        c = CHARMAP_VAULT;
      }
      if (level_is_wall(g, v, l, p) != nullptr) {
        c = CHARMAP_WALL;
      }
      if (level_is_barrel(g, v, l, p) != nullptr) {
        c = CHARMAP_BARREL;
      }
      if (level_is_brazier(g, v, l, p) != nullptr) {
        c = CHARMAP_BRAZIER;
      }
      if (level_is_bridge(g, v, l, p) != nullptr) {
        c = CHARMAP_BRIDGE;
      }
      if (level_is_chasm(g, v, l, p) != nullptr) {
        c = CHARMAP_CHASM;
      }
      if (level_is_door_unlocked(g, v, l, p) != nullptr) {
        if (level_open_is_door_unlocked(g, v, l, p) != nullptr) {
          c = CHARMAP_FLOOR;
        } else {
          c = CHARMAP_DOOR_UNLOCKED;
        }
      }
      if (level_is_door_locked(g, v, l, p) != nullptr) {
        if (level_open_is_door_locked(g, v, l, p) != nullptr) {
          c = CHARMAP_FLOOR;
        } else {
          c = CHARMAP_DOOR_LOCKED;
        }
      }
      if (level_is_door_secret(g, v, l, p) != nullptr) {
        if (level_open_is_door_secret(g, v, l, p) != nullptr) {
          c = CHARMAP_FLOOR;
        } else {
          c = CHARMAP_DOOR_SECRET;
        }
      }
      if (level_is_key(g, v, l, p) != nullptr) {
        c = CHARMAP_KEY;
      }
      if (level_is_lava(g, v, l, p) != nullptr) {
        c = CHARMAP_LAVA;
      }
      if (level_is_water(g, v, l, p) != nullptr) {
        c = CHARMAP_WATER;
      }
      if (level_is_deep_water(g, v, l, p) != nullptr) {
        c = CHARMAP_DEEP_WATER;
      }
      if (level_is_mob1(g, v, l, p) != nullptr) {
        c = CHARMAP_MOB1;
      }
      if (level_is_mob2(g, v, l, p) != nullptr) {
        c = CHARMAP_MOB2;
      }
      if (level_is_monst_group_easy(g, v, l, p) != nullptr) {
        c = CHARMAP_MONST_EASY;
      }
      if (level_is_monst_group_hard(g, v, l, p) != nullptr) {
        c = CHARMAP_MONST_HARD;
      }
      if (level_is_minion(g, v, l, p) != nullptr) {
        c = CHARMAP_MONST_EASY;
      }
      if (level_is_pillar(g, v, l, p) != nullptr) {
        c = CHARMAP_PILLAR;
      }
      if (level_is_teleport(g, v, l, p) != nullptr) {
        c = CHARMAP_TELEPORT;
      }
      if (level_is_trap(g, v, l, p) != nullptr) {
        c = CHARMAP_TRAP;
      }
      if (level_is_treasure(g, v, l, p) != nullptr) {
        c = CHARMAP_TREASURE;
      }
      if (level_is_entrance(g, v, l, p) != nullptr) {
        c = CHARMAP_ENTRANCE;
      }
      if (level_is_exit(g, v, l, p) != nullptr) {
        c = CHARMAP_EXIT;
      }
      if (level_is_fire(g, v, l, p) != nullptr) {
        c = CHARMAP_FIRE;
      }
      if (level_is_player(g, v, l, p) != nullptr) {
        c = CHARMAP_PLAYER;
      }
      if (level_is_steam(g, v, l, p) != nullptr) {
        c = CHARMAP_STEAM;
      }
      if (level_is_smoke(g, v, l, p) != nullptr) {
        c = CHARMAP_SMOKE;
      }
      if (level_is_projectile(g, v, l, p) != nullptr) {
        c = CHARMAP_PROJECTILE;
      }
      if (level_is_border(g, v, l, p) != nullptr) {
        c = CHARMAP_BORDER;
      }
      if (level_is_rock(g, v, l, p) != nullptr) {
        c = CHARMAP_ROCK;
      }

      out += c;
    }
  }

  return out;
}
