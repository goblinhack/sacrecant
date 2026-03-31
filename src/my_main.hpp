//
// Copyright goblinhack@gmail.com
//

#ifndef MY_MAIN_HPP
#define MY_MAIN_HPP

#include "my_globals.hpp"
#include "my_types.hpp"

#include <string>

auto log_dir_create() -> std::string;
auto redirect_stderr() -> FILE *;
auto redirect_stdout() -> FILE *;
void botcon_newline();
void botcon(const char *fmt, ...) CHECK_FORMAT_STR(printf, 1, 2);
void cleanup();
void close_stderr();
void close_stdout();
void con_newline();
void con(const char *fmt, ...) CHECK_FORMAT_STR(printf, 1, 2);
void crash_handler(int sig);
void croak_handler(bool clean, const char *fmt, ...) CHECK_FORMAT_STR(printf, 2, 3);
void ctrlc_handler(int sig);
void err_handler(const char *fmt, ...) CHECK_FORMAT_STR(printf, 1, 2);
void error_clear(Gamep g);
void error_message(Gamep g, const std::string &error);
void find_file_locations();
void log(const char *fmt, ...) CHECK_FORMAT_STR(printf, 1, 2);
void reset_globals();
void restart(Gamep g, const std::string & /*restart_arg*/);
void sdl_msg_box(const char *fmt, ...) CHECK_FORMAT_STR(printf, 1, 2);
void topcon_newline();
void topcon(const char *fmt, ...) CHECK_FORMAT_STR(printf, 1, 2);
void warn(const char *fmt, ...) CHECK_FORMAT_STR(printf, 1, 2);

#define MY_STDOUT (g_log_stdout ? g_log_stdout : redirect_stdout())
#define MY_STDERR (g_log_stderr ? g_log_stderr : redirect_stderr())

#define CROAK(...)                                                                                                                         \
  /* Log this now, just in case we crash later */                                                                                          \
  fprintf(stderr, "CROAK: " __VA_ARGS__);                                                                                                  \
  fprintf(stderr, "\n");                                                                                                                   \
  if (stderr != MY_STDERR) {                                                                                                               \
    fprintf(MY_STDERR, "CROAK: " __VA_ARGS__);                                                                                             \
    fprintf(MY_STDERR, "\n");                                                                                                              \
  }                                                                                                                                        \
  if (g_thread_id == MAIN_THREAD) {                                                                                                        \
    fprintf(stderr, "croaked it at %s:%s():%u, main thread\n", SRC_FILE_NAME, SRC_FUNC_NAME, SRC_LINE_NUM);                                \
    if (stderr != MY_STDERR) {                                                                                                             \
      fprintf(MY_STDERR, "croaked it at %s:%s():%u, main thread\n", SRC_FILE_NAME, SRC_FUNC_NAME, SRC_LINE_NUM);                           \
    }                                                                                                                                      \
    croak_handler(false, __VA_ARGS__);                                                                                                     \
    cleanup();                                                                                                                             \
    exit(1);                                                                                                                               \
  } else {                                                                                                                                 \
    fprintf(stderr, "croaked it at %s:%s():%u, thread %u\n", SRC_FILE_NAME, SRC_FUNC_NAME, SRC_LINE_NUM, g_thread_id);                     \
    if (stderr != MY_STDERR) {                                                                                                             \
      fprintf(MY_STDERR, "croaked it at %s:%s():%u, thread %u\n", SRC_FILE_NAME, SRC_FUNC_NAME, SRC_LINE_NUM, g_thread_id);                \
    }                                                                                                                                      \
    croak_handler(false, __VA_ARGS__);                                                                                                     \
    exit(1);                                                                                                                               \
  }

#define DIE_CLEAN(...)                                                                                                                     \
  croak_handler(true, "Exiting at %s:%s():%u", SRC_FILE_NAME, SRC_FUNC_NAME, SRC_LINE_NUM);                                                \
  cleanup();                                                                                                                               \
  exit(0);

#ifdef DEBUG_BUILD
#define ERR CROAK
#else
#define err(...)                                                                                                                           \
  /* Log this now, just in case we crash later */                                                                                          \
  fprintf(stderr, "ERR: " __VA_ARGS__);                                                                                                    \
  fprintf(stderr, "\n");                                                                                                                   \
  if (stderr != MY_STDERR) {                                                                                                               \
    fprintf(MY_STDERR, "ERR: " __VA_ARGS__);                                                                                               \
    fprintf(MY_STDERR, "\n");                                                                                                              \
  }                                                                                                                                        \
  if (g_thread_id == MAIN_THREAD) {                                                                                                        \
    fprintf(stderr, "error at %s:%s():%u, main thread\n", SRC_FILE_NAME, SRC_FUNC_NAME, SRC_LINE_NUM);                                     \
    if (stderr != MY_STDERR) {                                                                                                             \
      fprintf(MY_STDERR, "error at %s:%s():%u, main thread\n", SRC_FILE_NAME, SRC_FUNC_NAME, SRC_LINE_NUM);                                \
    }                                                                                                                                      \
    err_handler(__VA_ARGS__);                                                                                                              \
  } else {                                                                                                                                 \
    fprintf(stderr, "error at %s:%s():%u, thread %u\n", SRC_FILE_NAME, SRC_FUNC_NAME, SRC_LINE_NUM, g_thread_id);                          \
    if (stderr != MY_STDERR) {                                                                                                             \
      fprintf(MY_STDERR, "error at %s:%s():%u, thread %u\n", SRC_FILE_NAME, SRC_FUNC_NAME, SRC_LINE_NUM, g_thread_id);                     \
    }                                                                                                                                      \
    err_handler(__VA_ARGS__);                                                                                                              \
  }
#endif

#define DEBUG       (g_opt_debug1)
#define DEBUG2      (g_opt_debug2)
#define NODEBUG     (! g_opt_debug1)
#define NODEBUG2    (! g_opt_debug2)
#define IF_DEBUG    if (DEBUG) [[unlikely]]
#define IF_DEBUG2   if (DEBUG2) [[unlikely]]
#define IF_NODEBUG  if (NODEBUG) [[unlikely]]
#define IF_NODEBUG2 if (NODEBUG2) [[unlikely]]

#define DBG                                                                                                                                \
  if (DEBUG)                                                                                                                               \
  log

#define DBG2                                                                                                                               \
  if (DEBUG2)                                                                                                                              \
  log

//
// Used to stop the compiler removing unused code I want to keep
//
#ifdef DEBUG_BUILD
extern bool compiler_unused;
#else
#define compiler_unused false
#endif

#endif
