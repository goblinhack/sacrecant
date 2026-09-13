//
// Copyright goblinhack@gmail.com
//

#include "my_globals.hpp"
#include "my_string.hpp"
#include "my_time.hpp"
#include "my_types.hpp"

#include <SDL_timer.h>
#include <cstdio>
#include <ctime>
#include <string>

//
// This is the actual time
//
ts_t game_time_now;
ts_t user_visible_time_now;

static char buf_[ MAXSHORTSTR ];

[[nodiscard]] auto user_visible_time_ms_cached() -> ts_t { return user_visible_time_now; }

[[nodiscard]] auto user_visible_time_ms() -> ts_t
{
  user_visible_time_now = SDL_GetTicks();

  return user_visible_time_now;
}

[[nodiscard]] auto game_time_ms_cached() -> ts_t { return game_time_now; }

[[nodiscard]] auto game_time_ms() -> ts_t
{
  if (g_opt_tests) {
    game_time_now++;
  } else {
    game_time_now = user_visible_time_ms();
  }

  return game_time_now;
}

[[nodiscard]] auto time2str(ts_t ms, char *buf, int len) -> const char *
{
  int       log_msec = ms;
  int       log_secs = log_msec / ONESEC;
  int       log_mins = log_secs / 60;
  int const log_hrs  = log_mins / 60;

  log_msec = log_msec % ONESEC;
  log_secs = log_secs % 60;
  log_mins = log_mins % 60;

  if (buf == nullptr) {
    buf = buf_;
    len = SIZEOF(buf_);
  }

  buf[ 0 ] = '\0';
  snprintf(buf, len, "%02d:%02d:%02d.%03d: ", log_hrs, log_mins, log_secs, log_msec);

  if (buf != nullptr) {
    return buf;
  }

  return buf_;
}

[[nodiscard]] auto timestamp(char *buf, int len) -> const char *
{
  //
  // Beware, changing this to game_time_ms() will have an impact on tests as we use game_time_ms()
  // hacked to a constant time
  //
  int       log_msec = user_visible_time_ms();
  int       log_secs = log_msec / ONESEC;
  int       log_mins = log_secs / 60;
  int const log_hrs  = log_mins / 60;

  log_msec = log_msec % ONESEC;
  log_secs = log_secs % 60;
  log_mins = log_mins % 60;

  if (buf == nullptr) {
    buf = buf_;
    len = SIZEOF(buf_);
  }

  buf[ 0 ] = '\0';
  snprintf(buf, len, "%02d:%02d:%02d.%03d: ", log_hrs, log_mins, log_secs, log_msec);

  if (buf != nullptr) {
    return buf;
  }

  return buf_;
}

[[nodiscard]] auto game_time_have_x_hundredths_passed_since(ts_t val, ts_t since) -> bool
{
  (void) game_time_ms();

  //
  // Cater for negative future times.
  //
  ts_t const delay = game_time_now - since;

  return static_cast< ts_t >(delay / 10) > val;
}

[[nodiscard]] auto game_time_have_x_ms_passed_since(ts_t val, ts_t since) -> bool
{
  (void) game_time_ms();

  //
  // Cater for negative future times.
  //
  ts_t const delay = game_time_now - since;

  return delay >= val;
}

[[nodiscard]] auto game_time_have_x_tenths_passed_since(ts_t val, ts_t since) -> bool
{
  (void) game_time_ms();

  //
  // Cater for negative future times.
  //
  ts_t const delay = game_time_now - since;

  return static_cast< ts_t >(delay / 100) >= val;
}

[[nodiscard]] auto game_time_have_x_secs_passed_since(ts_t val, ts_t since) -> bool
{
  (void) game_time_ms();

  //
  // Cater for negative future times.
  //
  ts_t const delay = game_time_now - since;

  return static_cast< ts_t >(delay / ONESEC) >= val;
}

void get_timestamp(char *buf, int len)
{
#ifdef ENABLE_LOG_TIMESTAMPS
  char tmp[ MAXSTR ];
  string_timestamp(tmp, len);
  snprintf(buf, len, "%s %s", timestamp().c_str(), tmp);
#else
  (void) timestamp(buf, len);
#endif
}

// avoid: warning: ‘%c’ yields only last 2 digits of year in some locales [-Wformat-y2k]
static auto my_strftime(char *s, size_t max, const char *fmt, const struct tm *tm) -> size_t { return strftime(s, max, fmt, tm); }

[[nodiscard]] auto current_date() -> std::string
{
  struct tm const *timeinfo = nullptr;
  time_t           rawtime  = 0;
  char             buffer[ MAXSTR ];

  time(&rawtime);
  timeinfo = localtime(&rawtime);

  // Code    Example Description
  //  %a  Sun Weekday as locale’s abbreviated name.
  //  %A  Sunday  Weekday as locale’s full name.
  //  %w  0   Weekday as a decimal number, where 0 is Sunday and 6 is Saturday.
  //  %d  08  Day of the month as a zero-padded decimal number.
  //  %-d 8   Day of the month as a decimal number. (Platform specific)
  //  %b  Sep Month as locale’s abbreviated name.
  //  %B  September   Month as locale’s full name.
  //  %m  09  Month as a zero-padded decimal number.
  //  %-m 9   Month as a decimal number. (Platform specific)
  //  %y  13  Year without century as a zero-padded decimal number.
  //  %Y  2013    Year with century as a decimal number.
  //  %H  07  Hour (24-hour clock) as a zero-padded decimal number.
  //  %-H 7   Hour (24-hour clock) as a decimal number. (Platform specific)
  //  %I  07  Hour (12-hour clock) as a zero-padded decimal number.
  //  %-I 7   Hour (12-hour clock) as a decimal number. (Platform specific)
  //  %p  AM  Locale’s equivalent of either AM or PM.
  //  %M  06  Minute as a zero-padded decimal number.
  //  %-M 6   Minute as a decimal number. (Platform specific)
  //  %S  05  Second as a zero-padded decimal number.
  //  %-S 5   Second as a decimal number. (Platform specific)
  //  %f  000000  Microsecond as a decimal number, zero-padded on the left.
  //  %z  +0000   UTC offset in the form ±HHMM[SS[.ffffff]] (empty string if the object is naive).
  //  %Z  UTC Time zone name (empty string if the object is naive).
  //  %j  251 Day of the year as a zero-padded decimal number.
  //  %-j 251 Day of the year as a decimal number. (Platform specific)
  //  %U  36  Week number of the year (Sunday as the first day of the week)
  //          as a zero padded decimal number. All days in a new year preceding the first Sunday
  //          are considered to be in week 0.
  //  %W  35  Week number of the year (Monday as the first day of the week)
  //          as a decimal number. All days in a new year preceding the first Monday are considered to be in week 0.
  //  %c  Sun Sep 8 07:06:05 2013 Locale’s appropriate date and time representation.
  //  %X  09/08/13    Locale’s appropriate date representation.
  //  %X  07:06:05    Locale’s appropriate time representation.
  //  %%  %   A literal '%' character.
  //
  my_strftime(buffer, SIZEOF(buffer), "%c", timeinfo);

  return {buffer};
}

[[nodiscard]] auto string_timestamp() -> std::string &
{
  static ts_t        time_last;
  static std::string last_timestamp;
  auto               the_time_now = user_visible_time_ms();

  if (! last_timestamp.empty()) {
    if (the_time_now - time_last < 1000) {
      return last_timestamp;
    }
  }

  time_last                = the_time_now;
  std::time_t const result = std::time(nullptr);
  auto              s      = std::string(std::asctime(std::localtime(&result)));
  s.pop_back();
  last_timestamp = s;
  return last_timestamp;
}
