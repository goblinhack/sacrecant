//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_game.hpp"
#include "my_thing.hpp"
#include "my_thing_inlines.hpp"
#include "my_tp.hpp"
#include "my_types.hpp"
#include <string>

[[nodiscard]] auto thing_name_pluralize(Gamep g, Levelsp v, Levelp l, Thingp t) -> std::string
{
  TRACE();

  auto *tp  = thing_tp(t);
  auto  out = tp_name_pluralize(tp);

  if (out.empty()) {
    return tp_name_long(tp) + "s";
  }

  return out;
}

[[nodiscard]] auto thing_name_pluralize_the(Gamep g, Levelsp v, Levelp l, Thingp t) -> std::string
{
  TRACE();
  return "the " + thing_name_pluralize(g, v, l, t);
}

[[nodiscard]] auto thing_name_apostrophize(Gamep g, Levelsp v, Levelp l, Thingp t) -> std::string
{
  TRACE();

  auto *tp  = thing_tp(t);
  auto  out = tp_name_apostrophize(tp);

  if (out.empty()) {
    return tp_name_long(tp) + "'s";
  }

  return out;
}

[[nodiscard]] auto thing_name_apostrophize_the(Gamep g, Levelsp v, Levelp l, Thingp t) -> std::string
{
  TRACE();
  return "the " + thing_name_apostrophize(g, v, l, t);
}

[[nodiscard]] auto thing_name_a_or_an(Thingp t) -> std::string
{
  TRACE();

  auto *tp = thing_tp(t);
  return tp_name_a_or_an(tp);
}

[[nodiscard]] auto thing_name_long(Gamep g, Levelsp v, Levelp l, Thingp t, ThingTextFlags f) -> std::string
{
  TRACE();

  if (t == nullptr) [[unlikely]] {
    return "<no name>";
  }

  auto *tp = thing_tp(t);

  std::string out;

  //
  // "the kobalos's short sword" for example
  //
  auto *t_o = thing_owner(g, v, l, t);
  if ((f & TEXT_INCLUDE_OWNER) != 0) {
    if ((t_o != nullptr) && ! thing_is_player(t_o)) {

      out += tp_name_long(thing_tp(t_o));
      out += "'s ";
    }
  }

  if ((f & TEXT_EXCLUDE_DEATH) == 0) {
    if (thing_is_burning(t)) {
      if (thing_is_dead(t)) {
        out += "burnt ";
      } else {
        out += "burning ";
      }
    }

    if (thing_is_dead(t)) {
      if (thing_is_player(t) || thing_is_monst(t)) {
        if (thing_is_undead(t)) {
          out += "extra dead ";
        } else {
          out += "dead ";
        }
      }
      if (thing_is_broken_on_death(t)) {
        out += "broken ";
      }
      if (thing_is_extinguished_on_death(t)) {
        out += "extinguished ";
      }
    }
  }

  if ((f & TEXT_A_OR_AN) != 0) {
    out += thing_name_a_or_an(t);
  } else if ((f & TEXT_APOSTROPHIZE) != 0) {
    out += thing_name_apostrophize(g, v, l, t);
  } else if ((f & TEXT_PLURALIZE) != 0) {
    out += thing_name_pluralize(g, v, l, t);
  } else {
    auto name = tp_name_long(tp);
    if ((g != nullptr) && thing_is_player(t)) {
      name = game_player_name_get(g);
    }

    out += name;
  }

  return out;
}

[[nodiscard]] auto thing_name_long_the(Gamep g, Levelsp v, Levelp l, Thingp t, ThingTextFlags f) -> std::string
{
  TRACE();

  return "the " + thing_name_long(g, v, l, t, f);
}

[[nodiscard]] auto thing_name_long_The(Gamep g, Levelsp v, Levelp l, Thingp t, ThingTextFlags f) -> std::string
{
  TRACE();

  return "The " + thing_name_long(g, v, l, t, f);
}

[[nodiscard]] auto thing_name_short(Gamep g, Levelsp v, Levelp l, Thingp t, ThingTextFlags f) -> std::string
{
  TRACE();

  if (t == nullptr) [[unlikely]] {
    return "<no name>";
  }

  auto *tp = thing_tp(t);

  std::string out;

  //
  // "the kobalos's short sword" for example
  //
  auto *t_o = thing_owner(g, v, l, t);
  if ((f & TEXT_INCLUDE_OWNER) != 0) {
    if ((t_o != nullptr) && ! thing_is_player(t_o)) {

      out += tp_name_short(thing_tp(t_o));
      out += "'s ";
    }
  }

  if ((f & TEXT_EXCLUDE_DEATH) == 0) {
    if (thing_is_dead(t)) {
      if (thing_is_player(t) || thing_is_monst(t)) {
        if (thing_is_undead(t)) {
          out += "extra dead ";
        } else {
          out += "dead ";
        }
      }
    }
  }

  if ((f & TEXT_A_OR_AN) != 0) {
    out += thing_name_a_or_an(t);
  } else if ((f & TEXT_APOSTROPHIZE) != 0) {
    out += thing_name_apostrophize(g, v, l, t);
  } else if ((f & TEXT_PLURALIZE) != 0) {
    out += thing_name_pluralize(g, v, l, t);
  } else {
    auto name = tp_name_short(tp);
    if ((g != nullptr) && thing_is_player(t)) {
      name = game_player_name_get(g);
    }

    out += name;
  }

  return out;
}

[[nodiscard]] auto thing_name_short_the(Gamep g, Levelsp v, Levelp l, Thingp t, ThingTextFlags f) -> std::string
{
  TRACE();

  return "the " + thing_name_short(g, v, l, t, f);
}
