//
// Copyright goblinhack@gmail.com
//

#include "my_globals.hpp"
#include "my_level.hpp"
#include "my_main.hpp"
#include "my_test.hpp"

#include <format>

void game_debug_info(Gamep g)
{
  TRACE();

  auto *v = game_levels_get(g);
  if (v == nullptr) {
    return;
  }

  LOG("level info:");
  LOG("- Levels memory:                   %" PRI_SIZE_T " Mb", sizeof(Levels) / (1024 * 1024));
  LOG("- Thing ext memory:                %" PRI_SIZE_T " Mb", sizeof(v->thing_ext) / (1024 * 1024));
  LOG("- Thing fov memory:                %" PRI_SIZE_T " Mb", sizeof(v->thing_light) / (1024 * 1024));
  LOG("- sizeof(Thing):                   %" PRI_SIZE_T " b", sizeof(Thing));
  LOG("- sizeof(ThingInventory):          %" PRI_SIZE_T " b", sizeof(ThingInventory));
  LOG("- sizeof(ThingExt)                 %" PRI_SIZE_T " b", sizeof(ThingExt));
  LOG("- sizeof(ThingLight)               %" PRI_SIZE_T " b", sizeof(ThingLight));
  LOG("- sizeof(ThingPlayer):             %" PRI_SIZE_T " b", sizeof(ThingPlayer));
  LOG("- MAP_HEIGHT                       %u", MAP_HEIGHT);
  LOG("- MAP_WIDTH                        %u", MAP_WIDTH);
  LOG("- MAP_SLOTS                        %u", MAP_SLOTS);
  LOG("- LEVEL_SCALE                      %u", LEVEL_SCALE);
  LOG("- LEVEL_ACROSS                     %u", LEVEL_ACROSS);
  LOG("- LEVEL_DOWN                       %u", LEVEL_DOWN);
  LOG("- LEVEL_SELECT_ID                  %u", LEVEL_SELECT_ID);
  LOG("- LEVEL_MAX                        %u", LEVEL_MAX);
  LOG("- LEVEL_ID_MAX                     %u", THING_ID_PER_LEVEL_MAX);
  LOG("- THING_LEVEL_ID_BITS              %u (%u indices)", THING_LEVEL_ID_BITS, (1 << THING_LEVEL_ID_BITS) - 1);
  LOG("- THING_PER_LEVEL_THING_ID_BITS    %u (%u indices)", THING_PER_LEVEL_THING_ID_BITS, (1 << THING_PER_LEVEL_THING_ID_BITS) - 1);
  LOG("- THING_ARR_INDEX_BITS             %u (%u total indices)", THING_ARR_INDEX_BITS, (1 << THING_ARR_INDEX_BITS) - 1);
  LOG("- THING_ENTROPY_BITS               %u", THING_ENTROPY_BITS);
  LOG("- THING_ID_PER_LEVEL_REQ           %u", THING_ID_PER_LEVEL_REQ);
  LOG("- THING_ID_PER_LEVEL_MAX           %u", THING_ID_PER_LEVEL_MAX);
  LOG("- THING_ID_MAX                     %u", THING_ID_MAX);

  if (LEVEL_ID_REQ > LEVEL_ID_MAX) {
    CROAK("You need more bits for level IDs as the level will use too many");
  }

  if (THING_ID_PER_LEVEL_REQ > THING_ID_PER_LEVEL_MAX) {
    CROAK("You need more bits for thing IDs as the level will use too many");
  }

  thing_stats_dump(g, v);
}

auto levels_thing_count(Gamep g, Levelsp v) -> int
{
  TRACE();

  int thing_count = 0;
  for (auto &arr_index : v->thing_body) {
    auto *t = &arr_index;

#ifdef ENABLE_PER_THING_MEMORY
    if (v->thing_body_debug[ arr_index ]) {
      t = v->thing_body_debug[ arr_index ];
      if (! t) {
        continue;
      }
    }
#endif

    if (t->tp_id != 0U) {
      thing_count++;
    }
  }

  return thing_count;
}

auto levels_thing_ext_count(Gamep g, Levelsp v) -> int
{
  TRACE();

  return v->thing_ext_count;
}

void level_debug(Gamep g, Levelsp v, Levelp l)
{
  TRACE();

  level_log(g, v, l, "level         : %d", l->level_num);
  level_log(g, v, l, "seed          : %u", l->info.seed_num);

  if (l->info.room_count != 0) {
    level_log(g, v, l, "room count        : %d", l->info.room_count);
    level_log(g, v, l, "entrance at       : %d,%d", l->info.entrance_at.x, l->info.entrance_at.y);
    level_log(g, v, l, "exit at           : %d,%d", l->info.exit_at.x, l->info.exit_at.y);
    level_log(g, v, l, "fragment count    : %d", l->info.fragment_count);
    level_log(g, v, l, "treasure count    : %d", l->info.treasure_count);
    level_log(g, v, l, "monst count       : %d (mob:%d easy:%d hard:%d)", l->info.monst_count, l->info.monst_group_mob_count,
              l->info.monst_group_easy_count, l->info.monst_group_hard_count);
    level_log(g, v, l, "teleport count    : %d", l->info.teleport_count);
    level_log(g, v, l, "locked door count : %d", l->info.door_locked_count);
    level_log(g, v, l, "key count         : %d", l->info.key_count);
    if (l->player_has_entered_level) {
      level_log(g, v, l, "- player has entered level");
    }
    if (l->player_completed_level_via_exit) {
      level_log(g, v, l, "- player completed level via exit");
    }
    if (l->player_fell_out_of_level) {
      level_log(g, v, l, "- player fell out of level");
    }
    if (l->player_can_enter_this_level_next) {
      level_log(g, v, l, "- player can enter this level next");
    }
  }

  for (int y = 0; y < MAP_HEIGHT; y++) {
    std::string tmp;
    for (auto &x : l->debug) {
      auto c = x[ y ];
      if (c != 0) {
        tmp += x[ y ];
      }
    }

    if (! tmp.empty()) {
      level_log(g, v, l, "[%s]", tmp.c_str());
    }
  }

  level_log(g, v, l, "-");
}
