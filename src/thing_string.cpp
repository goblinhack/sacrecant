//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_game.hpp"
#include "my_globals.hpp"
#include "my_main.hpp"
#include "my_sprintf.hpp"
#include "my_thing_inlines.hpp"

#include <print>

auto to_string(Gamep g, Levelsp v, Levelp l, Thingp t) -> std::string
{
  TRACE();

  if (t == nullptr) {
    ERR("no thing pointer");
    return "<no thing>";
  }

  auto *tp = thing_tp(t);
  if (t == nullptr) {
    ERR("no thing template pointer");
    return "<no tp>";
  }

  auto name = tp_name(tp);
  if ((g != nullptr) && thing_is_player(t)) {
    name = game_player_name_get(g);
  }

  auto at = thing_at(t);

  return /* keep ( */ (
      std::format("{:08x}"
                  /* level num                     */ " l{}"
                  /* tick                          */ " t{:3}"
                  /* thing_health                  */ " h{:<3}"
                  /* at                            */ " @{:2},{:2}"
                  /* name                          */ " {}"
                  /* is_dead                       */ "{}"
                  /* is_moving                     */ "{}"
                  /* is_sleeping                   */ "{}"
                  /* is_falling                    */ "{}"
                  /* is_open                       */ "{}"
                  /* is_burning                    */ "{}"
                  /* is_scheduled_for_cleanup      */ "{}",
                  /* newline */ t->id,
                  /* newline */ t->level_num + 1,
                  /* newline */ t->tick,
                  /* newline */ thing_health(t),
                  /* newline */ at.x, at.y,
                  /* newline */ name,
                  /* newline */ thing_is_moving(t) ? "/mv" : "",
                  /* newline */ thing_is_dead(t) ? "/de" : "",
                  /* newline */ thing_is_sleeping(t) ? "/sl" : "",
                  /* newline */ (thing_is_falling(t) != 0) ? "/fl" : "",
                  /* newline */ thing_is_open(t) ? "/op" : "",
                  /* newline */ thing_is_burning(t) ? "/bn" : "",
                  /* newline */ thing_is_scheduled_for_cleanup(t) ? "/fre" : ""));
}

auto to_string(Gamep g, Levelsp v, Levelp l, ThingEvent &e) -> std::string
{
  TRACE();

  std::string s = "ev:";

  if (! e.reason.empty()) {
    s += " r:'";
    s += e.reason;
    s += "'";
  }

  if (e.damage != THING_EVENT_NONE) {
    s += " e:";
    s += ThingEventType_to_string(e.event_type);
  }

  if (e.damage != 0) {
    s += " d:";
    s += std::to_string(e.damage);
  }

  if (e.source != nullptr) {
    s += " src:(";
    s += to_string(g, v, l, e.source);
    s += ")";
  }

  return s;
}

auto to_death_reason_string(Gamep g, Levelsp v, Levelp l, Thingp t, ThingEvent &e) -> std::string
{
  TRACE();

  std::string s;
  auto       *source = e.source;

  switch (e.event_type) {
    case THING_EVENT_NONE : break;
    case THING_EVENT_FALL : //
      s += "fell to their death";
      break;
    case THING_EVENT_LIFESPAN_EXPIRED : //
      s += "ran out of life";
      break;
    case THING_EVENT_SHOVED : //
      s += "shoved and died";
      break;
    case THING_EVENT_CRUSH : //
      s += "crushed";
      break;
    case THING_EVENT_MELEE_DAMAGE : //
      s += "hacked to bits";
      break;
    case THING_EVENT_HEAT_DAMAGE : //
      s += "cooked";
      break;
    case THING_EVENT_WATER_DAMAGE : //
      s += "drowned";
      break;
    case THING_EVENT_EXPLOSION_DAMAGE : //
      s += "blown up";
      break;
    case THING_EVENT_FIRE_DAMAGE : //
      s += "burned";
      break;
    case THING_EVENT_OPEN : //
      s += "opened";
      break;
    case THING_EVENT_CARRIED : //
      s += "carried";
      break;
    case THING_EVENT_MELT : //
      s += "melted";
      break;
    case THING_EVENT_CARRIED_MERGED : //
      s += "carried-merged";
      break;
    case THING_EVENT_ENUM_MAX : break;
  }

  //
  // Add some more spice to the message
  //
  if (level_is_lava(g, v, l, thing_at(t)) != nullptr) {
    if ((source == nullptr) || ! thing_is_lava(source)) {
      s += " in lava";
    }
  } else if (level_is_deep_water(g, v, l, thing_at(t)) != nullptr) {
    if ((source == nullptr) || ! thing_is_water(source)) {
      s += " in the depths";
    }
  } else if (level_is_water(g, v, l, thing_at(t)) != nullptr) {
    if ((source == nullptr) || ! thing_is_water(source)) {
      s += " in a puddle";
    }
  }

  if (source != nullptr) {
    if (thing_is_lava(source) || thing_is_water(source)) {
      s += " in " + tp_name_long(thing_tp(source));
    } else if ((g != nullptr) && thing_is_player(source)) {
      std::string const name = game_player_name_get(g);
      s += " by " + name;
    } else {
      s += " by " + tp_name_a_or_an(thing_tp(source));
    }
  }

  return s;
}
