//
// Copyright goblinhack@gmail.com
//

#include "my_dice_class.hpp"
#include "my_random.hpp"
#include "my_string.hpp"

#include <string>

Dice::Dice(void) = default;

[[nodiscard]] auto Dice::to_string() const -> std::string { return hitdice; }

Dice::Dice(const std::string &s)
{
  if (s.empty()) {
    return;
  }

  initialized = true;
  hitdice     = s;

  //
  // 1d6+1
  //
  for (const auto &x : string_split_tokens(s, '+')) {
    auto sp = string_split_tokens(x, 'd');
    if (sp.size() == 2) {
      //
      // 1d6
      // ^
      ndice = std::stoi(sp[ 0 ]);
      //
      // 1d6
      //   ^
      //
      sides = std::stoi(sp[ 1 ]);
    } else {
      //
      // +1
      //
      modifier += std::stoi(sp[ 0 ]);
    }
  }
  // con("new dice [%s] %dd%d+%d", s.c_str(), ndice, sides, modifier);
}

[[nodiscard]] auto Dice::roll() const -> int
{
  if (! initialized) {
    return 0;
  }

  int n   = ndice;
  int tot = 0;
  // con("roll %dd%d+%d", ndice, sides, modifier);
  while (n-- > 0) {
    tot += PCG_RANDOM_RANGE(0, sides) + 1;
  }
  tot += modifier;
  // con("roll %dd%d+%d => %d", ndice, sides, modifier, tot);
  return tot;
}

[[nodiscard]] auto Dice::max_roll() const -> int
{
  if (! initialized) {
    return 0;
  }

  return (ndice * sides) + modifier;
}

[[nodiscard]] auto Dice::min_roll() const -> int
{
  if (! initialized) {
    return 0;
  }

  return (ndice * 1) + modifier;
}
