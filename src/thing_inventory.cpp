//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_thing_inlines.hpp"

#include <cstring>

//
// Anything in the inventory
//
auto thing_inventory_is_empty(Gamep g, Levelsp v, Levelp l, Thingp carrier) -> bool
{
  TRACE();

  if (! thing_is_player(carrier) && ! thing_is_monst(carrier)) {
    thing_err(carrier, "unexpected thing for %s", __FUNCTION__);
    return false;
  }

  FOR_ALL_INVENTORY_ITEMS(g, v, l, carrier, item) { return false; }

  return true;
}

//
// Can we combine identical items?
//
auto thing_inventory_item_mergeable(Gamep g, Levelsp v, Levelp l, Thingp a, Thingp b) -> bool
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
auto thing_inventory_add(Gamep g, Levelsp v, Levelp l, Thingp new_item, Thingp carrier) -> bool
{
  TRACE();

  if (! thing_is_player(carrier) && ! thing_is_monst(carrier)) {
    thing_err(carrier, "unexpected thing for %s", __FUNCTION__);
    return false;
  }

  if (new_item == nullptr) {
    return false;
  }

  auto *ext_struct = thing_ext_struct(g, carrier);
  if (ext_struct == nullptr) {
    return false;
  }

  //
  // Look for a matching item first.
  //
  FOR_ALL_INVENTORY_SLOTS(g, v, l, carrier, slot, item)
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
        .source     = carrier                     //
    };

    THING_DBG(new_item, "dead due to merging");
    TRACE_INDENT();

    thing_dead(g, v, l, new_item, e);

    return true;
  }

  //
  // Look for a free slot
  //
  FOR_ALL_INVENTORY_SLOTS(g, v, l, carrier, slot, item)
  {
    if (item != nullptr) {
      continue;
    }

    memset(slot, 0, sizeof(*slot));
    slot->item_id = new_item->id;
    slot->count   = 1;
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
auto thing_inventory_remove(Gamep g, Levelsp v, Levelp l, Thingp drop_item, Thingp carrier) -> bool
{
  TRACE();

  if (! thing_is_player(carrier) && ! thing_is_monst(carrier)) {
    thing_err(carrier, "unexpected thing for %s", __FUNCTION__);
    return false;
  }

  if (drop_item == nullptr) {
    return false;
  }

  auto *ext_struct = thing_ext_struct(g, carrier);
  if (ext_struct == nullptr) {
    return false;
  }

  //
  // Look for the thing
  //
  FOR_ALL_INVENTORY_SLOTS(g, v, l, carrier, slot, item)
  {
    if (thing_tp(drop_item) != thing_tp(item)) {
      continue;
    }

    slot->count--;

    if (slot->count > 1) {
      continue;
    }

    memset(slot, 0, sizeof(*slot));
    return true;
  }

  return true;
}

//
// Drop an item to the inventory
//
void thing_inventory_dump(Gamep g, Levelsp v, Levelp l, Thingp carrier)
{
  TRACE();

  if (! thing_is_player(carrier) && ! thing_is_monst(carrier)) {
    thing_err(carrier, "unexpected thing for %s", __FUNCTION__);
    return;
  }

  auto *ext_struct = thing_ext_struct(g, carrier);
  if (ext_struct == nullptr) {
    return;
  }

  FOR_ALL_INVENTORY_SLOTS(g, v, l, carrier, slot, item)
  { //
    if (item == nullptr) {
      THING_DBG(carrier, "slot %d: -", _n_);
      continue;
    }

    auto s = to_string(g, v, l, item);
    if (slot->count != 0) {
      THING_DBG(carrier, "slot %d: %s, count %d", _n_, s.c_str(), slot->count);
    } else {
      THING_DBG(carrier, "slot %d: %s", _n_, s.c_str());
    }
  }
}
