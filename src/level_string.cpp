//
// Copyright goblinhack@gmail.com
//

#include "my_bpoint.hpp"
#include "my_callstack.hpp"
#include "my_charmap.hpp"
#include "my_level.hpp"
#include "my_main.hpp"
#include "my_types.hpp"

#include <format>
#include <string>

[[nodiscard]] auto to_string(Gamep g, Levelsp v, Levelp l) -> std::string
{
  TRACE();

  if (l == nullptr) [[unlikely]] {
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
[[nodiscard]] auto level_string(Gamep g, Levelsp v, Levelp l, int w, int h) -> std::string
{
  std::string out;

  TRACE_INDENT();

  for (int y = 0; y < h; y++) {
    for (int x = 0; x < w; x++) {
      char         c = CHARMAP_EMPTY;
      bpoint const p(x, y);

      if (level_is_dirt_bool(g, v, l, p)) {
        c = CHARMAP_DIRT;
      }
      if (level_is_floor_bool(g, v, l, p)) {
        c = CHARMAP_FLOOR;
      }
      if (level_is_corridor_bool(g, v, l, p)) {
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
      if (level_is_vault_bool(g, v, l, p)) {
        c = CHARMAP_VAULT;
      }
      if (level_is_wall_bool(g, v, l, p)) {
        c = CHARMAP_WALL;
      }
      if (level_is_barrel_bool(g, v, l, p)) {
        c = CHARMAP_BARREL;
      }
      if (level_is_brazier_bool(g, v, l, p)) {
        c = CHARMAP_BRAZIER;
      }
      if (level_is_bridge_bool(g, v, l, p)) {
        c = CHARMAP_BRIDGE;
      }
      if (level_is_chasm_bool(g, v, l, p)) {
        c = CHARMAP_CHASM;
      }
      if (level_is_door_unlocked_bool(g, v, l, p)) {
        if (level_open_is_door_unlocked(g, v, l, p) != nullptr) {
          c = CHARMAP_FLOOR;
        } else {
          c = CHARMAP_DOOR_UNLOCKED;
        }
      }
      if (level_is_door_locked_bool(g, v, l, p)) {
        if (level_open_is_door_locked(g, v, l, p) != nullptr) {
          c = CHARMAP_FLOOR;
        } else {
          c = CHARMAP_DOOR_LOCKED;
        }
      }
      if (level_is_door_secret_bool(g, v, l, p)) {
        if (level_open_is_door_secret(g, v, l, p) != nullptr) {
          c = CHARMAP_FLOOR;
        } else {
          c = CHARMAP_DOOR_SECRET;
        }
      }
      if (level_is_key_bool(g, v, l, p)) {
        c = CHARMAP_KEY;
      }
      if (level_is_lava_bool(g, v, l, p)) {
        c = CHARMAP_LAVA;
      }
      if (level_is_water_shallow_bool(g, v, l, p)) {
        c = CHARMAP_WATER;
      }
      if (level_is_water_deep_bool(g, v, l, p)) {
        c = CHARMAP_DEEP_WATER;
      }
      if (level_is_rubble_bool(g, v, l, p)) {
        c = CHARMAP_RUBBLE;
      }
      if (level_is_spiderweb_bool(g, v, l, p)) {
        c = CHARMAP_SPIDERWEB;
      }
      if (level_is_mob1_bool(g, v, l, p)) {
        c = CHARMAP_MOB1;
      }
      if (level_is_mob2_bool(g, v, l, p)) {
        c = CHARMAP_MOB2;
      }

      if (level_alive_is_monst1(g, v, l, p)) {
        c = CHARMAP_MONST1;
      } else if (level_alive_is_monst2(g, v, l, p)) {
        c = CHARMAP_MONST2;
      } else if (level_alive_is_monst(g, v, l, p)) {
        c = CHARMAP_MONST1;
      }

      if (level_alive_is_minion(g, v, l, p)) {
        c = CHARMAP_MONST1;
      }

      if (level_is_pillar_bool(g, v, l, p)) {
        c = CHARMAP_PILLAR;
      }
      if (level_is_teleport_bool(g, v, l, p)) {
        c = CHARMAP_TELEPORT;
      }
      if (level_is_trap_bool(g, v, l, p)) {
        c = CHARMAP_TRAP;
      }
      if (level_is_treasure_bool(g, v, l, p)) {
        c = CHARMAP_TREASURE;
      }
      if (level_is_entrance_bool(g, v, l, p)) {
        c = CHARMAP_ENTRANCE;
      }
      if (level_is_exit_bool(g, v, l, p)) {
        c = CHARMAP_EXIT;
      }
      if (level_is_fire_bool(g, v, l, p)) {
        c = CHARMAP_FIRE;
      }
      if (level_is_explosion_bool(g, v, l, p)) {
        c = CHARMAP_FIRE;
      }
      if (level_is_player_bool(g, v, l, p)) {
        c = CHARMAP_ENTRANCE;
      }
      if (level_is_steam_bool(g, v, l, p)) {
        c = CHARMAP_STEAM;
      }
      if (level_is_smoke_bool(g, v, l, p)) {
        c = CHARMAP_SMOKE;
      }
      if (level_is_proj_bool(g, v, l, p)) {
        c = CHARMAP_WEAPON;
      }
      if (level_is_beam_weapon_bool(g, v, l, p)) {
        c = CHARMAP_WEAPON;
      }
      if (level_is_border_bool(g, v, l, p)) {
        c = CHARMAP_BORDER;
      }
      if (level_is_rock_bool(g, v, l, p)) {
        c = CHARMAP_ROCK;
      }

      out += c;
    }
  }

  return out;
}
