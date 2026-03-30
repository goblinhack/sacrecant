//
// Copyright goblinhack@gmail.com
//

#include "my_backtrace.hpp"
#include "my_callstack.hpp"
#include "my_console.hpp"
#include "my_globals.hpp"
#include "my_level.hpp"
#include "my_log.hpp"
#include "my_main.hpp"
#include "my_string.hpp"
#include "my_wids.hpp"

#include <cstring>

static void level_log_(Gamep g, Levelsp v, Levelp l, const char *fmt, va_list args)
{
  TRACE();

  char buf[ MAXLONGSTR ];
  int  len = 0;

  buf[ 0 ] = '\0';
  get_timestamp(buf, MAXLONGSTR);

  IF_DEBUG
  {
    len = static_cast< int >(strlen(buf));
    snprintf(buf + len, MAXLONGSTR - len, "[%-50s]: %s: ", "", to_string(g, v, l).c_str());
  }
  else
  {
    len = static_cast< int >(strlen(buf));
    snprintf(buf + len, MAXLONGSTR - len, "%s: ", to_string(g, v, l).c_str());
  }

  len = static_cast< int >(strlen(buf));
  vsnprintf(buf + len, MAXLONGSTR - len, fmt, args);

  putf(MY_STDOUT, buf);
}

void level_log(Gamep g, Levelsp v, Levelp l, const char *fmt, ...)
{
  TRACE();

  va_list args = {};
  va_start(args, fmt);
  level_log_(g, v, l, fmt, args);
  va_end(args);
}

void level_dbg(Gamep g, Levelsp v, Levelp l, const char *fmt, ...)
{
  TRACE();

  IF_NODEBUG return;

  va_list args = {};
  va_start(args, fmt);
  level_log_(g, v, l, fmt, args);
  va_end(args);
}

static void level_warn_(Gamep g, Levelsp v, Levelp l, const char *fmt, va_list args)
{
  TRACE();

  char buf[ MAXLONGSTR ];
  int  len = 0;

  buf[ 0 ] = '\0';
  get_timestamp(buf, MAXLONGSTR);
  len = static_cast< int >(strlen(buf));
  snprintf(buf + len, MAXLONGSTR - len, "%s: ", to_string(g, v, l).c_str());
  len = static_cast< int >(strlen(buf));
  vsnprintf(buf + len, MAXLONGSTR - len, fmt, args);

  putf(MY_STDOUT, buf);
  wid_console_log(buf);
}

void level_warn(Gamep g, Levelsp v, Levelp l, const char *fmt, ...)
{
  TRACE();

  va_list args = {};

  va_start(args, fmt);
  level_warn_(g, v, l, fmt, args);
  va_end(args);
}

static void level_con_(Gamep g, Levelsp v, Levelp l, const char *fmt, va_list args)
{
  TRACE();

  char buf[ MAXLONGSTR ];
  int  len = 0;

  buf[ 0 ] = '\0';
  get_timestamp(buf, MAXLONGSTR);
  len = static_cast< int >(strlen(buf));
  snprintf(buf + len, MAXLONGSTR - len, "%s: ", to_string(g, v, l).c_str());
  len = static_cast< int >(strlen(buf));
  vsnprintf(buf + len, MAXLONGSTR - len, fmt, args);
  putf(MY_STDOUT, buf);

  if (! g_opt_tests) {
    term_log(buf);
    putchar('\n');
  }

  wid_console_log(buf);
}

void level_con(Gamep g, Levelsp v, Levelp l, const char *fmt, ...)
{
  TRACE();

  va_list args = {};
  va_start(args, fmt);
  level_con_(g, v, l, fmt, args);
  va_end(args);
}

static void level_err_(Gamep g, Levelsp v, Levelp l, const char *fmt, va_list args)
{
  char buf[ MAXLONGSTR ];
  int  len = 0;

  buf[ 0 ] = '\0';
  snprintf(buf + len, MAXLONGSTR - len, "%s: ", to_string(g, v, l).c_str());
  len = static_cast< int >(strlen(buf));
  vsnprintf(buf + len, MAXLONGSTR - len, fmt, args);

  ERR("%s", buf);
}

void level_err(Gamep g, Levelsp v, Levelp l, const char *fmt, ...)
{
  TRACE();

  va_list args = {};
  va_start(args, fmt);
  level_err_(g, v, l, fmt, args);
  va_end(args);
}

static void level_topcon_(Gamep g, Levelsp v, Levelp l, const char *fmt, va_list args)
{
  TRACE();

  char buf[ MAXLONGSTR ];
  int  len = 0;

  buf[ 0 ] = '\0';
  get_timestamp(buf, MAXLONGSTR);
  len = static_cast< int >(strlen(buf));
  snprintf(buf + len, MAXLONGSTR - len, "%s: ", to_string(g, v, l).c_str());
  len = static_cast< int >(strlen(buf));
  vsnprintf(buf + len, MAXLONGSTR - len, fmt, args);

  putf(MY_STDOUT, buf);

  if (! g_opt_tests) {
    term_log(buf);
    putchar('\n');
  }

  wid_topcon_log(buf);
  wid_console_log(buf);
}

void level_topcon(Gamep g, Levelsp v, Levelp l, const char *fmt, ...)
{
  TRACE();

  va_list args = {};

  va_start(args, fmt);
  level_topcon_(g, v, l, fmt, args);
  va_end(args);
}

static void level_botcon_(Gamep g, Levelsp v, Levelp l, const char *fmt, va_list args)
{
  TRACE();

  char buf[ MAXLONGSTR ];
  int  len = 0;

  buf[ 0 ] = '\0';
  get_timestamp(buf, MAXLONGSTR);
  len = static_cast< int >(strlen(buf));
  snprintf(buf + len, MAXLONGSTR - len, "%s: ", to_string(g, v, l).c_str());
  len = static_cast< int >(strlen(buf));
  vsnprintf(buf + len, MAXLONGSTR - len, fmt, args);

  wid_botcon_log(buf);
}

void level_botcon(Gamep g, Levelsp v, Levelp l, const char *fmt, ...)
{
  TRACE();

  va_list args = {};

  va_start(args, fmt);
  level_botcon_(g, v, l, fmt, args);
  va_end(args);
}
