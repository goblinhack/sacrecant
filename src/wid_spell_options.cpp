//
// Copyright goblinhack@gmail.com
//

#include "my_ascii.hpp"
#include "my_callstack.hpp"
#include "my_color_defs.hpp"
#include "my_game.hpp"
#include "my_level.hpp"
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

static Widp wid_spell_options_window;

static Widp   wid_shortcut[ THING_SPELLBOOK_MAX ];
static Widp   wid_spell_option[ THING_SPELLBOOK_MAX ];
static Thingp wid_spell_option_chosen;

static void wid_spell_options_destroy(Gamep g)
{
  TRACE();

  memset(wid_shortcut, 0, sizeof(wid_shortcut));
  memset(wid_spell_option, 0, sizeof(wid_spell_option));

  if (wid_spell_options_window != nullptr) {
    wid_destroy(g, &wid_spell_options_window);

    game_state_reset(g, "close spellbook");
  }
}

void wid_spell_options_mouse_over_begin(Gamep g, Widp w, int /*relx*/, int /*rely*/, int /*wheelx*/, int /*wheely*/)
{
  TRACE();

  for (auto &n : wid_spell_option) {
    if (n != nullptr) {
      if (n != wid_over) {
        wid_set_color(n, WID_COLOR_BG, GRAY10);
      }
    }
  }

  auto *v = game_levels_get(g);
  if (v == nullptr) [[unlikely]] {
    return;
  }

  auto *t = wid_get_thing_context(g, v, w, 0);
  if (t == nullptr) {
    return;
  }

  level_cursor_describe_clear(g, v);
  if (level_cursor_describe_add(g, v, t)) {
    game_request_to_remake_ui_set(g);
  }
}

void wid_spell_options_mouse_over_end(Gamep g, Widp w)
{
  TRACE();

  auto *v = game_levels_get(g);
  if (v == nullptr) [[unlikely]] {
    return;
  }

  auto *t = wid_get_thing_context(g, v, w, 0);
  if (t == nullptr) {
    return;
  }

  if (level_cursor_describe_remove(g, v, t)) {
    game_request_to_remake_ui_set(g);
  }
}

[[nodiscard]] static auto wid_spell_options_mouse_down(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();

  auto *v = game_levels_get(g);
  if (v == nullptr) [[unlikely]] {
    return false;
  }

  auto *t = wid_get_thing_context(g, v, w, 0);
  if (t == nullptr) {
    return false;
  }

  wid_spell_options_destroy(g);

  (void) game_event_cast_spell(g, wid_spell_option_chosen, wid_get_int_context(w));

  return true;
}

static void wid_spell_options_key_down_which_spell(Gamep g, Widp w, const struct SDL_Keysym *key, int index)
{
  TRACE();

  for (auto &n : wid_spell_option) {
    w = n;
    if (w != nullptr) {
      wid_set_color(w, WID_COLOR_BG, GRAY10);
    }
  }

  wid_unset_focus(g);

  auto options = tp_spell_options_get(thing_tp(wid_spell_option_chosen));
  if (index >= (int) options.size()) {
    return;
  }

  w = wid_spell_option[ index ];
  if (w != nullptr) {
    (void) wid_spell_options_mouse_down(g, w, -1, -1, 0);
    wid_set_color(w, WID_COLOR_BG, GREEN);
    wid_set_color(w, WID_COLOR_TEXT_FG, WHITE);
  }

  wid_spell_options_destroy(g);

  (void) game_event_cast_spell(g, wid_spell_option_chosen, index);
}

[[nodiscard]] static auto wid_spell_options_key_down(Gamep g, Widp w, const struct SDL_Keysym *key) -> bool
{
  TRACE();

  if (sdlk_eq(*key, game_key_console_get(g))) {
    (void) sound_play(g, "keypress");
    return false;
  }

  if (sdlk_eq(*key, game_key_spell1_get(g))) {
    wid_spell_options_key_down_which_spell(g, w, key, 0);
    return true;
  }

  if (sdlk_eq(*key, game_key_spell2_get(g))) {
    wid_spell_options_key_down_which_spell(g, w, key, 1);
    return true;
  }

  if (sdlk_eq(*key, game_key_spell3_get(g))) {
    wid_spell_options_key_down_which_spell(g, w, key, 2);
    return true;
  }

  if (sdlk_eq(*key, game_key_spell4_get(g))) {
    wid_spell_options_key_down_which_spell(g, w, key, 3);
    return true;
  }

  if (sdlk_eq(*key, game_key_spell5_get(g))) {
    wid_spell_options_key_down_which_spell(g, w, key, 4);
    return true;
  }

  if (sdlk_eq(*key, game_key_spell6_get(g))) {
    wid_spell_options_key_down_which_spell(g, w, key, 5);
    return true;
  }

  if (sdlk_eq(*key, game_key_spell7_get(g))) {
    wid_spell_options_key_down_which_spell(g, w, key, 6);
    return true;
  }

  if (sdlk_eq(*key, game_key_spell8_get(g))) {
    wid_spell_options_key_down_which_spell(g, w, key, 7);
    return true;
  }

  if (sdlk_eq(*key, game_key_spell9_get(g))) {
    wid_spell_options_key_down_which_spell(g, w, key, 8);
    return true;
  }

  if (sdlk_eq(*key, game_key_spell10_get(g))) {
    wid_spell_options_key_down_which_spell(g, w, key, 9);
    return true;
  }

  if (sdlk_eq(*key, game_key_spell11_get(g))) {
    wid_spell_options_key_down_which_spell(g, w, key, 10);
    return true;
  }

  if (sdlk_eq(*key, game_key_spell12_get(g))) {
    wid_spell_options_key_down_which_spell(g, w, key, 11);
    return true;
  }

  if (sdlk_eq(*key, game_key_spell13_get(g))) {
    wid_spell_options_key_down_which_spell(g, w, key, 12);
    return true;
  }

  if (sdlk_eq(*key, game_key_spell14_get(g))) {
    wid_spell_options_key_down_which_spell(g, w, key, 13);
    return true;
  }

  if (sdlk_eq(*key, game_key_spell15_get(g))) {
    wid_spell_options_key_down_which_spell(g, w, key, 14);
    return true;
  }

  if (sdlk_eq(*key, game_key_spell16_get(g))) {
    wid_spell_options_key_down_which_spell(g, w, key, 15);
    return true;
  }

  if (sdlk_eq(*key, game_key_spell17_get(g))) {
    wid_spell_options_key_down_which_spell(g, w, key, 16);
    return true;
  }

  if (sdlk_eq(*key, game_key_spell18_get(g))) {
    wid_spell_options_key_down_which_spell(g, w, key, 17);
    return true;
  }

  if (sdlk_eq(*key, game_key_spell19_get(g))) {
    wid_spell_options_key_down_which_spell(g, w, key, 18);
    return true;
  }

  if (sdlk_eq(*key, game_key_spell20_get(g))) {
    wid_spell_options_key_down_which_spell(g, w, key, 19);
    return true;
  }

  switch (key->mod) {
    case KMOD_SHIFT :
    case KMOD_LCTRL :
    case KMOD_RCTRL :
    default :
      switch (key->sym) {
        default :
        case SDLK_ESCAPE :
          (void) sound_play(g, "keypress");
          wid_spell_options_destroy(g);
          return true;
      }
  }

  //
  // So screenshots can work
  //
  return false;
}

[[nodiscard]] static auto wid_spell_options_back(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();
  wid_spell_options_destroy(g);
  return true;
}

void wid_spell_options_show(Gamep g, Levelsp v, Levelp l, Thingp player, Thingp spell)
{
  TRACE();

  wid_spell_option_chosen = spell;

  if (wid_spell_options_window != nullptr) {
    wid_spell_options_destroy(g);
  }

  if (thing_is_dead(player)) {
    return;
  }

  auto *ext_struct = thing_ext_struct(g, v, player);
  if (ext_struct == nullptr) {
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

    wid_spell_options_window = wid_new_window(g, "widget spellbook");
    wid_set_pos(wid_spell_options_window, tl, br);
    wid_set_style(wid_spell_options_window, UI_WID_STYLE_BUTTON_OUTLINE);
    wid_set_on_key_down(wid_spell_options_window, wid_spell_options_key_down);
    wid_set_text(wid_spell_options_window, "Spell options");
    wid_set_text_top(wid_spell_options_window, 1u);
    wid_raise(g, wid_spell_options_window);
  }

  {
    TRACE();
    auto        *w = wid_new_square_button(g, wid_spell_options_window, "text");
    spoint const tl(0, y_at);
    spoint const br(menu_width, y_at);
    wid_set_pos(w, tl, br);
    wid_set_text(w, UI_FMT_STR "Mouse select a spell option");
    wid_set_style(w, UI_WID_STYLE_BUTTON_OUTLINE);
    wid_set_shape_none(w);
    wid_set_text_centerx(w, 1u);
    y_at += 2;
  }

  memset(wid_shortcut, 0, sizeof(wid_shortcut));
  memset(wid_spell_option, 0, sizeof(wid_spell_option));

  auto options = tp_spell_options_get(thing_tp(spell));
  auto index   = 0;
  for (auto o : options) {
    auto option = o.second;

    //
    // Key shortcut
    //
    {
      TRACE();
      auto *w = wid_new_square_button(g, wid_spell_options_window, "Key");

      std::string s;
      switch (index) {
        case 0 :  s += ::to_string(game_key_spell1_get(g)); break;
        case 1 :  s += ::to_string(game_key_spell2_get(g)); break;
        case 2 :  s += ::to_string(game_key_spell3_get(g)); break;
        case 3 :  s += ::to_string(game_key_spell4_get(g)); break;
        case 4 :  s += ::to_string(game_key_spell5_get(g)); break;
        case 5 :  s += ::to_string(game_key_spell6_get(g)); break;
        case 6 :  s += ::to_string(game_key_spell7_get(g)); break;
        case 7 :  s += ::to_string(game_key_spell8_get(g)); break;
        case 8 :  s += ::to_string(game_key_spell9_get(g)); break;
        case 9 :  s += ::to_string(game_key_spell10_get(g)); break;
        case 10 : s += ::to_string(game_key_spell11_get(g)); break;
        case 11 : s += ::to_string(game_key_spell12_get(g)); break;
        case 12 : s += ::to_string(game_key_spell13_get(g)); break;
        case 13 : s += ::to_string(game_key_spell14_get(g)); break;
        case 14 : s += ::to_string(game_key_spell15_get(g)); break;
        case 15 : s += ::to_string(game_key_spell16_get(g)); break;
        case 16 : s += ::to_string(game_key_spell17_get(g)); break;
        case 17 : s += ::to_string(game_key_spell18_get(g)); break;
        case 18 : s += ::to_string(game_key_spell19_get(g)); break;
        case 19 : s += ::to_string(game_key_spell20_get(g)); break;
      }
      s += ')';

      spoint const tl(3, y_at);
      spoint const br(6, y_at + button_height);
      wid_set_text_lhs(w, 1u);

      wid_set_mode(w, WID_MODE_NORMAL);
      wid_set_color(w, WID_COLOR_TEXT_FG, GRAY50);
      wid_set_style(w, button_style);
      wid_set_pos(w, tl, br);
      wid_set_text(w, s);

      if (spell != nullptr) {
        wid_set_int_context(w, index);
        wid_set_thing_context(g, v, w, spell);
        wid_set_on_mouse_down(w, wid_spell_options_mouse_down);
      }

      wid_set_on_mouse_over_begin(w, wid_spell_options_mouse_over_begin);
      wid_set_on_mouse_over_end(w, wid_spell_options_mouse_over_end);

      wid_shortcut[ index ] = w;
    }

    //
    // Spell name
    //
    {
      std::string line;

      if (spell != nullptr) {
        line = option.name;
      } else {
        line = "-";
      }

      line += " ";

      {
        TRACE();
        auto *w = wid_new_bar_button(g, wid_spell_options_window, "Spell");

        spoint const tl(7, y_at);
        spoint const br(button_width, y_at + button_height);
        wid_set_text_lhs(w, 1u);
        wid_set_pos(w, tl, br);
        wid_set_text(w, capitalize_first(line));

        if (spell != nullptr) {
          wid_set_int_context(w, index);
          wid_set_thing_context(g, v, w, spell);
          wid_set_on_mouse_down(w, wid_spell_options_mouse_down);
        }

        wid_set_on_mouse_over_begin(w, wid_spell_options_mouse_over_begin);
        wid_set_on_mouse_over_end(w, wid_spell_options_mouse_over_end);

        wid_spell_option[ index ] = w;
      }
    }

    y_at += button_step;

    index++;
  }

  {
    TRACE();
    auto *w = wid_new_back_button(g, wid_spell_options_window, "BACK");

    spoint const tl((menu_width / 2) - 4, menu_height - 4);
    spoint const br((menu_width / 2) + 3, menu_height - 2);
    wid_set_on_mouse_down(w, wid_spell_options_back);
    wid_set_pos(w, tl, br);
  }

  wid_update(g, wid_spell_options_window);

  game_state_change(g, STATE_SPELLBOOK_MENU, "spellbook");
}
