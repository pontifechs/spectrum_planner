
// Libs
#include <math.h>

// Classes
#include "math/Vec4.hpp"
#include "math/Vec3.hpp"

// Equality Overloads
bool Vec4::operator==(const Vec4& other) const {
  return (x == other.x) && (y == other.y) && (z == other.z) && (w == other.w);
}
bool Vec4::operator!=(const Vec4& other) const {
  return !(*this == other);
}

const Vec4 Vec4::operator-(const Vec4& rhs) const
{
  return Vec4(*this) -= rhs;
}

const Vec4 Vec4::operator+(const Vec4& rhs) const
{
  return Vec4(*this) += rhs;
}

const Vec4 Vec4::operator*(float scalar) const
{
  return Vec4(*this) *= scalar;
}


Vec4& Vec4::operator-=(const Vec4& rhs)
{
  x -= rhs.x;
  y -= rhs.y;
  z -= rhs.z;
  w -= rhs.w;
  return *this;
}

Vec4& Vec4::operator+=(const Vec4& rhs)
{
  x += rhs.x;
  y += rhs.y;
  z += rhs.z;
  w += rhs.w;
  return *this;
}

Vec4& Vec4::operator*=(float scalar)
{
  x *= scalar;
  y *= scalar;
  z *= scalar;
  w *= scalar;
  return *this;
}


// Constructor
Vec4::Vec4(float _x, float _y, float _z, float _w)
  : x(_x), y(_y), z(_z), w(_w)
{}

Vec4::Vec4(const Vec3& v3, float _w)
	: x(v3.x), y(v3.y), z(v3.z), w(_w)
{}

Vec4::Vec4()
  : x(0.0f), y(0.0f), z(0.0f), w(0.0f)
{}

// Dot-Product (4-space only).
float Vec4::dot(const Vec4& rhs) const
{
  return (x * rhs.x) + (y * rhs.y) + (z * rhs.z) + (w * rhs.w);
}

float Vec4::length() const
{
  return sqrt((x * x) + (y * y) + (z * z) + (w * w));
}

void Vec4::normalize()
{
  float len = this->length();
  x /= len;
  y /= len;
  z /= len;
	w /= len;
}

Vec4 Vec4::getNormalized() const
{
  Vec4 temp(x, y, z, w);
  temp.normalize();
  return temp;
}

const Vec4 operator*(float lhs, const Vec4& rhs)
{
  Vec4 ret(rhs);
  ret *= lhs;
  return ret;
}

