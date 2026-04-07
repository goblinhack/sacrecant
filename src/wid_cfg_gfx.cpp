//
// Copyright goblinhack@gmail.com
//

#include "my_ascii.hpp"
#include "my_callstack.hpp"
#include "my_cpp_template.hpp"
#include "my_game.hpp"
#include "my_globals.hpp"
#include "my_main.hpp"
#include "my_sdl_event.hpp"
#include "my_sdl_proto.hpp"
#include "my_sound.hpp"
#include "my_wids.hpp"

#include <climits>
#include <utility>

static WidPopup *wid_cfg_gfx_window;

static SDL_DisplayMode pending_mode;
static bool            pending_mode_set;

static void wid_cfg_gfx_destroy()
{
  TRACE();
  delete wid_cfg_gfx_window;
  wid_cfg_gfx_window = nullptr;
}

[[nodiscard]] static auto wid_cfg_gfx_save(Gamep g, int y, uint32_t button) -> bool
{
  TRACE();

  con("Save config for gfx");
  game_save_config(g);

  wid_cfg_gfx_destroy();

  g_need_restart_with_given_arguments = "--restart-in-gfx-menu";
  wid_main_menu_destroy(g);

  return true;
}

[[nodiscard]] static auto wid_cfg_gfx_cancel(Gamep g, int y, uint32_t button) -> bool
{
  TRACE();
  con("Reload config");
  wid_cfg_gfx_destroy();
  wid_options_menu_select(g);
  return true;
}

[[nodiscard]] static auto wid_cfg_gfx_back(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();
  wid_cfg_gfx_destroy();
  wid_options_menu_select(g);
  return true;
}

[[nodiscard]] static auto wid_cfg_gfx_vsync_enable_toggle(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();
  con("Toggle vsync");
  if (game_gfx_vsync_enable_get(g)) {
    game_gfx_vsync_enable_unset(g);
  } else {
    game_gfx_vsync_enable_set(g);
  }
  config_gfx_vsync_update(g);

  (void) wid_cfg_gfx_save(g, 0, 0);
  return true;
}

[[nodiscard]] static auto wid_cfg_gfx_fullscreen_toggle(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();
  con("Toggle gfx fullscreen");
  if (game_gfx_fullscreen_get(g)) {
    game_gfx_fullscreen_unset(g);
  } else {
    game_gfx_fullscreen_set(g);
  }

  if (game_gfx_fullscreen_get(g)) {
    game_gfx_fullscreen_desktop_unset(g);
  }

  if (game_gfx_fullscreen_get(g)) {
    SDL_SetWindowFullscreen(sdl.window, SDL_WINDOW_FULLSCREEN);
  } else if (game_gfx_fullscreen_desktop_get(g)) {
    SDL_SetWindowFullscreen(sdl.window, SDL_WINDOW_FULLSCREEN_DESKTOP);
  } else {
    SDL_SetWindowFullscreen(sdl.window, 0);
  }

  (void) wid_cfg_gfx_save(g, 0, 0);
  return true;
}

[[nodiscard]] static auto wid_cfg_gfx_fullscreen_desktop_toggle(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();
  con("Toggle gfx fullscreen desktop");

  if (game_gfx_fullscreen_desktop_get(g)) {
    game_gfx_fullscreen_desktop_unset(g);
  } else {
    game_gfx_fullscreen_desktop_set(g);
  }

  if (game_gfx_fullscreen_desktop_get(g)) {
    game_gfx_fullscreen_unset(g);
  }

  if (game_gfx_fullscreen_get(g)) {
    SDL_SetWindowFullscreen(sdl.window, SDL_WINDOW_FULLSCREEN);
  } else if (game_gfx_fullscreen_desktop_get(g)) {
    SDL_SetWindowFullscreen(sdl.window, SDL_WINDOW_FULLSCREEN_DESKTOP);
  } else {
    SDL_SetWindowFullscreen(sdl.window, 0);
  }

  (void) wid_cfg_gfx_save(g, 0, 0);
  return true;
}

[[nodiscard]] static auto wid_cfg_gfx_borderless_toggle(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();
  con("Toggle gfx borderless");

  if (game_gfx_borderless_get(g)) {
    game_gfx_borderless_unset(g);
  } else {
    game_gfx_borderless_set(g);
  }

  SDL_SetWindowBordered(sdl.window, game_gfx_borderless_get(g) ? SDL_TRUE : SDL_FALSE);

  (void) wid_cfg_gfx_save(g, 0, 0);
  return true;
}

[[nodiscard]] static auto wid_cfg_other_fps_counter_toggle(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();
  con("Toggle fps counter");

  if (game_fps_counter_get(g)) {
    game_fps_counter_unset(g);

  } else {
    game_fps_counter_set(g);
  }

  (void) wid_cfg_gfx_save(g, 0, 0);
  return true;
}

[[nodiscard]] static auto wid_cfg_gfx_resolution_apply(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();

  SDL_DisplayMode const mode = pending_mode;
  pending_mode_set           = false;

  game_window_pix_width_set(g, mode.w);
  game_window_pix_height_set(g, mode.h);
  game_config_pix_width_set(g, mode.w);
  game_config_pix_height_set(g, mode.h);
  SDL_SetWindowSize(sdl.window, mode.w, mode.h);
  (void) wid_cfg_gfx_save(g, 0, 0);

  return true;
}

//
// The mac menu bar can mean the current requested resolution is slightly
// different from what we can request. Find the closest match.
//
static auto wid_cfg_gfx_find_closest_resolution(Gamep g) -> std::string
{
  TRACE();

  auto w           = game_window_pix_width_get(g);
  auto h           = game_window_pix_height_get(g);
  auto current_res = std::to_string(w) + "x" + std::to_string(h);
  log("find closest resolution to %s:", current_res.c_str());

  //
  // Find the closest mode
  //
  int         dist      = 0;
  int         best_dist = INT_MAX;
  std::string best_cand;

  auto n = SDL_GetNumDisplayModes(0);
  for (int i = 0; i < n; ++i) {
    SDL_DisplayMode mode;
    SDL_GetDisplayMode(0, i, &mode);

    auto cand_res = std::to_string(mode.w) + "x" + std::to_string(mode.h);
    dist          = abs(((w - mode.w) * 10000) + (h - mode.h));
    log(" - candidate %s dist %d", cand_res.c_str(), dist);

    if (dist < best_dist) {
      best_cand = std::to_string(mode.w) + "x" + std::to_string(mode.h);
      best_dist = dist;
    }
  }

  if (! best_cand.empty()) {
    log(" - best %s", best_cand.c_str());
    return best_cand;
  }

  //
  // Failsafe, just choose a matching mode
  //
  if (best_cand.empty()) {
    for (int i = 0; i < n; ++i) {
      SDL_DisplayMode mode;
      SDL_GetDisplayMode(0, i, &mode);
      best_cand = std::to_string(mode.w) + "x" + std::to_string(mode.h);
      break;
    }
  }

  log("chose fallback resolution of: %s", best_cand.c_str());

  return best_cand;
}

[[nodiscard]] static auto wid_cfg_gfx_resolution_incr(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();

  //
  // What is the starting resolution?
  //
  std::string current_res;

  if (pending_mode_set) {
    current_res = std::to_string(pending_mode.w) + "x" + std::to_string(pending_mode.h);
    con("Increment resolution (pending %s)", current_res.c_str());
  } else {
    current_res = wid_cfg_gfx_find_closest_resolution(g);
    con("Increment resolution (current %s)", current_res.c_str());
  }

  auto                                     n = SDL_GetNumDisplayModes(0);
  std::string                              chosen;
  std::vector< std::string >               cands;
  std::map< std::string, SDL_DisplayMode > modes;
  for (int i = 0; i < n; ++i) {
    SDL_DisplayMode mode;
    SDL_GetDisplayMode(0, i, &mode);
    auto cand = std::to_string(mode.w) + "x" + std::to_string(mode.h);
    push_back_if_unique(cands, cand);
    modes[ cand ] = mode;
  }
  for (int i = 0; std::cmp_less(i, cands.size()); ++i) {
    const auto &cand = cands[ i ];
    if (current_res == cand) {
      if (i > 0) {
        chosen = cands[ i - 1 ];
        log(" - candidate: %s (current)", cand.c_str());
      } else {
        log(" - candidate: %s (at max)", cand.c_str());
      }
    } else {
      log(" - candidate: %s", cand.c_str());
    }
  }
  if (! chosen.empty()) {
    SDL_DisplayMode const mode = modes[ chosen ];
    log(" - chosen: %s", chosen.c_str());
    pending_mode_set = true;
    pending_mode     = mode;
    con("Pending resolution %s", chosen.c_str());
    wid_cfg_gfx_select(g);
  } else {
    sound_play(g, "error");
    con("At maximum resolution (current %s)", current_res.c_str());
  }

  return true;
}

[[nodiscard]] static auto wid_cfg_gfx_resolution_decr(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();

  //
  // What is the starting resolution?
  //
  std::string current_res;

  if (pending_mode_set) {
    current_res = std::to_string(pending_mode.w) + "x" + std::to_string(pending_mode.h);
    con("Decrement resolution (pending %s)", current_res.c_str());
  } else {
    current_res = wid_cfg_gfx_find_closest_resolution(g);
    con("Decrement resolution (current %s)", current_res.c_str());
  }

  auto                                     n = SDL_GetNumDisplayModes(0);
  std::string                              chosen;
  std::vector< std::string >               cands;
  std::map< std::string, SDL_DisplayMode > modes;
  for (int i = 0; i < n; ++i) {
    SDL_DisplayMode mode;
    SDL_GetDisplayMode(0, i, &mode);
    auto cand = std::to_string(mode.w) + "x" + std::to_string(mode.h);
    push_back_if_unique(cands, cand);
    modes[ cand ] = mode;
  }
  for (int i = 0; std::cmp_less(i, cands.size()); ++i) {
    const auto &cand = cands[ i ];
    if (current_res == cand) {
      if (i < static_cast< int >(cands.size()) - 1) {
        chosen = cands[ i + 1 ];
        log(" - candidate: %s (current)", cand.c_str());
      } else {
        log(" - candidate: %s (at max)", cand.c_str());
      }
    } else {
      log(" - candidate: %s", cand.c_str());
    }
  }
  if (! chosen.empty()) {
    SDL_DisplayMode const mode = modes[ chosen ];
    log(" - chosen: %s", chosen.c_str());
    pending_mode_set = true;
    pending_mode     = mode;
    con("Pending resolution %s", chosen.c_str());
    wid_cfg_gfx_select(g);
  } else {
    sound_play(g, "error");
    con("At minimum resolution (current %s)", current_res.c_str());
  }

  return true;
}

[[nodiscard]] static auto wid_cfg_gfx_key_down(Gamep g, Widp w, const struct SDL_Keysym *key) -> bool
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
                (void) wid_cfg_gfx_cancel(g, 0, 0);
                return true;
              case 's' :
              case 'S' :
                sound_play(g, "keypress");
                (void) wid_cfg_gfx_save(g, 0, 0);
                return true;
              case 'b' :
              case 'B' :
              case SDLK_ESCAPE :
                sound_play(g, "keypress");
                (void) wid_cfg_gfx_cancel(g, 0, 0);
                return true;
            }
          }
      }
  }

  return false;
}

void wid_cfg_gfx_select(Gamep g)
{
  TRACE();
  if (wid_cfg_gfx_window != nullptr) {
    wid_cfg_gfx_destroy();
  }

  auto m = TERM_WIDTH / 2;

  spoint const outer_tl(m - 27, (TERM_HEIGHT / 2) - 15);
  spoint const outer_br(m + 27, (TERM_HEIGHT / 2) + 15);

  auto width = outer_br.x - outer_tl.x - 2;

  wid_cfg_gfx_window = new WidPopup(g, "Gfx select", outer_tl, outer_br, nullptr, "", false, false);
  {
    TRACE();
    Widp w = wid_cfg_gfx_window->wid_popup_container;
    wid_set_on_key_down(w, wid_cfg_gfx_key_down);
  }

  int y_at = 0;
  {
    TRACE();
    auto *p = wid_cfg_gfx_window->wid_text_area->wid_text_area;
    auto *w = wid_new_square_button(g, p, "configuration");

    spoint const tl(1, y_at);
    spoint const br(width, y_at + 2);
    wid_set_shape_none(w);
    wid_set_pos(w, tl, br);
    wid_set_text(w, "Configuration");
  }

  y_at = 3;
  {
    TRACE();
    auto *p = wid_cfg_gfx_window->wid_text_area->wid_text_area;
    auto *w = wid_new_back_button(g, p, "BACK");

    spoint const tl(1, y_at);
    spoint const br(6, y_at + 2);
    wid_set_on_mouse_up(w, wid_cfg_gfx_back);
    wid_set_pos(w, tl, br);
  }

  if (pending_mode_set) {
    TRACE();
    auto *p = wid_cfg_gfx_window->wid_text_area->wid_text_area;
    auto *w = wid_new_menu_button(g, p, "Apply");

    spoint const tl(10, y_at);
    spoint const br(16, y_at + 2);
    wid_set_on_mouse_up(w, wid_cfg_gfx_resolution_apply);
    wid_set_pos(w, tl, br);
    wid_set_text(w, "Apply");
  }

  /////////////////////////////////////////////////////////////////////////
  // resolution
  /////////////////////////////////////////////////////////////////////////
  y_at += 4;
  {
    TRACE();
    auto *p = wid_cfg_gfx_window->wid_text_area->wid_text_area;
    auto *w = wid_new_square_button(g, p, "Resolution");

    spoint const tl(1, y_at);
    spoint const br(width / 2, y_at);
    wid_set_shape_none(w);
    wid_set_pos(w, tl, br);
    wid_set_text_lhs(w, 1u);
    wid_set_text(w, "Resolution");
  }
  {
    TRACE();
    auto *p = wid_cfg_gfx_window->wid_text_area->wid_text_area;
    auto *w = wid_new_square_button(g, p, "Resolution value");

    spoint const tl(width / 2, y_at);
    spoint const br((width / 2) + 10, y_at + 2);
    wid_set_shape_none(w);
    wid_set_pos(w, tl, br);

    auto res = std::to_string(game_window_pix_width_get(g)) + "x" + std::to_string(game_window_pix_height_get(g));
    if (pending_mode_set) {
      res = std::to_string(pending_mode.w) + "x" + std::to_string(pending_mode.h);
    }

    wid_set_text(w, res);
    wid_set_text_lhs(w, 1u);
  }

  if (! game_gfx_fullscreen_desktop_get(g)) {
    {
      TRACE();
      auto *p = wid_cfg_gfx_window->wid_text_area->wid_text_area;
      auto *w = wid_new_menu_button(g, p, "Resolution value +");

      spoint const tl((width / 2) + 12, y_at);
      spoint const br((width / 2) + 14, y_at + 2);
      wid_set_pos(w, tl, br);
      wid_set_on_mouse_up(w, wid_cfg_gfx_resolution_incr);
      wid_set_text(w, "+");
    }
    {
      TRACE();
      auto *p = wid_cfg_gfx_window->wid_text_area->wid_text_area;
      auto *w = wid_new_menu_button(g, p, "Resolution value -");

      spoint const tl((width / 2) + 16, y_at);
      spoint const br((width / 2) + 18, y_at + 2);
      wid_set_pos(w, tl, br);
      wid_set_on_mouse_up(w, wid_cfg_gfx_resolution_decr);
      wid_set_text(w, "-");
    }
  }
  y_at += 2;

  /////////////////////////////////////////////////////////////////////////
  // Terminal
  /////////////////////////////////////////////////////////////////////////
  {
    TRACE();
    auto *p = wid_cfg_gfx_window->wid_text_area->wid_text_area;
    auto *w = wid_new_square_button(g, p, "Terminal");

    spoint const tl(1, y_at);
    spoint const br(width / 2, y_at + 2);
    wid_set_shape_none(w);
    wid_set_pos(w, tl, br);
    wid_set_text_lhs(w, 1u);
    wid_set_text(w, "Terminal");
  }
  {
    TRACE();
    auto *p = wid_cfg_gfx_window->wid_text_area->wid_text_area;
    auto *w = wid_new_square_button(g, p, "Terminal value");

    spoint const tl(width / 2, y_at);
    spoint const br((width / 2) + 12, y_at + 2);
    wid_set_shape_none(w);
    wid_set_pos(w, tl, br);

    auto res = std::to_string(TERM_WIDTH) + "x" + std::to_string(TERM_HEIGHT);
    wid_set_text(w, res);
    wid_set_text_lhs(w, 1u);
  }
  y_at += 1;

  /////////////////////////////////////////////////////////////////////////
  // ascii font
  /////////////////////////////////////////////////////////////////////////
  {
    TRACE();
    auto *p = wid_cfg_gfx_window->wid_text_area->wid_text_area;
    auto *w = wid_new_square_button(g, p, "Font size");

    spoint const tl(1, y_at);
    spoint const br(width / 2, y_at + 2);
    wid_set_shape_none(w);
    wid_set_pos(w, tl, br);
    wid_set_text_lhs(w, 1u);
    wid_set_text(w, "Font size");
  }
  {
    TRACE();
    auto *p = wid_cfg_gfx_window->wid_text_area->wid_text_area;
    auto *w = wid_new_square_button(g, p, "Font size value");

    spoint const tl(width / 2, y_at);
    spoint const br((width / 2) + 12, y_at + 2);
    wid_set_shape_none(w);
    wid_set_pos(w, tl, br);

    auto fw  = game_ascii_pix_width_get(g);
    auto fh  = game_ascii_pix_height_get(g);
    auto res = std::to_string(fw) + "x" + std::to_string(fh);
    wid_set_text(w, res);
    wid_set_text_lhs(w, 1u);
  }
  y_at += 2;

  /////////////////////////////////////////////////////////////////////////
  // Fullscreen desktop
  /////////////////////////////////////////////////////////////////////////
  y_at++;
  {
    TRACE();
    auto *p = wid_cfg_gfx_window->wid_text_area->wid_text_area;
    auto *w = wid_new_square_button(g, p, "Full desktop");

    spoint const tl(1, y_at);
    spoint const br(width / 2, y_at + 2);
    wid_set_shape_none(w);
    wid_set_pos(w, tl, br);
    wid_set_text_lhs(w, 1u);
    wid_set_text(w, "Full desktop");
  }
  {
    TRACE();
    auto *p = wid_cfg_gfx_window->wid_text_area->wid_text_area;
    auto *w = wid_new_menu_button(g, p, "Fullscreen value");

    spoint const tl(23, y_at);
    spoint const br(37, y_at + 2);
    wid_set_pos(w, tl, br);
    wid_set_on_mouse_up(w, wid_cfg_gfx_fullscreen_desktop_toggle);

    if (game_gfx_fullscreen_desktop_get(g)) {
      wid_set_text(w, "True");
    } else {
      wid_set_text(w, "False");
    }
  }

  if (game_gfx_fullscreen_desktop_get(g)) {
    y_at += 4;
    {
      TRACE();
      auto *p = wid_cfg_gfx_window->wid_text_area->wid_text_area;
      auto *w = wid_new_square_button(g, p, "Notice");

      spoint const tl(1, y_at);
      spoint const br(width - 2, y_at);
      wid_set_shape_none(w);
      wid_set_pos(w, tl, br);
      wid_set_text(w, "^^ Disable to change resolution ^^");
      wid_set_text_lhs(w, 1u);
    }
  }

  /////////////////////////////////////////////////////////////////////////
  // Fullscreen
  /////////////////////////////////////////////////////////////////////////
  y_at += 3;
  {
    TRACE();
    auto *p = wid_cfg_gfx_window->wid_text_area->wid_text_area;
    auto *w = wid_new_square_button(g, p, "Fullscreen");

    spoint const tl(1, y_at);
    spoint const br(width / 2, y_at + 2);
    wid_set_shape_none(w);
    wid_set_pos(w, tl, br);
    wid_set_text_lhs(w, 1u);
    wid_set_text(w, "Fullscreen");
  }
  {
    TRACE();
    auto *p = wid_cfg_gfx_window->wid_text_area->wid_text_area;
    auto *w = wid_new_menu_button(g, p, "Fullscreen value");

    spoint const tl(23, y_at);
    spoint const br(37, y_at + 2);
    wid_set_pos(w, tl, br);
    wid_set_on_mouse_up(w, wid_cfg_gfx_fullscreen_toggle);

    if (game_gfx_fullscreen_get(g)) {
      wid_set_text(w, "True");
    } else {
      wid_set_text(w, "False");
    }
  }

  /////////////////////////////////////////////////////////////////////////
  // Borderless
  /////////////////////////////////////////////////////////////////////////
  y_at += 3;
  {
    TRACE();
    auto *p = wid_cfg_gfx_window->wid_text_area->wid_text_area;
    auto *w = wid_new_square_button(g, p, "Borderless");

    spoint const tl(1, y_at);
    spoint const br(width / 2, y_at + 2);
    wid_set_shape_none(w);
    wid_set_pos(w, tl, br);
    wid_set_text_lhs(w, 1u);
    wid_set_text(w, "Borderless window");
  }
  {
    TRACE();
    auto *p = wid_cfg_gfx_window->wid_text_area->wid_text_area;
    auto *w = wid_new_menu_button(g, p, "Borderless");

    spoint const tl(23, y_at);
    spoint const br(37, y_at + 2);
    wid_set_pos(w, tl, br);
    wid_set_on_mouse_up(w, wid_cfg_gfx_borderless_toggle);

    if (game_gfx_borderless_get(g)) {
      wid_set_text(w, "True");
    } else {
      wid_set_text(w, "False");
    }
  }

  /////////////////////////////////////////////////////////////////////////
  // vsync
  /////////////////////////////////////////////////////////////////////////
  y_at += 3;
  {
    TRACE();
    auto *p = wid_cfg_gfx_window->wid_text_area->wid_text_area;
    auto *w = wid_new_square_button(g, p, "Vertical sync");

    spoint const tl(1, y_at);
    spoint const br(width / 2, y_at + 2);
    wid_set_shape_none(w);
    wid_set_pos(w, tl, br);
    wid_set_text_lhs(w, 1u);
    wid_set_text(w, "Vertical sync");
  }
  {
    TRACE();
    auto *p = wid_cfg_gfx_window->wid_text_area->wid_text_area;
    auto *w = wid_new_menu_button(g, p, "Vertical sync value");

    spoint const tl(23, y_at);
    spoint const br(37, y_at + 2);
    wid_set_pos(w, tl, br);
    wid_set_on_mouse_up(w, wid_cfg_gfx_vsync_enable_toggle);

    if (game_gfx_vsync_enable_get(g)) {
      wid_set_text(w, "True");
    } else {
      wid_set_text(w, "False");
    }
  }

  //////////////////////////////////////////////////////////////////////
  // fps
  //////////////////////////////////////////////////////////////////////
  y_at += 3;
  {
    TRACE();
    auto *p = wid_cfg_gfx_window->wid_text_area->wid_text_area;
    auto *w = wid_new_square_button(g, p, "FPS counter");

    spoint const tl(1, y_at);
    spoint const br(width / 2, y_at + 2);
    wid_set_shape_none(w);
    wid_set_pos(w, tl, br);
    wid_set_text_lhs(w, 1u);
    wid_set_text(w, "FPS counter");
  }
  {
    TRACE();
    auto *p = wid_cfg_gfx_window->wid_text_area->wid_text_area;
    auto *w = wid_new_menu_button(g, p, "FPS counter value");

    spoint const tl(23, y_at);
    spoint const br(37, y_at + 2);
    wid_set_pos(w, tl, br);
    wid_set_on_mouse_up(w, wid_cfg_other_fps_counter_toggle);

    if (game_fps_counter_get(g)) {
      wid_set_text(w, "True");
    } else {
      wid_set_text(w, "False");
    }
  }

  wid_update(g, wid_cfg_gfx_window->wid_text_area->wid_text_area);
}
