//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_color.hpp"
#include "my_main.hpp"
#include "my_ptrcheck.hpp"
#include "my_string.hpp"
#include "my_tex.hpp"
#include "my_tile.hpp"
#include "my_tp.hpp"
#include "my_types.hpp"

#include <algorithm>
#include <cctype>
#include <cerrno>
#include <cstdarg>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <libgen.h>
#include <locale>
#include <ranges>
#include <sstream>
#include <string.h>
#include <string>
#include <strings.h>
#include <utility>
#include <vector>

/*
 * Return the common matching length of two strings.
 */
[[nodiscard]] auto strcommon(const char *a, const char *b) -> uint32_t
{
  TRACE();
  const char *o = nullptr;

  o = a;

  while ((*a == *b) && (*a != 0)) {
    a++;
    b++;
  }

  return static_cast< uint32_t >(a - o);
}

void remove_trailing(std::string &str, const char charToRemove)
{
  TRACE();
  str.erase(str.find_last_not_of(charToRemove) + 1, std::string::npos);
}

void remove_leading(std::string &str, const char charToRemove)
{
  TRACE();
  str.erase(0, std::min(str.find_first_not_of(charToRemove), str.size() - 1));
}

/*
 * Removes trailing characters.
 */
void strchopc(char *s, char c)
{
  TRACE();
  uint32_t size = 0;
  char    *end  = nullptr;

  size = static_cast< uint32_t >(strlen(s));
  if (! static_cast< bool >(size)) {
    return;
  }

  end = s + size - 1;
  while ((end >= s) && (*end == c)) {
    end--;
  }

  *(end + 1) = '\0';
}

[[nodiscard]] auto strisregexp(const char *in) -> int
{
  TRACE();
  const char *a = in;
  char        c = 0;

  while ((c = *a++) != 0) {
    switch (c) {
      case '[' : return 1;
      case ']' : return 1;
      case '{' : return 1;
      case '}' : return 1;
      case '+' : return 1;
      case '$' : return 1;
      case '^' : return 1;
      case '-' : return 1;
      case '*' : return 1;
    }
  }

  return 0;
}

void str_copy(std::span< char > dst, std::string_view src)
{
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
[[nodiscard]] auto mybasename(const std::string &in) -> std::string
{
  TRACE();
  std::string out = std::string(in);
  return in.substr(out.find_last_of("/\\") + 1);
}

[[nodiscard]] auto split(const std::string &text, int max_line_len) -> std::vector< std::string >
{
  TRACE();
  bool found_format_string = false;
  int  line_len            = 0;
  char c                   = 0;
  auto text_start          = text.begin();
  auto text_iter           = text_start;
  auto line_start          = text_start;
  auto line_end            = text_start;
  //
  // Save the format string such that if we split a line of text, we carry
  // the format onto subsequent lines.
  //
  std::string saved_format_string;
  std::string this_saved_format_string;

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
      c = *text_iter;
      if ((c == '\n') || (c == '\0')) {
        break;
      }
      line_len++;

      if (! found_format_string) {
        if (c == '%') {
          found_format_string = true;
        }
      } else if (found_format_string) {
        if (std::string(text_iter, text_iter + 3) == "fg=") {
          text_iter += 3;
          line_len -= 2; /* for the %% */
          auto tmp = std::string(text_iter, text.end());

          int  len                 = 0;
          auto color_name          = string2colorname(tmp, &len);
          this_saved_format_string = "%%fg=" + color_name + "$";
          text_iter += len + 1;

          found_format_string = false;
          continue;
        }
        if (std::string(text_iter, text_iter + 3) == "bg=") {
          text_iter += 3;
          line_len -= 2; /* for the %% */
          auto tmp = std::string(text_iter, text.end());

          int  len                 = 0;
          auto color_name          = string2colorname(tmp, &len);
          this_saved_format_string = "%%bg=" + color_name + "$";
          text_iter += len + 1;

          found_format_string = false;
          continue;
        }
        if (std::string(text_iter, text_iter + 4) == "tex=") {
          text_iter += 4;
          line_len -= 2; /* for the %% */
          auto tmp = std::string(text_iter, text.end());

          int len = 0;
          (void) string2tex(tmp, &len);
          text_iter += len + 1;

          found_format_string = false;
          continue;
        }
        if (std::string(text_iter, text_iter + 3) == "tp=") {
          text_iter += 3;
          auto tmp = std::string(text_iter, text.end());

          int         len  = 0;
          const char *tmpc = tmp.c_str();
          auto       *tp   = string2tp(&tmpc, &len);
          text_iter += len + 1;

          (void) tp_first_tile(tp, THING_ANIM_IDLE);
          found_format_string = false;
          continue;
        }
        if (std::string(text_iter, text_iter + 5) == "tile=") {
          text_iter += 5;
          auto tmp = std::string(text_iter, text.end());

          int len = 0;
          (void) string2tile(tmp, &len);
          text_iter += len + 1;

          found_format_string = false;
          continue;
        }
        if (c == '%') {
          line_len -= 1;
        } else {
          found_format_string = false;
          text_iter--;
        }
      }

      text_iter++;
      // printf("LEN %d [%c]\n", line_len, c);
    }
    // printf("END line_len %d\n", line_len);

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
    saved_format_string = this_saved_format_string;
    first_line          = false;

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

[[nodiscard]] auto length_without_format(const std::string &text) -> int
{
  TRACE();
  bool found_format_string = false;
  char c                   = 0;
  auto text_start          = text.begin();
  auto text_iter           = text_start;
  int  line_len            = 0;

  for (;;) {
    c = *text_iter;
    if (c == 0) {
      break;
    }
    line_len++;

    if (! found_format_string) {
      if (c == '%') {
        found_format_string = true;
      }
    } else if (found_format_string) {
      if (std::string(text_iter, text_iter + 3) == "fg=") {
        text_iter += 3;
        line_len -= 2; /* for the %% */
        auto tmp = std::string(text_iter, text.end());

        int len = 0;
        (void) string2color(tmp, &len);
        text_iter += len + 1;

        found_format_string = false;
        continue;
      }
      if (std::string(text_iter, text_iter + 3) == "bg=") {
        text_iter += 3;
        line_len -= 2; /* for the %% */
        auto tmp = std::string(text_iter, text.end());

        int len = 0;
        (void) string2color(tmp, &len);
        text_iter += len + 1;

        found_format_string = false;
        continue;
      }
      if (std::string(text_iter, text_iter + 4) == "tex=") {
        text_iter += 4;
        line_len -= 2; /* for the %% */
        auto tmp = std::string(text_iter, text.end());

        int len = 0;
        (void) string2tex(tmp, &len);
        text_iter += len + 1;

        found_format_string = false;
        continue;
      }
      if (std::string(text_iter, text_iter + 3) == "tp=") {
        text_iter += 3;
        auto tmp = std::string(text_iter, text.end());

        int         len  = 0;
        const char *tmpc = tmp.c_str();
        auto       *tp   = string2tp(&tmpc, &len);
        text_iter += len + 1;

        (void) tp_first_tile(tp, THING_ANIM_IDLE);
        found_format_string = false;
        continue;
      }
      if (std::string(text_iter, text_iter + 5) == "tile=") {
        text_iter += 5;
        auto tmp = std::string(text_iter, text.end());

        int len = 0;
        (void) string2tile(tmp, &len);
        text_iter += len + 1;

        found_format_string = false;
        continue;
      }
      if (c == '%') {
        line_len -= 1;
      } else {
        found_format_string = false;
        text_iter--;
      }
    }

    text_iter++;
    // printf("LEN %d [%c]\n", line_len, c);
  }
  // printf("END line_len %d\n", line_len);

  return line_len;
}

[[nodiscard]] auto split_tokens(const std::string &s, const char delimiter) -> std::vector< std::string >
{
  std::string                token;
  std::istringstream         s_stream(s);
  std::vector< std::string > tokens;
  while (std::getline(s_stream, token, delimiter)) {
    tokens.push_back(token);
  }
  return tokens;
}

// https://www.techiedelight.com/trim-string-cpp-remove-leading-trailing-spaces/
[[nodiscard]] auto ltrim(std::string &s) -> std::string &
{
  auto it = std::ranges::find_if(s, [](char c) -> bool { return ! std::isspace< char >(c, std::locale::classic()); });
  s.erase(s.begin(), it);
  return s;
}

[[nodiscard]] auto rtrim(std::string &s) -> std::string &
{
  auto it = std::ranges::find_if(std::ranges::reverse_view(s), [](char c) -> bool { return ! std::isspace< char >(c, std::locale::classic()); });
  s.erase(it.base(), s.end());
  return s;
}

[[nodiscard]] auto trim(std::string &s) -> std::string & { return ltrim(rtrim(s)); }

const std::string WHITESPACE = " \n\r\t\f\v";

[[nodiscard]] auto ltrim_ws(const std::string &s) -> std::string
{
  size_t const start = s.find_first_not_of(WHITESPACE);
  return (start == std::string::npos) ? "" : s.substr(start);
}

[[nodiscard]] auto rtrim_ws(const std::string &s) -> std::string
{
  size_t const end = s.find_last_not_of(WHITESPACE);
  return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}

[[nodiscard]] auto trim_ws(const std::string &s) -> std::string { return rtrim_ws(ltrim_ws(s)); }

//
// Concert errno to a std::string
//
[[nodiscard]] auto strerror_to_string(const int err) -> std::string
{
  static const auto MAX_ERRNO_BUF_SIZE = 1024U;
  char              err_out[ MAX_ERRNO_BUF_SIZE ];
  err_out[ 0 ] = '\0';

  //
  // Choose the correct strerror
  //
#if defined(__GLIBC__) && (_GNU_SOURCE || (_POSIX_C_SOURCE < 200112L && _XOPEN_SOURCE < 600))
  //
  // GNU version returns a string pointer
  //
  const char *str = strerror_r(errno, err_out, MAX_ERRNO_BUF_SIZE);
  if (str) {
    return std::string(str);
  }
#else
  //
  // XSI version returns 0 on success
  //
  if (strerror_r(err, err_out, SIZEOF(err_out)) == 0) {
    return {err_out};
  }
#endif
  return "could not decode errno: " + std::to_string(err) + " strerror_r errno=" + std::to_string(errno);
}

//
// foo bar -> Foo Bar
//
[[nodiscard]] auto capitalize(std::string in) -> std::string
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
[[nodiscard]] auto capitalize_first(const std::string &in) -> std::string
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
void replace(std::string &input, const std::string &pattern, const std::string &replace_with)
{
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

//
// Borrowed from
//
// https://stackoverflow.com/questions/19580877/how-to-truncate-a-string-formating-c#19590797
//
auto truncate(std::string str, size_t width, bool show_ellipsis) -> std::string
{
  if (str.length() > width) {
    if (show_ellipsis) {
      str.resize(width);
      return str.append("...");
    }
    str.resize(width);
    return str;
  }
  return str;
}
