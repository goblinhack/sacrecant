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

auto music_load(Gamep g, uint32_t rate, const char *file, const char *name_alias) -> bool
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

auto music_play(Gamep g, const std::string &name) -> bool
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

  if (Mix_FadeInMusicPos(m->m, -1, 2000, 0) == -1) {
    ERR("cannot play music %s: %s", name.c_str(), Mix_GetError());
    SDL_ClearError();
    return false;
  }

  music_update_volume(g);

  log("Playing %s volume %d", m->name.c_str(), game_music_volume_get(g));

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

  (void) music_load(g, 44100, "data/music/DST-XToFly.ogg", /*             */ "end");
  (void) music_load(g, 44100, "data/music/Spooky Bass.ogg", /*            */ "dead");
  (void) music_load(g, 44100, "data/music/Sacrificant - Intro.ogg", /*    */ "intro");

  (void) music_load(g, 44100, "data/music/DST-0mnis.ogg", /*              */ "dungeon.1");
  (void) music_load(g, 44100, "data/music/DST-OldCavern.ogg", /*          */ "dungeon.1");
  (void) music_load(g, 44100, "data/music/DST-Escape.ogg", /*             */ "dungeon.1");
  (void) music_load(g, 44100, "data/music/DST-OmegaCenturion.ogg", /*     */ "dungeon.1");
  (void) music_load(g, 44100, "data/music/DST-ThreeWorlds.ogg", /*        */ "dungeon.1");
  (void) music_load(g, 44100, "data/music/DST-Razornest.ogg", /*          */ "dungeon.1");
  (void) music_load(g, 44100, "data/music/DST-Arch-Delerium.ogg", /*      */ "dungeon.2");
  (void) music_load(g, 44100, "data/music/DST-WaitingfortheEnemy.ogg", /* */ "dungeon.2");
  (void) music_load(g, 44100, "data/music/DST-Psykick.ogg", /*            */ "dungeon.2");
  (void) music_load(g, 44100, "data/music/DST-Frontier.ogg", /*           */ "dungeon.2");
  (void) music_load(g, 44100, "data/music/DST-DoomCity.ogg", /*           */ "dungeon.3");
  (void) music_load(g, 44100, "data/music/DST-PurgatoryStep.ogg", /*      */ "dungeon.3");
  (void) music_load(g, 44100, "data/music/DST-WaterFell.ogg", /*          */ "dungeon.3");
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
  (void) music_load(g, 44100, "data/music/DST-Xmar.ogg", /*               */ "bogland.3");
  (void) music_load(g, 44100, "data/music/DST-Eretria.ogg", /*            */ "bogland.4");
  (void) music_load(g, 44100, "data/music/DST-Nvision.ogg", /*            */ "bogland.4");
  (void) music_load(g, 44100, "data/music/DST-ElectriKaos.ogg", /*        */ "bogland.boss");

  (void) music_load(g, 44100, "data/music/DST-DFree.ogg", /*              */ "nethervoid.1");
  (void) music_load(g, 44100, "data/music/DST-Vostros.ogg", /*            */ "nethervoid.1");
  (void) music_load(g, 44100, "data/music/DST-X.ogg", /*                  */ "nethervoid.1");
  (void) music_load(g, 44100, "data/music/DST-ExoPlanet.ogg", /*          */ "nethervoid.1");
  (void) music_load(g, 44100, "data/music/DST-Digitalus.ogg", /*          */ "nethervoid.2");
  (void) music_load(g, 44100, "data/music/DST-Expander.ogg", /*           */ "nethervoid.2");
  (void) music_load(g, 44100, "data/music/DST-DreamOfForever.ogg", /*     */ "nethervoid.3");
  (void) music_load(g, 44100, "data/music/DST-FrozenJellyFish.ogg", /*    */ "nethervoid.3");
  (void) music_load(g, 44100, "data/music/DST-Teleport.ogg", /*           */ "nethervoid.3");
  (void) music_load(g, 44100, "data/music/DST-CyberOps.ogg", /*           */ "nethervoid.4");
  (void) music_load(g, 44100, "data/music/DST-impuretechnology.ogg", /*   */ "nethervoid.4");
  (void) music_load(g, 44100, "data/music/DST-MysteriousMystery.ogg", /*  */ "nethervoid.4");
  (void) music_load(g, 44100, "data/music/DST-Beatdown.ogg", /*           */ "nethervoid.boss");

  (void) music_load(g, 44100, "data/music/DST-Tourette.ogg", /*           */ "graveyard.1");
  (void) music_load(g, 44100, "data/music/DST-ClockTower.ogg", /*         */ "graveyard.1");
  (void) music_load(g, 44100, "data/music/DST-ExInfernus.ogg", /*         */ "graveyard.1");
  (void) music_load(g, 44100, "data/music/DST-TowerAngels.ogg", /*        */ "graveyard.1");
  (void) music_load(g, 44100, "data/music/DST-Dangeroz.ogg", /*           */ "graveyard.2");
  (void) music_load(g, 44100, "data/music/DST-TwoHeaded.ogg", /*          */ "graveyard.2");
  (void) music_load(g, 44100, "data/music/DST-NoSuchThingAsGhosts.ogg", /**/ "graveyard.2");
  (void) music_load(g, 44100, "data/music/DST-DarkDance.ogg", /*          */ "graveyard.3");
  (void) music_load(g, 44100, "data/music/DST-GhostHouse.ogg", /*         */ "graveyard.3");
  (void) music_load(g, 44100, "data/music/DST-TheHauntedChapel.ogg", /*   */ "graveyard.3");
  (void) music_load(g, 44100, "data/music/DST-DarkestKnight.ogg", /*      */ "graveyard.4");
  (void) music_load(g, 44100, "data/music/DST-FourPointFour.ogg", /*      */ "graveyard.4");
  (void) music_load(g, 44100, "data/music/DST-ClubFight.ogg", /*          */ "graveyard.boss");

  (void) music_load(g, 44100, "data/music/DST-DeadToTheWorld.ogg", /*     */ "underhell.1");
  (void) music_load(g, 44100, "data/music/DST-Millenium.ogg", /*          */ "underhell.1");
  (void) music_load(g, 44100, "data/music/DST-Dominion.ogg", /*           */ "underhell.2");
  (void) music_load(g, 44100, "data/music/DST-Drakul.ogg", /*             */ "underhell.3");
  (void) music_load(g, 44100, "data/music/DST-MyDarkestHour.ogg", /*      */ "underhell.3");
  (void) music_load(g, 44100, "data/music/DST-Dredge.ogg", /*             */ "underhell.4");
  (void) music_load(g, 44100, "data/music/DST-Wdyw.ogg", /*               */ "underhell.4");
  (void) music_load(g, 44100, "data/music/DST-Flak.ogg", /*               */ "underhell.boss");
}
