//
// Copyright goblinhack@gmail.com
//

#include "my_audio.hpp"
#include "my_callstack.hpp"
#include "my_color.hpp"
#include "my_command.hpp"
#include "my_font.hpp"
#include "my_game.hpp"
#include "my_gl.hpp"
#include "my_globals.hpp"
#include "my_level.hpp"
#include "my_main.hpp"
#include "my_music.hpp"
#include "my_ptrcheck.hpp"
#include "my_sdl_proto.hpp"
#include "my_sound.hpp"
#include "my_test.hpp"
#include "my_tex.hpp"
#include "my_tile.hpp"
#include "my_tp.hpp"
#include "my_types.hpp"
#include "my_wid.hpp"
#include "my_wid_tiles.hpp"
#include "my_wids.hpp"

#include <cmath>
#include <csignal>
#include <cstring>

//
// Free all resources
//
void cleanup()
{
  log("FINI: cleanup, thread %d", g_thread_id);
  TRACE_INDENT();

  if (g_thread_id != MAIN_THREAD) {
    return;
  }

  if (g_quitting) {
    con("Quitting, nested");
    return;
  }

  if (AN_ERROR_OCCURRED()) {
    con("Quitting, errored");
    return;
  }

  g_quitting = true;

  extern Gamep game;
  auto        *g = game;

#ifdef ENABLE_CRASH_HANDLER
#ifdef SIGSEGV
  signal(SIGSEGV, nullptr); // uninstall our handler
#endif
#ifdef SIGBUS
  signal(SIGBUS, nullptr); // uninstall our handler
#endif
#ifdef SIGABRT
  signal(SIGABRT, nullptr); // uninstall our handler
#endif
#ifdef SIGINT
  signal(SIGINT, nullptr); // uninstall our handler
#endif
#ifdef SIGPIPE
  signal(SIGPIPE, nullptr); // uninstall our handler
#endif
#ifdef SIGFPE
  signal(SIGFPE, nullptr); // uninstall our handler
#endif
#endif // ENABLE_CRASH_HANDLER

  sdl_prepare_to_exit(g);
  wid_console_fini(g);
  command_fini();
  font_fini();
  tex_fini();
  wid_tiles_fini();
  tile_fini();
  blit_fini();
  color_fini();
#ifdef USE_SDL3
  //
  // The mixer owns the tracks, so it has to go last.
  //
  music_fini();
  sound_fini();
  audio_fini();
#else
  audio_fini();
  music_fini();
  sound_fini();
#endif
  sdl_fini(g);
  rooms_fini(g);
  levels_fini(g);
  fragments_fini(g);
  fragment_alts_fini(g);

  //
  // Do this last as sdl_fini depends on it.
  //
  game_fini(g);
  g = nullptr;

  //
  // Must be post game_fini
  //
  tp_fini();

  //
  // Wid fini has to be after game_fini, as the game state change to none will
  // destroy remaining windows.
  //
  wid_fini(nullptr);

  test_fini();

  log("FINI: cleanup done");

  ptrcheck_leak_print();
  ptrcheck_fini();

  close_stdout();
  close_stderr();
}
