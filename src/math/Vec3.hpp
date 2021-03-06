#ifndef __GUARD_VEC3_HPP__
#define __GUARD_VEC3_HPP__


class Vec3 {
public:

  float x;
  float y;
  float z;

  bool operator==(const Vec3& other) const;
  bool operator!=(const Vec3& other) const;

  const Vec3 operator-(const Vec3& rhs) const;
  const Vec3 operator+(const Vec3& rhs) const;
  const Vec3 operator*(float scalar) const;
	// Element-wise multiplication
	const Vec3 operator*(const Vec3& rhs) const;

  
  Vec3& operator-=(const Vec3& rhs);
  Vec3& operator+=(const Vec3& rhs);
  Vec3& operator*=(float scalar);
	Vec3& operator*=(const Vec3& rhs);
  
  
  Vec3(float _x, float _y, float _z);
  Vec3();
  
  // 3-space functions
  float dot(const Vec3& rhs) const;
  Vec3 cross(const Vec3& rhs) const;
  
  void normalize();
  Vec3 getNormalized() const;
  float length() const;
};


// Non-Member Operators
const Vec3 operator*(float lhs, const Vec3& rhs);


#endif
