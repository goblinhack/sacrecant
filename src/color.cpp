//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_color_defs.hpp"
#include "my_globals.hpp"
#include "my_main.hpp"
#include "my_string.hpp"
#include "my_ui.hpp"

#include <unordered_map>

using colors = std::unordered_map< std::string, color >;

static colors color_map;
static bool   color_init_done;

void color_set(const std::string &name, color *c, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
  TRACE();

  c->r = r;
  c->g = g;
  c->b = b;
  c->a = a;

  auto result = color_map.insert(std::make_pair(name, *c));
  if (! result.second) {
    ERR("color insert name [%s] failed", name.c_str());
  }
}

void color_fini()
{
  TRACE();

  //
  // Do not clear the color map - else an error during shutdown will fail to find
  // error colors
  //
  // color_map.clear();
}

auto string2color(const char **s) -> color
{
  TRACE();

  char        tmp[ MAXSHORTSTR ];
  const char *eo_tmp = tmp + SIZEOF(tmp);
  const char *c      = *s;
  char       *t      = tmp; // NOLINT

  while (t < eo_tmp) {
    if ((*c == '\0') || (*c == '$')) {
      break;
    }

    *t++ = *c++;
  }

  if (c == eo_tmp) {
    return WHITE;
  }

  *t++ = '\0';
  *s += (t - tmp);

  if (strcasecmp(tmp, "reset") == 0) {
    return UI_TEXT_COLOR;
  }

  auto result = color_map.find(std::string(tmp));

  if (result == color_map.end()) {
    if (color_init_done) { // avoids color warnings due to very early errors
      ERR("string2color: unknown color [%s]", tmp);
    }
    return WHITE;
  }

  return result->second;
}

auto string2colorname(std::string &s, int *len) -> std::string
{
  TRACE();

  auto        iter = s.begin();
  std::string out;

  while (iter != s.end()) {
    auto c = *iter;

    if ((c == '\0') || (c == '$')) {
      break;
    }

    out += c;
    iter++;
  }

  if (len != nullptr) {
    *len = iter - s.begin();
  }

  return out;
}

auto string2color(std::string &s, int *len) -> color
{
  TRACE();

  auto        iter = s.begin();
  std::string out;

  while (iter != s.end()) {
    auto c = *iter;

    if ((c == '\0') || (c == '$')) {
      break;
    }

    out += c;
    iter++;
  }

  if (len != nullptr) {
    *len = iter - s.begin();
  }

  if (out == "reset") {
    return UI_TEXT_COLOR;
  }

  auto result = color_map.find(out);

  if (result == color_map.end()) {
    if (color_init_done) { // avoids color warnings due to very early errors
      ERR("string2color(2): unknown color [%s]", out.c_str());
    }
    return WHITE;
  }

  return result->second;
}

auto string2color(std::string &s) -> color
{
  auto        iter = s.begin();
  std::string out;

  while (iter != s.end()) {
    auto c = *iter;

    if ((c == '\0') || (c == '$')) {
      break;
    }

    out += c;
    iter++;
  }

  if (out == "reset") {
    return UI_TEXT_COLOR;
  }

  auto result = color_map.find(out);

  if (result == color_map.end()) {
    if (color_init_done) { // avoids color warnings due to very early errors
      ERR("string2color(3): unknown color [%s]", out.c_str());
    }
    return WHITE;
  }

  return result->second;
}

auto color_find(const char *s) -> color
{
  TRACE();

  if (s == nullptr) {
    return WHITE;
  }

  if (strcmp(s, "") == 0) {
    return WHITE;
  }

  auto result = color_map.find(std::string(s));

  if (result == color_map.end()) {
    if (color_init_done) { // avoids color warnings due to very early errors
      ERR("color_find: unknown color [%s]", s);
    }
    return WHITE;
  }

  return result->second;
}

auto color_eq(const color &col1, const color &col2) -> bool
{
  return col1.r == col2.r && col1.g == col2.g && col1.b == col2.b && col1.a == col2.a;
}

auto color_neq(const color &col1, const color &col2) -> bool
{
  return col1.r != col2.r || col1.g != col2.g || col1.b != col2.b || col1.a != col2.a;
}
