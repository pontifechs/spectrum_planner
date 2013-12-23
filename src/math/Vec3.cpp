
// Libs
#include <math.h>

// Classes
#include "math/Vec3.hpp"


// Equality Overloads
bool Vec3::operator==(const Vec3& other) const {
  return (m_x == other.x()) && (m_y == other.y()) && (m_z == other.z());
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


Vec3& Vec3::operator-=(const Vec3& rhs)
{
  m_x -= rhs.m_x;
  m_y -= rhs.m_y;
  m_z -= rhs.m_z;
  return *this;
}

Vec3& Vec3::operator+=(const Vec3& rhs)
{
  m_x += rhs.m_x;
  m_y += rhs.m_y;
  m_z += rhs.m_z;
  return *this;
}

Vec3& Vec3::operator*=(float scalar)
{
  m_x *= scalar;
  m_y *= scalar;
  m_z *= scalar;
  return *this;
}

// Getters
float Vec3::x() const {
  return m_x;
}	
float Vec3::y() const {
  return m_y;
}
float Vec3::z() const {
  return m_z;
}

// Setters
void Vec3::setX(float f) {
  m_x = f;
}
void Vec3::setY(float f) {
  m_y = f;
}
void Vec3::setZ(float f) {
  m_z = f;
}

// Constructor
Vec3::Vec3(float _x, float _y, float _z)
  : m_x(_x), m_y(_y), m_z(_z)
{}

Vec3::Vec3()
  : m_x(0.0f), m_y(0.0f), m_z(0.0f)
{}



float Vec3::length() const
{
  return sqrt((m_x * m_x) + (m_y * m_y) + (m_z * m_z));
}

void Vec3::normalize()
{
  float len = this->length();
  m_x /= len;
  m_y /= len;
  m_z /= len;
}

Vec3 Vec3::getNormalized() const
{
  Vec3 temp(m_x, m_y, m_z);
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
  return (m_x * rhs.x()) + (m_y * rhs.y()) + (m_z * rhs.z());
}

// Cross-Product (3-space only).
Vec3 Vec3::cross(const Vec3& rhs) const
{
  float newX = (m_y * rhs.z()) - (m_z * rhs.y());
  float newY = (m_z * rhs.x()) - (m_x * rhs.z());
  float newZ = (m_x * rhs.y()) - (m_y * rhs.x());
  return Vec3(newX, newY, newZ);
}

