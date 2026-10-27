//
// Copyright goblinhack@gmail.com
//

#ifndef MY_THING_INLINES_HPP
#define MY_THING_INLINES_HPP

#ifdef DEBUG_BUILD
#include "my_callstack.hpp"
#endif

#include "my_level.hpp"
#include "my_main.hpp"
#include "my_thing.hpp"
#include "my_tp_class.hpp"
#include "my_tp_inlines.hpp"

[[nodiscard]] static inline auto tp_flag(Tpp tp, ThingFlagType f) -> int
{
#ifdef DEBUG_BUILD
  TRACE_DEBUG(); // expensive
  if (tp == nullptr) [[unlikely]] {
    no_tp_pointer();
    return 0;
  }
#endif

  return tp->flag[ f ];
}

//
// this belongs in my_thing.hpp but as it is inlined, it needs to access
// the levels structure
//
[[nodiscard]] static inline auto thing_find(Gamep g, Levelsp v, ThingId id) -> Thingp
{
#ifdef DEBUG_BUILD
  TRACE_DEBUG(); // expensive
  if (g == nullptr) [[unlikely]] {
    no_game_pointer();
    return nullptr;
  }
  //
  if (v == nullptr) [[unlikely]] {
    no_levels_pointer();
    return nullptr;
  }
#endif

  ThingIdPacked id_packed = {};
  id_packed.a.val         = id;
  auto arr_index          = id_packed.c.arr_index;

  auto *me = &v->thing_body[ arr_index ];

#ifdef ENABLE_PER_THING_MEMORY
  if (v->thing_body_debug[ arr_index ]) {
    me = v->thing_body_debug[ arr_index ];
  }
#endif

  [[unlikely]] if ((me == nullptr) || (me->id != id)) {
    me = thing_find_non_inline(g, v, id);
  }

  return me;
}

[[nodiscard]] static inline auto thing_tp(Thingp me) -> Tpp
{
#ifdef DEBUG_BUILD
  TRACE_DEBUG(); // expensive
  if (me == nullptr) {
    no_thing_pointer();
    return nullptr;
  }
#endif

  return tp_find(me->tp_id);
}

[[nodiscard]] static inline auto thing_is_falling(Thingp me) -> int
{
#ifdef DEBUG_BUILD
  TRACE_DEBUG();
  if (me == nullptr) {
    no_thing_pointer();
    return 0;
  }
#endif

  return me->_fall_ms;
}

[[nodiscard]] static inline auto thing_is_hit(Thingp me) -> int
{
#ifdef DEBUG_BUILD
  TRACE_DEBUG();
  if (me == nullptr) {
    no_thing_pointer();
    return 0;
  }
#endif

  return me->_is_hit;
}

[[nodiscard]] static inline auto thing_is_hidden(Thingp me) -> int
{
#ifdef DEBUG_BUILD
  TRACE_DEBUG();
  if (me == nullptr) {
    no_thing_pointer();
    return 0;
  }
#endif

  return me->_is_hidden;
}

[[nodiscard]] static inline auto thing_is_hot(Thingp me) -> int
{
#ifdef DEBUG_BUILD
  TRACE_DEBUG();
  if (me == nullptr) {
    no_thing_pointer();
    return 0;
  }
#endif

  return me->_is_hot;
}

[[nodiscard]] static inline auto thing_is_jumping(Thingp me) -> bool
{
#ifdef DEBUG_BUILD
  TRACE_DEBUG();
  if (me == nullptr) {
    no_thing_pointer();
    return false;
  }
#endif

  return me->_is_jumping;
}

[[nodiscard]] static inline auto thing_is_thrown(Thingp me) -> bool
{
#ifdef DEBUG_BUILD
  TRACE_DEBUG();
  if (me == nullptr) {
    no_thing_pointer();
    return false;
  }
#endif

  return me->_is_thrown;
}

[[nodiscard]] static inline auto thing_is_moving(Thingp me) -> bool
{
#ifdef DEBUG_BUILD
  TRACE_DEBUG();
  if (me == nullptr) {
    no_thing_pointer();
    return false;
  }
#endif

  return me->_is_moving;
}

[[nodiscard]] static inline auto thing_is_spawned(Thingp me) -> bool
{
#ifdef DEBUG_BUILD
  TRACE_DEBUG();
  if (me == nullptr) {
    no_thing_pointer();
    return false;
  }
#endif

  return me->_is_spawned;
}

[[nodiscard]] static inline auto thing_is_dead(Thingp me) -> bool
{
#ifdef DEBUG_BUILD
  TRACE_DEBUG();
  if (me == nullptr) {
    no_thing_pointer();
    return false;
  }
#endif

  return me->_is_dead;
}

[[nodiscard]] static inline auto thing_is_physics_temperature(Thingp me) -> bool
{
#ifdef DEBUG_BUILD
  TRACE_DEBUG();
  if (me == nullptr) {
    no_thing_pointer();
    return false;
  }
#endif

  return tp_flag(thing_tp(me), is_physics_temperature) != 0;
}

[[nodiscard]] static inline auto thing_is_fire(Thingp me) -> bool
{
#ifdef DEBUG_BUILD
  TRACE_DEBUG();
  if (me == nullptr) {
    no_thing_pointer();
    return 0;
  }
#endif

  return tp_flag(thing_tp(me), is_fire) != 0;
}

[[nodiscard]] static inline auto thing_is_lava(Thingp me) -> bool
{
#ifdef DEBUG_BUILD
  TRACE_DEBUG();
  if (me == nullptr) {
    no_thing_pointer();
    return 0;
  }
#endif

  return tp_flag(thing_tp(me), is_lava) != 0;
}

[[nodiscard]] static inline auto thing_is_player(const Thingp me) -> bool
{
#ifdef DEBUG_BUILD
  TRACE_DEBUG();
  if (me == nullptr) {
    no_thing_pointer();
    return false;
  }
#endif

  return me->_is_player;
}

[[nodiscard]] static inline auto thing_is_water_shallow(Thingp me) -> bool
{
#ifdef DEBUG_BUILD
  TRACE_DEBUG();

  if (me == nullptr) {
    no_thing_pointer();
    return false;
  }
#endif
  return tp_flag(thing_tp(me), is_water_shallow) != 0;
}

[[nodiscard]] static inline auto thing_is_foliage(Thingp me) -> bool
{
#ifdef DEBUG_BUILD
  TRACE_DEBUG();

  if (me == nullptr) {
    no_thing_pointer();
    return false;
  }
#endif
  return tp_flag(thing_tp(me), is_foliage) != 0;
}

[[nodiscard]] static inline auto thing_is_reeds(Thingp me) -> bool
{
#ifdef DEBUG_BUILD
  TRACE_DEBUG();

  if (me == nullptr) {
    no_thing_pointer();
    return false;
  }
#endif
  return tp_flag(thing_tp(me), is_reeds) != 0;
}

[[nodiscard]] static inline auto thing_is_wall(Thingp me) -> bool
{
#ifdef DEBUG_BUILD
  TRACE_DEBUG();

  if (me == nullptr) {
    no_thing_pointer();
    return false;
  }
#endif
  return tp_flag(thing_tp(me), is_wall) != 0;
}

[[nodiscard]] static inline auto thing_is_floor(Thingp me) -> bool
{
#ifdef DEBUG_BUILD
  TRACE_DEBUG();

  if (me == nullptr) {
    no_thing_pointer();
    return false;
  }
#endif
  return tp_flag(thing_tp(me), is_floor) != 0;
}

[[nodiscard]] static inline auto thing_is_obs_to_vision(Thingp me) -> bool
{
#ifdef DEBUG_BUILD
  TRACE_DEBUG();
  if (me == nullptr) {
    no_thing_pointer();
    return false;
  }
#endif

  return tp_flag(thing_tp(me), is_obs_to_vision) != 0;
}

[[nodiscard]] static inline auto thing_pix_at(Thingp me) -> spoint
{
#ifdef DEBUG_BUILD
  TRACE_DEBUG();
  if (me == nullptr) {
    no_thing_pointer();
    return spoint(0, 0);
  }
#endif

  return me->_curr_pix_at;
}

[[nodiscard]] static inline auto thing_is_open(Thingp me) -> bool
{
#ifdef DEBUG_BUILD
  TRACE_DEBUG();
  if (me == nullptr) {
    no_thing_pointer();
    return false;
  }
#endif

  return me->_is_open;
}

[[nodiscard]] static inline auto thing_is_engulfed(Thingp me) -> bool
{
#ifdef DEBUG_BUILD
  TRACE_DEBUG();
  if (me == nullptr) {
    no_thing_pointer();
    return false;
  }
#endif

  return me->_is_engulfed;
}

[[nodiscard]] static inline auto thing_is_ensnared(Thingp me) -> bool
{
#ifdef DEBUG_BUILD
  TRACE_DEBUG();
  if (me == nullptr) {
    no_thing_pointer();
    return false;
  }
#endif

  return me->_is_ensnared;
}

[[nodiscard]] static inline auto thing_find_optional(Gamep g, Levelsp v, ThingId id) -> Thingp
{
#ifdef DEBUG_BUILD
  TRACE_DEBUG(); // expensive
#endif

  if (! static_cast< bool >(id)) {
    return nullptr;
  }

  ThingIdPacked id_packed;
  id_packed.a.val = id;
  auto arr_index  = id_packed.c.arr_index;

  auto *me = &v->thing_body[ arr_index ];

#ifdef ENABLE_PER_THING_MEMORY
  if (v->thing_body_debug[ arr_index ]) {
    me = v->thing_body_debug[ arr_index ];
  }

  if (me == nullptr) {
    return nullptr;
  }
#endif

  if (me->id == id) {
    return me;
  }

  if (me->id != 0U) {
    thing_err(g, v, thing_level(g, v, me), me, "thing_find_optional: thing mismatch found for id, %p %08" PRIX32 "", (void *) me, me->id);
  } else {
    thing_err(g, v, thing_level(g, v, me), me, "thing_find_optional: thing already freed for id, %p %08" PRIX32 "", (void *) me, me->id);
  }

  return nullptr;
}

[[nodiscard]] static inline auto thing_find_optional_may_be_freed(Gamep g, Levelsp v, ThingId id) -> Thingp
{
#ifdef DEBUG_BUILD
  TRACE_DEBUG(); // expensive
#endif

  if (! static_cast< bool >(id)) {
    return nullptr;
  }

  ThingIdPacked id_packed;
  id_packed.a.val = id;
  auto arr_index  = id_packed.c.arr_index;

  auto *me = &v->thing_body[ arr_index ];
#ifdef ENABLE_PER_THING_MEMORY
  if (v->thing_body_debug[ arr_index ]) {
    me = v->thing_body_debug[ arr_index ];
  }

  if (me == nullptr) {
    return nullptr;
  }
#endif

  if (me->id == id) {
    return me;
  }

  if (me->id != 0U) {
    thing_err(g, v, thing_level(g, v, me), me, "thing_find_optional: thing mismatch found for id, %p %08" PRIX32 "", (void *) me, me->id);
  }

  return nullptr;
}

[[nodiscard]] static inline auto thing_is_light_source(Thingp me) -> int
{
#ifdef DEBUG_BUILD
  TRACE_DEBUG();
  if (me == nullptr) {
    no_thing_pointer();
    return 0;
  }
#endif

  return tp_flag(thing_tp(me), is_light_source);
}

[[nodiscard]] static inline auto thing_submerged_pct(Thingp me) -> int
{
#ifdef DEBUG_BUILD
  TRACE_DEBUG();
  if (me == nullptr) {
    no_thing_pointer();
    return 0;
  }
#endif

  if (thing_is_jumping(me)) {
    return (me->_submerged_pct * 2) / 3;
  }

  //
  // Floating when dead?
  //
  if (thing_is_dead(me)) {
    return me->_submerged_pct / 2;
  }

  return me->_submerged_pct;
}

[[nodiscard]] static inline auto thing_ext_struct(Gamep g, Levelsp v, Thingp me) -> ThingExtp
{
#ifdef DEBUG_BUILD
  TRACE_DEBUG();
  if (me == nullptr) {
    no_thing_pointer();
    return nullptr;
  }
#endif

  auto ext_id = me->ext_id;
  if (ext_id == 0U) [[unlikely]] {
    return nullptr;
  }

  return &v->thing_ext[ ext_id ];
}

[[nodiscard]] static inline auto thing_dmap_struct(Gamep g, Levelsp v, Thingp me) -> ThingDmapp
{
#ifdef DEBUG_BUILD
  TRACE_DEBUG();
  if (me == nullptr) {
    no_thing_pointer();
    return nullptr;
  }
#endif

  auto *ext = thing_ext_struct(g, v, me);
  if (ext == nullptr) [[unlikely]] {
    return nullptr;
  }

  auto dmap_id = ext->dmap_id;
  if (dmap_id == 0U) [[unlikely]] {
    return nullptr;
  }

  return &v->thing_dmap[ dmap_id ];
}

[[nodiscard]] static inline auto thing_z_depth_get(Gamep g, Levelsp v, Levelp l, Thingp me) -> MapZDepthType
{
#ifdef DEBUG_BUILD
  TRACE();
#endif

  auto *tp = thing_tp(me);
#ifdef DEBUG_BUILD
  if (tp == nullptr) [[unlikely]] {
    no_tp_pointer();
    return MAP_Z_DEPTH_FLOOR;
  }
#endif

  //
  // Ensure dead monsters behind alive ones
  //
  if (thing_is_monst(me) && (thing_is_dead(me) || thing_is_corpse(me))) {
    return MAP_Z_DEPTH_DEAD_MONST;
  }

  if (tp->z_depth_get == nullptr) {
    return tp_z_depth_get(tp);
  }

  return tp->z_depth_get(g, v, l, me);
}

[[nodiscard]] static inline auto thing_is_able_to_be_levitated(Thingp me) -> bool
{
#ifdef DEBUG_BUILD
  TRACE();
#endif

  auto *tp = thing_tp(me);
#ifdef DEBUG_BUILD
  if (tp == nullptr) [[unlikely]] {
    no_tp_pointer();
    return MAP_Z_DEPTH_FLOOR;
  }
#endif
  return tp_flag(tp, is_able_to_be_levitated) != 0;
}

[[nodiscard]] static inline auto thing_is_able_to_be_invisible(Thingp me) -> bool
{
#ifdef DEBUG_BUILD
  TRACE();
#endif

  auto *tp = thing_tp(me);
#ifdef DEBUG_BUILD
  if (tp == nullptr) [[unlikely]] {
    no_tp_pointer();
    return MAP_Z_DEPTH_FLOOR;
  }
#endif
  return tp_flag(tp, is_able_to_be_invisible) != 0;
}

[[nodiscard]] static inline auto thing_is_able_to_be_teleported(Thingp me) -> bool
{
#ifdef DEBUG_BUILD
  TRACE();
#endif

  auto *tp = thing_tp(me);
#ifdef DEBUG_BUILD
  if (tp == nullptr) [[unlikely]] {
    no_tp_pointer();
    return MAP_Z_DEPTH_FLOOR;
  }
#endif
  return tp_flag(tp, is_able_to_be_teleported) != 0;
}

[[nodiscard]] static inline auto thing_is_lunging(Gamep g, Levelsp v, Thingp me) -> int
{
#ifdef DEBUG_BUILD
  TRACE_DEBUG();
  if (me == nullptr) {
    no_thing_pointer();
    return 0;
  }
#endif

  auto *ext = thing_ext_struct(g, v, me);
  if (ext == nullptr) [[unlikely]] {
    return 0;
  }

  return ext->_lunge_ms;
}

[[nodiscard]] static inline auto thing_is_recoiling(Gamep g, Levelsp v, Thingp me) -> int
{
#ifdef DEBUG_BUILD
  TRACE_DEBUG();
  if (me == nullptr) {
    no_thing_pointer();
    return 0;
  }
#endif

  auto *ext = thing_ext_struct(g, v, me);
  if (ext == nullptr) [[unlikely]] {
    return 0;
  }

  return ext->_recoil_ms;
}

#endif // MY_THING_INLINES_HPP
