//
// Copyright goblinhack@gmail.com
//

#include "my_ascii.hpp"
#include "my_callstack.hpp"
#include "my_game.hpp"
#include "my_globals.hpp"
#include "my_main.hpp"
#include "my_sdl_event.hpp"
#include "my_sdl_proto.hpp"
#include "my_sound.hpp"
#include "my_spoint.hpp"
#include "my_types.hpp"
#include "my_ui.hpp"
#include "my_wid.hpp"
#include "my_wid_popup.hpp"
#include "my_wids.hpp"
#include <SDL_keyboard.h>
#include <SDL_keycode.h>
#include <cstdint>
#include <string>

static int       last_vert_scroll_offset = -1;
static WidPopup *wid_cfg_help_window;
static bool      local_g_config_changed;

static void wid_cfg_check_for_conflicts(Gamep g, SDL_Keysym code)
{
  TRACE();
  SDL_Keysym const none = {};

  if (sdlk_eq(game_key_help_get(g), code)) {
    con("%%fg=orange$Conflicting keyboard mapping, disabling key for help." UI_RESET_FMT);
    game_key_help_set(g, none);
  }
  if (sdlk_eq(game_key_load_get(g), code)) {
    con("%%fg=orange$Conflicting keyboard mapping, disabling key for load." UI_RESET_FMT);
    game_key_load_set(g, none);
  }
  if (sdlk_eq(game_key_move_down_get(g), code)) {
    con("%%fg=orange$Conflicting keyboard mapping, disabling key for move down." UI_RESET_FMT);
    game_key_move_down_set(g, none);
  }
  if (sdlk_eq(game_key_move_left_get(g), code)) {
    con("%%fg=orange$Conflicting keyboard mapping, disabling key for move left." UI_RESET_FMT);
    game_key_move_left_set(g, none);
  }
  if (sdlk_eq(game_key_move_right_get(g), code)) {
    con("%%fg=orange$Conflicting keyboard mapping, disabling key for move right." UI_RESET_FMT);
    game_key_move_right_set(g, none);
  }
  if (sdlk_eq(game_key_move_up_get(g), code)) {
    con("%%fg=orange$Conflicting keyboard mapping, disabling key for move up." UI_RESET_FMT);
    game_key_move_up_set(g, none);
  }
  if (sdlk_eq(game_key_ascend_get(g), code)) {
    con("%%fg=orange$Conflicting keyboard mapping, disabling key for descend" UI_RESET_FMT);
    game_key_ascend_set(g, none);
  }
  if (sdlk_eq(game_key_ascend_get(g), code)) {
    con("%%fg=orange$Conflicting keyboard mapping, disabling key for ascend" UI_RESET_FMT);
    game_key_ascend_set(g, none);
  }
  if (sdlk_eq(game_key_zoom_get(g), code)) {
    con("%%fg=orange$Conflicting keyboard mapping, disabling key for zoom" UI_RESET_FMT);
    game_key_zoom_set(g, none);
  }
  if (sdlk_eq(game_key_quit_get(g), code)) {
    con("%%fg=orange$Conflicting keyboard mapping, disabling key for quit." UI_RESET_FMT);
    game_key_quit_set(g, none);
  }
  if (sdlk_eq(game_key_save_get(g), code)) {
    con("%%fg=orange$Conflicting keyboard mapping, disabling key for save." UI_RESET_FMT);
    game_key_save_set(g, none);
  }
  if (sdlk_eq(game_key_screenshot_get(g), code)) {
    con("%%fg=orange$Conflicting keyboard mapping, disabling key for screenshot." UI_RESET_FMT);
    game_key_screenshot_set(g, none);
  }
  if (sdlk_eq(game_key_inventory_get(g), code)) {
    con("%%fg=orange$Conflicting keyboard mapping, disabling key for inventory" UI_RESET_FMT);
    game_key_inventory_set(g, none);
  }
  if (sdlk_eq(game_key_learn_get(g), code)) {
    con("%%fg=orange$Conflicting keyboard mapping, disabling key for learn" UI_RESET_FMT);
    game_key_learn_set(g, none);
  }
  if (sdlk_eq(game_key_fire_get(g), code)) {
    con("%%fg=orange$Conflicting keyboard mapping, disabling key for firing" UI_RESET_FMT);
    game_key_fire_set(g, none);
  }
  if (sdlk_eq(game_key_jump_get(g), code)) {
    con("%%fg=orange$Conflicting keyboard mapping, disabling key for jump" UI_RESET_FMT);
    game_key_jump_set(g, none);
  }
  if (sdlk_eq(game_key_abort_get(g), code)) {
    con("%%fg=orange$Conflicting keyboard mapping, disabling key for abort" UI_RESET_FMT);
    game_key_abort_set(g, none);
  }
  if (sdlk_eq(game_key_throw_get(g), code)) {
    con("%%fg=orange$Conflicting keyboard mapping, disabling key for throwing" UI_RESET_FMT);
    game_key_throw_set(g, none);
  }
  if (sdlk_eq(game_key_console_get(g), code)) {
    con("%%fg=orange$Conflicting keyboard mapping, disabling key for console" UI_RESET_FMT);
    game_key_console_set(g, none);
  }
  if (sdlk_eq(game_key_unused1_get(g), code)) {
    con("%%fg=orange$Conflicting keyboard mapping, disabling key for unused1" UI_RESET_FMT);
    game_key_unused1_set(g, none);
  }
  if (sdlk_eq(game_key_unused2_get(g), code)) {
    con("%%fg=orange$Conflicting keyboard mapping, disabling key for unused2" UI_RESET_FMT);
    game_key_unused2_set(g, none);
  }
  if (sdlk_eq(game_key_unused3_get(g), code)) {
    con("%%fg=orange$Conflicting keyboard mapping, disabling key for unused3" UI_RESET_FMT);
    game_key_unused3_set(g, none);
  }
  if (sdlk_eq(game_key_unused4_get(g), code)) {
    con("%%fg=orange$Conflicting keyboard mapping, disabling key for unused4" UI_RESET_FMT);
    game_key_unused4_set(g, none);
  }
  if (sdlk_eq(game_key_unused5_get(g), code)) {
    con("%%fg=orange$Conflicting keyboard mapping, disabling key for unused5" UI_RESET_FMT);
    game_key_unused5_set(g, none);
  }
  if (sdlk_eq(game_key_unused6_get(g), code)) {
    con("%%fg=orange$Conflicting keyboard mapping, disabling key for unused6" UI_RESET_FMT);
    game_key_unused6_set(g, none);
  }
  if (sdlk_eq(game_key_unused7_get(g), code)) {
    con("%%fg=orange$Conflicting keyboard mapping, disabling key for unused7" UI_RESET_FMT);
    game_key_unused7_set(g, none);
  }
  if (sdlk_eq(game_key_unused8_get(g), code)) {
    con("%%fg=orange$Conflicting keyboard mapping, disabling key for unused8" UI_RESET_FMT);
    game_key_unused8_set(g, none);
  }
  if (sdlk_eq(game_key_unused9_get(g), code)) {
    con("%%fg=orange$Conflicting keyboard mapping, disabling key for unused9" UI_RESET_FMT);
    game_key_unused9_set(g, none);
  }
  if (sdlk_eq(game_key_unused10_get(g), code)) {
    con("%%fg=orange$Conflicting keyboard mapping, disabling key for unused10" UI_RESET_FMT);
    game_key_unused10_set(g, none);
  }
  if (sdlk_eq(game_key_unused11_get(g), code)) {
    con("%%fg=orange$Conflicting keyboard mapping, disabling key for unused11" UI_RESET_FMT);
    game_key_unused11_set(g, none);
  }
  if (sdlk_eq(game_key_cast_get(g), code)) {
    con("%%fg=orange$Conflicting keyboard mapping, disabling key for unused12" UI_RESET_FMT);
    game_key_cast_set(g, none);
  }
  if (sdlk_eq(game_key_spell1_get(g), code)) {
    con("%%fg=orange$Conflicting keyboard mapping, disabling key for spell1" UI_RESET_FMT);
    game_key_spell1_set(g, none);
  }
  if (sdlk_eq(game_key_spell2_get(g), code)) {
    con("%%fg=orange$Conflicting keyboard mapping, disabling key for spell2" UI_RESET_FMT);
    game_key_spell2_set(g, none);
  }
  if (sdlk_eq(game_key_spell3_get(g), code)) {
    con("%%fg=orange$Conflicting keyboard mapping, disabling key for spell3" UI_RESET_FMT);
    game_key_spell3_set(g, none);
  }
  if (sdlk_eq(game_key_spell4_get(g), code)) {
    con("%%fg=orange$Conflicting keyboard mapping, disabling key for spell4" UI_RESET_FMT);
    game_key_spell4_set(g, none);
  }
  if (sdlk_eq(game_key_spell5_get(g), code)) {
    con("%%fg=orange$Conflicting keyboard mapping, disabling key for spell5" UI_RESET_FMT);
    game_key_spell5_set(g, none);
  }
  if (sdlk_eq(game_key_spell6_get(g), code)) {
    con("%%fg=orange$Conflicting keyboard mapping, disabling key for spell6" UI_RESET_FMT);
    game_key_spell6_set(g, none);
  }
  if (sdlk_eq(game_key_spell7_get(g), code)) {
    con("%%fg=orange$Conflicting keyboard mapping, disabling key for spell7" UI_RESET_FMT);
    game_key_spell7_set(g, none);
  }
  if (sdlk_eq(game_key_spell8_get(g), code)) {
    con("%%fg=orange$Conflicting keyboard mapping, disabling key for spell8" UI_RESET_FMT);
    game_key_spell8_set(g, none);
  }
  if (sdlk_eq(game_key_spell9_get(g), code)) {
    con("%%fg=orange$Conflicting keyboard mapping, disabling key for spell9" UI_RESET_FMT);
    game_key_spell9_set(g, none);
  }
  if (sdlk_eq(game_key_spell10_get(g), code)) {
    con("%%fg=orange$Conflicting keyboard mapping, disabling key for spell10" UI_RESET_FMT);
    game_key_spell10_set(g, none);
  }
  if (sdlk_eq(game_key_spell11_get(g), code)) {
    con("%%fg=orange$Conflicting keyboard mapping, disabling key for spell11" UI_RESET_FMT);
    game_key_spell11_set(g, none);
  }
  if (sdlk_eq(game_key_spell12_get(g), code)) {
    con("%%fg=orange$Conflicting keyboard mapping, disabling key for spell12" UI_RESET_FMT);
    game_key_spell12_set(g, none);
  }
  if (sdlk_eq(game_key_spell13_get(g), code)) {
    con("%%fg=orange$Conflicting keyboard mapping, disabling key for spell13" UI_RESET_FMT);
    game_key_spell13_set(g, none);
  }
  if (sdlk_eq(game_key_spell14_get(g), code)) {
    con("%%fg=orange$Conflicting keyboard mapping, disabling key for spell14" UI_RESET_FMT);
    game_key_spell14_set(g, none);
  }
  if (sdlk_eq(game_key_spell15_get(g), code)) {
    con("%%fg=orange$Conflicting keyboard mapping, disabling key for spell15" UI_RESET_FMT);
    game_key_spell15_set(g, none);
  }
  if (sdlk_eq(game_key_spell16_get(g), code)) {
    con("%%fg=orange$Conflicting keyboard mapping, disabling key for spell16" UI_RESET_FMT);
    game_key_spell16_set(g, none);
  }
  if (sdlk_eq(game_key_spell17_get(g), code)) {
    con("%%fg=orange$Conflicting keyboard mapping, disabling key for spell17" UI_RESET_FMT);
    game_key_spell17_set(g, none);
  }
  if (sdlk_eq(game_key_spell18_get(g), code)) {
    con("%%fg=orange$Conflicting keyboard mapping, disabling key for spell18" UI_RESET_FMT);
    game_key_spell18_set(g, none);
  }
  if (sdlk_eq(game_key_spell19_get(g), code)) {
    con("%%fg=orange$Conflicting keyboard mapping, disabling key for spell19" UI_RESET_FMT);
    game_key_spell19_set(g, none);
  }
  if (sdlk_eq(game_key_spell20_get(g), code)) {
    con("%%fg=orange$Conflicting keyboard mapping, disabling key for spell20" UI_RESET_FMT);
    game_key_spell20_set(g, none);
  }
}

static void wid_cfg_help_destroy(Gamep g)
{
  TRACE();
  local_g_config_changed = false;

  if (wid_cfg_help_window == nullptr) {
    return;
  }

  auto *w                 = wid_cfg_help_window->wid_text_area->wid_vert_scroll;
  last_vert_scroll_offset = wid_get_tl_y(w) - wid_get_tl_y(wid_get_parent(w));

  delete wid_cfg_help_window;
  wid_cfg_help_window = nullptr;
  game_state_reset(g, "widget keyboard destroy");
}

[[nodiscard]] static auto wid_cfg_help_cancel(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();
  con("Reload config");
  if (local_g_config_changed) {
    local_g_config_changed = false;
    (void) game_load_config(g);
  }
  wid_cfg_help_destroy(g);

  if (game_levels_get(g) != nullptr) {
    //
    // Back to the game
    //
    game_state_reset(g, "back to the game");
  } else {
    wid_options_menu_select(g);
  }

  return true;
}

[[nodiscard]] static auto wid_cfg_help_save(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();

  con("Save config for keyboard");
  game_save_config(g);

  wid_cfg_help_destroy(g);

  if (game_levels_get(g) != nullptr) {
    //
    // Back to the game
    //
    game_state_reset(g, "back to the game");
  } else {
    wid_options_menu_select(g);
  }

  return true;
}

[[nodiscard]] static auto wid_cfg_help_back(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();
  wid_cfg_help_destroy(g);

  if (game_levels_get(g) != nullptr) {
    //
    // Back to the game
    //
    game_state_reset(g, "back to the game");
  } else {
    wid_options_menu_select(g);
  }

  return true;
}

static SDL_Keysym none = {};

static void wid_cfg_key_move_left_set(Gamep g, SDL_Keysym code)
{
  TRACE();
  local_g_config_changed = true;
  game_key_move_left_set(g, none);
  wid_cfg_check_for_conflicts(g, code);
  game_key_move_left_set(g, code);
  wid_cfg_help_select(g);
}

static void wid_cfg_key_move_right_set(Gamep g, SDL_Keysym code)
{
  TRACE();
  local_g_config_changed = true;
  game_key_move_right_set(g, none);
  wid_cfg_check_for_conflicts(g, code);
  game_key_move_right_set(g, code);
  wid_cfg_help_select(g);
}

static void wid_cfg_key_move_up_set(Gamep g, SDL_Keysym code)
{
  TRACE();
  local_g_config_changed = true;
  game_key_move_up_set(g, none);
  wid_cfg_check_for_conflicts(g, code);
  game_key_move_up_set(g, code);
  wid_cfg_help_select(g);
}

static void wid_cfg_key_move_down_set(Gamep g, SDL_Keysym code)
{
  TRACE();
  local_g_config_changed = true;
  game_key_move_down_set(g, none);
  wid_cfg_check_for_conflicts(g, code);
  game_key_move_down_set(g, code);
  wid_cfg_help_select(g);
}

static void wid_cfg_key_wait_set(Gamep g, SDL_Keysym code)
{
  TRACE();
  local_g_config_changed = true;
  game_key_wait_set(g, none);
  wid_cfg_check_for_conflicts(g, code);
  game_key_wait_set(g, code);
  wid_cfg_help_select(g);
}

static void wid_cfg_key_unused1_set(Gamep g, SDL_Keysym code)
{
  TRACE();
  local_g_config_changed = true;
  game_key_unused1_set(g, none);
  wid_cfg_check_for_conflicts(g, code);
  game_key_unused1_set(g, code);
  wid_cfg_help_select(g);
}

static void wid_cfg_key_unused2_set(Gamep g, SDL_Keysym code)
{
  TRACE();
  local_g_config_changed = true;
  game_key_unused2_set(g, none);
  wid_cfg_check_for_conflicts(g, code);
  game_key_unused2_set(g, code);
  wid_cfg_help_select(g);
}

static void wid_cfg_key_unused3_set(Gamep g, SDL_Keysym code)
{
  TRACE();
  local_g_config_changed = true;
  game_key_unused3_set(g, none);
  wid_cfg_check_for_conflicts(g, code);
  game_key_unused3_set(g, code);
  wid_cfg_help_select(g);
}

static void wid_cfg_key_unused4_set(Gamep g, SDL_Keysym code)
{
  TRACE();
  local_g_config_changed = true;
  game_key_unused4_set(g, none);
  wid_cfg_check_for_conflicts(g, code);
  game_key_unused4_set(g, code);
  wid_cfg_help_select(g);
}

static void wid_cfg_key_unused5_set(Gamep g, SDL_Keysym code)
{
  TRACE();
  local_g_config_changed = true;
  game_key_unused5_set(g, none);
  wid_cfg_check_for_conflicts(g, code);
  game_key_unused5_set(g, code);
  wid_cfg_help_select(g);
}

static void wid_cfg_key_unused6_set(Gamep g, SDL_Keysym code)
{
  TRACE();
  local_g_config_changed = true;
  game_key_unused6_set(g, none);
  wid_cfg_check_for_conflicts(g, code);
  game_key_unused6_set(g, code);
  wid_cfg_help_select(g);
}

static void wid_cfg_key_unused7_set(Gamep g, SDL_Keysym code)
{
  TRACE();
  local_g_config_changed = true;
  game_key_unused7_set(g, none);
  wid_cfg_check_for_conflicts(g, code);
  game_key_unused7_set(g, code);
  wid_cfg_help_select(g);
}

static void wid_cfg_key_unused8_set(Gamep g, SDL_Keysym code)
{
  TRACE();
  local_g_config_changed = true;
  game_key_unused8_set(g, none);
  wid_cfg_check_for_conflicts(g, code);
  game_key_unused8_set(g, code);
  wid_cfg_help_select(g);
}

static void wid_cfg_key_unused9_set(Gamep g, SDL_Keysym code)
{
  TRACE();
  local_g_config_changed = true;
  game_key_unused9_set(g, none);
  wid_cfg_check_for_conflicts(g, code);
  game_key_unused9_set(g, code);
  wid_cfg_help_select(g);
}

static void wid_cfg_key_unused10_set(Gamep g, SDL_Keysym code)
{
  TRACE();
  local_g_config_changed = true;
  game_key_unused10_set(g, none);
  wid_cfg_check_for_conflicts(g, code);
  game_key_unused10_set(g, code);
  wid_cfg_help_select(g);
}

static void wid_cfg_key_unused11_set(Gamep g, SDL_Keysym code)
{
  TRACE();
  local_g_config_changed = true;
  game_key_unused11_set(g, none);
  wid_cfg_check_for_conflicts(g, code);
  game_key_unused11_set(g, code);
  wid_cfg_help_select(g);
}

static void wid_cfg_key_cast_set(Gamep g, SDL_Keysym code)
{
  TRACE();
  local_g_config_changed = true;
  game_key_cast_set(g, none);
  wid_cfg_check_for_conflicts(g, code);
  game_key_cast_set(g, code);
  wid_cfg_help_select(g);
}

static void wid_cfg_key_spell1_set(Gamep g, SDL_Keysym code)
{
  TRACE();
  local_g_config_changed = true;
  game_key_spell1_set(g, none);
  wid_cfg_check_for_conflicts(g, code);
  game_key_spell1_set(g, code);
  wid_cfg_help_select(g);
}

static void wid_cfg_key_spell2_set(Gamep g, SDL_Keysym code)
{
  TRACE();
  local_g_config_changed = true;
  game_key_spell2_set(g, none);
  wid_cfg_check_for_conflicts(g, code);
  game_key_spell2_set(g, code);
  wid_cfg_help_select(g);
}

static void wid_cfg_key_spell3_set(Gamep g, SDL_Keysym code)
{
  TRACE();
  local_g_config_changed = true;
  game_key_spell3_set(g, none);
  wid_cfg_check_for_conflicts(g, code);
  game_key_spell3_set(g, code);
  wid_cfg_help_select(g);
}

static void wid_cfg_key_spell4_set(Gamep g, SDL_Keysym code)
{
  TRACE();
  local_g_config_changed = true;
  game_key_spell4_set(g, none);
  wid_cfg_check_for_conflicts(g, code);
  game_key_spell4_set(g, code);
  wid_cfg_help_select(g);
}

static void wid_cfg_key_spell5_set(Gamep g, SDL_Keysym code)
{
  TRACE();
  local_g_config_changed = true;
  game_key_spell5_set(g, none);
  wid_cfg_check_for_conflicts(g, code);
  game_key_spell5_set(g, code);
  wid_cfg_help_select(g);
}

static void wid_cfg_key_spell6_set(Gamep g, SDL_Keysym code)
{
  TRACE();
  local_g_config_changed = true;
  game_key_spell6_set(g, none);
  wid_cfg_check_for_conflicts(g, code);
  game_key_spell6_set(g, code);
  wid_cfg_help_select(g);
}

static void wid_cfg_key_spell7_set(Gamep g, SDL_Keysym code)
{
  TRACE();
  local_g_config_changed = true;
  game_key_spell7_set(g, none);
  wid_cfg_check_for_conflicts(g, code);
  game_key_spell7_set(g, code);
  wid_cfg_help_select(g);
}

static void wid_cfg_key_spell8_set(Gamep g, SDL_Keysym code)
{
  TRACE();
  local_g_config_changed = true;
  game_key_spell8_set(g, none);
  wid_cfg_check_for_conflicts(g, code);
  game_key_spell8_set(g, code);
  wid_cfg_help_select(g);
}

static void wid_cfg_key_spell9_set(Gamep g, SDL_Keysym code)
{
  TRACE();
  local_g_config_changed = true;
  game_key_spell9_set(g, none);
  wid_cfg_check_for_conflicts(g, code);
  game_key_spell9_set(g, code);
  wid_cfg_help_select(g);
}

static void wid_cfg_key_spell10_set(Gamep g, SDL_Keysym code)
{
  TRACE();
  local_g_config_changed = true;
  game_key_spell10_set(g, none);
  wid_cfg_check_for_conflicts(g, code);
  game_key_spell10_set(g, code);
  wid_cfg_help_select(g);
}

static void wid_cfg_key_spell11_set(Gamep g, SDL_Keysym code)
{
  TRACE();
  local_g_config_changed = true;
  game_key_spell11_set(g, none);
  wid_cfg_check_for_conflicts(g, code);
  game_key_spell11_set(g, code);
  wid_cfg_help_select(g);
}

static void wid_cfg_key_spell12_set(Gamep g, SDL_Keysym code)
{
  TRACE();
  local_g_config_changed = true;
  game_key_spell12_set(g, none);
  wid_cfg_check_for_conflicts(g, code);
  game_key_spell12_set(g, code);
  wid_cfg_help_select(g);
}

static void wid_cfg_key_spell13_set(Gamep g, SDL_Keysym code)
{
  TRACE();
  local_g_config_changed = true;
  game_key_spell13_set(g, none);
  wid_cfg_check_for_conflicts(g, code);
  game_key_spell13_set(g, code);
  wid_cfg_help_select(g);
}

static void wid_cfg_key_spell14_set(Gamep g, SDL_Keysym code)
{
  TRACE();
  local_g_config_changed = true;
  game_key_spell14_set(g, none);
  wid_cfg_check_for_conflicts(g, code);
  game_key_spell14_set(g, code);
  wid_cfg_help_select(g);
}

static void wid_cfg_key_spell15_set(Gamep g, SDL_Keysym code)
{
  TRACE();
  local_g_config_changed = true;
  game_key_spell15_set(g, none);
  wid_cfg_check_for_conflicts(g, code);
  game_key_spell15_set(g, code);
  wid_cfg_help_select(g);
}

static void wid_cfg_key_spell16_set(Gamep g, SDL_Keysym code)
{
  TRACE();
  local_g_config_changed = true;
  game_key_spell16_set(g, none);
  wid_cfg_check_for_conflicts(g, code);
  game_key_spell16_set(g, code);
  wid_cfg_help_select(g);
}

static void wid_cfg_key_spell17_set(Gamep g, SDL_Keysym code)
{
  TRACE();
  local_g_config_changed = true;
  game_key_spell17_set(g, none);
  wid_cfg_check_for_conflicts(g, code);
  game_key_spell17_set(g, code);
  wid_cfg_help_select(g);
}

static void wid_cfg_key_spell18_set(Gamep g, SDL_Keysym code)
{
  TRACE();
  local_g_config_changed = true;
  game_key_spell18_set(g, none);
  wid_cfg_check_for_conflicts(g, code);
  game_key_spell18_set(g, code);
  wid_cfg_help_select(g);
}

static void wid_cfg_key_spell19_set(Gamep g, SDL_Keysym code)
{
  TRACE();
  local_g_config_changed = true;
  game_key_spell19_set(g, none);
  wid_cfg_check_for_conflicts(g, code);
  game_key_spell19_set(g, code);
  wid_cfg_help_select(g);
}

static void wid_cfg_key_spell20_set(Gamep g, SDL_Keysym code)
{
  TRACE();
  local_g_config_changed = true;
  game_key_spell20_set(g, none);
  wid_cfg_check_for_conflicts(g, code);
  game_key_spell20_set(g, code);
  wid_cfg_help_select(g);
}

static void wid_cfg_key_abort_set(Gamep g, SDL_Keysym code)
{
  TRACE();
  local_g_config_changed = true;
  game_key_abort_set(g, none);
  wid_cfg_check_for_conflicts(g, code);
  game_key_abort_set(g, code);
  wid_cfg_help_select(g);
}

static void wid_cfg_key_throw_set(Gamep g, SDL_Keysym code)
{
  TRACE();
  local_g_config_changed = true;
  game_key_throw_set(g, none);
  wid_cfg_check_for_conflicts(g, code);
  game_key_throw_set(g, code);
  wid_cfg_help_select(g);
}

static void wid_cfg_key_fire_set(Gamep g, SDL_Keysym code)
{
  TRACE();
  local_g_config_changed = true;
  game_key_fire_set(g, none);
  wid_cfg_check_for_conflicts(g, code);
  game_key_fire_set(g, code);
  wid_cfg_help_select(g);
}

static void wid_cfg_key_inventory_set(Gamep g, SDL_Keysym code)
{
  TRACE();
  local_g_config_changed = true;
  game_key_inventory_set(g, none);
  wid_cfg_check_for_conflicts(g, code);
  game_key_inventory_set(g, code);
  wid_cfg_help_select(g);
}

static void wid_cfg_key_learn_set(Gamep g, SDL_Keysym code)
{
  TRACE();
  local_g_config_changed = true;
  game_key_learn_set(g, none);
  wid_cfg_check_for_conflicts(g, code);
  game_key_learn_set(g, code);
  wid_cfg_help_select(g);
}

static void wid_cfg_key_jump_set(Gamep g, SDL_Keysym code)
{
  TRACE();
  local_g_config_changed = true;
  game_key_jump_set(g, none);
  wid_cfg_check_for_conflicts(g, code);
  game_key_jump_set(g, code);
  wid_cfg_help_select(g);
}

static void wid_cfg_key_ascend_set(Gamep g, SDL_Keysym code)
{
  TRACE();
  local_g_config_changed = true;
  game_key_ascend_set(g, none);
  wid_cfg_check_for_conflicts(g, code);
  game_key_ascend_set(g, code);
  wid_cfg_help_select(g);
}

static void wid_cfg_key_descend_set(Gamep g, SDL_Keysym code)
{
  TRACE();
  local_g_config_changed = true;
  game_key_descend_set(g, none);
  wid_cfg_check_for_conflicts(g, code);
  game_key_descend_set(g, code);
  wid_cfg_help_select(g);
}

static void wid_cfg_key_zoom_set(Gamep g, SDL_Keysym code)
{
  TRACE();
  local_g_config_changed = true;
  game_key_zoom_set(g, none);
  wid_cfg_check_for_conflicts(g, code);
  game_key_zoom_set(g, code);
  wid_cfg_help_select(g);
}

static void wid_cfg_key_save_set(Gamep g, SDL_Keysym code)
{
  TRACE();
  local_g_config_changed = true;
  game_key_save_set(g, none);
  wid_cfg_check_for_conflicts(g, code);
  game_key_save_set(g, code);
  wid_cfg_help_select(g);
}

static void wid_cfg_key_load_set(Gamep g, SDL_Keysym code)
{
  TRACE();
  local_g_config_changed = true;
  game_key_load_set(g, none);
  wid_cfg_check_for_conflicts(g, code);
  game_key_load_set(g, code);
  wid_cfg_help_select(g);
}

static void wid_cfg_key_help_set(Gamep g, SDL_Keysym code)
{
  TRACE();
  local_g_config_changed = true;
  game_key_help_set(g, none);
  wid_cfg_check_for_conflicts(g, code);
  game_key_help_set(g, code);
  wid_cfg_help_select(g);
}

static void wid_cfg_key_console_set(Gamep g, SDL_Keysym code)
{
  TRACE();
  local_g_config_changed = true;
  game_key_console_set(g, none);
  wid_cfg_check_for_conflicts(g, code);
  game_key_console_set(g, code);
  wid_cfg_help_select(g);
}

static void wid_cfg_key_quit_set(Gamep g, SDL_Keysym code)
{
  TRACE();
  local_g_config_changed = true;
  game_key_quit_set(g, none);
  wid_cfg_check_for_conflicts(g, code);
  game_key_quit_set(g, code);
  wid_cfg_help_select(g);
}

static void wid_cfg_key_screenshot_set(Gamep g, SDL_Keysym code)
{
  TRACE();
  local_g_config_changed = true;
  game_key_screenshot_set(g, none);
  wid_cfg_check_for_conflicts(g, code);
  game_key_screenshot_set(g, code);
  wid_cfg_help_select(g);
}

static void grab_key(const std::string &which)
{
  TRACE();
  wid_notice("Press a key for " + which);
  g_grab_next_key        = true;
  local_g_config_changed = true;
}

[[nodiscard]] static auto wid_cfg_help_profile_arrow_keys(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();
  SDL_Keysym k {};

  local_g_config_changed = true;
  game_key_move_up_set(g, none);
  k.sym = SDLK_UP;
  wid_cfg_check_for_conflicts(g, k);
  game_key_move_up_set(g, k);

  game_key_move_left_set(g, none);
  k.sym = SDLK_LEFT;
  wid_cfg_check_for_conflicts(g, k);
  game_key_move_left_set(g, k);

  game_key_move_down_set(g, none);
  k.sym = SDLK_DOWN;
  wid_cfg_check_for_conflicts(g, k);
  game_key_move_down_set(g, k);

  game_key_move_right_set(g, none);
  k.sym = SDLK_RIGHT;
  wid_cfg_check_for_conflicts(g, k);
  game_key_move_right_set(g, k);

  wid_cfg_help_select(g);

  return true;
}

[[nodiscard]] static auto wid_cfg_help_profile_wasd(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();
  SDL_Keysym k = {};

  local_g_config_changed = true;
  game_key_move_up_set(g, none);
  k.sym = SDLK_w;
  wid_cfg_check_for_conflicts(g, k);
  game_key_move_up_set(g, k);

  game_key_move_left_set(g, none);
  k.sym = SDLK_a;
  wid_cfg_check_for_conflicts(g, k);
  game_key_move_left_set(g, k);

  game_key_move_down_set(g, none);
  k.sym = SDLK_s;
  wid_cfg_check_for_conflicts(g, k);
  game_key_move_down_set(g, k);

  game_key_move_right_set(g, none);
  k.sym = SDLK_d;
  wid_cfg_check_for_conflicts(g, k);
  game_key_move_right_set(g, k);

  wid_cfg_help_select(g);

  return true;
}

[[nodiscard]] static auto wid_cfg_key_move_left(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();
  grab_key("move left");
  sdl.on_sdl_key_grab    = wid_cfg_key_move_left_set;
  local_g_config_changed = true;
  return true;
}

[[nodiscard]] static auto wid_cfg_key_move_right(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();
  grab_key("move right");
  sdl.on_sdl_key_grab    = wid_cfg_key_move_right_set;
  local_g_config_changed = true;
  return true;
}

[[nodiscard]] static auto wid_cfg_key_move_up(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();
  grab_key("move up");
  sdl.on_sdl_key_grab    = wid_cfg_key_move_up_set;
  local_g_config_changed = true;
  return true;
}

[[nodiscard]] static auto wid_cfg_key_move_down(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();
  grab_key("move down");
  sdl.on_sdl_key_grab    = wid_cfg_key_move_down_set;
  local_g_config_changed = true;
  return true;
}

[[nodiscard]] static auto wid_cfg_key_wait(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();
  grab_key("attack");
  sdl.on_sdl_key_grab    = wid_cfg_key_wait_set;
  local_g_config_changed = true;
  return true;
}

[[nodiscard]] static auto wid_cfg_key_unused1(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();
  grab_key("key_unused1");
  sdl.on_sdl_key_grab    = wid_cfg_key_unused1_set;
  local_g_config_changed = true;
  return true;
}

[[nodiscard]] static auto wid_cfg_key_unused2(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();
  grab_key("key_unused2");
  sdl.on_sdl_key_grab    = wid_cfg_key_unused2_set;
  local_g_config_changed = true;
  return true;
}

[[nodiscard]] static auto wid_cfg_key_unused3(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();
  grab_key("key_unused3");
  sdl.on_sdl_key_grab    = wid_cfg_key_unused3_set;
  local_g_config_changed = true;
  return true;
}

[[nodiscard]] static auto wid_cfg_key_unused4(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();
  grab_key("key_unused4");
  sdl.on_sdl_key_grab    = wid_cfg_key_unused4_set;
  local_g_config_changed = true;
  return true;
}

[[nodiscard]] static auto wid_cfg_key_unused5(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();
  grab_key("key_unused5");
  sdl.on_sdl_key_grab    = wid_cfg_key_unused5_set;
  local_g_config_changed = true;
  return true;
}

[[nodiscard]] static auto wid_cfg_key_unused6(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();
  grab_key("key_unused6");
  sdl.on_sdl_key_grab    = wid_cfg_key_unused6_set;
  local_g_config_changed = true;
  return true;
}

[[nodiscard]] static auto wid_cfg_key_unused7(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();
  grab_key("key_unused7");
  sdl.on_sdl_key_grab    = wid_cfg_key_unused7_set;
  local_g_config_changed = true;
  return true;
}

[[nodiscard]] static auto wid_cfg_key_unused8(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();
  grab_key("key_unused8");
  sdl.on_sdl_key_grab    = wid_cfg_key_unused8_set;
  local_g_config_changed = true;
  return true;
}

[[nodiscard]] static auto wid_cfg_key_unused9(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();
  grab_key("key_unused9");
  sdl.on_sdl_key_grab    = wid_cfg_key_unused9_set;
  local_g_config_changed = true;
  return true;
}

[[nodiscard]] static auto wid_cfg_key_unused10(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();
  grab_key("key_unused10");
  sdl.on_sdl_key_grab    = wid_cfg_key_unused10_set;
  local_g_config_changed = true;
  return true;
}

[[nodiscard]] static auto wid_cfg_key_unused11(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();
  grab_key("key_unused11");
  sdl.on_sdl_key_grab    = wid_cfg_key_unused11_set;
  local_g_config_changed = true;
  return true;
}

[[nodiscard]] static auto wid_cfg_key_cast(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();
  grab_key("key_cast");
  sdl.on_sdl_key_grab    = wid_cfg_key_cast_set;
  local_g_config_changed = true;
  return true;
}

[[nodiscard]] static auto wid_cfg_key_spell1(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();
  grab_key("key_spell1");
  sdl.on_sdl_key_grab    = wid_cfg_key_spell1_set;
  local_g_config_changed = true;
  return true;
}

[[nodiscard]] static auto wid_cfg_key_spell2(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();
  grab_key("key_spell2");
  sdl.on_sdl_key_grab    = wid_cfg_key_spell2_set;
  local_g_config_changed = true;
  return true;
}

[[nodiscard]] static auto wid_cfg_key_spell3(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();
  grab_key("key_spell3");
  sdl.on_sdl_key_grab    = wid_cfg_key_spell3_set;
  local_g_config_changed = true;
  return true;
}

[[nodiscard]] static auto wid_cfg_key_spell4(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();
  grab_key("key_spell4");
  sdl.on_sdl_key_grab    = wid_cfg_key_spell4_set;
  local_g_config_changed = true;
  return true;
}

[[nodiscard]] static auto wid_cfg_key_spell5(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();
  grab_key("key_spell5");
  sdl.on_sdl_key_grab    = wid_cfg_key_spell5_set;
  local_g_config_changed = true;
  return true;
}

[[nodiscard]] static auto wid_cfg_key_spell6(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();
  grab_key("key_spell6");
  sdl.on_sdl_key_grab    = wid_cfg_key_spell6_set;
  local_g_config_changed = true;
  return true;
}

[[nodiscard]] static auto wid_cfg_key_spell7(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();
  grab_key("key_spell7");
  sdl.on_sdl_key_grab    = wid_cfg_key_spell7_set;
  local_g_config_changed = true;
  return true;
}

[[nodiscard]] static auto wid_cfg_key_spell8(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();
  grab_key("key_spell8");
  sdl.on_sdl_key_grab    = wid_cfg_key_spell8_set;
  local_g_config_changed = true;
  return true;
}

[[nodiscard]] static auto wid_cfg_key_spell9(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();
  grab_key("key_spell9");
  sdl.on_sdl_key_grab    = wid_cfg_key_spell9_set;
  local_g_config_changed = true;
  return true;
}

[[nodiscard]] static auto wid_cfg_key_spell10(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();
  grab_key("key_spell10");
  sdl.on_sdl_key_grab    = wid_cfg_key_spell10_set;
  local_g_config_changed = true;
  return true;
}

[[nodiscard]] static auto wid_cfg_key_spell11(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();
  grab_key("key_spell11");
  sdl.on_sdl_key_grab    = wid_cfg_key_spell11_set;
  local_g_config_changed = true;
  return true;
}

[[nodiscard]] static auto wid_cfg_key_spell12(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();
  grab_key("key_spell12");
  sdl.on_sdl_key_grab    = wid_cfg_key_spell12_set;
  local_g_config_changed = true;
  return true;
}

[[nodiscard]] static auto wid_cfg_key_spell13(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();
  grab_key("key_spell13");
  sdl.on_sdl_key_grab    = wid_cfg_key_spell13_set;
  local_g_config_changed = true;
  return true;
}

[[nodiscard]] static auto wid_cfg_key_spell14(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();
  grab_key("key_spell14");
  sdl.on_sdl_key_grab    = wid_cfg_key_spell14_set;
  local_g_config_changed = true;
  return true;
}

[[nodiscard]] static auto wid_cfg_key_spell15(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();
  grab_key("key_spell15");
  sdl.on_sdl_key_grab    = wid_cfg_key_spell15_set;
  local_g_config_changed = true;
  return true;
}

[[nodiscard]] static auto wid_cfg_key_spell16(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();
  grab_key("key_spell16");
  sdl.on_sdl_key_grab    = wid_cfg_key_spell16_set;
  local_g_config_changed = true;
  return true;
}

[[nodiscard]] static auto wid_cfg_key_spell17(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();
  grab_key("key_spell17");
  sdl.on_sdl_key_grab    = wid_cfg_key_spell17_set;
  local_g_config_changed = true;
  return true;
}

[[nodiscard]] static auto wid_cfg_key_spell18(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();
  grab_key("key_spell18");
  sdl.on_sdl_key_grab    = wid_cfg_key_spell18_set;
  local_g_config_changed = true;
  return true;
}

[[nodiscard]] static auto wid_cfg_key_spell19(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();
  grab_key("key_spell19");
  sdl.on_sdl_key_grab    = wid_cfg_key_spell19_set;
  local_g_config_changed = true;
  return true;
}

[[nodiscard]] static auto wid_cfg_key_spell20(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();
  grab_key("key_spell20");
  sdl.on_sdl_key_grab    = wid_cfg_key_spell20_set;
  local_g_config_changed = true;
  return true;
}

[[nodiscard]] static auto wid_cfg_key_abort(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();
  grab_key("key_abort");
  sdl.on_sdl_key_grab    = wid_cfg_key_abort_set;
  local_g_config_changed = true;
  return true;
}

[[nodiscard]] static auto wid_cfg_key_throw(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();
  grab_key("key_throw");
  sdl.on_sdl_key_grab    = wid_cfg_key_throw_set;
  local_g_config_changed = true;
  return true;
}

[[nodiscard]] static auto wid_cfg_key_fire(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();
  grab_key("key_fire");
  sdl.on_sdl_key_grab    = wid_cfg_key_fire_set;
  local_g_config_changed = true;
  return true;
}

[[nodiscard]] static auto wid_cfg_key_inventory(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();
  grab_key("key_inventory");
  sdl.on_sdl_key_grab    = wid_cfg_key_inventory_set;
  local_g_config_changed = true;
  return true;
}

[[nodiscard]] static auto wid_cfg_key_learn(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();
  grab_key("key_learn");
  sdl.on_sdl_key_grab    = wid_cfg_key_learn_set;
  local_g_config_changed = true;
  return true;
}

[[nodiscard]] static auto wid_cfg_key_jump(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();
  grab_key("jump");
  sdl.on_sdl_key_grab    = wid_cfg_key_jump_set;
  local_g_config_changed = true;
  return true;
}

[[nodiscard]] static auto wid_cfg_key_ascend(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();
  grab_key("ascend");
  sdl.on_sdl_key_grab    = wid_cfg_key_ascend_set;
  local_g_config_changed = true;
  return true;
}

[[nodiscard]] static auto wid_cfg_key_descend(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();
  grab_key("descend");
  sdl.on_sdl_key_grab    = wid_cfg_key_descend_set;
  local_g_config_changed = true;
  return true;
}

[[nodiscard]] static auto wid_cfg_key_zoom(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();
  grab_key("zoom toggle");
  sdl.on_sdl_key_grab    = wid_cfg_key_zoom_set;
  local_g_config_changed = true;
  return true;
}

[[nodiscard]] static auto wid_cfg_key_save(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();
  grab_key("save game");
  sdl.on_sdl_key_grab    = wid_cfg_key_save_set;
  local_g_config_changed = true;
  return true;
}

[[nodiscard]] static auto wid_cfg_key_load(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();
  grab_key("load game");
  sdl.on_sdl_key_grab    = wid_cfg_key_load_set;
  local_g_config_changed = true;
  return true;
}

[[nodiscard]] static auto wid_cfg_key_help(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();
  grab_key("help");
  sdl.on_sdl_key_grab    = wid_cfg_key_help_set;
  local_g_config_changed = true;
  return true;
}

[[nodiscard]] static auto wid_cfg_key_console(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();
  grab_key("toggle console");
  sdl.on_sdl_key_grab    = wid_cfg_key_console_set;
  local_g_config_changed = true;
  return true;
}

[[nodiscard]] static auto wid_cfg_key_quit(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();
  grab_key("quit");
  sdl.on_sdl_key_grab    = wid_cfg_key_quit_set;
  local_g_config_changed = true;
  return true;
}

[[nodiscard]] static auto wid_cfg_key_screenshot(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();
  grab_key("screenshot grab");
  local_g_config_changed = true;
  sdl.on_sdl_key_grab    = wid_cfg_key_screenshot_set;
  return true;
}

[[nodiscard]] static auto wid_cfg_help_key_down(Gamep g, Widp w, const struct SDL_Keysym *key) -> bool
{
  TRACE();

  if (sdlk_eq(*key, game_key_console_get(g))) {
    ((void) sound_play(g, "keypress"));
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
              case 'b' :
              case 'B' :
              case SDLK_ESCAPE :
                ((void) sound_play(g, "keypress"));
                (void) wid_cfg_help_cancel(g, nullptr, 0, 0, 0);
                return true;
            }
          }
      }
  }

  return false;
}

void wid_cfg_help_select(Gamep g)
{
  TRACE();
  wid_notice_destroy();

  if (wid_cfg_help_window != nullptr) {
    wid_cfg_help_destroy(g);
  }

  const int menu_width  = UI_INVENTORY_WIDTH;
  const int menu_height = TERM_HEIGHT;

  const int left_half  = menu_width / 2;
  const int right_half = menu_width - left_half;
  const int top_half   = menu_height / 2;
  const int bot_half   = menu_height - top_half;

  {
    TRACE();
    spoint const tl((TERM_WIDTH / 2) - left_half, (TERM_HEIGHT / 2) - top_half);
    spoint const br((TERM_WIDTH / 2) + right_half - 1, (TERM_HEIGHT / 2) + bot_half - 1);

    wid_cfg_help_window = new WidPopup(g, "Keyboard select", tl, br, nullptr, "", false, true);

    TRACE();
    Widp w = wid_cfg_help_window->wid_popup_container;
    wid_set_on_key_down(w, wid_cfg_help_key_down);
    wid_set_style(w, UI_WID_STYLE_BUTTON_OUTLINE);
  }

  auto rhs_button_left  = menu_width - 14;
  auto rhs_button_right = menu_width - 3;

  int y_at = 0;
  {
    TRACE();
    auto *p = wid_cfg_help_window->wid_text_area->wid_text_area;
    auto *w = wid_new_square_button(g, p, "Keyboard");

    spoint const tl(0, y_at);
    spoint const br(menu_width, y_at);
    wid_set_shape_none(w);
    wid_set_pos(w, tl, br);
    wid_set_text(w, "Keyboard");
  }

  y_at = 2;
  {
    TRACE();
    auto *p = wid_cfg_help_window->wid_text_area->wid_text_area;
    auto *w = wid_new_back_button(g, p, "BACK");

    spoint const tl(1, y_at);
    spoint const br(8, y_at + 2);
    wid_set_on_mouse_down(w, wid_cfg_help_back);
    wid_set_pos(w, tl, br);
  }
  {
    TRACE();
    auto *p = wid_cfg_help_window->wid_text_area->wid_text_area;
    auto *w = wid_new_save_button(g, p, "Save");

    spoint const tl(menu_width - 17, y_at);
    spoint const br(menu_width - 12, y_at + 2);
    wid_set_on_mouse_down(w, wid_cfg_help_save);
    wid_set_pos(w, tl, br);
    wid_set_text(w, "Save");
  }
  {
    TRACE();
    auto *p = wid_cfg_help_window->wid_text_area->wid_text_area;
    auto *w = wid_new_cancel_button(g, p, "Cancel");

    spoint const tl(menu_width - 10, y_at);
    spoint const br(menu_width - 3, y_at + 2);
    wid_set_on_mouse_down(w, wid_cfg_help_cancel);
    wid_set_pos(w, tl, br);
    wid_set_text(w, "Cancel");
  }

  ///////////////////////////////////////////////////////////////////////
  y_at++;
  ///////////////////////////////////////////////////////////////////////

  y_at += 3;
  {
    TRACE();
    auto *p = wid_cfg_help_window->wid_text_area->wid_text_area;
    auto *w = wid_new_bar_button(g, p, "");

    spoint const tl(1, y_at);
    spoint const br(menu_width - 3, y_at);
    wid_set_on_mouse_down(w, wid_cfg_help_profile_wasd);
    wid_set_pos(w, tl, br);
    wid_set_text(w, "Use W,A,S,D for moving");
  }
  y_at++;
  {
    TRACE();
    auto *p = wid_cfg_help_window->wid_text_area->wid_text_area;
    auto *w = wid_new_bar_button(g, p, "");

    spoint const tl(1, y_at);
    spoint const br(menu_width - 3, y_at);
    wid_set_on_mouse_down(w, wid_cfg_help_profile_arrow_keys);
    wid_set_pos(w, tl, br);
    wid_set_text(w, "Use arrow keys for moving");
  }

  y_at++;
  ///////////////////////////////////////////////////////////////////////
  // save
  ///////////////////////////////////////////////////////////////////////
  y_at++;
  {
    TRACE();
    auto *p = wid_cfg_help_window->wid_text_area->wid_text_area;
    auto *w = wid_new_square_button(g, p, "save");

    spoint const tl(0, y_at);
    spoint const br(menu_width / 2, y_at);
    wid_set_shape_none(w);
    wid_set_pos(w, tl, br);
    wid_set_text_lhs(w);
    wid_set_text(w, "Save game");
  }
  {
    TRACE();
    auto *p = wid_cfg_help_window->wid_text_area->wid_text_area;
    auto *w = wid_new_bar_button(g, p, "value");

    spoint const tl(rhs_button_left, y_at);
    spoint const br(rhs_button_right, y_at);
    wid_set_text_rhs(w);
    wid_set_pos(w, tl, br);
    wid_set_text(w, ::to_string(game_key_save_get(g)));
    wid_set_on_mouse_down(w, wid_cfg_key_save);
  }
  ///////////////////////////////////////////////////////////////////////
  // load
  ///////////////////////////////////////////////////////////////////////
  y_at++;
  {
    TRACE();
    auto *p = wid_cfg_help_window->wid_text_area->wid_text_area;
    auto *w = wid_new_square_button(g, p, "load");

    spoint const tl(0, y_at);
    spoint const br(menu_width / 2, y_at);
    wid_set_shape_none(w);
    wid_set_pos(w, tl, br);
    wid_set_text_lhs(w);
    wid_set_text(w, "Load game");
  }
  {
    TRACE();
    auto *p = wid_cfg_help_window->wid_text_area->wid_text_area;
    auto *w = wid_new_bar_button(g, p, "value");

    spoint const tl(rhs_button_left, y_at);
    spoint const br(rhs_button_right, y_at);
    wid_set_text_rhs(w);
    wid_set_pos(w, tl, br);
    wid_set_text(w, ::to_string(game_key_load_get(g)));
    wid_set_on_mouse_down(w, wid_cfg_key_load);
  }

  ///////////////////////////////////////////////////////////////////////
  // Move up
  ///////////////////////////////////////////////////////////////////////
  y_at += 2;
  {
    TRACE();
    auto *p = wid_cfg_help_window->wid_text_area->wid_text_area;
    auto *w = wid_new_square_button(g, p, "Move up");

    spoint const tl(0, y_at);
    spoint const br(menu_width / 2, y_at);
    wid_set_shape_none(w);
    wid_set_pos(w, tl, br);
    wid_set_text_lhs(w);
    wid_set_text(w, "Move up");
  }
  {
    TRACE();
    auto *p = wid_cfg_help_window->wid_text_area->wid_text_area;
    auto *w = wid_new_bar_button(g, p, "value");

    spoint const tl(rhs_button_left, y_at);
    spoint const br(rhs_button_right, y_at);
    wid_set_text_rhs(w);
    wid_set_pos(w, tl, br);
    wid_set_text(w, ::to_string(game_key_move_up_get(g)));
    wid_set_on_mouse_down(w, wid_cfg_key_move_up);
  }

  ///////////////////////////////////////////////////////////////////////
  // Move left
  ///////////////////////////////////////////////////////////////////////
  y_at++;
  {
    TRACE();
    auto *p = wid_cfg_help_window->wid_text_area->wid_text_area;
    auto *w = wid_new_square_button(g, p, "Move left");

    spoint const tl(0, y_at);
    spoint const br(menu_width / 2, y_at);
    wid_set_shape_none(w);
    wid_set_pos(w, tl, br);
    wid_set_text_lhs(w);
    wid_set_text(w, "Move left");
  }
  {
    TRACE();
    auto *p = wid_cfg_help_window->wid_text_area->wid_text_area;
    auto *w = wid_new_bar_button(g, p, "value");

    spoint const tl(rhs_button_left, y_at);
    spoint const br(rhs_button_right, y_at);
    wid_set_text_rhs(w);
    wid_set_pos(w, tl, br);
    wid_set_text(w, ::to_string(game_key_move_left_get(g)));
    wid_set_on_mouse_down(w, wid_cfg_key_move_left);
  }

  ///////////////////////////////////////////////////////////////////////
  // Move down
  ///////////////////////////////////////////////////////////////////////
  y_at++;
  {
    TRACE();
    auto *p = wid_cfg_help_window->wid_text_area->wid_text_area;
    auto *w = wid_new_square_button(g, p, "Move down");

    spoint const tl(0, y_at);
    spoint const br(menu_width / 2, y_at);
    wid_set_shape_none(w);
    wid_set_pos(w, tl, br);
    wid_set_text_lhs(w);
    wid_set_text(w, "Move down");
  }
  {
    TRACE();
    auto *p = wid_cfg_help_window->wid_text_area->wid_text_area;
    auto *w = wid_new_bar_button(g, p, "value");

    spoint const tl(rhs_button_left, y_at);
    spoint const br(rhs_button_right, y_at);
    wid_set_text_rhs(w);
    wid_set_pos(w, tl, br);
    wid_set_text(w, ::to_string(game_key_move_down_get(g)));
    wid_set_on_mouse_down(w, wid_cfg_key_move_down);
  }

  ///////////////////////////////////////////////////////////////////////
  // Move right
  ///////////////////////////////////////////////////////////////////////
  y_at++;
  {
    TRACE();
    auto *p = wid_cfg_help_window->wid_text_area->wid_text_area;
    auto *w = wid_new_square_button(g, p, "Move right");

    spoint const tl(0, y_at);
    spoint const br(menu_width / 2, y_at);
    wid_set_shape_none(w);
    wid_set_pos(w, tl, br);
    wid_set_text_lhs(w);
    wid_set_text(w, "Move right");
  }
  {
    TRACE();
    auto *p = wid_cfg_help_window->wid_text_area->wid_text_area;
    auto *w = wid_new_bar_button(g, p, "value");

    spoint const tl(rhs_button_left, y_at);
    spoint const br(rhs_button_right, y_at);
    wid_set_text_rhs(w);
    wid_set_pos(w, tl, br);
    wid_set_text(w, ::to_string(game_key_move_right_get(g)));
    wid_set_on_mouse_down(w, wid_cfg_key_move_right);
  }

  ///////////////////////////////////////////////////////////////////////
  // fire
  ///////////////////////////////////////////////////////////////////////
  y_at++;
  {
    TRACE();
    auto *p = wid_cfg_help_window->wid_text_area->wid_text_area;
    auto *w = wid_new_square_button(g, p, "fire");

    spoint const tl(0, y_at);
    spoint const br(menu_width / 2, y_at);
    wid_set_shape_none(w);
    wid_set_pos(w, tl, br);
    wid_set_text_lhs(w);
    wid_set_text(w, "Fire");
  }
  {
    TRACE();
    auto *p = wid_cfg_help_window->wid_text_area->wid_text_area;
    auto *w = wid_new_bar_button(g, p, "value");

    spoint const tl(rhs_button_left, y_at);
    spoint const br(rhs_button_right, y_at);
    wid_set_text_rhs(w);
    wid_set_pos(w, tl, br);
    wid_set_text(w, ::to_string(game_key_fire_get(g)));
    wid_set_on_mouse_down(w, wid_cfg_key_fire);
  }

  ///////////////////////////////////////////////////////////////////////
  // jump
  ///////////////////////////////////////////////////////////////////////
  y_at++;
  {
    TRACE();
    auto *p = wid_cfg_help_window->wid_text_area->wid_text_area;
    auto *w = wid_new_square_button(g, p, "jump");

    spoint const tl(0, y_at);
    spoint const br(menu_width / 2, y_at);
    wid_set_shape_none(w);
    wid_set_pos(w, tl, br);
    wid_set_text_lhs(w);
    wid_set_text(w, "Jump");
  }
  {
    TRACE();
    auto *p = wid_cfg_help_window->wid_text_area->wid_text_area;
    auto *w = wid_new_bar_button(g, p, "value");

    spoint const tl(rhs_button_left, y_at);
    spoint const br(rhs_button_right, y_at);
    wid_set_text_rhs(w);
    wid_set_pos(w, tl, br);
    wid_set_text(w, ::to_string(game_key_jump_get(g)));
    wid_set_on_mouse_down(w, wid_cfg_key_jump);
  }
  ///////////////////////////////////////////////////////////////////////
  // throw item
  ///////////////////////////////////////////////////////////////////////
  y_at++;
  {
    TRACE();
    auto *p = wid_cfg_help_window->wid_text_area->wid_text_area;
    auto *w = wid_new_square_button(g, p, "key_throw");

    spoint const tl(0, y_at);
    spoint const br(menu_width / 2, y_at);
    wid_set_shape_none(w);
    wid_set_pos(w, tl, br);
    wid_set_text_lhs(w);
    wid_set_text(w, "Throw item");
  }
  {
    TRACE();
    auto *p = wid_cfg_help_window->wid_text_area->wid_text_area;
    auto *w = wid_new_bar_button(g, p, "value");

    spoint const tl(rhs_button_left, y_at);
    spoint const br(rhs_button_right, y_at);
    wid_set_text_rhs(w);
    wid_set_pos(w, tl, br);
    wid_set_text(w, ::to_string(game_key_throw_get(g)));
    wid_set_on_mouse_down(w, wid_cfg_key_throw);
  }
  ///////////////////////////////////////////////////////////////////////
  // abort
  ///////////////////////////////////////////////////////////////////////
  y_at++;
  {
    TRACE();
    auto *p = wid_cfg_help_window->wid_text_area->wid_text_area;
    auto *w = wid_new_square_button(g, p, "key_abort");

    spoint const tl(0, y_at);
    spoint const br(menu_width / 2, y_at);
    wid_set_shape_none(w);
    wid_set_pos(w, tl, br);
    wid_set_text_lhs(w);
    wid_set_text(w, "Abort task e.g. throw item");
  }
  {
    TRACE();
    auto *p = wid_cfg_help_window->wid_text_area->wid_text_area;
    auto *w = wid_new_bar_button(g, p, "value");

    spoint const tl(rhs_button_left, y_at);
    spoint const br(rhs_button_right, y_at);
    wid_set_text_rhs(w);
    wid_set_pos(w, tl, br);
    wid_set_text(w, ::to_string(game_key_abort_get(g)));
    wid_set_on_mouse_down(w, wid_cfg_key_abort);
  }

  ///////////////////////////////////////////////////////////////////////
  y_at++;
  ///////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////
  // wait
  ///////////////////////////////////////////////////////////////////////
  y_at++;
  {
    TRACE();
    auto *p = wid_cfg_help_window->wid_text_area->wid_text_area;
    auto *w = wid_new_square_button(g, p, "Wait");

    spoint const tl(0, y_at);
    spoint const br(menu_width / 2, y_at);
    wid_set_shape_none(w);
    wid_set_pos(w, tl, br);
    wid_set_text_lhs(w);
    wid_set_text(w, "Wait");
  }
  {
    TRACE();
    auto *p = wid_cfg_help_window->wid_text_area->wid_text_area;
    auto *w = wid_new_bar_button(g, p, "value");

    spoint const tl(rhs_button_left, y_at);
    spoint const br(rhs_button_right, y_at);
    wid_set_text_rhs(w);
    wid_set_pos(w, tl, br);
    wid_set_text(w, ::to_string(game_key_wait_get(g)));
    wid_set_on_mouse_down(w, wid_cfg_key_wait);
  }

  ///////////////////////////////////////////////////////////////////////
  // inventory
  ///////////////////////////////////////////////////////////////////////
  y_at++;
  {
    TRACE();
    auto *p = wid_cfg_help_window->wid_text_area->wid_text_area;
    auto *w = wid_new_square_button(g, p, "inventory");

    spoint const tl(0, y_at);
    spoint const br(menu_width / 2, y_at);
    wid_set_shape_none(w);
    wid_set_pos(w, tl, br);
    wid_set_text_lhs(w);
    wid_set_text(w, "Inventory");
  }
  {
    TRACE();
    auto *p = wid_cfg_help_window->wid_text_area->wid_text_area;
    auto *w = wid_new_bar_button(g, p, "value");

    spoint const tl(rhs_button_left, y_at);
    spoint const br(rhs_button_right, y_at);
    wid_set_text_rhs(w);
    wid_set_pos(w, tl, br);
    wid_set_text(w, ::to_string(game_key_inventory_get(g)));
    wid_set_on_mouse_down(w, wid_cfg_key_inventory);
  }

  ///////////////////////////////////////////////////////////////////////
  // Learn
  ///////////////////////////////////////////////////////////////////////
  y_at++;
  {
    TRACE();
    auto *p = wid_cfg_help_window->wid_text_area->wid_text_area;
    auto *w = wid_new_square_button(g, p, "Learn");

    spoint const tl(0, y_at);
    spoint const br(menu_width / 2, y_at);
    wid_set_shape_none(w);
    wid_set_pos(w, tl, br);
    wid_set_text_lhs(w);
    wid_set_text(w, "Inventory");
  }
  {
    TRACE();
    auto *p = wid_cfg_help_window->wid_text_area->wid_text_area;
    auto *w = wid_new_bar_button(g, p, "value");

    spoint const tl(rhs_button_left, y_at);
    spoint const br(rhs_button_right, y_at);
    wid_set_text_rhs(w);
    wid_set_pos(w, tl, br);
    wid_set_text(w, ::to_string(game_key_learn_get(g)));
    wid_set_on_mouse_down(w, wid_cfg_key_learn);
  }

  ///////////////////////////////////////////////////////////////////////
  // Cast
  ///////////////////////////////////////////////////////////////////////
  y_at++;
  {
    TRACE();
    auto *p = wid_cfg_help_window->wid_text_area->wid_text_area;
    auto *w = wid_new_square_button(g, p, "Cast");

    spoint const tl(0, y_at);
    spoint const br(menu_width / 2, y_at);
    wid_set_shape_none(w);
    wid_set_pos(w, tl, br);
    wid_set_text_lhs(w);
    wid_set_text(w, "key_cast");
  }
  {
    TRACE();
    auto *p = wid_cfg_help_window->wid_text_area->wid_text_area;
    auto *w = wid_new_bar_button(g, p, "value");

    spoint const tl(rhs_button_left, y_at);
    spoint const br(rhs_button_right, y_at);
    wid_set_text_rhs(w);
    wid_set_pos(w, tl, br);
    wid_set_text(w, ::to_string(game_key_cast_get(g)));
    wid_set_on_mouse_down(w, wid_cfg_key_cast);
  }

  ///////////////////////////////////////////////////////////////////////
  // Ascend
  ///////////////////////////////////////////////////////////////////////
  y_at++;
  {
    TRACE();
    auto *p = wid_cfg_help_window->wid_text_area->wid_text_area;
    auto *w = wid_new_square_button(g, p, "Ascend");

    spoint const tl(0, y_at);
    spoint const br(menu_width / 2, y_at);
    wid_set_shape_none(w);
    wid_set_pos(w, tl, br);
    wid_set_text_lhs(w);
    wid_set_text(w, "Ascend");
  }
  {
    TRACE();
    auto *p = wid_cfg_help_window->wid_text_area->wid_text_area;
    auto *w = wid_new_bar_button(g, p, "value");

    spoint const tl(rhs_button_left, y_at);
    spoint const br(rhs_button_right, y_at);
    wid_set_text_rhs(w);
    wid_set_pos(w, tl, br);
    wid_set_text(w, ::to_string(game_key_ascend_get(g)));
    wid_set_on_mouse_down(w, wid_cfg_key_ascend);
  }

  ///////////////////////////////////////////////////////////////////////
  // Descend
  ///////////////////////////////////////////////////////////////////////
  y_at++;
  {
    TRACE();
    auto *p = wid_cfg_help_window->wid_text_area->wid_text_area;
    auto *w = wid_new_square_button(g, p, "Descend");

    spoint const tl(0, y_at);
    spoint const br(menu_width / 2, y_at);
    wid_set_shape_none(w);
    wid_set_pos(w, tl, br);
    wid_set_text_lhs(w);
    wid_set_text(w, "Descend");
  }
  {
    TRACE();
    auto *p = wid_cfg_help_window->wid_text_area->wid_text_area;
    auto *w = wid_new_bar_button(g, p, "value");

    spoint const tl(rhs_button_left, y_at);
    spoint const br(rhs_button_right, y_at);
    wid_set_text_rhs(w);
    wid_set_pos(w, tl, br);
    wid_set_text(w, ::to_string(game_key_descend_get(g)));
    wid_set_on_mouse_down(w, wid_cfg_key_descend);
  }

  ///////////////////////////////////////////////////////////////////////
  y_at++;
  ///////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////
  // spell1
  ///////////////////////////////////////////////////////////////////////
  y_at++;
  {
    TRACE();
    auto *p = wid_cfg_help_window->wid_text_area->wid_text_area;
    auto *w = wid_new_square_button(g, p, "key_spell1");

    spoint const tl(0, y_at);
    spoint const br(menu_width / 2, y_at);
    wid_set_shape_none(w);
    wid_set_pos(w, tl, br);
    wid_set_text_lhs(w);
    wid_set_text(w, "Cast spell 1");
  }
  {
    TRACE();
    auto *p = wid_cfg_help_window->wid_text_area->wid_text_area;
    auto *w = wid_new_bar_button(g, p, "value");

    spoint const tl(rhs_button_left, y_at);
    spoint const br(rhs_button_right, y_at);
    wid_set_text_rhs(w);
    wid_set_pos(w, tl, br);
    wid_set_text(w, ::to_string(game_key_spell1_get(g)));
    wid_set_on_mouse_down(w, wid_cfg_key_spell1);
  }

  ///////////////////////////////////////////////////////////////////////
  // spell2
  ///////////////////////////////////////////////////////////////////////
  y_at++;
  {
    TRACE();
    auto *p = wid_cfg_help_window->wid_text_area->wid_text_area;
    auto *w = wid_new_square_button(g, p, "key_spell2");

    spoint const tl(0, y_at);
    spoint const br(menu_width / 2, y_at);
    wid_set_shape_none(w);
    wid_set_pos(w, tl, br);
    wid_set_text_lhs(w);
    wid_set_text(w, "Cast spell 2");
  }
  {
    TRACE();
    auto *p = wid_cfg_help_window->wid_text_area->wid_text_area;
    auto *w = wid_new_bar_button(g, p, "value");

    spoint const tl(rhs_button_left, y_at);
    spoint const br(rhs_button_right, y_at);
    wid_set_text_rhs(w);
    wid_set_pos(w, tl, br);
    wid_set_text(w, ::to_string(game_key_spell2_get(g)));
    wid_set_on_mouse_down(w, wid_cfg_key_spell2);
  }

  ///////////////////////////////////////////////////////////////////////
  // spell3
  ///////////////////////////////////////////////////////////////////////
  y_at++;
  {
    TRACE();
    auto *p = wid_cfg_help_window->wid_text_area->wid_text_area;
    auto *w = wid_new_square_button(g, p, "key_spell3");

    spoint const tl(0, y_at);
    spoint const br(menu_width / 2, y_at);
    wid_set_shape_none(w);
    wid_set_pos(w, tl, br);
    wid_set_text_lhs(w);
    wid_set_text(w, "Cast spell 3");
  }
  {
    TRACE();
    auto *p = wid_cfg_help_window->wid_text_area->wid_text_area;
    auto *w = wid_new_bar_button(g, p, "value");

    spoint const tl(rhs_button_left, y_at);
    spoint const br(rhs_button_right, y_at);
    wid_set_text_rhs(w);
    wid_set_pos(w, tl, br);
    wid_set_text(w, ::to_string(game_key_spell3_get(g)));
    wid_set_on_mouse_down(w, wid_cfg_key_spell3);
  }

  ///////////////////////////////////////////////////////////////////////
  // spell4
  ///////////////////////////////////////////////////////////////////////
  y_at++;
  {
    TRACE();
    auto *p = wid_cfg_help_window->wid_text_area->wid_text_area;
    auto *w = wid_new_square_button(g, p, "key_spell4");

    spoint const tl(0, y_at);
    spoint const br(menu_width / 2, y_at);
    wid_set_shape_none(w);
    wid_set_pos(w, tl, br);
    wid_set_text_lhs(w);
    wid_set_text(w, "Cast spell 4");
  }
  {
    TRACE();
    auto *p = wid_cfg_help_window->wid_text_area->wid_text_area;
    auto *w = wid_new_bar_button(g, p, "value");

    spoint const tl(rhs_button_left, y_at);
    spoint const br(rhs_button_right, y_at);
    wid_set_text_rhs(w);
    wid_set_pos(w, tl, br);
    wid_set_text(w, ::to_string(game_key_spell4_get(g)));
    wid_set_on_mouse_down(w, wid_cfg_key_spell4);
  }

  ///////////////////////////////////////////////////////////////////////
  // spell5
  ///////////////////////////////////////////////////////////////////////
  y_at++;
  {
    TRACE();
    auto *p = wid_cfg_help_window->wid_text_area->wid_text_area;
    auto *w = wid_new_square_button(g, p, "key_spell5");

    spoint const tl(0, y_at);
    spoint const br(menu_width / 2, y_at);
    wid_set_shape_none(w);
    wid_set_pos(w, tl, br);
    wid_set_text_lhs(w);
    wid_set_text(w, "Cast spell 5");
  }
  {
    TRACE();
    auto *p = wid_cfg_help_window->wid_text_area->wid_text_area;
    auto *w = wid_new_bar_button(g, p, "value");

    spoint const tl(rhs_button_left, y_at);
    spoint const br(rhs_button_right, y_at);
    wid_set_text_rhs(w);
    wid_set_pos(w, tl, br);
    wid_set_text(w, ::to_string(game_key_spell5_get(g)));
    wid_set_on_mouse_down(w, wid_cfg_key_spell5);
  }

  ///////////////////////////////////////////////////////////////////////
  // spell6
  ///////////////////////////////////////////////////////////////////////
  y_at++;
  {
    TRACE();
    auto *p = wid_cfg_help_window->wid_text_area->wid_text_area;
    auto *w = wid_new_square_button(g, p, "key_spell6");

    spoint const tl(0, y_at);
    spoint const br(menu_width / 2, y_at);
    wid_set_shape_none(w);
    wid_set_pos(w, tl, br);
    wid_set_text_lhs(w);
    wid_set_text(w, "Cast spell 6");
  }
  {
    TRACE();
    auto *p = wid_cfg_help_window->wid_text_area->wid_text_area;
    auto *w = wid_new_bar_button(g, p, "value");

    spoint const tl(rhs_button_left, y_at);
    spoint const br(rhs_button_right, y_at);
    wid_set_text_rhs(w);
    wid_set_pos(w, tl, br);
    wid_set_text(w, ::to_string(game_key_spell6_get(g)));
    wid_set_on_mouse_down(w, wid_cfg_key_spell6);
  }

  ///////////////////////////////////////////////////////////////////////
  // spell7
  ///////////////////////////////////////////////////////////////////////
  y_at++;
  {
    TRACE();
    auto *p = wid_cfg_help_window->wid_text_area->wid_text_area;
    auto *w = wid_new_square_button(g, p, "key_spell7");

    spoint const tl(0, y_at);
    spoint const br(menu_width / 2, y_at);
    wid_set_shape_none(w);
    wid_set_pos(w, tl, br);
    wid_set_text_lhs(w);
    wid_set_text(w, "Cast spell 7");
  }
  {
    TRACE();
    auto *p = wid_cfg_help_window->wid_text_area->wid_text_area;
    auto *w = wid_new_bar_button(g, p, "value");

    spoint const tl(rhs_button_left, y_at);
    spoint const br(rhs_button_right, y_at);
    wid_set_text_rhs(w);
    wid_set_pos(w, tl, br);
    wid_set_text(w, ::to_string(game_key_spell7_get(g)));
    wid_set_on_mouse_down(w, wid_cfg_key_spell7);
  }

  ///////////////////////////////////////////////////////////////////////
  // spell8
  ///////////////////////////////////////////////////////////////////////
  y_at++;
  {
    TRACE();
    auto *p = wid_cfg_help_window->wid_text_area->wid_text_area;
    auto *w = wid_new_square_button(g, p, "key_spell8");

    spoint const tl(0, y_at);
    spoint const br(menu_width / 2, y_at);
    wid_set_shape_none(w);
    wid_set_pos(w, tl, br);
    wid_set_text_lhs(w);
    wid_set_text(w, "Cast spell 8");
  }
  {
    TRACE();
    auto *p = wid_cfg_help_window->wid_text_area->wid_text_area;
    auto *w = wid_new_bar_button(g, p, "value");

    spoint const tl(rhs_button_left, y_at);
    spoint const br(rhs_button_right, y_at);
    wid_set_text_rhs(w);
    wid_set_pos(w, tl, br);
    wid_set_text(w, ::to_string(game_key_spell8_get(g)));
    wid_set_on_mouse_down(w, wid_cfg_key_spell8);
  }

  ///////////////////////////////////////////////////////////////////////
  // spell9
  ///////////////////////////////////////////////////////////////////////
  y_at++;
  {
    TRACE();
    auto *p = wid_cfg_help_window->wid_text_area->wid_text_area;
    auto *w = wid_new_square_button(g, p, "key_spell9");

    spoint const tl(0, y_at);
    spoint const br(menu_width / 2, y_at);
    wid_set_shape_none(w);
    wid_set_pos(w, tl, br);
    wid_set_text_lhs(w);
    wid_set_text(w, "Cast spell 9");
  }
  {
    TRACE();
    auto *p = wid_cfg_help_window->wid_text_area->wid_text_area;
    auto *w = wid_new_bar_button(g, p, "value");

    spoint const tl(rhs_button_left, y_at);
    spoint const br(rhs_button_right, y_at);
    wid_set_text_rhs(w);
    wid_set_pos(w, tl, br);
    wid_set_text(w, ::to_string(game_key_spell9_get(g)));
    wid_set_on_mouse_down(w, wid_cfg_key_spell9);
  }

  ///////////////////////////////////////////////////////////////////////
  // spell10
  ///////////////////////////////////////////////////////////////////////
  y_at++;
  {
    TRACE();
    auto *p = wid_cfg_help_window->wid_text_area->wid_text_area;
    auto *w = wid_new_square_button(g, p, "key_spell10");

    spoint const tl(0, y_at);
    spoint const br(menu_width / 2, y_at);
    wid_set_shape_none(w);
    wid_set_pos(w, tl, br);
    wid_set_text_lhs(w);
    wid_set_text(w, "Cast spell 10");
  }
  {
    TRACE();
    auto *p = wid_cfg_help_window->wid_text_area->wid_text_area;
    auto *w = wid_new_bar_button(g, p, "value");

    spoint const tl(rhs_button_left, y_at);
    spoint const br(rhs_button_right, y_at);
    wid_set_text_rhs(w);
    wid_set_pos(w, tl, br);
    wid_set_text(w, ::to_string(game_key_spell10_get(g)));
    wid_set_on_mouse_down(w, wid_cfg_key_spell10);
  }

  ///////////////////////////////////////////////////////////////////////
  // spell11
  ///////////////////////////////////////////////////////////////////////
  y_at++;
  {
    TRACE();
    auto *p = wid_cfg_help_window->wid_text_area->wid_text_area;
    auto *w = wid_new_square_button(g, p, "key_spell11");

    spoint const tl(0, y_at);
    spoint const br(menu_width / 2, y_at);
    wid_set_shape_none(w);
    wid_set_pos(w, tl, br);
    wid_set_text_lhs(w);
    wid_set_text(w, "Cast spell 11");
  }
  {
    TRACE();
    auto *p = wid_cfg_help_window->wid_text_area->wid_text_area;
    auto *w = wid_new_bar_button(g, p, "value");

    spoint const tl(rhs_button_left, y_at);
    spoint const br(rhs_button_right, y_at);
    wid_set_text_rhs(w);
    wid_set_pos(w, tl, br);
    wid_set_text(w, ::to_string(game_key_spell11_get(g)));
    wid_set_on_mouse_down(w, wid_cfg_key_spell11);
  }

  ///////////////////////////////////////////////////////////////////////
  // spell12
  ///////////////////////////////////////////////////////////////////////
  y_at++;
  {
    TRACE();
    auto *p = wid_cfg_help_window->wid_text_area->wid_text_area;
    auto *w = wid_new_square_button(g, p, "key_spell12");

    spoint const tl(0, y_at);
    spoint const br(menu_width / 2, y_at);
    wid_set_shape_none(w);
    wid_set_pos(w, tl, br);
    wid_set_text_lhs(w);
    wid_set_text(w, "Cast spell 12");
  }
  {
    TRACE();
    auto *p = wid_cfg_help_window->wid_text_area->wid_text_area;
    auto *w = wid_new_bar_button(g, p, "value");

    spoint const tl(rhs_button_left, y_at);
    spoint const br(rhs_button_right, y_at);
    wid_set_text_rhs(w);
    wid_set_pos(w, tl, br);
    wid_set_text(w, ::to_string(game_key_spell12_get(g)));
    wid_set_on_mouse_down(w, wid_cfg_key_spell12);
  }

  ///////////////////////////////////////////////////////////////////////
  // spell13
  ///////////////////////////////////////////////////////////////////////
  y_at++;
  {
    TRACE();
    auto *p = wid_cfg_help_window->wid_text_area->wid_text_area;
    auto *w = wid_new_square_button(g, p, "key_spell13");

    spoint const tl(0, y_at);
    spoint const br(menu_width / 2, y_at);
    wid_set_shape_none(w);
    wid_set_pos(w, tl, br);
    wid_set_text_lhs(w);
    wid_set_text(w, "Cast spell 13");
  }
  {
    TRACE();
    auto *p = wid_cfg_help_window->wid_text_area->wid_text_area;
    auto *w = wid_new_bar_button(g, p, "value");

    spoint const tl(rhs_button_left, y_at);
    spoint const br(rhs_button_right, y_at);
    wid_set_text_rhs(w);
    wid_set_pos(w, tl, br);
    wid_set_text(w, ::to_string(game_key_spell13_get(g)));
    wid_set_on_mouse_down(w, wid_cfg_key_spell13);
  }

  ///////////////////////////////////////////////////////////////////////
  // spell14
  ///////////////////////////////////////////////////////////////////////
  y_at++;
  {
    TRACE();
    auto *p = wid_cfg_help_window->wid_text_area->wid_text_area;
    auto *w = wid_new_square_button(g, p, "key_spell14");

    spoint const tl(0, y_at);
    spoint const br(menu_width / 2, y_at);
    wid_set_shape_none(w);
    wid_set_pos(w, tl, br);
    wid_set_text_lhs(w);
    wid_set_text(w, "Cast spell 14");
  }
  {
    TRACE();
    auto *p = wid_cfg_help_window->wid_text_area->wid_text_area;
    auto *w = wid_new_bar_button(g, p, "value");

    spoint const tl(rhs_button_left, y_at);
    spoint const br(rhs_button_right, y_at);
    wid_set_text_rhs(w);
    wid_set_pos(w, tl, br);
    wid_set_text(w, ::to_string(game_key_spell14_get(g)));
    wid_set_on_mouse_down(w, wid_cfg_key_spell14);
  }

  ///////////////////////////////////////////////////////////////////////
  // spell15
  ///////////////////////////////////////////////////////////////////////
  y_at++;
  {
    TRACE();
    auto *p = wid_cfg_help_window->wid_text_area->wid_text_area;
    auto *w = wid_new_square_button(g, p, "key_spell15");

    spoint const tl(0, y_at);
    spoint const br(menu_width / 2, y_at);
    wid_set_shape_none(w);
    wid_set_pos(w, tl, br);
    wid_set_text_lhs(w);
    wid_set_text(w, "Cast spell 15");
  }
  {
    TRACE();
    auto *p = wid_cfg_help_window->wid_text_area->wid_text_area;
    auto *w = wid_new_bar_button(g, p, "value");

    spoint const tl(rhs_button_left, y_at);
    spoint const br(rhs_button_right, y_at);
    wid_set_text_rhs(w);
    wid_set_pos(w, tl, br);
    wid_set_text(w, ::to_string(game_key_spell15_get(g)));
    wid_set_on_mouse_down(w, wid_cfg_key_spell15);
  }

  ///////////////////////////////////////////////////////////////////////
  // spell16
  ///////////////////////////////////////////////////////////////////////
  y_at++;
  {
    TRACE();
    auto *p = wid_cfg_help_window->wid_text_area->wid_text_area;
    auto *w = wid_new_square_button(g, p, "key_spell16");

    spoint const tl(0, y_at);
    spoint const br(menu_width / 2, y_at);
    wid_set_shape_none(w);
    wid_set_pos(w, tl, br);
    wid_set_text_lhs(w);
    wid_set_text(w, "Cast spell 16");
  }
  {
    TRACE();
    auto *p = wid_cfg_help_window->wid_text_area->wid_text_area;
    auto *w = wid_new_bar_button(g, p, "value");

    spoint const tl(rhs_button_left, y_at);
    spoint const br(rhs_button_right, y_at);
    wid_set_text_rhs(w);
    wid_set_pos(w, tl, br);
    wid_set_text(w, ::to_string(game_key_spell16_get(g)));
    wid_set_on_mouse_down(w, wid_cfg_key_spell16);
  }

  ///////////////////////////////////////////////////////////////////////
  // spell17
  ///////////////////////////////////////////////////////////////////////
  y_at++;
  {
    TRACE();
    auto *p = wid_cfg_help_window->wid_text_area->wid_text_area;
    auto *w = wid_new_square_button(g, p, "key_spell17");

    spoint const tl(0, y_at);
    spoint const br(menu_width / 2, y_at);
    wid_set_shape_none(w);
    wid_set_pos(w, tl, br);
    wid_set_text_lhs(w);
    wid_set_text(w, "Cast spell 17");
  }
  {
    TRACE();
    auto *p = wid_cfg_help_window->wid_text_area->wid_text_area;
    auto *w = wid_new_bar_button(g, p, "value");

    spoint const tl(rhs_button_left, y_at);
    spoint const br(rhs_button_right, y_at);
    wid_set_text_rhs(w);
    wid_set_pos(w, tl, br);
    wid_set_text(w, ::to_string(game_key_spell17_get(g)));
    wid_set_on_mouse_down(w, wid_cfg_key_spell17);
  }

  ///////////////////////////////////////////////////////////////////////
  // spell18
  ///////////////////////////////////////////////////////////////////////
  y_at++;
  {
    TRACE();
    auto *p = wid_cfg_help_window->wid_text_area->wid_text_area;
    auto *w = wid_new_square_button(g, p, "key_spell18");

    spoint const tl(0, y_at);
    spoint const br(menu_width / 2, y_at);
    wid_set_shape_none(w);
    wid_set_pos(w, tl, br);
    wid_set_text_lhs(w);
    wid_set_text(w, "Cast spell 18");
  }
  {
    TRACE();
    auto *p = wid_cfg_help_window->wid_text_area->wid_text_area;
    auto *w = wid_new_bar_button(g, p, "value");

    spoint const tl(rhs_button_left, y_at);
    spoint const br(rhs_button_right, y_at);
    wid_set_text_rhs(w);
    wid_set_pos(w, tl, br);
    wid_set_text(w, ::to_string(game_key_spell18_get(g)));
    wid_set_on_mouse_down(w, wid_cfg_key_spell18);
  }

  ///////////////////////////////////////////////////////////////////////
  // spell19
  ///////////////////////////////////////////////////////////////////////
  y_at++;
  {
    TRACE();
    auto *p = wid_cfg_help_window->wid_text_area->wid_text_area;
    auto *w = wid_new_square_button(g, p, "key_spell19");

    spoint const tl(0, y_at);
    spoint const br(menu_width / 2, y_at);
    wid_set_shape_none(w);
    wid_set_pos(w, tl, br);
    wid_set_text_lhs(w);
    wid_set_text(w, "Cast spell 19");
  }
  {
    TRACE();
    auto *p = wid_cfg_help_window->wid_text_area->wid_text_area;
    auto *w = wid_new_bar_button(g, p, "value");

    spoint const tl(rhs_button_left, y_at);
    spoint const br(rhs_button_right, y_at);
    wid_set_text_rhs(w);
    wid_set_pos(w, tl, br);
    wid_set_text(w, ::to_string(game_key_spell19_get(g)));
    wid_set_on_mouse_down(w, wid_cfg_key_spell19);
  }

  ///////////////////////////////////////////////////////////////////////
  // spell20
  ///////////////////////////////////////////////////////////////////////
  y_at++;
  {
    TRACE();
    auto *p = wid_cfg_help_window->wid_text_area->wid_text_area;
    auto *w = wid_new_square_button(g, p, "key_spell20");

    spoint const tl(0, y_at);
    spoint const br(menu_width / 2, y_at);
    wid_set_shape_none(w);
    wid_set_pos(w, tl, br);
    wid_set_text_lhs(w);
    wid_set_text(w, "Cast spell 20");
  }
  {
    TRACE();
    auto *p = wid_cfg_help_window->wid_text_area->wid_text_area;
    auto *w = wid_new_bar_button(g, p, "value");

    spoint const tl(rhs_button_left, y_at);
    spoint const br(rhs_button_right, y_at);
    wid_set_text_rhs(w);
    wid_set_pos(w, tl, br);
    wid_set_text(w, ::to_string(game_key_spell20_get(g)));
    wid_set_on_mouse_down(w, wid_cfg_key_spell20);
  }

  y_at++;

  ///////////////////////////////////////////////////////////////////////
  // Zoom
  ///////////////////////////////////////////////////////////////////////
  y_at++;
  {
    TRACE();
    auto *p = wid_cfg_help_window->wid_text_area->wid_text_area;
    auto *w = wid_new_square_button(g, p, "Change zoom");

    spoint const tl(0, y_at);
    spoint const br(menu_width / 2, y_at);
    wid_set_shape_none(w);
    wid_set_pos(w, tl, br);
    wid_set_text_lhs(w);
    wid_set_text(w, "Change zoom");
  }
  {
    TRACE();
    auto *p = wid_cfg_help_window->wid_text_area->wid_text_area;
    auto *w = wid_new_bar_button(g, p, "Change zoom");

    spoint const tl(rhs_button_left, y_at);
    spoint const br(rhs_button_right, y_at);
    wid_set_text_rhs(w);
    wid_set_pos(w, tl, br);
    wid_set_text(w, ::to_string(game_key_zoom_get(g)));
    wid_set_on_mouse_down(w, wid_cfg_key_zoom);
  }

  ///////////////////////////////////////////////////////////////////////
  // screenshot
  ///////////////////////////////////////////////////////////////////////
  y_at++;
  {
    TRACE();
    auto *p = wid_cfg_help_window->wid_text_area->wid_text_area;
    auto *w = wid_new_square_button(g, p, "Take a screenshot");

    spoint const tl(0, y_at);
    spoint const br(menu_width / 2, y_at);
    wid_set_shape_none(w);
    wid_set_pos(w, tl, br);
    wid_set_text_lhs(w);
    wid_set_text(w, "Take a screenshot");
  }
  {
    TRACE();
    auto *p = wid_cfg_help_window->wid_text_area->wid_text_area;
    auto *w = wid_new_bar_button(g, p, "value");

    spoint const tl(rhs_button_left, y_at);
    spoint const br(rhs_button_right, y_at);
    wid_set_text_rhs(w);
    wid_set_pos(w, tl, br);
    wid_set_text(w, ::to_string(game_key_screenshot_get(g)));
    wid_set_on_mouse_down(w, wid_cfg_key_screenshot);
  }

  ///////////////////////////////////////////////////////////////////////
  y_at++;
  ///////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////
  // quit
  ///////////////////////////////////////////////////////////////////////
  y_at++;
  {
    TRACE();
    auto *p = wid_cfg_help_window->wid_text_area->wid_text_area;
    auto *w = wid_new_square_button(g, p, "quit");

    spoint const tl(0, y_at);
    spoint const br(menu_width / 2, y_at);
    wid_set_shape_none(w);
    wid_set_pos(w, tl, br);
    wid_set_text_lhs(w);
    wid_set_text(w, "Quit");
  }
  {
    TRACE();
    auto *p = wid_cfg_help_window->wid_text_area->wid_text_area;
    auto *w = wid_new_bar_button(g, p, "value");

    spoint const tl(rhs_button_left, y_at);
    spoint const br(rhs_button_right, y_at);
    wid_set_text_rhs(w);
    wid_set_pos(w, tl, br);
    wid_set_text(w, ::to_string(game_key_quit_get(g)));
    wid_set_on_mouse_down(w, wid_cfg_key_quit);
  }
  ///////////////////////////////////////////////////////////////////////
  // console
  ///////////////////////////////////////////////////////////////////////
  y_at++;
  {
    TRACE();
    auto *p = wid_cfg_help_window->wid_text_area->wid_text_area;
    auto *w = wid_new_square_button(g, p, "console");

    spoint const tl(0, y_at);
    spoint const br(menu_width / 2, y_at);
    wid_set_shape_none(w);
    wid_set_pos(w, tl, br);
    wid_set_text_lhs(w);
    wid_set_text(w, "Debug console");
  }
  {
    TRACE();
    auto *p = wid_cfg_help_window->wid_text_area->wid_text_area;
    auto *w = wid_new_bar_button(g, p, "console");

    spoint const tl(rhs_button_left, y_at);
    spoint const br(rhs_button_right, y_at);
    wid_set_text_rhs(w);
    wid_set_pos(w, tl, br);
    wid_set_text(w, ::to_string(game_key_console_get(g)));
    wid_set_on_mouse_down(w, wid_cfg_key_console);
  }
  ///////////////////////////////////////////////////////////////////////
  // help
  ///////////////////////////////////////////////////////////////////////
  y_at++;
  {
    TRACE();
    auto *p = wid_cfg_help_window->wid_text_area->wid_text_area;
    auto *w = wid_new_square_button(g, p, "help");

    spoint const tl(0, y_at);
    spoint const br(menu_width / 2, y_at);
    wid_set_shape_none(w);
    wid_set_pos(w, tl, br);
    wid_set_text_lhs(w);
    wid_set_text(w, "This useless help");
  }
  {
    TRACE();
    auto *p = wid_cfg_help_window->wid_text_area->wid_text_area;
    auto *w = wid_new_bar_button(g, p, "value");

    spoint const tl(rhs_button_left, y_at);
    spoint const br(rhs_button_right, y_at);
    wid_set_text_rhs(w);
    wid_set_pos(w, tl, br);
    wid_set_text(w, ::to_string(game_key_help_get(g)));
    wid_set_on_mouse_down(w, wid_cfg_key_help);
  }

  y_at++;
  ///////////////////////////////////////////////////////////////////////
  // unused1
  ///////////////////////////////////////////////////////////////////////
  y_at++;
  {
    TRACE();
    auto *p = wid_cfg_help_window->wid_text_area->wid_text_area;
    auto *w = wid_new_square_button(g, p, "key_unused1");

    spoint const tl(0, y_at);
    spoint const br(menu_width / 2, y_at);
    wid_set_shape_none(w);
    wid_set_pos(w, tl, br);
    wid_set_text_lhs(w);
    wid_set_text(w, "key_unused1");
  }
  {
    TRACE();
    auto *p = wid_cfg_help_window->wid_text_area->wid_text_area;
    auto *w = wid_new_bar_button(g, p, "value");

    spoint const tl(rhs_button_left, y_at);
    spoint const br(rhs_button_right, y_at);
    wid_set_text_rhs(w);
    wid_set_pos(w, tl, br);
    wid_set_text(w, ::to_string(game_key_unused1_get(g)));
    wid_set_on_mouse_down(w, wid_cfg_key_unused1);
  }
  ///////////////////////////////////////////////////////////////////////
  // unused2
  ///////////////////////////////////////////////////////////////////////
  y_at++;
  {
    TRACE();
    auto *p = wid_cfg_help_window->wid_text_area->wid_text_area;
    auto *w = wid_new_square_button(g, p, "key_unused2");

    spoint const tl(0, y_at);
    spoint const br(menu_width / 2, y_at);
    wid_set_shape_none(w);
    wid_set_pos(w, tl, br);
    wid_set_text_lhs(w);
    wid_set_text(w, "key_unused2");
  }
  {
    TRACE();
    auto *p = wid_cfg_help_window->wid_text_area->wid_text_area;
    auto *w = wid_new_bar_button(g, p, "value");

    spoint const tl(rhs_button_left, y_at);
    spoint const br(rhs_button_right, y_at);
    wid_set_text_rhs(w);
    wid_set_pos(w, tl, br);
    wid_set_text(w, ::to_string(game_key_unused2_get(g)));
    wid_set_on_mouse_down(w, wid_cfg_key_unused2);
  }
  ///////////////////////////////////////////////////////////////////////
  // unused3
  ///////////////////////////////////////////////////////////////////////
  y_at++;
  {
    TRACE();
    auto *p = wid_cfg_help_window->wid_text_area->wid_text_area;
    auto *w = wid_new_square_button(g, p, "key_unused3");

    spoint const tl(0, y_at);
    spoint const br(menu_width / 2, y_at);
    wid_set_shape_none(w);
    wid_set_pos(w, tl, br);
    wid_set_text_lhs(w);
    wid_set_text(w, "key_unused3");
  }
  {
    TRACE();
    auto *p = wid_cfg_help_window->wid_text_area->wid_text_area;
    auto *w = wid_new_bar_button(g, p, "value");

    spoint const tl(rhs_button_left, y_at);
    spoint const br(rhs_button_right, y_at);
    wid_set_text_rhs(w);
    wid_set_pos(w, tl, br);
    wid_set_text(w, ::to_string(game_key_unused3_get(g)));
    wid_set_on_mouse_down(w, wid_cfg_key_unused3);
  }
  ///////////////////////////////////////////////////////////////////////
  // unused4
  ///////////////////////////////////////////////////////////////////////
  y_at++;
  {
    TRACE();
    auto *p = wid_cfg_help_window->wid_text_area->wid_text_area;
    auto *w = wid_new_square_button(g, p, "key_unused4");

    spoint const tl(0, y_at);
    spoint const br(menu_width / 2, y_at);
    wid_set_shape_none(w);
    wid_set_pos(w, tl, br);
    wid_set_text_lhs(w);
    wid_set_text(w, "key_unused4");
  }
  {
    TRACE();
    auto *p = wid_cfg_help_window->wid_text_area->wid_text_area;
    auto *w = wid_new_bar_button(g, p, "value");

    spoint const tl(rhs_button_left, y_at);
    spoint const br(rhs_button_right, y_at);
    wid_set_text_rhs(w);
    wid_set_pos(w, tl, br);
    wid_set_text(w, ::to_string(game_key_unused4_get(g)));
    wid_set_on_mouse_down(w, wid_cfg_key_unused4);
  }
  ///////////////////////////////////////////////////////////////////////
  // unused5
  ///////////////////////////////////////////////////////////////////////
  y_at++;
  {
    TRACE();
    auto *p = wid_cfg_help_window->wid_text_area->wid_text_area;
    auto *w = wid_new_square_button(g, p, "key_unused5");

    spoint const tl(0, y_at);
    spoint const br(menu_width / 2, y_at);
    wid_set_shape_none(w);
    wid_set_pos(w, tl, br);
    wid_set_text_lhs(w);
    wid_set_text(w, "key_unused5");
  }
  {
    TRACE();
    auto *p = wid_cfg_help_window->wid_text_area->wid_text_area;
    auto *w = wid_new_bar_button(g, p, "value");

    spoint const tl(rhs_button_left, y_at);
    spoint const br(rhs_button_right, y_at);
    wid_set_text_rhs(w);
    wid_set_pos(w, tl, br);
    wid_set_text(w, ::to_string(game_key_unused5_get(g)));
    wid_set_on_mouse_down(w, wid_cfg_key_unused5);
  }
  ///////////////////////////////////////////////////////////////////////
  // unused6
  ///////////////////////////////////////////////////////////////////////
  y_at++;
  {
    TRACE();
    auto *p = wid_cfg_help_window->wid_text_area->wid_text_area;
    auto *w = wid_new_square_button(g, p, "key_unused6");

    spoint const tl(0, y_at);
    spoint const br(menu_width / 2, y_at);
    wid_set_shape_none(w);
    wid_set_pos(w, tl, br);
    wid_set_text_lhs(w);
    wid_set_text(w, "key_unused6");
  }
  {
    TRACE();
    auto *p = wid_cfg_help_window->wid_text_area->wid_text_area;
    auto *w = wid_new_bar_button(g, p, "value");

    spoint const tl(rhs_button_left, y_at);
    spoint const br(rhs_button_right, y_at);
    wid_set_text_rhs(w);
    wid_set_pos(w, tl, br);
    wid_set_text(w, ::to_string(game_key_unused6_get(g)));
    wid_set_on_mouse_down(w, wid_cfg_key_unused6);
  }
  ///////////////////////////////////////////////////////////////////////
  // unused7
  ///////////////////////////////////////////////////////////////////////
  y_at++;
  {
    TRACE();
    auto *p = wid_cfg_help_window->wid_text_area->wid_text_area;
    auto *w = wid_new_square_button(g, p, "key_unused7");

    spoint const tl(0, y_at);
    spoint const br(menu_width / 2, y_at);
    wid_set_shape_none(w);
    wid_set_pos(w, tl, br);
    wid_set_text_lhs(w);
    wid_set_text(w, "key_unused7");
  }
  {
    TRACE();
    auto *p = wid_cfg_help_window->wid_text_area->wid_text_area;
    auto *w = wid_new_bar_button(g, p, "value");

    spoint const tl(rhs_button_left, y_at);
    spoint const br(rhs_button_right, y_at);
    wid_set_text_rhs(w);
    wid_set_pos(w, tl, br);
    wid_set_text(w, ::to_string(game_key_unused7_get(g)));
    wid_set_on_mouse_down(w, wid_cfg_key_unused7);
  }
  ///////////////////////////////////////////////////////////////////////
  // unused8
  ///////////////////////////////////////////////////////////////////////
  y_at++;
  {
    TRACE();
    auto *p = wid_cfg_help_window->wid_text_area->wid_text_area;
    auto *w = wid_new_square_button(g, p, "key_unused8");

    spoint const tl(0, y_at);
    spoint const br(menu_width / 2, y_at);
    wid_set_shape_none(w);
    wid_set_pos(w, tl, br);
    wid_set_text_lhs(w);
    wid_set_text(w, "key_unused8");
  }
  {
    TRACE();
    auto *p = wid_cfg_help_window->wid_text_area->wid_text_area;
    auto *w = wid_new_bar_button(g, p, "value");

    spoint const tl(rhs_button_left, y_at);
    spoint const br(rhs_button_right, y_at);
    wid_set_text_rhs(w);
    wid_set_pos(w, tl, br);
    wid_set_text(w, ::to_string(game_key_unused8_get(g)));
    wid_set_on_mouse_down(w, wid_cfg_key_unused8);
  }
  ///////////////////////////////////////////////////////////////////////
  // unused9
  ///////////////////////////////////////////////////////////////////////
  y_at++;
  {
    TRACE();
    auto *p = wid_cfg_help_window->wid_text_area->wid_text_area;
    auto *w = wid_new_square_button(g, p, "key_unused9");

    spoint const tl(0, y_at);
    spoint const br(menu_width / 2, y_at);
    wid_set_shape_none(w);
    wid_set_pos(w, tl, br);
    wid_set_text_lhs(w);
    wid_set_text(w, "key_unused9");
  }
  {
    TRACE();
    auto *p = wid_cfg_help_window->wid_text_area->wid_text_area;
    auto *w = wid_new_bar_button(g, p, "value");

    spoint const tl(rhs_button_left, y_at);
    spoint const br(rhs_button_right, y_at);
    wid_set_text_rhs(w);
    wid_set_pos(w, tl, br);
    wid_set_text(w, ::to_string(game_key_unused9_get(g)));
    wid_set_on_mouse_down(w, wid_cfg_key_unused9);
  }
  ///////////////////////////////////////////////////////////////////////
  // unused10
  ///////////////////////////////////////////////////////////////////////
  y_at++;
  {
    TRACE();
    auto *p = wid_cfg_help_window->wid_text_area->wid_text_area;
    auto *w = wid_new_square_button(g, p, "key_unused10");

    spoint const tl(0, y_at);
    spoint const br(menu_width / 2, y_at);
    wid_set_shape_none(w);
    wid_set_pos(w, tl, br);
    wid_set_text_lhs(w);
    wid_set_text(w, "key_unused10");
  }
  {
    TRACE();
    auto *p = wid_cfg_help_window->wid_text_area->wid_text_area;
    auto *w = wid_new_bar_button(g, p, "value");

    spoint const tl(rhs_button_left, y_at);
    spoint const br(rhs_button_right, y_at);
    wid_set_text_rhs(w);
    wid_set_pos(w, tl, br);
    wid_set_text(w, ::to_string(game_key_unused10_get(g)));
    wid_set_on_mouse_down(w, wid_cfg_key_unused10);
  }
  ///////////////////////////////////////////////////////////////////////
  // unused11
  ///////////////////////////////////////////////////////////////////////
  y_at++;
  {
    TRACE();
    auto *p = wid_cfg_help_window->wid_text_area->wid_text_area;
    auto *w = wid_new_square_button(g, p, "key_unused11");

    spoint const tl(0, y_at);
    spoint const br(menu_width / 2, y_at);
    wid_set_shape_none(w);
    wid_set_pos(w, tl, br);
    wid_set_text_lhs(w);
    wid_set_text(w, "key_unused11");
  }
  {
    TRACE();
    auto *p = wid_cfg_help_window->wid_text_area->wid_text_area;
    auto *w = wid_new_bar_button(g, p, "value");

    spoint const tl(rhs_button_left, y_at);
    spoint const br(rhs_button_right, y_at);
    wid_set_text_rhs(w);
    wid_set_text_rhs(w);
    wid_set_pos(w, tl, br);
    wid_set_text(w, ::to_string(game_key_unused11_get(g)));
    wid_set_on_mouse_down(w, wid_cfg_key_unused11);
  }

  wid_update(g, wid_cfg_help_window->wid_text_area->wid_text_area);

  if (last_vert_scroll_offset != -1) {
    auto *w = wid_cfg_help_window->wid_text_area->wid_vert_scroll;
    wid_move_to_y_off(g, w, last_vert_scroll_offset);
  }

  game_state_change(g, STATE_KEYBOARD_MENU, "configure keys");
}
