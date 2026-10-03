//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_level.hpp"
#include "my_level_inlines.hpp" // NOLINT
#include "my_thing.hpp"
#include "my_thing_inlines.hpp" // NOLINT
#include "my_types.hpp"

//
// See what's on the level
//
void level_count_items(Gamep g, Levelsp v, Levelp l)
{
  TRACE();

  l->info.monst_count       = 0;
  l->info.monst1_count      = 0;
  l->info.monst2_count      = 0;
  l->info.mob_count         = 0;
  l->info.mob1_count        = 0;
  l->info.mob2_count        = 0;
  l->info.treasure_count    = 0;
  l->info.teleport_count    = 0;
  l->info.door_locked_count = 0;
  l->info.key_count         = 0;

  FOR_ALL_THINGS_ON_LEVEL_NO_BREAK_UNSAFE_NO_BREAK(g, v, l, t)
  {
    if (thing_is_monst(t)) {
      l->info.monst_count++;
    }
    if (thing_is_monst1(t)) {
      l->info.monst1_count++;
    }
    if (thing_is_monst2(t)) {
      l->info.monst2_count++;
    }
    if (thing_is_mob(t)) {
      l->info.mob_count++;
    }
    if (thing_is_mob1(t)) {
      l->info.mob1_count++;
    }
    if (thing_is_mob2(t)) {
      l->info.mob2_count++;
    }
    if (thing_is_treasure(t)) {
      l->info.treasure_count++;
    }
    if (thing_is_teleport(t)) {
      l->info.teleport_count++;
    }
    if (thing_is_door_locked(t)) {
      l->info.door_locked_count++;
    }
    if (thing_is_key(t)) {
      l->info.key_count++;
    }
  }
}
