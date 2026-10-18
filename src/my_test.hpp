//
// Copyright goblinhack@gmail.com
//

#ifndef MY_TEST_HPP
#define MY_TEST_HPP

#include "my_callstack.hpp"
#include "my_game.hpp"
#include "my_level_ext.hpp"
#include "my_string.hpp"
#include "my_types.hpp"

#include <string>

class Test;

[[nodiscard]] auto test_name(Testp test) -> const char *;
[[nodiscard]] auto test_load(const char * /*name_in*/) -> Testp;

using test_callback_t = bool (*)(Gamep, Testp);
auto test_callback_set(Testp test, test_callback_t callback) -> void;
auto tests_run(Gamep g) -> void;

#define TEST_FAILED(test, why)                                                                                                                  \
  {                                                                                                                                             \
    TRACE();                                                                                                                                    \
    log("test %s: failed at %s:%u, reason: %s", test_name(test), __FUNCTION__, __LINE__, why);                                                  \
    result = false;                                                                                                                             \
    goto exit;                                                                                                                                  \
  }

#define TEST_ASSERT(test, what, why)                                                                                                            \
  {                                                                                                                                             \
    TRACE();                                                                                                                                    \
    if (! (what)) {                                                                                                                             \
      log("test %s: assertion failed at %s:%u, reason: %s", test_name(test), __FUNCTION__, __LINE__, why);                                      \
      result = false;                                                                                                                           \
      goto exit;                                                                                                                                \
    }                                                                                                                                           \
  }

#define TEST_PASSED(test) log("test %s: passed at %s:%u", test_name(test), __FUNCTION__, __LINE__);

#define TEST_PROGRESS(test) log("test %s: %s:%u: progress", test_name(test), __FUNCTION__, __LINE__);

#define TEST_LOOP_PROGRESS(test, g, v, l, tries, w, h)                                                                                          \
  TEST_LOG(test, "try: %d", tries);                                                                                                             \
  level_dump(g, v, l, w, h);                                                                                                                    \
  TEST_PROGRESS(test);                                                                                                                          \
  TRACE();

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wvariadic-macros"

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wgnu-zero-variadic-macro-arguments"
#endif

#define TEST_LOG(test, msg, ...)                                                                                                                \
  {                                                                                                                                             \
    char tmp[ MAXSTR ];                                                                                                                         \
    snprintf(tmp, sizeof(tmp), msg, ##__VA_ARGS__);                                                                                             \
    log("test %s: %s:%u: %s", test_name(test), __FUNCTION__, __LINE__, tmp);                                                                    \
  }

#define TEST_CON(test, msg, ...)                                                                                                                \
  {                                                                                                                                             \
    char tmp[ MAXSTR ];                                                                                                                         \
    snprintf(tmp, sizeof(tmp), msg, ##__VA_ARGS__);                                                                                             \
    con("test %s: %s:%u: %s", test_name(test), __FUNCTION__, __LINE__, tmp);                                                                    \
  }

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#pragma GCC diagnostic pop

auto test_init() -> void;
auto test_fini() -> void;

[[nodiscard]] auto game_test_init(Gamep g, Levelp *out, LevelArrIdx level_num, int w, int h, const char *contents,
                                  const Overrides &overrides = no_overrides) -> Levelsp;

void game_test_init_level(Gamep g, Levelsp v, Levelp *out, LevelArrIdx level_num, int w, int h, const char *contents,
                          const Overrides &overrides = no_overrides);

void game_test_init_level(Gamep g, Levelsp v, Levelp *out, LevelArrIdx level_num, bpoint level_at, int w, int h, const char *contents,
                          const Overrides &overrides = no_overrides);

#endif // MY_TEST_HPP
