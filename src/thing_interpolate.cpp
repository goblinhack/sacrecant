//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_main.hpp"
#include "my_spoint.hpp"
#include "my_thing.hpp"
#include "my_thing_inlines.hpp"
#include "my_tile.hpp"
#include "my_types.hpp"

//
// For things moving between tiles, calculate the pixel they are at based on the timestep
//
void thing_interpolate(Gamep g, Levelsp v, Levelp l, Thingp me, float dt)
{
  TRACE();

  auto at          = thing_at(g, v, l, me);
  auto moving_from = thing_moving_from(me);

  if (1 || compiler_unused) {
    THING_DBG(g, v, l, me, "interpolate from (%d,%d) to (%d,%d) dt %f", moving_from.x, moving_from.y, at.x, at.y, dt);
  }

  if (moving_from == at) {
    if (1 || compiler_unused) {
      THING_DBG(g, v, l, me, "already at destination");
    }
    return;
  }

  if (dt < 0) {
    CROAK("negative dt %f", dt);
  }

  //
  // Cannot use thing_update_pos here, as the thing is already at the destination
  //
  float const pix_x = static_cast< float >(moving_from.x) + ((static_cast< float >(at.x - moving_from.x)) * dt);
  float const pix_y = static_cast< float >(moving_from.y) + ((static_cast< float >(at.y - moving_from.y)) * dt);

  spoint pix_at;
  pix_at.x = static_cast< int >(pix_x * static_cast< float >(TILE_WIDTH));
  pix_at.y = static_cast< int >(pix_y * static_cast< float >(TILE_HEIGHT));
  thing_pix_at_set(g, v, l, me, pix_at);

  if (compiler_unused) {
    THING_DBG(g, v, l, me, "from (%d,%d) to (%d,%d) dt %f pix_at (%d,%d)", moving_from.x, moving_from.y, at.x, at.y, dt, pix_at.x, pix_at.y);
  }

  FOR_ALL_HOOKS(g, v, l, me, buff)
  {
    FOR_ALL_FOLLOWERS(g, v, l, buff, follower) { thing_interpolate(g, v, l, follower, dt); }
  }
}
