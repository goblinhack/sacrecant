//
// Copyright goblinhack@gmail.com
//

#ifndef MY_LEVEL_EXT_HPP
#define MY_LEVEL_EXT_HPP

#include "my_bpoint.hpp"
#include "my_fpoint.hpp"
#include "my_game.hpp"
#include "my_level.hpp"
#include "my_thing.hpp"
#include "my_tp.hpp"

#include <functional>
#include <map>

//
// Overriding specific characters
//
class LevelGenOverride
{
public:
  std::function< Tpp(char, bpoint) >                                                     get_tp;
  std::function< Thingp(Gamep g, Levelsp v, Levelp l, const bpoint &at, ThingEvent *e) > do_thing_spawn;
};

class LevelGenOverrides
{
public:
  std::map< char, LevelGenOverride > o;
};

extern LevelGenOverrides no_overrides;

void               level_fixed_add(Gamep g, int chance, LevelType level_type, const std::string &alias, const char *file, int line,
                                   const LevelGenOverrides &overrides, int dummy, ...);
[[nodiscard]] auto level_populate(Gamep g, Levelsp v, Levelp l, class LevelGen *lg, int w, int h, const char *in,
                                  const LevelGenOverrides &overrides_in = no_overrides) -> bool;

#endif
