//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_level.hpp"
#include "my_types.hpp"

//
// Create all levels
//
void levels_init(Gamep g)
{
  TRACE();

  levels_test(g);
  levels_fixed(g);
  levels_treasure(g);
}
