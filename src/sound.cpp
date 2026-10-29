//
// Copyright goblinhack@gmail.com
//

#include "my_audio.hpp"
#include "my_callstack.hpp"
#include "my_cpp_template.hpp"
#include "my_file.hpp"
#include "my_game.hpp"
#include "my_globals.hpp"
#include "my_main.hpp"
#include "my_ptrcheck.hpp"
#include "my_sound.hpp"
#include "my_thing.hpp"
#include "my_types.hpp"

#include "my_sdl_compat.hpp"
#include <array>
#include <cmath>
#include <cstdint>
#include <map>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

static const auto max_channels = 16;

class Sound
{
public:
  Sound() = default;

  ~Sound()
  {
#ifdef USE_SDL3
    if (chunk != nullptr) {
      MIX_DestroyAudio(chunk);
    }
#else
    if (chunk != nullptr) {
      Mix_FreeChunk(chunk);
    }
#endif
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
  SDL_RWops  *rw = {};
#ifdef USE_SDL3
  MIX_Audio *chunk = {};
#else
  Mix_Chunk *chunk = {};
#endif
  uint8_t *data = {};
  int      len  = {};
  float    volume {};
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

#ifdef USE_SDL3
//
// SDL3_mixer has no channels; we make our own, each being a track.
//
static std::array< MIX_Track *, max_channels > tracks {};
#endif

static void sound_finished(int channel)
{
  //
  // Warning, not thread safe
  //
  //  DBG2("Sound channel %d finished", channel);
  if (channel != -1) {
    already_playing[ channel ].name_alias = "";
  }
}

#ifdef USE_SDL3
static void sound_track_stopped(void *userdata, MIX_Track * /*track*/)
{
  sound_finished(static_cast< int >(reinterpret_cast< intptr_t >(userdata)));
}
#endif

[[nodiscard]] auto sound_init() -> bool
{
  TRACE();
#ifdef USE_SDL3
  for (auto c = 0; c < max_channels; c++) {
    tracks[ c ] = MIX_CreateTrack(audio_mixer_get());
    if (tracks[ c ] == nullptr) {
      ERR("MIX_CreateTrack fail: %s", SDL_GetError());
      SDL_ClearError();
      return false;
    }
    MIX_SetTrackStoppedCallback(tracks[ c ], sound_track_stopped, reinterpret_cast< void * >(static_cast< intptr_t >(c)));
  }
#else
  Mix_AllocateChannels(max_channels);
  Mix_ChannelFinished(sound_finished);
#endif

  sound_init_done = true;

  return true;
}

void sound_fade_out(Gamep g)
{
  TRACE();

  return;
  for (auto c = 0; c < max_channels; c++) {
#ifdef USE_SDL3
    MIX_StopTrack(tracks[ c ], MIX_TrackMSToFrames(tracks[ c ], 1000));
#else
    Mix_FadeOutChannel(c, 1000);
#endif
  }
}

void sound_fini()
{
  TRACE();

  log("FINI: sound fini");

  if (sound_init_done) {
    sound_init_done = false;

#ifdef USE_SDL3
    //
    // Stop and release the tracks before the sounds they may be playing.
    //
    for (auto &track : tracks) {
      if (track != nullptr) {
        MIX_DestroyTrack(track);
        track = nullptr;
      }
    }
#endif

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

[[nodiscard]] auto sound_load(float volume, const std::string &name, const std::string &name_alias, int concurrent_max) -> bool
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

#ifdef USE_SDL3
  m->chunk = MIX_LoadAudio_IO(audio_mixer_get(), m->rw, true /* predecode */, false /* we close the stream */);
#else
  m->chunk = Mix_LoadWAV_RW(m->rw, 0 /* A non-zero value mean is will automatically close/free the src for you. */);
#endif
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

[[nodiscard]] static auto sound_play_internal(Gamep g, Levelsp v, Levelp l, const std::string &name_alias, class Sound *m, float scale,
                                              int loops, Thingp me) -> bool
{
  TRACE();

  if (m->chunk == nullptr) {
    if (me != nullptr) {
      thing_err(g, v, l, me, "cannot find sound chunk %s", m->name_alias.c_str());
    } else {
      ERR("cannot find sound chunk %s", m->name_alias.c_str());
    }
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

#ifdef USE_SDL3
  //
  // Find a free track; this is what Mix_PlayChannel(-1,...) did for us.
  //
  int chan = -1;
  for (auto c = 0; c < max_channels; c++) {
    if ((tracks[ c ] != nullptr) && ! MIX_TrackPlaying(tracks[ c ])) {
      chan = c;
      break;
    }
  }

  if (chan != -1) {
    auto            *track = tracks[ chan ];
    SDL_PropertiesID props = SDL_CreateProperties();
    SDL_SetNumberProperty(props, MIX_PROP_PLAY_LOOPS_NUMBER, loops);
    if (! MIX_SetTrackAudio(track, m->chunk) || ! MIX_SetTrackGain(track, volume / static_cast< float >(MIX_MAX_VOLUME))
        || ! MIX_PlayTrack(track, props)) {
      chan = -1;
    }
    SDL_DestroyProperties(props);
  }
#else
  Mix_VolumeChunk(m->chunk, static_cast< int >(volume));

  auto chan = Mix_PlayChannel(-1, m->chunk, loops);
#endif
  if (chan == -1) {
    if (me != nullptr) {
      THING_DBG(g, v, l, me, "could not play sound %s volume %d channel %d: %s", name_alias.c_str(), static_cast< int >(volume), chan,
                Mix_GetError());
    } else {
      DBG("could not play sound %s volume %d channel %d: %s", name_alias.c_str(), static_cast< int >(volume), chan, Mix_GetError());
    }
    return false;
  }

  Playing p;
  p.name_alias            = m->name_alias;
  p.volume                = static_cast< int >(volume);
  already_playing[ chan ] = p;

  if (me != nullptr) {
    THING_DBG(g, v, l, me, "play sound %s volume %d channel %d", name_alias.c_str(), static_cast< int >(volume), chan);
  } else {
    DBG("play sound %s volume %d channel %d", name_alias.c_str(), static_cast< int >(volume), chan);
  }

  return false;
}

[[nodiscard]] auto sound_play(Gamep g, Levelsp v, Levelp l, const std::string &name_alias, float scale, int loops, Thingp me) -> bool
{
  TRACE();

  if (scale <= 0) {
    return false;
  }

  if (game_state(g) == STATE_QUITTING) {
    return false;
  }

  auto *sound = find_one(name_alias);
  if (sound == nullptr) {
    if (! g_opt_tests && ! g_opt_do_level_gen && ! g_opt_do_level_select_gen && ! g_opt_do_room_gen) {
      if (me != nullptr) {
        thing_err(g, v, l, me, "cannot find sound %s", name_alias.c_str());
      } else {
        ERR("cannot find sound %s", name_alias.c_str());
      }
    }
    return false;
  }

  if (g_opt_tests || g_opt_do_level_gen || g_opt_do_level_select_gen || g_opt_do_room_gen) {
    return true;
  }

  return sound_play_internal(g, v, l, name_alias, sound, scale, loops, me);
}

[[nodiscard]] auto sound_play(Gamep g, const std::string &name_alias, float scale, int loops) -> bool
{
  TRACE();

  if (scale <= 0) {
    return false;
  }

  if (game_state(g) == STATE_QUITTING) {
    return false;
  }

  auto *sound = find_one(name_alias);
  if (sound == nullptr) {
    if (! g_opt_tests && ! g_opt_do_level_gen && ! g_opt_do_level_select_gen && ! g_opt_do_room_gen) {
      ERR("cannot find sound %s", name_alias.c_str());
    }
    return false;
  }

  if (g_opt_tests || g_opt_do_level_gen || g_opt_do_level_select_gen || g_opt_do_room_gen) {
    return true;
  }

  return sound_play_internal(g, nullptr, nullptr, name_alias, sound, scale, loops, nullptr);
}
