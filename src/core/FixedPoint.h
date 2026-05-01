#pragma once

#include <cstdint>

namespace core {

template <int precision = 8> class FixedPoint {
public:
  using Storage = int32_t;
  using Wide = int64_t;

  constexpr FixedPoint() : value(0) {}
  constexpr FixedPoint(int v) : value(v << precision) {}
  inline operator int() { return value >> precision; }

  static constexpr FixedPoint fromRaw(Storage raw) {
    FixedPoint f;
    f.value = raw;
    return f;
  }

  static FixedPoint fromFloat(float f) {
    return fromRaw(static_cast<Storage>(f * (1 << precision)));
  }

  float toFloat() const { return static_cast<float>(value) / (1 << precision); }

  int toInt() const { return value >> precision; }

  explicit operator int() const { return toInt(); }

  explicit operator float() const { return toFloat(); }

  FixedPoint operator+(const FixedPoint &other) const {
    return fromRaw(value + other.value);
  }

  FixedPoint operator-(const FixedPoint &other) const {
    return fromRaw(value - other.value);
  }

  FixedPoint &operator+=(const FixedPoint &other) {
    value += other.value;
    return *this;
  }

  FixedPoint &operator-=(const FixedPoint &other) {
    value -= other.value;
    return *this;
  }

  FixedPoint operator*(const FixedPoint &other) const {
    return fromRaw((Wide(value) * other.value) >> precision);
  }

  FixedPoint &operator*=(const FixedPoint &other) {
    value = (Wide(value) * other.value) >> precision;
    return *this;
  }

  FixedPoint operator/(const FixedPoint &other) const {
    return fromRaw((Wide(value) << precision) / other.value);
  }

  FixedPoint &operator/=(const FixedPoint &other) {
    value = (Wide(value) << precision) / other.value;
    return *this;
  }

  bool operator==(const FixedPoint &other) const {
    return value == other.value;
  }

  bool operator!=(const FixedPoint &other) const {
    return value != other.value;
  }

  bool operator<(const FixedPoint &other) const { return value < other.value; }

  bool operator>(const FixedPoint &other) const { return value > other.value; }

  bool operator<=(const FixedPoint &other) const {
    return value <= other.value;
  }

  bool operator>=(const FixedPoint &other) const {
    return value >= other.value;
  }

  FixedPoint operator*(int v) const { return fromRaw(value * v); }

  Storage raw() const { return value; }

private:
  Storage value;
};

} // namespace core
