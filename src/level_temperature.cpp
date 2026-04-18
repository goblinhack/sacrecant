//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_level.hpp"
#include "my_level_inlines.hpp"
#include "my_main.hpp"
#include "my_thing_inlines.hpp"

#include <algorithm>
#include <cmath>
#include <set>
#include <vector>

//
// Allow things to return to the initial temperature
//
void level_tick_begin_temperature(Gamep g, Levelsp v, Levelp l)
{
  TRACE();

  int x = 0;
  int y = 0;

  FOR_ALL_MAP_POINTS(g, v, l, x, y)
  {
    std::vector< Thingp > things;

    //
    // Collect all things at this point into a vector
    //
    bpoint p(x, y);
    FOR_ALL_THINGS_AT_UNSAFE(g, v, l, t, p)
    {
      if (! thing_is_physics_temperature(t)) {
        continue;
      }

      //
      // Ignore burnt grass for example
      //
      if (thing_is_dead(t)) {
        continue;
      }

      things.push_back(t);
    }

    //
    // This is emulating returning to ambient temperature
    //
    for (auto *t : things) {
      //
      // It could be dead now.
      //
      if (thing_is_dead(t)) {
        continue;
      }

      //
      // Only cool down if not being actively heated.
      //
      if (v->tick - t->tick_temperature_last_change < 5) {
        continue;
      }

      auto       *tp            = thing_tp(t);
      float const Ta            = thing_temperature(t);
      float const To            = tp_temperature_initial_get(thing_tp(t));
      float const heat_capacity = tp_temperature_heat_capacity_get(tp);
      float const diff          = Ta - To;
      float const ndiff         = diff * (1.0F - ((HEAT_CAPACITY_MAX - heat_capacity) / HEAT_CAPACITY_MAX));
      int const   Tn            = static_cast< int >(Ta) - static_cast< int >(ndiff);

      //
      // No ice yet
      //
      if (Tn > Ta) {
        continue;
      }

      //
      // No need to handle return to temperature
      //
      if (Tn != Ta) {
        THING_DBG(t, "temperature return Ta %f To %f -> %d degrees", Ta, To, Tn);
        (void) thing_temperature_set(g, v, l, t, Tn);
      }
    }
  }
}

//
// Q is the heat transfer                           watts
// K is the thermal conductivity of the material
// A is the cross-sectional area in                 meters squared
// Thot Tcold the temperatures on either side       degrees
// d is the thickness of the material in meters     meters
//
// For example, if you have a wall with a thermal conductivity of 70 W/m·K, an area of 1 m²,
// a temperature difference of 50°C, and a thickness of 0.05m, the heat transfer would be
// calculated as follows:
//
// Q = K * A * (Thot - Tcold)
//     ----------------------
//              d
//
// Now for the change in temperature:
//
// dT is the change in temperature
// m  is the mass in g
// c  is the heat capacity in units per g
//
// dT = q / (c * m)
// Tfinal = Tinitial + (Q / (m * c))
//
static void thing_heat_exchange(Levelsp v, Thingp a, Thingp b, int &finalT)
{
  TRACE();

  auto       *tpA = thing_tp(a);
  float const Ta  = thing_temperature(a);
  float const Tb  = thing_temperature(b);

  finalT = static_cast< int >(Ta);

  //
  // Projectiles only heat up things they hit.
  //
  // Steam over water is handles as a special case.
  //
  // If we do turn on collision detection, for say water, then the water ends up boiling
  // off too soon. So the hack is just to ignore this case.
  //
  if (thing_is_projectile(b)) {
    if (! thing_is_collision_detection_enabled(a)) {
      return;
    }
  }

  if (Ta == Tb) {
    return;
  }

  if (a->tick_temperature == v->tick) {
    return;
  }

  float const K  = tp_temperature_thermal_conductivity_get(tpA);
  float const c  = tp_temperature_heat_capacity_get(tpA);
  float const A  = 1.0F;
  float const dT = Tb - Ta;
  float const d  = 0.01F;
  float const Q  = (K * A * dT) / d;
  float       m  = thing_weight(a);

  //
  // We don't have ice yet
  //
  if (Q < 0) {
    return;
  }

  //
  // Fire has no weight, so give it some so the equations below average the temperatures.
  //
  // Take care not to give too much weight or you end up with steam able to heat up a
  // tile of water, which seems wrong.
  //
  if ((static_cast< int >(m)) == 0) {
    if (thing_is_gaseous(a) || thing_is_projectile(a)) {
      m = 1;
    }
    if ((static_cast< int >(m)) == 0) {
      return;
    }
  }

  float const final_dT = (Q / (m * c));
  finalT               = static_cast< int >(ceilf((Ta) + final_dT));

  if (compiler_unused) {
    THING_DBG(a, "b");
    THING_DBG(b, "b");
  }
  THING_DBG(a, "Ta %f Tb %f dT %f K %f m %f c %f Q %f final dT %f => %d", Ta, Tb, dT, K, m, c, Q, final_dT, finalT);
}

void level_thing_pair_temperature_handle(Gamep g, Levelsp v, Levelp l, Thingp a, Thingp b)
{
  TRACE();

  //
  // It could be dead now.
  //
  if (thing_is_dead(a) || thing_is_dead(b)) {
    return;
  }

  int const Ta = thing_temperature(a);
  int const Tb = thing_temperature(b);

  if (compiler_unused) {
    THING_DBG(a, "a Ta %d", Ta);
    THING_DBG(b, "b Tb %d", Tb);
  }

  //
  // The new temperatures
  //
  int Na = 0;
  thing_heat_exchange(v, a, b, Na);

  int Nb = 0;
  thing_heat_exchange(v, b, a, Nb);

  //
  // First step is to mark things as burning and change temperatures
  //
  if (Ta != Na) {
    a->tick_temperature_last_change = v->tick;
    THING_DBG(a, "temperature change (a) %d -> %d degrees", Ta, Na);
    thing_temperature_handle(g, v, l, b, a, Na);
  }

  if (Tb != Nb) {
    b->tick_temperature_last_change = v->tick;
    THING_DBG(b, "temperature change (b) %d -> %d degrees", Tb, Nb);
    thing_temperature_handle(g, v, l, a, b, Nb);
  }

  //
  // Next step is to apply burning damage if the temperature increases
  //
  // If it decreases (e.g. cooling in water) then no damage.
  //
  if (Na > Ta) {
    thing_temperature_damage_handle(g, v, l, b /* source */, a, Na);
  }

  if (Nb > Tb) {
    thing_temperature_damage_handle(g, v, l, a /* source */, b, Nb);
  }
}

void level_tick_end_temperature(Gamep g, Levelsp v, Levelp l)
{
  TRACE();

  int x = 0;
  int y = 0;

  if (l->is_handling_temperature_changes) {
    return;
  }
  l->is_handling_temperature_changes = true;

  FOR_ALL_MAP_POINTS(g, v, l, x, y)
  {
    std::vector< Thingp > things;

    //
    // Collect all things at this point into a vector
    //
    bpoint p(x, y);
    FOR_ALL_THINGS_AT_UNSAFE(g, v, l, t, p)
    {
      //
      // Ignore burnt grass for example
      //
      if (thing_is_dead(t)) {
        continue;
      }

      if (thing_is_physics_temperature(t)) {
        things.push_back(t);

        //
        // Check if the thing needs to pulse
        //
        (void) thing_is_hot_check(g, v, l, t);
      }
    }

    //
    // Make unique pairs
    //
    std::set< std::pair< Thingp, Thingp > > pairs;

    auto sz = static_cast< int >(things.size());
    for (auto i = 0; i < sz; i++) {
      for (auto j = 0; j < sz; j++) {
        auto *Ti = things[ i ];
        auto *Tj = things[ j ];
        if (Ti == Tj) {
          continue;
        }

        if ((void *) Ti > (void *) Tj) {
          std::swap(Ti, Tj);
        }

        pairs.insert(std::make_pair(Ti, Tj));
      }
    }

    std::vector< std::pair< Thingp, Thingp > > sorted_pairs;
    sorted_pairs.reserve(pairs.size());
    for (auto a_pair : pairs) {
      sorted_pairs.push_back(a_pair);
    }

    if (compiler_unused) {
      for (auto a_pair : sorted_pairs) {
        auto *a = a_pair.first;
        auto *b = a_pair.second;

        THING_DBG(a, "A before prio %d", a->_priority + b->_priority);
        THING_DBG(b, "B before");
      }
    }

    //
    // Sort by event priority
    //
    std::ranges::sort(sorted_pairs, [](const std::pair< Thingp, Thingp > &a, const std::pair< Thingp, Thingp > &b) -> bool {
      auto *t1 = a.first;
      auto *t2 = a.second;
      auto *t3 = b.first;
      auto *t4 = b.second;
      return t1->_priority + t2->_priority < t3->_priority + t4->_priority;
    });

    if (compiler_unused) {
      for (auto a_pair : sorted_pairs) {
        auto *a = a_pair.first;
        auto *b = a_pair.second;

        THING_DBG(a, "A after prio %d", a->_priority + b->_priority);
        THING_DBG(b, "B after");
      }
    }

    for (auto a_pair : sorted_pairs) {
      auto *a = a_pair.first;
      auto *b = a_pair.second;
      level_thing_pair_temperature_handle(g, v, l, a, b);
    }

    for (auto *t : things) {
      t->tick_temperature = v->tick;
    }
  }

  l->is_handling_temperature_changes = false;
}
