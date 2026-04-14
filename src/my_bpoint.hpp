//
// Copyright goblinhack@gmail.com
//

#ifndef MY_BPOINT_HPP
#define MY_BPOINT_HPP

#include <my_math.hpp>

#include <math.h>

using bpoint = struct MyBpoint {
public:
  int8_t x;
  int8_t y;

  void operator+=(MyBpoint const &b);
  void operator-=(MyBpoint const &b);
  void operator*=(int8_t b);
  void operator/=(int8_t b);
  void unit();

  [[nodiscard]] auto length() const -> float;
};

auto operator<(bpoint const &a, bpoint const &b) -> bool; // Needed for std::map
auto operator==(bpoint const &a, bpoint const &b) -> bool;
auto operator+(bpoint const &a, bpoint const &b) -> bpoint;
auto operator-(bpoint const &a, bpoint const &b) -> bpoint;
auto operator*(bpoint const &a, int8_t b) -> bpoint;
auto operator/(bpoint const &a, int8_t b) -> bpoint;

static inline auto make_bpoint(const bpoint &f) -> bpoint { return bpoint(f.x, f.y); }
static inline auto make_bpoint(bpoint &f) -> bpoint { return bpoint(f.x, f.y); }
static inline auto make_bpoint(int8_t x, int8_t y) -> bpoint { return bpoint(x, y); }

static inline auto sort_distance(const bpoint &a, const bpoint &b) -> int { return SORT_DISTANCEf(a.x, a.y, b.x, b.y); }

static inline auto distance(const bpoint &a, const bpoint &b) -> float
{
  return DISTANCEf((float) a.x, (float) a.y, (float) b.x, (float) b.y);
}

auto adjacent_vert_or_horiz(const bpoint &a, const bpoint &b) -> bool;
auto unit(const bpoint &p) -> bpoint;
auto adjacent(const bpoint &a, const bpoint &b) -> bool;
auto adjacent_diagonal(const bpoint &a, const bpoint &b) -> bool;

#endif // MY_BPOINT_HPP
