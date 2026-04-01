//
// Copyright goblinhack@gmail.com
//

// #include "3rdparty/stb_image.hpp"
#include "my_callstack.hpp"
#include "my_file.hpp"
#include "my_gl.hpp"
#include "my_pixel.hpp"
#include "my_ptrcheck.hpp"
#include "my_string.hpp"
#include "my_tex.hpp"
#include "my_ui.hpp"

#include <unordered_map>
#include <utility>

extern "C" {
extern auto stbi_load_from_memory(const uint8_t *buffer, int len, int *x, int *y, int *comp, int req_comp) -> uint8_t *;
extern void stbi_image_free(void *retval_from_stbi_load);
};

class Tex
{
public:
  explicit Tex(std::string vname) : name(std::move(vname)) { NEWPTR(MTYPE_TEX, this, "Tex"); }

  ~Tex()
  {
    OLDPTR(MTYPE_TEX, this);

    if (surface != nullptr) {
      VERIFY(MTYPE_SDL, surface);
      SDL_FreeSurface(surface);
      OLDPTR(MTYPE_SDL, surface);
      surface = nullptr;
    }

    if (gl_surface_binding != 0) {
      GLuint tmp = 0;
      tmp        = gl_surface_binding;
      glDeleteTextures(1, &tmp);
      gl_surface_binding = 0;
    }
  }

  std::string  name;
  uint32_t     width              = {};
  uint32_t     height             = {};
  int          gl_surface_binding = {};
  SDL_Surface *surface            = {};
};

static std::unordered_map< std::string, Texp > textures;
static std::unordered_map< std::string, Texp > textures_monochrome;
static std::unordered_map< std::string, Texp > textures_mask;
static std::unordered_map< std::string, Texp > textures_outline_with_black_interior;
static std::unordered_map< std::string, Texp > textures_outline_with_empty_interior;

auto tex_init() -> bool
{
  TRACE();
  return true;
}

void tex_fini()
{
  TRACE();
  for (auto &t : textures) {
    delete t.second;
  }
  textures.clear();
  for (auto &t : textures_monochrome) {
    delete t.second;
  }
  textures_monochrome.clear();
  for (auto &t : textures_mask) {
    delete t.second;
  }
  textures_mask.clear();
  for (auto &t : textures_outline_with_black_interior) {
    delete t.second;
  }
  textures_outline_with_black_interior.clear();
  for (auto &t : textures_outline_with_empty_interior) {
    delete t.second;
  }
  textures_outline_with_empty_interior.clear();
}

void tex_free(Texp tex)
{
  TRACE();
  textures.erase(tex->name);
  textures_monochrome.erase(tex->name);
  textures_mask.erase(tex->name);
  textures_outline_with_black_interior.erase(tex->name);
  textures_outline_with_empty_interior.erase(tex->name);
  delete (tex);
}

static auto load_raw_image(const std::string &filename, int *x, int *y, int *comp) -> uint8_t *
{
  TRACE();
  uint8_t *file_data  = nullptr;
  uint8_t *image_data = nullptr; // NOLINT
  int      len        = 0;

  file_data = file_load(filename.c_str(), &len);
  if (file_data == nullptr) {
    CROAK("could not read file, '%s'", filename.c_str());
  }

  image_data = stbi_load_from_memory(file_data, len, x, y, comp, 0);
  if (image_data == nullptr) {
    CROAK("could not read memory for file, '%s'", filename.c_str());
  }

  DBG2("loaded '%s', %ux%u", filename.c_str(), *x, *y);

  MYFREE(file_data);

  return image_data;
}

static void free_raw_image(uint8_t *image_data)
{
  TRACE();
  stbi_image_free(image_data);
}

static auto load_image(const std::string &filename) -> SDL_Surface *
{
  TRACE();
  uint32_t     rmask      = 0;
  uint32_t     gmask      = 0;
  uint32_t     bmask      = 0;
  uint32_t     amask      = 0;
  uint8_t     *image_data = nullptr;
  SDL_Surface *surf       = nullptr;
  int          x          = 0;
  int          y          = 0;
  int          comp       = 0;

  image_data = load_raw_image(filename, &x, &y, &comp);
  if (image_data == nullptr) {
    ERR("could not read memory for file, '%s'", filename.c_str());
  }

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
  rmask = 0xff000000;
  gmask = 0x00ff0000;
  bmask = 0x0000ff00;
  amask = 0x000000ff;
#else
  rmask = 0x000000ff;
  gmask = 0x0000ff00;
  bmask = 0x00ff0000;
  amask = 0xff000000;
#endif

  if (comp == 4) {
    surf = SDL_CreateRGBSurface(0, x, y, 32, rmask, gmask, bmask, amask);
    NEWPTR(MTYPE_SDL, surf, "SDL_CreateRGBSurface1");
  } else if (comp == 3) {
    surf = SDL_CreateRGBSurface(0, x, y, 24, rmask, gmask, bmask, 0);
    NEWPTR(MTYPE_SDL, surf, "SDL_CreateRGBSurface2");
  } else if (comp == 2) {
    surf = SDL_CreateRGBSurface(0, x, y, 32, 0, 0, 0, 0);
    NEWPTR(MTYPE_SDL, surf, "SDL_CreateRGBSurface3");
  } else {
    ERR("could not handle image with %d components", comp);
    free_raw_image(image_data);
    return nullptr;
  }

  memcpy(surf->pixels, image_data, comp * x * y);

  if ((comp == 2) || (comp == 3)) {
    SDL_Surface *old_surf = surf;
    DBG2("- SDL_ConvertSurfaceFormat");
    surf = SDL_ConvertSurfaceFormat(old_surf, SDL_PIXELFORMAT_RGBA8888, 0);
    NEWPTR(MTYPE_SDL, surf, "SDL_CreateRGBSurface4");
    OLDPTR(MTYPE_SDL, old_surf);
    SDL_FreeSurface(old_surf);
    SDL_SaveBMP(surf, filename.c_str());
  }

  free_raw_image(image_data);

  return surf;
}

static void load_images(SDL_Surface **surf1_out, const std::string &filename)
{
  TRACE();
  uint32_t     rmask      = 0;
  uint32_t     gmask      = 0;
  uint32_t     bmask      = 0;
  uint32_t     amask      = 0;
  uint8_t     *image_data = nullptr;
  SDL_Surface *surf1      = nullptr;
  int          x          = 0;
  int          y          = 0;
  int          comp       = 0;

  image_data = load_raw_image(filename, &x, &y, &comp);
  if (image_data == nullptr) {
    ERR("could not read memory for file, '%s'", filename.c_str());
  }

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
  rmask = 0xff000000;
  gmask = 0x00ff0000;
  bmask = 0x0000ff00;
  amask = 0x000000ff;
#else
  rmask = 0x000000ff;
  gmask = 0x0000ff00;
  bmask = 0x00ff0000;
  amask = 0xff000000;
#endif

  if (comp == 4) {
    surf1 = SDL_CreateRGBSurface(0, x, y, 32, rmask, gmask, bmask, amask);
    if (surf1 == nullptr) {
      ERR("could not create surface");
      return;
    }
    NEWPTR(MTYPE_SDL, surf1, "SDL_CreateRGBSurface5");
  } else if (comp == 3) {
    surf1 = SDL_CreateRGBSurface(0, x, y, 24, rmask, gmask, bmask, 0);
    if (surf1 == nullptr) {
      ERR("could not create surface");
      return;
    }
    NEWPTR(MTYPE_SDL, surf1, "SDL_CreateRGBSurface6");
  } else if (comp == 2) {
    surf1 = SDL_CreateRGBSurface(0, x, y, 32, 0, 0, 0, 0);
    if (surf1 == nullptr) {
      ERR("could not create surface");
      return;
    }
    NEWPTR(MTYPE_SDL, surf1, "SDL_CreateRGBSurface7");
  } else {
    ERR("could not handle image with %d components", comp);
  }

  memcpy(surf1->pixels, image_data, comp * x * y);

  if ((comp == 2) || (comp == 3)) {
    SDL_Surface *old_surf = surf1;
    DBG2("- SDL_ConvertSurfaceFormat");
    surf1 = SDL_ConvertSurfaceFormat(old_surf, SDL_PIXELFORMAT_RGBA8888, 0);
    if (surf1 == nullptr) {
      ERR("could not convert surface");
      return;
    }
    NEWPTR(MTYPE_SDL, surf1, "SDL_CreateRGBSurface14");
    OLDPTR(MTYPE_SDL, old_surf);
    SDL_FreeSurface(old_surf);
    SDL_SaveBMP(surf1, filename.c_str());
  }

  free_raw_image(image_data);

  *surf1_out = surf1;
}

//
// Load a texture
//
auto tex_load(const std::string &file, const std::string &name, int mode) -> Texp
{
  TRACE();
  Texp t = tex_find(name);

  if (t != nullptr) {
    return t;
  }

  DBG2("Loading texture '%s', '%s'", file.c_str(), name.c_str());
  if (file.empty()) {
    if (name.empty()) {
      ERR("no file for tex");
      return nullptr;
    }
    ERR("no file for tex loading '%s'", name.c_str());
    return nullptr;
  }

  SDL_Surface *surface = nullptr;
  surface              = load_image(file);

  if (surface == nullptr) {
    ERR("could not make surface from file '%s'", file.c_str());
  }

  t = tex_from_surface(surface, file, name, mode);

  DBG2("- loaded texture '%s', '%s'", file.c_str(), name.c_str());

  return t;
}

//
// Returns textures:
//
// 1 - gray/monochrome
// 2 - white mask
// 3 - white outline with black interior
// 4 - white outline with empty interior
//
static auto tex_create_masks_from_surface(SDL_Surface *src, const std::string &file, const std::string &name, int mode)
    -> std::vector< Texp >
{
  auto name_monochrome                  = name + "_monochrome";
  auto name_mask                        = name + "_mask";
  auto name_outline_with_black_interior = name + "_outline_with_black_interior";
  auto name_outline_with_empty_interior = name + "_outline_with_empty_interior";

  Texp tex_dst_monochrome                  = new Tex(name_monochrome);
  Texp tex_dst_mask                        = new Tex(name_mask);
  Texp tex_dst_outline_with_black_interior = new Tex(name_outline_with_black_interior);
  Texp tex_dst_outline_with_empty_interior = new Tex(name_outline_with_empty_interior);

  textures_monochrome.insert(std::make_pair(name_monochrome, tex_dst_monochrome));
  textures_mask.insert(std::make_pair(name_mask, tex_dst_mask));
  textures_outline_with_black_interior.insert(std::make_pair(name_outline_with_black_interior, tex_dst_outline_with_black_interior));
  textures_outline_with_empty_interior.insert(std::make_pair(name_outline_with_empty_interior, tex_dst_outline_with_empty_interior));

  uint32_t rmask = 0, gmask = 0, bmask = 0, amask = 0;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
  rmask = 0xff000000;
  gmask = 0x00ff0000;
  bmask = 0x0000ff00;
  amask = 0x000000ff;
#else
  rmask = 0x000000ff;
  gmask = 0x0000ff00;
  bmask = 0x00ff0000;
  amask = 0xff000000;
#endif

  uint32_t const src_width  = src->w;
  uint32_t const src_height = src->h;
  uint32_t       src_x      = 0;
  uint32_t       src_y      = 0;

  SDL_Surface *dst_monochrome = SDL_CreateRGBSurface(0, src_width, src_height, 32, rmask, gmask, bmask, amask);
  NEWPTR(MTYPE_SDL, dst_monochrome, "SDL_CreateRGBSurface17");

  SDL_Surface *dst_mask = SDL_CreateRGBSurface(0, src_width, src_height, 32, rmask, gmask, bmask, amask);
  NEWPTR(MTYPE_SDL, dst_mask, "SDL_CreateRGBSurface18");

  SDL_Surface *dst_outline_with_black_interior = SDL_CreateRGBSurface(0, src_width, src_height, 32, rmask, gmask, bmask, amask);
  NEWPTR(MTYPE_SDL, dst_outline_with_black_interior, "SDL_CreateRGBSurface19");

  SDL_Surface *dst_outline_with_empty_interior = SDL_CreateRGBSurface(0, src_width, src_height, 32, rmask, gmask, bmask, amask);
  NEWPTR(MTYPE_SDL, dst_outline_with_empty_interior, "SDL_CreateRGBSurface19");

  color const col_white(255, 255, 255, 255);
  color const col_black(0, 0, 0, 255);
  color const col_none(0, 0, 0, 0);

  for (src_y = 0; src_y < src_height; src_y++) {
    for (src_x = 0; src_x < src_width; src_x++) {
      color col_orig;
      GET_PIXEL(src, src_x, src_y, col_orig);

      //
      // Solid black for the monster body and white pixels to replace any black outline_with_black_interior
      //
      if ((col_orig.a == 255) && (col_orig.r == 0) && (col_orig.g == 0) && (col_orig.b == 0)) {
        PUT_PIXEL(dst_outline_with_black_interior, src_x, src_y, col_white);
      } else if (col_orig.a != 0U) {
        PUT_PIXEL(dst_outline_with_black_interior, src_x, src_y, col_black);
      }

      //
      // Empty pixels for the monster body and white pixels to replace any black outline_with_black_interior
      //
      if ((col_orig.a == 255) && (col_orig.r == 0) && (col_orig.g == 0) && (col_orig.b == 0)) {
        PUT_PIXEL(dst_outline_with_empty_interior, src_x, src_y, col_white);
      } else if (col_orig.a != 0U) {
        PUT_PIXEL(dst_outline_with_black_interior, src_x, src_y, col_none);
      }

      //
      // Give an averaged, purpleish color to tiles
      //
      if (col_orig.a > 0) {
        auto          col_monochrome = col_orig;
        uint8_t const avg
            = (static_cast< int >(col_monochrome.r) + static_cast< int >(col_monochrome.g) + static_cast< int >(col_monochrome.b))
            / UI_LIGHT_BACKGROUND;
        col_monochrome.r = avg;
        col_monochrome.g = avg;
        col_monochrome.b = avg;
        col_monochrome.r /= UI_LIGHT_BACKGROUND;
        col_monochrome.g /= UI_LIGHT_BACKGROUND;

        PUT_PIXEL(dst_monochrome, src_x, src_y, col_monochrome);
      }

      //
      // The mask is set for any non alpha pixel
      //
      if ((col_orig.a > 0) && ((col_orig.r > 0) || (col_orig.g > 0) || (col_orig.b > 0))) {
        color col_mask = col_orig;
        col_mask.r     = 255;
        col_mask.g     = 255;
        col_mask.b     = 255;
        col_mask.a     = 255;
        PUT_PIXEL(dst_mask, src_x, src_y, col_mask);
      }
    }
  }

#if 0
  //
  // Unused
  //
  auto tiles_across = src_width / tile_width;
  auto tiles_down   = src_height / tile_height;

  for (uint32_t tile_x = 0; tile_x < tiles_across; tile_x++) {
    for (uint32_t tile_y = 0; tile_y < tiles_down; tile_y++) {
      for (uint32_t ox = 0; ox < tile_width; ox++) {
        for (uint32_t oy = 0; oy < tile_height; oy++) {
          auto px = tile_x * tile_width + ox;
          auto py = tile_y * tile_height + oy;

          color col_orig;
          GET_PIXEL(src, px, py, col_orig);

          if ((col_orig.a == 255) && (col_orig.r == 0) && (col_orig.g == 0) && (col_orig.b == 0)) {
            PUT_PIXEL(dst_outline, px + 0, py - 0, col_white);
          }
        }
      }
    }
  }
#endif

  tex_dst_monochrome                  = tex_from_surface(dst_monochrome, file, name_monochrome, mode);
  tex_dst_mask                        = tex_from_surface(dst_mask, file, name_mask, mode);
  tex_dst_outline_with_black_interior = tex_from_surface(dst_outline_with_black_interior, file, name_outline_with_black_interior, mode);
  tex_dst_outline_with_empty_interior = tex_from_surface(dst_outline_with_empty_interior, file, name_outline_with_empty_interior, mode);

  std::vector< Texp > out;

  out.push_back(tex_dst_monochrome);
  out.push_back(tex_dst_mask);
  out.push_back(tex_dst_outline_with_black_interior);
  out.push_back(tex_dst_outline_with_empty_interior);

  return out;
}

void tex_load_sprites(Texp *tex, Texp *tex_monochrome, Texp *tex_mask,    // newline
                      Texp              *tex_outline_with_black_interior, // newline
                      Texp              *tex_outline_with_empty_interior, // newline
                      const std::string &file, const std::string &name, uint32_t tile_width, uint32_t tile_height, int mode)
{
  TRACE();
  Texp t = tex_find(name);
  if (t != nullptr) {
    ERR("tex name already exists '%s'", name.c_str());
  }

  DBG2("Loading texture '%s', '%s'", file.c_str(), name.c_str());
  if (file.empty()) {
    if (name.empty()) {
      ERR("no file for tex");
    } else {
      ERR("no file for tex loading '%s'", name.c_str());
    }
  }

  DBG2("- create textures '%s', '%s'", file.c_str(), name.c_str());
  SDL_Surface *surface = nullptr;

  load_images(&surface, file);

  if (surface == nullptr) {
    ERR("could not make surface from file '%s'", file.c_str());
  }

  *tex                             = tex_from_surface(surface, file, name, mode);
  auto p                           = tex_create_masks_from_surface(surface, file, name, mode);
  *tex_monochrome                  = p[ 0 ];
  *tex_mask                        = p[ 1 ];
  *tex_outline_with_black_interior = p[ 2 ];
  *tex_outline_with_empty_interior = p[ 3 ];

  DBG2("- loaded texture '%s', '%s'", file.c_str(), name.c_str());
}

//
// Find an existing tex.
//
auto tex_find(const std::string &file) -> Texp
{
  TRACE();
  if (file.empty()) {
    ERR("no filename given for tex find");
  }

  auto result = textures.find(file);
  if (result == textures.end()) {
    return nullptr;
  }

  return result->second;
}

//
// Creae a texture from a surface
//
auto tex_from_surface(SDL_Surface *surface, const std::string &file, const std::string &name, int mode) -> Texp
{
  TRACE();

  if (surface == nullptr) {
    ERR("could not make surface from file, '%s'", file.c_str());
  }

  DBG2("Texture: '%s', %dx%d", file.c_str(), surface->w, surface->h);

  //
  // Get the number of channels in the SDL surface
  //
  int const channels      = surface->format->BytesPerPixel;
  int       textureFormat = 0;

  if (channels == 4) {
    //
    // Contains alpha channel
    //
    if (surface->format->Rmask == 0x000000ff) {
      textureFormat = GL_RGBA;
    } else {
      textureFormat = GL_BGRA;
    }
  } else if (channels == 3) {
    //
    // Contains no alpha channel
    //
    if (surface->format->Rmask == 0x000000ff) {
      textureFormat = GL_RGB;
    } else {
#ifdef GL_BGR
      textureFormat = GL_BGR;
#else
      ERR("'%s' need support for GL_BGR", file);
#endif
    }
  } else {
    ERR("'%s' is not truecolor, need %d bytes per pixel", file.c_str(), channels);
  }

  //
  // Create the tex
  //
  GLuint gl_surface_binding = 0;

  if (! g_opt_tests) {
    glEnable(GL_TEXTURE_2D); // Apparently needed for ATI drivers

    glGenTextures(1, &gl_surface_binding);

    //
    // Typical tex generation using data from the bitmap
    //
    glBindTexture(GL_TEXTURE_2D, gl_surface_binding);

    //
    // Generate the tex
    //
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, surface->w, surface->h, 0, textureFormat, GL_UNSIGNED_BYTE, surface->pixels);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    //
    // Linear filtering is best for pixel art.
    //
    // Nearest is meant to be quicker but I didn't see that in reality.
    //
    if (mode == GL_NEAREST) {
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    } else {
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }
  }

  Texp t      = new Tex(name);
  auto result = textures.insert(std::make_pair(name, t));

  if (! result.second) {
    CROAK("texture insert surface name '%s' failed", name.c_str());
  }

  t->width              = surface->w;
  t->height             = surface->h;
  t->gl_surface_binding = gl_surface_binding;
  t->surface            = surface;

  return t;
}

//
// Creae a texture from an FBO
//
auto tex_from_fbo(Gamep g, FboEnum fbo) -> Texp
{
  TRACE();

  int w = 0;
  int h = 0;
  fbo_get_size(g, fbo, w, h);
  auto name = FboEnum_to_string(fbo);

  DBG2("Texture: '%s', %dx%d", name.c_str(), w, h);

  Texp t      = new Tex(name);
  auto result = textures.insert(std::make_pair(name, t));

  if (! result.second) {
    ERR("tex insert name '%s' failed", name.c_str());
  }

  t->width              = w;
  t->height             = h;
  t->gl_surface_binding = g_fbo_tex_id[ fbo ];
  t->surface            = nullptr;

  return t;
}

auto tex_get_gl_binding(Texp tex) -> int
{
  TRACE();
  return tex->gl_surface_binding;
}

auto tex_get_width(Texp tex) -> uint32_t
{
  TRACE();
  if (tex == nullptr) {
    ERR("no texture");
  }

  return tex->width;
}

auto tex_get_height(Texp tex) -> uint32_t
{
  TRACE();
  if (tex == nullptr) {
    ERR("no texture");
  }

  return tex->height;
}

auto tex_get_surface(Texp tex) -> SDL_Surface *
{
  TRACE();
  return tex->surface;
}

auto string2tex(const char **s) -> Texp
{
  TRACE();

  static char              tmp[ MAXSHORTSTR ];
  static std::string const eo_tmp = tmp + MAXSHORTSTR - 1;
  const char              *c      = *s;
  char                    *t      = tmp; // NOLINT

  while (t < eo_tmp) {
    if ((*c == '\0') || (*c == '$')) {
      break;
    }

    *t++ = *c++;
  }

  if (c == eo_tmp) {
    return nullptr;
  }

  *t++ = '\0';
  *s += (t - tmp);

  auto result = textures.find(tmp);
  if (result == textures.end()) {
    ERR("unknown tex '%s'", tmp);
  }

  return result->second;
}

auto string2tex(std::string &s, int *len) -> Texp
{
  TRACE();
  auto        iter = s.begin();
  std::string out;

  while (iter != s.end()) {
    auto c = *iter;

    if ((c == '\0') || (c == '$')) {
      break;
    }

    out += c;
    iter++;
  }

  if (len != nullptr) {
    *len = iter - s.begin();
  }

  if (iter == s.end()) {
    ERR("unknown tex '%s'", out.c_str());
  }

  auto result = textures.find(out);
  if (result == textures.end()) {
    ERR("unknown tex '%s'", out.c_str());
  }

  return result->second;
}
