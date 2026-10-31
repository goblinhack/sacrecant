//
// Copyright goblinhack@gmail.com
//

#include "3rdparty/slre.hpp"
#include "my_callstack.hpp"
#include "my_main.hpp" // NOLINT
#include "my_string.hpp"
#include "my_token.hpp"
#include "my_types.hpp"

#include <cmath>
#include <cstdint>
#include <cstring>
#include <print>
#include <span>
#include <string>

static auto tokens_parse(const char *input, class Tokens *tokens) -> class Tokens *
{
  TRACE();

  const char *in  = nullptr;
  char       *out = nullptr;
  char        i   = 0;

  memset(tokens, 0, SIZEOF(*tokens));

  if (strlen(input) >= MAXSHORTSTR) {
    return tokens;
  }

  in   = input;
  out  = tokens->data;
  *out = '\0';

  for (; /*ever*/;) {
    /*
     * Check we have token space.
     */
    if (tokens->cnt >= ARRAY_SIZE(tokens->args)) {
      tokens->cnt = 0;
      return tokens;
    }

    i = *(in++);

    /*
     * End of line ?
     */
    if (i == '\0') {
      return tokens;
    }

    /*
     * Skip leading pad.
     */
    while ((i == ' ') || (i == '\t')) {
      i = *(in++);
    }

    /*
     * End of line ?
     */
    if (i == '\0') {
      return tokens;
    }

    /*
     * Read whole strings.
     */
    if (i == '\"') {
      tokens->args[ tokens->cnt++ ] = out;

      for (; /*ever*/;) {
        i = *(in++);
        switch (i) {
          case '\\' :
            i = *(in++);
            switch (i) {
              case '\"' : i = '\"'; break;
              case '\n' : i = '\n'; break;
              case '\r' : i = '\r'; break;
              case '\t' : i = '\t'; break;
              case '\\' : i = '\\'; break;
              default :   break;
            }
            break;

          case '\"' :
            if (i == '\"') {
              i = '\0';
              break;
            }
            break;
        }

        *out++ = i;

        if (i == '\0') {
          break;
        }
      }

      continue;
    }

    /*
     * End of line ?
     */
    if (i == '\0') {
      return tokens;
    }

    tokens->args[ tokens->cnt++ ] = out;

    for (; /*ever*/;) {
      switch (i) {
        case '\0' : *out++ = i; return tokens;

        case ' ' :  i = '\0'; break;

        case '\t' : i = '\0'; break;

        default :   break;
      }

      *out++ = i;

      if (i == '\0') {
        break;
      }

      i = *(in++);
    }
  }
}

static void tokens_compile(class Tokens *tokens)
{
  TRACE();
  uint32_t cnt = 0;

  cnt = 0;

  while (cnt < tokens->cnt) {
    if (slre_compile(&tokens->regexp[ cnt ], tokens->args[ cnt ]) == 0) {
      log("failed to compile \"%s\"", tokens->args[ cnt ]);
      return;
    }
    cnt++;
  }
}

void tokens_print(class Tokens *tokens)
{
  TRACE();
  uint32_t cnt = 0;

  std::print("tokens {}: ", tokens->cnt);
  cnt = 0;

  while (cnt < tokens->cnt) {
    std::print("[{}] ", tokens->args[ cnt ]);
    cnt++;
  }

  std::println("");
}

void tokens_print_to(class Tokens *tokens, char *output, int output_size)
{
  TRACE();
  std::string joined;

  for (uint32_t cnt = 0; cnt < tokens->cnt; cnt++) {
    if (cnt != 0) {
      joined += ' ';
    }
    joined += tokens->args[ cnt ];
  }

  if (output_size > 0) {
    str_copy(std::span< char >(output, static_cast< size_t >(output_size)), joined);
  }
}

void tokens_test()
{
  TRACE();
  class Tokens tmp {};

  tokens_print(tokens_to_string("s  ", &tmp));
  tokens_print(tokens_to_string("set god mode on", &tmp));
  tokens_print(tokens_to_string("set god \"mode on\"", &tmp));
}

[[nodiscard]] auto tokens_to_string(const char *input, class Tokens *tokens) -> class Tokens *
{
  TRACE();

  class Tokens *t = nullptr; // NOLINT
  t               = tokens_parse(input, tokens);
  tokens_compile(tokens);

  return t;
}
