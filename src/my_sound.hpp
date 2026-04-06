//
// Copyright goblinhack@gmail.com
//

#ifndef MY_SOUND_HPP
#define MY_SOUND_HPP

#include "my_game.hpp"

#include <string>

class Sound;
using soundp = class Sound *;

auto sound_init() -> bool;
void sound_fini();
auto sound_load(float volume, const std::string &file, const std::string &alias, int concurrent_max = 1) -> bool;
auto sound_find(const std::string &alias) -> bool;
auto sound_play(Gamep g, const std::string &alias, float scale = 1.0, int loops = 0) -> bool;
void sounds_load(Gamep g);
void sound_fade_out(Gamep g);

enum {
  CHANNEL_MISC,
  CHANNEL_FOOTSTEPS,
  CHANNEL_IMPACT,
  CHANNEL_MONST,
  CHANNEL_MONST_DEATH,
  CHANNEL_MONST_BIRTH,
  CHANNEL_WEAPON,
  CHANNEL_EXPLOSION,
};

#endif
