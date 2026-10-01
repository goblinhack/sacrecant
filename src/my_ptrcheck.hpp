
#ifndef MY_PTRCHECK_HPP
#define MY_PTRCHECK_HPP

#include "my_source_loc.hpp"

//
// __FUNCTION__ is not a preprocessor directive so we can't convert it into a
// string
//
#define PTRCHECK_AT SRC_FILE_NAME, SRC_FUNC_NAME, SRC_LINE_NUM

auto ptrcheck_fini() -> void;
auto MYZALLOC_(int size, const char *what, const char *file, const char *func, int line) -> void *;
auto MYMALLOC_(int size, const char *what, const char *file, const char *func, int line) -> void *;
auto MYREALLOC_(void *ptr, int size, const char *what, const char *file, const char *func, int line) -> void *;
auto MYFREE_(void *ptr, const char *file, const char *func, int line) -> void;
auto MYDUPSTR_(const char *in, const char *what, const char *file, const char *func, int line) -> char *;
auto strsub_(const char *in, const char *look_for, const char *replace_with, const char *what, const char *file, const char *func, int line)
    -> char *;

#define MYZALLOC(__size__, __what__) MYZALLOC_((__size__), (__what__), PTRCHECK_AT)

#define MYMALLOC(__size__, __what__) MYMALLOC_((__size__), (__what__), PTRCHECK_AT)

#define MYREALLOC(__ptr__, __size__, __what__) MYREALLOC_((__ptr__), (__size__), (__what__), PTRCHECK_AT)

#define MYFREE(__ptr__) MYFREE_((__ptr__), PTRCHECK_AT)

#define MYDUPSTR(__ptr__, __what__) MYDUPSTR_((__ptr__), (__what__), PTRCHECK_AT)

#define STRSUB(a, b, c, __what__) strsub_(a, b, c, (__what__), PTRCHECK_AT)

auto ptrcheck_alloc(int mtype, const void *ptr, const char *what, int size, const char *func, const char *file, int line) -> void *;
auto ptrcheck_verify(int mtype, const void *ptr, const char *func, const char *file, int line) -> int;
auto ptrcheck_free(int mtype, void *ptr, const char *func, const char *file, int line) -> int;
auto ptrcheck_leak_print(int mtype) -> void;
auto ptrcheck_leak_print() -> void;

//
// MUST use DEBUG3 else "set debug on" gets confused as it never learned pointers.
//
#define NEWPTR(__mtype__, __ptr__, __what__)                                                                                                    \
  {                                                                                                                                             \
    if (DEBUG3) {                                                                                                                               \
      TRACE();                                                                                                                                  \
      ptrcheck_alloc(__mtype__, __ptr__, __what__, sizeof(*(__ptr__)), PTRCHECK_AT);                                                            \
    }                                                                                                                                           \
  }

#define OLDPTR(__mtype__, __ptr__)                                                                                                              \
  {                                                                                                                                             \
    if (DEBUG3) {                                                                                                                               \
      TRACE();                                                                                                                                  \
      ptrcheck_free(__mtype__, __ptr__, PTRCHECK_AT);                                                                                           \
    }                                                                                                                                           \
  }

#define VERIFY(__mtype__, __ptr__)                                                                                                              \
  ((DEBUG3) ? (ptrcheck_verify(__mtype__, __ptr__, SRC_FILE_NAME, SRC_FUNC_NAME, SRC_LINE_NUM) ? (__ptr__) : nullptr) : (__ptr__))

enum {
  MTYPE_SDL,
  MTYPE_LIGHT,
  MTYPE_MISC,
  MTYPE_TEX,
  MTYPE_TILE,
  MTYPE_TP,
  MTYPE_WID,
  MTYPE_LEVELS,
  MTYPE_GAME,
  MTYPE_MAX,
};

#endif // MY__PTRCHECK_HPP_
