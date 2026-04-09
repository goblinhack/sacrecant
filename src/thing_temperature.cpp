//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_dice_rolls.hpp"
#include "my_thing_inlines.hpp"

//
// First step is to mark things as burning and change temperatures
//
void thing_temperature_handle(Gamep g, Levelsp v, Levelp l, Thingp source, Thingp t, int n)
{
  TRACE();

  THING_DBG(t, "temperature handle: %d", n);

  //
  // If not burnt already, burn it if over the threshold temperature.
  //
  auto *tp   = thing_tp(t);
  auto  Tb   = tp_temperature_burns_at_get(tp);
  auto  Tm   = tp_temperature_melts_at_get(tp);
  auto  Tmax = std::max(Tb, Tm);

  //
  // Don't allow things to become superheated
  //
  n = std::min(Tmax * 2, n);

  if ((Tb != 0) && (n >= Tb)) {
    if (thing_is_steam(source) || thing_is_water(source)) {
      //
      // You don't continue to burn with steam
      //
    } else if (level_is_fire(g, v, l, thing_at(t)) == nullptr) {
      //
      // No fire here. Set it on fire.
      //
      if (thing_is_combustible(t)) {
        //
        // But only if combustible.
        //
        THING_DBG(t, "set on fire");
        (void) thing_spawn(g, v, l, tp_random(g, v, l, is_fire), t);
        thing_is_burning_set(g, v, l, t);
      }
    } else {
      //
      // Fire is here already.
      //
      thing_is_burning_set(g, v, l, t);
    }
  }

  //
  // If not meltt already, melt it if over the threshold temperature.
  //
  if ((Tm != 0) && (n >= Tm)) {
    THING_DBG(t, "melt");
    thing_melt(g, v, l, t);
  }

  (void) thing_temperature_set(g, v, l, t, n);
}

static void thing_temperature_damage_apply(Gamep g, Levelsp v, Levelp l, Thingp source, Thingp t, int n)
{
  TRACE();

  auto     *tp        = thing_tp(t);
  int       damage    = d6();
  int const damage_at = tp_temperature_damage_at_get(tp);

  if (n > damage_at * 2) {
    damage *= 2;
  }

  if (thing_is_combustible(t)) {
    damage *= 2;
  }

  if (level_is_lava(g, v, l, thing_at(t)) != nullptr) {
    damage *= 2;
  }

  ThingEvent e {
      .reason = {},     //
      .damage = damage, //
      .source = source, //
  };

  if (thing_is_steam(source)) {
    e.event_type = THING_EVENT_HEAT_DAMAGE;
    e.reason     = "by steam";
  } else if (level_is_lava(g, v, l, thing_at(t)) != nullptr) {
    e.event_type = THING_EVENT_FIRE_DAMAGE;
    e.reason     = "by lava";
  } else if (level_is_fire(g, v, l, thing_at(t)) != nullptr) {
    e.event_type = THING_EVENT_FIRE_DAMAGE;
    e.reason     = "by fire";
  } else {
    e.event_type = THING_EVENT_HEAT_DAMAGE;
    e.reason     = "by heat damage";
  }

  thing_damage(g, v, l, t, e);
}

//
// Next step is to apply burning damage
//
void thing_temperature_damage_handle(Gamep g, Levelsp v, Levelp l, Thingp source, Thingp t, int n)
{
  TRACE();

  auto *tp = thing_tp(t);

  //
  // Pre burning heat damage
  //
  auto T = tp_temperature_damage_at_get(tp);
  if ((T != 0) && (n > T)) {
    thing_temperature_damage_apply(g, v, l, source, t, n);
    if (thing_is_dead(t)) {
      return;
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
  if (tp_is_water(tp) || tp_is_deep_water(tp)) {
    tp_temperature_thermal_conductivity_set(tp, THERMAL_CONDUCTIVITY_WATER);
    tp_temperature_heat_capacity_set(tp, HEAT_CAPACITY_WATER);
    heat_exchange_set = true;
  }
  if (tp_is_gaseous(tp)) {
    tp_temperature_thermal_conductivity_set(tp, THERMAL_CONDUCTIVITY_GAS);
    tp_temperature_heat_capacity_set(tp, HEAT_CAPACITY_GAS);
    heat_exchange_set = true;
  }
  if (tp_is_wood(tp) || tp_is_plant(tp) || tp_is_mob(tp)) {
    tp_temperature_thermal_conductivity_set(tp, THERMAL_CONDUCTIVITY_HIGH);
    tp_temperature_heat_capacity_set(tp, HEAT_CAPACITY_WOOD);
    heat_exchange_set = true;
  }
  if (tp_is_flesh(tp)) {
    tp_temperature_thermal_conductivity_set(tp, THERMAL_CONDUCTIVITY_HIGH);
    tp_temperature_heat_capacity_set(tp, HEAT_CAPACITY_FLESH);
    heat_exchange_set = true;
  }
  if (tp_is_insectoid(tp)) {
    tp_temperature_thermal_conductivity_set(tp, THERMAL_CONDUCTIVITY_HIGH);
    tp_temperature_heat_capacity_set(tp, HEAT_CAPACITY_FLESH);
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
