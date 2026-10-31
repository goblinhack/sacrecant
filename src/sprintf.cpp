//
// Copyright goblinhack@gmail.com
//

#include "my_sprintf.hpp"

#include <cstdarg>
#include <cstdio>
#include <string>

//
// For callers that already hold a va_list, e.g. printf style logging wrappers. Everything else
// should use std::format.
//
[[nodiscard]] auto string_sprintf(const char *format, va_list args) -> std::string
{
  va_list args_copy;
  va_copy(args_copy, args);
  const int len = vsnprintf(nullptr, 0, format, args_copy);
  va_end(args_copy);

  if (len < 0) {
    return {};
  }

  std::string ret(static_cast< size_t >(len), '\0');
  // The buffer has room for the terminating NUL that vsnprintf writes.
  (void) vsnprintf(ret.data(), ret.size() + 1, format, args);
  return ret;
}

[[nodiscard]] auto string_append_with_comma(const std::string &in, const std::string &add) -> std::string
{
  if (add.empty()) {
    return in;
  }

  if (in.empty()) {
    return add;
  }
  return in + ", " + add;
}
