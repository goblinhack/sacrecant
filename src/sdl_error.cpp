//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_main.hpp"
#include "my_string.hpp"

#include "my_sdl_compat.hpp"
#include <cmath>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>

static void sdl_msgerr_(const char *fmt, va_list args)
{
  TRACE();

  char buf[ MAXLONGSTR ];
  buf[ 0 ] = '\0';

  vsnprintf(buf, MAXLONGSTR, fmt, args);

#if SDL_MAJOR_VERSION >= 2
  if (compiler_unused) {
    //
    // Not needed
    //
    SDL_Log("%s", buf);
  }

  //
  // Fullscreen sometimes hides the error, so create a temp window
  //
  log("show SDL message box");

#ifdef USE_SDL3
  //
  // SDL_WINDOW_SHOWN no longer exists; windows are shown by default.
  //
  auto *window = sdl_compat_create_window("sacrecant error", 100, 100, SDL_WINDOW_ALWAYS_ON_TOP);
#else
  auto *window = sdl_compat_create_window("sacrecant error", 100, 100, SDL_WINDOW_SHOWN | SDL_WINDOW_ALWAYS_ON_TOP);
#endif
  SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Sacrecant", buf, window);
  log("launched SDL message box");
  SDL_DestroyWindow(window);
#endif

  //
  // We are inside an error already, so do not call ERR
  //
  log("SDL: %s", buf);
}

void sdl_msg_box(const char *fmt, ...)
{
  TRACE();

  va_list args = {};

  va_start(args, fmt);
  sdl_msgerr_(fmt, args);
  va_end(args);
}
