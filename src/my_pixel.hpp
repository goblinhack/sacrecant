//
// Copyright goblinhack@gmail.com
//

#ifndef MY_GETPUT_PIXEL_MACROS_HPP
#define MY_GETPUT_PIXEL_MACROS_HPP

#include "my_color.hpp"
#include "my_globals.hpp"
#include "my_main.hpp"

#include <SDL.h>

#define pixel_ANYw(s, y)              ((y) * ((s)->pitch / (s)->format->BytesPerPixel))
#define pixel_32bpp(s, x, y)          ((uint32_t *) (s)->pixels + (x) + pixel_ANYw(s, y))
#define PUT_PIXEL_32bpp(s, x, y, rgb) *pixel_32bpp(s, x, y) = rgb
#define GET_PIXEL_32bpp(s, x, y, rgb) *(rgb) = *pixel_32bpp(s, x, y)

//
// Lock a SDL_Surface for safe color access
//
static inline void lock(SDL_Surface *surface)
{
  //
  // Is this needed? unlocking seems slow
  //
  if (SDL_MUSTLOCK(surface)) {
    SDL_LockSurface(surface);
  }
}

//
// Unlock a SDL_Surface, after lock()
//
static inline void unlock(SDL_Surface *surface)
{
  //
  // Is this needed? unlocking seems slow
  //
  if (SDL_MUSTLOCK(surface)) {
    SDL_UnlockSurface(surface);
  }
}

//
// This function sets the specified color on a SDL_Surface. Sanity checks are
// performed on the coordinates and the SDL_Surface is locked for you.
// Safe, but slow. For more speed, try the lower level access function.
//
static void inline PUT_PIXEL(SDL_Surface *surface, uint16_t x, uint16_t y, const color &col)
{
  uint32_t rgb = 0;

#ifdef DEBUG_BUILD
  if (((x >= (uint32_t) surface->w) || (y >= (uint32_t) surface->h))) [[unlikely]] {
    CROAK("PUT_PIXEL out of range, pix %d,%d in size %d,%d", x, y, surface->w, surface->h);
  }
#endif

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
  rgb = ((col.r << 24) | (col.g << 16) | (col.b << 8) | col.a);
#else
  rgb = (col.r | (col.g << 8) | (col.b << 16) | (col.a << 24));
#endif

  PUT_PIXEL_32bpp(surface, x, y, rgb);
}

//
// This function sets the specified color on a SDL_Surface. Sanity checks are
// performed on the coordinates and the SDL_Surface is locked for you.
// Safe, but slow. For more speed, try the lower level access function.
//
static inline void GET_PIXEL(SDL_Surface *surface, uint16_t x, uint16_t y, color &col)
{
  uint32_t rgb = 0;

#ifdef DEBUG_BUILD
  if (((x >= (uint32_t) surface->w) || (y >= (uint32_t) surface->h))) [[unlikely]] {
    CROAK("GET_PIXEL out of range, pix %d,%d in size %d,%d", x, y, surface->w, surface->h);
  }
#endif

#if 0 // needed?
  lock(surface);
#endif

  GET_PIXEL_32bpp(surface, x, y, &rgb);

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
  col.r = (rgb & 0xff000000) >> 24;
  col.g = (rgb & 0x00ff0000) >> 16;
  col.b = (rgb & 0x0000ff00) >> 8;
  col.a = (rgb & 0x000000ff);
#else
  col.r = (rgb & 0x000000ff);
  col.g = (rgb & 0x0000ff00) >> 8;
  col.b = (rgb & 0x00ff0000) >> 16;
  col.a = (rgb & 0xff000000) >> 24;
#endif

#if 0 // needed?
  unlock(surface);
#endif
}

#endif
