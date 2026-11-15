//
// Copyright goblinhack@gmail.com
//

#ifndef MY_STRING_UTIL_HPP
#define MY_STRING_UTIL_HPP

#include "my_types.hpp"

#include <cstddef>
#include <cstdint>
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
// pointer, be explicit: string_copy(std::span< char >(ptr, size), src)
//
void string_copy(std::span< char > dst, std::string_view src);
void string_remove_trailing_chars(std::string &str, char charToRemove);
void string_remove_leading_chars(std::string &str, char charToRemove);
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

//
// The codes that can be embedded in displayed text:
//
//   %fg=<color>$   set the foreground color
//   %bg=<color>$   set the background color
//   %tex=<name>$   a texture (not currently used)
//   %tp=<name>$    show the first tile of a thing template in place of the $ that follows
//   %tile=<name>$  show a tile in place of the $ that follows
//
// Everything that needs to know what is actually displayed (drawing, measuring, stripping, wrapping)
// must agree on these rules, so they all use text_next_token() rather than each having a parser of
// their own. All reads are bounds checked, so text can safely end part way through a code.
//
enum class TextTokenType : uint8_t {
  Char, // a character that is displayed. This includes the $ that a %tp= or %tile= leaves behind.
  Skip, // a stray % that is not displayed
  Fg,
  Bg,
  Tex,
  Tp,
  Tile,
};

struct TextToken {
  TextTokenType type {TextTokenType::Char};
  bool          terminated {}; // does the argument of the code end in a $
  char          ch {};         // the character, if a Char
  size_t        arg {};        // where the argument of the code starts
  size_t        arg_len {};    // and how long it is, not including any $
  size_t        next {};       // where to carry on scanning from
};

//
// Get the token at pos. The caller must ensure that pos < text.size().
//
[[nodiscard]] inline auto text_next_token(const std::string &text, size_t pos) -> TextToken
{
  const size_t size = text.size();
  TextToken    tok;

  tok.ch   = text[ pos ];
  tok.next = pos + 1;

  const size_t at  = pos + 1;
  const size_t len = size - at; // characters after this one

  if ((tok.ch != '%') || (len == 0)) {
    return tok;
  }

  auto code = [ & ](TextTokenType type, size_t prefix_len, bool eat_terminator) -> TextToken {
    tok.type = type;
    tok.arg  = at + prefix_len;
    size_t n = 0;
    while ((tok.arg + n < size) && (text[ tok.arg + n ] != '$') && (text[ tok.arg + n ] != '\0')) {
      n++;
    }
    tok.arg_len    = n;
    tok.terminated = ((tok.arg + n) < size) && (text[ tok.arg + n ] == '$');
    tok.next       = tok.arg + n;
    if (eat_terminator && (tok.next < size)) {
      tok.next++;
    }
    return tok;
  };

  if ((len > 3) && (text.compare(at, 3, "fg=") == 0)) {
    return code(TextTokenType::Fg, 3, true);
  }
  if ((len > 3) && (text.compare(at, 3, "bg=") == 0)) {
    return code(TextTokenType::Bg, 3, true);
  }
  if ((len > 4) && (text.compare(at, 4, "tex=") == 0)) {
    return code(TextTokenType::Tex, 4, true);
  }
  if ((len > 3) && (text.compare(at, 3, "tp=") == 0)) {
    return code(TextTokenType::Tp, 3, false); // leave the $ to be displayed as the tile
  }
  if ((len > 5) && (text.compare(at, 5, "tile=") == 0)) {
    return code(TextTokenType::Tile, 5, false);
  }

  //
  // Not a code. A stray % is dropped, unless the character after next is also a %.
  //
  const char after = ((at + 1) < size) ? text[ at + 1 ] : '\0';
  if (after != '%') {
    tok.type = TextTokenType::Skip;
  }

  return tok;
}

//
// Call on_char for every character that is displayed.
//
template < typename F > inline void text_for_each_char(const std::string &text, F &&on_char)
{
  size_t pos = 0;
  while (pos < text.size()) {
    const auto tok = text_next_token(text, pos);
    if (tok.type == TextTokenType::Char) {
      on_char(tok.ch);
    }
    pos = tok.next;
  }
}

//
// The number of characters that are displayed.
//
[[nodiscard]] inline auto text_visible_length(const std::string &text) -> int
{
  int width = 0;
  text_for_each_char(text, [ & ](char) -> void { width++; });
  return width;
}

#endif
