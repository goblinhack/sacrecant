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
    ERR("no thing template pointer");
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
    ERR("no game pointer");
    return nullptr;
  }
  //
  if (v == nullptr) [[unlikely]] {
    ERR("no levels pointer");
    return nullptr;
  }
#endif

  ThingIdPacked id_packed = {};
  id_packed.a.val         = id;
  auto arr_index          = id_packed.c.arr_index;

  auto *t = &v->thing_body[ arr_index ];

#ifdef ENABLE_PER_THING_MEMORY
  if (v->thing_body_debug[ arr_index ]) {
    t = v->thing_body_debug[ arr_index ];
  }
#endif

  [[unlikely]] if ((t == nullptr) || (t->id != id)) {
    t = thing_find_non_inline(g, v, id);
  }

  return t;
}

[[nodiscard]] static inline auto thing_tp(Thingp t) -> Tpp
{
#ifdef DEBUG_BUILD
  TRACE_DEBUG(); // expensive
  if (t == nullptr) {
    ERR("no thing pointer");
    return nullptr;
  }
#endif

  return tp_find(t->tp_id);
}

[[nodiscard]] static inline auto thing_is_falling(Thingp t) -> int
{
#ifdef DEBUG_BUILD
  TRACE_DEBUG();
  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
#endif

  return t->_fall_ms;
}

[[nodiscard]] static inline auto thing_is_lunging(Thingp t) -> int
{
#ifdef DEBUG_BUILD
  TRACE_DEBUG();
  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
#endif

  return t->_lunge_ms;
}

[[nodiscard]] static inline auto thing_is_hit(Thingp t) -> int
{
#ifdef DEBUG_BUILD
  TRACE_DEBUG();
  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
#endif

  return t->_is_hit;
}

[[nodiscard]] static inline auto thing_is_hidden(Thingp t) -> int
{
#ifdef DEBUG_BUILD
  TRACE_DEBUG();
  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
#endif

  return t->_is_hidden;
}

[[nodiscard]] static inline auto thing_is_hot(Thingp t) -> int
{
#ifdef DEBUG_BUILD
  TRACE_DEBUG();
  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
#endif

  return t->_is_hot;
}

[[nodiscard]] static inline auto thing_is_jumping(Thingp t) -> bool
{
#ifdef DEBUG_BUILD
  TRACE_DEBUG();
  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
#endif

  return t->_is_jumping;
}

[[nodiscard]] static inline auto thing_is_thrown(Thingp t) -> bool
{
#ifdef DEBUG_BUILD
  TRACE_DEBUG();
  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
#endif

  return t->_is_thrown;
}

[[nodiscard]] static inline auto thing_is_moving(Thingp t) -> bool
{
#ifdef DEBUG_BUILD
  TRACE_DEBUG();
  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
#endif

  return t->_is_moving;
}

[[nodiscard]] static inline auto thing_is_spawned(Thingp t) -> bool
{
#ifdef DEBUG_BUILD
  TRACE_DEBUG();
  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
#endif

  return t->_is_spawned;
}

[[nodiscard]] static inline auto thing_is_dead(Thingp t) -> bool
{
#ifdef DEBUG_BUILD
  TRACE_DEBUG();
  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
#endif

  return t->_is_dead;
}

[[nodiscard]] static inline auto thing_is_physics_temperature(Thingp t) -> bool
{
#ifdef DEBUG_BUILD
  TRACE_DEBUG();
  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
#endif

  return tp_flag(thing_tp(t), is_physics_temperature) != 0;
}

[[nodiscard]] static inline auto thing_is_fire(Thingp t) -> bool
{
#ifdef DEBUG_BUILD
  TRACE_DEBUG();
  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
#endif

  return tp_flag(thing_tp(t), is_fire) != 0;
}

[[nodiscard]] static inline auto thing_is_lava(Thingp t) -> bool
{
#ifdef DEBUG_BUILD
  TRACE_DEBUG();
  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
#endif

  return tp_flag(thing_tp(t), is_lava) != 0;
}

[[nodiscard]] static inline auto thing_is_explosion_major(Thingp t) -> bool
{
#ifdef DEBUG_BUILD
  TRACE_DEBUG();
  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
#endif

  return tp_flag(thing_tp(t), is_explosion_major) != 0;
}

[[nodiscard]] static inline auto thing_is_player(const Thingp t) -> bool
{
#ifdef DEBUG_BUILD
  TRACE_DEBUG();
  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
#endif

  return t->_is_player;
}

[[nodiscard]] static inline auto thing_is_water_shallow(Thingp t) -> bool
{
#ifdef DEBUG_BUILD
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
#endif
  return tp_flag(thing_tp(t), is_water_shallow) != 0;
}

[[nodiscard]] static inline auto thing_is_foliage(Thingp t) -> bool
{
#ifdef DEBUG_BUILD
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
#endif
  return tp_flag(thing_tp(t), is_foliage) != 0;
}

[[nodiscard]] static inline auto thing_is_reeds(Thingp t) -> bool
{
#ifdef DEBUG_BUILD
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
#endif
  return tp_flag(thing_tp(t), is_reeds) != 0;
}

[[nodiscard]] static inline auto thing_is_wall(Thingp t) -> bool
{
#ifdef DEBUG_BUILD
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
#endif
  return tp_flag(thing_tp(t), is_wall) != 0;
}

[[nodiscard]] static inline auto thing_is_floor(Thingp t) -> bool
{
#ifdef DEBUG_BUILD
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
#endif
  return tp_flag(thing_tp(t), is_floor) != 0;
}

[[nodiscard]] static inline auto thing_is_obs_to_vision(Thingp t) -> bool
{
#ifdef DEBUG_BUILD
  TRACE_DEBUG();
  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
#endif

  return tp_flag(thing_tp(t), is_obs_to_vision) != 0;
}

[[nodiscard]] static inline auto thing_pix_at(Thingp t) -> spoint
{
#ifdef DEBUG_BUILD
  TRACE_DEBUG();
  if (t == nullptr) {
    CROAK("no thing pointer");
  }
#endif

  return t->_curr_pix_at;
}

[[nodiscard]] static inline auto thing_is_open(Thingp t) -> bool
{
#ifdef DEBUG_BUILD
  TRACE_DEBUG();
  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
#endif

  return t->_is_open;
}

[[nodiscard]] static inline auto thing_is_engulfed(Thingp t) -> bool
{
#ifdef DEBUG_BUILD
  TRACE_DEBUG();
  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
#endif

  return t->_is_engulfed;
}

[[nodiscard]] static inline auto thing_is_ensnared(Thingp t) -> bool
{
#ifdef DEBUG_BUILD
  TRACE_DEBUG();
  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
#endif

  return t->_is_ensnared;
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

  auto *t = &v->thing_body[ arr_index ];
#ifdef ENABLE_PER_THING_MEMORY
  if (v->thing_body_debug[ arr_index ]) {
    t = v->thing_body_debug[ arr_index ];
  }
#endif

  if (t == nullptr) {
    return nullptr;
  }

  if (t->id == id) {
    return t;
  }

  return nullptr;
}

[[nodiscard]] static inline auto thing_is_light_source(Thingp t) -> int
{
#ifdef DEBUG_BUILD
  TRACE_DEBUG();
  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
#endif

  return tp_flag(thing_tp(t), is_light_source);
}

[[nodiscard]] static inline auto thing_submerged_pct(Thingp t) -> int
{
#ifdef DEBUG_BUILD
  TRACE_DEBUG();
  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
#endif

  if (thing_is_jumping(t)) {
    return (t->_submerged_pct * 2) / 3;
  }

  //
  // Floating when dead?
  //
  if (thing_is_dead(t)) {
    return t->_submerged_pct / 2;
  }

  return t->_submerged_pct;
}

[[nodiscard]] static inline auto thing_ext_struct(Gamep g, Levelsp v, Thingp t) -> ThingExtp
{
#ifdef DEBUG_BUILD
  TRACE_DEBUG();
  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
#endif

  auto ext_id = t->ext_id;
  if (ext_id == 0U) [[unlikely]] {
    return nullptr;
  }

  return &v->thing_ext[ ext_id ];
}

[[nodiscard]] static inline auto thing_z_depth_get(Gamep g, Levelsp v, Levelp l, Thingp me) -> MapZDepthType
{
#ifdef DEBUG_BUILD
  TRACE();
#endif

  auto *tp = thing_tp(me);
#ifdef DEBUG_BUILD
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
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
    ERR("no thing template pointer");
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
    ERR("no thing template pointer");
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
    ERR("no thing template pointer");
    return MAP_Z_DEPTH_FLOOR;
  }
#endif
  return tp_flag(tp, is_able_to_be_teleported) != 0;
}

#endif // MY_THING_INLINES_HPP
