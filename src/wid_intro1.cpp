//
// Copyright goblinhack@gmail.com
//

#include "my_ascii.hpp"
#include "my_callstack.hpp"
#include "my_color.hpp"
#include "my_color_defs.hpp"
#include "my_game.hpp"
#include "my_gl.hpp"
#include "my_main.hpp" // NOLINT
#include "my_sdl_proto.hpp"
#include "my_sound.hpp"
#include "my_spoint.hpp"
#include "my_tile.hpp"
#include "my_types.hpp"
#include "my_wid.hpp"
#include "my_wids.hpp"

#include <SDL_keyboard.h>
#include <cstdint>

static Widp wid_intro1_window;

static void wid_intro1_destroy(Gamep g)
{
  if (wid_intro1_window == nullptr) {
    return;
  }

  con("Intro menu1: destroy");
  TRACE();

  wid_destroy(g, &wid_intro1_window);
}

[[nodiscard]] static auto wid_intro1_key_down(Gamep g, Widp w, const struct SDL_Keysym *key) -> bool
{
  TRACE();

  if (sdlk_eq(*key, game_key_console_get(g))) {
    (void) sound_play(g, "keypress");
    return false;
  }

  auto s = to_string(*key);

  if (s == "<Space>" || s == "<Return>" || s == "n" || s == "N") {
    TRACE();
    (void) sound_play(g, "keypress");
    wid_intro1_destroy(g);
    (void) sound_play(g, "select");
    wid_intro2_select(g);
    return true;
  }

  if (s == "<Escape>" || s == "b" || s == "B") {
    TRACE();
    wid_intro1_destroy(g);
    (void) sound_play(g, "select");
    wid_main_menu_select(g);
    return true;
  }

  return false;
}

[[nodiscard]] static auto wid_intro1_mouse_down(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  con("Intro menu1: next menu");
  TRACE();

  wid_intro1_destroy(g);
  (void) sound_play(g, "select");
  wid_intro2_select(g);

  return true;
}

static void game_display_intro(Gamep g)
{
  TRACE();

  color const fg = WHITE;

  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  float       w = game_window_pix_width_get(g);
  float const h = game_window_pix_height_get(g);

  auto       *tile = tile_find_mand("intro1");
  float const tw   = tile_width(tile);
  float const th   = tile_height(tile);

  w = (h * tw) / th;

  spoint tl(0, 0);
  spoint br(static_cast< int >(w), static_cast< int >(h));

  auto center = static_cast< int >((game_window_pix_width_get(g) - w) / 2);
  tl.x += center;
  br.x += center;

  blit_init();
  tile_blit(tile, tl, br, fg);
  blit_flush();
}

static void wid_intro1_tick(Gamep g, Widp w)
{
  TRACE();

  game_display_intro(g);
}

void wid_intro1_select(Gamep g)
{
  con("Intro menu1: select");
  TRACE();

  TRACE();
  wid_intro1_window = wid_new_window(g, "continue");

  spoint const tl(0, 0);
  spoint const br(TERM_WIDTH, TERM_HEIGHT);

  wid_set_on_mouse_down(wid_intro1_window, wid_intro1_mouse_down);
  wid_set_on_key_down(wid_intro1_window, wid_intro1_key_down);
  wid_set_shape_none(wid_intro1_window);
  wid_set_pos(wid_intro1_window, tl, br);
  wid_set_on_tick(wid_intro1_window, wid_intro1_tick);
}
