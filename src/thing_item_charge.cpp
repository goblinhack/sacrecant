//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_thing.hpp"         // NOLINT
#include "my_thing_inlines.hpp" // NOLINT
#include "my_types.hpp"

[[nodiscard]] auto thing_charge_count(Gamep g, Levelsp v, Levelp l, Thingp me) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    no_thing_pointer();
    return 0;
  }

  auto *ext = thing_ext_struct(g, v, me);
  if (ext == nullptr) {
    return 0;
  }

  return ext->_charge_count;
}

[[nodiscard]] auto thing_charge_count_set(Gamep g, Levelsp v, Levelp l, Thingp me, int val) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    no_thing_pointer();
    return 0;
  }

  auto *ext = thing_ext_struct(g, v, me);
  if (ext == nullptr) {
    return 0;
  }

  return ext->_charge_count = val;
}

[[nodiscard]] auto thing_charge_count_incr(Gamep g, Levelsp v, Levelp l, Thingp me, int val) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    no_thing_pointer();
    return 0;
  }

  auto *ext = thing_ext_struct(g, v, me);
  if (ext == nullptr) {
    return 0;
  }

  return ext->_charge_count += val;
}

[[nodiscard]] auto thing_charge_count_decr(Gamep g, Levelsp v, Levelp l, Thingp me, int val) -> int
{
  TRACE_DEBUG();

  if (me == nullptr) {
    no_thing_pointer();
    return 0;
  }

  auto *ext = thing_ext_struct(g, v, me);
  if (ext == nullptr) {
    return 0;
  }

  if (static_cast< int >(ext->_charge_count) - val <= 0) {
    return ext->_charge_count = 0;
  }

  return ext->_charge_count -= val;
}
