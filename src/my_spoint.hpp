//
// Copyright goblinhack@gmail.com
//

#ifndef MY_SPOINT_HPP
#define MY_SPOINT_HPP

#include "my_math.hpp"

#include <math.h>

using spoint = struct MySpoint {
public:
  short x;
  short y;

  void operator+=(MySpoint const &b);
  void operator-=(MySpoint const &b);
  void operator*=(short b);
  void operator/=(short b);
  void unit();

  [[nodiscard]] auto length() const -> float;
};

auto operator<(spoint const &a, spoint const &b) -> bool; // Needed for std::map
auto operator==(spoint const &a, spoint const &b) -> bool;
auto operator+(spoint const &a, spoint const &b) -> spoint;
auto operator-(spoint const &a, spoint const &b) -> spoint;
auto operator*(spoint const &a, short b) -> spoint;
auto operator/(spoint const &a, short b) -> spoint;

static inline auto make_spoint(const spoint &f) -> spoint { return spoint(f.x, f.y); }
static inline auto make_spoint(spoint &f) -> spoint { return spoint(f.x, f.y); }
static inline auto make_spoint(short x, short y) -> spoint { return spoint(x, y); }

static inline auto sort_distance(const spoint &a, const spoint &b) -> int { return SORT_DISTANCE_f(a.x, a.y, b.x, b.y); }

static inline auto distance(const spoint &a, const spoint &b) -> float { return DISTANCE_f((float) a.x, (float) a.y, (float) b.x, (float) b.y); }

#endif // MY_SPOINT_HPP
