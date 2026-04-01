//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_command.hpp"
#include "my_globals.hpp"
#include "my_main.hpp"
#include "my_ui.hpp"

#include <unordered_map>
#include <utility>

/*
 * Simple console expanding code, takes a command input and expands it as
 * much as possible to installed commands.
 *
 * Ambigious command : []
 *  Possible match   : set god-mode off
 *  Possible match   : set god-mode on
 *  Possible match   : set debug off
 *  Possible match   : set debug on
 *   Expand to       :
 *
 * Ambigious command : [s]
 *  Possible match   : set god-mode off
 *  Possible match   : set god-mode on
 *  Possible match   : set debug off
 *  Possible match   : set debug on
 *   Expand to       : set
 *
 * Ambigious command : [se]
 *  Possible match   : set god-mode off
 *  Possible match   : set god-mode on
 *  Possible match   : set debug off
 *  Possible match   : set debug on
 *   Expand to       : set
 *
 * Ambigious command : [set]
 *  Possible match   : set god-mode off
 *  Possible match   : set god-mode on
 *  Possible match   : set debug off
 *  Possible match   : set debug on
 *   Expand to       : set
 *
 * Ambigious command : [set d]
 *  Possible match   : set debug off
 *  Possible match   : set debug on
 *   Expand to       : set debug
 *
 * Ambigious command : [set g]
 *  Possible match   : set god-mode off
 *  Possible match   : set god-mode on
 *   Expand to       : set god-mode
 *
 * Ambigious command : [s g ]
 *  Possible match   : set god-mode off
 *  Possible match   : set god-mode on
 *   Expand to       : set god-mode
 *
 * Ambigious command : [s g o]
 *  Possible match   : set god-mode off
 *  Possible match   : set god-mode on
 *   Expand to       : set god-mode o
 *
 * Single match      : [s g of]
 *  Completes to     : set god-mode off
 *
 * Ambigious command : [s d]
 *  Possible match   : set debug off
 *  Possible match   : set debug on
 *   Expand to       : set debug
 *
 * Single match      : [s d on]
 *  Completes to     : set debug on
 *
 * Single match      : [s d of]
 *  Completes to     : set debug off
 *
 * Single match      : [set debug on]
 *  Completes to     : set debug on
 */

class CommandT
{
public:
  CommandT()
  {
    memset(&this->tokens, 0, SIZEOF(this->tokens));
    memset(&this->readable_tokens, 0, SIZEOF(this->readable_tokens));
    memset(&this->input_tokens, 0, SIZEOF(this->input_tokens));
  }

  ~CommandT() = default;

  class Tokens tokens {};
  class Tokens readable_tokens {};
  class Tokens input_tokens {};
  command_fn_t callback {nullptr};
};

using commandp = class CommandT *;
using commands = std::unordered_map< std::string, commandp >;
static commands commands_map;

static bool command_inited;

void command_fini()
{
  TRACE();

  if (command_inited) {
    command_inited = false;
    for (const auto &iter : commands_map) {
      auto *command = iter.second;
      delete command;
    }
  }
}

auto command_init() -> bool
{
  TRACE();

  command_inited = true;

  return true;
}

void command_add(Gamep g, command_fn_t callback, const std::string &input, const std::string &readable)
{
  TRACE();

  auto *command = new class CommandT();
  auto  result  = commands_map.insert(std::make_pair(input, command));

  if (! result.second) {
    ERR("command insert name [%s] failed", input.c_str());
    return;
  }

  command->callback = callback;

  //
  // Convert the command into tokens for matching.
  //
  tokens_to_string(input.c_str(), &command->tokens);
  tokens_to_string(input.c_str(), &command->input_tokens);
  tokens_to_string(readable.c_str(), &command->readable_tokens);
}

static auto command_matches(Gamep g, const char *input, char *output, uint8_t show_ambiguous, uint8_t show_complete,
                            uint8_t execute_command, void *context) -> int
{
  TRACE();

  char         cand_expand_to[ MAXSTR ];
  commandp     matched_command = nullptr;
  char         completes_to[ MAXSTR ];
  char         expands_to[ MAXSTR ];
  class Tokens input_tokens {};
  char         match[ MAXSTR ];
  char         match2[ MAXSTR ];
  int          longest_match = 0;
  int          common_len    = 0;
  int          matches       = 0;
  int          cnt           = 0;
  int          t             = 0;

  longest_match = -1;
  matches       = 0;

  /*
   * Convert the input into tokens for matching.
   */
  tokens_to_string(input, &input_tokens);

  /*
   * Find the command(s) with the most number of matching tokens.
   */
  for (const auto &iter : commands_map) {
    auto *command = iter.second;

    for (t = 0; std::cmp_less(t, std::min(command->tokens.cnt, input_tokens.cnt)); t++) {

      cnt = strncmp(command->tokens.args[ t ], input_tokens.args[ t ], strlen(input_tokens.args[ t ]));

      if (slre_match(&command->tokens.regexp[ t ], input_tokens.args[ t ], static_cast< int >(strlen(input_tokens.args[ t ])),
                     nullptr /* captures */)
          != 0) {
        /*
         * Success
         */
        cnt = 0;
      }

      if (cnt != 0) {
        t = -1;
        break;
      }
    }

    longest_match = std::max(t, longest_match);
  }

  if (longest_match == -1) {
    return 0;
  }

  /*
   * Repeat and optionally dump other possibilities if the command is
   * not complete.
   */
  for (const auto &iter : commands_map) {
    auto *command = iter.second;

    for (t = 0; std::cmp_less(t, std::min(command->tokens.cnt, input_tokens.cnt)); t++) {

      cnt = strncmp(command->tokens.args[ t ], input_tokens.args[ t ], strlen(input_tokens.args[ t ]));

      if (slre_match(&command->tokens.regexp[ t ], input_tokens.args[ t ], static_cast< int >(strlen(input_tokens.args[ t ])),
                     nullptr /* captures */)
          != 0) {
        /*
         * Success
         */
        cnt = 0;
      }

      if (cnt != 0) {
        break;
      }
    }

    // tokens_print_to(&command->readable_tokens, match, SIZEOF(match));
    if (t == longest_match) {
      matches++;
      // con("  MATCH    \"%s\" [%d] longest %d", match,t,longest_match);

      matched_command = command;

      if (static_cast< bool >(show_complete)) {
        completes_to[ 0 ] = '\0';

        for (t = 0; t < longest_match; t++) {
          my_strlcat(completes_to, command->tokens.args[ t ], SIZEOF(completes_to));
          my_strlcat(completes_to, " ", SIZEOF(completes_to));
        }

        if (output != nullptr) {
          my_strlcpy(output, completes_to, MAXSTR);
        }
      }

      tokens_print_to(&command->input_tokens, match, SIZEOF(match));

      tokens_print_to(&command->readable_tokens, match2, SIZEOF(match2));

      if (static_cast< bool >(show_ambiguous)) {
        con("  %s -- %s", match, match2);
      }
    } else {
      // con("  NO MATCH \"%s\" [%d] longest %d", match,t,longest_match);
    }
  }

  /*
   * Repeat and complete the command to any full matches.
   */
  {
    expands_to[ 0 ] = '\0';

    {
      for (const auto &iter : commands_map) {
        auto *command = iter.second;

        for (t = 0; std::cmp_less(t, std::min(command->tokens.cnt, input_tokens.cnt)); t++) {

          cnt = strncmp(command->tokens.args[ t ], input_tokens.args[ t ], strlen(input_tokens.args[ t ]));

          if (slre_match(&command->tokens.regexp[ t ], input_tokens.args[ t ], static_cast< int >(strlen(input_tokens.args[ t ])),
                         nullptr /* captures */)
              != 0) {
            /*
             * Success
             */
            cnt = 0;
          }

          if (cnt != 0) {
            break;
          }
        }

        if (t == longest_match) {
          cand_expand_to[ 0 ] = '\0';

          for (t = 0; t < longest_match; t++) {
            if (strisregexp(command->tokens.args[ t ]) != 0) {
              my_strlcat(cand_expand_to, input_tokens.args[ t ], SIZEOF(cand_expand_to));
              my_strlcat(cand_expand_to, " ", SIZEOF(cand_expand_to));
              continue;
            }

            my_strlcat(cand_expand_to, command->tokens.args[ t ], SIZEOF(cand_expand_to));

            my_strlcat(cand_expand_to, " ", SIZEOF(cand_expand_to));
          }

          if (expands_to[ 0 ] != '\0') {
            common_len               = strcommon(expands_to, cand_expand_to);
            expands_to[ common_len ] = '\0';
          } else {
            my_strlcpy(expands_to, cand_expand_to, SIZEOF(expands_to));
          }
        }
      }

      /*
       * Expands to:
       */
      if (output != nullptr) {
        my_strlcpy(output, expands_to, MAXSTR);
      }
    }
  }

  if ((static_cast< bool >(execute_command)) && (matched_command != nullptr) && (matches == 1)) {
    (*matched_command->callback)(g, &input_tokens, context);
  }

  return matches;
}

auto command_handle(Gamep g, const char *input, char *expandedtext, uint8_t show_ambiguous, uint8_t show_complete, uint8_t execute_command,
                    void *context) -> uint8_t
{
  TRACE();

  int matches = 0;

  if (expandedtext != nullptr) {
    *expandedtext = '\0';
  }

  /*
   * Check for ambiguous commands.
   */
  matches = command_matches(g, input, expandedtext, 0U, 0U, execute_command, context);
  if (matches == 0) {
    con(">" UI_IMPORTANT_FMT_STR "unknown command: \"%s\"" UI_RESET_FMT "", input);
    return 0U;
  }

  if (matches > 1) {
    if (static_cast< bool >(show_ambiguous)) {
      if (*input != 0) {
        con(">" UI_INFO_FMT_STR "Multiple matches, \"%s\"" UI_RESET_FMT ". Try:", input);
      } else {
        con(">" UI_INFO_FMT_STR "Commands:" UI_RESET_FMT);
      }
    }

    command_matches(g, input, expandedtext, show_ambiguous, show_complete, execute_command, context);

    if (! static_cast< bool >(show_ambiguous)) {
      if (expandedtext != nullptr) {
        if (strcasecmp(input, expandedtext) == 0) {
          con(">" UI_INFO_FMT_STR "Incomplete command, \"%s\"" UI_RESET_FMT ". Try:", input);

          command_matches(g, input, expandedtext, 1U, show_complete, execute_command, context);
        }
      } else {
        command_matches(g, input, expandedtext, 1U, show_complete, execute_command, context);
      }
    }

    return 0U;
  }

  if ((! static_cast< bool >(execute_command)) && (matches == 1)) {
    con(">" UI_INFO_FMT_STR "Incomplete command, \"%s\"" UI_RESET_FMT ". Try:", input);

    command_matches(g, input, expandedtext, 1U, show_complete, execute_command, context);
  }

  return 1U;
}

auto command_handle(Gamep g, const std::string &input, std::string *expanded_text, uint8_t show_ambiguous, uint8_t show_complete,
                    uint8_t execute_command, void *context) -> uint8_t
{
  char buf[ MAXSTR ];

  buf[ 0 ] = '\0';

  uint8_t const r = command_handle(g, input.c_str(), &buf[ 0 ], show_ambiguous, show_complete, execute_command, context);

  if (expanded_text != nullptr) {
    *expanded_text = std::string(buf);
  }

  return r;
}
