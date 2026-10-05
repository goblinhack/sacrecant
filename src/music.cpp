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
#include "my_types.hpp"

#include <SDL_error.h>
#include <SDL_mixer.h>
#include <SDL_rwops.h>
#include <cmath>
#include <cstdint>
#include <map>
#include <string>
#include <utility>
#include <vector>

class Music
{
public:
  Music() = default;

  ~Music()
  {
    if (m != nullptr) {
      Mix_FreeMusic(m);
    }
    if (rw != nullptr) {
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

static std::multimap< std::string, class Music * > all;

static std::string music_current;

static bool music_init_done;

[[nodiscard]] auto music_init() -> bool
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

    for (;;) {
      auto iter = all.begin();
      if (iter == all.end()) {
        break;
      }
      delete iter->second;
      iter = all.erase(iter);
    }

    Mix_Quit();
    log("SDL: Mix_Quit");
  }
  all.clear();
}

static auto find_one(const std::string &alias) -> Music *
{
  TRACE();

  std::vector< Music * > out;

  for (auto [ itr, rangeEnd ] = all.equal_range(alias); itr != rangeEnd; ++itr) {
    out.push_back(itr->second);
  }

  if (out.empty()) {
    return nullptr;
  }

  return rand_one_of(out);
}

[[nodiscard]] auto music_load(Gamep g, uint32_t rate, const char *file, const char *name_alias) -> bool
{
  TRACE();

  auto *m = new Music();

  m->name_alias = name_alias;
  m->name       = file;
  m->rate       = rate;
  m->data       = file_load(file, &m->len);
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
    delete m;
    return false;
  }

  all.insert(std::make_pair(name_alias, m));

  return true;
}

void music_update_volume(Gamep g)
{
  TRACE();
  Mix_VolumeMusic(game_music_volume_get(g));
  SDL_ClearError();
}

[[nodiscard]] auto music_play(Gamep g, const std::string &name) -> bool
{
  TRACE();

  if (name == music_current) {
    return true;
  }
  music_current = name;

  auto *m = find_one(name);
  if (m == nullptr) {
    if (! g_opt_tests && ! g_opt_do_level_gen && ! g_opt_do_level_select_gen && ! g_opt_do_room_gen) {
      ERR("cannot find music %s", name.c_str());
    }
    return false;
  }

  if (g_opt_tests || g_opt_do_level_gen || g_opt_do_level_select_gen || g_opt_do_room_gen) {
    return true;
  }

  if (Mix_FadeInMusicPos(m->m, -1, 2000, 0) == -1) {
    ERR("cannot play music %s: %s", name.c_str(), Mix_GetError());
    SDL_ClearError();
    return false;
  }

  music_update_volume(g);

  log("playing %s volume %d", m->name.c_str(), game_music_volume_get(g));

  return true;
}

[[nodiscard]] auto music_halt() -> bool
{
  TRACE();
  music_current = "";

  Mix_FadeOutMusic(1500);
  return true;
}

void music_load(Gamep g)
{
  TRACE();

  (void) music_load(g, 44100, "data/music2/DST-XToFly.ogg", /*             */ "game over");
  (void) music_load(g, 44100, "data/music2/Spooky Bass.ogg", /*            */ "dead");
  (void) music_load(g, 44100, "data/music2/Sacrificant - Intro.ogg", /*    */ "intro");

  (void) music_load(g, 44100, "data/music2/DST-0mnis.ogg", /*              */ "dungeon.1");
  (void) music_load(g, 44100, "data/music2/DST-OldCavern.ogg", /*          */ "dungeon.1");
  (void) music_load(g, 44100, "data/music1/DST-Escape.ogg", /*             */ "dungeon.1");
  (void) music_load(g, 44100, "data/music2/DST-OmegaCenturion.ogg", /*     */ "dungeon.1");
  (void) music_load(g, 44100, "data/music2/DST-ThreeWorlds.ogg", /*        */ "dungeon.1");
  (void) music_load(g, 44100, "data/music2/DST-Razornest.ogg", /*          */ "dungeon.1");
  (void) music_load(g, 44100, "data/music1/DST-Arch-Delerium.ogg", /*      */ "dungeon.2");
  (void) music_load(g, 44100, "data/music2/DST-WaitingfortheEnemy.ogg", /* */ "dungeon.2");
  (void) music_load(g, 44100, "data/music2/DST-Psykick.ogg", /*            */ "dungeon.2");
  (void) music_load(g, 44100, "data/music1/DST-Frontier.ogg", /*           */ "dungeon.2");
  (void) music_load(g, 44100, "data/music1/DST-DoomCity.ogg", /*           */ "dungeon.3");
  (void) music_load(g, 44100, "data/music2/DST-PurgatoryStep.ogg", /*      */ "dungeon.3");
  (void) music_load(g, 44100, "data/music2/DST-WaterFell.ogg", /*          */ "dungeon.3");
  (void) music_load(g, 44100, "data/music1/DST-Legends.ogg", /*            */ "dungeon.3");
  (void) music_load(g, 44100, "data/music1/DST-DasElectron.ogg", /*        */ "dungeon.4");
  (void) music_load(g, 44100, "data/music1/DST-NoSleep.ogg", /*            */ "dungeon.4");
  (void) music_load(g, 44100, "data/music2/DST-OffWorld.ogg", /*           */ "dungeon.4");
  (void) music_load(g, 44100, "data/music2/DST-OrganicSpace.ogg", /*       */ "dungeon.4");
  (void) music_load(g, 44100, "data/music2/DST-Perchlorate.ogg", /*        */ "dungeon.4");
  (void) music_load(g, 44100, "data/music2/DST-Protector.ogg", /*          */ "dungeon.4");
  (void) music_load(g, 44100, "data/music1/DST-BloodAndIron.ogg", /*       */ "dungeon.boss");

  (void) music_load(g, 44100, "data/music2/DST-Petaluna.ogg", /*           */ "bogland.1");
  (void) music_load(g, 44100, "data/music1/DST-DragonFly.ogg", /*          */ "bogland.1");
  (void) music_load(g, 44100, "data/music1/DST-Lightness.ogg", /*          */ "bogland.1");
  (void) music_load(g, 44100, "data/music1/DST-Drogs.ogg", /*              */ "bogland.2");
  (void) music_load(g, 44100, "data/music1/DST-MelodyB.ogg", /*            */ "bogland.2");
  (void) music_load(g, 44100, "data/music1/DST-Echorex.ogg", /*            */ "bogland.3");
  (void) music_load(g, 44100, "data/music2/DST-Xmar.ogg", /*               */ "bogland.3");
  (void) music_load(g, 44100, "data/music1/DST-Eretria.ogg", /*            */ "bogland.4");
  (void) music_load(g, 44100, "data/music1/DST-Nvision.ogg", /*            */ "bogland.4");
  (void) music_load(g, 44100, "data/music1/DST-ElectriKaos.ogg", /*        */ "bogland.boss");

  (void) music_load(g, 44100, "data/music1/DST-DFree.ogg", /*              */ "nethervoid.1");
  (void) music_load(g, 44100, "data/music2/DST-Vostros.ogg", /*            */ "nethervoid.1");
  (void) music_load(g, 44100, "data/music2/DST-X.ogg", /*                  */ "nethervoid.1");
  (void) music_load(g, 44100, "data/music1/DST-ExoPlanet.ogg", /*          */ "nethervoid.1");
  (void) music_load(g, 44100, "data/music1/DST-Digitalus.ogg", /*          */ "nethervoid.2");
  (void) music_load(g, 44100, "data/music1/DST-Expander.ogg", /*           */ "nethervoid.2");
  (void) music_load(g, 44100, "data/music1/DST-DreamOfForever.ogg", /*     */ "nethervoid.3");
  (void) music_load(g, 44100, "data/music1/DST-FrozenJellyFish.ogg", /*    */ "nethervoid.3");
  (void) music_load(g, 44100, "data/music2/DST-Teleport.ogg", /*           */ "nethervoid.3");
  (void) music_load(g, 44100, "data/music1/DST-CyberOps.ogg", /*           */ "nethervoid.4");
  (void) music_load(g, 44100, "data/music2/DST-impuretechnology.ogg", /*   */ "nethervoid.4");
  (void) music_load(g, 44100, "data/music1/DST-MysteriousMystery.ogg", /*  */ "nethervoid.4");
  (void) music_load(g, 44100, "data/music1/DST-Beatdown.ogg", /*           */ "nethervoid.boss");

  (void) music_load(g, 44100, "data/music2/DST-Tourette.ogg", /*           */ "graveyard.1");
  (void) music_load(g, 44100, "data/music1/DST-ClockTower.ogg", /*         */ "graveyard.1");
  (void) music_load(g, 44100, "data/music1/DST-ExInfernus.ogg", /*         */ "graveyard.1");
  (void) music_load(g, 44100, "data/music2/DST-TowerAngels.ogg", /*        */ "graveyard.1");
  (void) music_load(g, 44100, "data/music1/DST-Dangeroz.ogg", /*           */ "graveyard.2");
  (void) music_load(g, 44100, "data/music2/DST-TwoHeaded.ogg", /*          */ "graveyard.2");
  (void) music_load(g, 44100, "data/music1/DST-NoSuchThingAsGhosts.ogg", /**/ "graveyard.2");
  (void) music_load(g, 44100, "data/music1/DST-DarkDance.ogg", /*          */ "graveyard.3");
  (void) music_load(g, 44100, "data/music1/DST-GhostHouse.ogg", /*         */ "graveyard.3");
  (void) music_load(g, 44100, "data/music2/DST-TheHauntedChapel.ogg", /*   */ "graveyard.3");
  (void) music_load(g, 44100, "data/music1/DST-DarkestKnight.ogg", /*      */ "graveyard.4");
  (void) music_load(g, 44100, "data/music1/DST-FourPointFour.ogg", /*      */ "graveyard.4");
  (void) music_load(g, 44100, "data/music1/DST-ClubFight.ogg", /*          */ "graveyard.boss");

  (void) music_load(g, 44100, "data/music1/DST-DeadToTheWorld.ogg", /*     */ "underhell.1");
  (void) music_load(g, 44100, "data/music1/DST-Millennium.ogg", /*          */ "underhell.1");
  (void) music_load(g, 44100, "data/music1/DST-Dominion.ogg", /*           */ "underhell.2");
  (void) music_load(g, 44100, "data/music1/DST-Drakul.ogg", /*             */ "underhell.3");
  (void) music_load(g, 44100, "data/music1/DST-MyDarkestHour.ogg", /*      */ "underhell.3");
  (void) music_load(g, 44100, "data/music1/DST-Dredge.ogg", /*             */ "underhell.4");
  (void) music_load(g, 44100, "data/music2/DST-Wdyw.ogg", /*               */ "underhell.4");
  (void) music_load(g, 44100, "data/music1/DST-Flak.ogg", /*               */ "underhell.boss");
}
