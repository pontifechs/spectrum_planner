
#include <math.h> //sqrt

#include <math/Vec2.hpp>


bool Vec2::operator==(const Vec2& other) const
{
	return (x == other.x) && (y == other.y);
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
	x -= rhs.x;
	y -= rhs.y;
	return *this;
}

Vec2& Vec2::operator+=(const Vec2& rhs)
{
	x += rhs.x;
	y += rhs.y;
	return *this;
}

Vec2& Vec2::operator*=(float scalar)
{
	x *= scalar;
	y *= scalar;
	return *this;
}

Vec2& Vec2::operator*=(const Vec2& rhs)
{
	x *= rhs.x;
	y *= rhs.y;
	return *this;
}


Vec2::Vec2(float _x, float _y)
	:x(_x), y(_y)
{}

Vec2::Vec2()
	:x(0.0f), y(0.0f)
{}

float Vec2::dot(const Vec2& rhs) const
{
	return (x * rhs.x) + (y * rhs.y);
}

float Vec2::cross(const Vec2& rhs) const
{
	return (x * rhs.y) - (y * rhs.x);
}

void Vec2::normalize()
{
	float len = this->length();
	x /= len;
	y /= len;
}

Vec2 Vec2::getNormalized() const
{
	Vec2 temp(x, y);
	temp.normalize();
	return temp;
}

float Vec2::length() const
{
	return sqrt((x * x) + (y * y));
}

const Vec2 operator*(float lhs, const Vec2& rhs)
{
	Vec2 ret(rhs);
	ret *= lhs;
	return ret;
}
