//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_game.hpp"
#include "my_game_popups.hpp"
#include "my_gl.hpp"
#include "my_globals.hpp"
#include "my_level.hpp"
#include "my_level_inlines.hpp"
#include "my_main.hpp"
#include "my_time.hpp"

#include <algorithm>

void game_popup_text_add(Gamep g, int x, int y, const std::string &text, color c)
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return;
  }

  if (is_oob(x, y)) [[unlikely]] {
    ERR("popup text is oob");
    return;
  }

  //
  // Merge numeric values if we can
  //
  try {
    auto value1 = std::stoi(text);
    for (auto *i : *game_popups_get(g, x, y)) {
      try {
        auto value2 = std::stoi(i->text);
        i->text     = std::to_string(value1 + value2);
        i->created  = time_ms_cached();
        i->fg       = c;
        i->y_offset = 0;
        return;
      } catch (...) {
        ;
      }
    }
  } catch (...) {
    ;
  }

  //
  // Merge identical values if we can
  //
  for (auto *i : *game_popups_get(g, x, y)) {
    if (i->text == text) {
      i->count++;
      i->y_offset = 0;
      return;
    }
  }

  auto *l        = game_popups_get(g, x, y);
  auto *popup    = new GamePopup;
  popup->text    = text;
  popup->created = time_ms_cached();
  popup->fg      = c;
  l->push_front(popup);
}

void game_popups_age(Gamep g)
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return;
  }

  for (auto y = 0; y < MAP_HEIGHT; y++) {
    for (auto x = 0; x < MAP_WIDTH; x++) {
      if (! game_popups_present(g, x, y)) {
        continue;
      }

      //
      // Age out popups
      //
      std::list< GamePopup * > out;
      for (auto *i : *game_popups_get(g, x, y)) {
        if (! time_have_x_ms_passed_since(POPUP_DURATION_MS, i->created)) {
          out.push_back(i);
        } else {
          delete i;
        }
      }

      game_popups_set(g, x, y, out);
    }
  }
}

void game_popups_display(Gamep g, Levelsp v, Levelp l)
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return;
  }

  for (auto y = 0; y < MAP_HEIGHT; y++) {
    for (auto x = 0; x < MAP_WIDTH; x++) {
      if (! game_popups_present(g, x, y)) {
        continue;
      }

      spoint last_tl = {};
      spoint last_br = {};

      const auto debug = false;
      if (debug) {
        CON("(%d,%d)", x, y);
      }

      for (auto *i : *game_popups_get(g, x, y)) {
        uint16_t     tile_index = 0;
        bpoint const p(x, y);
        spoint       tl = {};
        spoint       br = {};

        //
        // Get the coords of the tile on the map this text would be over
        //
        static Tpp tp_once;
        if (tp_once == nullptr) {
          tp_once = tp_find_mand("cursor_path");
        }
        thing_display_get_tile_info(g, v, l, p, tp_once, NULL_THING, tl, br, &tile_index);

        //
        // Make the text look a bit more squarish
        //
        auto dx = br.x - tl.x;
        br.x    = tl.x + (int) (((float) dx * 0.7));

        //
        // Fade out and raise the text up with a percentage
        //
        float const pct = static_cast< float >(time_ms_cached() - i->created) / static_cast< float >(POPUP_DURATION_MS);

        //
        // Fade out
        //
        color fg = i->fg;
        color bg = BLACK;

        fg.a = 255 - static_cast< int >(255.0 * pct / 2);
        bg.a = 255 - static_cast< int >(255.0 * pct);

        //
        // Rise up
        //
        const auto tile_height = br.y - tl.y;
        auto ascend_height = static_cast< int >(pct * static_cast< float >(tile_height) * static_cast< float >(POPUP_DURATION_TILE_HEIGHT));
        tl.y -= ascend_height;
        br.y -= ascend_height;

        //
        // Start the text above the player
        //
        tl.y -= tile_height / 2;
        br.y -= tile_height / 2;
        tl.y -= i->y_offset;
        br.y -= i->y_offset;

        br.y = tl.y + (tile_height / 2);

        if (debug) {
          CON("popup: %s %u..%u last %u...%u", i->text.c_str(), tl.y, br.y, last_tl.y, last_br.y);
        }

        //
        // Avoid overlapping popups
        //
        while (((tl.y >= last_tl.y) && (tl.y <= last_br.y)) || ((br.y >= last_tl.y) && (br.y <= last_br.y))) {
          auto offset = 1;
          i->y_offset += offset;
          tl.y -= offset;
          br.y -= offset;

          if (debug) {
            CON("popup: changed to %s %u..%u last %u...%u", i->text.c_str(), tl.y, br.y, last_tl.y, last_br.y);
          }
        }

        auto text = i->text;
        if (i->count) {
          text += "x";
          text += std::to_string(i->count + 1);
        }

        //
        // Disable outlines when zoomed out
        //
        if (! game_map_zoom_is_full_map_visible(g)) {
          blit_init();
          thing_blit_text(g, v, l, tl, br, text, bg, true /* outline */);
          blit_flush();
        }

        blit_init();
        thing_blit_text(g, v, l, tl, br, text, fg, false /* outline */);
        blit_flush();

        last_tl = tl;
        last_br = br;
      }

      if (debug) {
        CON("-");
      }
    }
  }
}

void game_popups_clear(Gamep g)
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return;
  }

  for (auto y = 0; y < MAP_HEIGHT; y++) {
    for (auto x = 0; x < MAP_WIDTH; x++) {
      if (! game_popups_present(g, x, y)) {
        continue;
      }

      auto *l = game_popups_get(g, x, y);
      for (auto *p : *l) {
        delete p;
      }
      l->clear();
    }
  }
}
