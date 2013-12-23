#ifndef __GUARD_MAT3_HPP__
#define __GUARD_MAT3_HPP__

#include <ostream>

#include "math/Vec3.hpp"

class Mat3 {

public:
  /// Constructs the Identity matrx.
  Mat3();
  /// Constructs a matrix in which all cells contain fill
  /// @param[in] fill The value you wish all the cells to contain.
  Mat3(float fill);
  /// Constructs a matrix and fills it with the float data specified.
  Mat3(float m00, float m01, float m02,
			 float m10, float m11, float m12,
			 float m20, float m21, float m22);
  /// Deconstructor
  ~Mat3();
  /// Copy Constructor
  Mat3(const Mat3& original);
  /// Assignment operator
  Mat3& operator= (const Mat3& rhs);

  const float* GetData() const;

  /// Mutable Accessor
  float& operator() (int row, int col);
  /// Const Accessor
  float  operator() (int row, int col) const;

  // += Operator overload
  Mat3& operator+= (const Mat3& rhs);
  // -= Operator overload
  Mat3& operator-= (const Mat3& rhs);
  // *= Operator overload
  Mat3& operator*= (const Mat3& rhs);
  
  // + Operator overload
  Mat3 operator+ (const Mat3& rhs) const;
  // - Operator overload
  Mat3 operator- (const Mat3& rhs) const;
  // * Operator overload
  Mat3 operator* (const Mat3& rhs) const;
  // * Operator overload
  Vec3 operator* (const Vec3& rhs) const;
  

  bool operator== (const Mat3& rhs) const;
  bool operator!= (const Mat3& rhs) const;

	void print(std::ostream& stream) const;


protected:
private:
  float* m_pData;        ///< The storage array

};

// ostream
std::ostream& operator<< (std::ostream& stream, const Mat3& matrix);


#endif
