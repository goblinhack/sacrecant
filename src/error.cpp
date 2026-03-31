//
// Copyright goblinhack@gmail.com
//

#include "my_backtrace.hpp"
#include "my_callstack.hpp"
#include "my_game.hpp"
#include "my_globals.hpp"
#include "my_main.hpp"
#include "my_sdl_event.hpp"
#include "my_sdl_proto.hpp"
#include "my_string.hpp"
#include "my_wids.hpp"

#include <cerrno>
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <mutex>
#include <print>
#include <strings.h>
#include <unistd.h>

static void error_message_do(Gamep g, std::string &tech_support)
{
  TRACE();

  tech_support += "\n";

  if (g != nullptr) {
    const auto *seed_name = game_seed_name_get(g);
    if (*seed_name != 0) {
      tech_support += "Seed name: " + std::string(seed_name) + "\n";
    }
  }

  tech_support += "\n";
  tech_support += "Could you please email goblinhack@gmail.com and attach the following files and trace info?\n";
  tech_support += g_log_stdout_filename;
  tech_support += "\n";
  tech_support += g_log_stderr_filename;
  tech_support += "\n";
  tech_support += "\n";
  tech_support += backtrace_string();
  tech_support += "\n";
  tech_support += callstack_string();
  tech_support += "\n";
  tech_support += "The goblin responsible for this shall be punished!\n";

  con("%s", tech_support.c_str());

  if (g != nullptr) {
    wid_unset_focus(g);
    wid_unset_focus_lock();
    wid_console_raise(g);
  }

  if (g_dying) {
    //
    // Kill SDL windows as they hide behind fullscreen
    //
    sdl_display_fini(g);

    //
    // A terminal error
    //
    sdl_msg_box("%s", tech_support.c_str());
  } else if (sdl.window == nullptr) {
    //
    // An early error with no SDL windows yet
    //
    sdl_msg_box("%s", tech_support.c_str());
  }
}

void error_message(Gamep g, const std::string &error_msg)
{
  if (MY_STDOUT != stdout) {
    std::println(MY_STDOUT, "{}", error_msg);
  }

  std::println(stderr, "{}", error_msg);
  if (MY_STDERR != stderr) {
    std::println(MY_STDERR, "{}", error_msg);
  }

  fflush(stdout);
  fflush(stderr);
  fflush(MY_STDOUT);
  fflush(MY_STDERR);

  static std::mutex m;
  m.lock();
  g_error_last = error_msg + "\n " + callstack_string() + "\nLogfile: " + g_log_stderr_filename + ": ";
  m.unlock();

  if (g_thread_id != MAIN_THREAD) {
    callstack_dump(stderr);
    backtrace_dump(stderr);
    return;
  }

  if (g_opt_tests) {
    callstack_dump(MY_STDERR);
    backtrace_dump(MY_STDERR);
    return;
  }

  TRACE();

  std::string tech_support = "An error has occurred:\n";
  tech_support += error_msg;

  error_message_do(g, tech_support);
}

void error_clear(Gamep g)
{
  TRACE();

  g_errored_thread_id = -1;
  g_err_count         = 0;
}
