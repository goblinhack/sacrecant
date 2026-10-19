//

#include "my_callstack.hpp"
#include "my_main.hpp"
#include "my_thing.hpp"
#include "my_types.hpp"

[[nodiscard]] auto thing_age(Thingp t) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    no_thing_pointer();
    return 0;
  }
  return t->_age;
}

[[nodiscard]] auto thing_age_set(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    no_thing_pointer();
    return 0;
  }
  return t->_age = val;
}

[[nodiscard]] auto thing_age_incr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    no_thing_pointer();
    return 0;
  }
  return t->_age += val;
}

[[nodiscard]] auto thing_age_decr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    no_thing_pointer();
    return 0;
  }
  if (t->_age - val <= 0) {
    return t->_age = 0;
  }
  return t->_age -= val;
}
