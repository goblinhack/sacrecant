//
// Copyright goblinhack@gmail.com
//

#include "my_backtrace.hpp"
#include "my_callstack.hpp"
#include "my_console.hpp"
#include "my_globals.hpp"
#include "my_log.hpp"
#include "my_main.hpp"
#include "my_string.hpp"
#include "my_thing.hpp"
#include "my_wids.hpp"

#include <cstring>

static void thing_log_(Thingp t, const char *fmt, va_list args)
{
  TRACE();

  char buf[ MAXLONGSTR ];
  int  len = 0;

  buf[ 0 ] = '\0';
  get_timestamp(buf, MAXLONGSTR);
  len = static_cast< int >(strlen(buf));
  snprintf(buf + len, MAXLONGSTR - len, "[%-50s]: %*s", to_string(nullptr, nullptr, nullptr, t).c_str(), g_callframes_indent, "");
  len = static_cast< int >(strlen(buf));

  vsnprintf(buf + len, MAXLONGSTR - len, fmt, args);

  putf(MY_STDOUT, buf);
}

void thing_log(Thingp t, const char *fmt, ...)
{
  TRACE();

  //
  // Filter to interesting things
  //
  if (! thing_is_loggable(t)) {
    return;
  }

  va_list args = {};
  va_start(args, fmt);
  thing_log_(t, fmt, args);
  va_end(args);
}

void thing_dbg(Thingp t, const char *fmt, ...)
{
  TRACE();

  IF_NODEBUG return;

  va_list args = {};
  va_start(args, fmt);
  thing_log_(t, fmt, args);
  va_end(args);
}

static void thing_warn_(Thingp t, const char *fmt, va_list args)
{
  TRACE();

  char buf[ MAXLONGSTR ];
  int  len = 0;

  buf[ 0 ] = '\0';
  get_timestamp(buf, MAXLONGSTR);
  len = static_cast< int >(strlen(buf));
  snprintf(buf + len, MAXLONGSTR - len, "%s: ", to_string(nullptr, nullptr, nullptr, t).c_str());
  len = static_cast< int >(strlen(buf));
  vsnprintf(buf + len, MAXLONGSTR - len, fmt, args);

  putf(MY_STDOUT, buf);
  wid_console_log(buf);
}

void thing_warn(Thingp t, const char *fmt, ...)
{
  TRACE();

  va_list args = {};

  va_start(args, fmt);
  thing_warn_(t, fmt, args);
  va_end(args);
}

static void thing_con_(Thingp t, const char *fmt, va_list args)
{
  TRACE();

  char buf[ MAXLONGSTR ];
  int  len = 0;

  buf[ 0 ] = '\0';
  get_timestamp(buf, MAXLONGSTR);
  len = static_cast< int >(strlen(buf));
  snprintf(buf + len, MAXLONGSTR - len, "%s: ", to_string(nullptr, nullptr, nullptr, t).c_str());
  len = static_cast< int >(strlen(buf));
  vsnprintf(buf + len, MAXLONGSTR - len, fmt, args);
  putf(MY_STDOUT, buf);

  if (! g_opt_tests) {
    term_log(buf);
    putchar('\n');
  }

  wid_console_log(buf);
}

void thing_con(Thingp t, const char *fmt, ...)
{
  TRACE();

  va_list args = {};
  va_start(args, fmt);
  thing_con_(t, fmt, args);
  va_end(args);
}

static void thing_err_(Thingp t, const char *fmt, va_list args)
{
  char buf[ MAXLONGSTR ];
  int  len = 0;

  buf[ 0 ] = '\0';
  snprintf(buf + len, MAXLONGSTR - len, "%s: ", to_string(nullptr, nullptr, nullptr, t).c_str());
  len = static_cast< int >(strlen(buf));
  vsnprintf(buf + len, MAXLONGSTR - len, fmt, args);

  ERR("%s", buf);
}

void thing_err(Thingp t, const char *fmt, ...)
{
  TRACE();

  va_list args = {};
  va_start(args, fmt);
  thing_err_(t, fmt, args);
  va_end(args);
}

static void thing_croak_(Thingp t, const char *fmt, va_list args)
{
  char buf[ MAXLONGSTR ];
  int  len = 0;

  buf[ 0 ] = '\0';
  snprintf(buf + len, MAXLONGSTR - len, "%s: ", to_string(nullptr, nullptr, nullptr, t).c_str());
  len = static_cast< int >(strlen(buf));
  vsnprintf(buf + len, MAXLONGSTR - len, fmt, args);

  CROAK("%s", buf);
}

void thing_croak(Thingp t, const char *fmt, ...)
{
  TRACE();

  va_list args = {};
  va_start(args, fmt);
  thing_croak_(t, fmt, args);
  va_end(args);
}

static void thing_topcon_(Thingp t, const char *fmt, va_list args)
{
  TRACE();

  char buf[ MAXLONGSTR ];
  int  len = 0;

  buf[ 0 ] = '\0';
  get_timestamp(buf, MAXLONGSTR);
  len = static_cast< int >(strlen(buf));
  snprintf(buf + len, MAXLONGSTR - len, "%s: ", to_string(nullptr, nullptr, nullptr, t).c_str());
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

void thing_topcon(Thingp t, const char *fmt, ...)
{
  TRACE();

  va_list args = {};

  va_start(args, fmt);
  thing_topcon_(t, fmt, args);
  va_end(args);
}

static void thing_botcon_(Thingp t, const char *fmt, va_list args)
{
  TRACE();

  char buf[ MAXLONGSTR ];
  int  len = 0;

  buf[ 0 ] = '\0';
  get_timestamp(buf, MAXLONGSTR);
  len = static_cast< int >(strlen(buf));
  snprintf(buf + len, MAXLONGSTR - len, "%s: ", to_string(nullptr, nullptr, nullptr, t).c_str());
  len = static_cast< int >(strlen(buf));
  vsnprintf(buf + len, MAXLONGSTR - len, fmt, args);

  wid_botcon_log(buf);
}

void thing_botcon(Thingp t, const char *fmt, ...)
{
  TRACE();

  va_list args = {};

  va_start(args, fmt);
  thing_botcon_(t, fmt, args);
  va_end(args);
}
