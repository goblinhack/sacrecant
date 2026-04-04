//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_color_defs.hpp"
#include "my_game.hpp"
#include "my_gl.hpp"
#include "my_globals.hpp"
#include "my_level.hpp"
#include "my_main.hpp"
#include "my_sdl_event.hpp"
#include "my_sdl_proto.hpp"
#include "my_wids.hpp"

void sdl_display(Gamep g)
{
  TRACE();

  if (sdl.window == nullptr) {
    return;
  }

  blit_fbo_bind(FBO_FINAL);
  {
    gl_clear();
  }
  blit_fbo_unbind();

  //
  // Blit the game map to FBOs
  //
  level_blit(g);

  blit_fbo_bind(FBO_FINAL);
  {
    //
    // Blit the widgets
    //
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    blit_fbo(g, FBO_WID);
  }
  blit_fbo_unbind();

  glBlendFunc(GL_ONE, GL_ZERO);
  blit_fbo(g, FBO_FINAL);

  //
  // Screenshot?
  //
  if ((g_do_screenshot)) [[unlikely]] {
    g_do_screenshot = false;
    sdl_screenshot_do(g);
  }

  SDL_Delay(game_sdl_delay_get(g));

  //
  // Flip
  //
  SDL_GL_SwapWindow(sdl.window);
}

void sdl_flush_display(Gamep g, bool force)
{
  TRACE();

  if (sdl.window == nullptr) {
    return;
  }

  if (! force) {
    IF_NODEBUG { return; }
    if (g_opt_no_slow_log_flush) {
      return;
    }
  }

  wid_display_all(g);
  gl_enter_2d_mode(g, game_window_pix_width_get(g), game_window_pix_height_get(g));
  sdl_display(g);
}

void sdl_display_reset(Gamep g)
{
  TRACE();

  if (sdl.window == nullptr) {
    return;
  }

  con("SDL: Video resetting");

  auto old_console = wid_console_serialize();

  wid_console_fini(g);
  config_game_gfx_update(g);

  wid_console_init(g);
  wid_hide(g, wid_console_window);
  sdl_flush_display(g);

  wid_console_deserialize(old_console);

  //
  // DO NOT CALL THIS HERE. We could be inside a widget mouse up handler.
  //
  // wid_gc_all();
  //
  wid_display_all(g);

  con("SDL: Video reset");
  sdl_flush_display(g);
}

static inline void sdl_list_video_size()
{
  TRACE();
  int i = 0;

  if (! g_opt_debug1) {
    return;
  }

  for (i = 0; i < SDL_GetNumDisplayModes(0); ++i) {
    SDL_DisplayMode mode;
    SDL_GetDisplayMode(0, i, &mode);
    DBG("+ SDL video            : %dx%d available, ratio %f", mode.w, mode.h,
        (static_cast< float >(mode.w)) / (static_cast< float >(mode.h)));
  }
}

auto sdl_display_init(Gamep g) -> bool
{
  int video_width  = 0;
  int video_height = 0;

  sdl.init_video = 1;

  sdl_list_video_size();

  //
  // If we have a saved setting, use that.
  //
  if ((game_config_pix_width_get(g) != 0) && (game_config_pix_height_get(g) != 0)) {
    video_width  = game_config_pix_width_get(g);
    video_height = game_config_pix_height_get(g);
    log("SDL: Used saved resolution %ux%u", video_width, video_height);
  } else {
    //
    // Else guess.
    //
    SDL_DisplayMode mode;
    memset(&mode, 0, SIZEOF(mode));

    log("SDL: Init display");
    if (SDL_GetCurrentDisplayMode(0, &mode) < 0) {
      CROAK("SDL_GetCurrentDisplayMode couldn't set windowed display: '%s'", SDL_GetError());
      return false;
    }

    game_config_pix_width_set(g, mode.w);
    game_config_pix_height_set(g, mode.h);

    video_width  = game_config_pix_width_get(g);
    video_height = game_config_pix_height_get(g);
    log("SDL: Used current resolution %ux%u", video_width, video_height);
  }

  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

  uint32_t video_flags = 0;

  log("SDL: Set SDL_WINDOW_OPENGL");
  video_flags = SDL_WINDOW_OPENGL;

  if (AN_ERROR_OCCURRED()) {
    //
    // Safe mode
    //
    video_width  = 1024;
    video_height = 768;
  } else {
    if (game_gfx_borderless_get(g)) {
      log("SDL: Set SDL_WINDOW_BORDERLESS");
      video_flags |= SDL_WINDOW_BORDERLESS;
    }

    if (game_gfx_fullscreen_get(g)) {
      log("SDL: Set SDL_WINDOW_FULLSCREEN");
      video_flags |= SDL_WINDOW_FULLSCREEN;
    }

    if (game_gfx_fullscreen_desktop_get(g)) {
      log("SDL: Set SDL_WINDOW_FULLSCREEN_DESKTOP");
      video_flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
    }

    log("SDL: Set SDL_WINDOW_INPUT_FOCUS");
    video_flags |= SDL_WINDOW_INPUT_FOCUS;

    //
    // SDL_WINDOW_ALWAYS_ON_TOP is meant to help with focus
    //
    // but does it cause issues with debugging, where the debugger window is always behind?
    //
    // log("SDL: Set SDL_WINDOW_ALWAYS_ON_TOP");
    // video_flags |= SDL_WINDOW_ALWAYS_ON_TOP;
  }

  if (g_skip_audio_and_gfx) {
    video_width  = 640;
    video_height = 480;
    video_flags  = 0;
  }

  if (AN_ERROR_OCCURRED()) {
    log("SDL: Create safe mode window size %ux%u", video_width, video_height);
  } else {
    log("SDL: Create window size %ux%u", video_width, video_height);
  }

  sdl.window = SDL_CreateWindow("sacrecant", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, video_width, video_height, video_flags);

  if (sdl.window == nullptr) {
    ERR("SDL_CreateWindow couldn't set windowed display %ux%u: '%s'", video_width, video_height, SDL_GetError());
    game_config_reset(g);
    game_save_config(g);
    return false;
  }

  int w = 0;
  int h = 0;
  SDL_GetWindowSize(sdl.window, &w, &h);
  log("SDL: Created window size %ux%u", w, h);

  game_config_pix_width_set(g, w);
  game_config_pix_height_set(g, h);
  game_window_pix_width_set(g, w);
  game_window_pix_height_set(g, h);

  log("SDL: Call SDL_GL_CreateContext(%dx%d)", game_window_pix_width_get(g), game_window_pix_height_get(g));
  sdl.context = SDL_GL_CreateContext(sdl.window);
  if (sdl.context == nullptr) {
    SDL_ClearError();
    ERR("SDL_GL_CreateContext failed %s", SDL_GetError());
    return false;
  }

  log("SDL: Call SDL_GL_MakeCurrent()");
  if (SDL_GL_MakeCurrent(sdl.window, sdl.context) < 0) {
    SDL_ClearError();
    ERR("SDL_GL_MakeCurrent failed %s", SDL_GetError());
    return false;
  }

  SDL_ClearError();

  if (AN_ERROR_OCCURRED()) {
    SDL_SetWindowTitle(sdl.window, "sacrecant (safe mode)");
  } else {
    SDL_SetWindowTitle(sdl.window, "sacrecant");
  }

  SDL_SetWindowInputFocus(sdl.window);

#if __APPLE__
  SDL_Event evt;
  //
  // Work around macos focus issue, possibly caused by iTerm
  //
  while (SDL_PollEvent(&evt) != 0) {}
#endif

  //
  // Ensure the window is always in front.
  //
  SDL_RaiseWindow(sdl.window);

  log("SDL: OpenGL Vendor   : %s", glGetString(GL_VENDOR));
  log("SDL: OpenGL Renderer : %s", glGetString(GL_RENDERER));
  log("SDL: OpenGL Version  : %s", glGetString(GL_VERSION));

  //
  // Do we really need to do this? it takes a small bit of time.
  //
  if (compiler_unused) {
    //
    // Clear the screen, both buffers
    //
    gl_clear();
    SDL_GL_SwapWindow(sdl.window);
    gl_clear();
    SDL_GL_SwapWindow(sdl.window);
  }

  IF_DEBUG
  { //
    DBG("SDL: OpenGL Exts     : %s", glGetString(GL_EXTENSIONS));
  }

  return true;
}

void sdl_display_fini(Gamep g)
{
  log("SDL: Video fini");
  TRACE();

  if (sdl.init_video == 0) {
    return;
  }
  sdl.init_video = 0;

  gl_fini_2d_mode(g);

#ifdef ENABLE_UI_ASCII_MOUSE
  SDL_ShowCursor(0);
  SDL_ShowCursor(1);
#endif

  log("SDL: Video quit");
  SDL_VideoQuit();

  log("SDL: Delete GL context");
  SDL_GL_DeleteContext(sdl.context);
  sdl.context = nullptr;

  log("SDL: Destroy window");
  SDL_DestroyWindow(sdl.window);
  sdl.window = nullptr;
}
