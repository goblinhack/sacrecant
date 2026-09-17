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

static uint32_t wid_transition_window_created_ms;
static Widp     wid_transition_window;

static void wid_transition_destroy(Gamep g)
{
  if (wid_transition_window == nullptr) {
    return;
  }

  con("transition: destroy");
  TRACE();

  wid_destroy(g, &wid_transition_window);
  wid_transition_window_created_ms = 0;
}

static void wid_transition_display(Gamep g)
{
  TRACE();

  color const fg = WHITE;

  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  float       w = game_window_pix_width_get(g);
  float const h = game_window_pix_height_get(g);

  auto *tile = tile_find_mand("transition");

  //
  // Just sometimes, we have the silly spinning logo
  //
  if (OS_RANDOM_RANGE(0, 1000) > 0) {
    auto *player = thing_player(g);
    if (player != nullptr) {
      auto *tp = thing_tp(player);
      tile     = tp_tiles_get(tp, player->anim_type, player->anim_index);
    }
  }

  float const tw = tile_width(tile);
  float const th = tile_height(tile);

  w = (h * tw) / th;

  spoint tl(0, 0);
  spoint br(static_cast< int >(w), static_cast< int >(h));

  auto center = static_cast< int >((game_window_pix_width_get(g) - w) / 2);
  tl.x += center;
  br.x += center;

  const int   duration_total = 2000;
  float const zoomed         = (user_visible_time_ms() - wid_transition_window_created_ms) / static_cast< float >(duration_total);

  int const zoom_w_amount = static_cast< int >(zoomed * w);

  tl.x += zoom_w_amount;
  br.x -= zoom_w_amount;

  if (tl.x > br.x) {
    wid_transition_destroy(g);
    return;
  }

  float const new_width  = br.x - tl.x;
  float const new_height = (new_width / tw) * th;

  tl.y = static_cast< int >(h / 2) - static_cast< int >(new_height / 2);
  br.y = static_cast< int >(h / 2) + static_cast< int >(new_height / 2);

  if ((tl.x > br.x) || (tl.y > br.y)) {
    wid_transition_destroy(g);
    return;
  }

  spoint mid;
  mid.x = game_window_pix_width_get(g) / 2;
  mid.y = game_window_pix_height_get(g) / 2;

  blit_flush();
  glPushMatrix();
  glTranslatef(mid.x, mid.y, 0);
  float const ang = zoomed * 1000;
  glRotatef(ang, 0.0F, 0.0F, 1.0F);
  glTranslatef(-mid.x, -mid.y, 0);
  tile_blit(tile, tl, br, fg);
  blit_flush();
  glPopMatrix();

  if (game_time_have_x_ms_passed_since(duration_total, wid_transition_window_created_ms)) {
    wid_transition_destroy(g);
  }
}

static void wid_transition_tick(Gamep g, Widp w)
{
  TRACE();

  wid_transition_display(g);
}

void wid_transition_select(Gamep g)
{
  if (wid_transition_window_created_ms != 0U) {
    return;
  }

  wid_transition_window_created_ms = user_visible_time_ms();

  con("transition: select");
  TRACE();

  wid_transition_window = wid_new_window(g, "transition");

  spoint const tl(0, 0);
  spoint const br(TERM_WIDTH, TERM_HEIGHT);

  wid_set_shape_none(wid_transition_window);
  wid_set_pos(wid_transition_window, tl, br);
  wid_set_on_post_tick(wid_transition_window, wid_transition_tick);
}
