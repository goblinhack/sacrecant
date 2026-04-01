//
// Copyright goblinhack@gmail.com
//

#ifndef MY_GLOBALS_HPP
#define MY_GLOBALS_HPP

#include <string>

extern bool        g_dying;
extern bool        g_do_screenshot;             // Grab a screen shot next fram
extern bool        g_grab_next_key;             // Waiting for key input
extern bool        g_loading;                   // Currently loading
extern bool        g_main_loop_running;         // SDL main loop is running.
extern bool        g_quitting;                  // Exiting the game
extern bool        g_opt_debug1;                // Normal debugs
extern bool        g_opt_debug2;                // All debugs
extern bool        g_opt_do_level_gen;          // Test level gen
extern bool        g_opt_do_level_select_gen;   // Test level select gen
extern bool        g_opt_do_room_gen;           // Test room gen
extern bool        g_opt_no_slow_log_flush;     // Do not flush after each console log at start
extern bool        g_opt_override_debug_level;  // Temporary, used to disable existing debugs
extern bool        g_opt_level_select_menu;     // Start in the select menu
extern bool        g_opt_quick_start;           // Start in the main menu
extern bool        g_opt_restarted_in_gfx_menu; // Post restart
extern bool        g_opt_restarted;             // Post restart
extern bool        g_opt_tests;                 // Dungeon tests
extern int         g_memory_allocated;          // We've allocated something
extern bool        g_ptrcheck_inited;           // Ptrcheck is active
extern bool        g_monochrome;                // Ugly hack to switch to monochrome gfx
extern bool        g_skip_audio_and_gfx;        // For tests
extern int         g_err_count;                 // How many ERRs
extern char       *g_data_path;
extern char       *g_exec_dir;
extern char       *g_exec_full_path_and_name;
extern char       *g_gfx_path;
extern char       *g_ttf_path;
extern char      **g_argv;
extern int         g_argc;
extern std::string g_need_restart_with_given_arguments; // Need to restart game with the given arg
extern std::string g_program_name;
extern std::string g_opt_seed_name;
extern std::string g_opt_test_name_filter;
extern std::string g_opt_test_current;

extern int                      g_errored_thread_id; // An error has occurred
extern std::string              g_error_last;        // The last error cause
extern thread_local std::string g_log_stderr_filename;
extern thread_local std::string g_log_stdout_filename;
extern thread_local FILE       *g_log_stderr;
extern thread_local FILE       *g_log_stdout;

//
// Main thread is 0
// Game threads > 0
//
enum {
  MAIN_THREAD   = 0, // newline
  VISION_THREAD = 99 // newline
};
extern thread_local int g_thread_id;

static inline auto AN_ERROR_OCCURRED() -> bool { return g_errored_thread_id != -1; }
static inline auto NO_ERROR_OCCURRED() -> bool { return g_errored_thread_id == -1; }

#endif
