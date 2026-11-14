//
// Copyright goblinhack@gmail.com
//

#include <string>
#include <utility>

#include "my_callstack.hpp"
#include "my_color.hpp"
#include "my_color_defs.hpp"
#include "my_font.hpp"
#include "my_game.hpp"
#include "my_game_defs.hpp"
#include "my_spoint.hpp"
#include "my_string.hpp"
#include "my_tex.hpp"
#include "my_thing.hpp" // NOLINT
#include "my_tile.hpp"
#include "my_tp.hpp"
#include "my_types.hpp"

void thing_blit_text(Gamep g, Levelsp v, Levelp l, spoint tl, spoint br, std::string const &text, color fg, bool outline)
{
  TRACE();

  auto      single_pix_size = game_map_single_pix_size_get(g);
  int const len             = string_length_without_format(text);

  if (br.x < tl.x) {
    std::swap(tl, br);
    std::swap(tl.y, br.y);
  }

  auto w  = br.x - tl.x;
  auto ow = w;
  auto h  = br.y - tl.y;
  auto oh = h;

  w = static_cast< int >((static_cast< float >(w)) / static_cast< float >(POPUP_TEXT_SCALE_ACROSS));
  h = static_cast< int >((static_cast< float >(h)) / static_cast< float >(POPUP_TEXT_SCALE_DOWN));

  auto pix_w = w * len;
  tl.x -= pix_w / 2;
  tl.x += ow / 2;

  br.x = tl.x + w;
  br.y = tl.y + h;

  tl.y += oh / 2;
  br.y = tl.y + h;

  //
  // A %tp= or %tile= applies to the character that follows, so remember it until we draw that.
  //
  Tilep        tile = nullptr;
  const size_t size = text.size();
  size_t       pos  = 0;

  while (pos < size) {
    auto const tok = text_next_token(text, pos);
    pos            = tok.next;

    switch (tok.type) {
      case TextTokenType::Char : break;
      case TextTokenType::Skip :
      case TextTokenType::Bg :
      case TextTokenType::Tex :  continue;
      case TextTokenType::Fg :
        {
          std::string name = text.substr(tok.arg, tok.arg_len);
          int         len2 = 0;
          fg               = string2color(name, &len2);
          continue;
        }
      case TextTokenType::Tp :
        {
          std::string name = text.substr(tok.arg, tok.arg_len + (tok.terminated ? 1 : 0));
          const char *tmpc = name.c_str();
          int         len2 = 0;
          tile             = tp_first_tile(string2tp(&tmpc, &len2), THING_ANIM_IDLE);
          continue;
        }
      case TextTokenType::Tile :
        {
          std::string name = text.substr(tok.arg, tok.arg_len + (tok.terminated ? 1 : 0));
          int         len2 = 0;
          tile             = string2tile(name, &len2);
          continue;
        }
    }

    auto const c = tok.ch;

    if (tile == nullptr) {
      tile = font_ui->font_get_tile(c);
      if (tile == nullptr) {
        tile = tile_find_mand(FONT_TILENAME_UNKNOWN_STR);
      }
    }

    if (outline) {
      float x1 = 0;
      float x2 = 0;
      float y1 = 0;
      float y2 = 0;
      tile_coords(tile, &x1, &y1, &x2, &y2);
      if (single_pix_size > 1) {
        tile_blit_outlined(tile, x1, x2, y1, y2, tl, br, fg, BLACK, single_pix_size, true);
        tile_blit_outlined(tile, x1, x2, y1, y2, tl + spoint(1, 0), br + spoint(1, 0), fg, BLACK, single_pix_size, true);
        tile_blit_outlined(tile, x1, x2, y1, y2, tl - spoint(1, 0), br - spoint(1, 0), fg, BLACK, single_pix_size, true);
        tile_blit_outlined(tile, x1, x2, y1, y2, tl + spoint(0, 1), br + spoint(0, 1), fg, BLACK, single_pix_size, true);
        tile_blit_outlined(tile, x1, x2, y1, y2, tl - spoint(0, 1), br - spoint(0, 1), fg, BLACK, single_pix_size, true);
      }
    } else {
      tile_blit(tile, tl, br, fg);

      //
      // Text seems a bit skinny
      //
      if (single_pix_size > 1) {
        tile_blit(tile, tl + spoint(1, 0), br + spoint(1, 0), fg);
      }
    }

    tl.x += w;
    br.x += w;
    tile = nullptr;
  }
  GLCOLOR(WHITE);
}
