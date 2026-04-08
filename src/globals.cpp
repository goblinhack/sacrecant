//
// Copyright goblinhack@gmail.com
//

#include "my_globals.hpp"
#include "my_level.hpp"
#include "my_main.hpp"

#include <string>

bool g_do_screenshot; // Grab a screen shot next fram
bool g_dying;
bool g_grab_next_key;             // Waiting for key input
bool g_loading;                   // Currently loading
bool g_main_loop_running;         // SDL main loop running.
bool g_monochrome;                // Ugly hack to switch to monochrome gfx
bool g_opt_debug1;                // Basic debug
bool g_opt_debug2;                // All debugs. Slow.
bool g_opt_do_level_gen;          // Test level gen
bool g_opt_do_level_select_gen;   // Test level select gen
bool g_opt_do_room_gen;           // Test room gen
bool g_opt_level_select_menu;     // Start in the select menu
bool g_opt_no_slow_log_flush;     // Do not flush after each console log at start
bool g_opt_override_debug_level;  // Temporary used to disable debugs on load
bool g_opt_quick_start;           // Start in the main menu
bool g_opt_restarted_in_gfx_menu; // Post restart
bool g_opt_restarted;             // Post restart
bool g_opt_tests;                 // Run tests.
bool g_ptrcheck_inited;           // Ptrcheck is active
bool g_quitting;                  // Exiting the game
bool g_skip_audio_and_gfx;        // For tests

int g_intro_done;       // Have we shown the intro text yet?
int g_err_count;        // How many ERRs
int g_memory_allocated; // We've allocated something

char       *g_data_path;
char       *g_exec_dir;
char       *g_exec_full_path_and_name;
char       *g_gfx_path;
char       *g_ttf_path;
char      **g_argv;
int         g_argc;
std::string g_need_restart_with_given_arguments; // Need to restart game
std::string g_opt_seed_name;
std::string g_opt_test_name_filter;
std::string g_opt_test_current;
std::string g_program_name;

int                      g_errored_thread_id = -1; // An error has occurred on this thread
std::string              g_error_last;             // An error has occurred
thread_local std::string g_log_stderr_filename;
thread_local std::string g_log_stdout_filename;
thread_local FILE       *g_log_stderr; // stderr is redirected to a log file
thread_local FILE       *g_log_stdout; // stdout is redirected to a log file
thread_local int         g_thread_id;

void reset_globals()
{
  g_opt_no_slow_log_flush     = false;
  g_opt_override_debug_level  = false;
  g_opt_quick_start           = false;
  g_opt_level_select_menu     = false;
  g_opt_do_room_gen           = false;
  g_opt_do_level_select_gen   = false;
  g_opt_do_level_gen          = false;
  g_opt_restarted             = false;
  g_opt_restarted_in_gfx_menu = false;
  g_opt_debug1                = false;
  g_opt_debug2                = false;
  g_opt_tests                 = false;

  g_level_opt = {};
}
