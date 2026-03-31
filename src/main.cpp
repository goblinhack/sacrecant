//
// Copyright goblinhack@gmail.com
//

#include "my_ascii.hpp"
#include "my_audio.hpp"
#include "my_command.hpp"
#include "my_font.hpp"
#include "my_gfx.hpp"
#include "my_globals.hpp"
#include "my_level.hpp"
#include "my_main.hpp"
#include "my_music.hpp"
#include "my_ramdisk.hpp"
#include "my_sdl_proto.hpp"
#include "my_sound.hpp"
#include "my_test.hpp"
#include "my_tests.hpp"
#include "my_wid_tiles.hpp"
#include "my_wids.hpp"

#include <csignal>
#include <cstring>
#include <iostream>
#include <strings.h>
#include <sys/stat.h>
#include <unistd.h>

//
// Used to stop the compiler removing unused code I want to keep
//
#ifdef DEBUG_BUILD
bool compiler_unused;
#endif

static void usage()
{
  TRACE();
  static int whinged;

  if (whinged != 0) {
    return;
  }
  whinged = 1;

  con("Gorget, options:");
  con(" ");
  con("Commonly used options:");
  con(" --seed <name/number>              -- Set the random seed.");
  con(" ");
  con("Debugging:");
  con(" --debug                           -- Basic debug.");
  con(" --debug2                          -- All debugs. Slow.");
  con(" ");
  con("Testing:");
  con(" --level name/<number>             -- Start in this level only.");
  con(" --level-select                    -- Start in the level select menu.");
  con(" --quick-start                     -- Quick start inside level.");
  con(" --test <name>                     -- Run test foo only.");
  con(" --tests                           -- Run all tests.");
  con(" ");
  con("Code generation:");
  con(" --do-level-gen                    -- Do level gen only.");
  con(" --do-level-select-gen             -- Do level select gen only.");
  con(" --do-room-gen                     -- Generate room files only.");
  con(" ");
  con("Examples");
  con(" # start on seed crowcod, level 10");
  con(" ./gorget --seed crowcod --level 10");
  con(" ");
  con(" # start on seed crowcod, last level and at the level select meny");
  con(" ./gorget --seed crowcod --level 100 --level-select");
  con(" ");
  con("Written by goblinhack@gmail.com");
}

static void parse_args(int argc, char *argv[])
{
  TRACE();
  int i = 0;

  //
  // Parse format args
  //
  if (g_opt_debug1) {
    con("Parse command line arguments for '%s'", argv[ 0 ]);
  } else {
    log("parse command line arguments for '%s'", argv[ 0 ]);
  }

  for (i = 1; i < argc; i++) {
    if (g_opt_debug1) {
      con("+ argument: \"%s\"", argv[ i ]);
    } else {
      log("+ argument: \"%s\"", argv[ i ]);
    }
  }

  if (argc != 0) {
    g_opt_no_slow_log_flush = true;
  }

  for (i = 1; i < argc; i++) {
    if ((strcasecmp(argv[ i ], "--seed") == 0) || (strcasecmp(argv[ i ], "-seed") == 0)) {
      if (i == argc - 1) {
        usage();
        CROAK("missing parameter for argument, %s", argv[ i ]);
      }
      g_opt_seed_name = argv[ i + 1 ];
      i++;
      continue;
    }

    if ((strcasecmp(argv[ i ], "--debug") == 0) || (strcasecmp(argv[ i ], "-debug") == 0) || (strcasecmp(argv[ i ], "--debug1") == 0)
        || (strcasecmp(argv[ i ], "-debug1") == 0)) {
      g_opt_debug1               = true;
      g_opt_override_debug_level = true;
      continue;
    }

    if ((strcasecmp(argv[ i ], "--debug2") == 0) || (strcasecmp(argv[ i ], "-debug2") == 0)) {
      g_opt_debug1               = true;
      g_opt_debug2               = true;
      g_opt_override_debug_level = true;
      continue;
    }

    if ((strcasecmp(argv[ i ], "--quick-start") == 0) || (strcasecmp(argv[ i ], "-quick-start") == 0)) {
      g_opt_quick_start = true;
      continue;
    }

    if ((strcasecmp(argv[ i ], "--level-select") == 0) || (strcasecmp(argv[ i ], "-level-select") == 0)) {
      g_opt_level_select_menu = true;
      continue;
    }

    if ((strcasecmp(argv[ i ], "--level") == 0) || (strcasecmp(argv[ i ], "-level") == 0)) {
      if (i == argc - 1) {
        usage();
        CROAK("missing parameter for argument, %s", argv[ i ]);
      }

      //
      // Check if this is a level number or name
      //
      char *p   = nullptr; // NOLINT
      auto  num = strtol(argv[ i + 1 ], &p, 10);
      if (*p != 0) {
        //
        // Level name
        //
        g_level_opt.level_name = argv[ i + 1 ];
        g_level_opt.is_set     = true;
      } else if (num > 0) {
        //
        // Level number
        //
        g_level_opt.level_num = num - 1;
        g_level_opt.is_set    = true;
      }

      g_opt_quick_start = true;
      i++;
      continue;
    }

    if ((strcasecmp(argv[ i ], "--test") == 0) || (strcasecmp(argv[ i ], "-test") == 0)) {
      if (i == argc - 1) {
        usage();
        CROAK("missing parameter for argument, %s", argv[ i ]);
      }
      g_opt_test_name_filter = argv[ i + 1 ];
      g_opt_tests            = true;
      g_opt_debug1           = true;
      g_skip_audio_and_gfx   = true;
      i++;
      continue;
    }

    if ((strcasecmp(argv[ i ], "--tests") == 0) || (strcasecmp(argv[ i ], "-tests") == 0)) {
      g_opt_tests          = true;
      g_opt_debug1         = true;
      g_skip_audio_and_gfx = true;
      continue;
    }

    if ((strcasecmp(argv[ i ], "--do-room-gen") == 0) || (strcasecmp(argv[ i ], "-do-room-gen") == 0)) {
      g_opt_do_room_gen    = true;
      g_skip_audio_and_gfx = true;
      continue;
    }

    if ((strcasecmp(argv[ i ], "--do-level-select-gen") == 0) || (strcasecmp(argv[ i ], "-do-level-select-gen") == 0)) {
      g_opt_do_level_select_gen = true;
      g_skip_audio_and_gfx      = true;
      continue;
    }

    if ((strcasecmp(argv[ i ], "--do-level-gen") == 0) || (strcasecmp(argv[ i ], "-do-level-gen") == 0)) {
      g_opt_do_level_gen   = true;
      g_skip_audio_and_gfx = true;
      continue;
    }

    if ((strcasecmp(argv[ i ], "--restart") == 0) || (strcasecmp(argv[ i ], "-restart") == 0)) {
      g_opt_restarted = true;
      continue;
    }

    if ((strcasecmp(argv[ i ], "--restart-in-gfx-menu") == 0) || (strcasecmp(argv[ i ], "-restart-in-gfx-menu") == 0)) {
      g_opt_restarted_in_gfx_menu = true;
      continue;
    }

    //
    // Bad argument.
    //
    if (argv[ i ][ 0 ] == '-') {
      usage();
      //
      // Win32 puts spaces in argv when we restart the process. I'm not sure
      // how to handle that, so don't fail the program. Warn and continue.
      //
      con("unknown format argument, %s", argv[ i ]);
#ifdef _WIN32
      return;
#else
      exit(1);
#endif
    }

    usage();
    con("unknown format argument, %s", argv[ i ]);
#ifdef _WIN32
    return;
#else
    exit(1);
#endif
  }
}

auto main(int argc, char *argv[]) -> int
{
  Gamep g = nullptr;
  g_argv  = argv;
  g_argc  = argc;

  g_thread_id = MAIN_THREAD;

  TRACE();
  auto appdata = log_dir_create(); // Want this first so we get all logs

  TRACE_DEBUG();
  redirect_stdout();
  log("will use STDOUT as '%s'", g_log_stdout_filename.c_str());

  TRACE_DEBUG();
  redirect_stderr();
  log("will use STDERR as '%s'", g_log_stderr_filename.c_str());

  //////////////////////////////////////////////////////////////////////////////
  // Call parse_args before any memory allocations, in case debug2 is enabled
  //////////////////////////////////////////////////////////////////////////////
  TRACE_DEBUG();
  parse_args(argc, argv);

  //////////////////////////////////////////////////////////////////////////////
  // Use LOG instead of CON until we set stdout or you see two logs
  // v v v v v v v v v v v v v v v v v v v v v v v v v v v v v v v
  //////////////////////////////////////////////////////////////////////////////
  TRACE_DEBUG();
#ifdef _WIN32
  log("platform is _WIN32");
#endif
#ifdef __MINGW32__
  log("platform is __MINGW32__");
#endif
#ifdef __MINGW64__
  log("platform is __MINGW64__");
#endif
#ifdef __APPLE__
  log("platform is __APPLE__");
#endif
#ifdef __linux__
  log("platform is __linux__");
#endif

  //////////////////////////////////////////////////////////////////////////////
  // ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ Use LOG
  // instead of CON until we set stdout or you see two logs
  //////////////////////////////////////////////////////////////////////////////

  {
    TRACE_DEBUG();
    find_file_locations();
  }

  {
    log("ramdisk init");
    TRACE_DEBUG();
    ramdisk_init();
  }

  {
    log("create console");
    TRACE_DEBUG();
    ascii_init();
  }

  //
  // Need this to get the UTF on the console
  //
#ifndef _WIN32
  log("set locale for console");
  std::locale const loc("");
  std::ios_base::sync_with_stdio(false);
  std::wcout.imbue(loc);
#endif

#ifdef ENABLE_CRASH_HANDLER
  //
  // Crash handlers
  //
  log("install crash handlers");
#ifdef SIGSEGV
  signal(SIGSEGV, crash_handler);
#endif
#ifdef SIGBUS
  signal(SIGBUS, crash_handler);
#endif
#ifdef SIGABRT
  signal(SIGABRT, crash_handler);
#endif
#ifdef SIGFPE
  signal(SIGFPE, crash_handler);
#endif
#ifdef SIGILL
  signal(SIGILL, crash_handler);
#endif

#ifdef SIGPIPE
  signal(SIGPIPE, ctrlc_handler);
#endif
#ifdef SIGINT
  signal(SIGINT, ctrlc_handler);
#endif

#endif // ENABLE_CRASH_HANDLER

  //
  // Create and load the last saved game
  //
  (void) game_load_last_config(appdata.c_str());
  extern Gamep game;
  g = game;
  game_init(g);

  if (! g_opt_tests) {
    TRACE_DEBUG();
    if (! sdl_init()) {
      err("SDL: Init");
    }
  }

  if (! g_opt_tests) {
    TRACE_DEBUG();
    if (! sdl_display_init(g)) {
      err("SDL: Display init");
    }
  }

  if (! g_opt_tests) {
    TRACE_DEBUG();
    sdl_config_update_all(g);
  }

  if (g_skip_audio_and_gfx) {
    //
    // Skip for speed of test setup
    //
  } else {
    //
    // Causes a 0.3 sec delay first time it seems to run
    //
    log("SDL: Pump events");
    SDL_PumpEvents();
    log("SDL: Pump events done");
  }

  if (! g_need_restart_with_given_arguments.empty()) {
    TRACE_DEBUG();
    restart(g, g_need_restart_with_given_arguments);
  }

  {
    TRACE_DEBUG();
    if (g_opt_debug1) {
      con("Load early gfx tiles, text, UI etc...");
    } else {
      log("load early gfx tiles, text, UI etc...");
    }
    gfx_init();
  }

  //
  // Disable vsync so the console is faster
  //
  if (! g_opt_tests) {
    TRACE_DEBUG();
    SDL_GL_SetSwapInterval(0);
  }

  if (! wid_init()) {
    err("widget init");
  }

  //
  // Colors
  //
  color_init();

  if (! g_skip_audio_and_gfx) {
    TRACE_DEBUG();
    if (g_opt_debug1) {
      con("Load fonts");
    } else {
      log("load fonts");
    }
    if (! font_init()) {
      err("font init");
    }
  }

  if (! g_skip_audio_and_gfx) {
    TRACE_DEBUG();
    if (g_opt_debug1) {
      con("Load console");
    } else {
      log("load console");
    }
    if (! wid_console_init(g)) {
      err("wid_console init");
    }
    wid_console_flush(g);
  }

  if (! g_opt_tests) {
    con(UI_INFO_FMT_STR "          @@@@@@@@   @@@@@@   @@@@@@@    @@@@@@@@  @@@@@@@@  @@@@@@@" UI_RESET_FMT);
    con(UI_INFO_FMT_STR "         @@@@@@@@@  @@@@@@@@  @@@@@@@@  @@@@@@@@@  @@@@@@@@  @@@@@@@" UI_RESET_FMT);
    con(UI_INFO_FMT_STR "         !@@        @@!  @@@  @@!  @@@  !@@        @@!         @@!" UI_RESET_FMT);
    con(UI_INFO_FMT_STR "         !@!        !@!  @!@  !@!  @!@  !@!        !@!         !@!" UI_RESET_FMT);
    con(UI_INFO_FMT_STR "         !@! @!@!@  @!@  !@!  @!@!!@!   !@! @!@!@  @!!!:!      @!!" UI_RESET_FMT);
    con(UI_INFO_FMT_STR "         !!! !!@!!  !@!  !!!  !!@!@!    !!! !!@!!  !!!!!:      !!!" UI_RESET_FMT);
    con(UI_INFO_FMT_STR "         :!!   !!:  !!:  !!!  !!: :!!   :!!   !!:  !!:         !!:" UI_RESET_FMT);
    con(UI_INFO_FMT_STR "         :!:   !::  :!:  !:!  :!:  !:!  :!:   !::  :!:         :!:" UI_RESET_FMT);
    con(UI_INFO_FMT_STR "          ::: ::::  ::::: ::  ::   :::   ::: ::::   :: ::::     ::" UI_RESET_FMT);
    con(UI_INFO_FMT_STR "          :: :: :    : :  :    :   : :   :: :: :   : :: ::      :" UI_RESET_FMT);
    con(UI_INFO_FMT_STR "           :              :    .         :: :      :  :  :" UI_RESET_FMT "");
    con(UI_INFO_FMT_STR "           .              :               : .      .     ." UI_RESET_FMT "");
    con(UI_INFO_FMT_STR "                          :               ." UI_RESET_FMT);
    con(UI_INFO_FMT_STR "                          ." UI_RESET_FMT);
    con(UI_INFO_FMT_STR "Version: " MYVER UI_RESET_FMT);
    con("Press " UI_WARNING_FMT_STR "<tab>" UI_RESET_FMT " to complete commands.");
    con("Press " UI_WARNING_FMT_STR "?" UI_RESET_FMT "     to show command options.");
    auto key = ::to_string(game_key_console_get(g));
    con("Press " UI_WARNING_FMT_STR "<%s>" UI_RESET_FMT "   to hide this console", key.c_str());
  }

  //
  // Need to preserve spaces for restarting via exec
  //
  if (! g_skip_audio_and_gfx) {
    TRACE_DEBUG();
    g_program_name = std::string(argv[ 0 ]);
    if (g_opt_debug1) {
      con("Original program name: %s", g_program_name.c_str());
    } else {
      log("original program name: %s", g_program_name.c_str());
    }
    wid_console_flush(g);
  }

  if (! g_skip_audio_and_gfx) {
    TRACE_DEBUG();
    if (g_opt_debug1) {
      con("Load tiles");
    } else {
      log("load tiles");
    }
    if (! wid_tiles_init()) {
      err("widget tiles init");
    }
  }

  if (! g_skip_audio_and_gfx) {
    TRACE_DEBUG();
    if (! tile_init()) {
      err("tile init");
    }
    wid_console_flush(g);
  }

  if (! g_skip_audio_and_gfx) {
    TRACE_DEBUG();
    if (g_opt_debug1) {
      con("Load textures");
    } else {
      log("load textures");
    }
    if (! tex_init()) {
      err("tex init");
    }
    wid_console_flush(g);
  }

  if (! g_skip_audio_and_gfx) {
    TRACE_DEBUG();
    if (g_opt_debug1) {
      con("Init audio");
    } else {
      log("init audio");
    }
    if (! audio_init()) {
      err("audio init");
    }
    wid_console_flush(g);
  }

  if (! g_skip_audio_and_gfx) {
    TRACE_DEBUG();
    if (g_opt_debug1) {
      con("Init music");
    } else {
      log("init music");
    }
    if (! music_init()) {
      err("music init");
    }
    wid_console_flush(g);
  }

  if (! g_skip_audio_and_gfx) {
    TRACE_DEBUG();
    if (g_opt_debug1) {
      con("Load sounds");
    } else {
      log("load sounds");
    }
    if (! sound_init()) {
      err("sound init");
    } else {
      sounds_init();
    }
    wid_console_flush(g);
  }

  {
    TRACE_DEBUG();

    log("load templates");
    if (! tp_init()) {
      err("templates init");
    }
    log("loaded templates");
  }

  {
    TRACE_DEBUG();
    log("load commands");
    if (! command_init()) {
      err("command init");
    }
    log("loaded commands");
    wid_console_flush(g);
  }

  {
    rooms_init(g);
    levels_init(g);
    fragments_init(g);

    if (g_opt_tests) {
      test_init();

      con("Running tests");
      con("-------------");
      tests_run(g);

      DIE_CLEAN("done");
    }

    if (g_opt_do_level_gen) {
      con("Creating many levels. Look in the log file for output.");
      con("------------------------------------------------------");
      level_gen_test(g);
      DIE_CLEAN("done");
    }

    if (g_opt_do_level_select_gen) {
      con("Creating level select levels. Look in the log file for output.");
      con("--------------------------------------------------------------");
      level_select_test(g);
      DIE_CLEAN("done");
    }

    if (g_opt_do_room_gen) {
      con("Creating room gen files. You will need to recompile after this.");
      con("---------------------------------------------------------------");
      rooms_test(g);
      DIE_CLEAN("done");
    }
  }

  {
    TRACE_DEBUG();
    wid_console_flush(g);

    //
    // Main menu
    //
    TRACE_DEBUG();
    if (AN_ERROR_OCCURRED()) {
      wid_console_raise(g);
    } else if (g_opt_restarted_in_gfx_menu) {
      g_opt_restarted_in_gfx_menu = false;
      wid_hide(g, wid_console_window);
      wid_cfg_gfx_select(g);
    } else if (g_opt_quick_start) {
      wid_hide(g, wid_console_window);
      wid_new_game(g);
    } else if (g_opt_level_select_menu) {
      wid_hide(g, wid_console_window);
      wid_new_game(g);
    } else {
      wid_hide(g, wid_console_window);
      wid_main_menu_select(g);
    }
  }

  {
    glEnable(GL_TEXTURE_2D);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    GL_ERROR_CHECK();

    if (game_gfx_vsync_enable_get(g)) {
      //
      // Force display VSYNC
      //
      SDL_GL_SetSwapInterval(1);
    } else {
      //
      // No VSYNC
      //
      SDL_GL_SetSwapInterval(0);
    }
    GL_ERROR_CHECK();

    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    GL_ERROR_CHECK();
  }

  g_opt_no_slow_log_flush = false;

  sdl_loop(g);
  wid_console_flush(g);

  if (! g_need_restart_with_given_arguments.empty()) {
    TRACE_DEBUG();
    restart(g, g_need_restart_with_given_arguments);
  }

  log("quit");
  cleanup();

  con("Goodbye my friend and take care until next time!");
  return 0;
}
