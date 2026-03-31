//
// Copyright goblinhack@gmail.com
//

#include "3rdparty/stb_image_write.hpp"
#include "my_callstack.hpp"
#include "my_gl.hpp"
#include "my_globals.hpp"
#include "my_main.hpp"
#include "my_ptrcheck.hpp"
#include "my_sdl_event.hpp"
#include "my_sdl_proto.hpp"
#include "my_string.hpp"

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
  FboEnum const fbo = FBO_FINAL;
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

#if 0
  //
  // Not sure what this does and why I had it!
  //
  glReadBuffer(GL_BACK_LEFT);
  GL_ERROR_CHECK();
#endif

  std::vector< uint8_t > pixels(3 * w * h);
  glReadPixels(0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, pixels.data());
  GL_ERROR_CHECK();

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

auto sdl_fbo_save(Gamep g, FboEnum fbo) -> std::vector< uint8_t >
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

  glPixelStorei(GL_PACK_ALIGNMENT, 1);
  GL_ERROR_CHECK();

  glDrawPixels(w, h, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());
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
