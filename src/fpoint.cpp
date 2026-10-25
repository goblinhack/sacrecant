//
// Copyright goblinhack@gmail.com
//

#include "my_fpoint.hpp"
#include "my_math.hpp"

#include <cmath>

#include <numbers>

auto operator+(fpoint const &a, fpoint const &b) -> fpoint { return fpoint(a.x + b.x, a.y + b.y); }
auto operator-(fpoint const &a, fpoint const &b) -> fpoint { return fpoint(a.x - b.x, a.y - b.y); }
auto operator*(fpoint const &a, float b) -> fpoint { return fpoint(a.x * b, a.y * b); }
auto operator/(fpoint const &a, float b) -> fpoint { return fpoint(a.x / b, a.y / b); }
auto operator==(fpoint const &a, fpoint const &b) -> bool { return (a.x == b.x) && (a.y == b.y); }

void fpoint::operator+=(fpoint const &b)
{
  x += b.x;
  y += b.y;
}

void fpoint::operator-=(fpoint const &b)
{
  x -= b.x;
  y -= b.y;
}

void fpoint::operator*=(float b)
{
  x *= b;
  y *= b;
}

void fpoint::operator/=(float b)
{
  x /= b;
  y /= b;
}

auto fpoint::length() const -> float { return (sqrtf((x * x) + (y * y))); }

void fpoint::unit()
{
  float const len = sqrtf((x * x) + (y * y));
  x /= len;
  y /= len;
}

//
// Yields an angle between 0 and 360 deg radians - essentially, how much
// this point has been rotated about the origin.
//
auto angle_radians(const fpoint &p) -> float
{
  float const theta = asin(p.y / p.length());

  if (p.x > 0) {
    if (p.y > 0) {
      return theta;
    }
    return static_cast< float > RAD_360 + theta;
  }
  return std::numbers::pi_v< float > - theta;
}

auto rotate_radians(float angle, const fpoint &p, const fpoint &O) -> fpoint
{
  float s = 0;
  float c = 0;
  SINCOSF(angle, &s, &c);

  // translate point back to origin:
  float const X = p.x - O.x;
  float const Y = p.y - O.y;

  // rotate point
  float const xnew = (X * c) - (Y * s);
  float const ynew = (X * s) + (Y * c);

  // translate point back:
  return fpoint(xnew + O.x, ynew + O.y);
}

auto rotate_radians(const fpoint &p, float angle) -> fpoint
{
  float s = 0;
  float c = 0;
  SINCOSF(angle, &s, &c);

  float const X = p.x;
  float const Y = p.y;

  // rotate point
  float const xnew = (X * c) - (Y * s);
  float const ynew = (X * s) + (Y * c);

  return fpoint(xnew, ynew);
}

auto normal(const fpoint &p) -> fpoint
{
  float const length = sqrtf((p.x * p.x) + (p.y * p.y));
  return fpoint(p.x / length, p.y / length);
}

auto unit(const fpoint &p) -> fpoint
{
  float const length = sqrtf((p.x * p.x) + (p.y * p.y));
  return fpoint(p.x / length, p.y / length);
}

#ifdef is_unused_flag
uint8_t get_line_intersection(fpoint p0, fpoint p1, fpoint p2, fpoint p3, fpoint *intersect)
{
  float denominator = ((p3.y - p2.y) * (p1.x - p0.x)) - ((p3.x - p2.x) * (p1.y - p0.y));

  if (denominator == 0) {
    return false;
  }

  float a = p0.y - p2.y;
  float b = p0.x - p2.x;

  float numerator1 = ((p3.x - p2.x) * a) - ((p3.y - p2.y) * b);
  float numerator2 = ((p1.x - p0.x) * a) - ((p1.y - p0.y) * b);

  a = numerator1 / denominator;
  b = numerator2 / denominator;

  // if we cast these lines infinitely in both directions, they intersect
  // here:
  intersect->x = p0.x + (a * (p1.x - p0.x));
  intersect->y = p0.y + (a * (p1.y - p0.y));

  // if line1 is a segment and line2 is infinite, they intersect if:
  if ((a >= 0) && (a <= 1.0) && (b >= 0) && (b <= 1.0)) {
    return true;
  }

  return false;
}

uint8_t get_line_intersection(fpoint p0, fpoint p1, fpoint p2, fpoint p3)
{
  float denominator = ((p3.y - p2.y) * (p1.x - p0.x)) - ((p3.x - p2.x) * (p1.y - p0.y));

  if (denominator == 0) {
    return false;
  }

  float a = p0.y - p2.y;
  float b = p0.x - p2.x;

  float numerator1 = ((p3.x - p2.x) * a) - ((p3.y - p2.y) * b);
  float numerator2 = ((p1.x - p0.x) * a) - ((p1.y - p0.y) * b);

  a = numerator1 / denominator;
  b = numerator2 / denominator;

  // if line1 is a segment and line2 is infinite, they intersect if:
  if ((a >= 0) && (a <= 1.0) && (b >= 0) && (b <= 1.0)) {
    return true;
  }

  return false;
}

//
// Two lines we already know intersect.
//
uint8_t get_line_known_intersection(fpoint p0, fpoint p1, fpoint p2, fpoint p3, fpoint *intersect)
{
  float denominator = ((p3.y - p2.y) * (p1.x - p0.x)) - ((p3.x - p2.x) * (p1.y - p0.y));

  if (denominator == 0) {
    return false;
  }

  float a = p0.y - p2.y;
  float b = p0.x - p2.x;

  float numerator1 = ((p3.x - p2.x) * a) - ((p3.y - p2.y) * b);
  float numerator2 = ((p1.x - p0.x) * a) - ((p1.y - p0.y) * b);

  a = numerator1 / denominator;
  b = numerator2 / denominator;

  // if we cast these lines infinitely in both directions, they intersect
  // here:
  intersect->x = p0.x + (a * (p1.x - p0.x));
  intersect->y = p0.y + (a * (p1.y - p0.y));

  return true;
}

float dot (const fpoint b
{
  return x * b.x + y * b.y;
}
#endif

auto distance_to_line(fpoint P0, fpoint L0, fpoint L1, float *dist, fpoint *intersect_out) -> int
{
  fpoint intersect;
  float  mag = 0;
  float  U   = 0;

  //
  // Can get the squared distance to avoid this.
  //
  mag = distance(L1, L0);

  //
  // Project point P onto the line and then calc the dot product.
  //
  U = (((P0.x - L0.x) * (L1.x - L0.x)) + ((P0.y - L0.y) * (L1.y - L0.y))) / (mag * mag);

  if (U < 0.0F) {
    intersect = L0;
  } else if (U > 1.0F) {
    intersect = L1;
  } else {
    intersect.x = L0.x + (U * (L1.x - L0.x));
    intersect.y = L0.y + (U * (L1.y - L0.y));
  }

  *dist = distance(P0, intersect);

  if (intersect_out != nullptr) {
    *intersect_out = intersect;
  }

  if ((U < 0.0F) || (U > 1.0F)) {

    return 0; // closest P0 does not fall within the line segment
  }

  return 1;
}
