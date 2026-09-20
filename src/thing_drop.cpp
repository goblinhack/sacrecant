//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
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
static auto thing_drop_item(Gamep g, Levelsp v, Levelp l, Thingp user, Thingp item, ThingEvent &e) -> bool
{
  TRACE();

  if (! thing_is_item(item)) {
    thing_err(g, v, l, user, "unexpected non thing, %s", __FUNCTION__);
    return false;
  }

  if (! thing_is_carried(item)) {
    thing_err(g, v, l, user, "unexpected uncarried thing, %s", __FUNCTION__);
    return false;
  }

  if (! thing_is_player(user) && ! thing_is_monst(user)) {
    thing_err(g, v, l, user, "unexpected thing, %s", __FUNCTION__);
    return false;
  }

  auto s = to_string(g, v, l, item);
  THING_DBG(g, v, l, user, "drop: %s", s.c_str());
  TRACE_INDENT();

  if (! thing_is_carried_unset(g, v, l, item, user, e)) {
    THING_DBG(g, v, l, user, "drop: %s (failed)", s.c_str());
    TRACE_INDENT();

    if (thing_is_player(user)) {
      if (e.event_type == THING_EVENT_USER_INITIATED) {
        auto the_thing = thing_name_long_the(g, v, l, item);
        topcon(UI_WARN_FMT_STR "You fail to drop %s." UI_RESET_FMT, the_thing.c_str());
      }

      //
      // Needed for cursed items
      //
      if (thing_is_tick_on_drop(item)) {
        (void) level_tick_begin_requested(g, v, l, "player failed dropped an item");
      }
    }

    return false;
  }

  //
  // Drop the thing where the player is
  //
  if (! thing_is_thrown(item)) {
    THING_DBG(g, v, l, user, "drop: %s (need to place the item)", s.c_str());
    TRACE_INDENT();

    if (! thing_warp_to(g, v, l, item, thing_at(g, v, l, user))) {
      if (thing_is_player(user)) {
        if (e.event_type == THING_EVENT_USER_INITIATED) {
          auto the_thing = thing_name_long_the(g, v, l, item);
          topcon(UI_WARN_FMT_STR "You fail to place %s." UI_RESET_FMT, the_thing.c_str());
        }
      }
      return false;
    }

    THING_DBG(g, v, l, user, "drop: %s (placed the item)", s.c_str());
  }

  //
  // Replace the thing with a copy if count exists
  //
  if (thing_inventory_get_item_count(g, v, l, item, user) != -1) {
    THING_DBG(g, v, l, user, "drop: %s (item count remains, need a copy)", s.c_str());

    FOR_ALL_INVENTORY_SLOTS(g, v, l, user, slot, an_item)
    {
      if (an_item == nullptr) {
        continue;
      }

      //
      // Replace the thing dropped with a new thing
      //
      if (thing_tp(an_item) == thing_tp(item)) {
        auto *thing_copy = thing_spawn(g, v, l, thing_tp(item), user);

        if (thing_copy != nullptr) {
          THING_DBG(g, v, l, thing_copy, "drop: %s (thing copy)", s.c_str());
          TRACE_INDENT();

          slot->thing_id           = thing_copy->id;
          thing_copy->_is_carried = true;

          //
          // Is carried, so pop off the map
          //
          (void) thing_pop(g, v, thing_copy);

          //
          // Set the same owner
          //
          thing_owner_set(g, v, l, thing_copy, user);
        }
      }
    }
  }

  THING_DBG(g, v, l, user, "drop: %s (drop completed)", s.c_str());
  TRACE_INDENT();

  if (thing_is_player(user)) {
    if (e.event_type == THING_EVENT_USER_INITIATED) {
      auto the_thing = thing_name_long_the(g, v, l, item);
      topcon("You drop %s.", the_thing.c_str());
    }

    if (e.event_type == THING_EVENT_USER_INITIATED) {
      if (thing_is_tick_on_drop(item)) {
        (void) level_tick_begin_requested(g, v, l, "player dropped an item");
      }
    }
  }

  thing_owner_unset(g, v, l, item);

  thing_inventory_dump(g, v, l, user);

  item->tick_dropped = v->tick;

  return true;
}

void thing_on_drop_request_set(Tpp tp, thing_on_drop_request_t callback)
{
  TRACE();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return;
  }
  tp->on_drop_request = callback;
}

[[nodiscard]] auto thing_on_drop_request(Gamep g, Levelsp v, Levelp l, Thingp item, Thingp user, ThingEvent &e) -> bool
{
  TRACE();
  auto *tp = thing_tp(item);
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return false;
  }

  if (e.event_type == THING_EVENT_EATEN) {
    return true;
  }

  if (thing_is_cursed(item)) {
    if (tp->on_drop_request == nullptr) {
      if (thing_is_player(user)) {
        auto the_thing = thing_name_long_the(g, v, l, item);
        topcon(UI_WARN_FMT_STR "You fail to drop %s. It might be cursed..." UI_RESET_FMT, the_thing.c_str());
      }
      return false;
    }
  }

  if (tp->on_drop_request == nullptr) {
    return true;
  }

  if (! thing_is_player(user) && ! thing_is_monst(user)) {
    thing_err(g, v, l, user, "unexpected thing for %s", __FUNCTION__);
    return false;
  }
  return tp->on_drop_request(g, v, l, item, user, e);
}

void thing_on_drop_success_set(Tpp tp, thing_on_drop_success_t callback)
{
  TRACE();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return;
  }
  tp->on_drop_success = callback;
}

[[nodiscard]] auto thing_on_drop_success(Gamep g, Levelsp v, Levelp l, Thingp item, Thingp user, ThingEvent &e) -> bool
{
  TRACE();
  auto *tp = thing_tp(item);
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return false;
  }
  if (tp->on_drop_success == nullptr) {
    //
    // Assume success
    //
    if (e.event_type == THING_EVENT_USER_INITIATED) {
      if (thing_is_player(user)) {
        thing_sound_play(g, v, l, user, "item_drop");
      }
    }
    return true;
  }
  if (! thing_is_player(user) && ! thing_is_monst(user)) {
    thing_err(g, v, l, user, "unexpected thing for %s", __FUNCTION__);
    return false;
  }
  return tp->on_drop_success(g, v, l, item, user, e);
}

[[nodiscard]] auto thing_drop(Gamep g, Levelsp v, Levelp l, Thingp user, Thingp item, ThingEvent &e) -> bool
{
  TRACE();

  if (user == nullptr) {
    ERR("no thing pointer");
    return false;
  }

  if (item == nullptr) {
    thing_err(g, v, l, user, "no item to drop");
    return false;
  }

  if (thing_is_worn(item)) {
    if (! thing_strip_item(g, v, l, user, item, e)) {
      return false;
    }
  }

  return thing_drop_item(g, v, l, user, item, e);
}

[[nodiscard]] auto thing_drop_all(Gamep g, Levelsp v, Levelp l, Thingp user, ThingEvent &e) -> bool
{
  TRACE();

  if (user == nullptr) {
    ERR("no thing pointer");
    return false;
  }

  bool ok = true;

  while (thing_inventory_get_item_count(g, v, l, user) > 0) {
    FOR_ALL_INVENTORY_ITEMS(g, v, l, user, an_item)
    {
      if (! thing_drop(g, v, l, user, an_item, e)) {
        ok = false;
      }
    }
  }

  return ok;
}
