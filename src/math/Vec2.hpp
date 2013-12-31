#ifndef __GUARD_VEC2_HPP__
#define __GUARD_VEC2_HPP__


class Vec2 {
protected:
  float m_x;
  float m_y;
  
public:
  float x() const;
  float y() const;
  
  void setX(float f);
  void setY(float f);

  bool operator==(const Vec2& other) const;
  bool operator!=(const Vec2& other) const;

  const Vec2 operator-(const Vec2& rhs) const;
  const Vec2 operator+(const Vec2& rhs) const;
  const Vec2 operator*(float scalar) const;
	// Element-wise multiplication
	const Vec2 operator*(const Vec2& rhs) const;
  
  Vec2& operator-=(const Vec2& rhs);
  Vec2& operator+=(const Vec2& rhs);
  Vec2& operator*=(float scalar);
	Vec2& operator*=(const Vec2& rhs);
	
  Vec2(float _x, float _y);
  Vec2();
  
  // 3-space functions
  float dot(const Vec2& rhs) const;
  float cross(const Vec2& rhs) const;
  
  void normalize();
  Vec2 getNormalized() const;
  float length() const;
};


// Non-Member Operators
const Vec2 operator*(float lhs, const Vec2& rhs);


#endif
