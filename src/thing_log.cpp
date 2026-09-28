//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_console.hpp"
#include "my_globals.hpp"
#include "my_log.hpp"
#include "my_main.hpp"
#include "my_string.hpp"
#include "my_thing.hpp"
#include "my_time.hpp"
#include "my_types.hpp"
#include "my_wids.hpp"

#include <cstdarg>
#include <cstdio>
#include <cstring>

static void thing_log_(Gamep g, Levelsp v, Levelp l, Thingp t, const char *fmt, va_list args)
{
  TRACE();

  char buf[ MAXLONGSTR ];
  int  len = 0;

  buf[ 0 ] = '\0';
  get_timestamp(buf, MAXLONGSTR);
  len = static_cast< int >(strlen(buf));
  snprintf(buf + len, MAXLONGSTR - len, "[%-60s]: %*s", to_string(g, v, l, t).c_str(), g_callframes_indent, "");
  len = static_cast< int >(strlen(buf));

  vsnprintf(buf + len, MAXLONGSTR - len, fmt, args);

  putf(MY_STDOUT, buf);
}

void thing_log(Gamep g, Levelsp v, Levelp l, Thingp t, const char *fmt, ...)
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
  thing_log_(g, v, l, t, fmt, args);
  va_end(args);
}

void thing_dbg(Gamep g, Levelsp v, Levelp l, Thingp t, const char *fmt, ...)
{
  TRACE();

  IF_NODEBUG return;

  va_list args = {};
  va_start(args, fmt);
  thing_log_(g, v, l, t, fmt, args);
  va_end(args);
}

static void thing_warn_(Gamep g, Levelsp v, Levelp l, Thingp t, const char *fmt, va_list args)
{
  TRACE();

  char buf[ MAXLONGSTR ];
  int  len = 0;

  buf[ 0 ] = '\0';
  get_timestamp(buf, MAXLONGSTR);
  len = static_cast< int >(strlen(buf));
  snprintf(buf + len, MAXLONGSTR - len, "%s: ", to_string(g, v, l, t).c_str());
  len = static_cast< int >(strlen(buf));
  vsnprintf(buf + len, MAXLONGSTR - len, fmt, args);

  putf(MY_STDOUT, buf);
  wid_console_log(buf);
}

void thing_warn(Gamep g, Levelsp v, Levelp l, Thingp t, const char *fmt, ...)
{
  TRACE();

  va_list args = {};

  va_start(args, fmt);
  thing_warn_(g, v, l, t, fmt, args);
  va_end(args);
}

static void thing_con_(Gamep g, Levelsp v, Levelp l, Thingp t, const char *fmt, va_list args)
{
  TRACE();

  char buf[ MAXLONGSTR ];
  int  len = 0;

  buf[ 0 ] = '\0';
  get_timestamp(buf, MAXLONGSTR);
  len = static_cast< int >(strlen(buf));
  snprintf(buf + len, MAXLONGSTR - len, "%s: ", to_string(g, v, l, t).c_str());
  len = static_cast< int >(strlen(buf));
  vsnprintf(buf + len, MAXLONGSTR - len, fmt, args);
  putf(MY_STDOUT, buf);

  if (! g_opt_tests) {
    term_log(buf);
    putchar('\n');
  }

  wid_console_log(buf);
}

void thing_con(Gamep g, Levelsp v, Levelp l, Thingp t, const char *fmt, ...)
{
  TRACE();

  va_list args = {};
  va_start(args, fmt);
  thing_con_(g, v, l, t, fmt, args);
  va_end(args);
}

static void thing_err_(Gamep g, Levelsp v, Levelp l, Thingp t, const char *fmt, va_list args)
{
  char buf[ MAXLONGSTR ];
  int  len = 0;

  buf[ 0 ] = '\0';
  snprintf(buf + len, MAXLONGSTR - len, "%s: ", to_string(g, v, l, t).c_str());
  len = static_cast< int >(strlen(buf));
  vsnprintf(buf + len, MAXLONGSTR - len, fmt, args);

  ERR("%s", buf);
}

void thing_err(Gamep g, Levelsp v, Levelp l, Thingp t, const char *fmt, ...)
{
  TRACE();

  va_list args = {};
  va_start(args, fmt);
  thing_err_(g, v, l, t, fmt, args);
  va_end(args);
}

static void thing_croak_(Gamep g, Levelsp v, Levelp l, Thingp t, const char *fmt, va_list args)
{
  char buf[ MAXLONGSTR ];
  int  len = 0;

  buf[ 0 ] = '\0';
  snprintf(buf + len, MAXLONGSTR - len, "%s: ", to_string(g, v, l, t).c_str());
  len = static_cast< int >(strlen(buf));
  vsnprintf(buf + len, MAXLONGSTR - len, fmt, args);

  CROAK("%s", buf);
}

void thing_croak(Gamep g, Levelsp v, Levelp l, Thingp t, const char *fmt, ...)
{
  TRACE();

  va_list args = {};
  va_start(args, fmt);
  thing_croak_(g, v, l, t, fmt, args);
  va_end(args);
}

static void thing_topcon_(Gamep g, Levelsp v, Levelp l, Thingp t, const char *fmt, va_list args)
{
  TRACE();

  char buf[ MAXLONGSTR ];
  int  len = 0;

  buf[ 0 ] = '\0';
  get_timestamp(buf, MAXLONGSTR);
  len = static_cast< int >(strlen(buf));
  snprintf(buf + len, MAXLONGSTR - len, "%s: ", to_string(g, v, l, t).c_str());
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

void thing_topcon(Gamep g, Levelsp v, Levelp l, Thingp t, const char *fmt, ...)
{
  TRACE();

  va_list args = {};

  va_start(args, fmt);
  thing_topcon_(g, v, l, t, fmt, args);
  va_end(args);
}

static void thing_botcon_(Gamep g, Levelsp v, Levelp l, Thingp t, const char *fmt, va_list args)
{
  TRACE();

  char buf[ MAXLONGSTR ];
  int  len = 0;

  buf[ 0 ] = '\0';
  get_timestamp(buf, MAXLONGSTR);
  len = static_cast< int >(strlen(buf));
  snprintf(buf + len, MAXLONGSTR - len, "%s: ", to_string(g, v, l, t).c_str());
  len = static_cast< int >(strlen(buf));
  vsnprintf(buf + len, MAXLONGSTR - len, fmt, args);

  wid_botcon_log(buf);
}

void thing_botcon(Gamep g, Levelsp v, Levelp l, Thingp t, const char *fmt, ...)
{
  TRACE();

  va_list args = {};

  va_start(args, fmt);
  thing_botcon_(g, v, l, t, fmt, args);
  va_end(args);
}
