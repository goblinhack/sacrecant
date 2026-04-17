//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_cpp_template.hpp"
#include "my_file.hpp"
#include "my_game.hpp"
#include "my_globals.hpp"
#include "my_main.hpp"
#include "my_ptrcheck.hpp"
#include "my_sound.hpp"

#include <SDL_mixer.h>
#include <map>
#include <utility>
#include <vector>

static const auto max_channels = 16;

class Sound
{
public:
  Sound() = default;

  ~Sound()
  {
    if (chunk != nullptr) {
      Mix_FreeChunk(chunk);
    }
    if (rw != nullptr) {
      SDL_RWclose(rw);
    }
    MYFREE(data);
  }

  //
  // How many instances of this sound are allowed to play at the same time.
  //
  int concurrent_max = {1};

  std::string name_alias;
  std::string name;
  SDL_RWops  *rw    = {};
  Mix_Chunk  *chunk = {};
  uint8_t    *data  = {};
  int         len   = {};
  float       volume {};
};

static std::multimap< std::string, class Sound * > all;

static bool sound_init_done;

class Playing
{
public:
  std::string name_alias;
  int         volume {};
};

static std::unordered_map< int, Playing > already_playing;

static void sound_finished(int channel)
{
  DBG2("Sound channel %d finished", channel);
  if (channel != -1) {
    already_playing[ channel ].name_alias = "";
  }
}

auto sound_init() -> bool
{
  TRACE();
  Mix_AllocateChannels(max_channels);
  Mix_ChannelFinished(sound_finished);

  sound_init_done = true;

  return true;
}

void sound_fade_out(Gamep g)
{
  TRACE();

  for (auto c = 0; c < max_channels; c++) {
    Mix_FadeOutChannel(c, 1000);
  }
}

void sound_fini()
{
  TRACE();

  log("sound fini");

  if (sound_init_done) {
    sound_init_done = false;

    for (;;) {
      auto iter = all.begin();
      if (iter == all.end()) {
        break;
      }
      delete iter->second;
      iter = all.erase(iter);
    }
  }
  all.clear();
}

static auto find_one(const std::string &name_alias) -> Sound *
{
  TRACE();

  std::vector< Sound * > out;

  for (auto [ itr, rangeEnd ] = all.equal_range(name_alias); itr != rangeEnd; ++itr) {
    out.push_back(itr->second);
  }

  if (out.empty()) {
    return nullptr;
  }

  return rand_one_of(out);
}

auto sound_load(float volume, const std::string &name, const std::string &name_alias, int concurrent_max) -> bool
{
  TRACE();

  auto *m = new Sound();

  m->name_alias     = name_alias;
  m->name           = name;
  m->volume         = volume;
  m->concurrent_max = concurrent_max;
  m->data           = file_load(name.c_str(), &m->len);
  if (m->data == nullptr) {
    ERR("cannot load sound [%s]", name.c_str());
    delete m;
    return false;
  }

  m->rw = SDL_RWFromMem(m->data, m->len);
  if (m->rw == nullptr) {
    ERR("SDL_RWFromMem fail [%s]: %s %s", name.c_str(), Mix_GetError(), SDL_GetError());
    SDL_ClearError();
    delete m;
    return false;
  }

  m->chunk = Mix_LoadWAV_RW(m->rw, 0 /* A non-zero value mean is will automatically close/free the src for you. */);
  if (m->chunk == nullptr) {
    ERR("Mix_LoadWAV_RW fail [%s]: %s %s", name.c_str(), Mix_GetError(), SDL_GetError());
    SDL_ClearError();
    delete m;
    return false;
  }

  all.insert(std::make_pair(name_alias, m));

  // DBG("Load %s", name.c_str());

  return true;
}

[[nodiscard]] static auto sound_play_internal(Game *g, const std::string &name_alias, class Sound *m, float scale, int loops) -> bool
{
  TRACE();

  if (m->chunk == nullptr) {
    ERR("cannot find sound chunk %s", m->name_alias.c_str());
    return false;
  }

  float volume = m->volume * scale * ((static_cast< float >(game_sound_volume_get(g))) / (static_cast< float >(MIX_MAX_VOLUME)));
  volume *= MIX_MAX_VOLUME;

  if (volume <= 0) {
    return false;
  }

  //
  // Playing already? And louder?
  //
  int count = 0;
  for (const auto &p : already_playing) {
    if ((p.second.name_alias == name_alias) && (p.second.volume >= volume)) {
      count++;
    }
  }

  // topcon("%s count %d concurrent_max %d", name_alias.c_str(), count, m->concurrent_max);
  if (count >= m->concurrent_max) {
    return true;
  }

  Mix_VolumeChunk(m->chunk, static_cast< int >(volume));

  auto chan = Mix_PlayChannel(-1, m->chunk, loops);
  if (chan == -1) {
    DBG("Failed to play sound %s volume %d channel %d: %s", name_alias.c_str(), static_cast< int >(volume), chan, Mix_GetError());
    return false;
  }

  Playing p;
  p.name_alias            = m->name_alias;
  p.volume                = static_cast< int >(volume);
  already_playing[ chan ] = p;

  DBG("Play sound %s volume %d channel %d", name_alias.c_str(), static_cast< int >(volume), chan);

  return false;
}

auto sound_play(Gamep g, const std::string &name_alias, float scale, int loops) -> bool
{
  TRACE();

  if (scale <= 0) {
    return false;
  }

  auto *sound = find_one(name_alias);
  if (sound == nullptr) {
    if (! g_opt_tests && ! g_opt_do_level_gen && ! g_opt_do_level_select_gen && ! g_opt_do_room_gen) {
      ERR("cannot find sound %s", name_alias.c_str());
    }
    return false;
  }

  return sound_play_internal(g, name_alias, sound, scale, loops);
}
