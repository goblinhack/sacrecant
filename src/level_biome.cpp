//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_globals.hpp"
#include "my_level.hpp"
#include "my_main.hpp"

auto level_to_biome(Gamep g, Levelsp v, Levelp l) -> Biome
{
  TRACE();

  switch (l->level_num) {
    case 0 :  return BIOME_DUNGEON;
    case 1 :  return BIOME_DUNGEON;
    case 2 :  return BIOME_DUNGEON;
    case 3 :  return BIOME_DUNGEON;
    case 4 :  return BIOME_DUNGEON;
    case 5 :  return BIOME_BOGLAND;
    case 6 :  return BIOME_BOGLAND;
    case 7 :  return BIOME_BOGLAND;
    case 8 :  return BIOME_BOGLAND;
    case 9 :  return BIOME_BOGLAND;
    case 10 : return BIOME_NETHERVOID;
    case 11 : return BIOME_NETHERVOID;
    case 12 : return BIOME_NETHERVOID;
    case 13 : return BIOME_NETHERVOID;
    case 14 : return BIOME_NETHERVOID;
    case 15 : return BIOME_GRAVEYARD;
    case 16 : return BIOME_GRAVEYARD;
    case 17 : return BIOME_GRAVEYARD;
    case 18 : return BIOME_GRAVEYARD;
    case 19 : return BIOME_GRAVEYARD;
    case 20 : return BIOME_UNDERHELL;
    case 21 : return BIOME_UNDERHELL;
    case 22 : return BIOME_UNDERHELL;
    case 23 : return BIOME_UNDERHELL;
    case 24 : return BIOME_UNDERHELL;
    default : return BIOME_DUNGEON;
  }
}
