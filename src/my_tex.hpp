//
// Copyright goblinhack@gmail.com
//

#ifndef MY_TEX_HPP
#define MY_TEX_HPP

#include "my_gl.hpp"
#include "my_types.hpp"

#include <SDL.h>
#include <string>

auto tex_get_gl_binding(Texp tex) -> int;

auto tex_get_surface(Texp tex) -> SDL_Surface *;

auto string2tex(const char **s) -> Texp;
auto string2tex(std::string &s, int *len) -> Texp;
auto tex_find(const std::string &file) -> Texp;
auto tex_from_surface(SDL_Surface *surface, const std::string &file, const std::string &name, int mode) -> Texp;
auto tex_load(const std::string &file, const std::string &name, int mode) -> Texp;
auto tex_from_fbo(Gamep g, FboEnum fbo) -> Texp;

auto tex_get_height(Texp tex) -> uint32_t;
auto tex_get_width(Texp tex) -> uint32_t;

auto tex_init() -> bool;

void tex_fini();
void tex_free(Texp tex);
void tex_load_sprites(Texp *tex, Texp *tex_monochrome, Texp *tex_mask,
                      Texp              *tex_outline_with_black_interior, // newline
                      Texp              *tex_outline_with_empty_interior, // newline
                      const std::string &file, const std::string &name, uint32_t tile_width, uint32_t tile_height, int mode);

#endif
