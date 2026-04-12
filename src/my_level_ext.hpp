//
// Copyright goblinhack@gmail.com
//

#ifndef MY_LEVEL_EXT_HPP
#define MY_LEVEL_EXT_HPP

#include "my_bpoint.hpp"
#include "my_level.hpp"
#include "my_tp.hpp"

#include <functional>
#include <map>

//
// Overriding specific characters
//
using Override  = std::function< Tpp(char, bpoint) >;
using Overrides = std::map< char, Override >;

extern Overrides no_overrides;

void               level_fixed_add(Gamep g, int chance, LevelType level_type, const std::string &alias, const char *file, int line,
                                   const Overrides &overrides, int dummy, ...);
[[nodiscard]] auto level_populate(Gamep g, Levelsp v, Levelp l, class LevelGen *lg, const char *in,
                                  const Overrides &overrides = no_overrides) -> bool;
[[nodiscard]] auto level_populate(Gamep g, Levelsp v, Levelp l, class LevelGen *lg, int w, int h, const char *in,
                                  const Overrides &overrides = no_overrides) -> bool;

#endif
