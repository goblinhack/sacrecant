//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_color_defs.hpp"
#include "my_game_defs.hpp"
#include "my_game_popups.hpp"
#include "my_level.hpp"
#include "my_main.hpp"
#include "my_string.hpp"
#include "my_thing.hpp"
#include "my_thing_callbacks.hpp"
#include "my_thing_inlines.hpp"
#include "my_tp.hpp"
#include "my_tp_inlines.hpp"
#include "my_types.hpp"
#include "my_ui.hpp"

#include <algorithm>
#include <cassert>
#include <limits>
#include <string>
#include <vector>

//
// Roll for damage
//
[[nodiscard]] auto tp_damage(Tpp tp, ThingEventType val) -> int
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return 0;
  }

  if (val >= THING_EVENT_ENUM_MAX) {
    tp_err(tp, "bad value in tp for %s, %d", __FUNCTION__, val);
    return 0;
  }

  return tp->damage[ val ].roll();
}

[[nodiscard]] auto tp_damage_max(Tpp tp, ThingEventType event_type) -> int
{
  TRACE();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return 0;
  }

  if (event_type >= THING_EVENT_ENUM_MAX) {
    tp_err(tp, "bad value in tp for %s, %d", __FUNCTION__, event_type);
    return 0;
  }

  int damage = 0;

  for (const auto &d : tp->special_attacks) {
    auto spec = d.second;
    damage    = std::max(damage, spec.dice.max_roll());
  }

  return std::max(damage, tp->damage[ event_type ].max_roll());
}

[[nodiscard]] static auto tp_damage_max(Tpp tp) -> int
{
  TRACE();

  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return 0;
  }

  int damage = 0;

  FOR_ALL_THING_EVENT(e) { damage = std::max(damage, tp_damage_max(tp, e)); }

  return damage;
}

[[nodiscard]] auto thing_damage(Gamep g, Levelsp v, Levelp l, Thingp me, ThingEventType event_type) -> int
{
  TRACE();

  if (me == nullptr) {
    ERR("no thing pointer");
    return 0;
  }

  int final_damage {};
  int new_damage {};
  int initial_damage {};
  int additional_damage {};

  auto *tp = thing_tp(me);

  final_damage   = tp_damage(tp, event_type);
  initial_damage = final_damage;

  THING_DBG(g, v, l, me, "base damage: %d", final_damage);
  TRACE_INDENT();

  //
  // Add on my damage modifier
  //
  additional_damage = thing_stat_mod(g, v, l, me, THING_STAT_DMG);
  if (additional_damage != 0) {
    new_damage = final_damage + additional_damage;
    if (new_damage != final_damage) {
      THING_DBG(g, v, l, me, "damage-mod: %d->%d", final_damage, new_damage);
      final_damage = new_damage;
    }
  }

  //
  // Add on damage modifier from owner if any
  //
  Thingp from = nullptr;

  if (auto *fired_by = thing_missile_fired_by_get(g, v, l, me)) {
    from = fired_by;
  } else if (auto *owner = thing_owner(g, v, l, me)) {
    from = owner;
  } else {
    from = nullptr;
  }

  if (from != nullptr) {
    THING_DBG(g, v, l, me, "get owner damage:");
    TRACE_INDENT();

    additional_damage = thing_stat_mod(g, v, l, from, THING_STAT_DMG);
    if (additional_damage != 0) {
      new_damage = final_damage + additional_damage;
      if (new_damage != final_damage) {
        THING_DBG(g, v, l, me, "damage: %d->%d", final_damage, new_damage);
        final_damage = new_damage;
      }
    }
  }

  if (final_damage != initial_damage) {
    THING_DBG(g, v, l, me, "final damage: %d", final_damage);
  }

  return final_damage;
}

[[nodiscard]] auto thing_damage_max(Gamep g, Levelsp v, Levelp l, Thingp me, ThingEventType event_type) -> int
{
  TRACE();

  if (me == nullptr) {
    ERR("no thing pointer");
    return 0;
  }

  return tp_damage_max(thing_tp(me), event_type);
}

//
// What is the most damage this thing can do
//
[[nodiscard]] auto thing_damage_max(Gamep g, Levelsp v, Levelp l, Thingp me) -> int
{
  TRACE();

  int max_damage {};
  int new_damage {};
  int initial_damage {};

  auto *tp = thing_tp(me);

  max_damage     = std::max(max_damage, tp_damage_max(tp));
  initial_damage = max_damage;

  THING_DBG(g, v, l, me, "base damage max: %d", max_damage);
  TRACE_INDENT();

  for (const auto &d : tp->special_attacks) {
    auto val  = d.second;
    auto what = val.what;
    if (! what.empty()) {
      auto *what_tp = tp_find_mand(what);
      if (what_tp != nullptr) {
        new_damage = std::max(max_damage, tp_damage_max(what_tp));
        if (new_damage != max_damage) {
          TP_DBG(what_tp, "damage max: %d->%d (special attack)", max_damage, new_damage);
          max_damage = new_damage;
        }
      }
    }
  }

  FOR_ALL_WORN_TYPES(w)
  {
    auto *weapon = thing_worn_get(g, v, l, me, w);
    if (weapon != nullptr) {
      new_damage = std::max(max_damage, thing_damage_max(g, v, l, weapon));
      if (new_damage != max_damage) {
        THING_DBG(g, v, l, weapon, "damage max: %d->%d (item)", max_damage, new_damage);
        max_damage = new_damage;
      }
    }
  }

  new_damage = max_damage + thing_stat_mod(g, v, l, me, THING_STAT_DMG);
  if (new_damage != max_damage) {
    THING_DBG(g, v, l, me, "damage max: %d->%d (dmg stat)", max_damage, new_damage);
    max_damage = new_damage;
  }

  auto attack_count = tp_attack_count_max_per_tick_get(thing_tp(me));
  if (attack_count != 0) {
    new_damage = max_damage * attack_count;

    if (new_damage != max_damage) {
      THING_DBG(g, v, l, me, "damage max: %d->%d (attack count)", max_damage, new_damage);
      max_damage = new_damage;
    }
  }

  if (max_damage != initial_damage) {
    THING_DBG(g, v, l, me, "final damage max: %d", max_damage);
  }

  return max_damage;
}

[[nodiscard]] auto tp_damage_types(Tpp tp) -> std::vector< ThingEventType >
{
  TRACE();

  std::vector< ThingEventType > out;

  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return out;
  }

  FOR_ALL_THING_EVENT(e)
  {
    if (tp_damage_max(tp, e) != 0) {
      out.push_back(e);
    }
  }

  return out;
}

//
// What damage types can be done?
//
[[nodiscard]] auto thing_damage_types(Gamep g, Levelsp v, Levelp l, Thingp me) -> std::vector< ThingEventType >
{
  TRACE();

  std::vector< ThingEventType > out;

  auto *tp = thing_tp(me);

  {
    auto tmp = tp_damage_types(tp);
    out.insert(out.end(), tmp.begin(), tmp.end());
  }

  for (const auto &d : tp->special_attacks) {
    auto val = d.second;

    out.push_back(val.event_type);

    auto what = val.what;
    if (! what.empty()) {
      auto *what_tp = tp_find_mand(what);
      if (what_tp != nullptr) {
        auto tmp = tp_damage_types(what_tp);
        out.insert(out.end(), tmp.begin(), tmp.end());
      }
    }
  }

  FOR_ALL_WORN_TYPES(w)
  {
    auto *weapon = thing_worn_get(g, v, l, me, w);
    if (weapon != nullptr) {
      auto tmp = thing_damage_types(g, v, l, weapon);
      out.insert(out.end(), tmp.begin(), tmp.end());
    }
  }

  return out;
}

//
// The player has been attacked
//
static void thing_damage_to_player(Gamep g, Levelsp v, Levelp l, Thingp me, ThingEvent &e)
{
  TRACE();

  auto *it = e.source;

  //
  // Popup for damage
  //
  std::string msg;
  if (e.crit) {
    msg = "CRIT! -" + std::to_string(e.damage);
  } else {
    msg = "-" + std::to_string(e.damage);
  }
  auto at = thing_at(g, v, l, me);
  game_popup_text_add(g, at.x, at.y, msg, RED);

  auto damage_name = e.special_attack.name;

  if (it != nullptr) {
    std::string by_the_thing;
    auto       *fired_by = thing_missile_fired_by_get(g, v, l, it);
    if (fired_by != nullptr) {
      if (fired_by == me) {
        by_the_thing = "your " + thing_name_long(g, v, l, it);
      } else {
        by_the_thing = thing_name_apostrophize_the(g, v, l, fired_by) + " " + thing_name_long(g, v, l, it);
      }
    } else {
      by_the_thing = thing_name_long_the(g, v, l, it);
    }

    switch (e.event_type) {
      case THING_EVENT_THROWN : //
        topcon(UI_WARN_FMT_STR "You are thrown by %s!" UI_RESET_FMT, by_the_thing.c_str());
        break;
      case THING_EVENT_SHOVED : //
        topcon(UI_WARN_FMT_STR "You are shoved by %s." UI_RESET_FMT, by_the_thing.c_str());
        break;
      case THING_EVENT_CRUSH_DAMAGE : //
        topcon(UI_WARN_FMT_STR "You are crushed by %s." UI_RESET_FMT, by_the_thing.c_str());
        break;
      case THING_EVENT_ENGULF_DAMAGE :
        if (e.crit) {
          topcon(UI_WARN_FMT_STR "You suffer critted digestive damage from %s." UI_RESET_FMT, by_the_thing.c_str());
        } else {
          topcon(UI_WARN_FMT_STR "You suffer digestive damage from %s." UI_RESET_FMT, by_the_thing.c_str());
        }
        break;
      case THING_EVENT_THROWN_DAMAGE : [[fallthrough]];
      case THING_EVENT_POISON_DAMAGE : [[fallthrough]];
      case THING_EVENT_MELEE_DAMAGE :
        if (damage_name.empty()) {
          if (e.crit) {
            damage_name = "critted";
          } else {
            damage_name = "hit";
          }
        } else {
          if (e.crit) {
            damage_name = "critted(" + damage_name + ")";
          } else {
            damage_name = "hit(" + damage_name + ")";
          }
        }

        if (thing_attack_count_per_tick(it) > 1) {
          //
          // This is an assumption that only the player is being attacked.
          // We don't have a count per creature. This will be ok most of the time.
          //
          topcon(UI_WARN_FMT_STR "You are %s again by %s." UI_RESET_FMT, damage_name.c_str(), by_the_thing.c_str());
        } else {
          topcon(UI_WARN_FMT_STR "You are %s by %s." UI_RESET_FMT, damage_name.c_str(), by_the_thing.c_str());
        }
        break;
      case THING_EVENT_WATER_DAMAGE : //
        topcon(UI_WARN_FMT_STR "You suffer water damage from %s." UI_RESET_FMT, by_the_thing.c_str());
        break;
      case THING_EVENT_EXPLOSION_DAMAGE : //
        topcon(UI_WARN_FMT_STR "You suffer concussive damage from %s." UI_RESET_FMT, by_the_thing.c_str());
        break;
      case THING_EVENT_ENERGY_DAMAGE : //
        topcon(UI_WARN_FMT_STR "You suffer blast damage from %s." UI_RESET_FMT, by_the_thing.c_str());
        break;
      case THING_EVENT_FIRE_DAMAGE :
        if (e.temperature_damage) {
          if (e.nested_damage) {
            if (thing_is_lava(it)) {
              topcon(UI_WARN_FMT_STR "You suffer additional heat damage in lava!" UI_RESET_FMT);
            } else if (thing_is_fire(it)) {
              topcon(UI_WARN_FMT_STR "You suffer additional heat damage standing in the flames!" UI_RESET_FMT);
            } else if (thing_is_water_shallow(it) || thing_is_water_deep(it)) {
              topcon(UI_WARN_FMT_STR "You suffer additional heat damage from the boiling water." UI_RESET_FMT);
            } else if (thing_is_steam(it)) {
              topcon(UI_WARN_FMT_STR "You suffer additional heat damage from the scalding steam." UI_RESET_FMT);
            } else {
              topcon(UI_WARN_FMT_STR "You suffer additional heat damage from %s." UI_RESET_FMT, by_the_thing.c_str());
            }
          } else {
            if (thing_is_lava(it)) {
              topcon(UI_WARN_FMT_STR "You suffer heat damage in lava!" UI_RESET_FMT);
            } else if (thing_is_fire(it)) {
              topcon(UI_WARN_FMT_STR "You suffer heat damage from standing in the flames!" UI_RESET_FMT);
            } else if (thing_is_water_shallow(it) || thing_is_water_deep(it)) {
              topcon(UI_WARN_FMT_STR "You suffer heat damage from the boiling water." UI_RESET_FMT);
            } else if (thing_is_steam(it)) {
              topcon(UI_WARN_FMT_STR "You suffer heat damage from the scalding steam." UI_RESET_FMT);
            } else {
              topcon(UI_WARN_FMT_STR "You suffer heat damage from %s." UI_RESET_FMT, by_the_thing.c_str());
            }
          }
        } else {
          if (thing_is_lava(it)) {
            topcon(UI_WARN_FMT_STR "You are burning in lava!" UI_RESET_FMT);
          } else if (thing_is_fire(it)) {
            topcon(UI_WARN_FMT_STR "You are standing in flames!" UI_RESET_FMT);
          } else if (thing_is_water_shallow(it) || thing_is_water_deep(it)) {
            topcon(UI_WARN_FMT_STR "You are boiling in water." UI_RESET_FMT);
          } else if (thing_is_steam(it)) {
            topcon(UI_WARN_FMT_STR "You scalded by the steam." UI_RESET_FMT);
          } else {
            topcon(UI_WARN_FMT_STR "You are burnt by %s." UI_RESET_FMT, by_the_thing.c_str());
          }
        }
        break;
      case THING_EVENT_NONE :             [[fallthrough]];
      case THING_EVENT_USED :             [[fallthrough]];
      case THING_EVENT_EATEN :            [[fallthrough]];
      case THING_EVENT_LEVITATED :        [[fallthrough]];
      case THING_EVENT_OPEN :             [[fallthrough]];
      case THING_EVENT_GAME_OVER :        [[fallthrough]];
      case THING_EVENT_LIFESPAN_EXPIRED : [[fallthrough]];
      case THING_EVENT_FALL :             [[fallthrough]];
      case THING_EVENT_CARRIED :          [[fallthrough]];
      case THING_EVENT_CARRIED_MERGED :   [[fallthrough]];
      case THING_EVENT_MELT :             [[fallthrough]];
      case THING_EVENT_USER_INITIATED :   [[fallthrough]];
      case THING_EVENT_SPAWNED :          [[fallthrough]];
      case THING_EVENT_ENUM_MAX : //
        ERR("unexpected event: %s", ThingEventType_to_string(e.event_type).c_str());
        break;
    }
  } else {
    switch (e.event_type) {
      case THING_EVENT_THROWN : //
        topcon(UI_WARN_FMT_STR "You took %d damage from being thrown!" UI_RESET_FMT, e.damage);
        break;
      case THING_EVENT_FALL : //
        topcon(UI_WARN_FMT_STR "You took %d damage from falling." UI_RESET_FMT, e.damage);
        break;
      case THING_EVENT_SHOVED : //
        topcon(UI_WARN_FMT_STR "You are shoved." UI_RESET_FMT);
        break;
      case THING_EVENT_CRUSH_DAMAGE : //
        topcon(UI_WARN_FMT_STR "You are crushed." UI_RESET_FMT);
        break;
      case THING_EVENT_ENGULF_DAMAGE : //
        topcon(UI_WARN_FMT_STR "You are being digested!" UI_RESET_FMT);
        break;
      case THING_EVENT_POISON_DAMAGE : //
        topcon(UI_WARN_FMT_STR "You are being poisoned." UI_RESET_FMT);
        break;
      case THING_EVENT_THROWN_DAMAGE : [[fallthrough]];
      case THING_EVENT_MELEE_DAMAGE : //
        topcon(UI_WARN_FMT_STR "You are hit." UI_RESET_FMT);
        break;
      case THING_EVENT_WATER_DAMAGE : //
        topcon(UI_WARN_FMT_STR "You suffer water damage." UI_RESET_FMT);
        break;
      case THING_EVENT_EXPLOSION_DAMAGE : //
        topcon(UI_WARN_FMT_STR "You suffer explosion damage." UI_RESET_FMT);
        break;
      case THING_EVENT_ENERGY_DAMAGE : //
        topcon(UI_WARN_FMT_STR "You suffer dazzling damage." UI_RESET_FMT);
        break;
      case THING_EVENT_FIRE_DAMAGE : //
        topcon(UI_WARN_FMT_STR "You are burning." UI_RESET_FMT);
        break;
      case THING_EVENT_NONE :             [[fallthrough]];
      case THING_EVENT_OPEN :             [[fallthrough]];
      case THING_EVENT_USED :             [[fallthrough]];
      case THING_EVENT_EATEN :            [[fallthrough]];
      case THING_EVENT_LEVITATED :        [[fallthrough]];
      case THING_EVENT_LIFESPAN_EXPIRED : [[fallthrough]];
      case THING_EVENT_GAME_OVER :        [[fallthrough]];
      case THING_EVENT_CARRIED :          [[fallthrough]];
      case THING_EVENT_CARRIED_MERGED :   [[fallthrough]];
      case THING_EVENT_MELT :             [[fallthrough]];
      case THING_EVENT_USER_INITIATED :   [[fallthrough]];
      case THING_EVENT_SPAWNED :          [[fallthrough]];
      case THING_EVENT_ENUM_MAX : //
        ERR("unexpected event: %s", ThingEventType_to_string(e.event_type).c_str());
    }
  }
}

//
// The player has attacked
//
static void thing_damage_by_player(Gamep g, Levelsp v, Levelp l, Thingp it, ThingEvent &e)
{
  TRACE();

  auto *the_player = e.source;

  //
  // Popup for damage to monsters
  //
  if (thing_is_monst(it)) {
    std::string msg;
    if (e.crit) {
      msg = "CRIT! -" + std::to_string(e.damage);
    } else {
      msg = "-" + std::to_string(e.damage);
    }
    auto at = thing_at(g, v, l, it);
    game_popup_text_add(g, at.x, at.y, msg, WHITE);
  }

  if ((the_player != nullptr) && thing_is_loggable(it)) {
    auto the_thing_name_long = thing_name_long_the(g, v, l, it);
    auto The_thing_name_long = capitalize_first(the_thing_name_long);
    auto by_player           = thing_name_long(g, v, l, the_player);

    switch (e.event_type) {
      case THING_EVENT_THROWN : //
        topcon("You throw %s.", the_thing_name_long.c_str());
        break;
      case THING_EVENT_SHOVED : //
        topcon("You shove %s.", the_thing_name_long.c_str());
        break;
      case THING_EVENT_CRUSH_DAMAGE : //
        topcon("%s is crushed by %s.", The_thing_name_long.c_str(), by_player.c_str());
        break;
      case THING_EVENT_THROWN_DAMAGE : //
        topcon("You target and hit %s.", the_thing_name_long.c_str());
        break;
      case THING_EVENT_ENGULF_DAMAGE : //
        topcon("You digest %s.", the_thing_name_long.c_str());
        break;
      case THING_EVENT_POISON_DAMAGE : //
        topcon("You poison %s.", the_thing_name_long.c_str());
        break;
      case THING_EVENT_MELEE_DAMAGE :
        if (e.crit) {
          topcon("You crit %s.", the_thing_name_long.c_str());
        } else {
          topcon("You hit %s.", the_thing_name_long.c_str());
        }
        break;
      case THING_EVENT_WATER_DAMAGE : //
        topcon("%s suffers water damage from %s.", The_thing_name_long.c_str(), by_player.c_str());
        break;
      case THING_EVENT_EXPLOSION_DAMAGE : //
        topcon("You blast %s.", the_thing_name_long.c_str());
        break;
      case THING_EVENT_ENERGY_DAMAGE : //
        topcon("You dazzle %s.", the_thing_name_long.c_str());
        break;
      case THING_EVENT_FIRE_DAMAGE : //
        if (thing_is_burning(the_player)) {
          //
          // The player is burning.
          //
          if (thing_is_burning(it)) {
            topcon(UI_IMPORTANT_FMT_STR "Your burning body burns %s." UI_RESET_FMT, the_thing_name_long.c_str());
          } else {
            topcon(UI_IMPORTANT_FMT_STR "Your burning body sets fire to %s." UI_RESET_FMT, the_thing_name_long.c_str());
          }
        } else {
          //
          // The player is not burning.
          //
          if (thing_is_burning(it)) {
            topcon(UI_IMPORTANT_FMT_STR "You burn %s." UI_RESET_FMT, the_thing_name_long.c_str());
          } else {
            topcon(UI_IMPORTANT_FMT_STR "You set fire to %s." UI_RESET_FMT, the_thing_name_long.c_str());
          }
        }
        break;
      case THING_EVENT_NONE :             [[fallthrough]];
      case THING_EVENT_OPEN :             [[fallthrough]];
      case THING_EVENT_USED :             [[fallthrough]];
      case THING_EVENT_EATEN :            [[fallthrough]];
      case THING_EVENT_LEVITATED :        [[fallthrough]];
      case THING_EVENT_LIFESPAN_EXPIRED : [[fallthrough]];
      case THING_EVENT_FALL :             [[fallthrough]];
      case THING_EVENT_GAME_OVER :        [[fallthrough]];
      case THING_EVENT_CARRIED :          [[fallthrough]];
      case THING_EVENT_CARRIED_MERGED :   [[fallthrough]];
      case THING_EVENT_MELT :             [[fallthrough]];
      case THING_EVENT_USER_INITIATED :   [[fallthrough]];
      case THING_EVENT_SPAWNED :          [[fallthrough]];
      case THING_EVENT_ENUM_MAX : //
        ERR("unexpected event: %s", ThingEventType_to_string(e.event_type).c_str());
        break;
    }
  }
}

//
// Do not apply too much for one event
//
static void thing_damage_cap_for_this_event(Gamep g, Levelsp v, Levelp l, Thingp me, ThingEvent &e)
{
  TRACE();

  const auto h = thing_health_max(g, v, l, me);

  //
  // Limit the damage that can occur this event.
  //
  auto max_damage_this_time = h / 4;
  max_damage_this_time      = std::max(max_damage_this_time, 1);

  if (e.damage > max_damage_this_time) {
    auto old_d = e.damage;
    e.damage   = max_damage_this_time;
    THING_DBG(g, v, l, me, "%s: limit damage %d -> %d", to_string(g, v, l, e).c_str(), old_d, e.damage);
  }
}

//
// Do not apply too much damage per tick
//
static void thing_damage_cap_for_this_tick(Gamep g, Levelsp v, Levelp l, Thingp me, ThingEvent &e)
{
  TRACE();

  const auto h = thing_health_max(g, v, l, me);

  //
  // Limit the total damage that can occur per tick.
  //
  auto max_damage_per_tick = h / 3;
  max_damage_per_tick      = std::max(max_damage_per_tick, 1);

  auto d_total = thing_damage_this_tick_incr(g, v, l, me, e.damage);
  if (d_total > max_damage_per_tick) {
    auto old_d = e.damage;
    e.damage -= d_total - max_damage_per_tick;
    e.damage = std::max(e.damage, 0);
    THING_DBG(g, v, l, me, "%s: limit per tick damage %d -> %d", to_string(g, v, l, e).c_str(), old_d, e.damage);
  }
}

//
// Do not apply too much damage per tick
//
static void thing_damage_cap(Gamep g, Levelsp v, Levelp l, Thingp me, ThingEvent &e)
{
  TRACE();

  if (! thing_is_damage_capped(me)) {
    return;
  }

  thing_damage_cap_for_this_event(g, v, l, me, e);
  thing_damage_cap_for_this_tick(g, v, l, me, e);
}

//
// Apply a damage type to a thing
//
void thing_damage_apply(Gamep g, Levelsp v, Levelp l, Thingp me, ThingEvent &e)
{
  THING_DBG(g, v, l, me, "apply damage: %s", to_string(g, v, l, e).c_str());
  TRACE_INDENT();

  auto *tp = thing_tp(me);

  //
  // Indestructible?
  //
  if (thing_is_indestructible(me)) {
    THING_DBG(g, v, l, me, "%s: no damage as indestructible", to_string(g, v, l, e).c_str());
    return;
  }

  //
  // Already dead?
  //
  if (thing_is_dead(me)) {
    THING_DBG(g, v, l, me, "%s: no damage as already dead", to_string(g, v, l, e).c_str());
    if (thing_is_hit_when_dead(me) || thing_is_obs_when_dead(me)) {
      thing_is_hit_set(g, v, l, me, THING_HIT_FLASH_ANIM_MS);
    }

    if (! thing_is_hit_when_dead(me)) {
      return;
    }
  }

  //
  // Immune to this attack?
  //
  if (thing_is_immune_to(g, v, l, me, e.event_type)) {
    THING_DBG(g, v, l, me, "%s: no damage as immune", to_string(g, v, l, e).c_str());
    if (thing_is_player(me)) {
      topcon(UI_GOOD_FMT_STR "You take no damage from the heat." UI_RESET_FMT);
    }
    return;
  }

  //
  // Resistant to this attack?
  //
  if (thing_is_resistant_to(g, v, l, me, e.event_type)) {
    e.damage /= 2;
    THING_DBG(g, v, l, me, "%s: half damage as resistant", to_string(g, v, l, e).c_str());

    if (e.damage <= 0) {
      if (thing_is_player(me)) {
        topcon(UI_GOOD_FMT_STR "You take no damage from the heat." UI_RESET_FMT);
      }
      return;
    }

    if (thing_is_player(me)) {
      topcon(UI_GOOD_FMT_STR "You take half damage from the heat." UI_RESET_FMT);
    }
  }

  //
  // Critical attack?
  //
  if (e.crit) {
    e.damage *= 2;
  }

  //
  // Limit damage?
  //
  thing_damage_cap(g, v, l, me, e);

  //
  // No damage?
  //
  if (e.damage <= 0) {
    THING_DBG(g, v, l, me, "%s: no damage to apply", to_string(g, v, l, e).c_str());
    return;
  }

  //
  // Allow buffs to reduce damage
  //
  FOR_ALL_HOOKS(g, v, l, me, buff) { (void) thing_on_damage(g, v, l, buff, e); }

  //
  // Allow rings to reduce damage
  //
  FOR_ALL_ACTIVE_ITEMS(g, v, l, me, item) { (void) thing_on_damage(g, v, l, item, e); }

  if (e.damage <= 0) {
    THING_DBG(g, v, l, me, "%s: no damage to apply due to buffs", to_string(g, v, l, e).c_str());
    return;
  }

  //
  // Per thing callback
  //
  if (! thing_on_damage(g, v, l, me, e)) {
    THING_DBG(g, v, l, me, "%s: no damage due to callback", to_string(g, v, l, e).c_str());
    return;
  }

  //
  // Log the reason for attack?
  //
  THING_DBG(g, v, l, me, "%s: apply damage", to_string(g, v, l, e).c_str());
  TRACE_INDENT();

  if (thing_is_player(me)) {
    thing_damage_to_player(g, v, l, me, e);
  } else if ((e.source != nullptr) && thing_is_player(e.source)) {
    thing_damage_by_player(g, v, l, me, e);
  }

  //
  // Change the health
  //
  if (thing_health_decr(g, v, l, me, e.damage) <= 0) {
    //
    // Damage type specifics
    //
    switch (e.event_type) {
      case THING_EVENT_NONE :             [[fallthrough]];
      case THING_EVENT_ENERGY_DAMAGE :    [[fallthrough]];
      case THING_EVENT_EXPLOSION_DAMAGE : [[fallthrough]];
      case THING_EVENT_USED :             [[fallthrough]];
      case THING_EVENT_EATEN :            [[fallthrough]];
      case THING_EVENT_LEVITATED :        [[fallthrough]];
      case THING_EVENT_LIFESPAN_EXPIRED : [[fallthrough]];
      case THING_EVENT_FALL :             [[fallthrough]];
      case THING_EVENT_THROWN :           [[fallthrough]];
      case THING_EVENT_SHOVED :           [[fallthrough]];
      case THING_EVENT_CRUSH_DAMAGE :     [[fallthrough]];
      case THING_EVENT_THROWN_DAMAGE :    [[fallthrough]];
      case THING_EVENT_ENGULF_DAMAGE :    [[fallthrough]];
      case THING_EVENT_POISON_DAMAGE :    [[fallthrough]];
      case THING_EVENT_MELEE_DAMAGE :     break;
      case THING_EVENT_FIRE_DAMAGE :
        {
          auto temp_burn = tp_temperature_burns_at_get(tp);
          if ((temp_burn != 0) && (thing_temperature(me) > temp_burn)) {
            if (level_is_water_deep_bool(g, v, l, thing_at(g, v, l, me))) {
              //
              // No steam
              //
            } else if (level_is_water_shallow_bool(g, v, l, thing_at(g, v, l, me))) {
              if (! level_is_steam_bool(g, v, l, thing_at(g, v, l, me))) {
                THING_DBG(g, v, l, me, "spawn steam over water due to fire damage");
                TRACE_INDENT();
                (void) thing_spawn(g, v, l, tp_first(is_steam), me);
              }
            }

            thing_is_burning_set(g, v, l, me);
          }

          auto temp_melt = tp_temperature_melts_at_get(tp);
          if ((temp_melt != 0) && (thing_temperature(me) > temp_melt)) {
            thing_melt(g, v, l, me);
          }

          if (! level_is_fire_bool(g, v, l, thing_at(g, v, l, me))) {
            if (level_is_flammable_bool(g, v, l, thing_at(g, v, l, me))) {
              THING_DBG(g, v, l, me, "spawn flames as tile is flammable");
              TRACE_INDENT();
              (void) thing_spawn(g, v, l, tp_first(is_fire), me);
            } else if (thing_is_combustible(me)) {
              THING_DBG(g, v, l, me, "spawn flames as thing is combustible");
              TRACE_INDENT();
              (void) thing_spawn(g, v, l, tp_first(is_fire), me);
            }
          }
        }
        break;
      case THING_EVENT_WATER_DAMAGE :
        {
          if (! level_is_steam_bool(g, v, l, thing_at(g, v, l, me))) {
            THING_DBG(g, v, l, me, "spawn steam due to water damage");
            TRACE_INDENT();
            (void) thing_spawn(g, v, l, tp_first(is_steam), me);
          }
        }
        break;
      case THING_EVENT_OPEN :           [[fallthrough]];
      case THING_EVENT_CARRIED :        [[fallthrough]];
      case THING_EVENT_GAME_OVER :      [[fallthrough]];
      case THING_EVENT_CARRIED_MERGED : [[fallthrough]];
      case THING_EVENT_MELT :           [[fallthrough]];
      case THING_EVENT_USER_INITIATED : [[fallthrough]];
      case THING_EVENT_SPAWNED :        [[fallthrough]];
      case THING_EVENT_ENUM_MAX : //
        ERR("unexpected event: %s", ThingEventType_to_string(e.event_type).c_str());
        break;
    }

    THING_DBG(g, v, l, me, "dead due to damage");
    TRACE_INDENT();

    thing_dead(g, v, l, me, e);
  } else {
    //
    // Damage type specifics
    //
    switch (e.event_type) {
      case THING_EVENT_NONE :             [[fallthrough]];
      case THING_EVENT_USED :             [[fallthrough]];
      case THING_EVENT_EATEN :            [[fallthrough]];
      case THING_EVENT_LEVITATED :        [[fallthrough]];
      case THING_EVENT_LIFESPAN_EXPIRED : [[fallthrough]];
      case THING_EVENT_FALL :             [[fallthrough]];
      case THING_EVENT_THROWN :           [[fallthrough]];
      case THING_EVENT_SHOVED :           [[fallthrough]];
      case THING_EVENT_CRUSH_DAMAGE :     [[fallthrough]];
      case THING_EVENT_THROWN_DAMAGE :    [[fallthrough]];
      case THING_EVENT_ENGULF_DAMAGE :    [[fallthrough]];
      case THING_EVENT_POISON_DAMAGE :    [[fallthrough]];
      case THING_EVENT_MELEE_DAMAGE :     break;
      case THING_EVENT_FIRE_DAMAGE :
        //
        // Needed to allow things like projectiles to heat targets
        //
        if (e.source != nullptr) {
          if (! l->is_handling_temperature_changes) {
            l->is_handling_temperature_changes = true;
            level_thing_pair_temperature_handle(g, v, l, me, e.source, e);
            l->is_handling_temperature_changes = false;
          }
        }
        break;
      case THING_EVENT_ENERGY_DAMAGE :    [[fallthrough]];
      case THING_EVENT_WATER_DAMAGE :     [[fallthrough]];
      case THING_EVENT_EXPLOSION_DAMAGE : [[fallthrough]];
      case THING_EVENT_OPEN :             [[fallthrough]];
      case THING_EVENT_CARRIED :          [[fallthrough]];
      case THING_EVENT_GAME_OVER :        [[fallthrough]];
      case THING_EVENT_CARRIED_MERGED :   [[fallthrough]];
      case THING_EVENT_MELT :             [[fallthrough]];
      case THING_EVENT_SPAWNED :          [[fallthrough]];
      case THING_EVENT_USER_INITIATED :   break;
      case THING_EVENT_ENUM_MAX : //
        ERR("unexpected event: %s", ThingEventType_to_string(e.event_type).c_str());
        break;
    }
  }

  //
  // Flash the thing red
  //
  thing_is_hit_set(g, v, l, me, THING_HIT_FLASH_ANIM_MS);

  THING_DBG(g, v, l, me, "post damage");
}

[[nodiscard]] auto thing_damage_this_tick(Thingp t) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_damage_this_tick;
}

[[nodiscard]] auto thing_damage_this_tick_set(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }

  if (val > std::numeric_limits< decltype(t->_damage_this_tick) >::max()) {
    thing_err(g, v, l, t, "value overflow: %d", val);
    return 0;
  }

  return t->_damage_this_tick = val;
}

[[nodiscard]] auto thing_damage_this_tick_incr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_damage_this_tick += val;
}

[[nodiscard]] auto thing_damage_this_tick_decr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }

  if (static_cast< int >(t->_damage_this_tick) - val <= 0) {
    return t->_damage_this_tick = 0;
  }

  return t->_damage_this_tick -= val;
}

[[nodiscard]] auto thing_is_damage_capped(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_damage_capped) != 0;
}
