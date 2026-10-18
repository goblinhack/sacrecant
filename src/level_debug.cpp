//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_game.hpp"
#include "my_game_defs.hpp"
#include "my_level.hpp"
#include "my_main.hpp"
#include "my_thing.hpp"
#include "my_types.hpp"

#include <cmath>
#include <string>

void game_debug_info(Gamep g)
{
  TRACE();

  IF_NODEBUG { return; }

  auto *v = game_levels_get(g);
  if (v == nullptr) [[unlikely]] {
    return;
  }

  log("Level info:");
  log("  Levels memory:                   %" PRI_SIZE_T " Mb", sizeof(Levels) / (1024 * 1024));
  log("  Thing body memory:               %" PRI_SIZE_T " Mb", sizeof(v->thing_body) / (1024 * 1024));
  log("  Thing ext memory:                %" PRI_SIZE_T " Mb", sizeof(v->thing_ext) / (1024 * 1024));
  log("  Thing dmap memory:               %" PRI_SIZE_T " Mb", sizeof(v->thing_dmap) / (1024 * 1024));
  log("  Thing light memory:              %" PRI_SIZE_T " Mb", sizeof(v->thing_light) / (1024 * 1024));
  log("  sizeof(Thing):                   %" PRI_SIZE_T " b", sizeof(Thing));
  log("  sizeof(ThingInventory):          %" PRI_SIZE_T " b", sizeof(ThingInventory));
  log("  sizeof(ThingExt)                 %" PRI_SIZE_T " b", sizeof(ThingExt));
  log("  sizeof(ThingDmap)                %" PRI_SIZE_T " b", sizeof(ThingDmap));
  log("  sizeof(ThingLight)               %" PRI_SIZE_T " b", sizeof(ThingLight));
  log("  sizeof(ThingPlayer):             %" PRI_SIZE_T " b", sizeof(ThingPlayer));
  log("  MAP_HEIGHT                       %u", MAP_HEIGHT);
  log("  MAP_WIDTH                        %u", MAP_WIDTH);
  log("  MAP_SLOTS                        %u", MAP_SLOTS);
  log("  LEVEL_GRID_ACROSS                %u", LEVEL_GRID_ACROSS);
  log("  LEVEL_GRID_DOWN                  %u", LEVEL_GRID_DOWN);
  log("  LEVEL_ARR_IDX_LEVEL_SELECT       %u", LEVEL_ARR_IDX_LEVEL_SELECT);
  log("  LEVEL_ARR_IDX_LEVEL_SECRET       %u", LEVEL_ARR_IDX_LEVEL_SECRET);
  log("  LEVEL_ARR_IDX_MAX                %u", LEVEL_ARR_IDX_MAX);
  log("  LEVEL_ID_MAX                     %u", THING_ID_PER_LEVEL_MAX);
  log("  THING_LEVEL_ID_BITS              %u (%u indices)", THING_LEVEL_ID_BITS, (1 << THING_LEVEL_ID_BITS) - 1);
  log("  THING_PER_LEVEL_THING_ID_BITS    %u (%u indices)", THING_PER_LEVEL_THING_ID_BITS, (1 << THING_PER_LEVEL_THING_ID_BITS) - 1);
  log("  THING_ARR_INDEX_BITS             %u (%u total indices)", THING_ARR_INDEX_BITS, (1 << THING_ARR_INDEX_BITS) - 1);
  log("  THING_ENTROPY_BITS               %u", THING_ENTROPY_BITS);
  log("  THING_ID_PER_LEVEL_REQ           %u", THING_ID_PER_LEVEL_REQ);
  log("  THING_ID_PER_LEVEL_MAX           %u", THING_ID_PER_LEVEL_MAX);
  log("  THING_ID_MAX                     %u", THING_ID_MAX);

  levels_thing_id_histogram(g, v);
  levels_thing_ext_histogram(g, v);
  levels_thing_dmap_histogram(g, v);
  levels_thing_light_histogram(g, v);

  if (LEVEL_ID_REQ > LEVEL_ID_MAX) {
    CROAK("You need more bits for level IDs as the level will use too many");
  }

  if (THING_ID_PER_LEVEL_REQ > THING_ID_PER_LEVEL_MAX) {
    CROAK("You need more bits for thing IDs as the level will use too many");
  }

  if (THING_EXT_ID_MAX >= 1 << (sizeof(ThingExtId) * 8)) {
    CROAK("ThingExtId is not a big enough type");
  }

  if (THING_DMAP_ID_MAX >= 1 << (sizeof(ThingDmapId) * 8)) {
    CROAK("ThingDmapId is not a big enough type");
  }

  if (THING_LIGHT_ID_MAX >= 1 << (sizeof(ThingLightId) * 8)) {
    CROAK("ThingLightId is not a big enough type");
  }

  thing_stats_dump(g, v);
}

[[nodiscard]] auto levels_thing_count(Gamep g, Levelsp v) -> int
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

[[nodiscard]] auto levels_thing_ext_count(Gamep g, Levelsp v) -> int
{
  TRACE();

  auto count = 0;
  for (auto id = 0; id < THING_EXT_ID_MAX; id++) {
    if (v->thing_ext[ id ].in_use) {
      count++;
    }
  }
  return count;
}

static auto levels_histogram(Gamep g, Levelsp v, int level_num, int count, int max_count) -> void
{
  TRACE();

  int         histogram_width = 32;
  int         pct             = (int) (((float) count / (float) max_count) * histogram_width);
  std::string s;

  while (pct-- > 0) {
    s += '#';
  }

  while ((int) s.size() < histogram_width) {
    s += '.';
  }

  log("  L%2d used: %5d max: %5d %s", level_num, count, max_count, s.c_str());
}

auto levels_thing_id_histogram(Gamep g, Levelsp v) -> void
{
  TRACE();

  auto max_per_level_count = (1 << THING_PER_LEVEL_THING_ID_BITS);

  log("Thing id statistics:");

  for (auto level_num = 0; level_num < LEVEL_ARR_IDX_MAX; level_num++) {
    int count = {};

    for (auto i = 0; i < max_per_level_count; i++) {
      auto id = max_per_level_count * level_num;
      if (v->thing_body[ id + i ].tp_id) {
        count++;
      }
    }
    levels_histogram(g, v, level_num, count, max_per_level_count);
  }
}

auto levels_thing_ext_histogram(Gamep g, Levelsp v) -> void
{
  TRACE();

  auto max_per_level_count = THING_EXT_ID_PER_LEVEL_MAX;

  log("Thing ext statistics:");

  for (auto level_num = 0; level_num < LEVEL_ARR_IDX_MAX; level_num++) {
    int count = {};

    for (auto i = 0; i < max_per_level_count; i++) {
      auto id = max_per_level_count * level_num;
      if (v->thing_ext[ id + i ].in_use) {
        count++;
      }
    }
    levels_histogram(g, v, level_num, count, max_per_level_count);
  }
}

auto levels_thing_dmap_histogram(Gamep g, Levelsp v) -> void
{
  TRACE();

  auto max_per_level_count = THING_DMAP_ID_PER_LEVEL_MAX;

  log("Thing dmap statistics:");

  for (auto level_num = 0; level_num < LEVEL_ARR_IDX_MAX; level_num++) {
    int count = {};

    for (auto i = 0; i < max_per_level_count; i++) {
      auto id = max_per_level_count * level_num;
      if (v->thing_dmap[ id + i ].in_use) {
        count++;
      }
    }
    levels_histogram(g, v, level_num, count, max_per_level_count);
  }
}

auto levels_thing_light_histogram(Gamep g, Levelsp v) -> void
{
  TRACE();

  auto max_per_level_count = THING_LIGHT_ID_PER_LEVEL_MAX;

  log("Thing light statistics:");

  for (auto level_num = 0; level_num < LEVEL_ARR_IDX_MAX; level_num++) {
    int count = {};

    for (auto i = 0; i < max_per_level_count; i++) {
      auto id = max_per_level_count * level_num;
      if (v->thing_light[ id + i ].in_use) {
        count++;
      }
    }
    levels_histogram(g, v, level_num, count, max_per_level_count);
  }
}

[[nodiscard]] auto levels_thing_dmap_count(Gamep g, Levelsp v) -> int
{
  TRACE();

  auto count = 0;
  for (auto id = 0; id < THING_DMAP_ID_MAX; id++) {
    if (v->thing_dmap[ id ].in_use) {
      count++;
    }
  }
  return count;
}

[[nodiscard]] auto levels_thing_light_count(Gamep g, Levelsp v) -> int
{
  TRACE();

  auto count = 0;
  for (auto id = 0; id < THING_LIGHT_ID_MAX; id++) {
    if (v->thing_light[ id ].in_use) {
      count++;
    }
  }
  return count;
}

void level_debug(Gamep g, Levelsp v, Levelp l)
{
  TRACE();

  level_log(g, v, l, "level             : idx:%d num:%d", l->level_arr_idx, l->level_arr_idx + 1);
  level_log(g, v, l, "seed              : %u", l->info.seed_num);

  if (l->info.room_count != 0) {
    level_log(g, v, l, "room count        : %d", l->info.room_count);
    level_log(g, v, l, "entrance at       : (%d,%d)", l->info.entrance_at.x, l->info.entrance_at.y);
    level_log(g, v, l, "exit at           : (%d,%d)", l->info.exit_at.x, l->info.exit_at.y);
    level_log(g, v, l, "fragment count    : %d", l->info.fragment_count);
    level_log(g, v, l, "treasure count    : %d", l->info.treasure_count);
    level_log(g, v, l, "monst count       : %d (group1:%d group2:%d)", l->info.monst_count, l->info.monst1_count, l->info.monst2_count);
    level_log(g, v, l, "mob count         : %d (group1:%d group2:%d)", l->info.mob_count, l->info.mob1_count, l->info.mob2_count);
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
