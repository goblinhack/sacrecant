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

void thing_on_eaten_set(Tpp tp, thing_on_eaten_t callback)
{
  TRACE();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return;
  }
  tp->on_eaten = callback;
}

[[nodiscard]] auto thing_on_eaten(Gamep g, Levelsp v, Levelp l, Thingp item, Thingp eater) -> bool
{
  TRACE();
  auto *tp = thing_tp(item);
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return false;
  }
  if (tp->on_eaten == nullptr) {
    return false;
  }
  if (! thing_is_player(eater) && ! thing_is_monst(eater)) {
    thing_err(g, v, l, eater, "unexpected thing for %s", __FUNCTION__);
    return false;
  }
  return tp->on_eaten(g, v, l, item, eater);
}

//
// Use an item an remove it from the inventory if dead
//
static auto thing_eat_item(Gamep g, Levelsp v, Levelp l, Thingp item, Thingp eater, ThingEvent &e) -> bool
{
  TRACE();

  if (! thing_is_edible(item)) {
    thing_err(g, v, l, eater, "unexpected non item thing, %s", __FUNCTION__);
    return false;
  }

  if (! thing_is_carried(item)) {
    thing_err(g, v, l, eater, "unexpected uncarried thing, %s", __FUNCTION__);
    return false;
  }

  if (! thing_is_player(eater) && ! thing_is_monst(eater)) {
    thing_err(g, v, l, eater, "unexpected thing, %s", __FUNCTION__);
    return false;
  }

  if (thing_is_ethereal(g, v, l, eater) || thing_is_wall_walker(g, v, l, eater)) {
    if (thing_is_player(eater)) {
      topcon(UI_WARN_FMT_STR "You try to eat, but it passes right through you.\n");
    }

    auto new_event       = e;
    new_event.event_type = THING_EVENT_EATEN;

    (void) thing_drop(g, v, l, eater, item, new_event);

    if (thing_is_tick_on_eaten(item)) {
      (void) level_tick_begin_requested(g, v, l, "failed to eat");
    }

    return false;
  }

  {
    auto s = to_string(g, v, l, item);
    THING_DBG(g, v, l, eater, "eat: %s", s.c_str());
  }
  TRACE_INDENT();

  if (! thing_on_eaten(g, v, l, item, eater)) {
    if (thing_is_player(eater)) {
      if (e.event_type == THING_EVENT_USER_INITIATED) {
        auto the_thing = thing_name_long_the(g, v, l, item);
        topcon(UI_WARN_FMT_STR "You failed to eat %s." UI_RESET_FMT, the_thing.c_str());
      }
    }

    if (thing_is_tick_on_eaten(item)) {
      (void) level_tick_begin_requested(g, v, l, "failed to eat");
    }
    return false;
  }

  {
    auto s = to_string(g, v, l, item);
    THING_DBG(g, v, l, eater, "eat: %s", s.c_str());
  }
  TRACE_INDENT();

  //
  // This should either remove the thing or decrease the slot count
  //
  {
    auto new_event       = e;
    new_event.event_type = THING_EVENT_EATEN;

    (void) thing_drop(g, v, l, eater, item, new_event);

    if (thing_inventory_get_item_count(g, v, l, item, eater) == -1) {
      thing_dead(g, v, l, item, new_event);
    }
  }

  if (thing_is_player(eater)) {
    if (e.event_type == THING_EVENT_USER_INITIATED) {
      auto the_thing = thing_name_long_the(g, v, l, item);
      topcon("You ate %s.", the_thing.c_str());
    }
    game_request_to_remake_ui_set(g);

    if (thing_is_tick_on_eaten(item)) {
      (void) level_tick_begin_requested(g, v, l, "player ate an item");
    }
  }

  return true;
}

[[nodiscard]] auto thing_eat(Gamep g, Levelsp v, Levelp l, Thingp user, Thingp item, ThingEvent &e) -> bool
{
  TRACE();

  if (user == nullptr) {
    ERR("no thing pointer");
    return false;
  }

  if (item == nullptr) {
    thing_err(g, v, l, user, "no item to eat");
    return false;
  }

  return thing_eat_item(g, v, l, item, user, e);
}
