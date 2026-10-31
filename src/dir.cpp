//
// Copyright goblinhack@gmail.com
//

#include <cstdint>
#include <cstring>
#include <sys/param.h>
#include <sys/stat.h>
#include <unistd.h>

#include "my_callstack.hpp"
#include "my_dir.hpp"
#include "my_ptrcheck.hpp"
#include "my_string.hpp"
#include "my_types.hpp"

/*
 * Does the requested dir exist?
 */
[[nodiscard]] auto dir_exists(const std::string &in) -> bool
{
  TRACE();

  std::string tmp = std::string(in);
  struct stat buf {};

  /*
   * msys is crap and unless you remove the / from the end of the name
   * will not think it is a dir.
   */
  remove_trailing(tmp, DIR_SEP_CHAR);

  auto rc = 0;
  if (stat(tmp.c_str(), &buf) >= 0) {
    rc = S_ISDIR(buf.st_mode);
  }

  return static_cast< bool >(rc);
}

/*
 * Given a directory, find out what ./ is.
 */
[[nodiscard]] auto dir_dot() -> char *
{
  TRACE();

  static char buf[ MAXPATHLEN ];

  (void) dospath2unix(getcwd(buf, SIZEOF(buf)));

  return buf;
}

/*
 * Remove \ and change to /
 * Given a directory, find out what ../ is.
 */
[[nodiscard]] auto dir_dotdot(char *s) -> char *
{
  TRACE();

  uint32_t size = 0;
  char    *end  = nullptr;

  size = static_cast< uint32_t >(strlen(s));
  if (size <= 1) {
    return s;
  }

  /*
   * Skip trailing /
   */
  end = s + size - 2;
  while ((end >= s) && (*end != '/') && (*end != '\\')) {
    end--;
  }

  *(end + 1) = '\0';

  return dospath2unix(s);
}

/*
 * Remove \ and change to /
 */
[[nodiscard]] auto dospath2unix(char *in) -> char *
{
  TRACE();

  char *s = in;

  while (*s != 0) {
    if (*s == '\\') {
      *s = '/';
    }

    s++;
  }

  return in;
}
