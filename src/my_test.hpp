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

auto test_name(Testp test) -> const char *;
auto test_load(const char * /*name_in*/) -> Testp;

using test_callback_t = bool (*)(Gamep, Testp);
void test_callback_set(Testp test, test_callback_t callback);
void tests_run(Gamep g);

#define TEST_FAILED(test, why)                                                                                                             \
  {                                                                                                                                        \
    TRACE();                                                                                                                               \
    log("test %s: failed at %s:%u, reason: %s", test_name(test), __FUNCTION__, __LINE__, why);                                             \
    result = false;                                                                                                                        \
    goto exit;                                                                                                                             \
  }

#define TEST_ASSERT(test, what, why)                                                                                                       \
  {                                                                                                                                        \
    TRACE();                                                                                                                               \
    if (! (what)) {                                                                                                                        \
      log("test %s: assertion failed at %s:%u, reason: %s", test_name(test), __FUNCTION__, __LINE__, why);                                 \
      result = false;                                                                                                                      \
      goto exit;                                                                                                                           \
    }                                                                                                                                      \
  }

#define TEST_PASSED(test) log("test %s: passed at %s:%u", test_name(test), __FUNCTION__, __LINE__);

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wvariadic-macros"

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wgnu-zero-variadic-macro-arguments"
#endif

#define TEST_LOG(test, msg, ...)                                                                                                           \
  {                                                                                                                                        \
    char tmp[ MAXSTR ];                                                                                                                    \
    snprintf(tmp, sizeof(tmp), msg, ##__VA_ARGS__);                                                                                        \
    log("test %s: %s:%u: %s", test_name(test), __FUNCTION__, __LINE__, tmp);                                                               \
  }

#define TEST_CON(test, msg, ...)                                                                                                           \
  {                                                                                                                                        \
    char tmp[ MAXSTR ];                                                                                                                    \
    snprintf(tmp, sizeof(tmp), msg, ##__VA_ARGS__);                                                                                        \
    con("test %s: %s:%u: %s", test_name(test), __FUNCTION__, __LINE__, tmp);                                                               \
  }

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#pragma GCC diagnostic pop

#define TEST_PROGRESS(test) log("test %s: %s:%u: progress", test_name(test), __FUNCTION__, __LINE__);

void test_init();
void test_fini();

[[nodiscard]] auto game_test_init(Gamep g, Levelp *out, LevelNum level_num, int w, int h, const char *contents,
                                  const Overrides &overrides = no_overrides) -> Levelsp;

void game_test_init_level(Gamep g, Levelsp v, Levelp *out, LevelNum level_num, int w, int h, const char *contents,
                          const Overrides &overrides = no_overrides);

void game_test_init_level(Gamep g, Levelsp v, Levelp *out, LevelNum level_num, bpoint level_at, int w, int h, const char *contents,
                          const Overrides &overrides = no_overrides);

#endif // MY_TEST_HPP
