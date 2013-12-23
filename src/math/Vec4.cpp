
// Libs
#include <math.h>

// Classes
#include "math/Vec4.hpp"
#include "math/Vec3.hpp"

// Equality Overloads
bool Vec4::operator==(const Vec4& other) const {
  return (m_x == other.x()) && (m_y == other.y()) && (m_z == other.z()) && (m_w == other.w());
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
  m_x -= rhs.m_x;
  m_y -= rhs.m_y;
  m_z -= rhs.m_z;
  m_w -= rhs.m_w;
  return *this;
}

Vec4& Vec4::operator+=(const Vec4& rhs)
{
  m_x += rhs.m_x;
  m_y += rhs.m_y;
  m_z += rhs.m_z;
  m_w += rhs.m_w;
  return *this;
}

Vec4& Vec4::operator*=(float scalar)
{
  m_x *= scalar;
  m_y *= scalar;
  m_z *= scalar;
  m_w *= scalar;
  return *this;
}

// Getters
float Vec4::x() const {
  return m_x;
}	
float Vec4::y() const {
  return m_y;
}
float Vec4::z() const {
  return m_z;
}
float Vec4::w() const {
  return m_w;
}

// Setters
void Vec4::setX(float f) {
  m_x = f;
}
void Vec4::setY(float f) {
  m_y = f;
}
void Vec4::setZ(float f) {
  m_z = f;
}
void Vec4::setW(float f) {
  m_w = f;
}

// Constructor
Vec4::Vec4(float _x, float _y, float _z, float _w)
  : m_x(_x), m_y(_y), m_z(_z), m_w(_w)
{}

Vec4::Vec4(const Vec3& v3, float _w)
	: m_x(v3.x()), m_y(v3.y()), m_z(v3.z()), m_w(_w)
{}

Vec4::Vec4()
  : m_x(0.0f), m_y(0.0f), m_z(0.0f), m_w(0.0f)
{}

// Dot-Product (4-space only).
float Vec4::dot(const Vec4& rhs) const
{
  return (m_x * rhs.x()) + (m_y * rhs.y()) + (m_z * rhs.z()) + (m_w * rhs.w());
}

float Vec4::length() const
{
  return sqrt((m_x * m_x) + (m_y * m_y) + (m_z * m_z) + (m_w * m_w));
}

void Vec4::normalize()
{
  float len = this->length();
  m_x /= len;
  m_y /= len;
  m_z /= len;
	m_w /= len;
}

Vec4 Vec4::getNormalized() const
{
  Vec4 temp(m_x, m_y, m_z, m_w);
  temp.normalize();
  return temp;
}

const Vec4 operator*(float lhs, const Vec4& rhs)
{
  Vec4 ret(rhs);
  ret *= lhs;
  return ret;
}

