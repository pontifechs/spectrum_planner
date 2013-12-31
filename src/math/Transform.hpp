#ifndef __GUARD_TRANSFORM_HPP__
#define __GUARD_TRANSFORM_HPP__

class Mat4;
class Mat3;
class Vec4;
class Vec3;
class Vec2;

namespace Transform
{
	
  // -------------
  // 3D transforms
  // -------------
 
  // Model Transforms
  Mat4 RotateX(float theta);
  Mat4 RotateY(float theta);
  Mat4 RotateZ(float theta);
  
  Mat4 Translate(float x, float y, float z);
  Mat4 Scale(float x, float y, float z);

  // Coordinate Frame Transforms
  Mat4 View(Vec3 eye, Vec3 point, Vec3 up);
  Mat4 Projection(float fov, float aspect, float near, float far);


  // -------------
  // 2D transforms
  // -------------
 

	Mat3 Rotate(float theta);
	
	Mat3 Translate(Vec2 d);
	
	Mat3 RotateTranslate(float theta, Vec2 d);

	

};





#endif
