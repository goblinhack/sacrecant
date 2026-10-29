//
// Copyright goblinhack@gmail.com
//

#ifndef MY_AUDIO_HPP
#define MY_AUDIO_HPP

#ifdef USE_SDL3
struct MIX_Mixer;
[[nodiscard]] auto audio_mixer_get() -> MIX_Mixer *;
#endif

[[nodiscard]] auto audio_init() -> bool;
auto               audio_fini() -> void;

#endif
