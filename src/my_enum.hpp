//
// Copyright goblinhack@gmail.com
//

#ifndef MY_ENUM_HPP
#define MY_ENUM_HPP

#include "my_types.hpp"

#include <array>
#include <string>

/*
 * Enum list macros .h definitions
 */
#define ENUM_LIST_MACRO_VALUE(enum_arg, string_arg) enum_arg

#define ENUM_DEF_H(ENUM_NAME, enum_name)                                                                                                        \
                                                                                                                                                \
  typedef enum { ENUM_NAME(ENUM_LIST_MACRO_VALUE) ENUM_NAME##_MAX } __attribute__((__packed__)) (enum_name);                                    \
                                                                                                                                                \
  extern std::string enum_name##_to_string(enum_name val);                                                                                      \
  extern const char *enum_name##_to_c_str(enum_name val);                                                                                       \
  extern void        enum_name##_destroy(void);

/*
 * Enum list macros .c definitions
 */
#define ENUM_LIST_MACRO_STRING(enum_arg, string_arg) string_arg

/*
 * Enum to string.
 */
#define ENUM_DEF_C(ENUM_NAME, enum_name)                                                                                                        \
                                                                                                                                                \
  auto enum_name##_to_string(enum_name val) -> std::string                                                                                      \
  {                                                                                                                                             \
    static const auto arr = std::to_array< std::string >({ENUM_NAME(ENUM_LIST_MACRO_STRING)});                                                  \
                                                                                                                                                \
    if (val >= (__typeof__(val)) arr.size()) {                                                                                                  \
      return (std::string("[bad]"));                                                                                                            \
    }                                                                                                                                           \
                                                                                                                                                \
    return (std::string(arr[ val ]));                                                                                                           \
  }                                                                                                                                             \
                                                                                                                                                \
  auto enum_name##_to_c_str(enum_name val) -> const char *                                                                                      \
  {                                                                                                                                             \
    static const auto arr = std::to_array< std::string >({ENUM_NAME(ENUM_LIST_MACRO_STRING)});                                                  \
                                                                                                                                                \
    if (val >= (__typeof__(val)) arr.size()) {                                                                                                  \
      return ("[bad]");                                                                                                                         \
    }                                                                                                                                           \
                                                                                                                                                \
    return arr[ val ].c_str();                                                                                                                  \
  }                                                                                                                                             \
                                                                                                                                                \
  void enum_name##_destroy(void) {}

#define EXAMPLE(list_macro) list_macro(_RED = 0, "red"), list_macro(_GREEN, "green"),

#define CLANG_FORMAT_INDENT()

#endif
