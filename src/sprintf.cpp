//
// Copyright goblinhack@gmail.com
//

#include "my_sprintf.hpp"

#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <stdio.h>
#include <string>

// https://stackoverflow.com/questions/40159892/using-asprintf-on-windows

#if defined(__WIN64__) || defined(_WIN64) || defined(WIN64) || defined(__WIN32__) || defined(_WIN32) || defined(WIN32)                          \
    || defined(__WINDOWS_386__) || defined(__CYGWIN__)
#ifndef _vscprintf
/* For some reason, MSVC fails to honour this #ifndef. */
/* Hence function renamed to _vscprintf_so(). */
static int _vscprintf_so(const char *format, va_list pargs)
{
  int     retval;
  va_list argcopy;
  va_copy(argcopy, pargs);
  retval = vsnprintf(nullptr, 0, format, argcopy);
  va_end(argcopy);
  return retval;
}
#endif // _vscprintf

#ifndef vasprintf
int vasprintf(char **strp, const char *fmt, va_list ap)
{
  int len = _vscprintf_so(fmt, ap);
  if (len == -1)
    return -1;
  char *str = (char *) malloc((size_t) len + 1);
  if (! str)
    return -1;
  int r = vsnprintf(str, len + 1, fmt, ap); /* "secure" version of vsprintf */
  if (r == -1)
    return free(str), -1;
  *strp = str;
  return r;
}
#endif // vasprintf

#ifndef asprintf
int asprintf(char *strp[], const char *fmt, ...)
{
  va_list ap;
  va_start(ap, fmt);
  int r = vasprintf(strp, fmt, ap);
  va_end(ap);
  return r;
}
#endif // asprintf
#endif

[[nodiscard]] auto string_sprintf(const char *format, ...) -> std::string
{
  va_list args = {};
  char   *buf  = nullptr;

  va_start(args, format);

  if (vasprintf(&buf, format, args) == -1) {
    throw std::bad_alloc();
  }

  std::string ret = buf;
  free(buf);

  va_end(args);

  return ret;
}

[[nodiscard]] auto string_sprintf(const char *format, va_list args) -> std::string
{
  char *buf = nullptr;

  if (asprintf(&buf, format, args) == -1) {
    throw std::bad_alloc();
  }

  std::string ret = buf;
  free(buf);

  return ret;
}

[[nodiscard]] auto string_append_with_comma(const std::string &in, const std::string &add) -> std::string
{
  if (add.empty()) {
    return in;
  }

  if (in.empty()) {
    return add;
  }
  return in + ", " + add;
}

[[nodiscard]] auto string_sprintf_append_with_comma(const std::string &in, const char *format, va_list args) -> std::string
{
  char *buf = nullptr;

  if (asprintf(&buf, format, args) == -1) {
    throw std::bad_alloc();
  }

  std::string const ret = buf;
  free(buf);

  return string_append_with_comma(in, ret);
}

[[nodiscard]] auto string_sprintf_append_with_comma(const std::string &in, const char *format, ...) -> std::string
{
  va_list args = {};
  char   *buf  = nullptr;

  va_start(args, format);

  if (vasprintf(&buf, format, args) == -1) {
    throw std::bad_alloc();
  }

  std::string const ret = buf;
  free(buf);

  va_end(args);

  return string_append_with_comma(in, ret);
}
