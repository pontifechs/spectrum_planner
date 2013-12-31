
#include <math.h> //sqrt

#include <math/Vec2.hpp>


float Vec2::x() const
{
	return m_x;
}

float Vec2::y() const
{
	return m_y;
}

void Vec2::setX(float f)
{
	m_x = f;
}

void Vec2::setY(float f)
{
	m_y = f;
}

bool Vec2::operator==(const Vec2& other) const
{
	return (m_x == other.x()) && (m_y == other.y());
}

bool Vec2::operator!=(const Vec2& other) const
{
	return !(*this == other);
}

const Vec2 Vec2::operator-(const Vec2& rhs) const
{	
	return Vec2(*this) -= rhs;
}

const Vec2 Vec2::operator+(const Vec2& rhs) const
{
	return Vec2(*this) += rhs;
}

const Vec2 Vec2::operator*(float scalar) const
{
	return Vec2(*this) *= scalar;
}

const Vec2 Vec2::operator*(const Vec2& rhs) const
{
	return Vec2(*this) *= rhs;
}

Vec2& Vec2::operator-=(const Vec2& rhs)
{
	m_x -= rhs.m_x;
	m_y -= rhs.m_y;
	return *this;
}

Vec2& Vec2::operator+=(const Vec2& rhs)
{
	m_x += rhs.m_x;
	m_y += rhs.m_y;
	return *this;
}

Vec2& Vec2::operator*=(float scalar)
{
	m_x *= scalar;
	m_y *= scalar;
	return *this;
}

Vec2& Vec2::operator*=(const Vec2& rhs)
{
	m_x *= rhs.m_x;
	m_y *= rhs.m_y;
	return *this;
}


Vec2::Vec2(float _x, float _y)
	:m_x(_x), m_y(_y)
{}

Vec2::Vec2()
	:m_x(0.0f), m_y(0.0f)
{}

float Vec2::dot(const Vec2& rhs) const
{
	return (m_x * rhs.m_x) + (m_y * rhs.m_y);
}

float Vec2::cross(const Vec2& rhs) const
{
	return (m_x * rhs.m_y) - (m_y * rhs.m_x);
}

void Vec2::normalize()
{
	float len = this->length();
	m_x /= len;
	m_y /= len;
}

Vec2 Vec2::getNormalized() const
{
	Vec2 temp(m_x, m_y);
	temp.normalize();
	return temp;
}

float Vec2::length() const
{
	return sqrt((m_x * m_x) + (m_y * m_y));
}

const Vec2 operator*(float lhs, const Vec2& rhs)
{
	Vec2 ret(rhs);
	ret *= lhs;
	return ret;
}
