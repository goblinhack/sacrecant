//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_font.hpp"
#include "my_game.hpp"
#include "my_main.hpp"
#include "my_sdl_proto.hpp"
#include "my_tile.hpp"
#include "my_types.hpp"
#include "my_ui.hpp"

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <unordered_map>
#include <utility>

int UI_FONT_WIDTH  = UI_FONT_1_WIDTH;
int UI_FONT_HEIGHT = UI_FONT_1_HEIGHT;

Fontp font_ui;

static std::unordered_map< std::string, Fontp > fonts;

void font_fini() { TRACE(); }

static auto font_load(const std::string &name) -> Fontp
{
  TRACE();

  auto *f = font_find(name);

  if (f != nullptr) {
    return f;
  }

  if (name.empty()) {
    ERR("no name for font %s", __FUNCTION__);
    return nullptr;
  }

  f = new Font();

  auto result = fonts.insert(std::make_pair(name, f));

  if (! result.second) {
    ERR("font insert name [%s] failed", name.c_str());
    return f;
  }

  uint32_t d = 0;
  uint32_t c = FONT_CHAR_MIN;

  while (d < FONT_CHAR_MAX) {
    if (c >= FONT_CHAR_MAX) {
      break;
    }

    f->u_to_c[ c ] = d;
    c++;
    d++;
  }

  return f;
}

[[nodiscard]] auto font_find(const std::string &file) -> Fontp
{
  TRACE();

  if (file.empty()) {
    ERR("no filename given for font find %s", __FUNCTION__);
    return nullptr;
  }

  auto result = fonts.find(file);
  if (result == fonts.end()) {
    return nullptr;
  }

  return result->second;
}

[[nodiscard]] auto Font::font_get_tile(int u) -> Tilep
{
  Font const *me = this;
  if (me == nullptr) [[unlikely]] {
    CROAK("no font");
  }

  if (u < 0) {
    u = -u;
  }

  if ((u < 0) || (u >= FONT_CHAR_MAX)) {
    if (u == '?') {
      CROAK("char 0x%X/%d -> bad index", u, u);
      return font_get_tile('?');
    }
    CROAK("char 0x%X/%d -> bad index", u, u);
    return font_get_tile('?');

    return font_get_tile('?');
  }

  auto index = this->u_to_c[ u ];

  if ((index < 0) || (index >= FONT_CHAR_MAX)) {
    if (u == '?') {
      CROAK("char 0x%X/%d -> bad index %d", u, u, index);
      return font_get_tile('?');
    }
    CROAK("char 0x%X/%d -> bad index %d", u, u, index);
    return font_get_tile('?');

    return font_get_tile('?');
  }

  auto *tile = this->cache[ index ];
  if (tile != nullptr) {
    return tile;
  }

  char tile_name[ 10 ];
  snprintf(tile_name, SIZEOF(tile_name), "%d.%d", tile_index, index);

  tile = tile_find(tile_name);
  if (tile == nullptr) [[unlikely]] {
    if (u == '?') {
      CROAK("char 0x%X/%d -> not found as tile %s", u, u, tile_name);
      return font_get_tile('?');
    }
    CROAK("char 0x%X/%d -> not found as tile %s", u, u, tile_name);
    return font_get_tile('?');
  }

  this->cache[ index ] = tile;

  return tile;
}

[[nodiscard]] auto font_init(Gamep g) -> bool
{
  TRACE();

  auto *font_ui1       = font_load("font1");
  font_ui1->tile_index = 1;

  auto *font_ui2       = font_load("font2");
  font_ui2->tile_index = 2;

  if (game_config_font_get(g) == UI_FONT_1) {
    font_ui        = font_ui1;
    UI_FONT_WIDTH  = UI_FONT_1_WIDTH;
    UI_FONT_HEIGHT = UI_FONT_1_HEIGHT;
  } else {
    font_ui        = font_ui2;
    UI_FONT_WIDTH  = UI_FONT_2_WIDTH;
    UI_FONT_HEIGHT = UI_FONT_2_HEIGHT;
  }
  sdl_config_update_all(g);

  return true;
}
