//
// Copyright goblinhack@gmail.com
//

#include <SDL_mixer.h>

#include "my_ascii.hpp"
#include "my_callstack.hpp"
#include "my_game.hpp"
#include "my_main.hpp"
#include "my_music.hpp"
#include "my_sdl_proto.hpp"
#include "my_sound.hpp"
#include "my_wids.hpp"

static WidPopup *wid_cfg_sound_window;
static bool      local_g_config_changed;

static void wid_cfg_sound_destroy()
{
  TRACE();
  delete wid_cfg_sound_window;
  wid_cfg_sound_window   = nullptr;
  local_g_config_changed = false;
}

[[nodiscard]] static auto wid_cfg_sound_cancel(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();
  con("Reload config");
  if (local_g_config_changed) {
    local_g_config_changed = false;
    (void) game_load_config(g);
    sdl_config_update_all(g);
  }
  wid_cfg_sound_destroy();
  wid_options_menu_select(g);
  return true;
}

[[nodiscard]] static auto wid_cfg_sound_save(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();

  con("Save config for sound");
  game_save_config(g);

  wid_cfg_sound_destroy();
  wid_options_menu_select(g);
  return true;
}

[[nodiscard]] static auto wid_cfg_sound_back(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();
  wid_cfg_sound_destroy();
  wid_options_menu_select(g);
  return true;
}

[[nodiscard]] static auto wid_cfg_sound_effects_volume_incr(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();
  local_g_config_changed = true;
  con("Increment sound volume");
  auto vol = game_sound_volume_get(g);
  game_sound_volume_set(g, vol + 1);
  if (game_sound_volume_get(g) > MIX_MAX_VOLUME) {
    game_sound_volume_set(g, MIX_MAX_VOLUME);
  }
  wid_cfg_sound_select(g);
  return true;
}

[[nodiscard]] static auto wid_cfg_sound_effects_volume_decr(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();
  local_g_config_changed = true;
  con("Decrement sound volume");
  auto vol = game_sound_volume_get(g);
  game_sound_volume_set(g, vol - 1);
  if (game_sound_volume_get(g) < 0) {
    game_sound_volume_set(g, 0);
  }
  wid_cfg_sound_select(g);
  return true;
}

[[nodiscard]] static auto wid_cfg_sound_music_volume_incr(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();
  local_g_config_changed = true;
  con("Increment music volume");
  auto vol = game_music_volume_get(g);
  game_music_volume_set(g, vol + 1);
  if (game_music_volume_get(g) > MIX_MAX_VOLUME) {
    game_music_volume_set(g, MIX_MAX_VOLUME);
  }
  wid_cfg_sound_select(g);
  music_update_volume(g);
  return true;
}

[[nodiscard]] static auto wid_cfg_sound_music_volume_decr(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();
  local_g_config_changed = true;
  con("Decrement music volume");
  auto vol = game_music_volume_get(g);
  game_music_volume_set(g, vol - 1);
  if (game_music_volume_get(g) < 0) {
    game_music_volume_set(g, 0);
  }
  wid_cfg_sound_select(g);
  music_update_volume(g);
  return true;
}

[[nodiscard]] static auto wid_cfg_sound_key_down(Gamep g, Widp w, const struct SDL_Keysym *key) -> bool
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
              case 'c' :
              case 'C' :
                sound_play(g, "keypress");
                (void) wid_cfg_sound_cancel(g, nullptr, 0, 0, 0);
                return true;
              case 's' :
              case 'S' :
                sound_play(g, "keypress");
                (void) wid_cfg_sound_save(g, nullptr, 0, 0, 0);
                return true;
              case 'b' :
              case 'B' :
              case SDLK_ESCAPE :
                sound_play(g, "keypress");
                (void) wid_cfg_sound_cancel(g, nullptr, 0, 0, 0);
                return true;
            }
          }
      }
  }

  return false;
}

void wid_cfg_sound_select(Gamep g)
{
  TRACE();
  if (wid_cfg_sound_window != nullptr) {
    wid_cfg_sound_destroy();
  }

  auto m = TERM_WIDTH / 2;

  spoint const outer_tl(m - 16, (TERM_HEIGHT / 2) - 8);
  spoint const outer_br(m + 16, (TERM_HEIGHT / 2) + 8);

  auto width = outer_br.x - outer_tl.x - 2;

  wid_cfg_sound_window = new WidPopup(g, "Config sound select", outer_tl, outer_br, nullptr, "", false, false);

  {
    TRACE();
    Widp w = wid_cfg_sound_window->wid_popup_container;
    wid_set_on_key_down(w, wid_cfg_sound_key_down);
  }

  int y_at = 0;
  {
    TRACE();
    auto *p = wid_cfg_sound_window->wid_text_area->wid_text_area;
    auto *w = wid_new_square_button(g, p, "configuration");

    spoint const tl(1, y_at);
    spoint const br(width, y_at + 2);
    wid_set_shape_none(w);
    wid_set_pos(w, tl, br);
    wid_set_text(w, "Volume (max " + std::to_string(MIX_MAX_VOLUME) + ")");
  }

  y_at = 3;
  {
    TRACE();
    auto *p = wid_cfg_sound_window->wid_text_area->wid_text_area;
    auto *w = wid_new_back_button(g, p, "BACK");

    spoint const tl(1, y_at);
    spoint const br(6, y_at + 2);
    wid_set_on_mouse_up(w, wid_cfg_sound_back);
    wid_set_pos(w, tl, br);
  }
  {
    TRACE();
    auto *p = wid_cfg_sound_window->wid_text_area->wid_text_area;
    auto *w = wid_new_save_button(g, p, "Save");

    spoint const tl(width - 15, y_at);
    spoint const br(width - 10, y_at + 2);
    wid_set_on_mouse_up(w, wid_cfg_sound_save);
    wid_set_pos(w, tl, br);
    wid_set_text(w, UI_HIGHLIGHT_FMT_STR "S" UI_RESET_FMT "ave");
  }
  {
    TRACE();
    auto *p = wid_cfg_sound_window->wid_text_area->wid_text_area;
    auto *w = wid_new_cancel_button(g, p, "Cancel");

    spoint const tl(width - 8, y_at);
    spoint const br(width - 1, y_at + 2);
    wid_set_on_mouse_up(w, wid_cfg_sound_cancel);
    wid_set_pos(w, tl, br);
    wid_set_text(w, UI_HIGHLIGHT_FMT_STR "C" UI_RESET_FMT "ancel");
  }

  y_at += 4;
  {
    TRACE();
    auto *p = wid_cfg_sound_window->wid_text_area->wid_text_area;
    auto *w = wid_new_square_button(g, p, "Effects volume");

    spoint const tl(1, y_at);
    spoint const br(width / 2, y_at);
    wid_set_shape_none(w);
    wid_set_pos(w, tl, br);
    wid_set_text_lhs(w);
    wid_set_text(w, "Effects");
  }
  {
    TRACE();
    auto *p = wid_cfg_sound_window->wid_text_area->wid_text_area;
    auto *w = wid_new_button(g, p, "Effects volume value");

    spoint const tl(width / 2, y_at);
    spoint const br((width / 2) + 8, y_at);
    wid_set_pos(w, tl, br);
    wid_set_text(w, std::to_string(game_sound_volume_get(g)));
  }
  {
    TRACE();
    auto *p = wid_cfg_sound_window->wid_text_area->wid_text_area;
    auto *w = wid_new_menu_button(g, p, "Effects value +");

    spoint const tl((width / 2) + 9, y_at);
    spoint const br((width / 2) + 11, y_at + 2);
    wid_set_pos(w, tl, br);
    wid_set_on_mouse_down(w, wid_cfg_sound_effects_volume_incr);
    wid_set_on_mouse_held(w, wid_cfg_sound_effects_volume_incr);
    wid_set_text(w, "+");
  }
  {
    TRACE();
    auto *p = wid_cfg_sound_window->wid_text_area->wid_text_area;
    auto *w = wid_new_menu_button(g, p, "Effects value -");

    spoint const tl((width / 2) + 12, y_at);
    spoint const br((width / 2) + 14, y_at + 2);
    wid_set_pos(w, tl, br);
    wid_set_on_mouse_down(w, wid_cfg_sound_effects_volume_decr);
    wid_set_on_mouse_held(w, wid_cfg_sound_effects_volume_decr);
    wid_set_text(w, "-");
  }

  y_at += 4;
  {
    TRACE();
    auto *p = wid_cfg_sound_window->wid_text_area->wid_text_area;
    auto *w = wid_new_square_button(g, p, "Music volume");

    spoint const tl(1, y_at);
    spoint const br(width / 2, y_at);
    wid_set_shape_none(w);
    wid_set_pos(w, tl, br);
    wid_set_text_lhs(w);
    wid_set_text(w, "Music");
  }
  {
    TRACE();
    auto *p = wid_cfg_sound_window->wid_text_area->wid_text_area;
    auto *w = wid_new_button(g, p, "Music volume value");

    spoint const tl(width / 2, y_at);
    spoint const br((width / 2) + 8, y_at);
    wid_set_pos(w, tl, br);
    wid_set_text(w, std::to_string(game_music_volume_get(g)));
  }
  {
    TRACE();
    auto *p = wid_cfg_sound_window->wid_text_area->wid_text_area;
    auto *w = wid_new_menu_button(g, p, "Music value +");

    spoint const tl((width / 2) + 9, y_at);
    spoint const br((width / 2) + 11, y_at + 2);
    wid_set_pos(w, tl, br);
    wid_set_on_mouse_down(w, wid_cfg_sound_music_volume_incr);
    wid_set_on_mouse_held(w, wid_cfg_sound_music_volume_incr);
    wid_set_text(w, "+");
  }
  {
    TRACE();
    auto *p = wid_cfg_sound_window->wid_text_area->wid_text_area;
    auto *w = wid_new_menu_button(g, p, "Music value -");

    spoint const tl((width / 2) + 12, y_at);
    spoint const br((width / 2) + 14, y_at + 2);
    wid_set_pos(w, tl, br);
    wid_set_on_mouse_down(w, wid_cfg_sound_music_volume_decr);
    wid_set_on_mouse_held(w, wid_cfg_sound_music_volume_decr);
    wid_set_text(w, "-");
  }

  wid_update(g, wid_cfg_sound_window->wid_text_area->wid_text_area);
}
