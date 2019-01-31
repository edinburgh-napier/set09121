#pragma once

#include <SFML/System.hpp>
#include <cmath>
#include <iomanip>  // std::setprecision
#include <iostream> // std::cout, std::fixed
#include <sstream>
#include <vector>

namespace sf {

typedef Vector2<size_t> Vector2ul;

// Returns the length of the vector
template <typename T> double length(const Vector2<T>& v) {
  return sqrt(v.x * v.x + v.y * v.y);
}

// Normalizes the vector
template <typename T> Vector2<T> normalize(const Vector2<T>& v) {
  Vector2<T> vector;
  double l = length(v);

  if (l != 0) {
    vector.x = v.x / l;
    vector.y = v.y / l;
  }

  return vector;
}

template <typename T>
Vector2<T> operator*(const Vector2<T>& left, const Vector2<T>& right) {
  Vector2<T> r = left;
  r.x *= right.x;
  r.y *= right.y;
  return r;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const Vector2<T>& v) {
  os << '(' << v.x << ',' << v.y << ')';
  return os;
}

template <typename T, typename U> Vector2<T> Vcast(const Vector2<U>& v) {
  return Vector2<T>(static_cast<T>(v.x), static_cast<T>(v.y));
};

static double deg2rad(double degrees) {
  return degrees * 4.0 * atan(1.0) / 180.0;
}

template <typename T>
Vector2<T> rotate(const Vector2<T>& v, const double degrees) {
  const double theta = deg2rad(degrees);

  const double cs = cos(theta);
  const double sn = sin(theta);

  return {(T)(v.x * cs - v.y * sn), (T)(v.x * sn + v.y * cs)};
}

} // namespace sf

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v) {
  os << "vec[";
  for (const auto& a : v) {
    os << a << ',';
  }
  os << ']';
  return os;
}

template <typename T>
std::string toStrDecPt(const uint16_t& dp, const T& i) {
  std::stringstream stream;
  stream << std::fixed << std::setprecision(dp) << i;
  return stream.str();
}