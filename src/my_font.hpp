//
// Copyright goblinhack@gmail.com
//

#ifndef MY_FONT_HPP
#define MY_FONT_HPP

#include "my_tile.hpp"
#include <array>

extern Fontp font_ui;

#define FONT_CHAR_MAX    ((int) ((int) '~' + 8))
#define FONT_CHAR_MIN    ((int) ' ')
#define FONT_CHAR_CURSOR ((int) ((int) '~' + 1))
#define FONT_CHAR_SHIFT  ((int) ((int) '~' + 2))

#define FONT_TILENAME_UNKNOWN_STR "1.99"
#define FONT_TILENAME_BLOCK_STR   "1.97"
#define FONT_TILENAME_POINTER_STR "1.100"

auto               font_fini() -> void;
[[nodiscard]] auto font_init(Gamep g) -> bool;
[[nodiscard]] auto font_find(const std::string &file) -> Fontp;

class Font
{
public:
  Font()  = default;
  ~Font() = default;
  std::array< int, FONT_CHAR_MAX + 1 >   u_to_c {};
  std::array< Tilep, FONT_CHAR_MAX + 1 > cache {};
  int                                    tile_index {};
  auto                                   font_get_tile(int u) -> Tilep;
};
#endif
