

// Libs
#include <math.h> //sqrt

// Classes
#include "math/Vec3.hpp"



// Equality Overloads
bool Vec3::operator==(const Vec3& other) const {
  return (x == other.x) && (y == other.y) && (z == other.z);
}
bool Vec3::operator!=(const Vec3& other) const {
  return !(*this == other);
}

const Vec3 Vec3::operator-(const Vec3& rhs) const
{
  return Vec3(*this) -= rhs;
}

const Vec3 Vec3::operator+(const Vec3& rhs) const
{
  return Vec3(*this) += rhs;
}

const Vec3 Vec3::operator*(float scalar) const
{
  return Vec3(*this) *= scalar;
}

const Vec3 Vec3::operator*(const Vec3& rhs) const
{
	return Vec3(*this) *= rhs;
}


Vec3& Vec3::operator-=(const Vec3& rhs)
{
  x -= rhs.x;
  y -= rhs.y;
  z -= rhs.z;
  return *this;
}

Vec3& Vec3::operator+=(const Vec3& rhs)
{
  x += rhs.x;
  y += rhs.y;
  z += rhs.z;
  return *this;
}

Vec3& Vec3::operator*=(float scalar)
{
  x *= scalar;
  y *= scalar;
  z *= scalar;
  return *this;
}

Vec3& Vec3::operator*=(const Vec3& rhs)
{
	x *= rhs.x;
	y *= rhs.y;
	z *= rhs.z;
	return *this;
}


// Constructor
Vec3::Vec3(float _x, float _y, float _z)
  : x(_x), y(_y), z(_z)
{}

Vec3::Vec3()
  : x(0.0f), y(0.0f), z(0.0f)
{}



float Vec3::length() const
{
  return sqrt((x * x) + (y * y) + (z * z));
}

void Vec3::normalize()
{
  float len = this->length();
  x /= len;
  y /= len;
  z /= len;
}

Vec3 Vec3::getNormalized() const
{
  Vec3 temp(x, y, z);
  temp.normalize();
  return temp;
}

const Vec3 operator*(float lhs, const Vec3& rhs)
{
  Vec3 ret(rhs);
  ret *= lhs;
  return ret;
}


// Dot-Product (3-space only).
float Vec3::dot(const Vec3& rhs) const
{
  return (x * rhs.x) + (y * rhs.y) + (z * rhs.z);
}

// Cross-Product (3-space only).
Vec3 Vec3::cross(const Vec3& rhs) const
{
  float newX = (y * rhs.z) - (z * rhs.y);
  float newY = (z * rhs.x) - (x * rhs.z);
  float newZ = (x * rhs.y) - (y * rhs.x);
  return Vec3(newX, newY, newZ);
}

