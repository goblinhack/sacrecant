//
// Copyright goblinhack@gmail.com
//

#include "my_ascii.hpp"
#include "my_callstack.hpp"
#include "my_color_defs.hpp"
#include "my_game.hpp"
#include "my_game_inlines.hpp"
#include "my_level.hpp"
#include "my_main.hpp"
#include "my_sdl_proto.hpp"
#include "my_sound.hpp"
#include "my_spoint.hpp"
#include "my_thing.hpp"
#include "my_thing_inlines.hpp"
#include "my_tile.hpp"
#include "my_tp.hpp"
#include "my_types.hpp"
#include "my_ui.hpp"
#include "my_wid.hpp"
#include "my_wids.hpp"

#include <SDL_keyboard.h>
#include <SDL_keycode.h>
#include <cstdint>
#include <cstring>
#include <string>
#include <utility>
#include <vector>

static Widp wid_throw_item_window;

static Widp                  wid_shortcut[ THING_INVENTORY_MAX ];
static Widp                  wid_item[ THING_INVENTORY_MAX ];
static Widp                  wid_icon[ THING_INVENTORY_MAX ];
static std::vector< Thingp > items;

static void wid_throw_item_destroy(Gamep g)
{
  TRACE();

  memset(wid_shortcut, 0, sizeof(wid_shortcut));
  memset(wid_icon, 0, sizeof(wid_icon));
  memset(wid_item, 0, sizeof(wid_item));

  if (wid_throw_item_window != nullptr) {
    wid_destroy(g, &wid_throw_item_window);
  }

  game_throw_id_set(g, 0);
}

static void wid_throw_item_mouse_over_begin(Gamep g, Widp w, int /*relx*/, int /*rely*/, int /*wheelx*/, int /*wheely*/)
{
  TRACE();

  for (auto &n : wid_item) {
    if (n != nullptr) {
      if (n != wid_over) {
        wid_set_style(n, UI_WID_STYLE_BUTTON_ROUNDED_SOLID);
        wid_set_color(n, WID_COLOR_BG, GRAY20);
      }
    }
  }

  auto *v = game_levels_get(g);
  if (v == nullptr) [[unlikely]] {
    return;
  }

  auto *item = wid_get_thing_context(g, v, w, 0);
  if (item == nullptr) {
    return;
  }

  level_cursor_describe_clear(g, v);
  if (level_cursor_describe_add(g, v, item)) {
    game_request_to_remake_ui_set(g);
  }
}

static void wid_throw_item_mouse_over_end(Gamep g, Widp w)
{
  TRACE();

  auto *v = game_levels_get(g);
  if (v == nullptr) [[unlikely]] {
    return;
  }

  auto *item = wid_get_thing_context(g, v, w, 0);
  if (item == nullptr) {
    return;
  }

  if (level_cursor_describe_remove(g, v, item)) {
    game_request_to_remake_ui_set(g);
  }
}

[[nodiscard]] static auto wid_throw_item_mouse_down(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();

  auto *v = game_levels_get(g);
  if (v == nullptr) [[unlikely]] {
    return false;
  }

  auto *l = game_level_get(g, v);
  if (l == nullptr) [[unlikely]] {
    return false;
  }

  auto *player = thing_player(g);
  if (player == nullptr) [[unlikely]] {
    return false;
  }

  auto *item = wid_get_thing_context(g, v, w, 0);
  if (item == nullptr) {
    return false;
  }

  topcon("Choose your target.");
  wid_throw_item_destroy(g);

  game_throw_id_set(g, item->id);
  game_state_change(g, STATE_CHOOSE_THROW_TARGET, "choose a target");

  return true;
}

[[nodiscard]] static auto wid_throw_item_key_down(Gamep g, Widp w, const struct SDL_Keysym *key) -> bool
{
  TRACE();

  if (sdlk_eq(*key, game_key_console_get(g))) {
    (void) sound_play(g, "keypress");
    return false;
  }

  switch (key->mod) {
    case KMOD_LCTRL :
    case KMOD_RCTRL :
    default :
      switch (key->sym) {
        default :
          {
            TRACE();
            auto c = wid_event_to_char(key);
            switch (c) {
              case 'a' :
              case 'b' :
              case 'c' :
              case 'd' :
              case 'e' :
              case 'f' :
              case 'g' :
              case 'h' :
              case 'i' :
              case 'j' :
              case 'k' :
              case 'l' :
              case 'm' :
              case 'n' :
              case 'o' :
              case 'p' :
              case 'q' :
              case 'r' :
              case 's' :
              case 't' :
              case 'u' :
              case 'v' :
              case 'w' :
              case 'x' :
              case 'y' :
              case 'z' :
                (void) sound_play(g, "keypress");
                for (auto &n : wid_item) {
                  w = n;
                  if (w != nullptr) {
                    wid_set_style(w, UI_WID_STYLE_BUTTON_ROUNDED_SOLID);
                    wid_set_color(w, WID_COLOR_BG, GRAY20);
                  }
                }

                wid_unset_focus(g);

                w = wid_icon[ c - 'a' ];
                if (w != nullptr) {
                  (void) wid_throw_item_mouse_down(g, w, -1, -1, 0);
                }

                w = wid_item[ c - 'a' ];
                if (w != nullptr) {
                  wid_set_style(w, UI_WID_STYLE_BUTTON_ROUNDED_SOLID);
                  wid_set_color(w, WID_COLOR_BG, GREEN);
                  wid_set_color(w, WID_COLOR_TEXT_FG, WHITE);
                }
                return true;
              case '\n' :
              case SDLK_ESCAPE :
                {
                  TRACE();
                  (void) sound_play(g, "keypress");
                  wid_throw_item_destroy(g);
                  game_state_change(g, STATE_PLAYING, "close throw menu");
                  return true;
                }
            }
          }
      }
  }

  //
  // So screenshots can work
  //
  return false;
}

[[nodiscard]] static auto wid_throw_item_back(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();
  wid_throw_item_destroy(g);
  game_state_change(g, STATE_PLAYING, "close throw");
  return true;
}

void wid_throw_item_show(Gamep g, Levelsp v, Levelp l, Thingp player, std::vector< Thingp > items_in)
{
  TRACE();

  if (wid_throw_item_window != nullptr) {
    wid_throw_item_destroy(g);
  }

  items = std::move(items_in);

  if (thing_is_dead(player)) {
    return;
  }

  auto *ext = thing_ext_struct(g, v, player);
  if (ext == nullptr) {
    return;
  }

  const int menu_width  = UI_INVENTORY_WIDTH;
  const int menu_height = UI_INVENTORY_HEIGHT;

  const auto button_width  = menu_width - 4;
  const auto button_height = 0;
  const auto button_step   = 1;
  const auto button_style  = UI_WID_STYLE_SPARSE_NONE;

  auto y_at = 2;

  const int left_half  = menu_width / 2;
  const int right_half = menu_width - left_half;
  const int top_half   = menu_height / 2;
  const int bot_half   = menu_height - top_half;

  {
    TRACE();
    spoint const tl((TERM_WIDTH / 2) - left_half, (TERM_HEIGHT / 2) - top_half);
    spoint const br((TERM_WIDTH / 2) + right_half - 1, (TERM_HEIGHT / 2) + bot_half - 1);

    wid_throw_item_window = wid_new_window(g, "widget throw");
    wid_set_pos(wid_throw_item_window, tl, br);
    wid_set_style(wid_throw_item_window, UI_WID_STYLE_BUTTON_OUTLINE);
    wid_set_on_key_down(wid_throw_item_window, wid_throw_item_key_down);
    wid_set_text(wid_throw_item_window, "Select an item to throw");
    wid_set_text_top(wid_throw_item_window, 1u);
    wid_raise(g, wid_throw_item_window);
  }

  {
    TRACE();
    auto        *w = wid_new_square_button(g, wid_throw_item_window, "text");
    spoint const tl(0, y_at);
    spoint const br(menu_width, y_at);
    wid_set_pos(w, tl, br);
    wid_set_text(w, UI_FMT_STR "Mouse select an item or press key a-z");
    wid_set_style(w, UI_WID_STYLE_BUTTON_OUTLINE);
    wid_set_shape_none(w);
    wid_set_text_centerx(w, 1u);
    y_at += 2;
  }

  memset(wid_shortcut, 0, sizeof(wid_shortcut));
  memset(wid_icon, 0, sizeof(wid_icon));
  memset(wid_item, 0, sizeof(wid_item));

  auto _n_ = 0;
  for (auto *item : items) {

    auto *tp = thing_tp(item);

    //
    // Item icon
    //
    {
      Tilep tile = tp_tiles_get(tp, THING_ANIM_IDLE, 0);
      if (tile != nullptr) {
        TRACE();
        auto        *w = wid_new_square_button(g, wid_throw_item_window, "Icon");
        spoint const tl(1, y_at);
        spoint const br(1, y_at);
        wid_set_tile(TILE_LAYER_TEXT_FG, w, tile);
        wid_set_style(w, button_style);
        wid_set_pos(w, tl, br);

        if (item != nullptr) {
          wid_set_thing_context(g, v, w, item);
          wid_set_on_mouse_down(w, wid_throw_item_mouse_down);
        }

        wid_set_on_mouse_over_begin(w, wid_throw_item_mouse_over_begin);
        wid_set_on_mouse_over_end(w, wid_throw_item_mouse_over_end);

        wid_icon[ _n_ ] = w;
      }
    }

    //
    // Key shortcut
    //
    {
      TRACE();
      auto *w = wid_new_square_button(g, wid_throw_item_window, "Key");

      std::string s;
      s += static_cast< char >('a' + _n_);
      s += ')';

      spoint const tl(3, y_at);
      spoint const br(6, y_at + button_height);
      wid_set_text_lhs(w, 1u);

      wid_set_mode(w, WID_MODE_NORMAL);
      wid_set_color(w, WID_COLOR_TEXT_FG, GRAY50);
      wid_set_style(w, button_style);
      wid_set_pos(w, tl, br);
      wid_set_text(w, s);

      if (item != nullptr) {
        wid_set_thing_context(g, v, w, item);
        wid_set_on_mouse_down(w, wid_throw_item_mouse_down);
      }

      wid_set_on_mouse_over_begin(w, wid_throw_item_mouse_over_begin);
      wid_set_on_mouse_over_end(w, wid_throw_item_mouse_over_end);

      wid_shortcut[ _n_ ] = w;
    }

    //
    // Item name
    //
    {
      std::string line;

      if (item != nullptr) {
        line = tp_name_long(tp);
      } else {
        line = "-";
      }

      auto count = thing_inventory_get_item_count(g, v, l, item, player);
      if (count > 1) {
        line += " x";
        line += std::to_string(count);
      }

      line += " ";

      if (item != nullptr) {
        auto charge_count = thing_charge_count(item);
        if (charge_count > 0) {
          line += std::to_string(charge_count);
          line += " charges %%tile=icon_lightning$";
        }

        if (thing_is_worn(item)) {
          if (thing_is_boots(item)) {
            line += "%%tile=icon_boots$";
          } else if (thing_is_ring(item)) {
            line += "%%tile=icon_ring$";
          } else {
            line += "%%tile=icon_hand$";
          }
        }
      }

      {
        TRACE();
        auto *w = wid_new_button(g, wid_throw_item_window, "Item");

        spoint const tl(6, y_at);
        spoint const br(button_width, y_at + button_height);
        wid_set_text_lhs(w, 1u);
        wid_set_pos(w, tl, br);
        wid_set_text(w, line);

        if (item != nullptr) {
          wid_set_thing_context(g, v, w, item);
          wid_set_on_mouse_down(w, wid_throw_item_mouse_down);
        }

        wid_set_on_mouse_over_begin(w, wid_throw_item_mouse_over_begin);
        wid_set_on_mouse_over_end(w, wid_throw_item_mouse_over_end);

        wid_item[ _n_ ] = w;
      }
    }

    y_at += button_step;
    _n_++;
  }

  {
    TRACE();
    auto *w = wid_new_back_button(g, wid_throw_item_window, "BACK");

    spoint const tl((menu_width / 2) - 4, menu_height - 4);
    spoint const br((menu_width / 2) + 3, menu_height - 2);
    wid_set_on_mouse_down(w, wid_throw_item_back);
    wid_set_pos(w, tl, br);
  }

  wid_update(g, wid_throw_item_window);

  game_state_change(g, STATE_THROW_MENU, "throw");
}
