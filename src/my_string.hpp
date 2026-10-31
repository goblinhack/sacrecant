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
enum { MAXLONGSTR = 1024, MAXSTR = 512, MAXSHORTSTR = 128 };

//
// Bounds checked copy into a fixed size buffer. The destination size comes from the
// span, so it cannot be passed wrongly. The result is truncated if needed and is always
// NUL terminated (unless dst is empty). Arrays convert to a span implicitly. For a raw
// pointer, be explicit: str_copy(std::span< char >(ptr, size), src)
//
void str_copy(std::span< char > dst, std::string_view src);
void string_remove_trailing_chars(std::string &str, const char charToRemove);
void string_remove_leading_chars(std::string &str, const char charToRemove);
auto string_replace(std::string &input, const std::string &pattern, const std::string &replace_with) -> void;

[[nodiscard]] auto string_common_matching_len(std::string_view a, std::string_view b) -> size_t;
[[nodiscard]] auto string_is_regexp(std::string_view in) -> bool;
[[nodiscard]] auto string_basename(const std::string &in) -> std::string;
[[nodiscard]] auto string_capitalize_first(const std::string &in) -> std::string;
[[nodiscard]] auto string_capitalize_all(std::string in) -> std::string;
[[nodiscard]] auto string_length_without_format(const std::string &text) -> int;
[[nodiscard]] auto string_trim_trailing_whitespace(std::string &s) -> std::string &;
[[nodiscard]] auto string_split(const std::string &text, int max_line_len) -> std::vector< std::string >;
[[nodiscard]] auto string_split_tokens(const std::string &s, char delimiter) -> std::vector< std::string >;

#endif
