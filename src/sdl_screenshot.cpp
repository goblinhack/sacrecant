//
// Copyright goblinhack@gmail.com
//

#include "3rdparty/stb_image_write.hpp"
#include "my_callstack.hpp"
#include "my_color_defs.hpp"
#include "my_gl.hpp"
#include "my_globals.hpp"
#include "my_main.hpp"
#include "my_ptrcheck.hpp"
#include "my_sdl_event.hpp"
#include "my_sdl_proto.hpp"
#include "my_string.hpp"
#include "my_types.hpp"

#include <algorithm>
#include <cstdint>
#include <string>
#include <vector>

void sdl_screenshot(Gamep g)
{
  TRACE();

  if (sdl.window != nullptr) {
    g_do_screenshot = true;
  }
}

void sdl_screenshot_do(Gamep g)
{
  TRACE();

  if (sdl.window == nullptr) {
    return;
  }

  GL_ERROR_CHECK();
  FboEnum const fbo = FBO_FULL_SCREEN_FINAL;
  int           w   = 0;
  int           h   = 0;
  GL_ERROR_CHECK();
  fbo_get_size(g, fbo, w, h);
  GL_ERROR_CHECK();
  blit_fbo_bind(fbo);
  GL_ERROR_CHECK();

  static int count = 1;

  GL_ERROR_CHECK();
  glPixelStorei(GL_PACK_ALIGNMENT, 1);
  GL_ERROR_CHECK();

  //
  // OpenGL ES only guarantees reading back GL_RGBA/GL_UNSIGNED_BYTE, so read that and drop the alpha.
  //
  std::vector< uint8_t > rgba(4 * static_cast< size_t >(w) * static_cast< size_t >(h));
  glReadPixels(0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, rgba.data());
  GL_ERROR_CHECK();

  std::vector< uint8_t > pixels(3 * static_cast< size_t >(w) * static_cast< size_t >(h));
  for (size_t i = 0, n = static_cast< size_t >(w) * static_cast< size_t >(h); i < n; i++) {
    pixels[ (i * 3) + 0 ] = rgba[ (i * 4) + 0 ];
    pixels[ (i * 3) + 1 ] = rgba[ (i * 4) + 1 ];
    pixels[ (i * 3) + 2 ] = rgba[ (i * 4) + 2 ];
  }

  for (int line = 0; line != h / 2; ++line) {
    std::swap_ranges(pixels.begin() + 3 * w * line, pixels.begin() + 3 * w * (line + 1), pixels.begin() + 3 * w * (h - line - 1));
  }

  int const components = 3;

  char *png = dynprintf("screenshot.%d.png", count);
  stbi_write_png(png, w, h, components, pixels.data(), 3 * w);
  GL_ERROR_CHECK();
  topcon("Screenshot: %s", png);
  MYFREE(png);
  blit_fbo_unbind();
  GL_ERROR_CHECK();

  count++;
}

[[nodiscard]] auto sdl_fbo_save(Gamep g, FboEnum fbo) -> std::vector< uint8_t >
{
  TRACE();
  int w = 0;
  int h = 0;
  fbo_get_size(g, fbo, w, h);
  GL_ERROR_CHECK();

  blit_fbo_bind(fbo);
  GL_ERROR_CHECK();

  glPixelStorei(GL_PACK_ALIGNMENT, 1);
  GL_ERROR_CHECK();

  std::vector< uint8_t > pixels;
  pixels.resize(4 * w * h);

  glReadPixels(0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());
  GL_ERROR_CHECK();

  blit_fbo_unbind();
  GL_ERROR_CHECK();

  return pixels;
}

void sdl_fbo_load(Gamep g, FboEnum fbo, const std::vector< uint8_t > &pixels)
{
  TRACE();
  if (pixels.empty()) {
    return;
  }

  int w = 0;
  int h = 0;
  fbo_get_size(g, fbo, w, h);
  GL_ERROR_CHECK();

  blit_fbo_bind(fbo);
  GL_ERROR_CHECK();

  //
  // glDrawPixels does not exist in OpenGL ES, so upload the pixels to a temporary texture and draw it
  // over the whole FBO. The pixels were read back bottom row first, which is also the order the texture
  // is sampled in, so no flipping is needed here other than the usual v flip for our top left origin.
  //
  GLuint tex = 0;
  glGenTextures(1, &tex);
  glBindTexture(GL_TEXTURE_2D, tex);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glBindTexture(GL_TEXTURE_2D, 0);
  GL_ERROR_CHECK();

  //
  // Use our own projection for this so we do not disturb the 2d mode state of the caller.
  //
  GLint viewport[ 4 ] = {};
  glGetIntegerv(GL_VIEWPORT, viewport);
  glViewport(0, 0, w, h);

  gl_matrix_mode(GL_MATRIX_PROJECTION);
  gl_matrix_push();
  gl_matrix_identity();
  gl_matrix_ortho(0, static_cast< float >(w), static_cast< float >(h), 0, -1.0F, 1.0F);
  gl_matrix_mode(GL_MATRIX_MODELVIEW);
  gl_matrix_push();
  gl_matrix_identity();

  //
  // A straight copy, not a blend; this is restoring the FBO contents.
  //
  glBlendFunc(GL_ONE, GL_ZERO);
  blit_init();
  blit(static_cast< int >(tex), 0.0F, 1.0F, 1.0F, 0.0F, 0, 0, static_cast< GLshort >(w), static_cast< GLshort >(h), WHITE);
  blit_flush();
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  gl_matrix_mode(GL_MATRIX_MODELVIEW);
  gl_matrix_pop();
  gl_matrix_mode(GL_MATRIX_PROJECTION);
  gl_matrix_pop();
  gl_matrix_mode(GL_MATRIX_MODELVIEW);
  glViewport(viewport[ 0 ], viewport[ 1 ], viewport[ 2 ], viewport[ 3 ]);

  glDeleteTextures(1, &tex);
  GL_ERROR_CHECK();

  blit_fbo_unbind();
  GL_ERROR_CHECK();
}

void sdl_fbo_dump(Gamep g, FboEnum fbo, const std::string &name)
{
  TRACE();

  if (g_thread_id != MAIN_THREAD) {
    return;
  }

  int w = 0;
  int h = 0;
  fbo_get_size(g, fbo, w, h);
  GL_ERROR_CHECK();

  blit_fbo_push(fbo);
  GL_ERROR_CHECK();

  glPixelStorei(GL_PACK_ALIGNMENT, 1);
  GL_ERROR_CHECK();

  std::vector< uint8_t > pixels;
  pixels.resize(4 * w * h);

  glReadPixels(0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());
  GL_ERROR_CHECK();

  blit_fbo_pop();
  GL_ERROR_CHECK();

  static int count      = 0;
  int const  components = 4;
  char      *png        = dynprintf("screenshot.%s.%03d.png", name.c_str(), count);
  stbi_write_png(png, w, h, components, pixels.data(), 4 * w);
  con("Screenshot: %s", png);
  MYFREE(png);
  count++;
}
