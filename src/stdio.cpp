//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_globals.hpp"
#include "my_main.hpp"
#include "my_sprintf.hpp"

#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <print>

//
// Remap stdout to a file of our choosing.
//
// This can be done for a thread also.
//
auto redirect_stdout() -> FILE *
{
#if 0
  //
  // Useful if github tests fail
  //
  g_log_stdout = stdout;
#endif

  [[likely]] if ((g_log_stdout != nullptr)) {
    return g_log_stdout;
  }

  TRACE();

  const char *appdata = nullptr;
  appdata             = getenv("APPDATA");
  if ((appdata == nullptr) || (appdata[ 0 ] == 0)) {
    appdata = "appdata";
  }

  std::string out;
  if (g_thread_id == MAIN_THREAD) {
    out = std::format("{}{}{}{}{}", appdata, DIR_SEP, "sacrecant", DIR_SEP, "stdout.txt");
  } else {
    out = std::format("{}{}{}{}{}.level.{}", appdata, DIR_SEP, "sacrecant", DIR_SEP, "stdout.txt", g_thread_id);
  }
  g_log_stdout_filename = out;
  g_log_stdout          = fopen(out.c_str(), "w+");

  //
  // If we failed to create, try a local file
  //
  if (g_log_stdout == nullptr) {
    if (g_thread_id == MAIN_THREAD) {
      out = "sacrecant.stdout.txt";
    } else {
      out = std::format("{}.level.{}", "sacrecant.stdout.txt", g_thread_id);
    }
    g_log_stdout_filename = out;
    g_log_stdout          = fopen(out.c_str(), "w+");

    //
    // Last resort
    //
    if (g_log_stdout == nullptr) {
      std::println(stderr, "failed to create stdout log file \"{}\" for thread {}, error: {}", out, g_thread_id, strerror(errno));
      g_log_stdout = stdout;
    }
  }

  return g_log_stdout;
}

//
// Remap stderr to a file of our choosing.
//
// This can be done for a thread also.
//
auto redirect_stderr() -> FILE *
{
#if 0
  //
  // Useful if github tests fail
  //
  g_log_stderr = stderr;
#endif

  [[likely]] if ((g_log_stderr != nullptr)) {
    return g_log_stderr;
  }

  TRACE();

  const char *appdata = nullptr;
  appdata             = getenv("APPDATA");
  if ((appdata == nullptr) || (appdata[ 0 ] == 0)) {
    appdata = "appdata";
  }

  std::string out;
  if (g_thread_id == MAIN_THREAD) {
    out = std::format("{}{}{}{}{}", appdata, DIR_SEP, "sacrecant", DIR_SEP, "stderr.txt");
  } else {
    out = std::format("{}{}{}{}{}.level.{}", appdata, DIR_SEP, "sacrecant", DIR_SEP, "stderr.txt", g_thread_id);
  }
  g_log_stderr_filename = out;
  g_log_stderr          = fopen(out.c_str(), "w+");

  //
  // If we failed to create, try a local file
  //
  if (g_log_stderr == nullptr) {
    if (g_thread_id == MAIN_THREAD) {
      out = "sacrecant.stderr.txt";
    } else {
      out = std::format("{}.level.{}", "sacrecant.stderr.txt", g_thread_id);
    }
    g_log_stderr_filename = out;
    g_log_stderr          = fopen(out.c_str(), "w+");

    //
    // Last resort
    //
    if (g_log_stderr == nullptr) {
      std::println(stderr, "failed to create stderr log file \"{}\" for thread {}, error: {}", out, g_thread_id, strerror(errno));
      g_log_stderr = stderr;
    }
  }

  return g_log_stderr;
}

//
// Close the per thread file handle
//
void close_stdout()
{
  if ((g_log_stdout != nullptr) && (g_log_stdout != stdout)) {
    fclose(g_log_stdout);
    g_log_stdout = nullptr;
  }
}

//
// Close the per thread file handle
//
void close_stderr()
{
  if ((g_log_stderr != nullptr) && (g_log_stderr != stderr)) {
    fclose(g_log_stderr);
    g_log_stderr = nullptr;
  }
}
