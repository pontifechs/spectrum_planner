#ifndef __GUARD_VEC4_HPP__
#define __GUARD_VEC4_HPP__

class Vec3;


class Vec4 {
private:
  float m_x;
  float m_y;
  float m_z;
  float m_w;
  
public:
  float x() const;
  float y() const;
  float z() const;
  float w() const;
  
  void setX(float f);
  void setY(float f);
  void setZ(float f);
  void setW(float f);
  
  bool operator==(const Vec4& other) const;
  bool operator!=(const Vec4& other) const;

  const Vec4 operator-(const Vec4& rhs) const;
  const Vec4 operator+(const Vec4& rhs) const;
  const Vec4 operator*(float scalar) const;
  

  
  Vec4& operator-=(const Vec4& rhs);
  Vec4& operator+=(const Vec4& rhs);
  Vec4& operator*=(float scalar);
  
  
  Vec4(float _x, float _y, float _z, float _w);
	Vec4(const Vec3& v3, float _w);
  Vec4();
  
  
  // 4-space functions
  float dot(const Vec4& rhs) const;
	
	// Drop to vec3
	Vec3& vec3() const;
  
  void normalize();
  Vec4 getNormalized() const;
  float length() const;
};


// Non-Member Operators
const Vec4 operator*(float lhs, const Vec4& rhs);



#endif
