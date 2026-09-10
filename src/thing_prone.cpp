//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_thing.hpp"
#include "my_thing_inlines.hpp"
#include "my_tp.hpp"
#include "my_types.hpp"

[[nodiscard]] auto thing_is_prone_to(Gamep g, Levelsp v, Levelp l, Thingp me, ThingEventType val) -> bool
{
  TRACE();

  auto *tp = thing_tp(me);

  FOR_ALL_HOOKS(g, v, l, me, buff)
  {
    if (thing_is_prone_to(g, v, l, buff, val)) {
      return true;
    }
  }

  FOR_ALL_ACTIVE_ITEMS(g, v, l, me, item)
  {
    if (thing_is_prone_to(g, v, l, item, val)) {
      return true;
    }
  }

  return tp_is_prone_to(tp, val);
}
