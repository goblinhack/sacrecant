//
// Copyright goblinhack@gmail.com
//

#include "my_bpoint.hpp"
#include "my_callstack.hpp"
#include "my_color_defs.hpp"
#include "my_game_popups.hpp"
#include "my_level.hpp"
#include "my_level_inlines.hpp" // NOLINT
#include "my_main.hpp"
#include "my_string.hpp"
#include "my_thing.hpp"
#include "my_thing_callbacks.hpp"
#include "my_thing_inlines.hpp"
#include "my_tp.hpp"
#include "my_types.hpp"
#include "my_ui.hpp"

#include <algorithm>
#include <vector>

//
// The monster missed
//
static void thing_attack_missed_player(Gamep g, Levelsp v, Levelp l, Thingp me, ThingEvent &e)
{
  TRACE();

  auto *it = e.source;

  auto at = thing_at(g, v, l, me);
  game_popup_text_add(g, at.x, at.y, "!", WHITE);

  auto damage_name = e.special_attack.name;

  if (it != nullptr) {
    std::string by_the_thing;
    auto       *fired_by = thing_missile_fired_by_get(g, v, l, it);
    if (fired_by != nullptr) {
      if (fired_by == me) {
        by_the_thing = "your " + thing_name_long(g, v, l, it);
      } else {
        by_the_thing = thing_name_apostrophize_the(g, v, l, fired_by) + " " + thing_name_long(g, v, l, it);
      }
    } else {
      by_the_thing = thing_name_long_the(g, v, l, it);
    }

    topcon(UI_WARN_FMT_STR "%s misses!" UI_RESET_FMT, capitalize_first(by_the_thing).c_str());
  }
}

//
// The player missed
//
static void thing_attack_player_missed(Gamep g, Levelsp v, Levelp l, Thingp it, ThingEvent &e)
{
  TRACE();
  auto *the_player = e.source;

  if (thing_is_monst(it)) {
    auto at = thing_at(g, v, l, it);
    game_popup_text_add(g, at.x, at.y, "miss", WHITE);
  }

  if ((the_player != nullptr) && thing_is_loggable(it)) {
    auto the_thing_name_long = thing_name_long_the(g, v, l, it);
    auto The_thing_name_long = capitalize_first(the_thing_name_long);
    auto by_player           = thing_name_long(g, v, l, the_player);

    topcon("You miss %s.", the_thing_name_long.c_str());
  }
}

//
// We're trying to attack at this tile. What do we hit first?
//
static auto thing_attack_melee(Gamep g, Levelsp v, Levelp l, Thingp attacker, Thingp it, ThingEvent *e_in = nullptr) -> bool
{
  TRACE();

  auto *source     = attacker;
  auto  event_type = THING_EVENT_MELEE_DAMAGE;
  auto  damage     = thing_damage(g, v, l, source, event_type);

  //
  // Digestion damage
  //
  if (thing_is_engulfed(it)) {
    if (thing_is_able_to_engulf(attacker)) {
      //
      // As we're in the melee path, this can crit too!
      //
      event_type = THING_EVENT_ENGULF_DAMAGE;
    }
  }

  ThingEvent e {
      .reason     = "melee",    //
      .event_type = event_type, //
      .damage     = damage,     //
      .source     = source,     //
  };

  if (e_in != nullptr) {
    e = *e_in;
    if (e.source != nullptr) {
      attacker = e.source;
    } else {
      e.source = attacker;
    }
  }

  auto victim_at = thing_at(g, v, l, it);

  //
  // Keep track of where we tried to attack
  //
  attacker->attacked_at = victim_at;

  //
  // Even if the attack fails, make the monster point at the target
  //
  thing_set_dir_from_target(g, v, l, attacker, victim_at);

  //
  // Check not too many attacks
  //
  if (thing_is_monst(attacker) || thing_is_player(attacker)) {
    //
    // NOTE: door slam attack and anything else I've not thought of, bypasses this
    //
    THING_DBG(g, v, l, attacker, "attack");
    if (thing_attack_count_per_tick_incr(g, v, l, attacker) > tp_attack_count_max_per_tick_get(thing_tp(attacker))) {
      THING_DBG(g, v, l, attacker, "exceeded max attack count (%d vs %d), ignore", //
                thing_attack_count_per_tick(attacker),                             //
                tp_attack_count_max_per_tick_get(thing_tp(attacker)));
      return false;
    }
  }

  //
  // Attack or miss
  //
  // The attack modifier, say +4 has to beat the defence, say 10
  // We roll d20 and add 4 .
  //
  auto def = thing_stat(g, v, l, it, THING_STAT_DEF);

  //
  // Passing the event here allows for crit attacks
  //
  auto is_hit = thing_stat_success(g, v, l, attacker, THING_STAT_ATT, def, e);

  if (! is_hit) {
    if (! thing_on_missing(g, v, l, attacker, it, e)) {
      return false;
    }

    if (thing_is_monst(attacker)) {
      // Misses you
      thing_attack_missed_player(g, v, l, it, e);
      return false;
    }
    if (thing_is_player(attacker)) {
      // You miss
      thing_attack_player_missed(g, v, l, it, e);
      return false;
    }
  }

  //
  // Keep track of who attacked us
  //
  if (thing_is_monst(attacker)) {
    if (thing_is_player(it)) {
      auto *p = thing_player_struct(g);
      if (p != nullptr) {
        p->attacked_by[ tp_id_get(thing_tp(attacker)) ]++;
      }
    }
  }

  //
  // Thing callback
  //
  if (! thing_on_attacking(g, v, l, attacker, it, e)) {
    return false;
  }

  thing_damage_apply(g, v, l, it, e);

  if (thing_is_dead(it)) {
    if (! thing_on_killing(g, v, l, attacker, it, e)) {
      return false;
    }
  }

  return true;
}

//
// We're trying to attack at this tile. What do we hit first?
//
[[nodiscard]] auto thing_attack_at(Gamep g, Levelsp v, Levelp l, Thingp attacker, const bpoint &attack_at, ThingEvent *e) -> bool
{
  THING_DBG(g, v, l, attacker, "%s", __FUNCTION__);
  TRACE_INDENT();

  //
  // Only allow attacks on immediately adjacent tiles. Unless you can fire weapons.
  //
  if (thing_is_able_to_fire_weapons(attacker)) {
    //
    // Firing tiles do not need to be adjacent
    //
  } else {
    if (thing_at(g, v, l, attacker) == attack_at) {
      //
      // Allow door slam attack on same tile
      //
    } else if (! adjacent(thing_at(g, v, l, attacker), attack_at)) {
      //
      // Adjacent tile attack
      //
      THING_DBG(g, v, l, attacker, "not adjacent");
      return false;
    }
  }

  std::vector< Thingp > cands;

  FOR_ALL_THINGS_AT_UNSAFE(g, v, l, o, attack_at)
  {
    if (o == attacker) {
      continue;
    }

    if (thing_is_monst(attacker) || ((e != nullptr) && (e->source != nullptr) && thing_is_monst(e->source))) {
      if (thing_is_attackable_by_monst(o)) {
        cands.push_back(o);
      } else if (thing_is_flesh_eater(attacker) && thing_is_flesh(o)) {
        THING_DBG(g, v, l, o, "add flesh candidate");
        cands.push_back(o);
      }
    } else if (thing_is_player(attacker) || ((e != nullptr) && (e->source != nullptr) && thing_is_player(e->source))) {
      if (thing_is_attackable_by_player(o)) {
        cands.push_back(o);
      }
    } else {
      continue;
    }
  }

  if (cands.empty()) {
    return false;
  }

  //
  // Sort by priority
  //
  std::ranges::sort(cands, [](Thingp a, Thingp b) -> bool { return thing_priority(a) < thing_priority(b); });

  if (compiler_unused) {
    for (auto *cand : cands) {
      THING_DBG(g, v, l, cand, "prio %u", thing_priority(cand));
    }
  }

  for (auto *cand : cands) {
    if (thing_is_dead(cand)) {
      if (! thing_is_hit_when_dead(cand)) {
        continue;
      }
    }

    if (thing_attack_melee(g, v, l, attacker, cand, e)) {
      return true;
    }
  }
  return false;
}

[[nodiscard]] auto thing_attack_count_per_tick(Thingp t) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_attack_count_per_tick;
}

[[nodiscard]] auto thing_attack_count_per_tick_set(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_attack_count_per_tick = val;
}

[[nodiscard]] auto thing_attack_count_per_tick_incr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_attack_count_per_tick += val;
}

[[nodiscard]] auto thing_attack_count_per_tick_decr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  if (static_cast< int >(t->_attack_count_per_tick) - val <= 0) {
    return t->_attack_count_per_tick = 0;
  }
  return t->_attack_count_per_tick -= val;
}

[[nodiscard]] auto thing_is_attackable_by_player(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_attackable_by_player) != 0;
}

[[nodiscard]] auto thing_is_attackable_by_monst(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_attackable_by_monst) != 0;
}
