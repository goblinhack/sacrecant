//
// Copyright goblinhack@gmail.com
//

#include "my_bpoint.hpp"
#include "my_math.hpp"

#include <algorithm>
#include <cmath>

auto operator<(bpoint const &a, bpoint const &b) -> bool { return (a.x < b.x) || (a.x == b.x && a.y < b.y); }
auto operator+(bpoint const &a, bpoint const &b) -> bpoint { return bpoint(a.x + b.x, a.y + b.y); }
auto operator-(bpoint const &a, bpoint const &b) -> bpoint { return bpoint(a.x - b.x, a.y - b.y); }
auto operator*(bpoint const &a, int8_t b) -> bpoint { return bpoint(a.x * b, a.y * b); }
auto operator/(bpoint const &a, int8_t b) -> bpoint { return bpoint(a.x / b, a.y / b); }
auto operator==(bpoint const &a, bpoint const &b) -> bool { return (a.x == b.x) && (a.y == b.y); }

void bpoint::operator+=(bpoint const &b)
{
  x += b.x;
  y += b.y;
}

void bpoint::operator-=(bpoint const &b)
{
  x -= b.x;
  y -= b.y;
}

void bpoint::operator*=(int8_t b)
{
  x *= b;
  y *= b;
}

void bpoint::operator/=(int8_t b)
{
  x /= b;
  y /= b;
}

auto bpoint::length() const -> float { return (static_cast< float >(sqrt((x * x) + (y * y)))); }

void bpoint::unit()
{
  int8_t const len = static_cast< int >(sqrt((x * x) + (y * y)));
  x /= len;
  y /= len;
}

//
// Are points adjacent on the grid to each other?
//
auto adjacent_vert_or_horiz(const bpoint &a, const bpoint &b) -> bool
{
  if ((a.x != b.x) && (a.y != b.y)) {
    return false;
  }
  if (abs(a.x - b.x) > 1) {
    return false;
  }
  if (abs(a.y - b.y) > 1) {
    return false;
  }
  return true;
}

//
// Are points adjacent to each other?
//
auto adjacent(const bpoint &a, const bpoint &b) -> bool
{
  if (abs(a.x - b.x) > 1) {
    return false;
  }
  if (abs(a.y - b.y) > 1) {
    return false;
  }
  return true;
}

auto unit(const bpoint &p) -> bpoint
{
  auto o = p;

  o.x = std::min< int8_t >(o.x, 1);
  o.y = std::min< int8_t >(o.y, 1);
  o.x = std::max< int >(o.x, -1);
  o.y = std::max< int >(o.y, -1);
  return o;
}
