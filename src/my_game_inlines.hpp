//
// Copyright goblinhack@gmail.com
//

#ifndef MY_GAME_INLINES_HPP
#define MY_GAME_INLINES_HPP

#include "my_callstack.hpp"
#include "my_level.hpp"
#include "my_main.hpp"

[[nodiscard]] static inline auto game_level_get(Gamep g, Levelsp v, LevelArrIdx n) -> Levelp
{
#ifdef DEBUG_BUILD
  TRACE_DEBUG();

  if (g == nullptr) [[unlikely]] {
    ERR("game_level_get: no game pointer");
    return nullptr;
  }
  if (v == nullptr) [[unlikely]] {
    ERR("game_level_get: no levels pointer");
    return nullptr;
  }
#endif
  if (n >= LEVEL_ARR_IDX_MAX) [[unlikely]] {
    ERR("game_level_get: Exceeded max level: %u", n);
    return nullptr;
  }
  return &v->level[ n ];
}

[[nodiscard]] static inline auto game_level_get(Gamep g, Levelsp v) -> Levelp
{
#ifdef DEBUG_BUILD
  TRACE_DEBUG();

  if (g == nullptr) [[unlikely]] {
    ERR("game_level_get: no game pointer");
    return nullptr;
  }
  if (v == nullptr) [[unlikely]] {
    ERR("game_level_get: no levels pointer");
    return nullptr;
  }
#endif

  return game_level_get(g, v, v->level_arr_idx);
}

#endif
