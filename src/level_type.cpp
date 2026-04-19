//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_globals.hpp"
#include "my_level.hpp"
#include "my_main.hpp"

auto level_type(LevelNum level_num) -> LevelType
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

auto level_is_boss_level(Gamep g, Levelsp v, Levelp l) -> bool
{
  TRACE();

  switch (l->level_num + 1) {
    case 1 :  return false;
    case 2 :  return false;
    case 3 :  return false;
    case 4 :  return false;
    case 5 :  return true;
    case 6 :  return false;
    case 7 :  return false;
    case 8 :  return false;
    case 9 :  return false;
    case 10 : return true;
    case 11 : return false;
    case 12 : return false;
    case 13 : return false;
    case 14 : return false;
    case 15 : return true;
    case 16 : return false;
    case 17 : return false;
    case 18 : return false;
    case 19 : return false;
    case 20 : return true;
    case 21 : return false;
    case 22 : return false;
    case 23 : return false;
    case 24 : return false;
    case 25 : return true;
  }
  return false;
}
