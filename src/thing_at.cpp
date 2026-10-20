//
// Copyright goblinhack@gmail.com
//

#include "my_bpoint.hpp"
#include "my_callstack.hpp"
#include "my_fpoint.hpp"
#include "my_level.hpp"
#include "my_main.hpp"
#include "my_spoint.hpp"
#include "my_thing.hpp"
#include "my_thing_inlines.hpp"
#include "my_types.hpp"

[[nodiscard]] auto thing_at(Gamep g, Levelsp v, Levelp l, Thingp me) -> bpoint
{
  TRACE_DEBUG();

  if (me == nullptr) {
    no_thing_pointer();
    return bpoint(0, 0);
  }

  if (! thing_is_thrown(me)) {
    if (thing_is_carried(me)) {
      auto *owner = thing_owner(g, v, l, me);
      if (owner != nullptr) {
        return thing_at(g, v, l, owner);
      }
    }
  }

  return make_bpoint(me->_at);
}

[[nodiscard]] auto thing_at_no_owner(Gamep g, Levelsp v, Levelp l, Thingp me) -> bpoint
{
  TRACE_DEBUG();

  if (me == nullptr) {
    no_thing_pointer();
    return bpoint(0, 0);
  }

  return make_bpoint(me->_at);
}

void thing_at_set(Gamep g, Levelsp v, Levelp l, Thingp me, const bpoint &val)
{
  TRACE_DEBUG();

  if (me == nullptr) {
    no_thing_pointer();
    return;
  }

  auto valf = make_fpoint(val);

  //
  // Not sure we need this, it ends up doing updates twice per tick as we do one at the tick end anyway
  //
  if (compiler_unused) {
    if (thing_is_player(me)) {
      if (me->_at != valf) {
        l->request_to_update_per_tile_visibility = true;
      }
    }
  }

  me->_old_at = me->_at;
  me->_at     = valf;
}

[[nodiscard]] auto thing_real_at(Gamep g, Levelsp v, Levelp l, Thingp me) -> fpoint
{
  TRACE_DEBUG();

  if (me == nullptr) {
    no_thing_pointer();
    return fpoint(0, 0);
  }
  return me->_at;
}

void thing_at_set(Gamep g, Levelsp v, Levelp l, Thingp me, const fpoint &val)
{
  TRACE_DEBUG();

  if (me == nullptr) {
    no_thing_pointer();
    return;
  }

  //
  // Not sure we need this, it ends up doing updates twice per tick as we do one at the tick end anyway
  //
  if (compiler_unused) {
    if (thing_is_player(me)) {
      if (me->_at != val) {
        l->request_to_update_per_tile_visibility = true;
      }
    }
  }

  me->_old_at = me->_at;
  me->_at     = val;
}

[[nodiscard]] auto thing_old_at(Thingp me) -> bpoint
{
  TRACE_DEBUG();

  if (me == nullptr) {
    no_thing_pointer();
    return bpoint(0, 0);
  }
  return make_bpoint(me->_old_at);
}

[[nodiscard]] auto thing_prev_pix_at(Thingp me) -> spoint
{
  TRACE_DEBUG();

  if (me == nullptr) {
    no_thing_pointer();
    return spoint(0, 0);
  }
  return me->_prev_pix_at;
}

void thing_prev_pix_at_set(Gamep g, Levelsp v, Levelp l, Thingp me, const spoint &val)
{
  TRACE_DEBUG();

  if (me == nullptr) {
    no_thing_pointer();
    return;
  }
  me->_prev_pix_at = val;
}

void thing_pix_at_set(Gamep g, Levelsp v, Levelp l, Thingp me, const spoint &val)
{
  TRACE_DEBUG();

  if (me == nullptr) {
    no_thing_pointer();
    return;
  }

  if (thing_is_light_source(me) != 0) {
    if (me->_curr_pix_at != val) {
      l->request_to_update_per_pixel_visibility = true;
    }
  }

  me->_prev_pix_at = me->_curr_pix_at;
  me->_curr_pix_at = val;
}

void thing_pix_at_set(Gamep g, Levelsp v, Levelp l, Thingp me, short x, short y)
{
  TRACE_DEBUG();

  if (me == nullptr) {
    no_thing_pointer();
    return;
  }

  spoint const val(x, y);

  if (thing_is_light_source(me) != 0) {
    if (me->_curr_pix_at != val) {
      l->request_to_update_per_pixel_visibility = true;
    }
  }

  me->_prev_pix_at = me->_curr_pix_at;
  me->_curr_pix_at = val;
}
