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

auto level_populate(Gamep g, Levelsp v, Levelp l, class LevelGen *level_gen, int w, int h, const char *in, const Overrides &overrides)
    -> bool
{
  TRACE();

  auto expected_len = w * h;

  if (std::cmp_not_equal(strlen(in), expected_len)) {
    CROAK("bad map size, expected %d chars, got %d chars for map of expected size %dx%d", (int) expected_len, (int) strlen(in), w, h);
  }

  auto  biome         = level_to_biome(g, v, l);
  auto *tp_wall       = tp_random(g, v, l, is_wall);
  auto *tp_border     = tp_first(is_border);
  auto *tp_rock       = tp_random(g, v, l, is_rock);
  auto *tp_water      = tp_random(g, v, l, is_water);
  auto *tp_lava       = tp_random(g, v, l, is_lava);
  auto *tp_bridge     = tp_random(g, v, l, is_bridge);
  auto *tp_chasm      = tp_random(g, v, l, is_chasm);
  auto *tp_deep_water = tp_random(g, v, l, is_deep_water);
  auto *tp_brazier    = tp_random(g, v, l, is_brazier);
  auto *tp_pillar     = tp_random(g, v, l, is_pillar);
  auto *tp_barrel     = tp_random(g, v, l, is_barrel);
  auto *tp_teleport   = tp_random(g, v, l, is_teleport);
  auto *tp_foliage    = tp_random(g, v, l, is_foliage);
  auto *tp_corridor   = tp_random(g, v, l, is_corridor);
  auto *tp_grass      = tp_random(g, v, l, is_grass);
  auto *tp_floor      = tp_random(g, v, l, is_floor);
  auto *tp_dirt       = tp_find_mand("dirt");
  auto *tp_exit       = tp_find_mand("exit");
  auto *tp_player     = tp_find_mand("player");
  auto *tp_entrance   = tp_find_mand("entrance");

  //
  // We need secret doors to match the style of walls
  //
  auto wall_variant = tp_variant_get(tp_wall);

  bool const is_test_level = g_opt_tests || l->is_fixed_level;
  bool const need_deco     = ! is_test_level;

  for (auto y = 0; y < h; y++) {
    for (auto x = 0; x < w; x++) {
      auto         offset = (w * y) + x;
      auto         c      = in[ offset ];
      Tpp          tp     = nullptr;
      bpoint const at(x, y);

      auto is_room_entrance = level_gen_is_room_entrance(g, level_gen, at);
      auto is_room_exit     = level_gen_is_room_exit(g, level_gen, at);
      auto is_room_locked   = level_gen_is_room_locked(g, level_gen, at);

      l->debug[ x ][ y ] = c;

      bool       need_floor    = false;
      bool       need_entrance = false;
      bool       need_corridor = false;
      bool       need_water    = false;
      bool       need_dirt     = false;
      bool       need_foliage  = false;
      bool const need_border   = ! is_test_level && ((x == 0) || (x == MAP_WIDTH - 1) || (y == 0) || (y == MAP_HEIGHT - 1));

      auto o = overrides.find(c);
      if (o != overrides.end()) {
        tp = (o->second)(c, at);
        if (tp == nullptr) [[unlikely]] {
          ERR("could not find a template for override char %c", c);
          return false;
        }
        need_floor = true;
      } else if (need_border) {
        tp = tp_border;
      } else {
        //
        // Add an inner border for some levels
        //
        if ((x == 1) || (x == MAP_WIDTH - 2) || (y == 1) || (y == MAP_HEIGHT - 2)) {
          switch (biome) {
            case BIOME_DUNGEON :    break;
            case BIOME_BOGLAND :    need_foliage = true; break;
            case BIOME_NETHERVOID : break;
            case BIOME_GRAVEYARD :  need_foliage = true; break;
            case BIOME_UNDERHELL :  break;
            case BIOME_ANY :        [[fallthrough]];
            case BIOME_ENUM_MAX :   break;
          }
        }

        switch (c) {
          case CHARMAP_FLOOR :
            need_floor = true;
            if (need_deco) {
              if (d100() < 5) {
                need_foliage = true;
              }
            }
            break;
          case CHARMAP_DIRT :
            need_dirt = true;
            if (need_deco) {
              if (d100() < 50) {
                need_foliage = true;
              }
            }
            break;
          case CHARMAP_CHASM : tp = tp_chasm; break;
          case CHARMAP_JOIN :
            switch (biome) {
              case BIOME_DUNGEON :    need_corridor = true; break;
              case BIOME_BOGLAND :    need_dirt = true; break;
              case BIOME_NETHERVOID : need_corridor = true; break;
              case BIOME_GRAVEYARD :  need_dirt = true; break;
              case BIOME_UNDERHELL :  need_dirt = true; break;
              case BIOME_ANY :        [[fallthrough]];
              case BIOME_ENUM_MAX :   break;
            }
            break;
          case CHARMAP_CORRIDOR :
            switch (biome) {
              case BIOME_DUNGEON :    tp = tp_corridor; break;
              case BIOME_BOGLAND :    tp = tp_bridge; break;
              case BIOME_NETHERVOID : tp = tp_bridge; break;
              case BIOME_GRAVEYARD :  need_dirt = true; break;
              case BIOME_UNDERHELL :  need_dirt = true; break;
              case BIOME_ANY :        [[fallthrough]];
              case BIOME_ENUM_MAX :   break;
            }
            break;
          case CHARMAP_BRIDGE : tp = tp_bridge; break;
          case CHARMAP_WALL :
            if (is_room_locked || is_room_exit) {
              need_floor = true;
              tp         = tp_wall;
            } else {
              switch (biome) {
                case BIOME_DUNGEON :
                  need_floor = true;
                  tp         = tp_wall;
                  break;
                case BIOME_BOGLAND :
                  if (d100() < 30) {
                    need_floor = true;
                    tp         = tp_wall;
                  } else {
                    need_dirt  = true;
                    need_water = true;
                    if (need_deco) {
                      if (d100() < 50) {
                        need_foliage = true;
                      }
                    }
                  }
                  break;
                case BIOME_NETHERVOID :
                  // no walls
                  need_floor = true;
                  break;
                case BIOME_GRAVEYARD :
                  need_floor = true;
                  tp         = tp_wall;
                  break;
                case BIOME_UNDERHELL :
                  if (d100() < 30) {
                    need_floor = true;
                    tp         = tp_wall;
                  }
                  break;
                case BIOME_ANY :      [[fallthrough]];
                case BIOME_ENUM_MAX : break;
              }
            }
            break;
          case CHARMAP_ROCK :
            switch (biome) {
              case BIOME_DUNGEON :
                need_dirt = true;
                tp        = tp_rock;
                break;
              case BIOME_BOGLAND :
                need_dirt  = true;
                need_water = true;
                if (need_deco) {
                  if (d100() < 50) {
                    need_foliage = true;
                  }
                }
                break;
              case BIOME_NETHERVOID :
                // no walls
                need_floor = true;
                break;
              case BIOME_GRAVEYARD :
                need_dirt = true;
                tp        = tp_rock;
                break;
              case BIOME_UNDERHELL :
                need_dirt = true;
                tp        = tp_lava;
                break;
              case BIOME_ANY :      [[fallthrough]];
              case BIOME_ENUM_MAX : break;
            }
            break;
          case CHARMAP_TREASURE :
            need_floor = true;
            tp         = tp_random(g, v, l, is_treasure);
            break;
          case CHARMAP_TELEPORT :
            need_floor = true;
            tp         = tp_teleport;
            break;
          case CHARMAP_FOLIAGE : need_foliage = true; break;
          case CHARMAP_DEEP_WATER :
            need_dirt  = true;
            tp         = tp_deep_water;
            need_water = true;
            break;
          case CHARMAP_WATER :
            need_dirt  = true;
            need_water = true;
            if (need_deco) {
              if (d100() < 5) {
                need_foliage = true;
              }
            }
            break;
          case CHARMAP_BARREL :
            need_floor = true;
            tp         = tp_barrel;
            break;
          case CHARMAP_PILLAR :
            need_floor = true;
            tp         = tp_pillar;
            break;
          case CHARMAP_TRAP :
            need_floor = true;
            tp         = tp_random(g, v, l, is_trap);
            break;
          case CHARMAP_LAVA :
            switch (biome) {
              case BIOME_DUNGEON :
                need_dirt = true;
                tp        = tp_lava;
                break;
              case BIOME_BOGLAND :
                need_dirt  = true;
                tp         = tp_deep_water;
                need_water = true;
                break;
              case BIOME_NETHERVOID : tp = tp_chasm; break;
              case BIOME_GRAVEYARD :  tp = tp_chasm; break;
              case BIOME_UNDERHELL :
                need_dirt = true;
                tp        = tp_lava;
                break;
              case BIOME_ANY :      [[fallthrough]];
              case BIOME_ENUM_MAX : break;
            }
            break;
          case CHARMAP_BRAZIER :
            need_floor = true;
            tp         = tp_brazier;
            break;
          case CHARMAP_DOOR_UNLOCKED :
            switch (biome) {
              case BIOME_DUNGEON :
                need_floor = true;
                tp         = tp_random(g, v, l, is_door_unlocked);
                break;
              case BIOME_BOGLAND :    need_dirt = true; break;
              case BIOME_NETHERVOID : need_dirt = true; break;
              case BIOME_GRAVEYARD :  need_dirt = true; break;
              case BIOME_UNDERHELL :  need_dirt = true; break;
              case BIOME_ANY :        [[fallthrough]];
              case BIOME_ENUM_MAX :   break;
            }
            break;
          case CHARMAP_DOOR_LOCKED :
            need_floor = true;
            if (is_test_level) {
              tp = tp_random(g, v, l, is_door_locked);
            } else if (l->info.key_count != 0) {
              tp = tp_random(g, v, l, is_door_locked);
            } else {
              tp = tp_variant(is_door_secret, wall_variant);
            }
            break;
          case CHARMAP_DOOR_SECRET :
            need_floor = true;
            tp         = tp_variant(is_door_secret, wall_variant);
            break;
          case CHARMAP_GRASS :
            need_floor = true;
            tp         = tp_grass;
            break;
          case CHARMAP_MONST_EASY :
            need_floor = true;
            if (! is_room_entrance) {
              tp = tp_random_monst(g, v, l, MONST_GROUP_EASY);
            }
            break;
          case CHARMAP_MONST_HARD :
            need_floor = true;
            if (! is_room_entrance) {
              tp = tp_random_monst(g, v, l, MONST_GROUP_HARD);
            }
            break;
          case CHARMAP_MOB1 :
            need_floor = true;
            if (! is_room_entrance) {
              tp = tp_random(g, v, l, is_mob1);
            }
            break;
          case CHARMAP_MOB2 :
            need_floor = true;
            if (! is_room_entrance) {
              tp = tp_random(g, v, l, is_mob2);
            }
            break;
          case CHARMAP_FIRE :
            need_floor = true;
            tp         = tp_random(g, v, l, is_fire);
            break;
          case CHARMAP_ENTRANCE :
            need_floor = true;

            //
            // Every floor except the first needs an entrance
            //
            need_entrance = true;

            //
            // First level. Usually 0 but can be a specified level.
            //
            if ((g_level_opt.level_num == l->level_num) && g_level_opt.is_set) {
              //
              // This is the chosen start level
              //
              tp = tp_player;
            } else if ((l->level_num == 0) && ! g_level_opt.is_set) {
              //
              // This is the default start level
              //
              tp            = tp_player;
              need_entrance = false;
            }
            break;
          case CHARMAP_EXIT :
            need_floor = true;
            tp         = tp_exit;
            l->exit    = at;
            break;
          case CHARMAP_KEY :
            need_floor = true;
            tp         = tp_random(g, v, l, is_key);
            break;
          case CHARMAP_BORDER :
            need_dirt = true;
            tp        = tp_border;
            break;
          case CHARMAP_EMPTY :
            switch (biome) {
              case BIOME_DUNGEON :
                need_dirt = true;
                tp        = tp_rock;
                break;
              case BIOME_BOGLAND :
                need_dirt  = true;
                need_water = true;
                if (need_deco) {
                  if (d100() < 50) {
                    need_foliage = true;
                  }
                }
                break;
              case BIOME_NETHERVOID :
                tp         = tp_chasm;
                need_floor = true;
                break;
              case BIOME_GRAVEYARD :
                // newline
                tp = tp_chasm;
                break;
              case BIOME_UNDERHELL :
                need_dirt = true;
                tp        = tp_lava;
                break;
              case BIOME_ANY :      [[fallthrough]];
              case BIOME_ENUM_MAX : break;
            }
            break;
          default :
            if (! g_opt_do_level_gen) {
              CROAK("unexpected map char '%c'", c);
            }
        }
      }

      //
      // Makes more sense plants grow from dirt
      //
      if (need_foliage) {
        need_floor = false;
        need_dirt  = true;
      }

      if (need_floor) {
        auto *tp_add = tp_floor;
        if (thing_spawn(g, v, l, tp_add, at) == nullptr) {
          return false;
        }
      }

      if (need_corridor) {
        auto *tp_add = tp_corridor;
        if (thing_spawn(g, v, l, tp_add, at) == nullptr) {
          return false;
        }
      }

      if (need_dirt) {
        auto *tp_add = tp_dirt;
        if (thing_spawn(g, v, l, tp_add, at) == nullptr) {
          return false;
        }
      }

      if (need_water) {
        if (thing_spawn(g, v, l, tp_water, at) == nullptr) {
          return false;
        }
      }

      if (need_foliage) {
        auto *tp_add = tp_foliage;
        if (thing_spawn(g, v, l, tp_add, at) == nullptr) {
          return false;
        }
      }

      if (need_entrance) {
        auto *tp_add = tp_entrance;
        if (thing_spawn(g, v, l, tp_add, at) == nullptr) {
          return false;
        }
        l->entrance = at;
      }

      if (tp != nullptr) {
        if (thing_spawn(g, v, l, tp, at) == nullptr) {
          return false;
        }
      }

      if (! g_opt_tests) {
        if (0) {
          if (tp == tp_player) {
            {
              if (thing_spawn(g, v, l, tp_random(g, v, l, is_exit), at + bpoint(2, 0)) == nullptr) {
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
