//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_main.hpp"
#include "my_thing.hpp" // NOLINT
#include "my_types.hpp"

[[nodiscard]] auto thing_charge_count(Thingp t) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    no_thing_pointer();
    return 0;
  }
  return t->_charge_count;
}

[[nodiscard]] auto thing_charge_count_set(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    no_thing_pointer();
    return 0;
  }
  return t->_charge_count = val;
}

[[nodiscard]] auto thing_charge_count_incr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    no_thing_pointer();
    return 0;
  }
  return t->_charge_count += val;
}

[[nodiscard]] auto thing_charge_count_decr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    no_thing_pointer();
    return 0;
  }
  if (static_cast< int >(t->_charge_count) - val <= 0) {
    return t->_charge_count = 0;
  }
  return t->_charge_count -= val;
}
