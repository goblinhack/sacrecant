//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_level.hpp"
#include "my_thing.hpp"
#include "my_types.hpp"
#include <cstdint>

[[nodiscard]] auto thing_debug(Gamep g, Levelsp v, Levelp l, Thingp t, uint32_t iter_index) -> bool
{
  TRACE();
  if (! thing_is_projectile(t)) {
    return true;
  }
  THING_DBG(g, v, l, t, "debug iter %u l-iter %u", t->iter[ iter_index ], l->iter[ iter_index ]);
  return true;
}
