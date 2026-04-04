//
// Copyright goblinhack@gmail.com
//

#include "my_globals.hpp"
#include "my_main.hpp"
#include "my_test.hpp"
#include "my_wids.hpp"

#include <csignal>
#include <cstring>
#include <iostream>
#include <strings.h>
#include <sys/stat.h>
#include <unistd.h>

#ifndef PATH_MAX
#define PATH_MAX 512
#endif

//
// Restart the game, with an optional restart argument to indicate how to restart
//
void restart(Gamep g, const std::string &restart_arg)
{
  con("Exiting, restart called");
  TRACE();

  char const *executable = nullptr;
  const char *argv[ MAXSHORTSTR /* in pointers, should be enough */ ];
  int         argc = 0;
  int         i    = 0;

  memset(argv, 0, sizeof(argv));

  //
  // Original program name
  //
  executable     = const_cast< char * >(g_program_name.c_str());
  argv[ argc++ ] = executable;

  //
  // Copy arguments and append any we need
  //
  for (i = 1; i < g_argc; i++) {
    if (strstr(g_argv[ i ], "-restart") != nullptr) {
      continue;
    }
    argv[ argc++ ] = g_argv[ i ];
  }
  argv[ argc++ ] = restart_arg.c_str();

  //
  // Build the full command line
  //
  std::string argument_line;
  log("command line arguments for restarting '%s'", executable);
  for (i = 1; i < argc; i++) {
    log("+ argument: \"%s\"", argv[ i ]);
    argument_line += ' ';
    argument_line += argv[ i ];
  }

  wid_console_flush(g);

  bool use_system = false;

#ifdef __linux__
  //
  // For some reason SDL audio fails if we use execve
  //
  use_system = true;
#elif _WIN32
  //
  // Windows has spaces in the path name and that ends up being incorrectly
  // split by execve on the 2nd boot. So, just avoid the issue.
  //
  executable = (char *) "sacrecant.exe";
  use_system = false;
#else
  use_system = false;
#endif

  cleanup();

  if (use_system) {
    char tmp_cmd[ PATH_MAX ];
    snprintf(tmp_cmd, SIZEOF(tmp_cmd), "%s &", argument_line.c_str());
    con("system(%s)", tmp_cmd);
    int const ret = system(tmp_cmd);
    exit(ret);
  }

  argv[ 0 ] = executable;
  con("execve(%s%s)", executable, argument_line.c_str());
  execve(executable, (char *const *) argv, nullptr);

  CROAK("failed to restart");
}
