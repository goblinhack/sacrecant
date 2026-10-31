//
// Copyright goblinhack@gmail.com
//

#include "my_ascii.hpp"
#include "my_callstack.hpp"
#include "my_game.hpp"
#include "my_game_inlines.hpp"
#include "my_level.hpp"
#include "my_main.hpp"
#include "my_sdl_proto.hpp"
#include "my_sound.hpp"
#include "my_spoint.hpp"
#include "my_thing.hpp"
#include "my_tp.hpp"
#include "my_types.hpp"
#include "my_ui.hpp"
#include "my_wid.hpp"
#include "my_wid_popup.hpp"
#include "my_wids.hpp"
#include <SDL_keyboard.h>
#include <SDL_keycode.h>
#include <cmath>
#include <cstdint>

static WidPopup *wid_item_menu_window;

static bool wid_got_here_from_inventory;

//
// Used for keypresses, so we know which item to operate on
//
static Thingp g_item;

[[nodiscard]] static auto wid_item_menu_destroy() -> bool
{
  TRACE();

  delete wid_item_menu_window;
  wid_item_menu_window = nullptr;

  return true;
}

[[nodiscard]] static auto wid_item_menu_go_back(Gamep g) -> bool
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

  if (wid_got_here_from_inventory) {
    if (thing_inventory_is_empty(g, v, l, player)) {
      //
      // If empty, just go back to playing
      //
      game_state_reset(g, "close inventory");
    } else {
      //
      // Re-open the inventory
      //
      wid_inventory_show(g, v, l, player);
    }
  } else {
    //
    // Just go back to playing
    //
    game_state_change(g, STATE_PLAYING, "close inventory");
  }

  return true;
}

[[nodiscard]] static auto wid_item_menu_close(Gamep g) -> bool
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

  //
  // If empty, just go back to playing
  //
  game_state_change(g, STATE_PLAYING, "close inventory");

  return true;
}

[[nodiscard]] static auto wid_item_menu_drop(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
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

  auto *item = g_item;
  if (item == nullptr) {
    return false;
  }

  if (level_is_level_select(g, v, l)) {
    topcon(UI_WARN_FMT_STR "You can't drop things here!" UI_RESET_FMT);
    (void) sound_play(g, "error");
    return false;
  }

  ThingEvent e {
      .reason     = "user dropped",             //
      .event_type = THING_EVENT_USER_INITIATED, //
      .source     = player,                     //
  };

  if (! thing_drop(g, v, l, player, item, e)) {
    (void) sound_play(g, "error");
  }

  (void) wid_item_menu_destroy();

  game_state_reset(g, "dropped item");

  return true;
}

[[nodiscard]] static auto wid_item_menu_eat(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
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

  auto *item = g_item;
  if (item == nullptr) {
    return false;
  }

  if (level_is_level_select(g, v, l)) {
    topcon(UI_WARN_FMT_STR "You can't eat things here!" UI_RESET_FMT);
    (void) sound_play(g, "error");
    return false;
  }

  ThingEvent e {
      .reason     = "eater atr item",           //
      .event_type = THING_EVENT_USER_INITIATED, //
      .source     = player,                     //
  };

  if (! thing_eat(g, v, l, player, item, e)) {
    (void) sound_play(g, "error");
    return false;
  }

  (void) wid_item_menu_destroy();

  //
  // Just go back to playing
  //
  game_state_reset(g, "ate item");

  return true;
}

[[nodiscard]] static auto wid_item_menu_use(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
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

  auto *item = g_item;
  if (item == nullptr) {
    return false;
  }

  if (thing_is_edible(item)) {
    return wid_item_menu_eat(g, w, x, y, button);
  }

  if (level_is_level_select(g, v, l)) {
    topcon(UI_WARN_FMT_STR "You can't use things here!" UI_RESET_FMT);
    (void) sound_play(g, "error");
    return false;
  }

  ThingEvent e {
      .reason     = "user used item",           //
      .event_type = THING_EVENT_USER_INITIATED, //
      .source     = player,                     //
  };

  if (! thing_use(g, v, l, player, item, e)) {
    (void) sound_play(g, "error");
    return false;
  }

  (void) wid_item_menu_destroy();

  //
  // Just go back to playing
  //
  game_state_reset(g, "dropped item");

  return true;
}

[[nodiscard]] static auto wid_item_menu_throw(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
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

  auto *item = g_item;
  if (item == nullptr) {
    return false;
  }

  if (level_is_level_select(g, v, l)) {
    topcon(UI_WARN_FMT_STR "You can't throw things here!" UI_RESET_FMT);
    (void) sound_play(g, "error");
    return false;
  }

  (void) wid_item_menu_destroy();

  game_throw_id_set(g, item->id);
  game_state_change(g, STATE_CHOOSE_THROW_TARGET, "choose a target");

  return true;
}

[[nodiscard]] static auto wid_item_menu_wield(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
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

  auto *item = g_item;
  if (item == nullptr) {
    return false;
  }

  if (! thing_is_weapon(item) && ! thing_is_wearable(item)) {
    topcon("Item cannot be worn.");
    (void) sound_play(g, "error");
    return true;
  }

  if (thing_is_worn(item)) {
    topcon("Item is already worn.");
    (void) sound_play(g, "error");
    return true;
  }

  ThingEvent e {
      .reason     = "user worn",                //
      .event_type = THING_EVENT_USER_INITIATED, //
      .source     = player,                     //
  };

  if (! thing_wear_item(g, v, l, player, item, e)) {
    (void) sound_play(g, "error");
    return true;
  }

  (void) wid_item_menu_destroy();

  //
  // Just go back to playing
  //
  game_state_change(g, STATE_PLAYING, "close inventory");

  return true;
}

[[nodiscard]] static auto wid_item_menu_strip(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
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

  auto *item = g_item;
  if (item == nullptr) {
    return false;
  }

  if (! thing_is_weapon(item) && ! thing_is_wearable(item)) {
    topcon("Item cannot be worn.");
    (void) sound_play(g, "error");
    return true;
  }

  if (! thing_is_worn(item)) {
    topcon("Item is not worn.");
    (void) sound_play(g, "error");
    return true;
  }

  ThingEvent e {
      .reason     = "user striped",             //
      .event_type = THING_EVENT_USER_INITIATED, //
      .source     = player,                     //
  };

  if (! thing_strip_item(g, v, l, player, item, e)) {
    (void) sound_play(g, "error");
    return true;
  }

  (void) wid_item_menu_destroy();

  //
  // Just go back to playing
  //
  game_state_change(g, STATE_PLAYING, "close inventory");

  return true;
}

[[nodiscard]] static auto wid_item_menu_wield_toggle(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
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

  auto *item = g_item;
  if (item == nullptr) {
    return false;
  }

  if (thing_is_worn(item)) {
    return wid_item_menu_strip(g, w, x, y, button);
  }
  return wid_item_menu_wield(g, w, x, y, button);
}

[[nodiscard]] static auto wid_item_menu_back(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();
  (void) wid_item_menu_destroy();
  (void) wid_item_menu_go_back(g);
  return true;
}

[[nodiscard]] static auto wid_item_menu_close(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();
  (void) wid_item_menu_destroy();
  (void) wid_item_menu_close(g);
  return true;
}

[[nodiscard]] static auto wid_item_menu_key_down(Gamep g, Widp w, const struct SDL_Keysym *key) -> bool
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
              case 'u' :
              case 'U' :
                (void) sound_play(g, "keypress");
                (void) wid_item_menu_use(g, nullptr, 0, 0, 0);
                return true;
              case 'e' :
              case 'E' :
                (void) sound_play(g, "keypress");
                (void) wid_item_menu_eat(g, nullptr, 0, 0, 0);
                return true;
              case 't' :
              case 'T' :
                (void) sound_play(g, "keypress");
                (void) wid_item_menu_throw(g, nullptr, 0, 0, 0);
                return true;
              case 'd' :
              case 'D' :
                (void) sound_play(g, "keypress");
                (void) wid_item_menu_drop(g, nullptr, 0, 0, 0);
                return true;
              case 'w' :
              case 'W' :
                (void) sound_play(g, "keypress");
                (void) wid_item_menu_wield_toggle(g, nullptr, 0, 0, 0);
                return true;
              case 'b' :
              case 'B' :
              case SDLK_ESCAPE :
                (void) sound_play(g, "keypress");
                (void) wid_item_menu_back(g, nullptr, 0, 0, 0);
                return true;
            }
          }
      }
  }

  return true;
}

void wid_item_menu_select(Gamep g, Levelsp v, Thingp item, bool from_inventory)
{
  TRACE();
  log("item menu");

  wid_got_here_from_inventory = from_inventory;

  auto *l = game_level_get(g, v);
  if (l == nullptr) [[unlikely]] {
    return;
  }

  auto *player = thing_player(g);
  if (player == nullptr) [[unlikely]] {
    return;
  }

  g_item = item;

  if (wid_item_menu_window != nullptr) {
    (void) wid_item_menu_destroy();
  }

  auto box_height  = 2;
  auto box_step    = 3;
  auto menu_height = 0;

  //
  // Need to describe the item
  //
  level_cursor_describe_clear(g, v);
  (void) level_cursor_describe_add(g, v, item);
  (void) wid_rightbar_init(g);

  if (thing_is_usable(item)) {
    menu_height += box_step;
  }

  if (thing_is_edible(item)) {
    menu_height += box_step;
  }

  if (thing_is_weapon(item)) {
    menu_height += box_step;
  }

  if (thing_is_wearable(item)) {
    menu_height += box_step;
  }

  if (thing_is_throwable(item)) {
    menu_height += box_step;
  }

  if (thing_is_droppable(item)) {
    menu_height += box_step;
  }

  //
  // Back menu
  //
  menu_height += box_step;

  int const    menu_width = UI_WID_POPUP_WIDTH_NORMAL;
  spoint const outer_tl((TERM_WIDTH / 2) - (menu_width / 2), (TERM_HEIGHT / 2) - (menu_height / 2));
  spoint       outer_br;

  outer_br.x = outer_tl.x + menu_width + 1;
  outer_br.y = outer_tl.y + menu_height + 1;

  wid_item_menu_window = new WidPopup(g, "Item menu", outer_tl, outer_br, nullptr, "", false, false);

  auto button_width = outer_br.x - outer_tl.x - 2;

  {
    TRACE();
    Widp w = wid_item_menu_window->wid_popup_container;
    wid_set_on_key_down(w, wid_item_menu_key_down);
  }

  int y_at = 0;

  if (thing_is_usable(item)) {
    TRACE();
    auto *p = wid_item_menu_window->wid_text_area->wid_text_area;
    auto *w = wid_new_menu_button(g, p, "Use");

    spoint const tl(0, y_at);
    spoint const br(button_width, y_at + box_height);

    if (level_is_level_select(g, v, l)) {
      wid_gray_out_button(g, w);
    }

    wid_set_on_mouse_down(w, wid_item_menu_use);
    wid_set_pos(w, tl, br);
    if (thing_is_potion(item)) {
      wid_set_text(w, UI_HIGHLIGHT_FMT_STR "U" UI_FMT_STR "se (drink)");
    } else {
      wid_set_text(w, UI_HIGHLIGHT_FMT_STR "U" UI_FMT_STR "se (consume)");
    }
    y_at += box_step;
  }

  if (thing_is_edible(item)) {
    TRACE();
    auto *p = wid_item_menu_window->wid_text_area->wid_text_area;
    auto *w = wid_new_menu_button(g, p, "Use");

    spoint const tl(0, y_at);
    spoint const br(button_width, y_at + box_height);

    if (level_is_level_select(g, v, l)) {
      wid_gray_out_button(g, w);
    }

    wid_set_on_mouse_down(w, wid_item_menu_eat);
    wid_set_pos(w, tl, br);
    wid_set_text(w, UI_HIGHLIGHT_FMT_STR "U" UI_FMT_STR "se (eat)");
    y_at += box_step;
  }

  if (thing_is_weapon(item)) {
    if (thing_is_worn(item)) {
      TRACE();
      auto *p = wid_item_menu_window->wid_text_area->wid_text_area;
      auto *w = wid_new_menu_button(g, p, "Unwield");

      spoint const tl(0, y_at);
      spoint const br(button_width, y_at + box_height);
      wid_set_on_mouse_down(w, wid_item_menu_strip);
      wid_set_pos(w, tl, br);
      wid_set_text(w, "Unwield (" UI_HIGHLIGHT_FMT_STR "W" UI_FMT_STR ")");
      y_at += box_step;
    } else {
      TRACE();
      auto *p = wid_item_menu_window->wid_text_area->wid_text_area;
      auto *w = wid_new_menu_button(g, p, "Wield");

      spoint const tl(0, y_at);
      spoint const br(button_width, y_at + box_height);
      wid_set_on_mouse_down(w, wid_item_menu_wield);
      wid_set_pos(w, tl, br);
      wid_set_text(w, UI_HIGHLIGHT_FMT_STR "W" UI_FMT_STR "ield");
      y_at += box_step;
    }
  }

  if (thing_is_wearable(item)) {
    if (thing_is_worn(item)) {
      TRACE();
      auto *p = wid_item_menu_window->wid_text_area->wid_text_area;
      auto *w = wid_new_menu_button(g, p, "Strip");

      spoint const tl(0, y_at);
      spoint const br(button_width, y_at + box_height);
      wid_set_on_mouse_down(w, wid_item_menu_strip);
      wid_set_pos(w, tl, br);
      wid_set_text(w, "Strip (" UI_HIGHLIGHT_FMT_STR "W" UI_FMT_STR ")");
      y_at += box_step;
    } else {
      TRACE();
      auto *p = wid_item_menu_window->wid_text_area->wid_text_area;
      auto *w = wid_new_menu_button(g, p, "Wear");

      spoint const tl(0, y_at);
      spoint const br(button_width, y_at + box_height);
      wid_set_on_mouse_down(w, wid_item_menu_wield);
      wid_set_text(w, UI_HIGHLIGHT_FMT_STR "W" UI_FMT_STR "ear");
      wid_set_pos(w, tl, br);
      y_at += box_step;
    }
  }

  if (thing_is_throwable(item)) {
    TRACE();
    auto *p = wid_item_menu_window->wid_text_area->wid_text_area;
    auto *w = wid_new_menu_button(g, p, "Throw");

    spoint const tl(0, y_at);
    spoint const br(button_width, y_at + box_height);

    if (level_is_level_select(g, v, l)) {
      wid_gray_out_button(g, w);
    }

    wid_set_on_mouse_down(w, wid_item_menu_throw);
    wid_set_pos(w, tl, br);
    wid_set_text(w, UI_HIGHLIGHT_FMT_STR "T" UI_FMT_STR "hrow");
    y_at += box_step;
  }

  if (thing_is_droppable(item)) {
    TRACE();
    auto *p = wid_item_menu_window->wid_text_area->wid_text_area;
    auto *w = wid_new_menu_button(g, p, "Drop");

    spoint const tl(0, y_at);
    spoint const br(button_width, y_at + box_height);

    if (level_is_level_select(g, v, l)) {
      wid_gray_out_button(g, w);
    }

    wid_set_on_mouse_down(w, wid_item_menu_drop);
    wid_set_pos(w, tl, br);
    wid_set_text(w, UI_HIGHLIGHT_FMT_STR "D" UI_FMT_STR "rop");
    y_at += box_step;
  }

  if (from_inventory) {
    TRACE();
    auto *p = wid_item_menu_window->wid_text_area->wid_text_area;
    auto *w = wid_new_back_button(g, p, "BACK");

    spoint const tl(0, y_at);
    spoint const br(button_width, y_at + box_height);
    wid_set_on_mouse_down(w, wid_item_menu_back);
    wid_set_pos(w, tl, br);
  } else {
    TRACE();
    auto *p = wid_item_menu_window->wid_text_area->wid_text_area;
    auto *w = wid_new_close_button(g, p, "CLOSE");

    spoint const tl(0, y_at);
    spoint const br(button_width, y_at + box_height);
    wid_set_on_mouse_down(w, wid_item_menu_close);
    wid_set_pos(w, tl, br);
  }

  wid_update(g, wid_item_menu_window->wid_text_area->wid_text_area);

  game_state_change(g, STATE_ITEM_MENU, "close inventory");
}
