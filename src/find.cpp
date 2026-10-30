//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_dir.hpp"
#include "my_file.hpp"
#include "my_globals.hpp"
#include "my_main.hpp"
#include "my_ptrcheck.hpp"
#include "my_sprintf.hpp"
#include "my_string.hpp"
#include "my_types.hpp"

#include <cmath>
#include <cstdlib>
#include <cstring>
#include <libgen.h>
#include <string>

//
// Find the binary we are running.
//
static void find_executable()
{
  TRACE();

  char       *parent_dir = nullptr;
  std::string exec_name;
  char       *path = nullptr;
  char       *tmp  = nullptr;

  exec_name = mybasename(g_argv[ 0 ], __FUNCTION__);
  log("INIT: will use EXEC_NAME as '%s'", exec_name.c_str());

  //
  // Get the current directory, ending in a single /
  //
  std::string curr_dir = string_sprintf("%s" DIR_SEP, dir_dot());
  replace(curr_dir, DIR_SEP DIR_SEP, DIR_SEP);

  //
  // Get the parent directory, ending in a single /
  //
  parent_dir = dynprintf("%s" DIR_SEP, dir_dotdot(dir_dot()));
  tmp        = STRSUB(parent_dir, DIR_SEP DIR_SEP, DIR_SEP, "parent_dir");
  MYFREE(parent_dir);
  parent_dir = tmp;

  //
  // Get rid of ../ from the program name and replace with the path.
  //
  std::string exec_expanded_name = std::string(g_argv[ 0 ]);
  if (exec_expanded_name[ 0 ] == '.') {
    replace(exec_expanded_name, ".." DIR_SEP, parent_dir);
  }

  //
  // Get rid of ./ from the program name.
  //
  if (exec_expanded_name[ 0 ] == '.') {
    replace(exec_expanded_name, "." DIR_SEP, "");
  }

  //
  // Get rid of any // from the path
  //
  replace(exec_expanded_name, DIR_SEP DIR_SEP, DIR_SEP);

  //
  // Look in the simplest case first.
  //
  g_exec_full_path_and_name = string_sprintf("%s%s", curr_dir.c_str(), exec_name.c_str());
  if (static_cast< bool >(file_exists(g_exec_full_path_and_name.c_str()))) {
    g_exec_dir = std::string(curr_dir);
    goto cleanup;
  }

  //
  // Try the parent dir.
  //
  g_exec_full_path_and_name = string_sprintf("%s%s", parent_dir, exec_name.c_str());
  if (static_cast< bool >(file_exists(g_exec_full_path_and_name.c_str()))) {
    g_exec_dir = std::string(parent_dir);
    goto cleanup;
  }

  //
  // Try the PATH.
  //
  path = getenv("PATH");
  if (path != nullptr) {
    char const *dir = nullptr;

    path = MYDUPSTR(path, "path");

    for (dir = strtok(path, PATHSEP); dir != nullptr; dir = strtok(nullptr, PATHSEP)) {
      g_exec_full_path_and_name = string_sprintf("%s" DIR_SEP "%s", dir, exec_name.c_str());
      if (static_cast< bool >(file_exists(g_exec_full_path_and_name.c_str()))) {
        g_exec_dir = dynprintf("%s" DIR_SEP, dir);
        goto cleanup;
      }
    }

    MYFREE(path);
    path = nullptr;
  }

  g_exec_full_path_and_name = std::string(exec_expanded_name);
  g_exec_dir                = std::string(exec_expanded_name);

cleanup:
  auto *new_g_exec_dir = STRSUB(g_exec_dir.c_str(), "/", DIR_SEP, "g_exec_dir");
  g_exec_dir           = std::string(new_g_exec_dir);

  DBG2("g_exec_dir set to %s", g_exec_dir.c_str());
  DBG2("Parent dir  : \"%s\"", parent_dir);
  DBG2("Curr dir    : \"%s\"", curr_dir.c_str());
  DBG2("Full name   : \"%s\"", exec_expanded_name.c_str());

  if (path != nullptr) {
    MYFREE(path);
  }

  if (parent_dir != nullptr) {
    MYFREE(parent_dir);
  }
}

//
// Find all installed file locations.
//
static void find_exec_dir()
{
  TRACE();

  find_executable();

  //
  // Make sure the exec dir ends in a /
  //
  g_exec_dir = string_sprintf("%s" DIR_SEP, g_exec_dir.c_str());
  replace(g_exec_dir, "//", DIR_SEP);
  replace(g_exec_dir, "\\\\", DIR_SEP);
  replace(g_exec_dir, "/", DIR_SEP);
  replace(g_exec_dir, "\\", DIR_SEP);
}

//
// Hunt down the data/ dir.
//
static void find_data_dir()
{
  TRACE();

  g_data_path = string_sprintf("%sdata" DIR_SEP, g_exec_dir.c_str());
  if (dir_exists(g_data_path.c_str())) {
    return;
  }

  g_data_path = string_sprintf(".." DIR_SEP "data");
  if (dir_exists(g_data_path.c_str())) {
    return;
  }

  g_data_path = g_exec_dir;
}

//
// Hunt down the gfx/ dir.
//
static void find_gfx_dir()
{
  TRACE();

  g_gfx_path = string_sprintf("%sdata" DIR_SEP "gfx" DIR_SEP, g_exec_dir.c_str());
  if (dir_exists(g_gfx_path.c_str())) {
    return;
  }

  g_gfx_path = string_sprintf(".." DIR_SEP "gfx" DIR_SEP);
  if (dir_exists(g_gfx_path.c_str())) {
    return;
  }

  g_gfx_path = g_exec_dir;
}

//
// Find all installed file locations.
//
void find_file_locations()
{
  TRACE();

  find_exec_dir();
  find_data_dir();
  find_gfx_dir();

  log("INIT: exec path   : \"%s\"", g_exec_dir.c_str());
  log("INIT: data path   : \"%s\"", g_data_path.c_str());
  log("INIT: gfx path    : \"%s\"", g_gfx_path.c_str());
}
