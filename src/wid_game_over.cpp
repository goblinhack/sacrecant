//
// Copyright goblinhack@gmail.com
//

#include <algorithm>

#include "my_ascii.hpp"
#include "my_callstack.hpp"
#include "my_game.hpp"
#include "my_gl.hpp"
#include "my_main.hpp"
#include "my_music.hpp"
#include "my_random.hpp"
#include "my_sdl_proto.hpp"
#include "my_sound.hpp"
#include "my_tile.hpp"
#include "my_wids.hpp"

static WidPopup *wid_game_over_window;

static const int flames_width  = 256;
static const int flames_height = 192;
static color     bg[ flames_width + 2 ][ flames_height + 2 ];
static color     bg2[ flames_width + 2 ][ flames_height + 2 ];

static const int max_skulls = 20;
static spoint    skull_at[ max_skulls ];
static bool      skull_set[ max_skulls ];
static int       skull_speed[ max_skulls ];
static float     skull_rotate[ max_skulls ];

static void wid_game_over_destroy(Gamep g)
{
  TRACE();
  delete wid_game_over_window;
  wid_game_over_window = nullptr;
}

[[nodiscard]] static auto wid_game_over_mouse_up(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();
  wid_game_over_destroy(g);

  TRACE();
  game_cleanup(g);

  TRACE();
  game_state_change(g, STATE_MAIN_MENU, "game over");

  if (g_opt_quick_start) {
    DIE_CLEAN("Quick quit");
  }
  return true;
}

static void game_display_flames_tiles(int w, int h)
{
  TRACE();

  float const bright = 2.5;

  static Tilep tile;
  if (tile == nullptr) {
    tile = tile_find_mand("solid");
  }

  int const tw = 1;
  int const th = 1;

  //
  // Account for rounding
  //
  int const offset = 0;

  blit_init();

  //
  // Display the flames
  //
  for (auto x = 0; x < w; x++) {
    for (auto y = 0; y < h; y++) {
      auto c = bg[ x ][ y ];

      int r = static_cast< int >(((static_cast< float >(static_cast< int >(c.r)))) * bright);
      int g = static_cast< int >(((static_cast< float >(static_cast< int >(c.g)))) * bright);
      int b = static_cast< int >(((static_cast< float >(static_cast< int >(c.b)))) * bright);

      r = std::min(r, 255);
      g = std::min(g, 255);
      b = std::min(b, 255);

      color cn(r, g, b, 255);

      //
      // Gradiated flames
      //
      if (r < 10) {
        cn = RED4;
        cn.r /= 6;
        cn.g /= 6;
        cn.b /= 6;
      } else if (r < 15) {
        if (OS_RANDOM_RANGE(0, 100) < 50) {
          cn = RED4;
          cn.r /= 3;
          cn.g /= 3;
          cn.b /= 3;
        } else {
          cn = RED4;
        }
      } else if (r < 50) {
        cn = RED4;
      } else if (r < 80) {
        cn = RED;
      } else if (r < 120) {
        cn = ORANGE;
      } else if (r < 140) {
        cn = YELLOW;
      } else if (r < 170) {
        cn = GRAY90;
      } else {
        cn = WHITE;
      }

      tile_blit(tile, spoint(tw * x, (th * y) + offset), spoint(tw * (x + 1), (th * (y + 1)) + offset + 1), cn);
    }

    blit_flush();
  }
}

static void game_display_skulls_do()
{
  TRACE();

  static Tilep tile;
  if (tile == nullptr) {
    tile = tile_find_mand("skull");
  }

  if (OS_RANDOM_RANGE(0, 100) < 50) {
    auto s = OS_RANDOM_RANGE(0, max_skulls);
    if (! skull_set[ s ]) {
      skull_at[ s ]     = spoint(OS_RANDOM_RANGE(0, 150), -10);
      skull_set[ s ]    = true;
      skull_speed[ s ]  = OS_RANDOM_RANGE(3, 10);
      skull_rotate[ s ] = 0.5; // (float) OS_RANDOM_RANGE(0, 2) / 2.0;
    }
  }

  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  for (auto i = 0; i < max_skulls; i++) {
    if (! skull_set[ i ]) {
      continue;
    }

    spoint *at = &skull_at[ i ];

    auto tl = *at;
    auto br = spoint(at->x + OUTLINE_TILE_WIDTH, at->y + OUTLINE_TILE_WIDTH);

    auto mid = (tl + br) / static_cast< short >(2);

    glPushMatrix();
    glTranslatef(mid.x, mid.y, 0);
    static float ang;
    ang += skull_rotate[ i ];
    glRotatef(ang, 0.0F, 0.0F, 1.0F);
    glTranslatef(-mid.x, -mid.y, 0);

    tile_blit(tile, tl, br, WHITE);

    blit_flush();
    glPopMatrix();

    at->y += skull_speed[ i ];

    if (at->y > 200) {
      skull_set[ i ] = false;
    }
  }
}

static void game_display_skulls(Gamep g)
{
  TRACE();

  auto w = flames_width;
  auto h = flames_height;

  gl_enter_2d_mode(g, w, h);
  {
    blit_fbo_bind(FBO_SKULLS);
    {
      gl_clear();

      game_display_skulls_do();
    }
    blit_fbo_unbind();
  }
  gl_leave_2d_mode(g);
}

static void game_display_flames_change(int w, int h)
{
  TRACE();

  //
  // Spawn new flames
  //
  int flames = 3;
  while ((flames--) != 0) {
    auto radius = 3;
    auto xr     = OS_RANDOM_RANGE(radius, 50);
    auto r      = OS_RANDOM_RANGE(0, radius);
    h           = 144;

    if (r < 5) {
      bg[ xr - 3 ][ h ] = GRAY10;
      bg[ xr - 2 ][ h ] = GRAY10;
      bg[ xr - 1 ][ h ] = WHITE;
      bg[ xr ][ h ]     = WHITE;
      bg[ xr + 1 ][ h ] = WHITE;
      bg[ xr + 2 ][ h ] = GRAY10;
      bg[ xr + 3 ][ h ] = GRAY10;
    } else if (r < 60) {
      bg[ xr - 3 ][ h ] = GRAY10;
      bg[ xr - 2 ][ h ] = GRAY10;
      bg[ xr - 1 ][ h ] = GRAY10;
      bg[ xr ][ h ]     = WHITE;
      bg[ xr + 1 ][ h ] = GRAY10;
      bg[ xr + 2 ][ h ] = GRAY10;
      bg[ xr + 3 ][ h ] = GRAY10;
    } else if (r < 90) {
      bg[ xr - 1 ][ h ] = COLOR_NONE;
      bg[ xr - 2 ][ h ] = COLOR_NONE;
      bg[ xr ][ h ]     = COLOR_NONE;
      bg[ xr + 1 ][ h ] = COLOR_NONE;
      bg[ xr + 2 ][ h ] = COLOR_NONE;
    }
  }

  flames = 3;
  while ((flames--) != 0) {
    auto radius = 3;
    auto xr     = OS_RANDOM_RANGE(90, 157);
    auto r      = OS_RANDOM_RANGE(0, radius);
    h           = 144;

    if (r < 5) {
      bg[ xr - 3 ][ h ] = GRAY10;
      bg[ xr - 2 ][ h ] = GRAY10;
      bg[ xr - 1 ][ h ] = WHITE;
      bg[ xr ][ h ]     = WHITE;
      bg[ xr + 1 ][ h ] = WHITE;
      bg[ xr + 2 ][ h ] = GRAY10;
      bg[ xr + 3 ][ h ] = GRAY10;
    } else if (r < 60) {
      bg[ xr - 3 ][ h ] = GRAY10;
      bg[ xr - 2 ][ h ] = GRAY10;
      bg[ xr - 1 ][ h ] = GRAY10;
      bg[ xr ][ h ]     = WHITE;
      bg[ xr + 1 ][ h ] = GRAY10;
      bg[ xr + 2 ][ h ] = GRAY10;
      bg[ xr + 3 ][ h ] = GRAY10;
    } else if (r < 90) {
      bg[ xr - 1 ][ h ] = COLOR_NONE;
      bg[ xr - 2 ][ h ] = COLOR_NONE;
      bg[ xr ][ h ]     = COLOR_NONE;
      bg[ xr + 1 ][ h ] = COLOR_NONE;
      bg[ xr + 2 ][ h ] = COLOR_NONE;
    }
  }

  //
  // Some random sparks
  //
  for (auto x = 0; x < w; x++) {
    if (OS_RANDOM_RANGE(0, 1000) < 995) {
      continue;
    }

    int sparks = 4;
    while ((sparks--) != 0) {
      for (auto y = h / 2; y < h - 1; y++) {
        auto c0 = bg[ x ][ y ];
        auto c1 = bg[ x ][ y + 1 ];

        if (c0.r == 0) {
          if (c1.r > 0) {
            bg[ x ][ y ].r = 255;
            bg[ x ][ y ].g = 200;
            bg[ x ][ y ].b = 200;
          }
        }
      }
    }
  }

  //
  // Scroll the flames at different speeds
  //
  for (auto x = 0; x < w; x++) {
    if (OS_RANDOM_RANGE(0, 100) < 99) {
      continue;
    }

    int scroll = 10;

    while ((scroll--) != 0) {
      for (auto y = 0; y < h - 1; y++) {
        auto c1      = bg[ x ][ y + 1 ];
        bg[ x ][ y ] = c1;
      }
    }
  }

  //
  // Scroll the flames at different speeds
  //
  for (auto x = 0; x < w; x++) {
    if (OS_RANDOM_RANGE(0, 100) < 95) {
      continue;
    }

    int scroll = 2;

    while ((scroll--) != 0) {
      for (auto y = 0; y < h - 1; y++) {
        auto c1      = bg[ x ][ y + 1 ];
        bg[ x ][ y ] = c1;
      }
    }
  }

  //
  // Scroll the flames at different speeds
  //
  for (auto x = 0; x < w; x++) {
    if (OS_RANDOM_RANGE(0, 100) < 50) {
      continue;
    }

    int scroll = 1;

    while ((scroll--) != 0) {
      for (auto y = 0; y < h - 1; y++) {
        auto c1      = bg[ x ][ y + 1 ];
        bg[ x ][ y ] = c1;
      }
    }
  }

  //
  // Blend the flames
  //
  static int const blend_max = 2;

  for (auto blend = 0; blend < blend_max; blend++) {
    for (auto x = 1; x < w; x++) {
      for (auto y = 1; y < h; y++) {
        auto c0 = bg[ x - 1 ][ y - 1 ];
        auto c1 = bg[ x ][ y - 1 ];
        auto c2 = bg[ x + 1 ][ y - 1 ];
        auto c3 = bg[ x - 1 ][ y ];
        auto c4 = bg[ x ][ y ];
        auto c5 = bg[ x + 1 ][ y ];
        auto c6 = bg[ x - 1 ][ y + 1 ];
        auto c7 = bg[ x ][ y + 1 ];
        auto c8 = bg[ x + 1 ][ y + 1 ];

        int const r = (static_cast< int >(c0.r) + static_cast< int >(c1.r) + static_cast< int >(c2.r) + static_cast< int >(c3.r)
                       + static_cast< int >(c4.r) + static_cast< int >(c5.r) + static_cast< int >(c6.r) + static_cast< int >(c7.r)
                       + static_cast< int >(c8.r))
                    / 10;
        int const g = (static_cast< int >(c0.g) + static_cast< int >(c1.g) + static_cast< int >(c2.g) + static_cast< int >(c3.g)
                       + static_cast< int >(c4.g) + static_cast< int >(c5.g) + static_cast< int >(c6.g) + static_cast< int >(c7.g)
                       + static_cast< int >(c8.g))
                    / 10;
        int const b = (static_cast< int >(c0.b) + static_cast< int >(c1.b) + static_cast< int >(c2.b) + static_cast< int >(c3.b)
                       + static_cast< int >(c4.b) + static_cast< int >(c5.b) + static_cast< int >(c6.b) + static_cast< int >(c7.b)
                       + static_cast< int >(c8.b))
                    / 10;
        int const a = 255;

        bg2[ x ][ y ] = color(r, g, b, a);
      }
    }
  }

  for (auto x = 1; x < w; x++) {
    for (auto y = 1; y < h; y++) {
      bg[ x ][ y ] = bg2[ x ][ y ];
    }
  }
}

static void game_display_flames(Gamep g)
{
  TRACE();

  auto w = flames_width;
  auto h = flames_height;

  gl_enter_2d_mode(g, w, h);
  {
    blit_fbo_bind(FBO_FLAMES);
    {
      gl_clear();

      game_display_flames_tiles(w, h);

      game_display_flames_change(w, h);
    }
    blit_fbo_unbind();
  }
  gl_leave_2d_mode(g);
}

static void game_display_game_over(Gamep g)
{
  TRACE();

  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  float       w = game_window_pix_width_get(g);
  float const h = game_window_pix_height_get(g);

  auto *tile_bg = tile_find_mand("game_over_bg");
  auto *tile_fg = tile_find_mand("game_over_fg");

  float const tw = tile_width(tile_bg);
  float const th = tile_height(tile_bg);

  w = (h * tw) / th;

  spoint tl(0, 0);
  spoint br(static_cast< int >(w), static_cast< int >(h));

  auto center = static_cast< int >((game_window_pix_width_get(g) - w) / 2);
  tl.x += center;
  br.x += center;

  blit_init();
  tile_blit(tile_bg, tl, br, WHITE);
  blit_flush();

  //
  // Paste this code prior to the blend in question
  //
  glBlendFunc(GL_ONE_MINUS_CONSTANT_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  if (0) {
    extern int         vals[];
    extern std::string vals_str[];
    extern int         g_blend_a;
    extern int         g_blend_b;
    con("glBlendFunc(%s, %s)", vals_str[ g_blend_a ].c_str(), vals_str[ g_blend_b ].c_str());
    glBlendFunc(vals[ g_blend_a ], vals[ g_blend_b ]);
  }
  glBlendFunc(GL_ONE, GL_ONE);

  blit_init();
  blit_fbo(g, FBO_FLAMES, tl.x, tl.y, br.x, br.y);
  blit_flush();

  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  blit_init();
  tile_blit(tile_fg, tl, br, WHITE);
  blit_flush();

  blit_init();
  blit_fbo(g, FBO_SKULLS, tl.x, tl.y, br.x, br.y);
  blit_flush();
}

static void wid_game_over_pre_tick(Gamep g, Widp w)
{
  TRACE();

  game_display_flames(g);
  game_display_skulls(g);
  SDL_Delay(20);
}

static void wid_game_over_tick(Gamep g, Widp w)
{
  TRACE();

  game_display_game_over(g);
}

void wid_game_over_select(Gamep g)
{
  TRACE();
  con("The end");

  if (wid_game_over_window != nullptr) {
    wid_game_over_destroy(g);
  }

  music_play(g, "game over");
  game_state_change(g, STATE_THE_END_MENU, "game over");

  int const    menu_height = 38;
  int const    menu_width  = UI_WID_POPUP_WIDTH_NORMAL * 2;
  spoint const outer_tl(TERM_WIDTH - menu_width - 16, 14);
  spoint const outer_br(TERM_WIDTH - 16, 10 + menu_height);
  wid_game_over_window = new WidPopup(g, "game over", outer_tl, outer_br, nullptr, "", false, false);

  const auto *name = game_player_name_get(g);

  wid_game_over_window->log_empty_line(g);
  wid_game_over_window->log(g, UI_GREEN_FMT_STR + std::string("Congratulations! ") + name + ".");
  wid_game_over_window->log_empty_line(g);
  wid_game_over_window->log_empty_line(g);
  wid_game_over_window->log(g,
                            UI_INFO1_FMT_STR // newline
                            "You are now lord of the underhell realms and all nearby facilities (*) Conditions apply.",
                            TEXT_FORMAT_LHS);
  wid_game_over_window->log_empty_line(g);

  wid_game_over_window->log(g,
                            UI_INFO2_FMT_STR // newline
                            "Lord Batcat III is no more and all is at peace or in pieces and the vantablack throne "
                            "is once again yours!",
                            TEXT_FORMAT_LHS);
  wid_game_over_window->log_empty_line(g);

  wid_game_over_window->log(
      g,
      UI_INFO3_FMT_STR // newline
      "As you sit in the dark and wonder at the many corpses lying in your wake and as you listen to the utter silence "
      "of the dungeon, you think to yourself... I'm going to need new employees.",
      TEXT_FORMAT_LHS);
  wid_game_over_window->log_empty_line(g);

  wid_game_over_window->log(g,
                            UI_INFO4_FMT_STR // newline
                            "If your dark heart is so inclined, and your dungeon has internet access, please let the goblin lurking at "
                            "goblinhack@gmail.com know that you finished this game.",
                            TEXT_FORMAT_LHS);
  wid_game_over_window->log_empty_line(g);

  wid_game_over_window->log(g, UI_INFO5_FMT_STR "And now, farewell...", TEXT_FORMAT_LHS);
  wid_game_over_window->log_empty_line(g);

  {
    TRACE();
    auto *p = wid_game_over_window->wid_text_area->wid_text_area;
    auto *w = wid_new_continue_button(g, p, "continue");

    spoint const tl((menu_width / 2) - 5, menu_height - 9);
    spoint const br((menu_width / 2) + 4, menu_height - 7);

    wid_set_on_mouse_up(w, wid_game_over_mouse_up);
    wid_set_pos(w, tl, br);
  }

  wid_set_on_pre_tick(wid_game_over_window->wid_text_area->wid_text_area, wid_game_over_pre_tick);
  wid_set_on_tick(wid_game_over_window->wid_text_area->wid_text_area, wid_game_over_tick);

  wid_update(g, wid_game_over_window->wid_text_area->wid_text_area);
}
