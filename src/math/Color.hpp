#ifndef __GUARD_COLOR_HPP__
#define __GUARD_COLOR_HPP__

// Libs
#include <vector>

// Classes
#include "math/Vec4.hpp"


class Color : public Vec4 
{
public:
  
  static Color f2c(float f);
  static float c2f(Color c);
  static float clip(float f);
  
  bool operator==(const Color& rhs) const;
  bool operator!=(const Color& rhs) const;

  static Color linearCombo(Color c1, Color c2, float c1Toc2);
  static Color linearComboMulti(std::vector<Color> colors, std::vector<float> contribs);

  float r() const;
  float g() const;
  float b() const;
  float a() const;
  
  void setR(float _r);
  void setG(float _g);
  void setB(float _b);
  void setA(float _a);
  
  Color();
  Color(float _r, float _g, float _b, float _a);
  
  void blendIn(Color newColor);

  unsigned int ARGB();
  unsigned int RGBA();
  unsigned int XRGB();
  
  //virtual std::string toString();
};



// Basic Color Shortcuts
#define SOLID_WHITE 			Color(1.0, 1.0, 1.0, 1.0)
#define SOLID_LIGHT_GREY 			Color(0.75, 0.75, 0.75, 1.0)
#define SOLID_GREY 			Color(0.5, 0.5, 0.5, 1.0)
#define SOLID_DARK_GREY 			Color(0.25, 0.25, 0.25, 1.0)
#define SOLID_BLACK 			Color(0.0, 0.0, 0.0, 1.0)




#define SOLID_RED 			Color(1.0, 0.0, 0.0, 1.0)
#define SOLID_GREEN 			Color(0.0, 1.0, 0.0, 1.0)
#define SOLID_BLUE 			Color(0.0, 0.0, 1.0, 1.0)
#define SOLID_YELLOW 			Color(1.0, 1.0, 0.0, 1.0)
#define SOLID_CYAN 			Color(0.0, 1.0, 1.0, 1.0)
#define SOLID_MAGENTA 			Color(1.0, 0.0, 1.0, 1.0)

#define SOLID_DARK_RED 			Color(0.5, 0.0, 0.0, 1.0)
#define SOLID_DARK_GREEN 		Color(0.0, 0.5, 0.0, 1.0)
#define SOLID_DARK_BLUE 		Color(0.0, 0.0, 0.5, 1.0)
#define SOLID_DARK_YELLOW 		Color(0.5, 0.5, 0.0, 1.0)
#define SOLID_DARK_CYAN 		Color(0.0, 0.5, 0.5, 1.0)
#define SOLID_DARK_MAGENTA 		Color(1.0, 0.0, 1.0, 1.0)

// Half-Strength
#define TRANS_WHITE 			Color(0.0, 0.0, 0.0, 0.5)
#define TRANS_BLACK 			Color(1.0, 1.0, 1.0, 0.5)
#define TRANS_GREY 			Color(0.5, 0.5, 0.5, 0.5)

#define TRANS_RED 			Color(1.0, 0.0, 0.0, 0.5)
#define TRANS_GREEN 			Color(0.0, 1.0, 0.0, 0.5)
#define TRANS_BLUE 			Color(0.0, 0.0, 1.0, 0.5)
#define TRANS_YELLOW 			Color(1.0, 1.0, 0.0, 0.5)
#define TRANS_CYAN 			Color(0.0, 1.0, 1.0, 0.5)
#define TRANS_MAGENTA 			Color(1.0, 0.0, 1.0, 0.5)

#define TRANS_DARK_RED 			Color(0.5, 0.0, 0.0, 0.5)
#define TRANS_DARK_GREEN 		Color(0.0, 0.5, 0.0, 0.5)
#define TRANS_DARK_BLUE 		Color(0.0, 0.0, 0.5, 0.5)
#define TRANS_DARK_YELLOW 		Color(0.5, 0.5, 0.0, 0.5)
#define TRANS_DARK_CYAN 		Color(0.0, 0.5, 0.5, 0.5)
#define TRANS_DARK_MAGENTA 		Color(1.0, 0.0, 1.0, 0.5)

#define COLOR_ERROR       	       	Color(-1.0, -1.0, -1.0, -1.0)
#define COLOR_NONE                      Color(0.0, 0.0, 0.0, 0.0)

#endif
