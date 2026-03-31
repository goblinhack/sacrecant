//
// Copyright goblinhack@gmail.com
//

#include "my_ascii.hpp"
#include "my_backtrace.hpp"
#include "my_callstack.hpp"
#include "my_console.hpp"
#include "my_globals.hpp"
#include "my_log.hpp"
#include "my_main.hpp"
#include "my_ptrcheck.hpp"
#include "my_string.hpp"
#include "my_wids.hpp"

#include <cstdlib>
#include <mutex>
#include <print>
#include <sys/stat.h>

extern Game *game;

//
// Where all logs go
//
auto log_dir_create() -> std::string
{
  TRACE();

  const char *appdata = nullptr;
  appdata             = getenv("APPDATA");
  if ((appdata == nullptr) || (appdata[ 0 ] == 0)) {
    appdata = "appdata";
  }

#ifdef _WIN32
  mkdir(appdata);
#else
  mkdir(appdata, 0700);
#endif

  char *dir = dynprintf("%s%s%s", appdata, DIR_SEP, "gorget");
#ifdef _WIN32
  mkdir(dir);
#else
  mkdir(dir, 0700);
#endif

  MYFREE(dir);

  return {appdata};
}

void log_(const char *fmt, va_list args)
{
  TRACE_DEBUG();

  if (MY_STDOUT == stdout) {
    return;
  }

  char buf[ MAXLONGSTR ];
  buf[ 0 ] = '\0';
  int len  = 0;

  get_timestamp(buf, MAXLONGSTR);
  len = static_cast< int >(strlen(buf));

  IF_DEBUG
  {
    snprintf(buf + len, MAXLONGSTR - len, "[%-50s]: %s%*s", "", "", g_callframes_indent, "");
    len = static_cast< int >(strlen(buf));
  }

  vsnprintf(buf + len, MAXLONGSTR - len, fmt, args);

  putf(MY_STDOUT, buf);
}

void log(const char *fmt, ...)
{
  TRACE_DEBUG();

  va_list args = {};
  va_start(args, fmt);
  log_(fmt, args);
  va_end(args);
}

static void warn_(const char *fmt, va_list args)
{
  TRACE_DEBUG();

  char buf[ MAXLONGSTR ];
  buf[ 0 ] = '\0';
  int len  = 0;

  get_timestamp(buf, MAXLONGSTR);
  len = static_cast< int >(strlen(buf));
  vsnprintf(buf + len, MAXLONGSTR - len, fmt, args);

  putf(MY_STDOUT, buf);

  wid_console_log(buf);
}

void warn(const char *fmt, ...)
{
  TRACE_DEBUG();

  va_list args = {};

  va_start(args, fmt);
  warn_(fmt, args);
  va_end(args);
}

static void con_(const char *fmt, va_list args)
{
  TRACE_DEBUG();

  char buf[ MAXLONGSTR ];
  buf[ 0 ] = '\0';
  int len  = 0;

  get_timestamp(buf, MAXLONGSTR);
  len = static_cast< int >(strlen(buf));
  if (fmt != nullptr) {
    vsnprintf(buf + len, MAXLONGSTR - len, fmt, args);
  }

  putf(MY_STDOUT, buf);

  if (! g_opt_tests) {
    term_log(buf);
    putchar('\n');
  }

  wid_console_log(buf);
}

void con(const char *fmt, ...)
{
  TRACE_DEBUG();

  va_list args = {};
  va_start(args, fmt);
  con_(fmt, args);
  va_end(args);
}

void con_newline()
{
  TRACE_DEBUG();

  va_list args = {};
  con_(nullptr, args);
}

static void croak_handle(bool clean, const char *fmt, va_list args)
{
  TRACE();

  auto *g = game;

  if (g_dying) {
    //
    // Could be a different thread also crashing.
    //
    std::println(stderr, "Nested dying loop, exiting");
    exit(1);
  }
  g_dying = true;

  //
  // std::println seems to need the whoel buffer initialized
  //
  char buf[ MAXLONGSTR * 10 ] = {};
  int  len                    = 0;

  get_timestamp(buf, MAXLONGSTR);
  len = static_cast< int >(strlen(buf));

  if (! g_opt_test_current.empty()) {
    snprintf(buf + len, MAXLONGSTR - len, "test %s: ", g_opt_test_current.c_str());
    len = static_cast< int >(strlen(buf));
  }

  if (clean) {
    snprintf(buf + len, MAXLONGSTR - len, "EXITED: ");
  } else {
    snprintf(buf + len, MAXLONGSTR - len, "FATAL ERROR: ");
  }
  len = static_cast< int >(strlen(buf));
  vsnprintf(buf + len, MAXLONGSTR - len, fmt, args);

  if (clean) {
    std::println(stderr, "{}", buf);
  } else {
    if (MY_STDERR != stderr) {
      std::println(MY_STDERR, "{}", buf);
    }
    error_message(g, buf);
  }
}

void croak_handler(bool clean, const char *fmt, ...)
{
  TRACE();

  va_list args = {};

  if (! clean) {
    g_errored_thread_id = g_thread_id;
  }

  va_start(args, fmt);
  croak_handle(clean, fmt, args);
  va_end(args);
}

static void err_handle(Gamep g, const char *fmt, va_list args)
{
  TRACE();

  char buf[ MAXLONGSTR * 10 ];
  buf[ 0 ] = '\0';
  int len  = 0;

  get_timestamp(buf, MAXLONGSTR);
  len = static_cast< int >(strlen(buf));

  if (! g_opt_test_current.empty()) {
    snprintf(buf + len, MAXLONGSTR - len, "test %s: ", g_opt_test_current.c_str());
    len = static_cast< int >(strlen(buf));
  }

  snprintf(buf + len, MAXLONGSTR - len, "ERROR: ");
  len = static_cast< int >(strlen(buf));
  vsnprintf(buf + len, MAXLONGSTR - len, fmt, args);

  error_message(g, buf);
}

void err_handler(const char *fmt, ...)
{
  TRACE();

  if (g_err_count++ > ENABLE_MAX_ERR_COUNT) {
    CROAK("Too many errors: %d, max %d", g_err_count, ENABLE_MAX_ERR_COUNT);
    exit(1);
  }

  auto *g             = game;
  g_errored_thread_id = g_thread_id;

  va_list args = {};
  va_start(args, fmt);
  err_handle(g, fmt, args);
  va_end(args);
}

void putf(FILE *fp, const char *s)
{
  TRACE_DEBUG();

  const auto *sp = s;
  while (*sp != 0) {
    if (*sp == '%') {
      std::string const out = ascii_strip(s);
      fputs(out.c_str(), fp);
      putc('\n', fp);
      return;
    }
    sp++;
  }

  fputs(s, fp);
  putc('\n', fp);
}

static void topcon_(const char *fmt, va_list args)
{
  TRACE_DEBUG();

  char buf[ MAXLONGSTR ];
  buf[ 0 ] = '\0';
  char ts[ MAXLONGSTR / 2 ];
  int  len = 0;

  get_timestamp(ts, MAXLONGSTR);
  snprintf(buf, SIZEOF(buf) - 1, "%s", ts);
  len = static_cast< int >(strlen(buf));
  if (fmt != nullptr) {
    vsnprintf(buf + len, MAXLONGSTR - len, fmt, args);
  }

  putf(MY_STDOUT, buf);

  if (! g_opt_tests) {
    term_log(buf);
    putchar('\n');
  }

  wid_topcon_log(buf + len);
  wid_console_log(buf + len);
}

void topcon(const char *fmt, ...)
{
  TRACE_DEBUG();

  va_list args = {};

  va_start(args, fmt);
  topcon_(fmt, args);
  va_end(args);
}

void topcon_newline()
{
  TRACE_DEBUG();

  va_list args = {};
  topcon_(nullptr, args);
}

static void botcon_(const char *fmt, va_list args)
{
  TRACE_DEBUG();

  char buf[ MAXLONGSTR ];
  buf[ 0 ] = '\0';

  if (fmt != nullptr) {
    vsnprintf(buf, MAXLONGSTR, fmt, args);
  }

  wid_botcon_log(buf);
}

void botcon(const char *fmt, ...)
{
  TRACE_DEBUG();

  va_list args = {};

  va_start(args, fmt);
  botcon_(fmt, args);
  va_end(args);
}

void botcon_newline()
{
  TRACE_DEBUG();

  va_list args = {};

  botcon_(nullptr, args);
}
