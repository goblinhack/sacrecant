//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_main.hpp"
#include "my_string.hpp"
#include "my_tp.hpp"
#include "my_types.hpp"

#include <algorithm>
#include <cctype>
#include <cerrno>
#include <cstdarg>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <locale>
#include <ranges>
#include <span>
#include <sstream>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

//
// Return the common matching length of two strings.
//
[[nodiscard]] auto string_common_matching_len(std::string_view a, std::string_view b) -> size_t
{
  TRACE();
  return static_cast< size_t >(std::ranges::mismatch(a, b).in1 - a.begin());
}

void string_remove_trailing_chars(std::string &str, const char charToRemove)
{
  TRACE();
  str.erase(str.find_last_not_of(charToRemove) + 1, std::string::npos);
}

void string_remove_leading_chars(std::string &str, const char charToRemove)
{
  TRACE();
  str.erase(0, std::min(str.find_first_not_of(charToRemove), str.size() - 1));
}

[[nodiscard]] auto string_is_regexp(std::string_view in) -> bool
{
  TRACE();
  return in.find_first_of("[]{}+$^-*") != std::string_view::npos;
}

void string_copy(std::span< char > dst, std::string_view src)
{
  TRACE();
  if (dst.empty()) {
    return;
  }

  const auto len = std::min(src.size(), dst.size() - 1);
  std::copy_n(src.begin(), len, dst.begin());
  dst[ len ] = '\0';
}

//
// A safe wrapper for basename to avoid modifications to the input string.
//
[[nodiscard]] auto string_basename(const std::string &in) -> std::string
{
  TRACE();
  std::string const out = std::string(in);
  return in.substr(out.find_last_of("/\\") + 1);
}

[[nodiscard]] auto string_split(const std::string &text, int max_line_len) -> std::vector< std::string >
{
  TRACE();
  int  line_len   = 0;
  char c          = 0;
  auto text_start = text.begin();
  auto text_iter  = text_start;
  auto line_start = text_start;
  auto line_end   = text_start;
  //
  // Save the format string such that if we split a line of text, we carry
  // the format onto subsequent lines. The foreground and background are tracked separately so
  // a background code does not lose the foreground.
  //
  std::string saved_format_string;
  std::string saved_fg;
  std::string saved_bg;

  if (max_line_len < 0) {
    CROAK("bad max line len %d text [%s]", max_line_len, text.c_str());
  }

  // printf("SPLIT1 [%s] max_line_len %d\n", text.c_str(), max_line_len);

  auto result = std::vector< std::string >();

  if (text.empty()) {
    return result;
  }

  bool keep_leading_spaces = true;
  bool first_line          = true;

  for (; /*ever*/;) {
    line_len   = 0;
    line_start = text_iter;

    /*
     * The first newline is used as a split char. Two newlines means one
     * of them is forced.
     */
    if (text_iter != text_start) {
      if (*text_iter == '\n') {
        text_iter++;
      }
    }

    if (! keep_leading_spaces) {
      while (*text_iter == ' ') {
        text_iter++;
      }
    }
    keep_leading_spaces = false;

    /*
     * Read in as much of the line we can up to the line length.
     */
    while (line_len < max_line_len) {
      auto const pos = static_cast< size_t >(text_iter - text_start);
      if (pos >= text.size()) {
        break;
      }

      c = text[ pos ];
      if ((c == '\n') || (c == '\0')) {
        break;
      }

      //
      // Only displayed characters count towards the line length. The format codes take no space.
      //
      auto const tok = text_next_token(text, pos);
      if (tok.type == TextTokenType::Char) {
        line_len++;
      }

      text_iter = text_start + static_cast< std::ptrdiff_t >(tok.next);
    }

    line_end = text_iter;

    /*
     * Step back looking for a word break.
     */
    if (line_len >= max_line_len) {
      while ((line_end > line_start) && ((*line_end != ' ') && (*line_end != '\0') && (*line_end != '\n'))) {
        line_end--;
      }
    }

    while (line_end > line_start) {
      if (*(line_end - 1) == ' ') {
        line_end--;
        continue;
      }
      break;
    }

    if (line_end == line_start) {
      line_end = text_iter;
    }

    /*
     * Skip leading junk.
     */
    while (line_start < line_end) {
      if (*line_start == '\n') {
        line_start++;
        continue;
      }

      if (! first_line) {
        if (*line_start == ' ') {
          line_start++;
          continue;
        }
      }
      break;
    }

    line_len = static_cast< int >(line_end - line_start);
    auto tmp = std::string(line_start, line_start + line_len);

    // printf("OUT [%s] max_line_len %d\n", tmp.c_str(), line_len);
    //
    result.push_back(saved_format_string + tmp);

    //
    // Work out the format in force at the end of this line, to carry onto the next. This cannot be
    // taken from the scan above, as that can run on past the end of the line before we back up to a
    // word break, which would let a color from the next line leak onto the end of this one.
    //
    {
      auto       pos     = static_cast< size_t >(line_start - text_start);
      auto const end_pos = static_cast< size_t >(line_end - text_start);
      while (pos < end_pos) {
        auto const tok = text_next_token(text, pos);
        if (tok.type == TextTokenType::Fg) {
          saved_fg = "%%fg=" + text.substr(tok.arg, tok.arg_len) + "$";
        } else if (tok.type == TextTokenType::Bg) {
          saved_bg = "%%bg=" + text.substr(tok.arg, tok.arg_len) + "$";
        }
        pos = tok.next;
      }
      saved_format_string = saved_fg + saved_bg;
    }
    first_line = false;

    text_iter = line_end;
    if (*text_iter == 0) {
      break;
    }

    if (text_iter == text_start) {
      if (line_len == 0) {
        text_iter++;
      }
    }
  }

  return result;
}

[[nodiscard]] auto string_length_without_format(const std::string &text) -> int
{
  TRACE();
  return text_visible_length(text);
}

[[nodiscard]] auto string_split_tokens(const std::string &s, const char delimiter) -> std::vector< std::string >
{
  TRACE();
  std::string                token;
  std::istringstream         s_stream(s);
  std::vector< std::string > tokens;
  while (std::getline(s_stream, token, delimiter)) {
    tokens.push_back(token);
  }
  return tokens;
}

//
// foo bar -> Foo Bar
//
[[nodiscard]] auto string_capitalize_all(std::string in) -> std::string
{
  TRACE();
  std::string out = std::move(in);

  char       *b          = const_cast< char * >(out.c_str());
  char const *e          = b + out.size();
  char       *c          = b;
  bool        word_start = true;
  while (c < e) {
    if (word_start) {
      if (islower(*c) != 0) {
        *c = toupper(*c);
      }
      word_start = false;
    } else if (*c == ' ') {
      word_start = true;
    }

    c++;
  }

  return out;
}

//
// foo bar -> Foo bar
//
[[nodiscard]] auto string_capitalize_first(const std::string &in) -> std::string
{
  TRACE();
  std::string out = in;

  char       *b          = const_cast< char * >(out.c_str());
  char const *e          = b + out.size();
  char       *c          = b;
  bool        word_start = true;
  while (c < e) {
    if (word_start) {
      if (islower(*c) != 0) {
        *c = toupper(*c);
      }
      word_start = false;
    }

    c++;
  }

  return out;
}

//
// Modify the input string with and change "pattern" to "replace_with" repeatedly.
//
void string_replace(std::string &input, const std::string &pattern, const std::string &replace_with)
{
  TRACE();
  size_t pos = 0;
  for (;;) {
    pos = input.find(pattern, pos);
    if (pos == std::string::npos) {
      return;
    }
    input.replace(pos, pattern.length(), replace_with);
    pos += replace_with.length();
  }
}

[[nodiscard]] auto string2tp(const char **s, int *len) -> Tpp
{
  TRACE();
  static char        tmp[ MAXSHORTSTR ];
  static const char *eo_tmp = tmp + MAXSHORTSTR;
  const char        *c      = *s;
  char              *t      = tmp; // NOLINT

  while (t < eo_tmp) {
    if ((*c == '\0') || (*c == '$')) {
      break;
    }

    *t++ = *c++;
  }

  if (c == eo_tmp) {
    ERR("thing template name %s is too long", tmp);
    return nullptr;
  }

  *t++ = '\0';
  *s += (t - tmp);

  if (len != nullptr) {
    *len = t - tmp;
  }

  Tpp tp = tp_find_mand(tmp);
  if (tp == nullptr) [[unlikely]] {
    ERR("thing template name [%s] not found", tmp) [[unlikely]];
  }

  return tp;
}

// https://www.techiedelight.com/trim-string-cpp-remove-leading-trailing-spaces/
[[nodiscard]] static auto ltrim(std::string &s) -> std::string &
{
  TRACE();
  auto it = std::ranges::find_if(s, [](char c) -> bool { return ! std::isspace< char >(c, std::locale::classic()); });
  s.erase(s.begin(), it);
  return s;
}

[[nodiscard]] static auto rtrim(std::string &s) -> std::string &
{
  TRACE();
  auto it = std::ranges::find_if(std::ranges::reverse_view(s), [](char c) -> bool { return ! std::isspace< char >(c, std::locale::classic()); });
  s.erase(it.base(), s.end());
  return s;
}

[[nodiscard]] auto string_trim_trailing_whitespace(std::string &s) -> std::string &
{
  TRACE();
  return ltrim(rtrim(s));
}
