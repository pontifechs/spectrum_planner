#include "math/Transform.hpp"

#include "math/Mat4.hpp"
#include "math/Vec3.hpp"


#define _USE_MATH_DEFINES
#include <math.h> // Trig functions

#include <stdio.h>

Mat4 Transform::Translate(float x, float y, float z)
{
  Mat4 ret;
  ret(0, 3) = x;
  ret(1, 3) = y;
  ret(2, 3) = z;
  return ret;
}

Mat4 Transform::Scale(float x, float y, float z)
{
  Mat4 ret;
  ret(0, 0) = x;
  ret(1, 1) = y;
  ret(2, 2) = z;
  return ret;
}

Mat4 Transform::RotateX(float theta)
{
  Mat4 ret;
  float cosT = cos(theta * M_PI / 180.0f);
  float sinT = sin(theta * M_PI / 180.0f);
  ret(1, 1) = cosT;
  ret(1, 2) = -sinT;
  ret(2, 2) = cosT;
  ret(2, 1) = sinT;
  return ret;
}

Mat4 Transform::RotateY(float theta)
{
  Mat4 ret;
  float cosT = cos(theta * M_PI / 180.0f);
  float sinT = sin(theta * M_PI / 180.0f);
  ret(0, 0) = cosT;
  ret(0, 2) = sinT;
  ret(2, 0) = -sinT;
  ret(2, 2) = cosT;
  return ret;
}

Mat4 Transform::RotateZ(float theta)
{
  Mat4 ret;
  float cosT = cos(theta * M_PI / 180.0f);
  float sinT = sin(theta * M_PI / 180.0f);
  ret(0, 0) = cosT;
  ret(0, 1) = -sinT;
  ret(1, 0) = sinT;
  ret(1, 1) = cosT;
  return ret;
}

Mat4 Transform::View(Vec3 eye, Vec3 point, Vec3 upGiven)
{
  Mat4 ret;
  
  // Ensure coordinate system is orthogonal
  Vec3 look = point - eye;
  look.normalize();
  Vec3 right = look.cross(upGiven);
  right.normalize();
  Vec3 up = right.cross(look);
  up.normalize();

  // First column
  ret(0, 0) = right.x();
  ret(1, 0) = up.x();
  ret(2, 0) = -look.x();
  // Second Column
  ret(0, 1) = right.y();
  ret(1, 1) = up.y();
  ret(2, 1) = -look.y();
  // Last Column
  ret(0, 2) = right.z();
  ret(1, 2) = up.z();
  ret(2, 2) = -look.z();

  // Translate to origin
  Mat4 trans = Transform::Translate(-eye.x(), -eye.y(), -eye.z());

  // Rotate then Translate
  return ret * trans;
}

Mat4 Transform::Projection(float fov, float aspect, float near, float far)
{
  Mat4 ret;
  float thetaRad = fov * M_PI / 180;
  float d = 1.0 / tan(thetaRad / 2.0);

  ret(0, 0) = d / aspect;
  ret(1, 1) = d;
  ret(2, 2) = (near + far) / (near - far);
  ret(3, 2) = -1;
  ret(2, 3) = (2*near*far) / (near - far);
  ret(3, 3) = 0;
  return ret;
}

