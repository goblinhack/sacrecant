//
// Copyright goblinhack@gmail.com
//

#ifndef MY_MUSIC_HPP
#define MY_MUSIC_HPP

#include "my_types.hpp"

#include <string>

auto music_init() -> bool;
void music_fini();
auto music_load(Gamep g, uint32_t rate, const char *file, const char *name) -> bool;
auto music_play(Gamep g, const std::string &name) -> bool;
auto music_halt() -> bool;
void music_update_volume(Gamep g);
void music_load(Gamep g);

#endif
