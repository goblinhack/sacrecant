//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_game.hpp"
#include "my_level.hpp"
#include "my_main.hpp"
#include "my_thing.hpp"
#include "my_thing_inlines.hpp"
#include "my_tp.hpp"
#include "my_types.hpp"

#include <format>
#include <string>

[[nodiscard]] auto to_string(Gamep g, Levelsp v, Levelp l, Thingp t) -> std::string
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

  auto at = thing_at(g, v, l, t);

  std::string out
      = /* keep ( */ (std::format("{:08x}"
                                  /* level num                     */ " l{}"
                                  /* tick                          */ " t{:3}"
                                  /* thing_health                  */ " h{:<3}"
                                  /* at                            */ " @{:2},{:2}"
                                  /* name                          */ "{}"
                                  /* name                          */ " {}"
                                  /* is_dead                       */ "{}"
                                  /* is_moving                     */ "{}"
                                  /* is_worn                       */ "{}"
                                  /* is_jumping                    */ "{}"
                                  /* is_levitating                 */ "{}"
                                  /* is_thrown                     */ "{}"
                                  /* is_engulfed                   */ "{}"
                                  /* is_ensnared                   */ "{}"
                                  /* is_carried                    */ "{}"
                                  /* is_sleeping                   */ "{}"
                                  /* is_falling                    */ "{}"
                                  /* is_open                       */ "{}"
                                  /* is_burning                    */ "{}"
                                  /* is_scheduled_for_cleanup      */ "{}",
                                  /* newline */ t->id,
                                  /* newline */ t->level_num + 1,
                                  /* newline */ t->tick,
                                  /* newline */ thing_health(g, v, l, t),
                                  /* newline */ at.x, at.y,
                                  /* newline */ t->_is_on_map ? "" : "(nomap)",
                                  /* newline */ name,
                                  /* newline */ thing_is_moving(t) ? "/move" : "",
                                  /* newline */ thing_is_worn(t) ? "/worn" : "",
                                  /* newline */ thing_is_jumping(t) ? "/jump" : "",
                                  /* newline */ thing_is_levitating(g, v, l, t) ? "/lev" : "",
                                  /* newline */ thing_is_thrown(t) ? "/thrw" : "",
                                  /* newline */ thing_is_engulfed(t) ? "/gulf" : "",
                                  /* newline */ thing_is_ensnared(t) ? "/snrd" : "",
                                  /* newline */ thing_is_carried(t) ? "/carr" : "",
                                  /* newline */ thing_is_dead(t) ? "/dead" : "",
                                  /* newline */ thing_is_sleeping(t) ? "/zzz" : "",
                                  /* newline */ (thing_is_falling(t) != 0) ? "/fall" : "",
                                  /* newline */ thing_is_open(t) ? "/open" : "",
                                  /* newline */ thing_is_burning(t) ? "/burn" : "",
                                  /* newline */ thing_is_scheduled_for_cleanup(t) ? "/fre" : ""));

  return out;
}

[[nodiscard]] auto to_string(Gamep g, Levelsp v, Levelp l, ThingEvent &e) -> std::string
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

[[nodiscard]] auto to_death_reason_string(Gamep g, Levelsp v, Levelp l, Thingp t, ThingEvent &e) -> std::string
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
    case THING_EVENT_CRUSH_DAMAGE : //
      s += "crushed";
      break;
    case THING_EVENT_THROWN_DAMAGE : //
      s += "pelted";
      break;
    case THING_EVENT_ENGULF_DAMAGE : //
      s += "digested";
      break;
    case THING_EVENT_SPELL_DAMAGE : //
      s += "blasted to bits";
      break;
    case THING_EVENT_POISON_DAMAGE : //
      s += "poisoned";
      break;
    case THING_EVENT_MELEE_DAMAGE : //
      s += "hacked to bits";
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
    case THING_EVENT_ENERGY_DAMAGE : //
      s += "blasted";
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
    case THING_EVENT_GAME_OVER : //
      s += "became the new even darker dark lord";
      break;
    case THING_EVENT_USER_INITIATED : //
      s += "user initiated";
      break;
    case THING_EVENT_SPAWNED : //
      s += "spawned";
      break;
    case THING_EVENT_LEVITATED : //
      s += "levitated";
      break;
    case THING_EVENT_EATEN : //
      s += "used";
      break;
    case THING_EVENT_USED : //
      s += "eaten";
      break;
    case THING_EVENT_THROWN : //
      s += "thrown";
      break;
    case THING_EVENT_ENUM_MAX : break;
  }

  //
  // Add some more spice to the message
  //
  if (level_is_lava_bool(g, v, l, thing_at(g, v, l, t)) || ((source != nullptr) && thing_is_lava(source))) {
    s += " in lava";
  } else if (level_is_water_deep_bool(g, v, l, thing_at(g, v, l, t)) || ((source != nullptr) && thing_is_water_deep(source))) {
    s += " in the depths";
  } else if (level_is_water_shallow_bool(g, v, l, thing_at(g, v, l, t)) || ((source != nullptr) && thing_is_water_shallow(source))) {
    s += " in a puddle";
  } else if (level_is_steam_bool(g, v, l, thing_at(g, v, l, t)) || ((source != nullptr) && thing_is_steam(source))) {
    s += " in the steam";
  } else if (level_is_smoke_bool(g, v, l, thing_at(g, v, l, t)) || ((source != nullptr) && thing_is_smoke(source))) {
    s += " in the smoke";
  }

  if (source != nullptr) {
    if ((g != nullptr) && thing_is_player(source)) {
      std::string const name = game_player_name_get(g);
      s += " by " + name;
    } else {
      s += " by " + tp_name_a_or_an(thing_tp(source));
    }
  }

  return s;
}
