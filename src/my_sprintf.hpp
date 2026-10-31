//
// Copyright goblinhack@gmail.com
//

#ifndef MY_SPRINTF_HPP
#define MY_SPRINTF_HPP

#include "my_types.hpp"

#include <cstdarg>
#include <string>

//
// Prefer std::format. This is only for printf style wrappers that already have a va_list.
//
[[nodiscard]] auto string_sprintf(const char *format, va_list args) -> std::string;

//
// Join two strings with ", ", ignoring either if empty. Use with std::format e.g.:
//
//   out = string_append_with_comma(out, std::format("Multi-attack({})", n));
//
[[nodiscard]] auto string_append_with_comma(const std::string &in, const std::string &add) -> std::string;

#endif
