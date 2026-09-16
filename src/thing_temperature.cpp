//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_dice_rolls.hpp"
#include "my_level.hpp"
#include "my_main.hpp"
#include "my_thing.hpp"
#include "my_thing_inlines.hpp"
#include "my_tp.hpp"
#include "my_tp_inlines.hpp"
#include "my_types.hpp"
#include <algorithm>
#include <limits>
#include <utility>

//
// First step is to mark things as burning and change temperatures
//
void thing_temperature_handle(Gamep g, Levelsp v, Levelp l, Thingp source, Thingp me, int n)
{
  TRACE();

  //
  // If not burnt already, burn it if over the threshold temperature.
  //
  auto *tp   = thing_tp(me);
  auto  Tb   = tp_temperature_burns_at_get(tp);
  auto  Tm   = tp_temperature_melts_at_get(tp);
  auto  Tmax = std::max(Tb, Tm);

  //
  // Don't allow things to become superheated
  //
  n = std::min(Tmax * 2, n);

  //
  // Nor hotter than the source
  //
  n = std::min(thing_temperature(source), n);

  THING_DBG(g, v, l, me, "temperature handle: %d degrees (Tmax %d)", n, Tmax * 2);

  if ((Tb != 0) && (n >= Tb)) {
    if (thing_is_steam(source) || thing_is_water_deep(source) || thing_is_water_shallow(source)) {
      //
      // You don't continue to burn with steam
      //
    } else if (! level_is_fire_bool(g, v, l, thing_at(g, v, l, me))) {
      //
      // No fire here. Set it on fire.
      //
      if (thing_is_flammable(me) || thing_is_combustible(me)) {
        //
        // But only if combustible.
        //
        THING_DBG(g, v, l, me, "set on fire");
        (void) thing_spawn(g, v, l, tp_random(g, v, l, is_fire), me);
        thing_is_burning_set(g, v, l, me);
      }
    } else {
      //
      // Fire is here already.
      //
      thing_is_burning_set(g, v, l, me);
    }
  }

  //
  // If not meltt already, melt it if over the threshold temperature.
  //
  if ((Tm != 0) && (n >= Tm)) {
    THING_DBG(g, v, l, me, "melt");
    thing_melt(g, v, l, me);
  }

  (void) thing_temperature_set(g, v, l, me, n);
}

static void thing_temperature_damage_apply(Gamep g, Levelsp v, Levelp l, Thingp source, Thingp me, int n, ThingEvent e)
{
  TRACE();

  auto     *tp        = thing_tp(me);
  int       damage    = d6();
  int const damage_at = tp_temperature_damage_at_get(tp);

  if (n > damage_at * 2) {
    damage *= 2;
  }

  if (thing_is_flammable(me) || thing_is_combustible(me)) {
    damage *= 2;
  }

  if (level_is_lava_bool(g, v, l, thing_at(g, v, l, me))) {
    damage *= 2;
  }

  if (e.damage != 0) {
    e.nested_damage = true;
  }

  e.reason     = "temperature damage";
  e.event_type = THING_EVENT_FIRE_DAMAGE;
  e.damage     = damage;
  if (source != nullptr) {
    e.source = source;
  }

  e.temperature_damage = true;

  if (thing_is_steam(source)) {
    e.reason = "by steam";
  } else if (level_is_lava_bool(g, v, l, thing_at(g, v, l, me))) {
    e.reason = "by lava";
  } else if (level_is_fire_bool(g, v, l, thing_at(g, v, l, me))) {
    e.reason = "by fire";
  } else {
    e.reason = "by heat damage";
  }

  THING_DBG(g, v, l, me, "apply temperature damage");
  TRACE_INDENT();

  thing_damage_apply(g, v, l, me, e);
}

//
// Next step is to apply burning damage
//
void thing_temperature_damage_handle(Gamep g, Levelsp v, Levelp l, Thingp source, Thingp me, int n, ThingEvent e)
{
  TRACE();

  auto *tp = thing_tp(me);

  //
  // Pre burning heat damage
  //
  auto T = tp_temperature_damage_at_get(tp);
  if ((T != 0) && (n > T)) {
    thing_temperature_damage_apply(g, v, l, source, me, n, std::move(e));
    if (thing_is_dead(me)) {
      if (! thing_is_hit_when_dead(me)) {
        return;
      }
    }
  }
}

void tp_temperature_init(Tpp tp)
{
  TRACE();

  bool heat_exchange_set {};
  if (tp_is_stone(tp) || tp_is_lava(tp)) {
    tp_temperature_thermal_conductivity_set(tp, THERMAL_CONDUCTIVITY_STONE);
    tp_temperature_heat_capacity_set(tp, HEAT_CAPACITY_STONE);
    heat_exchange_set = true;
  }
  if (tp_is_wall(tp)) {
    tp_temperature_thermal_conductivity_set(tp, THERMAL_CONDUCTIVITY_WALL);
    tp_temperature_heat_capacity_set(tp, HEAT_CAPACITY_WALL);
    heat_exchange_set = true;
  }
  if (tp_is_glass(tp)) {
    tp_temperature_thermal_conductivity_set(tp, THERMAL_CONDUCTIVITY_GLASS);
    tp_temperature_heat_capacity_set(tp, HEAT_CAPACITY_GLASS);
    heat_exchange_set = true;
  }
  if (tp_is_gold(tp)) {
    tp_temperature_thermal_conductivity_set(tp, THERMAL_CONDUCTIVITY_GOLD);
    tp_temperature_heat_capacity_set(tp, HEAT_CAPACITY_GOLD);
    heat_exchange_set = true;
  }
  if (tp_is_metal(tp)) {
    tp_temperature_thermal_conductivity_set(tp, THERMAL_CONDUCTIVITY_STEEL);
    tp_temperature_heat_capacity_set(tp, HEAT_CAPACITY_STEEL);
    heat_exchange_set = true;
  }
  if (tp_is_water_shallow(tp) || tp_is_water_deep(tp)) {
    tp_temperature_thermal_conductivity_set(tp, THERMAL_CONDUCTIVITY_WATER);
    tp_temperature_heat_capacity_set(tp, HEAT_CAPACITY_WATER);
    heat_exchange_set = true;
  }
  if (tp_is_gaseous(tp) || tp_is_beam_weapon(tp)) {
    tp_temperature_thermal_conductivity_set(tp, THERMAL_CONDUCTIVITY_GAS);
    tp_temperature_heat_capacity_set(tp, HEAT_CAPACITY_GAS);
    heat_exchange_set = true;
  }
  if (tp_is_wood(tp) || tp_is_plant(tp) || tp_is_mob(tp)) {
    tp_temperature_thermal_conductivity_set(tp, THERMAL_CONDUCTIVITY_HIGH);
    tp_temperature_heat_capacity_set(tp, HEAT_CAPACITY_WOOD);
    heat_exchange_set = true;
  }
  if (tp_is_flesh(tp) || tp_is_leather(tp)) {
    tp_temperature_thermal_conductivity_set(tp, THERMAL_CONDUCTIVITY_HIGH);
    tp_temperature_heat_capacity_set(tp, HEAT_CAPACITY_FLESH);
    heat_exchange_set = true;
  }
  if (tp_is_spiderweb(tp) || tp_is_chocolate(tp) || tp_is_insectoid(tp) || tp_is_edible(tp)) {
    tp_temperature_thermal_conductivity_set(tp, THERMAL_CONDUCTIVITY_UNKNOWN);
    tp_temperature_heat_capacity_set(tp, HEAT_CAPACITY_UNKNOWN);
    heat_exchange_set = true;
  }
  if (tp_is_slime(tp)) {
    tp_temperature_thermal_conductivity_set(tp, THERMAL_CONDUCTIVITY_GEL);
    tp_temperature_heat_capacity_set(tp, HEAT_CAPACITY_GEL);
    heat_exchange_set = true;
  }
  if (tp_is_physics_temperature(tp)) {
    if (! heat_exchange_set) {
      tp_err(tp, "tp has heat physics set but no conductivity type set");
    }
  }
}

[[nodiscard]] auto thing_temperature(Thingp me) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return me->_temperature;
}

[[nodiscard]] auto thing_temperature_set(Gamep g, Levelsp v, Levelp l, Thingp me, int val) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    ERR("no thing pointer");
    return 0;
  }

  if (val > std::numeric_limits< decltype(me->_temperature) >::max()) {
    thing_err(g, v, l, me, "value overflow: %d", val);
    return 0;
  }

  if (! thing_is_physics_temperature(me)) {
    return 0;
  }

  //
  // Don't keep on heating up forever!
  //
  auto *tp    = thing_tp(me);
  auto  limit = std::max(tp_temperature_burns_at_get(tp), tp_temperature_melts_at_get(tp));
  if ((limit != 0) && (val > limit)) {
    val = limit;
  }

  IF_DEBUG2
  { //
    THING_DBG(g, v, l, me, "temperature set to %u degrees", val);
  }

  return me->_temperature = val;
}

[[nodiscard]] auto thing_temperature_incr(Gamep g, Levelsp v, Levelp l, Thingp me, int val) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return thing_temperature_set(g, v, l, me, me->_temperature + val);
}

[[nodiscard]] auto thing_temperature_decr(Gamep g, Levelsp v, Levelp l, Thingp me, int val) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return thing_temperature_set(g, v, l, me, me->_temperature - val);
}
