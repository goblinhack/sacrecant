//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_file.hpp"
#include "my_game.hpp"
#include "my_globals.hpp"
#include "my_main.hpp"
#include "my_music.hpp"
#include "my_ptrcheck.hpp"

#include <SDL_mixer.h>
#include <map>
#include <utility>

class Music
{
public:
  Music(std::string vname_alias) : name_alias(vname_alias) {}

  ~Music()
  {
    if (m) {
      Mix_FreeMusic(m);
    }
    if (rw) {
      SDL_RWclose(rw);
    }
    MYFREE(data);
  }

  std::string name_alias;
  SDL_RWops  *rw   = {};
  Mix_Music  *m    = {};
  uint8_t    *data = {};
  int         len  = {};
  uint32_t    rate = 44100;
};

static std::map< std::string, class Music * > all_music;

static std::string music_current;

static bool music_init_done;

auto music_init() -> bool
{
  TRACE();

  //
  // MP3 is a pain to use, use OGG instead
  // int flags = MIX_INIT_OGG|MIX_INIT_MP3;
  //
  int const flags   = MIX_INIT_OGG;
  int const initted = Mix_Init(flags);
  log("SDL: Load mixer");
  if ((initted & flags) != flags) {
    CROAK("Mix_Init: Failed to init required music support");
  }

  music_init_done = true;
  return true;
}

void music_fini()
{
  log("music fini");
  TRACE();

  if (music_init_done) {
    music_init_done = false;

    auto iter = all_music.begin();

    while (iter != all_music.end()) {
      delete iter->second;
      iter = all_music.erase(iter);
    }

    Mix_Quit();
    log("SDL: Mix_Quit");
  }
  all_music.clear();
}

auto music_load(Gamep g, uint32_t rate, const char *file, const char *name_alias) -> bool
{
  TRACE();

  if ((name_alias != nullptr) && (*name_alias != 0)) {
    auto m = music_find(name_alias);
    if (m) {
      return true;
    }
  }

  auto *m = new Music(name_alias);

  m->rate = rate;
  m->data = file_load(file, &m->len);
  if (m->data == nullptr) {
    CROAK("cannot load music [%s]", file);
    return false;
  }

  m->rw = SDL_RWFromMem(m->data, m->len);
  if (m->rw == nullptr) {
    CROAK("SDL_RWFromMem fail [%s]: %s %s", file, Mix_GetError(), SDL_GetError());
    SDL_ClearError();
    return false;
  }

  m->m = Mix_LoadMUS_RW(m->rw, 0);
  if (m->m == nullptr) {
    CROAK("Mix_LoadMUS_RW fail [%s]: %s %s", file, Mix_GetError(), SDL_GetError());
    SDL_ClearError();
    SDL_RWclose(m->rw);
    delete m;
    return false;
  }

  auto result = all_music.insert(std::make_pair(name_alias, m));
  if (! result.second) {
    CROAK("cannot insert music name [%s]", name_alias);
    SDL_RWclose(m->rw);
    delete m;
    return false;
  }

  return true;
}

/*
 * Find an existing pice of music.
 */
auto music_find(const char *name_alias) -> bool
{
  TRACE();
  auto result = all_music.find(name_alias);
  return result != all_music.end();
}

void music_update_volume(Gamep g)
{
  TRACE();
  Mix_VolumeMusic(game_music_volume_get(g));
  SDL_ClearError();
}

auto music_play(Gamep g, const char *name) -> bool
{
  TRACE();

  if (name == music_current) {
    return true;
  }
  music_current = name;

  auto music = all_music.find(name);
  if (music == all_music.end()) {
    CROAK("cannot find music %s: %s", name, Mix_GetError());
    SDL_ClearError();
    return false;
  }

  if (! music->second) {
    ERR("cannot play music %s: %s (bug)", name, Mix_GetError());
    SDL_ClearError();
    return false;
  }

  if (! music->second->m) {
    ERR("cannot play music %s: %s (bug)", name, Mix_GetError());
    SDL_ClearError();
    return false;
  }

  if (Mix_FadeInMusicPos(music->second->m, -1, 2000, 0) == -1) {
    ERR("cannot play music %s: %s", name, Mix_GetError());
    SDL_ClearError();
    return false;
  }

  return true;
}

auto music_halt() -> bool
{
  TRACE();
  music_current = "";

  Mix_FadeOutMusic(1500);
  return true;
}

void music_load(Gamep g)
{
  TRACE();

  (void) music_load(g, 44100, "data/music/DST-PhaserSwitch.ogg", "intro");
}
