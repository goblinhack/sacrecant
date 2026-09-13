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
#include "my_time.hpp"
#include "my_types.hpp"
#include "my_wid.hpp"
#include "my_wids.hpp"

#include <SDL_keyboard.h>
#include <cmath>
#include <cstdint>

static int      wid_intro2_anim_vert_scroll_offset;
static uint32_t wid_intro2_anim_vert_scroll_ticker;
static uint32_t wid_intro2_anim_vert_scroll_start;

static Widp wid_intro2_window;

static void wid_intro2_destroy(Gamep g)
{
  if (wid_intro2_window == nullptr) {
    return;
  }

  con("Intro menu2: destroy");
  TRACE();

  wid_destroy(g, &wid_intro2_window);
}

[[nodiscard]] static auto wid_intro2_key_down(Gamep g, Widp w, const struct SDL_Keysym *key) -> bool
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
    wid_intro2_destroy(g);
    wid_player_select(g);
    return true;
  }

  if (s == "<Escape>" || s == "b" || s == "B") {
    TRACE();
    wid_intro2_destroy(g);
    wid_main_menu_select(g);
    return true;
  }

  return false;
}

[[nodiscard]] static auto wid_intro2_mouse_down(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  con("Intro menu2: next menu");
  TRACE();

  wid_intro2_destroy(g);
  wid_player_select(g);

  return true;
}

static void game_display_intro(Gamep g)
{
  TRACE();

  {
    color const fg = WHITE;

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    float       w = game_window_pix_width_get(g);
    float const h = game_window_pix_height_get(g);

    auto       *tile             = tile_find_mand("intro2");
    float const tw               = tile_width(tile);
    float const th               = tile_height(tile);
    const auto  th_single_screen = 384;

    w = ceilf((h * tw) / th_single_screen);

    spoint tl(0, 0);
    spoint br(static_cast< int >(w), static_cast< int >((th / th_single_screen) * h));

    auto center = static_cast< int >((game_window_pix_width_get(g) - w) / 2);
    tl.x += center;
    br.x += center;

    if (wid_intro2_anim_vert_scroll_start == 0U) {
      wid_intro2_anim_vert_scroll_start = game_time_ms();
    }

    if (wid_intro2_anim_vert_scroll_ticker == 0U) {
      wid_intro2_anim_vert_scroll_ticker = game_time_ms();
    }

    //
    // Small delay before scrolling
    //
    if (game_time_have_x_hundredths_passed_since(200, wid_intro2_anim_vert_scroll_start)) {
      if (game_time_have_x_hundredths_passed_since(1, wid_intro2_anim_vert_scroll_ticker)) {
        wid_intro2_anim_vert_scroll_offset += 2;
        wid_intro2_anim_vert_scroll_ticker = game_time_ms();
      }
    }

    tl.y -= wid_intro2_anim_vert_scroll_offset;
    br.y -= wid_intro2_anim_vert_scroll_offset;

    if (wid_intro2_anim_vert_scroll_offset > h * 3) {
      wid_intro2_anim_vert_scroll_offset = 0;
      wid_intro2_anim_vert_scroll_ticker = 0;
      wid_intro2_anim_vert_scroll_start  = 0;
    }

    blit_init();
    tile_blit(tile, tl, br, fg);
    blit_flush();
  }

  {
    color const fg = WHITE;

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    float       w = game_window_pix_width_get(g);
    float const h = game_window_pix_height_get(g);

    auto       *tile = tile_find_mand("intro3");
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
}

static void wid_intro2_tick(Gamep g, Widp w)
{
  TRACE();

  game_display_intro(g);
}

void wid_intro2_select(Gamep g)
{
  con("Intro menu2: select");
  TRACE_INDENT();

  wid_intro2_window = wid_new_window(g, "continue");

  wid_intro2_anim_vert_scroll_offset = 0;
  wid_intro2_anim_vert_scroll_ticker = 0;
  wid_intro2_anim_vert_scroll_start  = 0;

  spoint const tl(0, 0);
  spoint const br(TERM_WIDTH, TERM_HEIGHT);

  wid_set_on_mouse_down(wid_intro2_window, wid_intro2_mouse_down);
  wid_set_on_key_down(wid_intro2_window, wid_intro2_key_down);
  wid_set_shape_none(wid_intro2_window);
  wid_set_pos(wid_intro2_window, tl, br);
  wid_set_on_tick(wid_intro2_window, wid_intro2_tick);
}
