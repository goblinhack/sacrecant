//
// Copyright goblinhack@gmail.com
//

#ifndef MY_TYPES_HPP
#define MY_TYPES_HPP

////////////////////////////////////////////////////////////////////////
// Bare minimal includes. Do not add any c++ includes here for speed.
////////////////////////////////////////////////////////////////////////

#include "my_cfg.hpp"

#include <stdint.h>
#include <string>

#if __GNUC__ >= 8
// warns about intentional truncation like % 10s!
#pragma GCC diagnostic ignored "-Wformat-truncation"
#endif

//
// Settings to override compiler errors
//
// `error' to treat this diagnostic as an error,
// `warning' to treat it like a warning (even if -Werror is in effect)
// `ignored' if the diagnostic is to be ignored
//
// #pragma GCC diagnostic warning "-Wformat"
// #pragma GCC diagnostic error "-Wformat"
// #pragma GCC diagnostic ignored "-Wformat"
//
#ifdef __clang__
/*code specific to clang compiler*/
#elif __GNUC__
/*code for GNU C compiler */
#elif _MSC_VER
/*usually has the version number in _MSC_VER*/
/*code specific to MSVC compiler*/
#elif __BORLANDC__
/*code specific to borland compilers*/
#elif __MINGW32__
/*code specific to mingw compilers*/
#endif

//
// Check if std::float16_t is defined
//
#ifdef __is_identifier
#if ! __is_identifier(_Float16)
#include <float.h>
#undef f16
#define f16 _Float16
#endif
#endif

#ifdef __has_include
#if __has_include(<stdfloat>)
#include <stdfloat>
#if __cplusplus > 202002L && defined(__STDCPP_FLOAT16_T__)
#undef f16
#define f16 std::float16_t
#endif
#endif
#endif

#ifndef f16
#define f16 float
#endif

// %d--> for int
//
// %u--> for unsigned int
//
// %ld--> for long int
//
// %lu--> for unsigned long int
//
// %lld--> for long long int
//
// %llu--> for unsigned long long int
//
// "%zu" workaround for moronic windows
// https://stackoverflow.com/questions/44382862/how-to-printf-a-size-t-without-warning-in-mingw-w64-gcc-7-1
//
#ifdef _WIN32
#ifdef _WIN64
#ifdef __MINGW32__
#define PRI_SIZE_T "llu"
#else
#define PRI_SIZE_T PRIu64
#endif
#else
#define PRI_SIZE_T PRIu32
#endif
#else
#define PRI_SIZE_T "zu"
#endif

////////////////////////////////////////////////////////////////////////////
// Misc
////////////////////////////////////////////////////////////////////////////

//
// Simple array routines
//
#define ARRAY_SIZE(my_array) ((int) (sizeof(my_array) / sizeof((my_array)[ 0 ])))

//
// Tired of compiler warnings on assuming int.
//
#define SIZEOF(my_array) ((int) sizeof(my_array))

//
// Works around some parentheses warnings that clang tidy ands and gcc doesn't like
//
#define AUTO(a) auto a // NOLINT

#define FOR_ALL_IN_ARRAY(my_iterator, my_array)                                                                                                 \
  for (AUTO(my_iterator) = (my_array); (my_iterator) < ((my_array) + ARRAY_SIZE(my_array)); (my_iterator)++)

//
// Types
//
#ifndef _BITS_STDINT_INTN_H
#ifndef __int8_t_defined
#ifndef _INT8_T
#define _INT8_T
typedef signed char int8_t;
#endif //_INT8_T

#ifndef _INT16_T
#define _INT16_T
typedef short int16_t;
#endif // _INT16_T

#ifndef _INT32_T
#define _INT32_T
typedef int int32_t;
#endif // _INT32_T

#ifndef _INT64_T
#define _INT64_T
typedef long long int64_t;
#endif // _INT64_T
#endif

#ifndef _UINT8_T
#define _UINT8_T
typedef unsigned char uint8_t;
#endif //_UINT8_T

#ifndef _UINT16_T
#define _UINT16_T
typedef unsigned short uint16_t;
#endif // _UINT16_T

#ifndef _UINT32_T
#define _UINT32_T
typedef unsigned int uint32_t;
#endif // _UINT32_T

#ifndef _UINT64_T
#define _UINT64_T
#ifndef __MINGW32__
#ifndef __MINGW64__
typedef unsigned long int uint64_t;
#endif
#endif
#endif // _UINT64_T
#endif

//
// msys functions seem to accept either / or \ so we don't need to worry.
//
#ifdef _WIN32
#define PATHSEP      ";"
#define DIR_SEP      "\\"
#define DIR_SEP_CHAR '\\'
#else
#define PATHSEP      ":"
#define DIR_SEP      "/"
#define DIR_SEP_CHAR '/'
#endif

#if defined(__GNUC__) || defined(__clang__)
#define CHECK_FORMAT_STR(a, b, c) __attribute__((format(a, b, c)))
#else
#define CHECK_FORMAT_STR(a, b, c)
#endif

class Charmap;
class Font;
class Game;
class HiScore;
class HiScores;
class Tex;
class Tile;
class Tp;
class Test;
class Wid;
class Fragment;
class FragmentAlt;
class WidPopup;

struct Level;
struct LevelSelect;
struct Levels;
struct Thing;
struct ThingExt;
struct ThingLight;
struct ThingPlayer;
struct Dmap;
struct SDL_Surface;

using Gamep        = class Game *;
using Dmapp        = struct Dmap *;
using Fontp        = class Font *;
using Levelp       = struct Level *;
using LevelSelectp = struct LevelSelect *;
using Levelsp      = struct Levels *;
using Texp         = class Tex *;
using ThingId      = unsigned int;
using ThingExtId   = uint16_t;
using ThingLightId = uint16_t;
using Thingp       = struct Thing *;
using ThingSlotp   = struct ThingSlot *;
using ThingExtp    = struct ThingExt *;
using ThingLightp  = struct ThingLight *;
using ThingPlayerp = struct ThingPlayer *;
using Tilep        = class Tile *;
using TpId         = uint32_t;
using Tpp          = class Tp *;
using Testp        = class Test *;
using Widp         = class Wid *;
using Fragmentp    = class Fragment *;
using FragmentAltp = class FragmentAlt *;
using LevelNum     = uint8_t;

#define NULL_THING ((Thingp) nullptr)
#define NULL_TP    ((Tpp) nullptr)

#endif // MY_TYPES_HPP
