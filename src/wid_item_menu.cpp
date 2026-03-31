//
// Copyright goblinhack@gmail.com
//

#include "my_ascii.hpp"
#include "my_callstack.hpp"
#include "my_game.hpp"
#include "my_level.hpp"
#include "my_main.hpp"
#include "my_sdl_proto.hpp"
#include "my_sound.hpp"
#include "my_wids.hpp"

static WidPopup *wid_item_menu_window;

//
// Used for keypresses, so we know which item to operate on
//
static Thingp g_item;

[[nodiscard]] static auto wid_item_menu_destroy(Gamep g) -> bool
{
  TRACE();

  delete wid_item_menu_window;
  wid_item_menu_window = nullptr;

  auto *v = game_levels_get(g);
  if (v == nullptr) {
    return false;
  }

  auto *l = game_level_get(g, v);
  if (l == nullptr) {
    return false;
  }

  auto *player = thing_player(g);
  if (player == nullptr) [[unlikely]] {
    return false;
  }

  if (thing_inventory_is_empty(g, v, l, player)) {
    //
    // If empty, just go back to playing
    //
    game_state_change(g, STATE_PLAYING, "close inventory");
  } else {
    //
    // Re-open the inventory
    //
    wid_inventory_show(g, v, l, player);
  }

  return true;
}

[[nodiscard]] static auto wid_item_menu_drop(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();

  auto *v = game_levels_get(g);
  if (v == nullptr) {
    return false;
  }

  auto *l = game_level_get(g, v);
  if (l == nullptr) {
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
    topcon(UI_WARNING_FMT_STR "You can't drop things here!" UI_RESET_FMT);
    sound_play(g, "error");
    return false;
  }

  if (! thing_drop_item(g, v, l, item, player)) {
    sound_play(g, "error");
    return false;
  }

  return wid_item_menu_destroy(g);
}

[[nodiscard]] static auto wid_item_menu_equip(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();
  topcon("TODO equip");
  return wid_item_menu_destroy(g);
}

[[nodiscard]] static auto wid_item_menu_back(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();
  return wid_item_menu_destroy(g);
}

[[nodiscard]] static auto wid_item_menu_key_down(Gamep g, Widp w, const struct SDL_Keysym *key) -> bool
{
  TRACE();

  if (sdlk_eq(*key, game_key_console_get(g))) {
    sound_play(g, "keypress");
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
              case 'd' :
              case 'D' :
                sound_play(g, "keypress");
                (void) wid_item_menu_drop(g, nullptr, 0, 0, 0);
                return true;
              case 'e' :
              case 'E' :
                sound_play(g, "keypress");
                (void) wid_item_menu_equip(g, nullptr, 0, 0, 0);
                return true;
              case 'b' :
              case 'B' :
              case SDLK_ESCAPE :
                sound_play(g, "keypress");
                (void) wid_item_menu_back(g, nullptr, 0, 0, 0);
                return true;
            }
          }
      }
  }

  return false;
}

void wid_item_menu_select(Gamep g, Levelsp v, Thingp item)
{
  TRACE();
  log("item menu");

  auto *l = game_level_get(g, v);
  if (l == nullptr) {
    return;
  }

  auto *player = thing_player(g);
  if (player == nullptr) [[unlikely]] {
    return;
  }

  g_item = item;

  if (wid_item_menu_window != nullptr) {
    (void) wid_item_menu_destroy(g);
  }

  auto box_height  = 2;
  auto box_step    = 3;
  auto menu_height = 2;

  if (thing_is_item_droppable(item)) {
    menu_height += box_step;
  }

  if (thing_is_item_equipable(item)) {
    menu_height += box_step;
  }

  //
  // Back menu
  //
  menu_height += box_step;

  int const    menu_width = UI_WID_POPUP_WIDTH_NORMAL;
  spoint const outer_tl((TERM_WIDTH / 2) - (menu_width / 2), (TERM_HEIGHT / 2) - (menu_height / 2));
  spoint const outer_br((TERM_WIDTH / 2) + (menu_width / 2), (TERM_HEIGHT / 2) + (menu_height / 2) - 1);
  wid_item_menu_window = new WidPopup(g, "Item menu", outer_tl, outer_br, nullptr, "", false, false);

  auto button_width = outer_br.x - outer_tl.x - 2;

  {
    TRACE();
    Widp w = wid_item_menu_window->wid_popup_container;
    wid_set_on_key_down(w, wid_item_menu_key_down);
  }

  int y_at = 0;

  if (thing_is_item_droppable(item)) {
    TRACE();
    auto *p = wid_item_menu_window->wid_text_area->wid_text_area;
    auto *w = wid_new_menu_button(g, p, "Drop");

    spoint const tl(0, y_at);
    spoint const br(button_width, y_at + box_height);

    if (level_is_level_select(g, v, l)) {
      wid_gray_out_button(g, w);
    }

    wid_set_on_mouse_up(w, wid_item_menu_drop);
    wid_set_pos(w, tl, br);
    wid_set_text(w, UI_HIGHLIGHT_FMT_STR "D" UI_FMT_STR "rop");
    y_at += box_step;
  }

  if (thing_is_item_equipable(item)) {
    TRACE();
    auto *p = wid_item_menu_window->wid_text_area->wid_text_area;
    auto *w = wid_new_menu_button(g, p, "Equip");

    spoint const tl(0, y_at);
    spoint const br(button_width, y_at + box_height);
    wid_set_on_mouse_up(w, wid_item_menu_equip);
    wid_set_pos(w, tl, br);
    wid_set_text(w, UI_HIGHLIGHT_FMT_STR "E" UI_FMT_STR "quip");
    y_at += box_step;
  }

  {
    TRACE();
    auto *p = wid_item_menu_window->wid_text_area->wid_text_area;
    auto *w = wid_new_back_button(g, p, "BACK");

    spoint const tl(0, y_at);
    spoint const br(button_width, y_at + box_height);
    wid_set_on_mouse_up(w, wid_item_menu_back);
    wid_set_pos(w, tl, br);
  }

  wid_update(g, wid_item_menu_window->wid_text_area->wid_text_area);

  game_state_change(g, STATE_ITEM_MENU, "close inventory");
}
