//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_main.hpp"
#include "my_thing.hpp"
#include "my_thing_inlines.hpp"
#include "my_types.hpp"

[[nodiscard]] auto thing_owner(Gamep g, Levelsp v, Levelp l, Thingp me) -> Thingp
{
  TRACE();

  if (me == nullptr) {
    no_thing_pointer();
    return nullptr;
  }

  if (! static_cast< bool >(me->owner_id)) {
    return nullptr;
  }

  return thing_find_optional(g, v, me->owner_id);
}

void thing_owner_unset(Gamep g, Levelsp v, Levelp l, Thingp me)
{
  TRACE();

  if (me == nullptr) {
    no_thing_pointer();
    return;
  }

  auto *owner = thing_owner(g, v, l, me);
  if (owner != nullptr) {
    me->owner_id = 0;
  }
}

void thing_owner_set(Gamep g, Levelsp v, Levelp l, Thingp me, Thingp owner)
{
  TRACE();

  if (me == nullptr) {
    no_thing_pointer();
    return;
  }

  if (owner == nullptr) {
    thing_err(g, v, l, me, "no owner");
    return;
  }

  thing_owner_unset(g, v, l, me);

  me->owner_id = owner->id;
}
