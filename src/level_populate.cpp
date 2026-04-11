//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_charmap.hpp"
#include "my_dice_rolls.hpp"
#include "my_globals.hpp"
#include "my_level.hpp"
#include "my_level_ext.hpp"
#include "my_main.hpp"
#include "my_thing_callbacks.hpp"
#include "my_thing_inlines.hpp"

#include <cstring>
#include <utility>

static auto level_populated(Gamep g, Levelsp v, Levelp l)
{
  TRACE();

  FOR_ALL_THINGS_ON_LEVEL_UNSAFE(g, v, l, t)
  {
    thing_on_level_populated(g, v, l, t);

    if (thing_is_grouped_thing(t)) {
      if (t->group_id == 0U) {
        level_group_things(g, v, l, t);
      }
    }

    //
    // Update hidden and submerged status
    //
    thing_update_pos(g, v, l, t);
  }

  level_count_items(g, v, l);

  return true;
}

class LevelPopulate
{
public:
  char   c = {};
  bpoint at;
  Biome  biome                 = {BIOME_DUNGEON};
  Tpp    tp_wall               = {};
  Tpp    tp_vault              = {};
  Tpp    tp_border             = {};
  Tpp    tp_rock               = {};
  Tpp    tp_water              = {};
  Tpp    tp_lava               = {};
  Tpp    tp_bridge             = {};
  Tpp    tp_chasm              = {};
  Tpp    tp_deep_water         = {};
  Tpp    tp_brazier            = {};
  Tpp    tp_pillar             = {};
  Tpp    tp_barrel             = {};
  Tpp    tp_teleport           = {};
  Tpp    tp_foliage            = {};
  Tpp    tp_corridor           = {};
  Tpp    tp_grass              = {};
  Tpp    tp_floor              = {};
  Tpp    tp_dirt               = {};
  Tpp    tp_exit               = {};
  Tpp    tp_player             = {};
  Tpp    tp_entrance           = {};
  int    wall_variant          = {};
  bool   is_test_level         = {};
  bool   is_decoration_allowed = {};
  bool   need_floor            = {};
  bool   need_entrance         = {};
  bool   need_corridor         = {};
  bool   need_water            = {};
  bool   need_dirt             = {};
  bool   need_foliage          = {};
  bool   need_border           = {};
  bool   is_room_entrance      = {};
  bool   is_room_exit          = {};
  bool   is_room_locked        = {};
  bool   is_room_secret        = {};
};

static auto level_populate_biome_dungeon(Gamep g, Levelsp v, Levelp l, class LevelGen *level_gen, class LevelPopulate &lp) -> Tpp
{
  TRACE();

  Tpp tp = {};

  switch (lp.c) {
    case CHARMAP_FLOOR :
      lp.need_floor = true;
      if (lp.is_decoration_allowed) {
        if (d100() < 5) {
          lp.need_foliage = true;
        }
      }
      break;
    case CHARMAP_DIRT :
      lp.need_dirt = true;
      if (lp.is_decoration_allowed) {
        if (d100() < 50) {
          lp.need_foliage = true;
        }
      }
      break;
    case CHARMAP_CHASM : tp = lp.tp_chasm; break;
    case CHARMAP_JOIN :
      lp.need_corridor = true;
      break;
      break;
    case CHARMAP_CORRIDOR :
      tp = lp.tp_corridor;
      break;
      break;
    case CHARMAP_BRIDGE : tp = lp.tp_bridge; break;
    case CHARMAP_VAULT :
      lp.need_floor = true;
      tp            = lp.tp_vault;
      break;
    case CHARMAP_WALL :
      lp.need_floor = true;
      tp            = lp.tp_wall;
      break;
    case CHARMAP_ROCK :
      lp.need_dirt = true;
      tp           = lp.tp_rock;
      break;
    case CHARMAP_TREASURE :
      lp.need_floor = true;
      tp            = tp_random(g, v, l, is_treasure);
      break;
    case CHARMAP_TELEPORT :
      lp.need_floor = true;
      tp            = lp.tp_teleport;
      break;
    case CHARMAP_FOLIAGE : lp.need_foliage = true; break;
    case CHARMAP_DEEP_WATER :
      lp.need_dirt  = true;
      tp            = lp.tp_deep_water;
      lp.need_water = true;
      break;
    case CHARMAP_WATER :
      lp.need_dirt  = true;
      lp.need_water = true;
      if (lp.is_decoration_allowed) {
        if (d100() < 5) {
          lp.need_foliage = true;
        }
      }
      break;
    case CHARMAP_BARREL :
      lp.need_floor = true;
      tp            = lp.tp_barrel;
      break;
    case CHARMAP_PILLAR :
      lp.need_floor = true;
      tp            = lp.tp_pillar;
      break;
    case CHARMAP_TRAP :
      lp.need_floor = true;
      tp            = tp_random(g, v, l, is_trap);
      break;
    case CHARMAP_LAVA :
      lp.need_dirt = true;
      tp           = lp.tp_lava;
      break;
    case CHARMAP_BRAZIER :
      lp.need_floor = true;
      tp            = lp.tp_brazier;
      break;
    case CHARMAP_DOOR_UNLOCKED :
      lp.need_floor = true;
      tp            = tp_random(g, v, l, is_door_unlocked);
      break;
    case CHARMAP_DOOR_LOCKED :
      lp.need_floor = true;
      if (lp.is_test_level) {
        tp = tp_random(g, v, l, is_door_locked);
      } else if (l->info.key_count != 0) {
        tp = tp_random(g, v, l, is_door_locked);
      } else {
        tp = tp_variant(is_door_secret, lp.wall_variant);
      }
      break;
    case CHARMAP_DOOR_SECRET :
      lp.need_floor = true;
      tp            = tp_variant(is_door_secret, lp.wall_variant);
      break;
    case CHARMAP_GRASS :
      lp.need_floor = true;
      tp            = lp.tp_grass;
      break;
    case CHARMAP_MONST_EASY :
      lp.need_floor = true;
      if (! lp.is_room_entrance) {
        tp = tp_random_monst(g, v, l, MONST_GROUP_EASY);
      }
      break;
    case CHARMAP_MONST_HARD :
      lp.need_floor = true;
      if (! lp.is_room_entrance) {
        tp = tp_random_monst(g, v, l, MONST_GROUP_HARD);
      }
      break;
    case CHARMAP_MOB1 :
      lp.need_floor = true;
      if (! lp.is_room_entrance) {
        tp = tp_random(g, v, l, is_mob1);
      }
      break;
    case CHARMAP_MOB2 :
      lp.need_floor = true;
      if (! lp.is_room_entrance) {
        tp = tp_random(g, v, l, is_mob2);
      }
      break;
    case CHARMAP_FIRE :
      lp.need_floor = true;
      tp            = tp_random(g, v, l, is_fire);
      break;
    case CHARMAP_ENTRANCE :
      lp.need_floor = true;

      //
      // Every floor except the first lp.needs an entrance
      //
      lp.need_entrance = true;

      //
      // First level. Usually 0 but can be a specified level.
      //
      if ((g_level_opt.level_num == l->level_num) && g_level_opt.is_set) {
        //
        // This is the chosen start level
        //
        tp = lp.tp_player;
      } else if ((l->level_num == 0) && ! g_level_opt.is_set) {
        //
        // This is the default start level
        //
        tp               = lp.tp_player;
        lp.need_entrance = false;
      }
      break;
    case CHARMAP_EXIT :
      lp.need_floor = true;
      tp            = lp.tp_exit;
      l->exit       = lp.at;
      break;
    case CHARMAP_KEY :
      lp.need_floor = true;
      tp            = tp_random(g, v, l, is_key);
      break;
    case CHARMAP_BORDER :
      lp.need_dirt = true;
      tp           = lp.tp_border;
      break;
    case CHARMAP_EMPTY :
      lp.need_dirt = true;
      tp           = lp.tp_rock;
      break;
    default :
      if (! g_opt_do_level_gen) {
        CROAK("unexpected map char '%c'", lp.c);
      }
  }

  return tp;
}

static auto level_populate_biome_bogland(Gamep g, Levelsp v, Levelp l, class LevelGen *level_gen, class LevelPopulate &lp) -> Tpp
{
  TRACE();

  Tpp tp = {};

  //
  // Add an inner border to obscure the outer border
  //
  if ((lp.c != CHARMAP_WALL) && (lp.c != CHARMAP_VAULT) && (lp.c != CHARMAP_ROCK)) {
    if ((lp.at.x == 1) || (lp.at.x == MAP_WIDTH - 2) || (lp.at.y == 1) || (lp.at.y == MAP_HEIGHT - 2)) {
      lp.need_foliage = true;
    }
  }

  switch (lp.c) {
    case CHARMAP_FLOOR :
      lp.need_dirt = true;
      if (d100() < 50) {
        lp.need_foliage = true;
      }
      if (d100() < 10) {
        lp.need_water = true;
      }
      break;
    case CHARMAP_JOIN :     lp.need_corridor = true; break;
    case CHARMAP_CORRIDOR : tp = lp.tp_bridge; break;
    case CHARMAP_VAULT :
      lp.need_floor = true;
      tp            = lp.tp_vault;
      break;
    case CHARMAP_WALL :
      if (d100() < 30) {
        lp.need_floor = true;
        tp            = lp.tp_wall;
      } else {
        lp.need_dirt  = true;
        lp.need_water = true;
        if (d100() < 50) {
          lp.need_foliage = true;
        }
      }
      break;
    case CHARMAP_ROCK :
      lp.need_dirt  = true;
      lp.need_water = true;
      if (d100() < 50) {
        lp.need_foliage = true;
      }
      break;
    case CHARMAP_LAVA :
      lp.need_dirt  = true;
      tp            = lp.tp_deep_water;
      lp.need_water = true;
      break;
    case CHARMAP_DOOR_UNLOCKED : lp.need_dirt = true; break;
    case CHARMAP_EMPTY :
      lp.need_dirt  = true;
      lp.need_water = true;
      if (d100() < 50) {
        lp.need_foliage = true;
      }
      break;
    case CHARMAP_BRAZIER :     [[fallthrough]];
    case CHARMAP_DIRT :        [[fallthrough]];
    case CHARMAP_CHASM :       [[fallthrough]];
    case CHARMAP_BRIDGE :      [[fallthrough]];
    case CHARMAP_TREASURE :    [[fallthrough]];
    case CHARMAP_TELEPORT :    [[fallthrough]];
    case CHARMAP_FOLIAGE :     [[fallthrough]];
    case CHARMAP_DEEP_WATER :  [[fallthrough]];
    case CHARMAP_WATER :       [[fallthrough]];
    case CHARMAP_BARREL :      [[fallthrough]];
    case CHARMAP_PILLAR :      [[fallthrough]];
    case CHARMAP_TRAP :        [[fallthrough]];
    case CHARMAP_DOOR_LOCKED : [[fallthrough]];
    case CHARMAP_DOOR_SECRET : [[fallthrough]];
    case CHARMAP_GRASS :       [[fallthrough]];
    case CHARMAP_MONST_EASY :  [[fallthrough]];
    case CHARMAP_MONST_HARD :  [[fallthrough]];
    case CHARMAP_MOB1 :        [[fallthrough]];
    case CHARMAP_MOB2 :        [[fallthrough]];
    case CHARMAP_FIRE :        [[fallthrough]];
    case CHARMAP_ENTRANCE :    [[fallthrough]];
    case CHARMAP_EXIT :        [[fallthrough]];
    case CHARMAP_KEY :         [[fallthrough]];
    case CHARMAP_BORDER :      return level_populate_biome_dungeon(g, v, l, level_gen, lp);
    default :
      if (! g_opt_do_level_gen) {
        CROAK("unexpected map char '%c'", lp.c);
      }
  }

  return tp;
}

auto level_populate(Gamep g, Levelsp v, Levelp l, class LevelGen *level_gen, int w, int h, const char *in, const Overrides &overrides)
    -> bool
{
  TRACE();

  auto expected_len = w * h;

  if (std::cmp_not_equal(strlen(in), expected_len)) {
    CROAK("bad map size, expected %d chars, got %d chars for map of expected size %dx%d", (int) expected_len, (int) strlen(in), w, h);
  }

  LevelPopulate lp;

  lp.biome         = level_to_biome(g, v, l);
  lp.tp_wall       = tp_random(g, v, l, is_wall);
  lp.tp_border     = tp_first(is_border);
  lp.tp_rock       = tp_random(g, v, l, is_rock);
  lp.tp_water      = tp_random(g, v, l, is_water);
  lp.tp_lava       = tp_random(g, v, l, is_lava);
  lp.tp_bridge     = tp_random(g, v, l, is_bridge);
  lp.tp_chasm      = tp_random(g, v, l, is_chasm);
  lp.tp_deep_water = tp_random(g, v, l, is_deep_water);
  lp.tp_brazier    = tp_random(g, v, l, is_brazier);
  lp.tp_pillar     = tp_random(g, v, l, is_pillar);
  lp.tp_barrel     = tp_random(g, v, l, is_barrel);
  lp.tp_teleport   = tp_random(g, v, l, is_teleport);
  lp.tp_foliage    = tp_random(g, v, l, is_foliage);
  lp.tp_corridor   = tp_random(g, v, l, is_corridor);
  lp.tp_grass      = tp_random(g, v, l, is_grass);
  lp.tp_floor      = tp_random(g, v, l, is_floor);
  lp.tp_dirt       = tp_find_mand("dirt");
  lp.tp_exit       = tp_find_mand("exit");
  lp.tp_player     = tp_find_mand("player");
  lp.tp_entrance   = tp_find_mand("entrance");

  //
  // We need secret doors to match the style of walls
  //
  lp.wall_variant = tp_variant_get(lp.tp_wall);

  //
  // Same with vaults
  //
  lp.tp_vault = tp_variant(is_vault, lp.wall_variant);

  lp.is_test_level         = g_opt_tests || l->is_fixed_level;
  lp.is_decoration_allowed = ! lp.is_test_level;

  for (auto y = 0; y < h; y++) {
    for (auto x = 0; x < w; x++) {
      auto offset = (w * y) + x;
      Tpp  tp     = nullptr;

      lp.c  = in[ offset ];
      lp.at = bpoint(x, y);

      lp.is_room_entrance = level_gen_is_room_entrance(g, level_gen, lp.at);
      lp.is_room_exit     = level_gen_is_room_exit(g, level_gen, lp.at);
      lp.is_room_locked   = level_gen_is_room_locked(g, level_gen, lp.at);
      lp.is_room_secret   = level_gen_is_room_secret(g, level_gen, lp.at);

      l->debug[ x ][ y ] = lp.c;

      lp.need_floor    = false;
      lp.need_entrance = false;
      lp.need_corridor = false;
      lp.need_water    = false;
      lp.need_dirt     = false;
      lp.need_foliage  = false;
      lp.need_border   = ! lp.is_test_level && ((x == 0) || (x == MAP_WIDTH - 1) || (y == 0) || (y == MAP_HEIGHT - 1));

      auto o = overrides.find(lp.c);
      if (o != overrides.end()) {
        //
        // Allow some characters to be overriden. e.g. to place a specific monster
        //
        tp = (o->second)(lp.c, lp.at);
        if (tp == nullptr) [[unlikely]] {
          ERR("could not find a template for override char %c", lp.c);
          return false;
        }
        lp.need_floor = true;
      } else if (lp.need_border) {
        //
        // Border tiles around the edge to stop the player
        //
        tp = lp.tp_border;
      } else if (lp.is_test_level || lp.is_room_exit || lp.is_room_locked || lp.is_room_secret) {
        //
        // Keep these special rooms as is
        //
        tp = level_populate_biome_dungeon(g, v, l, level_gen, lp);
      } else {
        //
        // Per biome tweaks
        //
        switch (lp.biome) {
          case BIOME_DUNGEON :    tp = level_populate_biome_dungeon(g, v, l, level_gen, lp); break;
          case BIOME_BOGLAND :    tp = level_populate_biome_bogland(g, v, l, level_gen, lp); break;
          case BIOME_NETHERVOID : tp = level_populate_biome_dungeon(g, v, l, level_gen, lp); break;
          case BIOME_GRAVEYARD :  tp = level_populate_biome_dungeon(g, v, l, level_gen, lp); break;
          case BIOME_UNDERHELL :  tp = level_populate_biome_dungeon(g, v, l, level_gen, lp); break;
          case BIOME_ANY :        [[fallthrough]];
          case BIOME_ENUM_MAX :   break;
        }
      }

      //
      // Makes more sense plants grow from dirt
      //
      if (lp.need_foliage) {
        lp.need_floor = false;
        lp.need_dirt  = true;
      }

      if (lp.need_floor) {
        auto *tp_add = lp.tp_floor;
        if (thing_spawn(g, v, l, tp_add, lp.at) == nullptr) {
          return false;
        }
      }

      if (lp.need_corridor) {
        auto *tp_add = lp.tp_corridor;
        if (thing_spawn(g, v, l, tp_add, lp.at) == nullptr) {
          return false;
        }
      }

      if (lp.need_dirt) {
        auto *tp_add = lp.tp_dirt;
        if (thing_spawn(g, v, l, tp_add, lp.at) == nullptr) {
          return false;
        }
      }

      if (lp.need_water) {
        if (thing_spawn(g, v, l, lp.tp_water, lp.at) == nullptr) {
          return false;
        }
      }

      if (lp.need_foliage) {
        auto *tp_add = lp.tp_foliage;
        if (thing_spawn(g, v, l, tp_add, lp.at) == nullptr) {
          return false;
        }
      }

      if (lp.need_entrance) {
        auto *tp_add = lp.tp_entrance;
        if (thing_spawn(g, v, l, tp_add, lp.at) == nullptr) {
          return false;
        }
        l->entrance = lp.at;
      }

      if (tp != nullptr) {
        if (thing_spawn(g, v, l, tp, lp.at) == nullptr) {
          return false;
        }
      }

      if (! g_opt_tests) {
        if (0) {
          if (tp == lp.tp_player) {
            {
              if (thing_spawn(g, v, l, tp_random(g, v, l, is_exit), lp.at + bpoint(2, 0)) == nullptr) {
                return false;
              }
            }
          }
        }
      }
    }
  }

  if (0) {
    if (l->entrance.x > 0) {
      if (thing_spawn(g, v, l, tp_random(g, v, l, is_exit), l->entrance + bpoint(1, 1)) == nullptr) {
        return false;
      }
    }
  }

  return level_populated(g, v, l);
}

auto level_populate(Gamep g, Levelsp v, Levelp l, class LevelGen *level_gen, const char *in, const Overrides &overrides) -> bool
{
  TRACE();

  if (! level_populate(g, v, l, level_gen, MAP_WIDTH, MAP_HEIGHT, in, overrides)) {
    ERR("level populate failed");
    return false;
  }

  return true;
}
