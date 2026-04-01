//
// Copyright goblinhack@gmail.com
//

#include "my_backtrace.hpp"
#include "my_callstack.hpp"
#include "my_console.hpp"
#include "my_globals.hpp"
#include "my_log.hpp"
#include "my_main.hpp"
#include "my_ptrcheck.hpp"
#include "my_string.hpp"
#include "my_tp.hpp"
#include "my_wids.hpp"

#include <cstring>

void tp_log_(Tpp tp, const char *fmt, va_list args)
{
  TRACE();

  VERIFY(MTYPE_TP, tp);
  char buf[ MAXLONGSTR ];
  int  len = 0;

  buf[ 0 ] = '\0';
  get_timestamp(buf, MAXLONGSTR);
  len = static_cast< int >(strlen(buf));

  IF_DEBUG
  {
    snprintf(buf + len, MAXLONGSTR - len, "[%-50s]: %*s", tp_name(tp).c_str(), g_callframes_indent, "");
    len = static_cast< int >(strlen(buf));
  }

  vsnprintf(buf + len, MAXLONGSTR - len, fmt, args);

  putf(MY_STDOUT, buf);
}

void tp_log(Tpp tp, const char *fmt, ...)
{
  TRACE();

  VERIFY(MTYPE_TP, tp);

  va_list args = {};
  va_start(args, fmt);
  tp_log_(tp, fmt, args);
  va_end(args);
}

void tp_dbg_(Tpp tp, const char *fmt, ...)
{
  TRACE();

  IF_NODEBUG { return; }

  VERIFY(MTYPE_TP, tp);

  va_list args = {};
  va_start(args, fmt);
  tp_log_(tp, fmt, args);
  va_end(args);
}

void tp_die_(Tpp tp, const char *fmt, va_list args)
{
  TRACE();

  VERIFY(MTYPE_TP, tp);
  char buf[ MAXLONGSTR ];
  int  len = 0;

  buf[ 0 ] = '\0';
  get_timestamp(buf, MAXLONGSTR);
  len = static_cast< int >(strlen(buf));
  snprintf(buf + len, MAXLONGSTR - len, "%s: ", tp_name(tp).c_str());

  len = static_cast< int >(strlen(buf));
  vsnprintf(buf + len, MAXLONGSTR - len, fmt, args);

  CROAK("%s", buf);
}

void tp_die(Tpp tp, const char *fmt, ...)
{
  TRACE();

  VERIFY(MTYPE_TP, tp);
  va_list args = {};

  va_start(args, fmt);
  tp_die_(tp, fmt, args);
  va_end(args);
}

void tp_con_(Tpp tp, const char *fmt, va_list args)
{
  TRACE();

  VERIFY(MTYPE_TP, tp);
  char buf[ MAXLONGSTR ];
  int  len = 0;

  buf[ 0 ] = '\0';
  get_timestamp(buf, MAXLONGSTR);
  len = static_cast< int >(strlen(buf));
  snprintf(buf + len, MAXLONGSTR - len, "%s: ", tp_name(tp).c_str());

  len = static_cast< int >(strlen(buf));
  vsnprintf(buf + len, MAXLONGSTR - len, fmt, args);

  putf(MY_STDOUT, buf);

  if (! g_opt_tests) {
    term_log(buf);
    putchar('\n');
  }

  wid_console_log(buf);
}

void tp_con(Tpp tp, const char *fmt, ...)
{
  TRACE();

  VERIFY(MTYPE_TP, tp);
  va_list args = {};

  va_start(args, fmt);
  tp_con_(tp, fmt, args);
  va_end(args);
}

void tp_err_(Tpp tp, const char *fmt, va_list args)
{
  char buf[ MAXLONGSTR ];
  int  len = 0;

  buf[ 0 ] = '\0';
  snprintf(buf + len, MAXLONGSTR - len, "%s: ", tp_name(tp).c_str());
  len = static_cast< int >(strlen(buf));
  vsnprintf(buf + len, MAXLONGSTR - len, fmt, args);

  ERR("%s", buf);
}

void tp_err(Tpp tp, const char *fmt, ...)
{
  TRACE();

  va_list args = {};
  va_start(args, fmt);
  tp_err_(tp, fmt, args);
  va_end(args);
}
