//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_globals.hpp"
#include "my_level.hpp"
#include "my_main.hpp"

LevelType level_type(LevelNum level_num)
{
  TRACE();

  switch (level_num) {
    case 1 :  return LEVEL_TYPE_NORMAL;
    case 2 :  return LEVEL_TYPE_NORMAL;
    case 3 :  return LEVEL_TYPE_NORMAL;
    case 4 :  return LEVEL_TYPE_NORMAL;
    case 5 :  return LEVEL_TYPE_BOSS1;
    case 6 :  return LEVEL_TYPE_NORMAL;
    case 7 :  return LEVEL_TYPE_NORMAL;
    case 8 :  return LEVEL_TYPE_NORMAL;
    case 9 :  return LEVEL_TYPE_NORMAL;
    case 10 : return LEVEL_TYPE_BOSS2;
    case 11 : return LEVEL_TYPE_NORMAL;
    case 12 : return LEVEL_TYPE_NORMAL;
    case 13 : return LEVEL_TYPE_NORMAL;
    case 14 : return LEVEL_TYPE_NORMAL;
    case 15 : return LEVEL_TYPE_BOSS3;
    case 16 : return LEVEL_TYPE_NORMAL;
    case 17 : return LEVEL_TYPE_NORMAL;
    case 18 : return LEVEL_TYPE_NORMAL;
    case 19 : return LEVEL_TYPE_NORMAL;
    case 20 : return LEVEL_TYPE_BOSS4;
    case 21 : return LEVEL_TYPE_NORMAL;
    case 22 : return LEVEL_TYPE_NORMAL;
    case 23 : return LEVEL_TYPE_NORMAL;
    case 24 : return LEVEL_TYPE_NORMAL;
    case 25 : return LEVEL_TYPE_BOSS5;
  }
  return LEVEL_TYPE_NORMAL;
}
