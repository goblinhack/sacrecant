//
// Copyright goblinhack@gmail.com
//

#include "my_ascii.hpp"
#include "my_callstack.hpp"
#include "my_game.hpp"
#include "my_main.hpp"
#include "my_sdl_proto.hpp"
#include "my_sound.hpp"
#include "my_wids.hpp"

static WidPopup *wid_intro_window;

static void wid_intro_destroy(Gamep g)
{
  TRACE();
  delete wid_intro_window;
  wid_intro_window = nullptr;
}

[[nodiscard]] static auto wid_intro_key_down(Gamep g, Widp w, const struct SDL_Keysym *key) -> bool
{
  TRACE();

  if (sdlk_eq(*key, game_key_console_get(g))) {
    sound_play(g, "keypress");
    return false;
  }

  auto s = to_string(*key);

  if (s == "<Return>" || s == "n" || s == "N") {
    TRACE();
    sound_play(g, "keypress");
    wid_intro_destroy(g);
    wid_new_game(g);
    return true;
  }

  if (s == "<Escape>" || s == "b" || s == "B") {
    TRACE();
    wid_intro_destroy(g);
    wid_main_menu_select(g);
    return true;
  }

  return false;
}

[[nodiscard]] static auto wid_intro_mouse_up(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();
  wid_intro_destroy(g);
  wid_new_game(g);
  return true;
}

static void game_display_intro(Gamep g)
{
  TRACE();

  color const fg = WHITE;

  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  float       w = game_window_pix_width_get(g);
  float const h = game_window_pix_height_get(g);

  auto       *tile = tile_find_mand("intro");
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

static void wid_intro_tick(Gamep g, Widp w)
{
  TRACE();

  game_display_intro(g);
}

void wid_intro_select(Gamep g)
{
  TRACE();
  con("Intro");

  if (wid_intro_window != nullptr) {
    wid_intro_destroy(g);
  }

  int const    menu_height = TERM_HEIGHT - 20;
  int const    menu_width  = UI_WID_POPUP_WIDTH_NORMAL * 2;
  spoint const outer_tl(TERM_WIDTH - menu_width - 30, 10);
  spoint const outer_br(TERM_WIDTH - 30, menu_height + 10);
  wid_intro_window = new WidPopup(g, "intro", outer_tl, outer_br, nullptr, "", false, false);

  wid_set_on_key_down(wid_intro_window->wid_popup_container, wid_intro_key_down);

  const auto *name = game_player_name_get(g);
  wid_intro_window->log(g, UI_GREEN_FMT_STR + std::string("Greetings, foolish ") + name + ".");
  wid_intro_window->log_empty_line(g);
  wid_intro_window->log_empty_line(g);
  wid_intro_window->log(
      g,
      UI_INFO1_FMT_STR
      "You were a once powerful dark wizard, cast out of the underhell realms by the slightly darker wizard, Lord Batcat III.",
      TEXT_FORMAT_LHS);
  wid_intro_window->log_empty_line(g);

  wid_intro_window->log(g,
                        UI_INFO2_FMT_STR "Your quest is to reclaim your vantablack throne by fighting through twenty five levels of doom.",
                        TEXT_FORMAT_LHS);
  wid_intro_window->log_empty_line(g);

  wid_intro_window->log(g,
                        UI_INFO3_FMT_STR
                        "Along the way you will need to make sacrifices... Fortunately, the dungeons are full of wizards captured by Lord "
                        "Batcat.",
                        TEXT_FORMAT_LHS);
  wid_intro_window->log_empty_line(g);

  wid_intro_window->log(
      g, UI_INFO4_FMT_STR "You will need to decide if such wizards are worthy of sacrifice to the very very dark gods you worship.",
      TEXT_FORMAT_LHS);
  wid_intro_window->log_empty_line(g);

  wid_intro_window->log(g, UI_INFO1_FMT_STR "If you decide to sacrifice:");
  wid_intro_window->log_empty_line(g);
  wid_intro_window->log(g, UI_GREEN_FMT_STR "- points mean prizes! Wizard souls can be converted to spell points", TEXT_FORMAT_LHS);
  wid_intro_window->log_empty_line(g);
  wid_intro_window->log(g, UI_RED_FMT_STR "- wizard might not be evil enough and incur the wrath of your god!", TEXT_FORMAT_LHS);
  wid_intro_window->log_empty_line(g);

  wid_intro_window->log(g, UI_INFO1_FMT_STR "If you decide to spare:");
  wid_intro_window->log_empty_line(g);
  wid_intro_window->log(g, UI_GREEN_FMT_STR "- the wizard might reward you...", TEXT_FORMAT_LHS);
  wid_intro_window->log_empty_line(g);
  wid_intro_window->log(g, UI_RED_FMT_STR "- the reward could be the magical equivalent of garbage!", TEXT_FORMAT_LHS);
  wid_intro_window->log_empty_line(g);

  wid_intro_window->log(g, UI_INFO1_FMT_STR "Enough talk... Begin your quest to defeat Lord Batcat!");
  wid_intro_window->log_empty_line(g);

  {
    TRACE();
    auto *p = wid_intro_window->wid_text_area->wid_text_area;
    auto *w = wid_new_continue_button(g, p, "continue");

    spoint const tl((menu_width / 2) - 5, menu_height - 6);
    spoint const br((menu_width / 2) + 4, menu_height - 4);

    wid_set_on_mouse_up(w, wid_intro_mouse_up);
    wid_set_pos(w, tl, br);
  }

  wid_set_on_tick(wid_intro_window->wid_text_area->wid_text_area, wid_intro_tick);

  wid_update(g, wid_intro_window->wid_text_area->wid_text_area);
}
