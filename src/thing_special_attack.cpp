//
// Copyright goblinhack@gmail.com
//

#include "my_bpoint.hpp"
#include "my_callstack.hpp"
#include "my_dice_rolls.hpp"
#include "my_main.hpp"
#include "my_thing.hpp"
#include "my_thing_inlines.hpp"
#include "my_tp.hpp"
#include "my_tp_class.hpp"
#include "my_types.hpp"

#include <utility>
#include <vector>

auto thing_special_attack_get_random(Gamep g, Levelsp v, Levelp l, Thingp attacker, Thingp it_maybe_null, TpSpecialAttack &out) -> bool
{
  TRACE();

  if (attacker == nullptr) [[unlikely]] {
    ERR("no thing pointer");
    return false;
  }

  auto *tp = thing_tp(attacker);

  if (tp->special_attacks.empty()) {
    return false;
  }

  auto dice_roll = d100();

  std::vector< TpSpecialAttack > filtered;

  //
  // Check for things matching the dice roll first.
  //
  for (const auto &d : tp->special_attacks) {
    auto val = d.second;

    if (it_maybe_null != nullptr) {
      if (val.when_adjacent) {
        auto target = thing_at(g, v, l, it_maybe_null);
        if (! adjacent(thing_at(g, v, l, attacker), target)) {
          continue;
        }
      }

      if (val.when_distant) {
        auto target = thing_at(g, v, l, it_maybe_null);
        if (distance(thing_at(g, v, l, attacker), target) <= 1) {
          continue;
        }
      }
    }

    filtered.push_back(val);
  }

  //
  // Check for things matching the dice roll first.
  //
  for (const auto &d : filtered) {
    if (d.d100 == 0U) {
      continue;
    }

    if (std::cmp_less(dice_roll, d.d100)) {
      out = d;
      return true;
    }
  }

  //
  // Fallback to any valid default attack
  //
  for (const auto &d : filtered) {
    if (d.d100 != 0U) {
      continue;
    }

    out = d;
    return true;
  }

  return false;
}

auto thing_special_attack_get_all(Gamep g, Levelsp v, Levelp l, Thingp me) -> std::vector< TpSpecialAttack >
{
  TRACE();

  std::vector< TpSpecialAttack > out;

  if (me == nullptr) [[unlikely]] {
    ERR("no thing pointer");
    return out;
  }

  auto *tp = thing_tp(me);

  for (auto i : tp->special_attacks) {
    out.push_back(i.second);
  }

  return out;
}
