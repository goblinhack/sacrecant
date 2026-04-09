//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_level.hpp"

//
// Create all rooms
//
void rooms_init(Gamep g)
{
  TRACE();

  rooms_start(g);
  rooms_blend1(g);
  rooms_blend2(g);
  rooms_chunky(g);
  rooms_circular(g);
  rooms_cross(g);
  rooms_cross_sym(g);
  rooms_small(g);
  rooms_medium(g);
  rooms_large(g);
  rooms_tall(g);
  rooms_long(g);
  rooms_exit(g);
  rooms_key(g);
  rooms_prefab(g);
  rooms_prefab_secret(g);
  rooms_prefab(g);
  rooms_prefab_secret(g);
}
