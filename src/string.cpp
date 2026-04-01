//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_globals.hpp"
#include "my_main.hpp"
#include "my_ptrcheck.hpp"
#include "my_string.hpp"
#include "my_tex.hpp"
#include "my_tile.hpp"
#include "my_tp.hpp"

#include <algorithm>
#include <iostream>
#include <libgen.h>
#include <ranges>
#include <sstream>
#include <string>
#include <utility>

/*
 * Replace chars in replace_set with replace_with.
 */
void strrepc(char *s, const char *replace_set, char replace_with)
{
  TRACE();
  char *c = nullptr;

  for (c = s; *c != 0; c++) {
    if (strchr(replace_set, *c) != nullptr) {
      *c = replace_with;
    }
  }
}

/*
 * return a part of a string.
 *
 * substr("foo.zip", -4, 4) -> "zip"
 */
#if 0
static char *substr (const char *in, int pos, int len)
{ TRACE();
  int slen;
  char *out;

  if (!in) {
    return 0;
  }

  slen = (uint32_t)strlen(in);

  /*
   * Negative positions from the end.
   */
  if (pos < 0) {
    pos = slen + pos;
  }

  if (pos < 0) {
    pos = 0;
  }

  if (len < 0) {
    len = 0;
  }

  if (pos > slen) {
    pos = slen;
  }

  if (len > slen - pos) {
    len = slen - pos;
  }

  out = (__typeof__(out)) MYMALLOC(len + SIZEOF((char)'\0'), "substr");
  if (!out) {
    return 0;
  }

  memcpy(out, &(in[pos]), len);
  out[len] = '\0';

  return out;
}
#endif

//
// Recursively replace part of a string with another.
//
// STRSUB("foo.zip", ".zip", ""); -> "foo"
//
auto strsub_(const char *in, const char *look_for, const char *replace_with, const char *what, const char *file, const char *func, int line)
    -> char *
{
  TRACE();
  char       *buf    = nullptr;
  const char *at     = nullptr;
  int         newlen = 0;
  int         oldlen = 0;
  int         len    = 0;

  if ((in == nullptr) || (look_for == nullptr) || (replace_with == nullptr)) {
    return nullptr;
  }

  // printf("in %s\n", in);
  // printf("  look for %s\n", look_for);
  // printf("  replace  %s\n", replace_with);
  at = strstr(in, look_for);
  if (at == nullptr) {
    buf = MYDUPSTR(in, what);
    return buf;
  }

  oldlen = static_cast< uint32_t >(strlen(look_for));
  newlen = static_cast< uint32_t >(strlen(replace_with));

  len = static_cast< uint32_t >(strlen(in)) - oldlen + newlen;
  buf = static_cast< __typeof__(buf) >(MYZALLOC_(len + SIZEOF((char) '\0'), what, file, func, line));
  if (buf == nullptr) {
    return nullptr;
  }

  *buf = '\0';
  my_strlcpy(buf, in, at - in + 1);
  strcat(buf, replace_with);
  strcat(buf, at + oldlen);

  if (strcmp(buf, in) == 0) {
    return buf;
  }

  auto *out = strsub_(buf, look_for, replace_with, what, file, func, line);
  MYFREE(buf);
  return out;
}

/*
 * Add onto the end of a string.
 *
 * strappend("foo", ".zip"); -> "foo.zip"
 */
auto strappend(const char *in, const char *append) -> char *
{
  TRACE();
  char *buf    = nullptr;
  int   newlen = 0;
  int   len    = 0;

  if ((in == nullptr) || (append == nullptr)) {
    return nullptr;
  }

  newlen = static_cast< uint32_t >(strlen(append));
  len    = static_cast< uint32_t >(strlen(in)) + newlen;
  buf    = static_cast< __typeof__(buf) >(MYZALLOC(len + SIZEOF((char) '\0'), "strappend"));
  if (buf == nullptr) {
    return nullptr;
  }

  strcpy(buf, in);
  strcat(buf, append);

  return buf;
}

/*
 * Add onto the start of a string.
 *
 * strprepend("foo", "bar"); -> "barfoo"
 */
auto strprepend(const char *in, const char *prepend) -> char *
{
  TRACE();
  char *buf    = nullptr;
  int   newlen = 0;
  int   len    = 0;

  if ((in == nullptr) || (prepend == nullptr)) {
    return nullptr;
  }

  newlen = static_cast< uint32_t >(strlen(prepend));
  len    = static_cast< uint32_t >(strlen(in)) + newlen;
  buf    = static_cast< __typeof__(buf) >(MYZALLOC(len + SIZEOF((char) '\0'), "strprepend"));
  if (buf == nullptr) {
    return nullptr;
  }

  strcpy(buf, prepend);
  strcat(buf, in);

  return buf;
}

/*
 * Return the common matching length of two strings.
 */
auto strcommon(const char *a, const char *b) -> uint32_t
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

/*
 * Removes trailing whitespace.
 */
void strchop(char *s)
{
  TRACE();
  uint32_t size = 0;
  char    *end  = nullptr;

  size = static_cast< uint32_t >(strlen(s));
  if (! static_cast< bool >(size)) {
    return;
  }

  end = s + size - 1;
  while ((end >= s) && (*end == ' ')) {
    end--;
  }

  *(end + 1) = '\0';
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

auto strisregexp(const char *in) -> int
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

auto my_strlcpy(char *dst, const char *src, size_t max_len) -> size_t
{
  TRACE();
  size_t const srclen = strlen(src);
  if (max_len > 0) {
    uint32_t const len = std::min(srclen, max_len - 1);
    memcpy(dst, src, len);
    dst[ len ] = '\0';
  }
  return srclen;
}

auto my_strlcat(char *dst, const char *src, size_t max_len) -> size_t
{
  TRACE();
  uint32_t const dstlen = strlen(dst);
  uint32_t const srclen = strlen(src);
  if (dstlen < max_len) {
    my_strlcpy(dst + dstlen, src, max_len - dstlen);
  }
  return (dstlen + srclen);
}

/*
 * Shrink a string by the space needed for its escape characters.
 */
void strnoescape(char *uncompressed)
{
  TRACE();
  char *t = uncompressed;
  char *s = uncompressed;
  char  c = 0;

  while ((c = *s++) != 0) {
    if (c != '\\') {
      *t++ = c;
      continue;
    }

    switch (*s++) {
      case 'n' :  *t++ = '\n'; break;
      case 't' :  *t++ = '\t'; break;
      case '%' :  *t++ = '%'; break;
      case '#' :  *t++ = '@'; break;
      case '\"' : *t++ = '\"'; break;
      case '\'' : *t++ = '\''; break;
      case '\\' : *t++ = '\\'; break;
      default :
        {
          char const *o    = s - 1;
          char        orig = 0;

          while (isdigit(*s) != 0) {
            s++;
          }

          orig = *s;
          *s   = '\0';
          *t++ = atoi(o);
          *s   = orig;
          break;
        }
    }
  }
  *t = '\0';
}

static auto dynvprintf_(const char *fmt, va_list args) -> const char *
{
  TRACE();
  static char buf[ MAXSTR ];

  buf[ 0 ] = '\0';
  vsnprintf(buf, MAXSTR, fmt, args);

  return buf;
}

/*
 * dynamically allocate a printf formatted string. e.g.
 * dynprintf("%sbar", "foo"); => "foobar"
 */
auto dynprintf(const char *fmt, ...) -> char *
{
  TRACE();
  const char *ret  = nullptr;
  va_list     args = {};

  va_start(args, fmt);
  ret = dynvprintf_(fmt, args);
  va_end(args);

  return (MYDUPSTR(ret, __FUNCTION__));
}

/*
 * mybasename
 *
 * A safe wrapper for basename to avoid modifications to the input string.
 */
auto mybasename(const char *in, const char *who) -> std::string
{
  TRACE();
  char       *tmp = MYDUPSTR(in, who);
  std::string tmp2(basename(tmp));
  MYFREE(tmp);

  return (tmp2);
}

/*-
 * Copyright (c) 1990, 1993
 *      The Regents of the University of California.  All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Chris Torek.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *      This product includes software developed by the University of
 *      California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

/*
 * Find the first occurrence of find in s, ignore case.
 */
auto my_strcasestr(const char *s, const char *find) -> char *
{
  TRACE();
  char   c   = 0;
  char   sc  = 0;
  size_t len = 0;

  if ((c = *find++) != 0) {
    c   = tolower(static_cast< uint8_t >(c));
    len = strlen(find);
    do {
      do {
        if ((sc = *s++) == 0) {
          return nullptr;
        }
      } while (static_cast< char >(tolower(static_cast< uint8_t >(sc))) != c);
    } while (strncasecmp(s, find, len) != 0);
    s--;
  }
  return (const_cast< char * >(s));
}

auto split(const std::string &text, int max_line_len) -> std::vector< std::string >
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
    CROAK("bad max line len");
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

auto length_without_format(const std::string &text) -> int
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

auto split_tokens(const std::string &s, const char delimiter) -> std::vector< std::string >
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
auto ltrim(std::string &s) -> std::string &
{
  auto it = std::ranges::find_if(s, [](char c) -> bool { return ! std::isspace< char >(c, std::locale::classic()); });
  s.erase(s.begin(), it);
  return s;
}

auto rtrim(std::string &s) -> std::string &
{
  auto it = std::ranges::find_if(std::ranges::reverse_view(s),
                                 [](char c) -> bool { return ! std::isspace< char >(c, std::locale::classic()); });
  s.erase(it.base(), s.end());
  return s;
}

auto trim(std::string &s) -> std::string & { return ltrim(rtrim(s)); }

const std::string WHITESPACE = " \n\r\t\f\v";

auto ltrim_ws(const std::string &s) -> std::string
{
  size_t const start = s.find_first_not_of(WHITESPACE);
  return (start == std::string::npos) ? "" : s.substr(start);
}

auto rtrim_ws(const std::string &s) -> std::string
{
  size_t const end = s.find_last_not_of(WHITESPACE);
  return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}

auto trim_ws(const std::string &s) -> std::string { return rtrim_ws(ltrim_ws(s)); }

//
// Concert errno to a std::string
//
auto strerror_to_string(const int err) -> std::string
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
auto capitalize(std::string in) -> std::string
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
auto capitalize_first(const std::string &in) -> std::string
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

auto string2tp(const char **s, int *len) -> Tpp
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
