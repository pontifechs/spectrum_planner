#ifndef __GUARD_MAT4_HPP__
#define __GUARD_MAT4_HPP__

#include "math/Vec4.hpp"

class Mat4 {

public:
  /// Constructs the Identity matrx.
  Mat4();
  /// Constructs a matrix in which all cells contain fill
  /// @param[in] fill The value you wish all the cells to contain.
  Mat4(float fill);
  /// Constructs a matrix and fills it with the float data specified.
  Mat4(float m00, float m01, float m02, float m03,
			 float m10, float m11, float m12, float m13,
			 float m20, float m21, float m22, float m23,
			 float m30, float m31, float m32, float m33);
  /// Deconstructor
  ~Mat4();
  /// Copy Constructor
  Mat4(const Mat4& original);
  /// Assignment operator
  Mat4& operator= (const Mat4& rhs);

  const float* GetData() const;

  /// Mutable Accessor
  float& operator() (int row, int col);
  /// Const Accessor
  float  operator() (int row, int col) const;

  // += Operator overload
  Mat4& operator+= (const Mat4& rhs);
  // -= Operator overload
  Mat4& operator-= (const Mat4& rhs);
  // *= Operator overload
  Mat4& operator*= (const Mat4& rhs);
  
  // + Operator overload
  Mat4 operator+ (const Mat4& rhs) const;
  // - Operator overload
  Mat4 operator- (const Mat4& rhs) const;
  // * Operator overload
  Mat4 operator* (const Mat4& rhs) const;
  // * Operator overload
  Vec4 operator* (const Vec4& rhs) const;
  

  bool operator== (const Mat4& rhs) const;
  bool operator!= (const Mat4& rhs) const;


protected:
private:
  float* m_pData;        ///< The storage array

};

#endif
