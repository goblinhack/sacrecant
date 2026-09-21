//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_main.hpp"
#include "my_thing.hpp"
#include "my_thing_inlines.hpp"
#include "my_tp.hpp"
#include "my_types.hpp"

#include <cstring>

//
// Anything in the inventory
//
[[nodiscard]] auto thing_inventory_is_empty(Gamep g, Levelsp v, Levelp l, Thingp owner) -> bool
{
  TRACE();

  if (! thing_is_player(owner) && ! thing_is_monst(owner)) {
    thing_err(g, v, l, owner, "unexpected thing for %s", __FUNCTION__);
    return false;
  }

  FOR_ALL_INVENTORY_ITEMS(g, v, l, owner, item)
  {
    if (item != nullptr) {}
    return false;
  }

  return true;
}

//
// Can we combine identical items?
//
[[nodiscard]] auto thing_inventory_item_mergeable(Gamep g, Levelsp v, Levelp l, Thingp a, Thingp b) -> bool
{
  if (! thing_is_item_mergeable(a)) {
    return false;
  }

  if (! thing_is_item_mergeable(b)) {
    return false;
  }

  if (a->tp_id != b->tp_id) {
    return false;
  }

  return true;
}

//
// Add an item to the inventory
//
[[nodiscard]] auto thing_inventory_add(Gamep g, Levelsp v, Levelp l, Thingp new_item, Thingp owner) -> bool
{
  THING_DBG(g, v, l, new_item, "inventory add");
  TRACE();

  if (! thing_is_player(owner) && ! thing_is_monst(owner)) {
    thing_err(g, v, l, owner, "unexpected thing for %s", __FUNCTION__);
    return false;
  }

  if (new_item == nullptr) {
    return false;
  }

  auto *ext_struct = thing_ext_struct(g, v, owner);
  if (ext_struct == nullptr) {
    return false;
  }

  //
  // Look for a matching item first.
  //
  FOR_ALL_INVENTORY_SLOTS(g, v, l, owner, slot, item)
  {
    if (item == nullptr) {
      continue;
    }

    if (! thing_inventory_item_mergeable(g, v, l, item, new_item)) {
      continue;
    }

    slot->count++;

    ThingEvent e {
        .reason     = "by merging",               //
        .event_type = THING_EVENT_CARRIED_MERGED, //
        .source     = owner                       //
    };

    THING_DBG(g, v, l, new_item, "dead due to merging");
    TRACE_INDENT();

    thing_dead(g, v, l, new_item, e);

    return true;
  }

  //
  // Look for a free slot
  //
  FOR_ALL_INVENTORY_SLOTS(g, v, l, owner, slot, item)
  {
    if (item != nullptr) {
      continue;
    }

    memset(slot, 0, sizeof(*slot));
    slot->thing_id = new_item->id;
    slot->count    = 1;
    return true;
  }

  //
  // Out of slots
  //
  return false;
}

//
// Drop an item to the inventory
//
[[nodiscard]] auto thing_inventory_remove(Gamep g, Levelsp v, Levelp l, Thingp drop_item, Thingp owner) -> bool
{
  THING_DBG(g, v, l, drop_item, "inventory remove");
  TRACE();

  if (! thing_is_player(owner) && ! thing_is_monst(owner)) {
    thing_err(g, v, l, owner, "unexpected thing for %s", __FUNCTION__);
    return false;
  }

  if (drop_item == nullptr) {
    return false;
  }

  auto *ext_struct = thing_ext_struct(g, v, owner);
  if (ext_struct == nullptr) {
    return false;
  }

  //
  // Look for the thing
  //
  FOR_ALL_INVENTORY_SLOTS(g, v, l, owner, slot, item)
  {
    if (item == nullptr) {
      continue;
    }

    if (drop_item != item) {
      continue;
    }

    THING_DBG(g, v, l, item, "slot %d: count %d", _n_, slot->count);
    TRACE_INDENT();

    slot->count--;

    if (slot->count > 0) {
      THING_DBG(g, v, l, item, "item count remains");
      return true;
    }

    memset(slot, 0, sizeof(*slot));
    return true;
  }

  return false;
}

//
// Get a single items slot count
//
[[nodiscard]] auto thing_inventory_get_item_count(Gamep g, Levelsp v, Levelp l, Thingp item, Thingp owner) -> int
{
  TRACE();

  if (item == nullptr) {
    return -1;
  }

  auto *ext_struct = thing_ext_struct(g, v, owner);
  if (ext_struct == nullptr) {
    return -1;
  }

  //
  // Look for the thing
  //
  FOR_ALL_INVENTORY_SLOTS(g, v, l, owner, slot, an_item)
  {
    if (item == nullptr) {
      continue;
    }

    if (an_item != item) {
      continue;
    }

    return slot->count;
  }

  return -1;
}

//
// Get a single items slot count
//
[[nodiscard]] auto thing_inventory_get_item_count(Gamep g, Levelsp v, Levelp l, Thingp owner) -> int
{
  TRACE();

  int count = 0;

  auto *ext_struct = thing_ext_struct(g, v, owner);
  if (ext_struct == nullptr) {
    return 0;
  }

  FOR_ALL_INVENTORY_SLOTS(g, v, l, owner, slot, item)
  {
    if (item == nullptr) {
      continue;
    }

    count += slot->count;
  }

  return count;
}

//
// Drop an item to the inventory
//
void thing_inventory_dump(Gamep g, Levelsp v, Levelp l, Thingp owner)
{
  IF_NODEBUG { return; }

  TRACE();

  if (! thing_is_player(owner) && ! thing_is_monst(owner)) {
    thing_err(g, v, l, owner, "unexpected thing for %s", __FUNCTION__);
    return;
  }

  auto *ext_struct = thing_ext_struct(g, v, owner);
  if (ext_struct == nullptr) {
    return;
  }

  FOR_ALL_INVENTORY_SLOTS(g, v, l, owner, slot, item)
  {
    if (item == nullptr) {
      THING_DBG(g, v, l, owner, "slot %d: -", _n_);
      continue;
    }

    auto s = to_string(g, v, l, item);
    THING_DBG(g, v, l, owner, "slot %d: %s, count %d", _n_, s.c_str(), slot->count);
  }
}
