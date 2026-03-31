//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_main.hpp"
#include "my_thing_inlines.hpp"

//
// Add an item to the things inventory
//
auto thing_carry_item(Gamep g, Levelsp v, Levelp l, Thingp item, Thingp carrier) -> bool
{
  TRACE();

  if (! thing_is_item(item)) {
    return false;
  }

  if (! thing_is_able_to_collect_items(carrier)) {
    return false;
  }

  if (! thing_is_player(carrier) && ! thing_is_monst(carrier)) {
    thing_err(carrier, "unexpected thing for %s", __FUNCTION__);
    return false;
  }

  auto s = to_string(g, v, l, item);
  THING_DBG(carrier, "carry: %s", s.c_str());
  TRACE_INDENT();

  if (! thing_is_carried_try_set(g, v, l, item, carrier)) {
    THING_DBG(carrier, "carry: %s (failed)", s.c_str());
    TRACE_INDENT();

    if (thing_is_player(carrier)) {
      auto the_thing = thing_name_long_the(g, v, l, item);
      topcon("You fail to carry %s.", the_thing.c_str());
    }
    return false;
  }

  (void) thing_pop(g, v, item);

  thing_inventory_dump(g, v, l, carrier);

  if (thing_is_player(carrier)) {
    auto the_thing = thing_name_long_the(g, v, l, item);
    topcon("You carry %s.", the_thing.c_str());
  }

  return true;
}

//
// Drop an item from the things inventory
//
auto thing_drop_item(Gamep g, Levelsp v, Levelp l, Thingp item, Thingp dropper) -> bool
{
  TRACE();

  if (! thing_is_item(item)) {
    return false;
  }

  if (! thing_is_carried(item)) {
    return false;
  }

  if (! thing_is_player(dropper) && ! thing_is_monst(dropper)) {
    thing_err(dropper, "unexpected thing for %s", __FUNCTION__);
    return false;
  }

  auto s = to_string(g, v, l, item);
  THING_DBG(dropper, "drop: %s", s.c_str());
  TRACE_INDENT();

  if (! thing_is_carried_try_unset(g, v, l, item, dropper)) {
    THING_DBG(dropper, "drop: %s (failed)", s.c_str());
    TRACE_INDENT();

    if (thing_is_player(dropper)) {
      auto the_thing = thing_name_long_the(g, v, l, item);
      topcon("You fail to drop %s.", the_thing.c_str());
    }
    return false;
  }

  //
  // Drop the thing where the player is
  //
  if (! thing_warp_to(g, v, l, item, thing_at(dropper))) {
    auto the_thing = thing_name_long_the(g, v, l, item);
    topcon("You fail to place %s.", the_thing.c_str());
    return false;
  }

  if (thing_is_player(dropper)) {
    auto the_thing = thing_name_long_the(g, v, l, item);
    topcon("You drop %s.", the_thing.c_str());
  }

  return true;
}

auto thing_is_carried(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    err("no thing pointer");
    return false;
  }
  return t->_is_carried;
}

//
// Returns true/false on success/fail
//
auto thing_is_carried_try_set(Gamep g, Levelsp v, Levelp l, Thingp item, Thingp carrier, bool val) -> bool
{
  TRACE_DEBUG();

  if (! thing_is_player(carrier) && ! thing_is_monst(carrier)) {
    thing_err(carrier, "unexpected thing for %s", __FUNCTION__);
    return false;
  }

  if (item == nullptr) {
    err("no thing pointer");
    return false;
  }

  if (item->_is_carried == static_cast< int >(val)) {
    auto s = to_string(g, v, l, item);
    THING_DBG(carrier, "carry-try: %s (failed, already carried)", s.c_str());
    return true;
  }
  auto old_value    = item->_is_carried;
  item->_is_carried = val;

  //
  // Attempt the collect/drop. It can fail.
  //
  if (val) {
    //
    // Try to collect
    //
    if (! thing_on_carry_request(g, v, l, item, carrier)) {
      //
      // Collect failed
      //
      item->_is_carried = old_value;

      auto s = to_string(g, v, l, item);
      THING_DBG(carrier, "carry-try: %s (failed, carry request)", s.c_str());
      return false;
    }

    //
    // Add to the inventory.
    //
    if (! thing_inventory_add(g, v, l, item, carrier)) {
      //
      // Possibly out of slots
      //
      item->_is_carried = old_value;

      auto s = to_string(g, v, l, item);
      THING_DBG(carrier, "carry-try: %s (failed, inventory add)", s.c_str());
      return false;
    }

  } else {
    //
    // Try to drop
    //
    if (! thing_on_drop_request(g, v, l, item, carrier)) {
      //
      // Drop failed
      //
      item->_is_carried = old_value;

      auto s = to_string(g, v, l, item);
      THING_DBG(carrier, "drop-try: %s (failed, drop request)", s.c_str());
      return false;
    }

    //
    // Remove from the inventory.
    //
    if (! thing_inventory_remove(g, v, l, item, carrier)) {
      item->_is_carried = old_value;

      auto s = to_string(g, v, l, item);
      THING_DBG(carrier, "drop-try: %s (failed, inventory remove)", s.c_str());
      return false;
    }
  }

  //
  // Reset animation
  //
  thing_anim_init(g, v, l, item, THING_ANIM_IDLE);

  return true;
}

auto thing_is_carried_try_unset(Gamep g, Levelsp v, Levelp l, Thingp item, Thingp carrier) -> bool
{
  TRACE_DEBUG();

  return thing_is_carried_try_set(g, v, l, item, carrier, false);
}
