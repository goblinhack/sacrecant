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

auto wid_actionbar_init(Gamep g) -> bool;
auto wid_leftbar_init(Gamep g) -> bool;
auto wid_rightbar_init(Gamep g) -> bool;
void wid_actionbar_fini(Gamep g);
void wid_cfg_gfx_select(Gamep g);
void wid_cfg_help_select(Gamep g);
void wid_cfg_mouse_select(Gamep g);
void wid_options_menu_select(Gamep g);
void wid_cfg_sound_select(Gamep g);
void wid_credits_select(Gamep g);
void wid_intro_select(Gamep g);
void wid_game_over_select(Gamep g);
void wid_dead_fini(Gamep g);
void wid_dead_select(Gamep g, const std::string &reason);
void wid_hiscores_show(Gamep g);
void wid_inventory_show(Gamep g, Levelsp v, Levelp l, Thingp player);
void wid_leftbar_fini(Gamep g);
void wid_load_destroy(Gamep g);
void wid_load_select(Gamep g);
void wid_main_menu_destroy(Gamep g);
void wid_main_menu_select(Gamep g);
void wid_more_select(Gamep g);
void wid_item_menu_select(Gamep g, Levelsp v, Thingp it);
void wid_new_game(Gamep g);
void wid_player_name_select(Gamep g);
void wid_quit_destroy(Gamep g);
void wid_quit_select(Gamep g);
void wid_rightbar_fini(Gamep g);
void wid_save_destroy(Gamep g);
void wid_save_select(Gamep g);
void wid_seed_select(Gamep g);
void wid_gray_out_button(Gamep g, Widp w);
auto wid_new_back_button(Gamep g, Widp parent, const std::string &name) -> Widp;
auto wid_new_continue_button(Gamep g, Widp parent, const std::string &name) -> Widp;
auto wid_new_menu_button(Gamep g, Widp parent, const std::string &name) -> Widp;
auto wid_new_save_button(Gamep g, Widp parent, const std::string &name) -> Widp;
auto wid_new_cancel_button(Gamep g, Widp parent, const std::string &name) -> Widp;
auto wid_new_green_button(Gamep g, Widp parent, const std::string &name) -> Widp;
auto wid_new_red_button(Gamep g, Widp parent, const std::string &name) -> Widp;
auto wid_new_button(Gamep g, Widp parent, const std::string &name) -> Widp;
void wid_botcon_fini(Gamep g);
auto wid_botcon_init(Gamep g) -> bool;
void wid_botcon_log(const std::string &s);
void wid_botcon_flush(Gamep g);
void wid_topcon_fini(Gamep g);
auto wid_topcon_init(Gamep g) -> bool;
void wid_topcon_log(const std::string &s);
void wid_topcon_flush(Gamep g);
void wid_notice(const std::string &s);
void wid_notice_destroy();
void wid_console_fini(Gamep g);
auto wid_console_init(Gamep g) -> bool;
void wid_console_log(const std::string &s);
void wid_console_raise(Gamep g);
void wid_console_flush(Gamep g);
void wid_progress_bar(Gamep /*g*/, const std::string &title, float pct);
void wid_progress_bar_destroy(Gamep /*g*/);
auto wid_console_receive_input(Gamep g, Widp w, const SDL_Keysym *key) -> bool;
void wid_console_deserialize(const std::vector< std::string > &r);
void wid_error(Gamep /*g*/, const std::string &error);

auto wid_console_serialize() -> std::vector< std::string >;

extern Widp wid_topcon_input_line;
extern Widp wid_topcon_window;
extern Widp wid_topcon_container;

extern Widp wid_botcon_input_line;
extern Widp wid_botcon_window;
extern Widp wid_botcon_container;

extern Widp wid_console_input_line;
extern Widp wid_console_window;
extern Widp wid_console_container;

#endif
