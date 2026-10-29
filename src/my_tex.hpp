//
// Copyright goblinhack@gmail.com
//

#ifndef MY_TEX_HPP
#define MY_TEX_HPP

#include "my_gl.hpp"
#include "my_types.hpp"

#include "my_sdl_compat.hpp"
#include <string>

[[nodiscard]] auto tex_get_gl_binding(Texp tex) -> int;

[[nodiscard]] auto tex_get_surface(Texp tex) -> SDL_Surface *;

[[nodiscard]] auto string2tex(const char **s) -> Texp;
[[nodiscard]] auto string2tex(std::string &s, int *len) -> Texp;
[[nodiscard]] auto tex_find(const std::string &file) -> Texp;
[[nodiscard]] auto tex_from_surface(SDL_Surface *surface, const std::string &file, const std::string &name, int mode) -> Texp;
[[nodiscard]] auto tex_load(const std::string &file, const std::string &name, int mode) -> Texp;
[[nodiscard]] auto tex_from_fbo(Gamep g, FboEnum fbo) -> Texp;

[[nodiscard]] auto tex_get_height(Texp tex) -> uint32_t;
[[nodiscard]] auto tex_get_width(Texp tex) -> uint32_t;

[[nodiscard]] auto tex_init() -> bool;

auto tex_fini() -> void;
auto tex_free(Texp tex) -> void;
void tex_load_sprites(Texp *tex, Texp *tex_monochrome, Texp *tex_mask,
                      Texp              *tex_outline_w_black_inside, // newline
                      Texp              *tex_outline_w_invis_inside, // newline
                      const std::string &file, const std::string &name, uint32_t tile_width, uint32_t tile_height, int mode);

#endif
