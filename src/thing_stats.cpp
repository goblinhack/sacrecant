//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_dice_rolls.hpp"
#include "my_main.hpp"
#include "my_sprintf.hpp"
#include "my_thing.hpp"
#include "my_thing_inlines.hpp" // NOLINT
#include "my_tp.hpp"
#include "my_types.hpp"
#include "my_ui.hpp"

#include <algorithm>
#include <cstdint>
#include <string>

[[nodiscard]] auto stat_to_mod(int val) -> int
{
  if (val <= THING_STAT_MIN) {
    return -9;
  }

  if (val >= THING_STAT_MAX) {
    return val - 10;
  }

  switch (val) {
    case 1 :  return -9;
    case 2 :  return -8;
    case 3 :  return -7;
    case 4 :  return -6;
    case 5 :  return -5;
    case 6 :  return -4;
    case 7 :  return -3;
    case 8 :  return -2;
    case 9 :  return -1;
    case 10 : return +0;
    case 11 : return +1;
    case 12 : return +2;
    case 13 : return +3;
    case 14 : return +4;
    case 15 : return +5;
    case 16 : return +6;
    case 17 : return +7;
    case 18 : return +8;
    case 19 : return +9;
    case 20 : return +10;
    default : return 0;
  }
}

[[nodiscard]] static auto stat_to_mod_string(int val) -> const std::string
{
  if (val <= THING_STAT_MIN) {
    return "-9";
  }

  if (val >= THING_STAT_MAX) {
    return "+" + std::to_string(val - 10);
  }

  switch (val) {
    case 1 :  return "-9";
    case 2 :  return "-8";
    case 3 :  return "-7";
    case 4 :  return "-6";
    case 5 :  return "-5";
    case 6 :  return "-4";
    case 7 :  return "-3";
    case 8 :  return "-2";
    case 9 :  return "-1";
    case 10 : return "+0";
    case 11 : return "+1";
    case 12 : return "+2";
    case 13 : return "+3";
    case 14 : return "+4";
    case 15 : return "+5";
    case 16 : return "+6";
    case 17 : return "+7";
    case 18 : return "+8";
    case 19 : return "+9";
    case 20 : return "+X";
    default : return "??";
  }
}

[[nodiscard]] static auto stat_to_name(ThingStatType stat) -> const std::string
{
  switch (stat) {
    case THING_STAT_ATT :  return "Att";
    case THING_STAT_DMG :  return "Dmg";
    case THING_STAT_DEF :  return "Def";
    case THING_STAT_STR :  return "Str";
    case THING_STAT_CON :  return "Con";
    case THING_STAT_INT :  return "Int";
    case THING_STAT_DEX :  return "Dex";
    case THING_STAT_PSI :  return "Psi";
    case THING_STAT_LUCK : return "Lck";
    default :              return "???";
  }
}

[[nodiscard]] auto thing_stat_set(Gamep g, Levelsp v, Levelp l, Thingp me, ThingStatType stat, uint8_t val) -> uint8_t
{
  TRACE_DEBUG();

  if (me == nullptr) {
    ERR("no thing pointer");
    return THING_STAT_DEFAULT;
  }

  if (stat >= THING_STAT_ENUM_MAX) {
    thing_err(g, v, l, me, "bad value in stat for %s, %d", __FUNCTION__, stat);
    return THING_STAT_DEFAULT;
  }

  return me->_stat[ stat ] = val;
}

//
// 19 -> 9
//
[[nodiscard]] auto thing_stat_mod(Gamep g, Levelsp v, Levelp l, Thingp me, ThingStatType stat) -> int
{
  TRACE_DEBUG();

  return stat_to_mod(thing_stat(g, v, l, me, stat));
}

//
// 19 -> "+9"
//
[[nodiscard]] static auto thing_stat_mod_value_string(Gamep g, Levelsp v, Levelp l, Thingp me, ThingStatType stat) -> std::string
{
  TRACE_DEBUG();

  auto val = thing_stat(g, v, l, me, stat);

  return stat_to_mod_string(val);
}

//
// "Con +9"
//
[[nodiscard]] auto thing_stat_mod_string(Gamep g, Levelsp v, Levelp l, Thingp me, ThingStatType stat) -> std::string
{
  TRACE_DEBUG();

  auto              mod       = thing_stat_mod(g, v, l, me, stat);
  auto              mod_str   = thing_stat_mod_value_string(g, v, l, me, stat);
  std::string const stat_name = stat_to_name(stat);
  std::string       stat_str;

  if (mod < -5) {
    stat_str = string_sprintf(UI_INFO_FMT_STR "%s" UI_IMPORTANT_FMT_STR " %-2s", stat_name.c_str(), mod_str.c_str());
  } else if (mod < -3) {
    stat_str = string_sprintf(UI_INFO_FMT_STR "%s" UI_WARN_FMT_STR " %-2s", stat_name.c_str(), mod_str.c_str());
  } else if (mod > 0) {
    stat_str = string_sprintf(UI_INFO_FMT_STR "%s" UI_GOOD_FMT_STR " %-2s", stat_name.c_str(), mod_str.c_str());
  } else {
    stat_str = string_sprintf(UI_INFO_FMT_STR "%s" UI_RESET_FMT " %-2s", stat_name.c_str(), mod_str.c_str());
  }

  return stat_str;
}

//
// "Att 20/+9"
//
[[nodiscard]] static auto thing_stat_dbg_string(Gamep g, Levelsp v, Levelp l, Thingp me, ThingStatType stat) -> std::string
{
  TRACE_DEBUG();

  auto              val       = thing_stat(g, v, l, me, stat);
  auto              mod       = thing_stat_mod(g, v, l, me, stat);
  auto              mod_str   = thing_stat_mod_value_string(g, v, l, me, stat);
  std::string const stat_name = stat_to_name(stat);
  std::string       stat_str;

  switch (stat) {
    case THING_STAT_ATT :  return string_sprintf("%s %2d", stat_name.c_str(), val);
    case THING_STAT_DEF :  return string_sprintf("%s %2d", stat_name.c_str(), val);
    case THING_STAT_STR :  break;
    case THING_STAT_CON :  break;
    case THING_STAT_INT :  break;
    case THING_STAT_DEX :  break;
    case THING_STAT_PSI :  break;
    case THING_STAT_DMG :  break;
    case THING_STAT_LUCK : break;
    default :              break;
  }

  if (mod < -5) {
    stat_str = string_sprintf("%s%2d/%-2s", stat_name.c_str(), val, mod_str.c_str());
  } else if (mod < -3) {
    stat_str = string_sprintf("%s%2d/%-2s", stat_name.c_str(), val, mod_str.c_str());
  } else if (mod > 0) {
    stat_str = string_sprintf("%s%2d/%-2s", stat_name.c_str(), val, mod_str.c_str());
  } else {
    stat_str = string_sprintf("%s%2d/%-2s", stat_name.c_str(), val, mod_str.c_str());
  }

  return stat_str;
}

//
// "Att 20/+9"
//
[[nodiscard]] auto thing_stat_string(Gamep g, Levelsp v, Levelp l, Thingp me, ThingStatType stat) -> std::string
{
  TRACE_DEBUG();

  auto              val       = thing_stat(g, v, l, me, stat);
  auto              mod       = thing_stat_mod(g, v, l, me, stat);
  auto              mod_str   = thing_stat_mod_value_string(g, v, l, me, stat);
  std::string const stat_name = stat_to_name(stat);
  std::string       stat_str;

  switch (stat) {
    case THING_STAT_ATT : [[fallthrough]];
    case THING_STAT_DEF :
      if (mod < -5) {
        return string_sprintf(UI_INFO_FMT_STR "%s" UI_RESET_FMT UI_IMPORTANT_FMT_STR " %2d", stat_name.c_str(), val);
      } else if (mod < -3) {
        return string_sprintf(UI_INFO_FMT_STR "%s" UI_RESET_FMT UI_WARN_FMT_STR " %2d", stat_name.c_str(), val);
      } else if (mod > 0) {
        return string_sprintf(UI_INFO_FMT_STR "%s" UI_RESET_FMT UI_GOOD_FMT_STR " %2d", stat_name.c_str(), val);
      } else {
        return string_sprintf(UI_INFO_FMT_STR "%s" UI_RESET_FMT " %2d", stat_name.c_str(), val);
      }

    case THING_STAT_STR :  break;
    case THING_STAT_CON :  break;
    case THING_STAT_INT :  break;
    case THING_STAT_DEX :  break;
    case THING_STAT_PSI :  break;
    case THING_STAT_DMG :  break;
    case THING_STAT_LUCK : break;
    default :              break;
  }

  if (mod < -5) {
    stat_str = string_sprintf(UI_INFO_FMT_STR "%s" UI_IMPORTANT_FMT_STR "%2d/%-2s", stat_name.c_str(), val, mod_str.c_str());
  } else if (mod < -3) {
    stat_str = string_sprintf(UI_INFO_FMT_STR "%s" UI_WARN_FMT_STR "%2d/%-2s", stat_name.c_str(), val, mod_str.c_str());
  } else if (mod > 0) {
    stat_str = string_sprintf(UI_INFO_FMT_STR "%s" UI_GOOD_FMT_STR "%2d/%-2s", stat_name.c_str(), val, mod_str.c_str());
  } else {
    stat_str = string_sprintf(UI_INFO_FMT_STR "%s" UI_RESET_FMT "%2d/%-2s", stat_name.c_str(), val, mod_str.c_str());
  }

  return stat_str;
}

//
// Luck -> 10 + +1 -> 11
//
[[nodiscard]] auto thing_stat(Gamep g, Levelsp v, Levelp l, Thingp me, ThingStatType stat) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    ERR("no thing pointer");
    return THING_STAT_DEFAULT;
  }

  if (stat >= THING_STAT_ENUM_MAX) {
    thing_err(g, v, l, me, "bad value in stat for %s, %d", __FUNCTION__, stat);
    return THING_STAT_DEFAULT;
  }

  auto stat_string = stat_to_name(stat);
  int  out         = me->_stat[ stat ];

  IF_DEBUG2
  {
    THING_DBG(g, v, l, me, "stat get: %s", stat_string.c_str());
    TRACE_INDENT();
  }

  FOR_ALL_HOOKS(g, v, l, me, buff)
  {
    auto mod = thing_stat_mod(g, v, l, buff, stat);

    out += mod;

    IF_DEBUG2
    {
      if (mod > 0) {
        THING_DBG(g, v, l, me, "mod: %s +%d (from buff)", stat_string.c_str(), out);
      } else {
        THING_DBG(g, v, l, me, "mod: %s %d (from buff)", stat_string.c_str(), out);
      }
    }
  }

  FOR_ALL_ACTIVE_ITEMS(g, v, l, me, item)
  {
    auto mod = thing_stat_mod(g, v, l, item, stat);
    if (mod == 0) {
      continue;
    }

    auto item_count = thing_inventory_get_item_count(g, v, l, item, me);
    if (item_count > 1) {
      IF_DEBUG2
      {
        if (mod > 0) {
          THING_DBG(g, v, l, item, "mod: +%d x %d (from item)", mod, item_count);
        } else {
          THING_DBG(g, v, l, item, "mod: %d x %d (from item)", mod, item_count);
        }
      }
      mod *= item_count;
    } else {
      IF_DEBUG2
      {
        if (mod > 0) {
          THING_DBG(g, v, l, item, "mod: +%d (from item)", mod);
        } else {
          THING_DBG(g, v, l, item, "mod: %d (from item)", mod);
        }
      }
    }

    out += mod;
  }

  out = std::max(THING_STAT_MIN, out);

  IF_DEBUG2
  {
    auto final_mod = stat_to_mod(out);
    if (final_mod > 0) {
      THING_DBG(g, v, l, me, "final stat: %s %d mod:+%d", stat_string.c_str(), out, final_mod);
    } else if (final_mod < 0) {
      THING_DBG(g, v, l, me, "final stat: %s %d mod:%d", stat_string.c_str(), out, final_mod);
    } else {
      THING_DBG(g, v, l, me, "final stat: %s %d no-mod", stat_string.c_str(), out);
    }
  }

  return out;
}

//
// Does d20 + stat modifier reach or exceed the target roll
//
[[nodiscard]] auto thing_stat_success(Gamep g, Levelsp v, Levelp l, Thingp me, ThingStatType stat, int target_roll, ThingEvent &e) -> bool
{
  TRACE_DEBUG();

  auto roll = d20();

  //
  // Override the level. This can happen during falling and the level we want to use is that of the thing.
  //
  l = thing_level(g, v, me);

  auto mod = thing_stat_mod(g, v, l, me, stat);

  if (roll <= 1) {
    THING_DBG(g, v, l, me, "roll: %s d20:%d mod:%d tot:%d vs:%d => fumble", thing_stat_dbg_string(g, v, l, me, stat).c_str(), roll, mod,
              roll + mod, target_roll);
    e.fumble = true;
    return false;
  }

  if (roll >= 20) {
    THING_DBG(g, v, l, me, "roll: %s d20:%d mod:%d tot:%d vs:%d => crit", thing_stat_dbg_string(g, v, l, me, stat).c_str(), roll, mod,
              roll + mod, target_roll);
    e.crit = true;
    return true;
  }

  if (roll + mod >= target_roll) {
    THING_DBG(g, v, l, me, "roll: %s d20:%d mod:%d tot:%d vs:%d => pass", thing_stat_dbg_string(g, v, l, me, stat).c_str(), roll, mod,
              roll + mod, target_roll);
    return true;
  }

  THING_DBG(g, v, l, me, "roll: %s d20:%d mod:%d tot:%d vs:%d => fail", thing_stat_dbg_string(g, v, l, me, stat).c_str(), roll, mod, roll + mod,
            target_roll);
  return false;
}

//
// Does d20 + stat modifier reach or exceed the target roll
//
[[nodiscard]] auto thing_stat_success(Gamep g, Levelsp v, Levelp l, Thingp me, ThingStatType stat, int target_roll) -> bool
{
  TRACE_DEBUG();

  ThingEvent e = {};

  return thing_stat_success(g, v, l, me, stat, target_roll, e);
}
