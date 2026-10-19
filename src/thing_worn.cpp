//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_game.hpp"
#include "my_level.hpp"
#include "my_main.hpp"
#include "my_thing.hpp"
#include "my_thing_callbacks.hpp"
#include "my_thing_inlines.hpp"
#include "my_tp.hpp"
#include "my_types.hpp"
#include "my_ui.hpp"

//
// Drop an item from the things inventory
//
auto thing_strip_item(Gamep g, Levelsp v, Levelp l, Thingp owner, Thingp item, ThingEvent &e) -> bool
{
  TRACE();

  if (! thing_is_item(item)) {
    auto s = to_string(g, v, l, item);
    THING_DBG(g, v, l, owner, "strip fail (not an item): %s", s.c_str());
    return false;
  }

  if (! thing_is_worn(item)) {
    auto s = to_string(g, v, l, item);
    THING_DBG(g, v, l, owner, "strip fail (not worn): %s", s.c_str());
    return false;
  }

  if (! thing_is_player(owner) && ! thing_is_monst(owner)) {
    thing_err(g, v, l, owner, "unexpected thing for %s", __FUNCTION__);
    return false;
  }

  auto *ext = thing_ext_struct(g, v, owner);
  if (ext == nullptr) {
    return false;
  }

  auto s = to_string(g, v, l, item);
  THING_DBG(g, v, l, owner, "strip: %s", s.c_str());
  TRACE_INDENT();

  if (! thing_is_worn_try_unset(g, v, l, item, owner)) {
    THING_DBG(g, v, l, owner, "strip: %s (failed)", s.c_str());
    TRACE_INDENT();

    if (thing_is_player(owner)) {
      if (e.event_type == THING_EVENT_USER_INITIATED) {
        auto the_thing = thing_name_long_the(g, v, l, item);
        topcon(UI_IMPORTANT_FMT_STR "You fail to remove %s." UI_RESET_FMT, the_thing.c_str());
      }
    }
    return false;
  }

  if (thing_is_player(owner)) {
    if (e.event_type == THING_EVENT_USER_INITIATED) {
      auto the_thing = thing_name_long_the(g, v, l, item);
      topcon("You remove %s.", the_thing.c_str());
    }
    game_request_to_remake_ui_set(g);

    if (thing_is_tick_on_strip(item)) {
      (void) level_tick_begin_requested(g, v, l, "player worn an item");
    }
  }

  bool removed {};

  FOR_ALL_WORN_TYPES(w)
  {
    if (ext->worn_id[ w ] == item->id) {
      ext->worn_id[ w ] = 0;
      removed           = true;
      break;
    }
  }

  if (! removed) {
    thing_err(g, v, l, owner, "strip failed (remove did not find): %s", s.c_str());
    return false;
  }

  THING_DBG(g, v, l, owner, "strip success: %s", s.c_str());
  TRACE_INDENT();

  thing_inventory_dump(g, v, l, owner);

  return true;
}

[[nodiscard]] auto thing_is_worn(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    no_thing_pointer();
    return false;
  }
  return t->_is_worn;
}

//
// Returns true/false on success/fail
//
[[nodiscard]] auto thing_is_worn_try_set(Gamep g, Levelsp v, Levelp l, Thingp item, Thingp owner, bool val) -> bool
{
  TRACE_DEBUG();

  if (! thing_is_player(owner) && ! thing_is_monst(owner)) {
    thing_err(g, v, l, owner, "unexpected thing for %s", __FUNCTION__);
    return false;
  }

  if (item == nullptr) {
    no_thing_pointer();
    return false;
  }

  if (item->_is_worn == static_cast< int >(val)) {
    auto s = to_string(g, v, l, item);
    THING_DBG(g, v, l, owner, "wield-try: %s (failed, already worn)", s.c_str());
    return true;
  }
  auto old_value = item->_is_worn;
  item->_is_worn = val;

  //
  // Attempt the wield/strip. It can fail.
  //
  if (val) {
    //
    // Try to wield
    //
    if (! thing_on_worn_request(g, v, l, item, owner)) {
      //
      // Collect failed
      //
      item->_is_worn = old_value;

      auto s = to_string(g, v, l, item);
      THING_DBG(g, v, l, owner, "wield-try: %s (failed, wield request)", s.c_str());
      return false;
    }
  } else {
    //
    // Try to strip
    //
    if (! thing_on_strip_request(g, v, l, item, owner)) {
      //
      // Drop failed
      //
      item->_is_worn = old_value;

      auto s = to_string(g, v, l, item);
      THING_DBG(g, v, l, owner, "strip-try: %s (failed, strip request)", s.c_str());
      return false;
    }
  }

  return true;
}

[[nodiscard]] auto thing_is_worn_try_unset(Gamep g, Levelsp v, Levelp l, Thingp item, Thingp owner) -> bool
{
  TRACE_DEBUG();

  return thing_is_worn_try_set(g, v, l, item, owner, false);
}

[[nodiscard]] auto thing_worn_get(Gamep g, Levelsp v, Levelp l, Thingp owner, WornType w) -> Thingp
{
  TRACE();

  if (owner == nullptr) {
    no_thing_pointer();
    return nullptr;
  }

  auto *ext = thing_ext_struct(g, v, owner);
  if (ext == nullptr) {
    return nullptr;
  }

  if (! static_cast< bool >(ext->worn_id[ w ])) {
    return nullptr;
  }

  return thing_find_optional(g, v, ext->worn_id[ w ]);
}

//
// Add an item to the things inventory
//
static auto thing_wear_item_do(Gamep g, Levelsp v, Levelp l, Thingp owner, Thingp item, ThingEvent &e) -> bool
{
  TRACE();

  if (! thing_is_item(item)) {
    return false;
  }

  if (! thing_is_able_to_wear_items(owner)) {
    return false;
  }

  if (! thing_is_player(owner) && ! thing_is_monst(owner)) {
    thing_err(g, v, l, owner, "unexpected thing for %s", __FUNCTION__);
    return false;
  }

  auto s = to_string(g, v, l, item);
  THING_DBG(g, v, l, owner, "wield: %s", s.c_str());
  TRACE_INDENT();

  if (! thing_is_worn_try_set(g, v, l, item, owner)) {
    THING_DBG(g, v, l, owner, "wield: %s (failed)", s.c_str());
    TRACE_INDENT();

    if (thing_is_player(owner)) {
      auto the_thing = thing_name_long_the(g, v, l, item);
      topcon(UI_IMPORTANT_FMT_STR "You fail to remove %s." UI_RESET_FMT, the_thing.c_str());
    }
    return false;
  }

  (void) thing_pop(g, v, item);

  thing_inventory_dump(g, v, l, owner);

  if (thing_is_player(owner)) {
    if (e.event_type != THING_EVENT_SPAWNED) {
      auto the_thing = thing_name_long_the(g, v, l, item);
      if (thing_is_weapon(item)) {
        topcon("You wield %s.", the_thing.c_str());
      } else {
        topcon("You wear %s.", the_thing.c_str());
      }
    }
    game_request_to_remake_ui_set(g);

    if (thing_is_tick_on_worn(item)) {
      (void) level_tick_begin_requested(g, v, l, "player worn an item");
    }
  }

  if (! thing_is_worn(item)) {
    thing_err(g, v, l, owner, "wield fail (claimed success): %s", s.c_str());
    return false;
  }

  return true;
}

[[nodiscard]] static auto thing_wear_item_slot(Gamep g, Levelsp v, Levelp l, Thingp owner, Thingp item, WornType w, ThingEvent &e) -> bool
{
  TRACE();

  if (owner == nullptr) {
    no_thing_pointer();
    return false;
  }

  if (item == nullptr) {
    thing_err(g, v, l, owner, "no item to wield");
    return false;
  }

  auto *ext = thing_ext_struct(g, v, owner);
  if (ext == nullptr) {
    return false;
  }

  auto s = to_string(g, v, l, item);
  THING_DBG(g, v, l, owner, "wield: %s (%s)", s.c_str(), WornType_to_string(w).c_str());
  TRACE_INDENT();

  if (ext->worn_id[ w ] != 0U) {
    auto *existing_item = thing_worn_get(g, v, l, owner, w);
    if (existing_item == nullptr) {
      thing_err(g, v, l, owner, "worn_id is set, but no item");
      return false;
    }

    //
    // Already worn
    //
    if (existing_item == item) {
      THING_DBG(g, v, l, owner, "wield (already worn): %s", s.c_str());
      return true;
    }

    auto s2 = to_string(g, v, l, existing_item);
    THING_DBG(g, v, l, owner, "strip existing: %s", s2.c_str());
    TRACE_INDENT();

    if (! thing_strip_item(g, v, l, owner, existing_item, e)) {
      THING_DBG(g, v, l, owner, "strip existing fail: %s (%s)", s.c_str(), WornType_to_string(w).c_str());
      return false;
    }
  }

  if (! thing_wear_item_do(g, v, l, owner, item, e)) {
    THING_DBG(g, v, l, owner, "wield fail: %s (%s)", s.c_str(), WornType_to_string(w).c_str());
    return false;
  }

  if (! thing_is_worn(item)) {
    thing_err(g, v, l, owner, "wield fail (claimed success): %s", s.c_str());
    return false;
  }

  THING_DBG(g, v, l, owner, "wield success: %s (%s)", s.c_str(), WornType_to_string(w).c_str());

  ext->worn_id[ w ] = item->id;

  return true;
}

//
// Wield into a free slot. If there is something there already, or no free slots, strip something
//
[[nodiscard]] auto thing_wear_item(Gamep g, Levelsp v, Levelp l, Thingp owner, Thingp item, ThingEvent &e) -> bool
{
  TRACE();

  if (owner == nullptr) {
    no_thing_pointer();
    return false;
  }

  if (item == nullptr) {
    thing_err(g, v, l, owner, "no item to wield");
    return false;
  }

  auto s = to_string(g, v, l, item);
  THING_DBG(g, v, l, owner, "wield: %s", s.c_str());
  TRACE_INDENT();

  if (thing_is_weapon(item)) {
    auto *existing_item = thing_worn_get(g, v, l, owner, WORN_TYPE_WEAPON);
    if (existing_item != nullptr) {
      //
      // Already worn
      //
      if (existing_item == item) {
        THING_DBG(g, v, l, owner, "wield (already worn): %s", s.c_str());
        return true;
      }

      auto s2 = to_string(g, v, l, existing_item);
      THING_DBG(g, v, l, owner, "strip weapon: %s", s2.c_str());
      TRACE_INDENT();

      if (! thing_strip_item(g, v, l, owner, existing_item, e)) {
        return false;
      }
    }
    return thing_wear_item_slot(g, v, l, owner, item, WORN_TYPE_WEAPON, e);
  }

  if (thing_is_boots(item)) {
    auto *existing_item = thing_worn_get(g, v, l, owner, WORN_TYPE_BOOTS);
    if (existing_item != nullptr) {
      //
      // Already worn
      //
      if (existing_item == item) {
        THING_DBG(g, v, l, owner, "wield (already worn): %s", s.c_str());
        return true;
      }

      auto s2 = to_string(g, v, l, existing_item);
      THING_DBG(g, v, l, owner, "strip boots: %s", s2.c_str());
      TRACE_INDENT();

      if (! thing_strip_item(g, v, l, owner, existing_item, e)) {
        return false;
      }
    }
    return thing_wear_item_slot(g, v, l, owner, item, WORN_TYPE_BOOTS, e);
  }

  if (thing_is_ring(item)) {
    auto *ring1 = thing_worn_get(g, v, l, owner, WORN_TYPE_RING1);
    auto *ring2 = thing_worn_get(g, v, l, owner, WORN_TYPE_RING2);

    //
    // Already worn
    //
    if ((ring1 == item) || (ring2 == item)) {
      THING_DBG(g, v, l, owner, "wield (already worn): %s", s.c_str());
      return true;
    }

    if (ring1 != nullptr) {
      if (ring2 != nullptr) {
        auto s2 = to_string(g, v, l, ring2);
        THING_DBG(g, v, l, owner, "strip existing ring: %s", s2.c_str());
        TRACE_INDENT();

        if (! thing_strip_item(g, v, l, owner, ring2, e)) {
          return false;
        }
      }

      THING_DBG(g, v, l, owner, "wield as ring2: %s", s.c_str());
      TRACE_INDENT();

      return thing_wear_item_slot(g, v, l, owner, item, WORN_TYPE_RING2, e);
    }
    THING_DBG(g, v, l, owner, "wield as ring1: %s", s.c_str());
    TRACE_INDENT();

    return thing_wear_item_slot(g, v, l, owner, item, WORN_TYPE_RING1, e);
  }

  if (! thing_is_worn(item)) {
    thing_err(g, v, l, owner, "wield fail (claimed success): %s", s.c_str());
    return false;
  }

  return false;
}

//
// Wield into a free slot if there is nothing there already.
//
[[nodiscard]] auto thing_auto_wear_try(Gamep g, Levelsp v, Levelp l, Thingp owner, Thingp item, ThingEvent &e) -> bool
{
  TRACE();

  if (! thing_is_auto_wear(item)) {
    return false;
  }

  auto s = to_string(g, v, l, item);
  THING_DBG(g, v, l, owner, "auto wield: %s", s.c_str());
  TRACE_INDENT();

  if (thing_is_weapon(item)) {
    if (thing_worn_get(g, v, l, owner, WORN_TYPE_WEAPON) == nullptr) {
      return thing_wear_item(g, v, l, owner, item, e);
    }
  }

  if (thing_is_boots(item)) {
    if (thing_worn_get(g, v, l, owner, WORN_TYPE_BOOTS) == nullptr) {
      return thing_wear_item(g, v, l, owner, item, e);
    }
  }

  if (thing_is_ring(item)) {
    if (thing_worn_get(g, v, l, owner, WORN_TYPE_RING1) == nullptr) {
      return thing_wear_item_slot(g, v, l, owner, item, WORN_TYPE_RING1, e);
    }
    if (thing_worn_get(g, v, l, owner, WORN_TYPE_RING2) == nullptr) {
      return thing_wear_item_slot(g, v, l, owner, item, WORN_TYPE_RING2, e);
    }
  }

  return false;
}

void thing_on_worn_request_set(Tpp tp, thing_on_worn_request_t callback)
{
  TRACE();
  if (tp == nullptr) [[unlikely]] {
    no_tp_pointer();
    return;
  }
  tp->on_worn_request = callback;
}

[[nodiscard]] auto thing_on_worn_request(Gamep g, Levelsp v, Levelp l, Thingp me, Thingp owner) -> bool
{
  TRACE();
  auto *tp = thing_tp(me);
  if (tp == nullptr) [[unlikely]] {
    no_tp_pointer();
    return false;
  }
  if (tp->on_worn_request == nullptr) {
    //
    // Assume success
    //
    return true;
  }
  if (! thing_is_player(owner) && ! thing_is_monst(owner)) {
    thing_err(g, v, l, owner, "unexpected thing for %s", __FUNCTION__);
    return false;
  }
  return tp->on_worn_request(g, v, l, me, owner);
}

void thing_on_strip_request_set(Tpp tp, thing_on_strip_request_t callback)
{
  TRACE();
  if (tp == nullptr) [[unlikely]] {
    no_tp_pointer();
    return;
  }
  tp->on_strip_request = callback;
}

[[nodiscard]] auto thing_on_strip_request(Gamep g, Levelsp v, Levelp l, Thingp me, Thingp owner) -> bool
{
  TRACE();
  auto *tp = thing_tp(me);
  if (tp == nullptr) [[unlikely]] {
    no_tp_pointer();
    return false;
  }
  if (tp->on_strip_request == nullptr) {
    //
    // Assume success
    //
    return true;
  }
  if (! thing_is_player(owner) && ! thing_is_monst(owner)) {
    thing_err(g, v, l, owner, "unexpected thing for %s", __FUNCTION__);
    return false;
  }
  return tp->on_strip_request(g, v, l, me, owner);
}
