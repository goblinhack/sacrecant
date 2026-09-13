//
// Copyright goblinhack@gmail.com
//

#ifndef MY_FPOINT_HPP
#define MY_FPOINT_HPP

#include "my_bpoint.hpp"
#include "my_math.hpp"
#include "my_spoint.hpp"

#include <math.h>

using fpoint = struct MyFpoint {
public:
  float x;
  float y;

  void               operator+=(MyFpoint const &b);
  void               operator-=(MyFpoint const &b);
  void               operator*=(float b);
  void               operator/=(float b);
  [[nodiscard]] auto length() const -> float;
  void               unit();
};

auto operator==(fpoint const &a, fpoint const &b) -> bool;
auto operator+(fpoint const &a, fpoint const &b) -> fpoint;
auto operator-(fpoint const &a, fpoint const &b) -> fpoint;
auto operator*(fpoint const &a, float b) -> fpoint;
auto operator/(fpoint const &a, float b) -> fpoint;

auto rotate_radians(float angle, const fpoint &p, const fpoint &origin) -> fpoint;
auto rotate_radians(const fpoint &p, float angle) -> fpoint;
auto normal(const fpoint &p) -> fpoint;
auto unit(const fpoint &p) -> fpoint;

auto angle_radians(const fpoint &p) -> float;
auto distance_to_line(fpoint P0, fpoint L0, fpoint L1, float *dist, fpoint *intersect_out) -> int;

static inline auto make_fpoint(const spoint f) -> fpoint { return fpoint(f.x, f.y); }

static inline auto make_fpoint(const bpoint f) -> fpoint { return fpoint(f.x, f.y); }

static inline auto sort_distance(const fpoint &a, const fpoint &b) -> float { return SORT_DISTANCE_f(a.x, a.y, b.x, b.y); }

static inline auto distance(const fpoint &a, const fpoint &b) -> float { return DISTANCE_f(a.x, a.y, b.x, b.y); }

//
// round() is better than floor() as if we do a jump on a diagonal, that is 0.707
// units. We want to round to 1.0 so the diagonal move works and not 0.0.
//
static inline auto make_spoint(const fpoint &f) -> spoint
{
  return spoint(static_cast< short >(roundf(f.x)), static_cast< short >(roundf(f.y)));
}

static inline auto make_bpoint(const fpoint &f) -> bpoint
{
  return bpoint(static_cast< int8_t >(roundf(f.x)), static_cast< int8_t >(roundf(f.y)));
}

#endif // MY_FPOINT_HPP
