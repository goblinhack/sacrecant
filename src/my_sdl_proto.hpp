//
// Copyright goblinhack@gmail.com
//

#ifndef MY_SDL_PROTO_HPP
#define MY_SDL_PROTO_HPP

#include "my_gl.hpp"
#include "my_types.hpp"

#include "my_sdl_compat.hpp"
#include <string>
#include <vector>

[[nodiscard]] auto sdlk_eq(const SDL_Keysym &a, const SDL_Keysym &b) -> bool;
[[nodiscard]] auto sdlk_eq(const SDL_Scancode &s, const SDL_Keysym &k) -> bool;

[[nodiscard]] auto sdl_filter_events(void *userdata, SDL_Event *event) -> sdl_compat_event_filter_ret_t;
[[nodiscard]] auto sdl_mouse_read_position() -> int;

[[nodiscard]] auto sdlk_normalize(SDL_Keysym k) -> SDL_Keysym;
[[nodiscard]] auto sdlk_to_scancode(const SDL_Keysym &k) -> SDL_Scancode;

[[nodiscard]] auto to_string(const SDL_Keysym &k) -> std::string;
[[nodiscard]] auto to_string_ignoring_mods(const SDL_Keysym &k) -> std::string;

[[nodiscard]] auto sdl_fbo_save(Gamep g, FboEnum fbo) -> std::vector< uint8_t >;

[[nodiscard]] auto config_errored_clear(Gamep g, class Tokens *tokens, void *context) -> uint8_t;
[[nodiscard]] auto show_error(Gamep g, class Tokens *tokens, void *context) -> uint8_t;
[[nodiscard]] auto config_fps_counter_set(Gamep g, class Tokens *tokens, void *context) -> uint8_t;
[[nodiscard]] auto config_debug_set(Gamep g, class Tokens *tokens, void *context) -> uint8_t;
[[nodiscard]] auto config_gfx_vsync_enable(Gamep g, class Tokens *tokens, void *context) -> uint8_t;
[[nodiscard]] auto sdl_init() -> bool;

class Tokens;
[[nodiscard]] auto sdl_user_exit(Gamep g, class Tokens *tokens, void *context) -> uint8_t;

[[nodiscard]] auto sdl_display_init(Gamep g) -> bool;

auto config_game_gfx_update(Gamep g) -> void;
auto config_gfx_vsync_update(Gamep g) -> void;
auto sdl_config_update_all(Gamep g) -> void;
auto sdl_display(Gamep g) -> void;
auto sdl_display_reset(Gamep g) -> void;
auto sdl_event(Gamep g, SDL_Event *event, bool &processed_mouse_motion_event) -> void;
int  sdl_get_mouse_state(int *x, int *y);
void sdl_warp_mouse(int x, int y);
auto sdl_prepare_to_exit(Gamep g) -> void;
auto sdl_fbo_dump(Gamep g, FboEnum fbo, const std::string &name) -> void;
auto sdl_fbo_load(Gamep g, FboEnum fbo, const std::vector< uint8_t > &pixels) -> void;
auto sdl_fini(Gamep g) -> void;
auto sdl_display_fini(Gamep g) -> void;
auto sdl_flush_display(Gamep g, bool force = false) -> void;
auto sdl_joy_rumble(float strength, uint32_t ms) -> void;
auto sdl_key_repeat_events(Gamep g) -> void;
auto sdl_loop(Gamep g) -> void;
void sdl_get_drawable_size(int *x, int *y);
auto sdl_mouse_center(Gamep g) -> void;
auto sdl_mouse_warp(Gamep g, int x, int y) -> void;
auto sdl_screenshot_do(Gamep g) -> void;
auto sdl_screenshot(Gamep g) -> void;
auto sdl_tick(Gamep g) -> void;

#endif
