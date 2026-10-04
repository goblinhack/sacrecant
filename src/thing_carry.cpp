//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_game.hpp"
#include "my_main.hpp"
#include "my_thing.hpp"
#include "my_thing_callbacks.hpp"
#include "my_thing_inlines.hpp"
#include "my_tp.hpp"
#include "my_types.hpp"
#include "my_ui.hpp"
#include <initializer_list>
#include <vector>

//
// Add an item to the things inventory
//
static auto thing_carry_item(Gamep g, Levelsp v, Levelp l, Thingp item, Thingp collector, ThingEvent &e) -> bool
{
  TRACE();

  if (! thing_is_able_to_collect_items(collector)) {
    THING_DBG(g, v, l, collector, "cannot carry items");
    return false;
  }

  //
  // Need to stop the player collecting a chest, like an engulfer would do. We want the player to open the chest.
  //
  if (thing_is_player(collector)) {
    if (thing_is_chest(item)) {
      THING_DBG(g, v, l, collector, "cannot carry a chest");
      return false;
    }
  }

  if (! thing_is_item(item)) {
    THING_DBG(g, v, l, collector, "cannot carry a non item");
    return false;
  }

  if (! thing_is_player(collector) && ! thing_is_monst(collector)) {
    thing_err(g, v, l, collector, "unexpected thing, %s", __FUNCTION__);
    return false;
  }

  auto s = to_string(g, v, l, item);
  THING_DBG(g, v, l, collector, "carry: %s", s.c_str());
  TRACE_INDENT();

  //
  // Is there a limit to what can be collected? e.g. engulfers
  //
  auto max_items = thing_items_collected_max(collector);
  if (max_items != 0) {
    if (thing_inventory_get_item_count(g, v, l, collector) >= thing_items_collected_max(collector)) {
      THING_DBG(g, v, l, collector, "no, too many items carried");
      return false;
    }
  }

  if (! thing_is_carried_set(g, v, l, item, collector, e, true /* carry */, false /* force */)) {
    THING_DBG(g, v, l, collector, "carry: %s (failed)", s.c_str());
    TRACE_INDENT();

    if (thing_is_player(collector)) {
      auto the_thing = thing_name_long_the(g, v, l, item);
      topcon(UI_WARN_FMT_STR "You fail to carry %s." UI_RESET_FMT, the_thing.c_str());
    }
    return false;
  }

  (void) thing_pop(g, v, item);

  if (thing_is_player(collector)) {
    if (e.event_type != THING_EVENT_SPAWNED) {
      auto the_thing = thing_name_long_the(g, v, l, item);
      topcon("You carry %s.", the_thing.c_str());
    }
    game_request_to_remake_ui_set(g);
  }

  thing_owner_set(g, v, l, item, collector);

  thing_inventory_dump(g, v, l, collector);

  return true;
}

[[nodiscard]] auto thing_is_carried(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return t->_is_carried;
}

//
// Returns true/false on success/fail
//
[[nodiscard]] auto thing_is_carried_set(Gamep g, Levelsp v, Levelp l, Thingp item, Thingp owner, ThingEvent &e, bool val, bool force) -> bool
{
  TRACE_DEBUG();

  if (! thing_is_player(owner) && ! thing_is_monst(owner)) {
    thing_err(g, v, l, owner, "unexpected thing for %s", __FUNCTION__);
    return false;
  }

  if (item == nullptr) {
    ERR("no thing pointer");
    return false;
  }

  if (item->_is_carried == static_cast< int >(val)) {
    auto s = to_string(g, v, l, item);
    THING_DBG(g, v, l, owner, "carry-try: %s (failed, already carried)", s.c_str());
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
    THING_DBG(g, v, l, item, "carry request");
    TRACE_INDENT();

    if (! thing_on_carry_request(g, v, l, item, owner, e)) {
      //
      // Collect failed
      //
      item->_is_carried = old_value;

      auto s = to_string(g, v, l, item);
      THING_DBG(g, v, l, owner, "carry-try: %s (failed, carry request)", s.c_str());
      return false;
    }

    //
    // Add to the inventory.
    //
    if (thing_inventory_add(g, v, l, item, owner)) {
      //
      // Success
      //
      (void) thing_on_carry_success(g, v, l, item, owner, e);
    } else {
      //
      // Possibly out of slots
      //
      item->_is_carried = old_value;

      auto s = to_string(g, v, l, item);
      THING_DBG(g, v, l, owner, "carry-try: %s (failed, inventory add)", s.c_str());
      return false;
    }

  } else {
    //
    // Try to drop
    //
    THING_DBG(g, v, l, item, "drop request");
    TRACE_INDENT();

    if (! thing_on_drop_request(g, v, l, item, owner, e)) {
      //
      // Drop failed
      //
      item->_is_carried = old_value;

      auto s = to_string(g, v, l, item);
      THING_DBG(g, v, l, owner, "drop-try: %s (failed, drop request)", s.c_str());

      //
      // Cursed items need forceably dropped
      //
      if (force) {
        THING_DBG(g, v, l, owner, "drop-try: %s (failed, force drop request)", s.c_str());
      } else {
        return false;
      }
    }

    //
    // Remove from the inventory.
    //
    if (thing_inventory_remove(g, v, l, item, owner)) {
      //
      // Success
      //
      (void) thing_on_drop_success(g, v, l, item, owner, e);
    } else {
      //
      // Remove failed
      //
      item->_is_carried = old_value;

      auto s = to_string(g, v, l, item);
      THING_DBG(g, v, l, owner, "drop-try: %s (failed, inventory remove)", s.c_str());
      return false;
    }
  }

  //
  // Reset animation
  //
  thing_anim_init(g, v, l, item, THING_ANIM_IDLE);

  return true;
}

[[nodiscard]] auto thing_is_carried_unset(Gamep g, Levelsp v, Levelp l, Thingp item, Thingp owner, ThingEvent &e) -> bool
{
  TRACE_DEBUG();

  return thing_is_carried_set(g, v, l, item, owner, e, false, false);
}

void thing_on_carry_request_set(Tpp tp, thing_on_carry_request_t callback)
{
  TRACE();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return;
  }
  tp->on_carry_request = callback;
}

[[nodiscard]] auto thing_on_carry_request(Gamep g, Levelsp v, Levelp l, Thingp me, Thingp collector, ThingEvent &e) -> bool
{
  TRACE();
  auto *tp = thing_tp(me);
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return false;
  }
  if (tp->on_carry_request == nullptr) {
    //
    // Assume success
    //
    return true;
  }
  if (! thing_is_player(collector) && ! thing_is_monst(collector)) {
    thing_err(g, v, l, collector, "unexpected thing for %s", __FUNCTION__);
    return false;
  }
  return tp->on_carry_request(g, v, l, me, collector, e);
}

void thing_on_carry_success_set(Tpp tp, thing_on_carry_success_t callback)
{
  TRACE();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return;
  }
  tp->on_carry_success = callback;
}

[[nodiscard]] auto thing_on_carry_success(Gamep g, Levelsp v, Levelp l, Thingp me, Thingp collector, ThingEvent &e) -> bool
{
  TRACE();
  auto *tp = thing_tp(me);
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return false;
  }
  if (tp->on_carry_success == nullptr) {
    //
    // Assume success
    //
    if (thing_is_player(collector)) {
      thing_sound_play(g, v, l, collector, "item_collect");
    }
    return true;
  }
  if (! thing_is_player(collector) && ! thing_is_monst(collector)) {
    thing_err(g, v, l, collector, "unexpected thing for %s", __FUNCTION__);
    return false;
  }
  return tp->on_carry_success(g, v, l, me, collector, e);
}

[[nodiscard]] auto thing_carry(Gamep g, Levelsp v, Levelp l, Thingp collector, Thingp item, ThingEvent &e) -> bool
{
  TRACE();

  if (collector == nullptr) {
    ERR("no thing pointer");
    return false;
  }

  if (item == nullptr) {
    thing_err(g, v, l, collector, "no item to carry");
    return false;
  }

  auto s = to_string(g, v, l, item);
  THING_DBG(g, v, l, collector, "drop: %s", s.c_str());
  TRACE_INDENT();

  //
  // Avoid rapid drop/collect loops
  //
  if (item->tick_dropped != 0U) {
    if (item->tick_dropped >= v->tick - 1) {
      return false;
    }
  }

  if (! thing_carry_item(g, v, l, item, collector, e)) {
    THING_DBG(g, v, l, collector, "failed to carry item: %s", s.c_str());
    return false;
  }

  (void) thing_auto_wear_try(g, v, l, collector, item, e);

  return true;
}

[[nodiscard]] auto thing_carry(Gamep g, Levelsp v, Levelp l, Thingp collector, const std::initializer_list< std::string > &items) -> bool
{
  TRACE();

  if (collector == nullptr) {
    ERR("no thing pointer");
    return false;
  }

  bool ok = true;

  for (const auto &tp : items) {
    auto *item_tp = tp_find_mand(tp);
    if (item_tp != nullptr) {
      auto *item = thing_spawn(g, v, l, item_tp, thing_at(g, v, l, collector));
      if (item != nullptr) {
        ThingEvent e {
            .reason     = "spawned",           //
            .event_type = THING_EVENT_SPAWNED, //
            .source     = collector,           //
        };

        if (! thing_carry(g, v, l, collector, item, e)) {
          ok = false;
        }
      }
    }
  }

  return ok;
}

[[nodiscard]] auto thing_carry(Gamep g, Levelsp v, Levelp l, Thingp collector, const std::vector< Thingp > &items) -> bool
{
  TRACE();

  if (collector == nullptr) {
    ERR("no thing pointer");
    return false;
  }

  bool ok = true;

  for (const auto &item : items) {

    ThingEvent e {
        .reason     = "collected",         //
        .event_type = THING_EVENT_SPAWNED, //
        .source     = collector,           //
    };

    if (! thing_carry(g, v, l, collector, item, e)) {
      ok = false;
    }
  }

  return ok;
}
