
#include "math/Mat3.hpp"


// Libs
#include <iostream>

// Classes




/// Constructs the Identity matrx.
Mat3::Mat3() 
{
  m_pData = new float[9];
  m_pData[0]  = 1.0f; m_pData[3]  = 0.0f; m_pData[6]  = 0.0f;
  m_pData[1]  = 0.0f; m_pData[4]  = 1.0f; m_pData[7]  = 0.0f;
  m_pData[2]  = 0.0f; m_pData[5]  = 0.0f; m_pData[8] = 1.0f;
}

/// Constructs a matrix in which all cells contain fill
/// @param[in] fill The value you wish all the cells to contain.
Mat3::Mat3(float fill)
{
  m_pData = new float[9];
  m_pData[0]  = fill; m_pData[3]  = fill; m_pData[6]  = fill; 
  m_pData[1]  = fill; m_pData[4]  = fill; m_pData[7]  = fill; 
  m_pData[2]  = fill; m_pData[5]  = fill; m_pData[8] = fill; 
}

/// Constructs a matrix and fills it with the float data specified.
Mat3::Mat3(float m00, float m01, float m02,
					 float m10, float m11, float m12,
					 float m20, float m21, float m22)
{
  m_pData = new float[9];
  m_pData[0]  = m00; m_pData[3]  = m01; m_pData[6]  = m02; 
  m_pData[1]  = m10; m_pData[4]  = m11; m_pData[7]  = m12;
  m_pData[2]  = m20; m_pData[5]  = m21; m_pData[8] = m22;
}

/// Deconstructor
Mat3::~Mat3()
{
  delete[] m_pData;
}

/// Copy Constructor
Mat3::Mat3(const Mat3& original)
{
  m_pData = new float[9];
  for (int row = 0; row < 3; row++)
  {
    for (int col = 0; col < 3; col++)
    {
      m_pData[col * 3 + row] = original(row, col);
    }
  }
}
/// Assignment operator
Mat3& Mat3::operator= (const Mat3& rhs)
{
  // De-allocate m_pData
  delete[] m_pData;
  m_pData = new float[9];
  for (int row = 0; row < 3; row++)
  {
    for (int col = 0; col < 3; col++)
    {
      m_pData[col * 3 + row] = rhs(row, col);
    }
  }
  return *this;
}

/// Mutable Accessor
float& Mat3::operator() (int row, int col)
{
  return m_pData[col * 3 + row];
}

/// Const Accessor
float  Mat3::operator() (int row, int col) const
{
  return m_pData[col * 3 + row];
}

// += Operator overload
Mat3& Mat3::operator+= (const Mat3& rhs)
{
  for (int i = 0; i < 9; i++)
  {
    m_pData[i] += rhs.m_pData[i];
  }
  return *this;
}

// -= Operator overload
Mat3& Mat3::operator-= (const Mat3& rhs)
{
  for (int i = 0; i < 9; i++)
  {
    m_pData[i] -= rhs.m_pData[i];
  }
  return *this;
}

// *= Operator overload
Mat3& Mat3::operator*= (const Mat3& rhs) 
{
  float temp[9];

	// Row 1
	temp[0] = 
		(m_pData[0] * rhs.m_pData[0]) +
		(m_pData[3] * rhs.m_pData[1]) +
		(m_pData[6] * rhs.m_pData[2]);

	temp[3] =
		(m_pData[0] * rhs.m_pData[3]) +
		(m_pData[3] * rhs.m_pData[4]) +
		(m_pData[6] * rhs.m_pData[5]);

	temp[6] =
		(m_pData[0] * rhs.m_pData[6]) +
		(m_pData[3] * rhs.m_pData[7]) +
		(m_pData[6] * rhs.m_pData[8]);

	// Row 2
	temp[1] = 
		(m_pData[1] * rhs.m_pData[0]) +
		(m_pData[4] * rhs.m_pData[1]) +
		(m_pData[7] * rhs.m_pData[2]);

	temp[4] =
		(m_pData[1] * rhs.m_pData[3]) +
		(m_pData[4] * rhs.m_pData[4]) +
		(m_pData[7] * rhs.m_pData[5]);

	temp[7] =
		(m_pData[1] * rhs.m_pData[6]) +
		(m_pData[4] * rhs.m_pData[7]) +
		(m_pData[7] * rhs.m_pData[8]);

	// Row 3
	temp[2] = 
		(m_pData[2] * rhs.m_pData[0]) +
		(m_pData[5] * rhs.m_pData[1]) +
		(m_pData[8] * rhs.m_pData[2]);

	temp[5] =
		(m_pData[2] * rhs.m_pData[3]) +
		(m_pData[5] * rhs.m_pData[4]) +
		(m_pData[8] * rhs.m_pData[5]);

	temp[8] =
		(m_pData[2] * rhs.m_pData[6]) +
		(m_pData[5] * rhs.m_pData[7]) +
		(m_pData[8] * rhs.m_pData[8]);


  for (int i = 0; i < 9; i++)
  {
    m_pData[i] = temp[i];
  }
  return *this;
}  

// + Operator overload
Mat3 Mat3::operator+ (const Mat3& rhs) const
{
  Mat3 result = *this;
  result += rhs;
  return result;
}

// - Operator overload
Mat3 Mat3::operator- (const Mat3& rhs) const
{
  Mat3 result = *this;
  result -= rhs;
  return result;
}

// * Operator overload
Mat3 Mat3::operator* (const Mat3& rhs) const
{
  Mat3 result = *this;
  result *= rhs;
  return result;
}

Vec3 Mat3::operator* (const Vec3& rhs) const
{
  float x = 
    (m_pData[0] * rhs.x) + 
    (m_pData[3] * rhs.y) + 
    (m_pData[6] * rhs.z);
  float y = 
    (m_pData[1] * rhs.x) + 
    (m_pData[4] * rhs.y) + 
    (m_pData[7] * rhs.z);

  float z = 
    (m_pData[2] * rhs.x) + 
    (m_pData[5] * rhs.y) + 
    (m_pData[8] * rhs.z);
  return Vec3(x, y, z);
}

bool Mat3::operator== (const Mat3& rhs) const
{
  for (int i = 0; i < 9; i++)
  {
    if (m_pData[i] != rhs.m_pData[i])
    {
      return false;
    }
  }
  return true;
}

bool Mat3::operator!= (const Mat3& rhs) const
{
  return !(*this == rhs);
}
  
  
const float* Mat3::GetData() const
{
  return m_pData;
}


void Mat3::print(std::ostream& stream) const
{
	stream <<"{"<< m_pData[0] <<","<< m_pData[3] <<","<< m_pData[6] <<","<< std::endl;
	stream << m_pData[1] <<","<< m_pData[4] <<","<< m_pData[7] <<","<< std::endl;
	stream << m_pData[2] <<","<< m_pData[5] <<","<< m_pData[8] <<"}"<< std::endl;
}

std::ostream& operator<<(std::ostream& stream,const Mat3& m) {
	m.print(stream);
	return stream;
}
