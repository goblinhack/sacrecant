//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_game_popups.hpp"
#include "my_globals.hpp"
#include "my_main.hpp"
#include "my_thing_callbacks.hpp"
#include "my_thing_inlines.hpp"
#include "my_ui.hpp"

#include <algorithm>

//
// The player has been attacked
//
static void thing_damage_to_player(Gamep g, Levelsp v, Levelp l, Thingp me, ThingEvent &e)
{
  TRACE();

  auto *it = e.source;

  std::string const msg = "-" + std::to_string(e.damage);
  auto              at  = thing_at(me);
  game_popup_text_add(g, at.x, at.y, msg, RED);

  if (it != nullptr) {
    auto by_the_thing = thing_name_long_the(g, v, l, it);

    switch (e.event_type) {
      case THING_EVENT_SHOVED : //
        topcon(UI_WARNING_FMT_STR "You are shoved by %s." UI_RESET_FMT, by_the_thing.c_str());
        break;
      case THING_EVENT_CRUSH : //
        topcon(UI_WARNING_FMT_STR "You are crushed by %s." UI_RESET_FMT, by_the_thing.c_str());
        break;
      case THING_EVENT_MELEE_DAMAGE : //
        topcon(UI_WARNING_FMT_STR "You are hit by %s." UI_RESET_FMT, by_the_thing.c_str());
        break;
      case THING_EVENT_HEAT_DAMAGE : //
        topcon(UI_WARNING_FMT_STR "You suffer heat damage from %s." UI_RESET_FMT, by_the_thing.c_str());
        break;
      case THING_EVENT_WATER_DAMAGE : //
        topcon(UI_WARNING_FMT_STR "You suffer water damage from %s." UI_RESET_FMT, by_the_thing.c_str());
        break;
      case THING_EVENT_EXPLOSION_DAMAGE : //
        topcon(UI_WARNING_FMT_STR "You suffer blast damage from %s." UI_RESET_FMT, by_the_thing.c_str());
        break;
      case THING_EVENT_FIRE_DAMAGE :
        if (thing_is_lava(it)) {
          topcon(UI_WARNING_FMT_STR "You are burning in lava!" UI_RESET_FMT);
        } else if (thing_is_fire(it)) {
          topcon(UI_WARNING_FMT_STR "The flames wrap around you!" UI_RESET_FMT);
        } else if (thing_is_water(it)) {
          topcon(UI_WARNING_FMT_STR "You are burnt by scalding %s." UI_RESET_FMT, by_the_thing.c_str());
        } else if (thing_is_steam(it)) {
          topcon(UI_WARNING_FMT_STR "You scalded by %s." UI_RESET_FMT, by_the_thing.c_str());
        } else {
          topcon(UI_WARNING_FMT_STR "You are burnt by %s." UI_RESET_FMT, by_the_thing.c_str());
        }
        break;
      case THING_EVENT_NONE :             //
      case THING_EVENT_OPEN :             //
      case THING_EVENT_LIFESPAN_EXPIRED : //
      case THING_EVENT_FALL :             //
      case THING_EVENT_CARRIED :          //
      case THING_EVENT_CARRIED_MERGED :   //
      case THING_EVENT_MELT :             //
      case THING_EVENT_ENUM_MAX :         //
        ERR("unexpected event: %s", ThingEventType_to_string(e.event_type).c_str());
        break;
    }
  } else {
    switch (e.event_type) {
      case THING_EVENT_FALL : //
        topcon(UI_WARNING_FMT_STR "You took %d damage from falling." UI_RESET_FMT, e.damage);
        break;
      case THING_EVENT_SHOVED : //
        topcon(UI_WARNING_FMT_STR "You are shoved." UI_RESET_FMT);
        break;
      case THING_EVENT_CRUSH : //
        topcon(UI_WARNING_FMT_STR "You are crushed." UI_RESET_FMT);
        break;
      case THING_EVENT_MELEE_DAMAGE : //
        topcon(UI_WARNING_FMT_STR "You are hit." UI_RESET_FMT);
        break;
      case THING_EVENT_HEAT_DAMAGE : //
        topcon(UI_WARNING_FMT_STR "You suffer heat damage." UI_RESET_FMT);
        break;
      case THING_EVENT_WATER_DAMAGE : //
        topcon(UI_WARNING_FMT_STR "You suffer water damage." UI_RESET_FMT);
        break;
      case THING_EVENT_EXPLOSION_DAMAGE : //
        topcon(UI_WARNING_FMT_STR "You suffer blast damage." UI_RESET_FMT);
        break;
      case THING_EVENT_FIRE_DAMAGE : //
        topcon(UI_WARNING_FMT_STR "You are burnt." UI_RESET_FMT);
        break;
      case THING_EVENT_NONE :             //
      case THING_EVENT_OPEN :             //
      case THING_EVENT_LIFESPAN_EXPIRED : //
      case THING_EVENT_CARRIED :          //
      case THING_EVENT_CARRIED_MERGED :   //
      case THING_EVENT_MELT :             //
      case THING_EVENT_ENUM_MAX :         //
        ERR("unexpected event: %s", ThingEventType_to_string(e.event_type).c_str());
    }
  }
}

//
// The player has attacked
//
static void thing_damage_by_player(Gamep g, Levelsp v, Levelp l, Thingp me, ThingEvent &e)
{
  TRACE();
  auto *the_player = e.source;

  std::string const msg = "-" + std::to_string(e.damage);
  auto              at  = thing_at(me);
  game_popup_text_add(g, at.x, at.y, msg, WHITE);

  if ((the_player != nullptr) && thing_is_loggable(me)) {
    auto the_thing_name_long  = capitalize_first(thing_name_long_the(g, v, l, me));
    auto the_thing_name_short = capitalize_first(thing_name_short_the(g, v, l, me));
    auto by_player            = thing_name_long(g, v, l, the_player);

    switch (e.event_type) {
      case THING_EVENT_SHOVED : //
        topcon("%s is shoved by %s.", the_thing_name_long.c_str(), by_player.c_str());
        break;
      case THING_EVENT_CRUSH : //
        topcon("%s is crushed by %s.", the_thing_name_long.c_str(), by_player.c_str());
        break;
      case THING_EVENT_MELEE_DAMAGE : //
        topcon("%s is hit by %s.", the_thing_name_long.c_str(), by_player.c_str());
        break;
      case THING_EVENT_HEAT_DAMAGE : //
        topcon("%s suffers heat damage from %s.", the_thing_name_long.c_str(), by_player.c_str());
        break;
      case THING_EVENT_WATER_DAMAGE : //
        topcon("%s suffers water damage from %s.", the_thing_name_long.c_str(), by_player.c_str());
        break;
      case THING_EVENT_EXPLOSION_DAMAGE : //
        topcon("%s suffers blast damage from %s.", the_thing_name_long.c_str(), by_player.c_str());
        break;
      case THING_EVENT_FIRE_DAMAGE : //
        if (thing_is_burning(the_player)) {
          //
          // The player is burning.
          //
          if (thing_is_burning(me)) {
            topcon("Your burning body burns %s", the_thing_name_short.c_str());
          } else {
            topcon("Your burning body sets fire to %s", the_thing_name_short.c_str());
          }
        } else {
          //
          // The player is not burning.
          //
          if (thing_is_burning(me)) {
            topcon("You burn %s", the_thing_name_short.c_str());
          } else {
            topcon("You set fire to %s", the_thing_name_short.c_str());
          }
        }
        break;
      case THING_EVENT_NONE :             //
      case THING_EVENT_OPEN :             //
      case THING_EVENT_LIFESPAN_EXPIRED : //
      case THING_EVENT_FALL :             //
      case THING_EVENT_CARRIED :          //
      case THING_EVENT_CARRIED_MERGED :   //
      case THING_EVENT_MELT :             //
      case THING_EVENT_ENUM_MAX :         //
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

  const auto h = thing_health_max(me);

  //
  // Limit the damage that can occur this event.
  //
  auto max_damage_this_time = h / 4;
  max_damage_this_time      = std::max(max_damage_this_time, 1);

  if (e.damage > max_damage_this_time) {
    auto old_d = e.damage;
    e.damage   = max_damage_this_time;
    THING_DBG(me, "%s: limit damage %d -> %d", to_string(g, v, l, e).c_str(), old_d, e.damage);
  }
}

//
// Do not apply too much damage per tick
//
static void thing_damage_cap_for_this_tick(Gamep g, Levelsp v, Levelp l, Thingp me, ThingEvent &e)
{
  TRACE();

  const auto h = thing_health_max(me);

  //
  // Limit the total damage that can occur per tick.
  //
  auto max_damage_per_tick = h / 3;
  max_damage_per_tick      = std::max(max_damage_per_tick, 1);

  auto d_total = thing_damage_this_tick_incr(g, v, l, me, e.damage);
  if (d_total > max_damage_per_tick) {
    auto old_d = e.damage;
    e.damage -= d_total - max_damage_per_tick;
    THING_DBG(me, "%s: limit per tick damage %d -> %d", to_string(g, v, l, e).c_str(), old_d, e.damage);
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
void thing_damage(Gamep g, Levelsp v, Levelp l, Thingp me, ThingEvent &e)
{
  TRACE();

  auto *tp = thing_tp(me);

  //
  // Indestructible?
  //
  if (thing_is_indestructible(me)) {
    THING_DBG(me, "%s: no damage as indestructible", to_string(g, v, l, e).c_str());
    return;
  }

  //
  // Already dead?
  //
  if (thing_is_dead(me)) {
    THING_DBG(me, "%s: no damage as already dead", to_string(g, v, l, e).c_str());
    return;
  }

  //
  // Immune to this attack?
  //
  if (thing_is_immune_to(me, e.event_type)) {
    THING_DBG(me, "%s: no damage as immune", to_string(g, v, l, e).c_str());
    return;
  }

  //
  // Limit damage?
  //
  thing_damage_cap(g, v, l, me, e);

  //
  // Per thing callback
  //
  if (! thing_on_damage(g, v, l, me, e)) {
    THING_DBG(me, "%s: no damage due to callback", to_string(g, v, l, e).c_str());
    return;
  }

  //
  // No damage?
  //
  if (e.damage <= 0) {
    THING_DBG(me, "%s: no damage to apply", to_string(g, v, l, e).c_str());
    return;
  }

  //
  // Log the reason for attack?
  //
  THING_DBG(me, "%s: apply damage", to_string(g, v, l, e).c_str());
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
      case THING_EVENT_NONE : //
        break;
      case THING_EVENT_LIFESPAN_EXPIRED : //
        break;
      case THING_EVENT_FALL : //
        break;
      case THING_EVENT_SHOVED : //
        break;
      case THING_EVENT_CRUSH : //
        break;
      case THING_EVENT_MELEE_DAMAGE : //
        break;
      case THING_EVENT_HEAT_DAMAGE : //
        {
          auto temp_burn = tp_temperature_burns_at_get(tp);
          if ((temp_burn != 0) && (thing_temperature(me) > temp_burn)) {
            if (level_is_water(g, v, l, thing_at(me)) != nullptr) {
              if (level_is_steam(g, v, l, thing_at(me)) == nullptr) {
                THING_DBG(me, "spawn steam over water due to fire damage");
                (void) thing_spawn(g, v, l, tp_first(is_steam), me);
              }
            }
          }
        }

        {
          auto temp_melt = tp_temperature_melts_at_get(tp);
          if ((temp_melt != 0) && (thing_temperature(me) > temp_melt)) {
            thing_melt(g, v, l, me);
          }
        }
        break;
      case THING_EVENT_FIRE_DAMAGE :
        if (level_is_fire(g, v, l, thing_at(me)) == nullptr) {
          if (level_is_combustible(g, v, l, thing_at(me)) != nullptr) {
            THING_DBG(me, "spawn flames due to fire damage");
            (void) thing_spawn(g, v, l, tp_first(is_fire), me);
          }
        }

        if (level_is_water(g, v, l, thing_at(me)) != nullptr) {
          if (level_is_steam(g, v, l, thing_at(me)) == nullptr) {
            THING_DBG(me, "spawn steam over water due to fire damage");
            (void) thing_spawn(g, v, l, tp_first(is_steam), me);
          }
        }

        thing_is_burning_set(g, v, l, me);
        break;
      case THING_EVENT_WATER_DAMAGE : //
        if (level_is_steam(g, v, l, thing_at(me)) == nullptr) {
          THING_DBG(me, "spawn steam due to water damage");
          (void) thing_spawn(g, v, l, tp_first(is_steam), me);
        }
        break;
      case THING_EVENT_EXPLOSION_DAMAGE : //
        break;
      case THING_EVENT_OPEN :           //
      case THING_EVENT_CARRIED :        //
      case THING_EVENT_CARRIED_MERGED : //
      case THING_EVENT_MELT :           //
      case THING_EVENT_ENUM_MAX :       //
        ERR("unexpected event: %s", ThingEventType_to_string(e.event_type).c_str());
        break;
    }

    THING_DBG(me, "dead due to damage");
    TRACE_INDENT();

    thing_dead(g, v, l, me, e);
  } else {
    //
    // Damage type specifics
    //
    switch (e.event_type) {
      case THING_EVENT_NONE : //
        break;
      case THING_EVENT_LIFESPAN_EXPIRED : //
        break;
      case THING_EVENT_FALL : //
        break;
      case THING_EVENT_SHOVED : //
        break;
      case THING_EVENT_CRUSH : //
        break;
      case THING_EVENT_MELEE_DAMAGE : //
        break;
      case THING_EVENT_HEAT_DAMAGE : //
      case THING_EVENT_FIRE_DAMAGE :
        //
        // Needed to allow things like projectiles to heat targets
        //
        if (e.source != nullptr) {
          if (! l->is_handling_temperature_changes) {
            l->is_handling_temperature_changes = true;
            level_thing_pair_temperature_handle(g, v, l, me, e.source);
            l->is_handling_temperature_changes = false;
          }
        }
        break;
      case THING_EVENT_WATER_DAMAGE :     //
      case THING_EVENT_EXPLOSION_DAMAGE : //
      case THING_EVENT_OPEN :             //
      case THING_EVENT_CARRIED :          //
      case THING_EVENT_CARRIED_MERGED :   //
      case THING_EVENT_MELT :             //
        break;
      case THING_EVENT_ENUM_MAX : //
        ERR("unexpected event: %s", ThingEventType_to_string(e.event_type).c_str());
        break;
    }
  }

  THING_DBG(me, "post damage");
}
