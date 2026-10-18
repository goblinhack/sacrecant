//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_game_defs.hpp"
#include "my_level.hpp"
#include "my_main.hpp"
#include "my_tp.hpp"
#include "my_types.hpp"

[[nodiscard]] auto level_to_biome(Gamep g, Levelsp v, Levelp l) -> BiomeType
{
  TRACE();

  if (l->biome != 0U) {
    return l->biome;
  }

  switch (l->level_arr_idx) {
    case 0 :                          return BIOME_DUNGEON;
    case 1 :                          return BIOME_DUNGEON;
    case 2 :                          return BIOME_DUNGEON;
    case 3 :                          return BIOME_DUNGEON;
    case 4 :                          return BIOME_DUNGEON;
    case 5 :                          return BIOME_DUNGEON;
    case 6 :                          return BIOME_DUNGEON;
    case 7 :                          return BIOME_DUNGEON;
    case 8 :                          return BIOME_DUNGEON;
    case 9 :                          return BIOME_DUNGEON;
    case 10 :                         return BIOME_DUNGEON;
    case 11 :                         return BIOME_DUNGEON;
    case 12 :                         return BIOME_DUNGEON;
    case 13 :                         return BIOME_DUNGEON;
    case 14 :                         return BIOME_DUNGEON;
    case LEVEL_ARR_IDX_BOSS_1 :       return BIOME_DUNGEON;
    case 16 :                         return BIOME_BOGLAND;
    case 17 :                         return BIOME_BOGLAND;
    case 18 :                         return BIOME_BOGLAND;
    case 19 :                         return BIOME_BOGLAND;
    case 20 :                         return BIOME_BOGLAND;
    case 21 :                         return BIOME_BOGLAND;
    case 22 :                         return BIOME_BOGLAND;
    case 23 :                         return BIOME_BOGLAND;
    case 24 :                         return BIOME_BOGLAND;
    case 25 :                         return BIOME_BOGLAND;
    case 26 :                         return BIOME_BOGLAND;
    case 27 :                         return BIOME_BOGLAND;
    case 28 :                         return BIOME_BOGLAND;
    case 29 :                         return BIOME_BOGLAND;
    case 30 :                         return BIOME_BOGLAND;
    case LEVEL_ARR_IDX_BOSS_2 :       return BIOME_BOGLAND;
    case 32 :                         return BIOME_BOGLAND;
    case 33 :                         return BIOME_NETHERVOID;
    case 34 :                         return BIOME_NETHERVOID;
    case 35 :                         return BIOME_NETHERVOID;
    case 36 :                         return BIOME_NETHERVOID;
    case 37 :                         return BIOME_NETHERVOID;
    case 38 :                         return BIOME_NETHERVOID;
    case 39 :                         return BIOME_NETHERVOID;
    case 40 :                         return BIOME_NETHERVOID;
    case 41 :                         return BIOME_NETHERVOID;
    case 42 :                         return BIOME_NETHERVOID;
    case 43 :                         return BIOME_NETHERVOID;
    case 44 :                         return BIOME_NETHERVOID;
    case 45 :                         return BIOME_NETHERVOID;
    case 46 :                         return BIOME_NETHERVOID;
    case LEVEL_ARR_IDX_BOSS_3 :       return BIOME_NETHERVOID;
    case 48 :                         return BIOME_NETHERVOID;
    case 49 :                         return BIOME_GRAVEYARD;
    case 50 :                         return BIOME_GRAVEYARD;
    case 51 :                         return BIOME_GRAVEYARD;
    case 52 :                         return BIOME_GRAVEYARD;
    case 53 :                         return BIOME_GRAVEYARD;
    case 54 :                         return BIOME_GRAVEYARD;
    case 55 :                         return BIOME_GRAVEYARD;
    case 56 :                         return BIOME_GRAVEYARD;
    case 57 :                         return BIOME_GRAVEYARD;
    case 58 :                         return BIOME_GRAVEYARD;
    case 59 :                         return BIOME_GRAVEYARD;
    case 60 :                         return BIOME_GRAVEYARD;
    case 61 :                         return BIOME_GRAVEYARD;
    case 62 :                         return BIOME_GRAVEYARD;
    case LEVEL_ARR_IDX_BOSS_4 :       return BIOME_GRAVEYARD;
    case 64 :                         return BIOME_GRAVEYARD;
    case 65 :                         return BIOME_UNDERHELL;
    case 66 :                         return BIOME_UNDERHELL;
    case 67 :                         return BIOME_UNDERHELL;
    case 68 :                         return BIOME_UNDERHELL;
    case 69 :                         return BIOME_UNDERHELL;
    case 70 :                         return BIOME_UNDERHELL;
    case 71 :                         return BIOME_UNDERHELL;
    case 72 :                         return BIOME_UNDERHELL;
    case 73 :                         return BIOME_UNDERHELL;
    case 74 :                         return BIOME_UNDERHELL;
    case 75 :                         return BIOME_UNDERHELL;
    case 76 :                         return BIOME_UNDERHELL;
    case 77 :                         return BIOME_UNDERHELL;
    case 78 :                         return BIOME_UNDERHELL;
    case LEVEL_ARR_IDX_BOSS_5 :       return BIOME_UNDERHELL;
    case LEVEL_ARR_IDX_LEVEL_SELECT : return BIOME_DUNGEON;
    case LEVEL_ARR_IDX_LEVEL_SECRET : return BIOME_DUNGEON;
    default :                         ERR("unexpected level number with no biome assigned: %d", l->level_arr_idx); return BIOME_DUNGEON;
  }
}
