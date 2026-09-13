//
// Copyright goblinhack@gmail.com
//

#ifndef MY_MATH_HPP
#define MY_MATH_HPP

#include <stdlib.h>

#ifdef __APPLE__
#define SINCOS(x, s, c)  __sincos(x, s, c)
#define SINCOSF(x, s, c) __sincosf(x, s, c)
#define SINCOS(x, s, c)  __sincos(x, s, c)
#else
#define SINCOS(x, s, c)  sincos(x, s, c)
#define SINCOSF(x, s, c) sincosf(x, s, c)
#define SINCOS(x, s, c)  sincos(x, s, c)
#endif

#undef SQUARED
#define SQUARED(x) ((x) * (x))

#define ROUNDUP(x, r) ((r) * floor(((x) + (r)) / (r)))

//
// If distance is only needed for sorting
//
#define SORT_DISTANCE_f(xa, ya, xb, yb) (SQUARED((xb) - (xa)) + SQUARED((yb) - (ya)))
#define SORT_DISTANCE_d(xa, ya, xb, yb) (SQUARED((xb) - (xa)) + SQUARED((yb) - (ya)))

#define DISTANCE_f(xa, ya, xb, yb) sqrtf((float) (SQUARED((xb) - (xa)) + SQUARED((yb) - (ya))))
#define DISTANCE_d(xa, ya, xb, yb) sqrt((double) (SQUARED((xb) - (xa)) + SQUARED((yb) - (ya))))

#define PI      3.14159265358979323846f        // pi
#define RAD_45  0.78539816339744830962f        // pi/4
#define RAD_90  1.57079632679489661923f        // pi/2
#define RAD_180 3.14159265358979323846f        // pi
#define RAD_360 (2.0 * 3.14159265358979323846) // 2pi
#define RAD_270 (RAD_180 + RAD_90)

[[nodiscard]] auto nextpoweroftwo(uint32_t val) -> uint32_t;

#endif
