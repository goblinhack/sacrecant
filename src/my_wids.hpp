//
// Copyright goblinhack@gmail.com
//

#ifndef MY_WIDS_HPP
#define MY_WIDS_HPP

#include "my_color_defs.hpp"
#include "my_thing.hpp"
#include "my_wid.hpp"
#include "my_wid_popup.hpp"

#include <string>
#include <vector>

auto wid_actionbar_fini(Gamep g) -> void;
auto wid_botcon_fini(Gamep g) -> void;
auto wid_botcon_flush(Gamep g) -> void;
auto wid_botcon_log(const std::string &s) -> void;
auto wid_cfg_display_select(Gamep g, bool menu_was_created_due_to_game_restarting = false) -> void;
auto wid_cfg_help_select(Gamep g) -> void;
auto wid_cfg_mouse_select(Gamep g) -> void;
void wid_spellbook(Gamep g, Levelsp v, Levelp l, Thingp player, ThingStatType filter = THING_STAT_NONE);
void wid_transition_select(Gamep g);
auto wid_cfg_sound_select(Gamep g) -> void;
auto wid_console_deserialize(const std::vector< std::string > &r) -> void;
auto wid_console_fini(Gamep g) -> void;
auto wid_console_flush(Gamep g) -> void;
auto wid_console_clear(Gamep g) -> void;
auto wid_console_log(const std::string &s) -> void;
auto wid_console_raise(Gamep g) -> void;
auto wid_credits_select(Gamep g) -> void;
auto wid_dead_fini(Gamep g) -> void;
auto wid_dead_select(Gamep g, const std::string &reason) -> void;
auto wid_error(Gamep g, const std::string &error) -> void;
auto wid_game_over_select(Gamep g) -> void;
auto wid_gray_out_button(Gamep g, Widp w) -> void;
auto wid_hiscores_show(Gamep g) -> void;
auto wid_intro1_select(Gamep g) -> void;
auto wid_intro2_select(Gamep g) -> void;
auto wid_level_select(Gamep g, Levelsp v, Levelp l) -> void;
auto wid_player_select(Gamep g) -> void;
auto wid_spell_learn(Gamep g, Levelsp v, Levelp l, Thingp player, ThingStatType filter = THING_STAT_NONE) -> void;
auto wid_inventory_mouse_over_begin(Gamep g, Widp w, int relx, int rely, int wheelx, int wheely) -> void;
auto wid_inventory_mouse_over_end(Gamep g, Widp w) -> void;
auto wid_inventory_show(Gamep g, Levelsp v, Levelp l, Thingp player) -> void;
auto wid_collect_show(Gamep g, Levelsp v, Levelp l, Thingp player, std::vector< Thingp > /*items_in*/) -> void;
auto wid_throw_item_show(Gamep g, Levelsp v, Levelp l, Thingp player, std::vector< Thingp > /*items_in*/) -> void;
auto wid_item_menu_select(Gamep g, Levelsp v, Thingp it, bool from_inventory) -> void;
auto wid_leftbar_fini(Gamep g) -> void;
auto wid_load_destroy(Gamep g) -> void;
auto wid_options_menu_destroy(Gamep g) -> void;
auto wid_load_select(Gamep g) -> void;
auto wid_main_menu_destroy(Gamep g) -> void;
auto wid_main_menu_hide(Gamep g) -> void;
auto wid_main_menu_select(Gamep g) -> void;
auto wid_more_select(Gamep g) -> void;
auto wid_new_game(Gamep g) -> void;
auto wid_notice_destroy() -> void;
auto wid_notice(const std::string &s) -> void;
auto wid_options_menu_select(Gamep g) -> void;
auto wid_player_name_select(Gamep g) -> void;
auto wid_progress_bar_destroy(Gamep g) -> void;
auto wid_progress_bar(Gamep g, const std::string &title, float pct) -> void;
auto wid_quit_destroy(Gamep g) -> void;
auto wid_quit_select(Gamep g) -> void;
auto wid_rightbar_fini(Gamep g) -> void;
auto wid_save_destroy(Gamep g) -> void;
auto wid_save_select(Gamep g) -> void;
auto wid_seed_select(Gamep g) -> void;
auto wid_statistics_show(Gamep g, Levelsp v, Levelp l, Thingp player) -> void;
void wid_spell_options_show(Gamep g, Levelsp v, Levelp l, Thingp player, Thingp spell);
auto wid_topcon_fini(Gamep g) -> void;
auto wid_topcon_flush(Gamep g) -> void;
void wid_apply_bar_button(Gamep g, Widp w);
auto wid_level_select_destroy() -> void;
auto wid_topcon_log(const std::string &s) -> void;

[[nodiscard]] auto wid_actionbar_init(Gamep g) -> bool;
[[nodiscard]] auto wid_botcon_init(Gamep g) -> bool;
[[nodiscard]] auto wid_console_init(Gamep g) -> bool;
[[nodiscard]] auto wid_console_receive_input(Gamep g, Widp w, const SDL_Keysym *key) -> bool;
[[nodiscard]] auto wid_console_serialize() -> std::vector< std::string >;
[[nodiscard]] auto wid_inventory_mouse_up(Gamep g, Widp w, int x, int y, uint32_t button) -> bool;
[[nodiscard]] auto wid_leftbar_init(Gamep g) -> bool;
[[nodiscard]] auto wid_new_back_button(Gamep g, Widp parent, const std::string &name) -> Widp;
[[nodiscard]] auto wid_new_button(Gamep g, Widp parent, const std::string &name) -> Widp;
[[nodiscard]] auto wid_new_cancel_button(Gamep g, Widp parent, const std::string &name) -> Widp;
[[nodiscard]] auto wid_new_close_button(Gamep g, Widp parent, const std::string &name) -> Widp;
[[nodiscard]] auto wid_new_continue_button(Gamep g, Widp parent, const std::string &name) -> Widp;
[[nodiscard]] auto wid_new_green_button(Gamep g, Widp parent, const std::string &name) -> Widp;
[[nodiscard]] auto wid_new_bar_button(Gamep g, Widp parent, const std::string &name) -> Widp;
[[nodiscard]] auto wid_new_menu_button(Gamep g, Widp parent, const std::string &name) -> Widp;
[[nodiscard]] auto wid_new_bright_button(Gamep g, Widp parent, const std::string &name) -> Widp;
[[nodiscard]] auto wid_console_find_text(Gamep g, const std::string &in) -> bool;
[[nodiscard]] auto wid_new_red_button(Gamep g, Widp parent, const std::string &name) -> Widp;
[[nodiscard]] auto wid_new_save_button(Gamep g, Widp parent, const std::string &name) -> Widp;
[[nodiscard]] auto wid_rightbar_init(Gamep g) -> bool;
[[nodiscard]] auto wid_topcon_init(Gamep g) -> bool;
[[nodiscard]] auto wid_arcana_pyro_mouse_over_begin(Gamep g) -> WidPopup *;
[[nodiscard]] auto wid_arcana_geo_mouse_over_begin(Gamep g) -> WidPopup *;
[[nodiscard]] auto wid_arcana_bio_mouse_over_begin(Gamep g) -> WidPopup *;
[[nodiscard]] auto wid_arcana_necro_mouse_over_begin(Gamep g) -> WidPopup *;

//
// For buttons that pulse
//
enum { WID_PULSE_ANIM_MS = 1500 };

#endif
