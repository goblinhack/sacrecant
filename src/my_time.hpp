//
// Copyright goblinhack@gmail.com
//

#ifndef MY_TIME_UTIL_HPP
#define MY_TIME_UTIL_HPP

#undef ONESEC
#define ONESEC 1000

#define MY_TIMESTAMP_SIZE 20

using ts_t = int;

#include <string>

[[nodiscard]] auto game_time_have_x_hundredths_passed_since(ts_t val, ts_t since) -> bool;
[[nodiscard]] auto game_time_have_x_ms_passed_since(ts_t val, ts_t since) -> bool;
[[nodiscard]] auto game_time_have_x_secs_passed_since(ts_t val, ts_t since) -> bool;
[[nodiscard]] auto game_time_have_x_tenths_passed_since(ts_t val, ts_t since) -> bool;

[[nodiscard]] auto time2str(ts_t ms, char *buf, int len) -> const char *;
[[nodiscard]] auto timestamp(char *buf, int len) -> const char *;

[[nodiscard]] auto game_time_ms() -> ts_t;
[[nodiscard]] auto game_time_ms_cached() -> ts_t;

[[nodiscard]] auto user_visible_time_ms() -> ts_t;
[[nodiscard]] auto user_visible_time_ms_cached() -> ts_t;

auto get_timestamp(char *buf, int len) -> void;

[[nodiscard]] auto current_date() -> std::string;
[[nodiscard]] auto string_timestamp() -> std::string &;

extern ts_t game_time_now;
extern ts_t user_visible_time_now;

#endif
