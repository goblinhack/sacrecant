//
// Copyright goblinhack@gmail.com
//

#include "my_ascii.hpp"
#include "my_callstack.hpp"
#include "my_game.hpp"
#include "my_game_inlines.hpp"
#include "my_globals.hpp"
#include "my_level.hpp"
#include "my_sdl_proto.hpp"
#include "my_sound.hpp"
#include "my_spoint.hpp"
#include "my_thing.hpp"
#include "my_thing_inlines.hpp"
#include "my_time.hpp"
#include "my_types.hpp"
#include "my_ui.hpp"
#include "my_wid.hpp"
#include "my_wid_popup.hpp"
#include "my_wids.hpp"

#include <cmath>
#include <cstdint>
#include <string>

static Widp      wid_actionbar {};
static Widp      wid_actionbar_container {};
static WidPopup *wid_over_save {};
static WidPopup *wid_over_load {};
static WidPopup *wid_over_wait {};
static WidPopup *wid_over_inventory {};
static WidPopup *wid_over_learn {};
static WidPopup *wid_over_cast {};
static WidPopup *wid_over_ascend {};
static WidPopup *wid_over_fire {};
static WidPopup *wid_over_descend {};
static WidPopup *wid_over_quit {};
static WidPopup *wid_over_zoom {};
static WidPopup *wid_over_help {};

static ts_t wid_last_key_press;
static ts_t wid_last_key_repeat;

//
// Used to detect changes in the menu
//
static std::string last_menu_string;

[[nodiscard]] static auto wid_actionbar_save(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();

  log("actionbar save");

  (void) sound_play(g, "click");
  return game_event_save(g);
}

static void wid_actionbar_save_over_begin(Gamep g, Widp w, int /*relx*/, int /*rely*/, int /*wheelx*/, int /*wheely*/)
{
  TRACE();

  int tlx = 0;
  int tly = 0;
  int brx = 0;
  int bry = 0;
  wid_get_abs_coords(w, &tlx, &tly, &brx, &bry);

  int const width  = 32;
  int const height = 8;

  tlx -= width / 2;
  brx += width / 2;
  tly -= height;

  bry += 2;
  tly += 2;

  spoint const tl(tlx, tly);
  spoint const br(brx, bry);

  wid_over_save = new WidPopup(g, "Save game", tl, br, nullptr, "", false, false);
  wid_over_save->log(g, UI_HIGHLIGHT_FMT_STR "Save game");
  wid_over_save->log_empty_line(g);
  wid_over_save->log(g, "Select this to save your progress");
  wid_over_save->compress(g);

  level_cursor_path_reset(g);
}

static void wid_actionbar_save_over_end(Gamep g, Widp w)
{
  TRACE();

  delete wid_over_save;
  wid_over_save = nullptr;
}

[[nodiscard]] static auto wid_actionbar_load(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  log("actionbar load");
  TRACE();
  (void) sound_play(g, "click");
  return game_event_load(g);
}

static void wid_actionbar_load_over_begin(Gamep g, Widp w, int /*relx*/, int /*rely*/, int /*wheelx*/, int /*wheely*/)
{
  TRACE();

  int tlx = 0;
  int tly = 0;
  int brx = 0;
  int bry = 0;
  wid_get_abs_coords(w, &tlx, &tly, &brx, &bry);

  int const width  = 32;
  int const height = 8;

  tlx -= width / 2;
  brx += width / 2;
  tly -= height;

  bry += 2;
  tly += 2;

  spoint const tl(tlx, tly);
  spoint const br(brx, bry);

  wid_over_load = new WidPopup(g, "Load game", tl, br, nullptr, "", false, false);
  wid_over_load->log(g, UI_HIGHLIGHT_FMT_STR "Load game");
  wid_over_load->log_empty_line(g);
  wid_over_load->log(g, "Select this to load an old game");
  wid_over_load->compress(g);

  level_cursor_path_reset(g);
}

static void wid_actionbar_load_over_end(Gamep g, Widp w)
{
  TRACE();

  delete wid_over_load;
  wid_over_load = nullptr;
}

[[nodiscard]] static auto wid_actionbar_wait(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  log("actionbar wait");
  TRACE();

  wid_last_key_press = game_time_ms_cached();

  return game_event_wait(g);
}

[[nodiscard]] static auto wid_actionbar_repeat_wait(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  log("actionbar wait");
  TRACE();

  if (! game_time_have_x_tenths_passed_since(1, wid_last_key_press)) {
    return true;
  }

  if (! game_time_have_x_tenths_passed_since(1, wid_last_key_repeat)) {
    return true;
  }

  wid_last_key_repeat = game_time_ms_cached();
  return game_event_wait(g);
}

static void wid_actionbar_wait_over_begin(Gamep g, Widp w, int /*relx*/, int /*rely*/, int /*wheelx*/, int /*wheely*/)
{
  TRACE();

  int tlx = 0;
  int tly = 0;
  int brx = 0;
  int bry = 0;
  wid_get_abs_coords(w, &tlx, &tly, &brx, &bry);

  int const width  = 32;
  int const height = 10;

  tlx -= width / 2;
  brx += width / 2;
  tly -= height;

  bry -= 1;
  tly += 1;

  spoint const tl(tlx, tly);
  spoint const br(brx, bry);

  wid_over_wait = new WidPopup(g, "Wait/rest", tl, br, nullptr, "", false, false);
  wid_over_wait->log(g, UI_HIGHLIGHT_FMT_STR "Wait/rest");
  wid_over_wait->log_empty_line(g);
  wid_over_wait->log(g, "Select this to waste one turn of your existence waiting.");
  wid_over_wait->log_empty_line(g);
  wid_over_wait->log(g, "Hold down to pass multiple turns.");
  wid_over_wait->compress(g);

  level_cursor_path_reset(g);
}

static void wid_actionbar_wait_over_end(Gamep g, Widp w)
{
  TRACE();

  delete wid_over_wait;
  wid_over_wait = nullptr;
}

[[nodiscard]] static auto wid_actionbar_inventory(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  log("actionbar inventory");
  TRACE();

  (void) sound_play(g, "click");
  return game_event_inventory(g);
}

static void wid_actionbar_inventory_over_begin(Gamep g, Widp w, int /*relx*/, int /*rely*/, int /*wheelx*/, int /*wheely*/)
{
  TRACE();

  int tlx = 0;
  int tly = 0;
  int brx = 0;
  int bry = 0;
  wid_get_abs_coords(w, &tlx, &tly, &brx, &bry);

  int const width  = 32;
  int const height = 8;

  tlx -= width / 2;
  brx += width / 2;
  tly -= height;

  bry -= 1;
  tly += 1;

  spoint const tl(tlx, tly);
  spoint const br(brx, bry);

  wid_over_inventory = new WidPopup(g, "Inventory", tl, br, nullptr, "", false, false);
  wid_over_inventory->log(g, UI_HIGHLIGHT_FMT_STR "Inventory");
  wid_over_inventory->log_empty_line(g);
  wid_over_inventory->log(g, "Select this to view your hard gotten loot.");
  wid_over_inventory->compress(g);

  level_cursor_path_reset(g);
}

static void wid_actionbar_inventory_over_end(Gamep g, Widp w)
{
  TRACE();

  delete wid_over_inventory;
  wid_over_inventory = nullptr;
}

[[nodiscard]] static auto wid_actionbar_learn(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  log("actionbar learn");
  TRACE();

  (void) sound_play(g, "click");
  return game_event_learn(g);
}

[[nodiscard]] static auto wid_actionbar_cast(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  log("actionbar cast");
  TRACE();

  (void) sound_play(g, "click");
  return game_event_cast(g);
}

static void wid_actionbar_learn_over_begin(Gamep g, Widp w, int /*relx*/, int /*rely*/, int /*wheelx*/, int /*wheely*/)
{
  TRACE();

  int tlx = 0;
  int tly = 0;
  int brx = 0;
  int bry = 0;
  wid_get_abs_coords(w, &tlx, &tly, &brx, &bry);

  int const width  = 32;
  int const height = 8;

  tlx -= width / 2;
  brx += width / 2;
  tly -= height;

  bry -= 1;
  tly += 1;

  spoint const tl(tlx, tly);
  spoint const br(brx, bry);

  wid_over_learn = new WidPopup(g, "Learn", tl, br, nullptr, "", false, false);
  wid_over_learn->log(g, UI_HIGHLIGHT_FMT_STR "Learn");
  wid_over_learn->log_empty_line(g);
  wid_over_learn->log(g, "Select this to learn spells and add them to your spellbook.");
  wid_over_learn->compress(g);

  level_cursor_path_reset(g);
}

static void wid_actionbar_learn_over_end(Gamep g, Widp w)
{
  TRACE();

  delete wid_over_learn;
  wid_over_learn = nullptr;
}

static void wid_actionbar_cast_over_begin(Gamep g, Widp w, int /*relx*/, int /*rely*/, int /*wheelx*/, int /*wheely*/)
{
  TRACE();

  int tlx = 0;
  int tly = 0;
  int brx = 0;
  int bry = 0;
  wid_get_abs_coords(w, &tlx, &tly, &brx, &bry);

  int const width  = 32;
  int const height = 8;

  tlx -= width / 2;
  brx += width / 2;
  tly -= height;

  bry -= 1;
  tly += 1;

  spoint const tl(tlx, tly);
  spoint const br(brx, bry);

  wid_over_cast = new WidPopup(g, "Cast", tl, br, nullptr, "", false, false);
  wid_over_cast->log(g, UI_HIGHLIGHT_FMT_STR "Cast");
  wid_over_cast->log_empty_line(g);
  wid_over_cast->log(g, "Select this to cast learned spells.");
  wid_over_cast->compress(g);

  level_cursor_path_reset(g);
}

static void wid_actionbar_cast_over_end(Gamep g, Widp w)
{
  TRACE();

  delete wid_over_cast;
  wid_over_cast = nullptr;
}

[[nodiscard]] static auto wid_actionbar_fire(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  log("actionbar fire");
  TRACE();

  wid_last_key_repeat = game_time_ms_cached();

  auto *v = game_levels_get(g);
  if (v == nullptr) [[unlikely]] {
    return false;
  }

  auto *l = game_level_get(g, v);
  if (l == nullptr) [[unlikely]] {
    return false;
  }

  (void) player_fire(g, v, l, 0, 0, nullptr);

  return true;
}

[[nodiscard]] static auto wid_actionbar_repeat_fire(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  log("actionbar fire");
  TRACE();

  if (! game_time_have_x_tenths_passed_since(1, wid_last_key_repeat)) {
    return true;
  }

  if (! game_time_have_x_tenths_passed_since(1, wid_last_key_repeat)) {
    return true;
  }

  wid_last_key_repeat = game_time_ms_cached();

  auto *v = game_levels_get(g);
  if (v == nullptr) [[unlikely]] {
    return false;
  }

  auto *l = game_level_get(g, v);
  if (l == nullptr) [[unlikely]] {
    return false;
  }

  (void) player_fire(g, v, l, 0, 0, nullptr);

  return true;
}

static void wid_actionbar_fire_over_begin(Gamep g, Widp w, int /*relx*/, int /*rely*/, int /*wheelx*/, int /*wheely*/)
{
  TRACE();

  int tlx = 0;
  int tly = 0;
  int brx = 0;
  int bry = 0;
  wid_get_abs_coords(w, &tlx, &tly, &brx, &bry);

  int const width  = 32;
  int const height = 14;

  tlx -= width / 2;
  brx += width / 2;
  tly -= height;

  bry -= 2;
  tly += 3;

  spoint const tl(tlx, tly);
  spoint const br(brx, bry);

  wid_over_fire = new WidPopup(g, "Fire", tl, br, nullptr, "", false, false);
  wid_over_fire->log(g, UI_HIGHLIGHT_FMT_STR "Fire");
  wid_over_fire->log_empty_line(g);
  wid_over_fire->log(g, "Hover the mouse over your chosen enemy and press this key to blast them to deserved oblivion.");
  wid_over_fire->log_empty_line(g);
  wid_over_fire->log(g, "For melee damage, walk into the monster to deliver a knuckle sandwich.");
  wid_over_fire->log_empty_line(g);
  wid_over_fire->compress(g);

  level_cursor_path_reset(g);
}

static void wid_actionbar_fire_over_end(Gamep g, Widp w)
{
  TRACE();

  delete wid_over_fire;
  wid_over_fire = nullptr;
}

[[nodiscard]] static auto wid_actionbar_ascend(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  log("actionbar ascend");
  TRACE();

  (void) sound_play(g, "click");
  return game_event_ascend(g);
}

static void wid_actionbar_ascend_over_begin(Gamep g, Widp w, int /*relx*/, int /*rely*/, int /*wheelx*/, int /*wheely*/)
{
  TRACE();

  int tlx = 0;
  int tly = 0;
  int brx = 0;
  int bry = 0;
  wid_get_abs_coords(w, &tlx, &tly, &brx, &bry);

  int const width  = 32;
  int const height = 7;

  tlx -= width / 2;
  brx += width / 2;
  tly -= height;

  bry -= 1;

  spoint const tl(tlx, tly);
  spoint const br(brx, bry);

  wid_over_ascend = new WidPopup(g, "Up", tl, br, nullptr, "", false, false);
  wid_over_ascend->log(g, UI_HIGHLIGHT_FMT_STR "Up");
  wid_over_ascend->log_empty_line(g);
  wid_over_ascend->log(g, "Select this return to level selection.");
  wid_over_ascend->compress(g);

  level_cursor_path_reset(g);
}

static void wid_actionbar_ascend_over_end(Gamep g, Widp w)
{
  TRACE();

  delete wid_over_ascend;
  wid_over_ascend = nullptr;
}

[[nodiscard]] static auto wid_actionbar_descend(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  log("actionbar descend");
  TRACE();

  (void) sound_play(g, "click");
  return game_event_descend(g);
}

static void wid_actionbar_descend_over_begin(Gamep g, Widp w, int /*relx*/, int /*rely*/, int /*wheelx*/, int /*wheely*/)
{
  TRACE();

  int tlx = 0;
  int tly = 0;
  int brx = 0;
  int bry = 0;
  wid_get_abs_coords(w, &tlx, &tly, &brx, &bry);

  int const width  = 32;
  int const height = 8;

  tlx -= width / 2;
  brx += width / 2;
  tly -= height;

  bry -= 1;
  tly += 1;

  spoint const tl(tlx, tly);
  spoint const br(brx, bry);

  wid_over_descend = new WidPopup(g, "Down", tl, br, nullptr, "", false, false);
  wid_over_descend->log(g, UI_HIGHLIGHT_FMT_STR "Down");
  wid_over_descend->log_empty_line(g);
  wid_over_descend->log(g, "Select this to descend further into the dungeon.");
  wid_over_descend->compress(g);

  level_cursor_path_reset(g);
}

static void wid_actionbar_descend_over_end(Gamep g, Widp w)
{
  TRACE();

  delete wid_over_descend;
  wid_over_descend = nullptr;
}

[[nodiscard]] static auto wid_actionbar_quit(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  log("actionbar quit");
  TRACE();

  (void) sound_play(g, "click");
  return game_event_quit(g);
}

static void wid_actionbar_quit_over_begin(Gamep g, Widp w, int /*relx*/, int /*rely*/, int /*wheelx*/, int /*wheely*/)
{
  TRACE();

  int tlx = 0;
  int tly = 0;
  int brx = 0;
  int bry = 0;
  wid_get_abs_coords(w, &tlx, &tly, &brx, &bry);

  int const width  = 32;
  int const height = 8;

  tlx -= width / 2;
  brx += width / 2;
  tly -= height;
  tly += 2;
  bry += 2;

  spoint const tl(tlx, tly);
  spoint const br(brx, bry);

  wid_over_quit = new WidPopup(g, "Quit", tl, br, nullptr, "", false, false);
  wid_over_quit->log(g, UI_HIGHLIGHT_FMT_STR "Quit game");
  wid_over_quit->log_empty_line(g);
  wid_over_quit->log(g, "Select this to abandon all hope.");
  wid_over_quit->compress(g);

  level_cursor_path_reset(g);
}

static void wid_actionbar_quit_over_end(Gamep g, Widp w)
{
  TRACE();

  delete wid_over_quit;
  wid_over_quit = nullptr;
}

[[nodiscard]] static auto wid_actionbar_zoom(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  log("actionbar zoom");
  TRACE();

  (void) sound_play(g, "click");
  game_map_zoom_toggle(g);

  return true;
}

static void wid_actionbar_zoom_over_begin(Gamep g, Widp w, int /*relx*/, int /*rely*/, int /*wheelx*/, int /*wheely*/)
{
  TRACE();

  int tlx = 0;
  int tly = 0;
  int brx = 0;
  int bry = 0;
  wid_get_abs_coords(w, &tlx, &tly, &brx, &bry);

  int const width  = 32;
  int const height = 8;

  tlx -= width / 2;
  brx += width / 2;
  tly -= height;
  tly += 2;
  bry += 2;

  spoint const tl(tlx, tly);
  spoint const br(brx, bry);

  wid_over_zoom = new WidPopup(g, "Zoom", tl, br, nullptr, "", false, false);
  wid_over_zoom->log(g, UI_HIGHLIGHT_FMT_STR "Zoom");
  wid_over_zoom->log_empty_line(g);
  wid_over_zoom->log(g, "Select this to toggle map zoom.");
  wid_over_zoom->compress(g);

  level_cursor_path_reset(g);
}

static void wid_actionbar_zoom_over_end(Gamep g, Widp w)
{
  TRACE();

  delete wid_over_zoom;
  wid_over_zoom = nullptr;
}

[[nodiscard]] static auto wid_actionbar_help(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  log("actionbar help");
  TRACE();

  (void) sound_play(g, "click");
  return game_event_help(g);
}

static void wid_actionbar_help_over_begin(Gamep g, Widp w, int /*relx*/, int /*rely*/, int /*wheelx*/, int /*wheely*/)
{
  TRACE();

  int tlx = 0;
  int tly = 0;
  int brx = 0;
  int bry = 0;
  wid_get_abs_coords(w, &tlx, &tly, &brx, &bry);

  int const width  = 32;
  int const height = 8;

  tlx -= width / 2;
  brx += width / 2;
  tly -= height;
  bry += 1;
  tly += 1;

  spoint const tl(tlx, tly);
  spoint const br(brx, bry);

  wid_over_help = new WidPopup(g, "Help", tl, br, nullptr, "", false, false);
  wid_over_help->log(g, UI_HIGHLIGHT_FMT_STR "Help");
  wid_over_help->log_empty_line(g);
  wid_over_help->log(g, "Select this to configure keyboard options.");
  wid_over_help->compress(g);

  level_cursor_path_reset(g);
}

static void wid_actionbar_help_over_end(Gamep g, Widp w)
{
  TRACE();

  delete wid_over_help;
  wid_over_help = nullptr;
}

static auto wid_actionbar_create_window(Gamep g) -> bool
{
  TRACE();

  auto *v = game_levels_get(g);
  if (v == nullptr) [[unlikely]] {
    return false;
  }

  auto *l = game_level_get(g, v);
  if (l == nullptr) [[unlikely]] {
    return false;
  }

  auto *player = thing_player(g);
  if (player == nullptr) [[unlikely]] {
    return false;
  }

  if (thing_is_dead(player)) {
    return false;
  }

  auto box_style           = UI_WID_STYLE_HORIZ_DARK;
  auto box_highlight_style = UI_WID_STYLE_HORIZ_LIGHT;

  int  option_width  = 13;
  bool opt_wait      = true;
  bool opt_inventory = true;
  bool opt_learn     = true;
  bool opt_cast      = true;
  bool opt_quit      = true;
  bool opt_zoom      = true;
  bool opt_help      = true;
  bool opt_load      = false;
  bool opt_save      = false;
  bool opt_descend   = level_is_exit(g, v, l, thing_at(g, v, l, player)) != nullptr;
  bool opt_ascend    = level_is_entrance(g, v, l, thing_at(g, v, l, player)) != nullptr;
  bool opt_fire      = true;

  if (g_opt_debug1 || level_is_level_select(g, v, l)) {
    if (v->tick != 0U) {
      opt_save = true;
      opt_load = true;
    }
  }

  if (level_is_level_select(g, v, l)) {
    opt_ascend    = false;
    opt_descend   = false;
    opt_fire      = false;
    opt_help      = false;
    opt_inventory = false;
    opt_learn     = false;
    opt_cast      = false;
    opt_quit      = false;
    opt_wait      = false;
    opt_zoom      = false;
  }

  //
  // Used to detect changes in the menu
  //
  std::string menu_string;
  if (opt_wait) {
    menu_string += "opt_wait";
  }
  if (opt_inventory) {
    menu_string += "opt_inventory";
  }
  if (opt_learn) {
    menu_string += "opt_learn";
  }
  if (opt_cast) {
    menu_string += "opt_cast";
  }
  if (opt_quit) {
    menu_string += "opt_quit";
  }
  if (opt_zoom) {
    menu_string += "opt_zoom";
  }
  if (opt_help) {
    menu_string += "opt_help";
  }
  if (opt_load) {
    menu_string += "opt_load";
  }
  if (opt_save) {
    menu_string += "opt_save";
  }
  if (opt_descend) {
    menu_string += "opt_descend";
  }
  if (opt_ascend) {
    menu_string += "opt_ascend";
  }
  if (opt_fire) {
    menu_string += "opt_fire";
  }

  if ((wid_actionbar != nullptr) && (menu_string == last_menu_string)) {
    wid_raise(g, wid_actionbar);
    if (wid_over_save != nullptr) {
      wid_raise(g, wid_over_save->wid_popup_container);
    }
    if (wid_over_load != nullptr) {
      wid_raise(g, wid_over_load->wid_popup_container);
    }
    if (wid_over_wait != nullptr) {
      wid_raise(g, wid_over_wait->wid_popup_container);
    }
    if (wid_over_inventory != nullptr) {
      wid_raise(g, wid_over_inventory->wid_popup_container);
    }
    if (wid_over_learn != nullptr) {
      wid_raise(g, wid_over_learn->wid_popup_container);
    }
    if (wid_over_cast != nullptr) {
      wid_raise(g, wid_over_cast->wid_popup_container);
    }
    if (wid_over_ascend != nullptr) {
      wid_raise(g, wid_over_ascend->wid_popup_container);
    }
    if (wid_over_fire != nullptr) {
      wid_raise(g, wid_over_fire->wid_popup_container);
    }
    if (wid_over_descend != nullptr) {
      wid_raise(g, wid_over_descend->wid_popup_container);
    }
    if (wid_over_quit != nullptr) {
      wid_raise(g, wid_over_quit->wid_popup_container);
    }
    if (wid_over_zoom != nullptr) {
      wid_raise(g, wid_over_zoom->wid_popup_container);
    }
    if (wid_over_help != nullptr) {
      wid_raise(g, wid_over_help->wid_popup_container);
    }
    return true;
  }

  wid_actionbar_fini(g);

  last_menu_string = menu_string;

  {
    auto tl = spoint(0, TERM_HEIGHT - 2);
    auto br = spoint(TERM_WIDTH, TERM_HEIGHT - 2);

    wid_actionbar = wid_new_window(g, "widget actionbar");
    wid_set_ignore_scroll_events(wid_actionbar);
    wid_set_pos(wid_actionbar, tl, br);
    wid_set_style(wid_actionbar, box_highlight_style);
    wid_set_shape_none(wid_actionbar);
    wid_lower(g, wid_actionbar);
  }

  {
    auto tl = spoint(0, 0);
    auto br = spoint(TERM_WIDTH, 0);

    wid_actionbar_container = wid_new_container(g, wid_actionbar, "widget actionbar container");
    wid_set_pos(wid_actionbar_container, tl, br);
    wid_set_shape_none(wid_actionbar_container);
  }

  int x_at = 0;

  if (opt_fire) {
    auto *w      = wid_new_square_button(g, wid_actionbar_container, "widget actionbar fire");
    option_width = (::to_string(game_key_fire_get(g)) + "Fire").size();
    auto tl      = spoint(x_at, 0);
    auto br      = spoint(x_at + option_width - 1, 0);
    wid_set_pos(w, tl, br);
    wid_set_on_mouse_down(w, wid_actionbar_fire);
    wid_set_on_mouse_held(w, wid_actionbar_repeat_fire);
    wid_set_on_mouse_over_begin(w, wid_actionbar_fire_over_begin);
    wid_set_on_mouse_over_end(w, wid_actionbar_fire_over_end);
    wid_set_text(w, UI_SHORTCUT_FMT_STR "" + ::to_string(game_key_fire_get(g)) + UI_HIGHLIGHT_FMT_STR "" + "Fire");
    wid_set_mode(w, WID_MODE_OVER);
    wid_set_style(w, box_highlight_style);
    wid_set_mode(w, WID_MODE_NORMAL);
    wid_set_style(w, box_style);
    x_at += option_width + 1;
  }

  if (opt_descend) {
    auto *w      = wid_new_square_button(g, wid_actionbar_container, "widget actionbar descend");
    auto  tl     = spoint(x_at, 0);
    option_width = (::to_string(game_key_descend_get(g)) + "Down").size();
    auto br      = spoint(x_at + option_width - 1, 0);
    wid_set_pos(w, tl, br);
    wid_set_on_mouse_down(w, wid_actionbar_descend);
    wid_set_on_mouse_over_begin(w, wid_actionbar_descend_over_begin);
    wid_set_on_mouse_over_end(w, wid_actionbar_descend_over_end);
    wid_set_text(w, UI_SHORTCUT_FMT_STR "" + ::to_string(game_key_descend_get(g)) + UI_HIGHLIGHT_FMT_STR "" + "Down");
    wid_set_mode(w, WID_MODE_OVER);
    wid_set_style(w, box_highlight_style);
    wid_set_mode(w, WID_MODE_NORMAL);
    wid_set_style(w, box_style);
    x_at += option_width + 1;
  }

  if (opt_ascend) {
    auto *w      = wid_new_square_button(g, wid_actionbar_container, "widget actionbar ascend");
    auto  tl     = spoint(x_at, 0);
    option_width = (::to_string(game_key_ascend_get(g)) + "Up").size();
    auto br      = spoint(x_at + option_width - 1, 0);
    wid_set_pos(w, tl, br);
    wid_set_on_mouse_down(w, wid_actionbar_ascend);
    wid_set_on_mouse_over_begin(w, wid_actionbar_ascend_over_begin);
    wid_set_on_mouse_over_end(w, wid_actionbar_ascend_over_end);
    wid_set_text(w, UI_SHORTCUT_FMT_STR "" + ::to_string(game_key_ascend_get(g)) + UI_HIGHLIGHT_FMT_STR "" + "Up");
    wid_set_mode(w, WID_MODE_OVER);
    wid_set_style(w, box_highlight_style);
    wid_set_mode(w, WID_MODE_NORMAL);
    wid_set_style(w, box_style);
    x_at += option_width + 1;
  }

  if (opt_wait) {
    auto *w      = wid_new_square_button(g, wid_actionbar_container, "widget actionbar wait");
    auto  tl     = spoint(x_at, 0);
    option_width = (::to_string(game_key_wait_get(g)) + "Wait").size();
    auto br      = spoint(x_at + option_width - 1, 0);
    wid_set_pos(w, tl, br);
    wid_set_on_mouse_down(w, wid_actionbar_wait);
    wid_set_on_mouse_held(w, wid_actionbar_repeat_wait);
    wid_set_on_mouse_over_begin(w, wid_actionbar_wait_over_begin);
    wid_set_on_mouse_over_end(w, wid_actionbar_wait_over_end);
    wid_set_text(w, UI_SHORTCUT_FMT_STR "" + ::to_string(game_key_wait_get(g)) + UI_HIGHLIGHT_FMT_STR "" + "Wait");
    wid_set_mode(w, WID_MODE_OVER);
    wid_set_style(w, box_highlight_style);
    wid_set_mode(w, WID_MODE_NORMAL);
    wid_set_style(w, box_style);
    x_at += option_width + 1;
  }

  if (opt_learn) {
    auto *w      = wid_new_square_button(g, wid_actionbar_container, "widget actionbar learn");
    auto  tl     = spoint(x_at, 0);
    option_width = (::to_string(game_key_learn_get(g)) + "Lrn").size();
    auto br      = spoint(x_at + option_width - 1, 0);
    wid_set_pos(w, tl, br);
    wid_set_on_mouse_down(w, wid_actionbar_learn);
    wid_set_on_mouse_over_begin(w, wid_actionbar_learn_over_begin);
    wid_set_on_mouse_over_end(w, wid_actionbar_learn_over_end);
    wid_set_text(w, UI_SHORTCUT_FMT_STR "" + ::to_string(game_key_learn_get(g)) + UI_HIGHLIGHT_FMT_STR "" + "Lrn");
    wid_set_mode(w, WID_MODE_OVER);
    wid_set_style(w, box_highlight_style);
    wid_set_mode(w, WID_MODE_NORMAL);
    wid_set_style(w, box_style);
    x_at += option_width + 1;
  }

  if (opt_cast) {
    auto *w      = wid_new_square_button(g, wid_actionbar_container, "widget actionbar cast");
    auto  tl     = spoint(x_at, 0);
    option_width = (::to_string(game_key_cast_get(g)) + "Cst").size();
    auto br      = spoint(x_at + option_width - 1, 0);
    wid_set_pos(w, tl, br);
    wid_set_on_mouse_down(w, wid_actionbar_cast);
    wid_set_on_mouse_over_begin(w, wid_actionbar_cast_over_begin);
    wid_set_on_mouse_over_end(w, wid_actionbar_cast_over_end);
    wid_set_text(w, UI_SHORTCUT_FMT_STR "" + ::to_string(game_key_cast_get(g)) + UI_HIGHLIGHT_FMT_STR "" + "Cst");
    wid_set_mode(w, WID_MODE_OVER);
    wid_set_style(w, box_highlight_style);
    wid_set_mode(w, WID_MODE_NORMAL);
    wid_set_style(w, box_style);
    x_at += option_width + 1;
  }

  if (opt_inventory) {
    auto *w      = wid_new_square_button(g, wid_actionbar_container, "widget actionbar inventory");
    auto  tl     = spoint(x_at, 0);
    option_width = (::to_string(game_key_inventory_get(g)) + "Inv").size();
    auto br      = spoint(x_at + option_width - 1, 0);
    wid_set_pos(w, tl, br);
    wid_set_on_mouse_down(w, wid_actionbar_inventory);
    wid_set_on_mouse_over_begin(w, wid_actionbar_inventory_over_begin);
    wid_set_on_mouse_over_end(w, wid_actionbar_inventory_over_end);
    wid_set_text(w, UI_SHORTCUT_FMT_STR "" + ::to_string(game_key_inventory_get(g)) + UI_HIGHLIGHT_FMT_STR "" + "Inv");
    wid_set_mode(w, WID_MODE_OVER);
    wid_set_style(w, box_highlight_style);
    wid_set_mode(w, WID_MODE_NORMAL);
    wid_set_style(w, box_style);
    x_at += option_width + 1;
  }

  if (opt_load) {
    auto *w      = wid_new_square_button(g, wid_actionbar_container, "widget actionbar load");
    auto  tl     = spoint(x_at, 0);
    option_width = (::to_string(game_key_load_get(g)) + "Load").size();
    auto br      = spoint(x_at + option_width - 1, 0);
    wid_set_pos(w, tl, br);
    wid_set_on_mouse_down(w, wid_actionbar_load);
    wid_set_on_mouse_over_begin(w, wid_actionbar_load_over_begin);
    wid_set_on_mouse_over_end(w, wid_actionbar_load_over_end);
    wid_set_text(w, UI_SHORTCUT_FMT_STR "" + ::to_string(game_key_load_get(g)) + UI_HIGHLIGHT_FMT_STR "" + "Load");
    wid_set_mode(w, WID_MODE_OVER);
    wid_set_style(w, box_highlight_style);
    wid_set_mode(w, WID_MODE_NORMAL);
    wid_set_style(w, box_style);
    x_at += option_width + 1;
  }

  if (opt_save) {
    auto *w      = wid_new_square_button(g, wid_actionbar_container, "widget actionbar save");
    auto  tl     = spoint(x_at, 0);
    option_width = (::to_string(game_key_save_get(g)) + "Save").size();
    auto br      = spoint(x_at + option_width - 1, 0);
    wid_set_pos(w, tl, br);
    wid_set_on_mouse_down(w, wid_actionbar_save);
    wid_set_on_mouse_over_begin(w, wid_actionbar_save_over_begin);
    wid_set_on_mouse_over_end(w, wid_actionbar_save_over_end);
    wid_set_text(w, UI_SHORTCUT_FMT_STR "" + ::to_string(game_key_save_get(g)) + UI_HIGHLIGHT_FMT_STR "" + "Save");
    wid_set_mode(w, WID_MODE_OVER);
    wid_set_style(w, box_highlight_style);
    wid_set_mode(w, WID_MODE_NORMAL);
    wid_set_style(w, box_style);
    x_at += option_width + 1;
  }

  if (opt_zoom) {
    auto *w      = wid_new_square_button(g, wid_actionbar_container, "widget actionbar zoom");
    auto  tl     = spoint(x_at, 0);
    option_width = (::to_string(game_key_zoom_get(g)) + "Zoom").size();
    auto br      = spoint(x_at + option_width - 1, 0);
    wid_set_pos(w, tl, br);
    wid_set_on_mouse_down(w, wid_actionbar_zoom);
    wid_set_on_mouse_over_begin(w, wid_actionbar_zoom_over_begin);
    wid_set_on_mouse_over_end(w, wid_actionbar_zoom_over_end);
    wid_set_text(w, UI_SHORTCUT_FMT_STR "" + ::to_string(game_key_zoom_get(g)) + UI_HIGHLIGHT_FMT_STR "" + "Zoom");
    wid_set_mode(w, WID_MODE_OVER);
    wid_set_style(w, box_highlight_style);
    wid_set_mode(w, WID_MODE_NORMAL);
    wid_set_style(w, box_style);
    x_at += option_width + 1;
  }

  if (opt_help) {
    auto *w      = wid_new_square_button(g, wid_actionbar_container, "widget actionbar help");
    auto  tl     = spoint(x_at, 0);
    option_width = (::to_string(game_key_help_get(g)) + "Help").size();
    auto br      = spoint(x_at + option_width - 1, 0);
    wid_set_pos(w, tl, br);
    wid_set_on_mouse_down(w, wid_actionbar_help);
    wid_set_on_mouse_over_begin(w, wid_actionbar_help_over_begin);
    wid_set_on_mouse_over_end(w, wid_actionbar_help_over_end);
    wid_set_text(w, UI_SHORTCUT_FMT_STR "" + ::to_string(game_key_help_get(g)) + UI_HIGHLIGHT_FMT_STR "" + "Help");
    wid_set_mode(w, WID_MODE_OVER);
    wid_set_style(w, box_highlight_style);
    wid_set_mode(w, WID_MODE_NORMAL);
    wid_set_style(w, box_style);
    x_at += option_width + 1;
  }

  if (opt_quit) {
    auto *w      = wid_new_square_button(g, wid_actionbar_container, "widget actionbar quit");
    auto  tl     = spoint(x_at, 0);
    option_width = (::to_string(game_key_quit_get(g)) + "Quit").size();
    auto br      = spoint(x_at + option_width - 1, 0);
    wid_set_pos(w, tl, br);
    wid_set_on_mouse_down(w, wid_actionbar_quit);
    wid_set_on_mouse_over_begin(w, wid_actionbar_quit_over_begin);
    wid_set_on_mouse_over_end(w, wid_actionbar_quit_over_end);
    wid_set_text(w, UI_SHORTCUT_FMT_STR "" + ::to_string(game_key_quit_get(g)) + UI_HIGHLIGHT_FMT_STR "" + "Quit");
    wid_set_mode(w, WID_MODE_OVER);
    wid_set_style(w, box_highlight_style);
    wid_set_mode(w, WID_MODE_NORMAL);
    wid_set_style(w, box_style);
    x_at += option_width + 1;
  }

  {
    wid_resize(g, wid_actionbar_container, x_at, -1);
    wid_move_to_pct_centered(g, wid_actionbar_container, 0.5, 0.0);
  }

  wid_update(g, wid_actionbar);

  return true;
}

void wid_actionbar_fini(Gamep g)
{
  TRACE();
  wid_destroy(g, &wid_actionbar);

  delete wid_over_save;
  wid_over_save = nullptr;
  delete wid_over_load;
  wid_over_load = nullptr;
  delete wid_over_wait;
  wid_over_wait = nullptr;
  delete wid_over_inventory;
  wid_over_inventory = nullptr;
  delete wid_over_learn;
  wid_over_learn = nullptr;
  delete wid_over_ascend;
  wid_over_ascend = nullptr;
  delete wid_over_fire;
  wid_over_fire = nullptr;
  delete wid_over_descend;
  wid_over_descend = nullptr;
  delete wid_over_quit;
  wid_over_quit = nullptr;
  delete wid_over_zoom;
  wid_over_zoom = nullptr;
  delete wid_over_help;
  wid_over_help    = nullptr;
  last_menu_string = "";
}

[[nodiscard]] auto wid_actionbar_init(Gamep g) -> bool
{
  TRACE();
  return wid_actionbar_create_window(g);
}
