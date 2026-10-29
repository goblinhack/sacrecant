//
// Copyright goblinhack@gmail.com
//

#include "my_audio.hpp"
#include "my_callstack.hpp"
#include "my_main.hpp"

#include "my_sdl_compat.hpp"
#include <cmath>

static bool audio_init_done;

#ifdef USE_SDL3

//
// SDL3_mixer has a different model to SDL2_mixer; a mixer on an audio device, onto which we attach tracks.
//
static MIX_Mixer *g_mixer;

[[nodiscard]] auto audio_mixer_get() -> MIX_Mixer * { return g_mixer; }

void audio_fini()
{
  log("FINI: audio fini");
  TRACE_INDENT();

  if (g_mixer != nullptr) {
    MIX_DestroyMixer(g_mixer);
    g_mixer = nullptr;
  }
  MIX_Quit();
  audio_init_done = false;
}

[[nodiscard]] auto audio_init() -> bool
{
  TRACE();

  if (! audio_init_done) {
    if (! MIX_Init()) {
      ERR("MIX_Init fail: %s", SDL_GetError());
      SDL_ClearError();
      return false;
    }

    SDL_AudioSpec spec {};
    spec.format   = SDL_AUDIO_S16;
    spec.channels = 2;
    spec.freq     = 44100;

    g_mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec);
    if (g_mixer == nullptr) {
      ERR("MIX_CreateMixerDevice fail: %s", SDL_GetError());
      SDL_ClearError();
      return false;
    }

    audio_init_done = true;
  }

  return true;
}

#else // SDL2

void audio_fini()
{
  log("FINI: audio fini");
  TRACE_INDENT();

  Mix_CloseAudio();
  audio_init_done = 0u;
}

[[nodiscard]] auto audio_init() -> bool
{
  TRACE();

  //
  // Output sampling freq in samples per second (Hz).
  // you might use MIX_DEFAULT_FREQUENCY(22050) since that is a good value
  // for most games.
  //
  int const audio_freq   = MIX_DEFAULT_FREQUENCY;
  int const audio_format = MIX_DEFAULT_FORMAT;

  //
  // Number of sound channels in output.
  // Set to 2 for stereo, 1 for mono. This has nothing to do with
  // mixing channels.
  //
  int const channels = 2;

  //
  // chunksize is the size of each mixed sample. The smaller this is the
  // more your hooks will be called. If make this too small on a slow
  // system, sound may skip. If made to large, sound effects will lag
  // behind the action more. You want a happy medium for your target
  // computer. You also may make this 4096, or larger, if you are just
  // playing music.
  //
  // int chunksize = 4096;
  int const chunksize = 1024; // less latency but might be choppy

  if (! audio_init_done) {
    if (Mix_OpenAudio(audio_freq, audio_format, channels, chunksize) != 0) {
      ERR("Mix_OpenAudio fail: %s %s", Mix_GetError(), SDL_GetError());
      SDL_ClearError();
      return false;
    }

    audio_init_done = 1u;
  }

  return true;
}

#endif // USE_SDL3
