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
#include "my_random.hpp"
#include "my_spoint.hpp"
#include "my_thing.hpp"
#include "my_thing_inlines.hpp"
#include "my_tile.hpp"
#include "my_time.hpp"
#include "my_tp.hpp"
#include "my_types.hpp"
#include "my_wid.hpp"
#include "my_wids.hpp"

#include <cstdint>

const int       duration_total = 8000;
static uint32_t wid_powerup_window_created_ms;
static Widp     wid_powerup_window;

static const int max_powerups = 200;
static spoint    powerup_at[ max_powerups ];
static bool      powerup_set[ max_powerups ];
static int       powerup_speed[ max_powerups ];

static void wid_powerup_destroy(Gamep g)
{
  if (wid_powerup_window == nullptr) {
    return;
  }

  con("Intro powerup menu: destroy");
  TRACE();

  wid_destroy(g, &wid_powerup_window);
  wid_powerup_window_created_ms = 0;
}

static void game_display_powerups_do(Gamep g)
{
  TRACE();

  static Tilep tile;
  if (tile == nullptr) {
    tile = tile_find_mand("powerup");
  }

  int w = game_window_pix_width_get(g);
  int h = game_window_pix_height_get(g);

  auto zoomed = (user_visible_time_ms() - wid_powerup_window_created_ms) / static_cast< float >(duration_total);

  if (zoomed < 0.2) {
    if (OS_RANDOM_RANGE(0, 100) < 80) {
      auto s = OS_RANDOM_RANGE(0, max_powerups);
      if (! powerup_set[ s ]) {
        powerup_at[ s ]    = spoint(OS_RANDOM_RANGE(0, w), h + 10);
        powerup_set[ s ]   = true;
        powerup_speed[ s ] = OS_RANDOM_RANGE(10, 60);
      }
    }
  }

  for (auto i = 0; i < max_powerups; i++) {
    if (! powerup_set[ i ]) {
      continue;
    }

    spoint *at = &powerup_at[ i ];

    auto tl = *at;
    auto br = spoint(at->x + w / 8, at->y + h / 6);

    blit_init();
    tile_blit(tile, tl, br, WHITE);
    blit_flush();

    at->y -= powerup_speed[ i ];

    if (at->y < -100) {
      powerup_set[ i ] = false;
    }
  }
}

static void wid_powerup_display(Gamep g)
{
  TRACE();

  auto *player = thing_player(g);
  if (player == nullptr) {
    return;
  }

  color const fg = WHITE;
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  float       w = game_window_pix_width_get(g);
  float const h = game_window_pix_height_get(g);

  static int frame;
  frame++;
  if (frame >= 16) {
    frame = 0;
  }

  auto tile = tile_find_mand("powerup" + std::string(".") + std::to_string(frame));

  float const tw = tile_width(tile);
  float const th = tile_height(tile);

  w = (h * tw) / th;

  spoint tl(0, 0);
  spoint br(static_cast< int >(w), static_cast< int >(h));

  auto center = static_cast< int >((game_window_pix_width_get(g) - w) / 2);
  tl.x += center;
  br.x += center;

  auto      zoomed        = (user_visible_time_ms() - wid_powerup_window_created_ms) / static_cast< float >(duration_total);
  int const zoom_w_amount = static_cast< int >(zoomed * w);

  tl.x += zoom_w_amount;
  br.x -= zoom_w_amount;

  if (tl.x > br.x) {
    wid_powerup_destroy(g);
    return;
  }

  float const new_width  = br.x - tl.x;
  float const new_height = (new_width / tw) * th;

  tl.y = static_cast< int >(h / 2) - static_cast< int >(new_height / 2);
  br.y = static_cast< int >(h / 2) + static_cast< int >(new_height / 2);

  if ((tl.x > br.x) || (tl.y > br.y)) {
    wid_powerup_destroy(g);
    return;
  }

  spoint mid;
  mid.x = game_window_pix_width_get(g) / 2;
  mid.y = game_window_pix_height_get(g) / 2;

  blit_flush();
  tile_blit(tile, tl, br, fg);
  blit_flush();

  game_display_powerups_do(g);

  auto *tp = thing_tp(player);
  tile     = tp_tiles_get(tp, player->anim_type, player->anim_index);

  if (tile) {
    blit_flush();

    auto   dw = (br.x - tl.x) / 4;
    auto   dh = (br.y - tl.y) / 4;
    spoint tl2(tl.x + dw, tl.y + dh);
    spoint br2(br.x - dw, br.y - dh);
    tile_blit(tile, tl2, br2, fg);
    blit_flush();
  }

  if (game_time_have_x_ms_passed_since(duration_total, wid_powerup_window_created_ms)) {
    wid_powerup_destroy(g);
  }
}

static void wid_powerup_tick(Gamep g, Widp w)
{
  TRACE();

  wid_powerup_display(g);
}

void wid_powerup_select(Gamep g)
{
  if (wid_powerup_window_created_ms != 0U) {
    return;
  }

  wid_powerup_window_created_ms = user_visible_time_ms();

  con("Intro powerup menu: select");
  TRACE();

  wid_powerup_window = wid_new_window(g, "powerup");

  spoint const tl(0, 0);
  spoint const br(TERM_WIDTH, TERM_HEIGHT);

  wid_set_shape_none(wid_powerup_window);
  wid_set_pos(wid_powerup_window, tl, br);
  wid_set_on_post_tick(wid_powerup_window, wid_powerup_tick);
}
