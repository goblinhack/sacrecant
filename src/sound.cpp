//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_file.hpp"
#include "my_game.hpp"
#include "my_globals.hpp"
#include "my_main.hpp"
#include "my_ptrcheck.hpp"
#include "my_sound.hpp"

#include <SDL_mixer.h>
#include <map>
#include <utility>

class Sound
{
public:
  explicit Sound(std::string valias) : alias(std::move(valias)) {}

  ~Sound()
  {
    Mix_FreeChunk(chunk);
    MYFREE(data);
  }

  //
  // Sound name
  //
  std::string alias;

  //
  // How many instances of this sound are allowed to play at the same time.
  //
  int concurrent_max = {1};

  Mix_Chunk *chunk = {};
  uint8_t   *data  = {};
  int        len   = {};
  float      volume {};
};

static std::unordered_map< std::string, class Sound * > all_sound;

static bool sound_init_done;

class Playing
{
public:
  std::string alias;
  int         volume {};
};

static std::unordered_map< int, Playing > already_playing;

static void sound_finished(int channel)
{
  DBG2("Sound channel %d finished", channel);
  if (channel != -1) {
    already_playing[ channel ].alias = "";
  }
}

auto sound_init() -> bool
{
  TRACE();
  Mix_AllocateChannels(16);
  Mix_ChannelFinished(sound_finished);

  sound_init_done = true;

  return true;
}

void sound_fini()
{
  TRACE();

  log("sound fini");

  if (sound_init_done) {
    sound_init_done = false;

    for (;;) {
      auto iter = all_sound.begin();
      if (iter == all_sound.end()) {
        break;
      }
      delete iter->second;
      iter = all_sound.erase(iter);
    }
  }
  all_sound.clear();
}

#if 0
static auto sound_load(float volume, const char *file_in, const char *alias_in) -> bool
{
  TRACE();
  auto file  = std::string(file_in);
  auto alias = std::string(alias_in);

  return sound_load(volume, file, alias);
}
#endif

auto sound_load(float volume, const std::string &file, const std::string &alias, int concurrent_max) -> bool
{
  TRACE();
  if (alias.empty()) {
    auto s = sound_find(alias);
    if (s) {
      return true;
    }
  }

  auto *s = new Sound(alias);

  s->volume         = volume;
  s->concurrent_max = concurrent_max;
  s->data           = file_load(file.c_str(), &s->len);
  if (s->data == nullptr) {
    err("cannot load sound [%s]", file.c_str());
    delete s;
    return false;
  }

  SDL_RWops *rw = nullptr;

  rw = SDL_RWFromMem(s->data, s->len);
  if (rw == nullptr) {
    err("SDL_RWFromMem fail [%s]: %s %s", file.c_str(), Mix_GetError(), SDL_GetError());
    SDL_ClearError();
    delete s;
    return false;
  }

  s->chunk = Mix_LoadWAV_RW(rw, 0 /* A non-zero value mean is will automatically close/free the src for you. */);
  if (s->chunk == nullptr) {
    err("Mix_LoadWAV_RW fail [%s]: %s %s", file.c_str(), Mix_GetError(), SDL_GetError());
    SDL_ClearError();
    SDL_RWclose(rw);
    delete s;
    return false;
  }

  auto result = all_sound.insert(std::make_pair(alias, s));
  if (! result.second) {
    err("cannot insert sound name [%s]", alias.c_str());
    SDL_RWclose(rw);
    delete s;
    return false;
  }

  SDL_RWclose(rw);
  // DBG("Load %s", file.c_str());

  return true;
}

//
// Find an existing pice of sound.
//
auto sound_find(const std::string &alias) -> bool
{
  TRACE();
  auto result = all_sound.find(alias);
  return result != all_sound.end();
}

[[nodiscard]] static auto sound_play_internal(Game *g, const std::string &alias, class Sound *s, float scale) -> bool
{
  TRACE();

  if (s->chunk == nullptr) {
    err("cannot find sound chunk %s", s->alias.c_str());
    return false;
  }

  float volume = s->volume * scale * ((static_cast< float >(game_sound_volume_get(g))) / (static_cast< float >(MIX_MAX_VOLUME)));
  volume *= MIX_MAX_VOLUME;

  if (volume <= 0) {
    return false;
  }

  //
  // Playing already? And louder?
  //
  int count = 0;
  for (const auto &p : already_playing) {
    if ((p.second.alias == alias) && (p.second.volume >= volume)) {
      count++;
    }
  }

  // topcon("%s count %d concurrent_max %d", alias.c_str(), count, s->concurrent_max);
  if (count >= s->concurrent_max) {
    return true;
  }

  Mix_VolumeChunk(s->chunk, static_cast< int >(volume));

  auto chan = Mix_PlayChannel(-1, s->chunk, 0 /* loops */);
  if (chan == -1) {
    DBG("Failed to play sound %s volume %d channel %d: %s", alias.c_str(), static_cast< int >(volume), chan, Mix_GetError());
    return false;
  }

  Playing p;
  p.alias                 = s->alias;
  p.volume                = static_cast< int >(volume);
  already_playing[ chan ] = p;

  DBG("Play sound %s volume %d channel %d", alias.c_str(), static_cast< int >(volume), chan);

  return false;
}

auto sound_play(Gamep g, const std::string &alias, float scale) -> bool
{
  TRACE();

  if (scale <= 0) {
    return false;
  }

  auto sound = all_sound.find(alias);
  if (sound == all_sound.end()) {
    if (! g_opt_tests) {
      err("cannot find sound %s", alias.c_str());
    }
    return false;
  }

  if (sound->second == nullptr) {
    err("cannot find sound data %s", alias.c_str());
    return false;
  }

  return sound_play_internal(g, alias, sound->second, scale);
}

void sound_halt() { TRACE(); }
