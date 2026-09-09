//
// Copyright goblinhack@gmail.com
//

#ifndef MY_GAME_HPP
#define MY_GAME_HPP

#include "my_enum.hpp"
#include "my_spoint.hpp"
#include "my_types.hpp"

#include <SDL.h>
#include <string>

#define GAME_STATE_ENUM(list_macro)                                                                                                             \
  CLANG_FORMAT_INDENT()                                           /* dummy line for clang indentation fixup */                                  \
  list_macro(STATE_INIT, "INIT"),                                 /* newline */                                                                 \
      list_macro(STATE_MAIN_MENU, "MAIN-MENU"),                   /* newline */                                                                 \
      list_macro(STATE_GENERATING, "GENERATING"),                 /* newline */                                                                 \
      list_macro(STATE_GENERATED, "GENERATED"),                   /* newline */                                                                 \
      list_macro(STATE_PLAYING, "PLAYING"),                       /* newline */                                                                 \
      list_macro(STATE_LEVEL_SELECT_MENU, "LEVEL-SELECT-MENU"),   /* newline */                                                                 \
      list_macro(STATE_PLAYER_SELECT_MENU, "PLAYER-SELECT-MENU"), /* newline */                                                                 \
      list_macro(STATE_MOVE_WARNING_MENU, "MOVE-WARNING-MENU"),   /* newline */                                                                 \
      list_macro(STATE_QUITTING, "QUITTING"),                     /* newline */                                                                 \
      list_macro(STATE_KEYBOARD_MENU, "KEYBOARD-MENU"),           /* newline */                                                                 \
      list_macro(STATE_LOAD_MENU, "LOAD-MENU"),                   /* newline */                                                                 \
      list_macro(STATE_LOADED, "LOADED"),                         /* newline */                                                                 \
      list_macro(STATE_INVENTORY_MENU, "INVENTORY-MENU"),         /* newline */                                                                 \
      list_macro(STATE_COLLECT_MENU, "COLLECT-MENU"),             /* newline */                                                                 \
      list_macro(STATE_THROW_MENU, "THROW-MENU"),                 /* newline */                                                                 \
      list_macro(STATE_THROW_ITEM, "THROW-ITEM"),                 /* newline */                                                                 \
      list_macro(STATE_ITEM_MENU, "ITEM-MENU"),                   /* newline */                                                                 \
      list_macro(STATE_DEAD_MENU, "DEAD-MENU"),                   /* newline */                                                                 \
      list_macro(STATE_GAME_OVER_MENU, "GAME-OVER-MENU"),         /* newline */                                                                 \
      list_macro(STATE_SAVE_MENU, "SAVE-MENU"),                   /* newline */                                                                 \
      list_macro(STATE_QUIT_MENU, "QUIT-MENU"),                   /* newline */

ENUM_DEF_H(GAME_STATE_ENUM, GameStateType)

using SeedSource = enum SeedSource_ {
  SEED_SOURCE_RANDOM,
  SEED_SOURCE_USER,
  SEED_SOURCE_TEST,
  SEED_SOURCE_COMMAND_LINE,
};

enum {
  CONFIG_MAGIC_1 = 0xaabbccdd,
  CONFIG_MAGIC_2 = 0xddccbbaa,
  CONFIG_MAGIC_3 = 0x11223344,
  CONFIG_MAGIC_4 = 0x44332211,
  CONFIG_MAGIC_5 = 0xcafecafe,
  CONFIG_MAGIC_6 = 0xfeedface
};

[[nodiscard]] auto game_hiscores_get(Gamep g) -> class HiScores *;
[[nodiscard]] auto game_hiscore_get(Gamep g) -> uint32_t;

auto game_visible_map_pix_get(Gamep g, int *visible_map_tl_x, int *visible_map_tl_y, int *visible_map_br_x, int *visible_map_br_y) -> void;
auto game_visible_map_pix_set(Gamep g, int visible_map_tl_x, int visible_map_tl_y, int visible_map_br_x, int visible_map_br_y) -> void;

[[nodiscard]] auto game_tiles_visible_across_get(Gamep g) -> int;
auto               game_tiles_visible_across_set(Gamep g, int val) -> void;

[[nodiscard]] auto game_tiles_visible_down_get(Gamep g) -> int;
auto               game_tiles_visible_down_set(Gamep g, int val) -> void;

[[nodiscard]] auto game_last_mouse_down_get(Gamep g) -> int;
auto               game_last_mouse_down_set(Gamep g, int val) -> void;

[[nodiscard]] auto game_aspect_ratio_get(Gamep g) -> float;
auto               game_aspect_ratio_set(Gamep g, float val) -> void;

[[nodiscard]] auto game_ui_term_height_get(Gamep g) -> int;
auto               game_ui_term_height_set(Gamep g, int val) -> void;

[[nodiscard]] auto game_ui_term_width_get(Gamep g) -> int;
auto               game_ui_term_width_set(Gamep g, int val) -> void;

[[nodiscard]] auto game_debug_mode_get(Gamep g) -> bool;
auto               game_debug_mode_set(Gamep g, bool val) -> void;

[[nodiscard]] auto game_fps_counter_get(Gamep g) -> bool;
auto               game_fps_counter_set(Gamep g) -> void;
auto               game_fps_counter_unset(Gamep g) -> void;

[[nodiscard]] auto game_fps_value_get(Gamep g) -> int;
auto               game_fps_value_set(Gamep g, int val) -> void;

[[nodiscard]] auto game_gfx_borderless_get(Gamep g) -> bool;
auto               game_gfx_borderless_set(Gamep g) -> void;
auto               game_gfx_borderless_unset(Gamep g) -> void;

[[nodiscard]] auto game_gfx_fullscreen_get(Gamep g) -> bool;
auto               game_gfx_fullscreen_set(Gamep g) -> void;
auto               game_gfx_fullscreen_unset(Gamep g) -> void;

[[nodiscard]] auto game_gfx_fullscreen_desktop_get(Gamep g) -> bool;
auto               game_gfx_fullscreen_desktop_set(Gamep g) -> void;
auto               game_gfx_fullscreen_desktop_unset(Gamep g) -> void;

[[nodiscard]] auto game_gfx_vsync_enable_get(Gamep g) -> bool;
auto               game_gfx_vsync_enable_set(Gamep g) -> void;
auto               game_gfx_vsync_enable_unset(Gamep g) -> void;

[[nodiscard]] auto game_mouse_wheel_lr_negated_get(Gamep g) -> bool;
auto               game_mouse_wheel_lr_negated_set(Gamep g) -> void;
auto               game_mouse_wheel_lr_negated_unset(Gamep g) -> void;

[[nodiscard]] auto game_mouse_wheel_ud_negated_get(Gamep g) -> bool;
auto               game_mouse_wheel_ud_negated_set(Gamep g) -> void;
auto               game_mouse_wheel_ud_negated_unset(Gamep g) -> void;

[[nodiscard]] auto game_config_font_get(Gamep g) -> int;
auto               game_config_font_set(Gamep g, int val) -> void;

[[nodiscard]] auto game_config_font_height_get(Gamep g) -> int;
auto               game_config_font_height_set(Gamep g, int val) -> void;

[[nodiscard]] auto game_config_font_width_get(Gamep g) -> int;
auto               game_config_font_width_set(Gamep g, int val) -> void;

[[nodiscard]] auto game_config_pix_height_get(Gamep g) -> int;
auto               game_config_pix_height_set(Gamep g, int val) -> void;

[[nodiscard]] auto game_config_pix_width_get(Gamep g) -> int;
auto               game_config_pix_width_set(Gamep g, int val) -> void;

[[nodiscard]] auto game_map_fbo_height_get(Gamep g) -> int;
auto               game_map_fbo_height_set(Gamep g, int val) -> void;

[[nodiscard]] auto game_map_fbo_width_get(Gamep g) -> int;
auto               game_map_fbo_width_set(Gamep g, int val) -> void;

[[nodiscard]] auto game_window_pix_height_get(Gamep g) -> int;
auto               game_window_pix_height_set(Gamep g, int val) -> void;

[[nodiscard]] auto game_window_pix_width_get(Gamep g) -> int;
auto               game_window_pix_width_set(Gamep g, int val) -> void;

[[nodiscard]] auto game_ascii_pix_height_get(Gamep g) -> int;
auto               game_ascii_pix_height_set(Gamep g, int val) -> void;

[[nodiscard]] auto game_ascii_pix_width_get(Gamep g) -> int;
auto               game_ascii_pix_width_set(Gamep g, int val) -> void;

[[nodiscard]] auto game_music_volume_get(Gamep g) -> int;
auto               game_music_volume_set(Gamep g, int val) -> void;

[[nodiscard]] auto game_sdl_delay_get(Gamep g) -> int;
auto               game_sdl_delay_set(Gamep g, int val) -> void;

[[nodiscard]] auto game_sound_volume_get(Gamep g) -> int;
auto               game_sound_volume_set(Gamep g, int val) -> void;

[[nodiscard]] auto game_difficulty_get(Gamep g) -> int;
auto               game_difficulty_set(Gamep g, int val) -> void;

[[nodiscard]] auto game_levels_get(Gamep g) -> Levelsp;
[[nodiscard]] auto game_levels_set(Gamep g, Levelsp val) -> Levelsp;
[[nodiscard]] auto game_level_populate(Gamep g, Levelsp v, LevelNum n) -> Levelp;

[[nodiscard]] auto game_seed_name_get(Gamep g) -> const char *;
[[nodiscard]] auto game_seed_source_get(Gamep g) -> SeedSource;
[[nodiscard]] auto game_seed_num_get(Gamep g) -> uint32_t;
auto               game_seed_set(Gamep g, const char *maybe_seed) -> void;
auto               game_seed_clear(Gamep g) -> void;
auto               game_seed_set(Gamep g, uint32_t seed) -> void;

[[nodiscard]] auto game_player_name_get(Gamep g) -> const char *;
auto               game_player_name_set(Gamep g, const char *player_name = nullptr) -> void;

[[nodiscard]] auto game_key_wait_get(Gamep g) -> SDL_Keysym;
auto               game_key_wait_set(Gamep g, SDL_Keysym key) -> void;

[[nodiscard]] auto game_key_console_get(Gamep g) -> SDL_Keysym;
auto               game_key_console_set(Gamep g, SDL_Keysym key) -> void;

[[nodiscard]] auto game_key_help_get(Gamep g) -> SDL_Keysym;
auto               game_key_help_set(Gamep g, SDL_Keysym key) -> void;

[[nodiscard]] auto game_key_load_get(Gamep g) -> SDL_Keysym;
auto               game_key_load_set(Gamep g, SDL_Keysym key) -> void;

[[nodiscard]] auto game_key_move_down_get(Gamep g) -> SDL_Keysym;
auto               game_key_move_down_set(Gamep g, SDL_Keysym key) -> void;

[[nodiscard]] auto game_key_move_left_get(Gamep g) -> SDL_Keysym;
auto               game_key_move_left_set(Gamep g, SDL_Keysym key) -> void;

[[nodiscard]] auto game_key_move_right_get(Gamep g) -> SDL_Keysym;
auto               game_key_move_right_set(Gamep g, SDL_Keysym key) -> void;

[[nodiscard]] auto game_key_move_up_get(Gamep g) -> SDL_Keysym;
auto               game_key_move_up_set(Gamep g, SDL_Keysym key) -> void;

[[nodiscard]] auto game_key_quit_get(Gamep g) -> SDL_Keysym;
auto               game_key_quit_set(Gamep g, SDL_Keysym key) -> void;

[[nodiscard]] auto game_key_save_get(Gamep g) -> SDL_Keysym;
auto               game_key_save_set(Gamep g, SDL_Keysym key) -> void;

[[nodiscard]] auto game_key_screenshot_get(Gamep g) -> SDL_Keysym;
auto               game_key_screenshot_set(Gamep g, SDL_Keysym key) -> void;

[[nodiscard]] auto game_key_unused1_get(Gamep g) -> SDL_Keysym;
auto               game_key_unused1_set(Gamep g, SDL_Keysym key) -> void;

[[nodiscard]] auto game_key_unused2_get(Gamep g) -> SDL_Keysym;
auto               game_key_unused2_set(Gamep g, SDL_Keysym key) -> void;

[[nodiscard]] auto game_key_unused3_get(Gamep g) -> SDL_Keysym;
auto               game_key_unused3_set(Gamep g, SDL_Keysym key) -> void;

[[nodiscard]] auto game_key_unused4_get(Gamep g) -> SDL_Keysym;
auto               game_key_unused4_set(Gamep g, SDL_Keysym key) -> void;

[[nodiscard]] auto game_key_unused5_get(Gamep g) -> SDL_Keysym;
auto               game_key_unused5_set(Gamep g, SDL_Keysym key) -> void;

[[nodiscard]] auto game_key_unused6_get(Gamep g) -> SDL_Keysym;
auto               game_key_unused6_set(Gamep g, SDL_Keysym key) -> void;

[[nodiscard]] auto game_key_unused7_get(Gamep g) -> SDL_Keysym;
auto               game_key_unused7_set(Gamep g, SDL_Keysym key) -> void;

[[nodiscard]] auto game_key_unused8_get(Gamep g) -> SDL_Keysym;
auto               game_key_unused8_set(Gamep g, SDL_Keysym key) -> void;

[[nodiscard]] auto game_key_unused9_get(Gamep g) -> SDL_Keysym;
auto               game_key_unused9_set(Gamep g, SDL_Keysym key) -> void;

[[nodiscard]] auto game_key_unused10_get(Gamep g) -> SDL_Keysym;
auto               game_key_unused10_set(Gamep g, SDL_Keysym key) -> void;

[[nodiscard]] auto game_key_unused11_get(Gamep g) -> SDL_Keysym;
auto               game_key_unused11_set(Gamep g, SDL_Keysym key) -> void;

[[nodiscard]] auto game_key_unused12_get(Gamep g) -> SDL_Keysym;
auto               game_key_unused12_set(Gamep g, SDL_Keysym key) -> void;

[[nodiscard]] auto game_key_abort_get(Gamep g) -> SDL_Keysym;
auto               game_key_abort_set(Gamep g, SDL_Keysym key) -> void;

[[nodiscard]] auto game_key_throw_get(Gamep g) -> SDL_Keysym;
auto               game_key_throw_set(Gamep g, SDL_Keysym key) -> void;

[[nodiscard]] auto game_key_fire_get(Gamep g) -> SDL_Keysym;
auto               game_key_fire_set(Gamep g, SDL_Keysym key) -> void;

[[nodiscard]] auto game_key_inventory_get(Gamep g) -> SDL_Keysym;
auto               game_key_inventory_set(Gamep g, SDL_Keysym key) -> void;

[[nodiscard]] auto game_key_jump_get(Gamep g) -> SDL_Keysym;
auto               game_key_jump_set(Gamep g, SDL_Keysym key) -> void;

[[nodiscard]] auto game_key_ascend_get(Gamep g) -> SDL_Keysym;
auto               game_key_ascend_set(Gamep g, SDL_Keysym key) -> void;

[[nodiscard]] auto game_key_descend_get(Gamep g) -> SDL_Keysym;
auto               game_key_descend_set(Gamep g, SDL_Keysym key) -> void;

[[nodiscard]] auto game_key_zoom_get(Gamep g) -> SDL_Keysym;
auto               game_key_zoom_set(Gamep g, SDL_Keysym key) -> void;

[[nodiscard]] auto game_request_to_remake_ui_get(Gamep g) -> bool;
auto               game_request_to_remake_ui_set(Gamep g) -> void;
auto               game_request_to_remake_ui_unset(Gamep g) -> void;

[[nodiscard]] auto game_request_to_save_game_get(Gamep g) -> bool;
auto               game_request_to_save_game_set(Gamep g) -> void;
auto               game_request_to_save_game_unset(Gamep g) -> void;

[[nodiscard]] auto game_request_to_update_cursor_get(Gamep g) -> bool;
auto               game_request_to_update_cursor_set(Gamep g) -> void;
auto               game_request_to_update_cursor_unset(Gamep g) -> void;

[[nodiscard]] auto game_request_reached_exit_get(Gamep g) -> bool;
auto               game_request_reached_exit_set(Gamep g) -> void;
auto               game_request_reached_exit_unset(Gamep g) -> void;

[[nodiscard]] auto game_request_reached_entrance_get(Gamep g) -> bool;
auto               game_request_reached_entrance_set(Gamep g) -> void;
auto               game_request_reached_entrance_unset(Gamep g) -> void;

[[nodiscard]] auto game_request_to_end_game_get(Gamep g) -> bool;
auto               game_request_to_end_game_set(Gamep g) -> void;
auto               game_request_to_end_game_unset(Gamep g) -> void;

[[nodiscard]] auto game_request_to_end_game_reason_get(Gamep g) -> std::string;
auto               game_request_to_end_game_reason_set(Gamep g, const std::string &val) -> void;

[[nodiscard]] auto game_chosen_player_get(Gamep g) -> Tpp;
void               game_chosen_player_set(Gamep g, Tpp t);

[[nodiscard]] auto game_chosen_sacrifice_get(Gamep g) -> Tpp;
void               game_chosen_sacrifice_set(Gamep g, Tpp t);

[[nodiscard]] auto game_mouse_down_player_get(Gamep g) -> Thingp;
void               game_mouse_down_player_set(Gamep g, Thingp t);

[[nodiscard]] auto game_mouse_down_sacrifice_get(Gamep g) -> Thingp;
void               game_mouse_down_sacrifice_set(Gamep g, Thingp t);

[[nodiscard]] auto game_mouse_over_player_get(Gamep g) -> Thingp;
void               game_mouse_over_player_set(Gamep g, Thingp t);

[[nodiscard]] auto game_mouse_over_sacrifice_get(Gamep g) -> Thingp;
void               game_mouse_over_sacrifice_set(Gamep g, Thingp t);

[[nodiscard]] auto game_is_new_highest_hiscore(Gamep g, uint32_t score) -> bool;
[[nodiscard]] auto game_is_new_hiscore(Gamep g, uint32_t score) -> bool;
[[nodiscard]] auto game_wait_for_tick_to_finish(Gamep g, Levelsp v, Levelp l) -> bool;
[[nodiscard]] auto game_event_ascend(Gamep g) -> bool;
[[nodiscard]] auto game_event_descend(Gamep g) -> bool;
[[nodiscard]] auto game_event_help(Gamep g) -> bool;
[[nodiscard]] auto game_event_inventory(Gamep g) -> bool;
[[nodiscard]] auto game_event_load(Gamep g) -> bool;
[[nodiscard]] auto game_event_quit(Gamep g) -> bool;
[[nodiscard]] auto game_event_save(Gamep g) -> bool;
[[nodiscard]] auto game_event_wait(Gamep g) -> bool;
[[nodiscard]] auto game_input(Gamep g, const SDL_Keysym *key) -> bool;
[[nodiscard]] auto game_load_config(Gamep game) -> bool;
[[nodiscard]] auto game_load_last_config(const char *appdata) -> bool;
[[nodiscard]] auto game_load_snapshot(Gamep) -> bool;
[[nodiscard]] auto game_load(Gamep g, const std::string &file_to_load) -> bool;
[[nodiscard]] auto game_map_zoom_is_full_map_visible(Gamep g) -> bool;
[[nodiscard]] auto game_mouse_down(Gamep g, int x, int y, uint32_t button) -> bool;
[[nodiscard]] auto game_mouse_motion(Gamep g, int x, int y, int relx, int rely, int wheelx, int wheely) -> bool;
[[nodiscard]] auto game_mouse_up(Gamep g, int x, int y, uint32_t button) -> bool;
[[nodiscard]] auto game_save(Gamep g, const std::string &file_to_save) -> bool;
[[nodiscard]] auto game_place_str(Gamep g, uint32_t score) -> const char *;
[[nodiscard]] auto game_state(Gamep g) -> GameStateType;
[[nodiscard]] auto game_map_single_pix_size_get(Gamep g) -> int;
[[nodiscard]] auto game_map_zoom_def_get(Gamep g) -> int;
[[nodiscard]] auto game_map_zoom_get(Gamep g) -> int;
[[nodiscard]] auto game_tick_get(Gamep g, Levelsp v) -> uint32_t;

auto game_cleanup(Gamep g) -> void;
auto game_config_reset(Gamep g) -> void;
auto game_create_levels(Gamep g) -> void;
auto game_destroy_levels(Gamep g) -> void;
auto game_display(Gamep g) -> void;
auto game_fini(Gamep g) -> void;
auto game_init(Gamep g) -> void;
auto game_map_single_pix_size_set(Gamep g, int val) -> void;
auto game_map_zoom_in(Gamep g) -> void;
auto game_map_zoom_out(Gamep g) -> void;
auto game_map_zoom_set(Gamep g, int val) -> void;
auto game_map_zoom_toggle(Gamep g) -> void;
auto game_save_config(Gamep g) -> void;
auto game_set_currently_saving_snapshot(Gamep) -> void;
auto game_start_playing(Gamep g) -> void;
auto game_state_change(Gamep g, GameStateType state, const char *why) -> void;
auto game_state_reset(Gamep g, const char *why) -> void;
auto game_tick(Gamep g) -> void;
auto game_unset_currently_saving_snapshot(Gamep) -> void;
auto game_unset_request_reset_state_change(Gamep) -> void;
auto game_unset_request_to_save_snapshot(Gamep) -> void;
auto game_unset_request_to_update_same_level(Gamep) -> void;
auto game_add_new_hiscore(Gamep g, uint32_t score, LevelNum level_num, const char *name, const char *reason) -> void;

#endif
