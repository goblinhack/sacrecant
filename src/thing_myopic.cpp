//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_level_inlines.hpp" // NOLINT
#include "my_main.hpp"
#include "my_thing.hpp"
#include "my_thing_callbacks.hpp"
#include "my_thing_inlines.hpp" // NOLINT
#include "my_tp.hpp"
#include "my_tp_class.hpp"
#include "my_types.hpp"

[[nodiscard]] auto thing_is_myopic(Gamep g, Levelsp v, Levelp l, Thingp me) -> bool
{
  TRACE_DEBUG();

  if (me == nullptr) {
    ERR("no thing pointer");
    return false;
  }

  if (tp_flag(thing_tp(me), is_myopic) != 0) {
    return true;
  }

  FOR_ALL_HOOKS(g, v, l, me, buff)
  {
    if (thing_is_myopic(g, v, l, buff)) {
      return true;
    }
  }

  FOR_ALL_ACTIVE_ITEMS(g, v, l, me, item)
  {
    if (thing_is_myopic(g, v, l, item)) {
      return true;
    }
  }

  return false;
}
