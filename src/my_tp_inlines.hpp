
//
// Copyright goblinhack@gmail.com
//

#ifndef MY_TP_INLINES_HPP
#define MY_TP_INLINES_HPP

#ifdef DEBUG_BUILD
#include "my_callstack.hpp"
#endif

#include "my_tp.hpp"
#include "my_tp_class.hpp"

[[nodiscard]] static inline auto tp_temperature_burns_at_get(Tpp tp) -> int
{
#ifdef DEBUG_BUILD
  TRACE_DEBUG(); // expensive
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return 0;
  }
#endif
  return tp->temperature_burns_at;
}

[[nodiscard]] static inline auto tp_temperature_melts_at_get(Tpp tp) -> int
{
#ifdef DEBUG_BUILD
  TRACE_DEBUG(); // expensive
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return 0;
  }
#endif
  return tp->temperature_melts_at;
}

[[nodiscard]] static inline auto tp_temperature_initial_get(Tpp tp) -> int
{
#ifdef DEBUG_BUILD
  TRACE_DEBUG(); // expensive
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return 0;
  }
#endif
  return tp->temperature_initial;
}

extern TpVec tp_vec;

[[nodiscard]] static inline auto tp_find(TpId id) -> Tpp
{
#ifdef DEBUG_BUILD
  TRACE_DEBUG(); // expensive

  if (! id) [[unlikely]] {
    CROAK("tp_find: thing template %" PRIX16 " invalid id", id);
    return nullptr;
  }

  if (static_cast< int >(id) - 1 > static_cast< int >(tp_vec.size())) {
    CROAK("tp_find: thing template %" PRIX16 " bad id, beyond size of tp_vec", id);
    return nullptr;
  }
#else

  if (id == 0U) [[unlikely]] {
    return nullptr;
  }

  if (id > tp_vec.size()) [[unlikely]] {
    return nullptr;
  }
#endif

  auto *result = tp_vec[ id - 1 ];

#ifdef DEBUG_BUILD
  if (result == nullptr) {
    CROAK("tp_find: thing template %" PRIX16 " not found", id);
    return nullptr;
  }
#endif

  return result;
}

#endif
