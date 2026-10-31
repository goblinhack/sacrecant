//
// Copyright goblinhack@gmail.com
//

#ifndef MY_STRING_UTIL_HPP
#define MY_STRING_UTIL_HPP

#include "my_types.hpp"

#include <span>
#include <string>
#include <string_view>
#include <vector>

//
// Max long string size
//
enum {
  MAXLONGSTR  = (1024 * 16), // Needs to be big for long tracebacks
  MAXSTR      = 1024,
  MAXSHORTSTR = 128
};

//
// Bounds checked copy into a fixed size buffer. The destination size comes from the
// span, so it cannot be passed wrongly. The result is truncated if needed and is always
// NUL terminated (unless dst is empty). Arrays convert to a span implicitly. For a raw
// pointer, be explicit: str_copy(std::span< char >(ptr, size), src)
//
void str_copy(std::span< char > dst, std::string_view src);

//
// Old C APIs
//
[[nodiscard]] auto my_strlcat(char *dst, const char *src, size_t max_len) -> size_t;
[[nodiscard]] auto strcommon(const char *a, const char *b) -> uint32_t;
[[nodiscard]] auto strisregexp(const char *in) -> int;

void               remove_trailing(std::string &str, const char charToRemove);
void               remove_leading(std::string &str, const char charToRemove);
[[nodiscard]] auto mybasename(const std::string &in) -> std::string;
[[nodiscard]] auto capitalize_first(const std::string &in) -> std::string;
[[nodiscard]] auto capitalize(std::string in) -> std::string;
[[nodiscard]] auto length_without_format(const std::string &text) -> int;
[[nodiscard]] auto ltrim_ws(const std::string &s) -> std::string;
[[nodiscard]] auto ltrim(std::string &s) -> std::string &;
[[nodiscard]] auto rtrim_ws(const std::string &s) -> std::string;
[[nodiscard]] auto rtrim(std::string &s) -> std::string &;
[[nodiscard]] auto split(const std::string &text, int max_line_len) -> std::vector< std::string >;
[[nodiscard]] auto strerror_to_string(int err) -> std::string;
[[nodiscard]] auto trim_ws(const std::string &s) -> std::string;
[[nodiscard]] auto trim(std::string &s) -> std::string &;
[[nodiscard]] auto split_tokens(const std::string &s, char delimiter) -> std::vector< std::string >;
[[nodiscard]] auto truncate(std::string str, size_t width, bool show_ellipsis = true) -> std::string;

auto hexdump(const uint8_t *addr, size_t len) -> void;
auto hexdump(std::vector< uint8_t > &v) -> void;
auto replace(std::string &input, const std::string &pattern, const std::string &replace_with) -> void;

#ifdef _WIN32
#define strerror_r(_errno_, _buf_, _len_) strerror_s(_buf_, _len_, _errno_)
#endif

#endif
