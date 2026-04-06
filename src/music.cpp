//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_cpp_template.hpp"
#include "my_file.hpp"
#include "my_game.hpp"
#include "my_globals.hpp"
#include "my_main.hpp"
#include "my_music.hpp"
#include "my_ptrcheck.hpp"

#include <SDL_mixer.h>
#include <map>
#include <utility>
#include <vector>

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
  std::string name;
  SDL_RWops  *rw   = {};
  Mix_Music  *m    = {};
  uint8_t    *data = {};
  int         len  = {};
  uint32_t    rate = 44100;
};

static std::vector< class Music * > all_music;

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

    for (auto m : all_music) {
      delete m;
    }

    Mix_Quit();
    log("SDL: Mix_Quit");
  }
  all_music.clear();
}

auto music_load(Gamep g, uint32_t rate, const char *file, const char *name_alias) -> bool
{
  TRACE();

  auto *m = new Music(name_alias);

  m->name = file;
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

  all_music.push_back(m);

  return true;
}

void music_update_volume(Gamep g)
{
  TRACE();
  Mix_VolumeMusic(game_music_volume_get(g));
  SDL_ClearError();
}

auto music_play(Gamep g, const std::string &name) -> bool
{
  TRACE();

  if (name == music_current) {
    return true;
  }
  music_current = name;

  std::vector< class Music * > cand;

  for (auto m : all_music) {
    if (m->name_alias == name) {
      cand.push_back(m);
    }
  }

  if (cand.empty()) {
    if (! g_opt_tests) {
      ERR("cannot find music %s", name.c_str());
    }
    return false;
  }

  auto m = rand_one_of(cand);

  if (Mix_FadeInMusicPos(m->m, -1, 2000, 0) == -1) {
    ERR("cannot play music %s: %s", name.c_str(), Mix_GetError());
    SDL_ClearError();
    return false;
  }

  topcon("Playing %s", m->name.c_str());

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

  (void) music_load(g, 44100, "data/music/DST-Razornest.ogg", /*          */ "intro");
  (void) music_load(g, 44100, "data/music/DST-EndingCredits.ogg", /*      */ "end");
  (void) music_load(g, 44100, "data/music/DST-Incantation.ogg", /*        */ "dead");

  (void) music_load(g, 44100, "data/music/DST-0mnis.ogg", /*              */ "dungeon.1");
  (void) music_load(g, 44100, "data/music/DST-OldCavern.ogg", /*          */ "dungeon.1");
  (void) music_load(g, 44100, "data/music/DST-Escape.ogg", /*             */ "dungeon.1");
  (void) music_load(g, 44100, "data/music/DST-OmegaCenturion.ogg", /*     */ "dungeon.1");
  (void) music_load(g, 44100, "data/music/DST-Arch-Delerium.ogg", /*      */ "dungeon.2");
  (void) music_load(g, 44100, "data/music/DST-Psykick.ogg", /*            */ "dungeon.2");
  (void) music_load(g, 44100, "data/music/DST-Frontier.ogg", /*           */ "dungeon.2");
  (void) music_load(g, 44100, "data/music/DST-DoomCity.ogg", /*           */ "dungeon.3");
  (void) music_load(g, 44100, "data/music/DST-PurgatoryStep.ogg", /*      */ "dungeon.3");
  (void) music_load(g, 44100, "data/music/DST-Legends.ogg", /*            */ "dungeon.3");
  (void) music_load(g, 44100, "data/music/DST-DasElectron.ogg", /*        */ "dungeon.4");
  (void) music_load(g, 44100, "data/music/DST-NoSleep.ogg", /*            */ "dungeon.4");
  (void) music_load(g, 44100, "data/music/DST-OffWorld.ogg", /*           */ "dungeon.4");
  (void) music_load(g, 44100, "data/music/DST-OrganicSpace.ogg", /*       */ "dungeon.4");
  (void) music_load(g, 44100, "data/music/DST-Perchlorate.ogg", /*        */ "dungeon.4");
  (void) music_load(g, 44100, "data/music/DST-Protector.ogg", /*          */ "dungeon.4");
  (void) music_load(g, 44100, "data/music/DST-BloodAndIron.ogg", /*       */ "dungeon.boss");

  (void) music_load(g, 44100, "data/music/DST-Petaluna.ogg", /*           */ "bogland.1");
  (void) music_load(g, 44100, "data/music/DST-DragonFly.ogg", /*          */ "bogland.1");
  (void) music_load(g, 44100, "data/music/DST-Lightness.ogg", /*          */ "bogland.1");
  (void) music_load(g, 44100, "data/music/DST-Drogs.ogg", /*              */ "bogland.2");
  (void) music_load(g, 44100, "data/music/DST-MelodyB.ogg", /*            */ "bogland.2");
  (void) music_load(g, 44100, "data/music/DST-Echorex.ogg", /*            */ "bogland.3");
  (void) music_load(g, 44100, "data/music/DST-Eretria.ogg", /*            */ "bogland.4");
  (void) music_load(g, 44100, "data/music/DST-Nvision.ogg", /*            */ "bogland.4");
  (void) music_load(g, 44100, "data/music/DST-ElectriKaos.ogg", /*        */ "bogland.boss");

  (void) music_load(g, 44100, "data/music/DST-DFree.ogg", /*              */ "nethervoid.1");
  (void) music_load(g, 44100, "data/music/DST-ExoPlanet.ogg", /*          */ "nethervoid.1");
  (void) music_load(g, 44100, "data/music/DST-Digitalus.ogg", /*          */ "nethervoid.2");
  (void) music_load(g, 44100, "data/music/DST-Expander.ogg", /*           */ "nethervoid.2");
  (void) music_load(g, 44100, "data/music/DST-DreamOfForever.ogg", /*     */ "nethervoid.3");
  (void) music_load(g, 44100, "data/music/DST-FrozenJellyFish.ogg", /*    */ "nethervoid.3");
  (void) music_load(g, 44100, "data/music/DST-CyberOps.ogg", /*           */ "nethervoid.4");
  (void) music_load(g, 44100, "data/music/DST-impuretechnology.ogg", /*   */ "nethervoid.4");
  (void) music_load(g, 44100, "data/music/DST-MysteriousMystery.ogg", /*  */ "nethervoid.4");
  (void) music_load(g, 44100, "data/music/DST-Beatdown.ogg", /*           */ "nethervoid.boss");

  (void) music_load(g, 44100, "data/music/DST-ClockTower.ogg", /*         */ "graveyard.1");
  (void) music_load(g, 44100, "data/music/DST-ExInfernus.ogg", /*         */ "graveyard.1");
  (void) music_load(g, 44100, "data/music/DST-Dangeroz.ogg", /*           */ "graveyard.2");
  (void) music_load(g, 44100, "data/music/DST-NoSuchThingAsGhosts.ogg", /**/ "graveyard.2");
  (void) music_load(g, 44100, "data/music/DST-DarkDance.ogg", /*          */ "graveyard.3");
  (void) music_load(g, 44100, "data/music/DST-GhostHouse.ogg", /*         */ "graveyard.3");
  (void) music_load(g, 44100, "data/music/DST-DarkestKnight.ogg", /*      */ "graveyard.4");
  (void) music_load(g, 44100, "data/music/DST-FourPointFour.ogg", /*      */ "graveyard.4");
  (void) music_load(g, 44100, "data/music/DST-ClubFight.ogg", /*          */ "graveyard.boss");

  (void) music_load(g, 44100, "data/music/DST-DeadToTheWorld.ogg", /*     */ "underhell.1");
  (void) music_load(g, 44100, "data/music/DST-Millenium.ogg", /*          */ "underhell.1");
  (void) music_load(g, 44100, "data/music/DST-Dominion.ogg", /*           */ "underhell.2");
  (void) music_load(g, 44100, "data/music/DST-Drakul.ogg", /*             */ "underhell.3");
  (void) music_load(g, 44100, "data/music/DST-MyDarkestHour.ogg", /*      */ "underhell.3");
  (void) music_load(g, 44100, "data/music/DST-Dredge.ogg", /*             */ "underhell.4");
  (void) music_load(g, 44100, "data/music/DST-Flak.ogg", /*               */ "underhell.boss");
}
