//
// Copyright goblinhack@gmail.com
//

#include "my_ascii.hpp"
#include "my_callstack.hpp"
#include "my_game.hpp"
#include "my_main.hpp"
#include "my_music.hpp"
#include "my_sdl_proto.hpp"
#include "my_sound.hpp"
#include "my_wids.hpp"

static WidPopup *wid_the_end_window;

static void wid_the_end_destroy(Gamep g)
{
  TRACE();
  delete wid_the_end_window;
  wid_the_end_window = nullptr;
}

[[nodiscard]] static auto wid_the_end_mouse_up(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();
  wid_the_end_destroy(g);

  TRACE();
  game_cleanup(g);

  TRACE();
  game_state_change(g, STATE_MAIN_MENU, "the end");

  if (g_opt_quick_start) {
    DIE_CLEAN("Quick quit");
  }
  return true;
}

static void game_display_the_end(Gamep g)
{
  TRACE();

  color const fg = WHITE;

  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  float       w = game_window_pix_width_get(g);
  float const h = game_window_pix_height_get(g);

  auto       *tile = tile_find_mand("the_end");
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

static void wid_the_end_tick(Gamep g, Widp w)
{
  TRACE();

  game_display_the_end(g);
}

void wid_the_end_select(Gamep g)
{
  TRACE();
  con("The end");

  if (wid_the_end_window != nullptr) {
    wid_the_end_destroy(g);
  }

  music_play(g, "the end");
  game_state_change(g, STATE_THE_END_MENU, "the end");

  int const    menu_height = 38;
  int const    menu_width  = UI_WID_POPUP_WIDTH_NORMAL * 2;
  spoint const outer_tl(TERM_WIDTH - menu_width - 16, 14);
  spoint const outer_br(TERM_WIDTH - 16, 10 + menu_height);
  wid_the_end_window = new WidPopup(g, "the end", outer_tl, outer_br, nullptr, "", false, false);

  const auto *name = game_player_name_get(g);

  wid_the_end_window->log_empty_line(g);
  wid_the_end_window->log(g, UI_GREEN_FMT_STR + std::string("Congratulations! ") + name + ".");
  wid_the_end_window->log_empty_line(g);
  wid_the_end_window->log_empty_line(g);
  wid_the_end_window->log(g,
                          UI_INFO1_FMT_STR // newline
                          "You are now lord of the underhell realms and all nearby facilities (*) Conditions apply.",
                          TEXT_FORMAT_LHS);
  wid_the_end_window->log_empty_line(g);

  wid_the_end_window->log(g,
                          UI_INFO2_FMT_STR // newline
                          "Lord Batcat III is no more and all is at peace or in pieces and the vantablack throne "
                          "is once again yours!",
                          TEXT_FORMAT_LHS);
  wid_the_end_window->log_empty_line(g);

  wid_the_end_window->log(g,
                          UI_INFO3_FMT_STR // newline
                          "As you sit in the dark and wonder at the many corpses lying in your wake and as you listen to the utter silence "
                          "of the dungeon, you think to yourself... I'm going to need new employees.",
                          TEXT_FORMAT_LHS);
  wid_the_end_window->log_empty_line(g);

  wid_the_end_window->log(g,
                          UI_INFO4_FMT_STR // newline
                          "If your dark heart is so inclined, and your dungeon has internet access, please let the goblin lurking at "
                          "goblinhack@gmail.com know that you finished this game.",
                          TEXT_FORMAT_LHS);
  wid_the_end_window->log_empty_line(g);

  wid_the_end_window->log(g, UI_INFO5_FMT_STR "And now, my lord, farewell...", TEXT_FORMAT_LHS);
  wid_the_end_window->log_empty_line(g);

  {
    TRACE();
    auto *p = wid_the_end_window->wid_text_area->wid_text_area;
    auto *w = wid_new_continue_button(g, p, "continue");

    spoint const tl((menu_width / 2) - 5, menu_height - 9);
    spoint const br((menu_width / 2) + 4, menu_height - 7);

    wid_set_on_mouse_up(w, wid_the_end_mouse_up);
    wid_set_pos(w, tl, br);
  }

  wid_set_on_tick(wid_the_end_window->wid_text_area->wid_text_area, wid_the_end_tick);

  wid_update(g, wid_the_end_window->wid_text_area->wid_text_area);
}
