
// Libs
#include <algorithm>
#include <stdint.h>

// Classes
#include "math/Color.hpp"



Color::Color()
  : Vec4(1.0f, 1.0f, 1.0f, 1.0f)
{}

Color::Color(float _r, float _g, float _b, float _a)
  : Vec4(_r, _g, _b, _a)
{}

float Color::r() const
{
  return x;
}

float Color::g() const
{
  return y;
}

float Color::b() const
{
  return z;
}

float Color::a() const
{
  return w;
}


Color Color::f2c(float num) {
  return Color(num, num, num, 1.0f);
}
float Color::c2f(Color c) {
  if (c.r() == c.g() && c.g() == c.b()) {
    return c.r();
  }
  else {
    return -1;
  }
}

Color Color::linearCombo(Color c1, Color c2, float c1Toc2) {
  if (c1Toc2 >= 1.0f) {
    return c1;
  }
  
  float c2Contrib = 1.0f - c1Toc2;

  float r = (c1.r() * c1Toc2) + (c2.r() * c2Contrib);
  float g = (c1.g() * c1Toc2) + (c2.g() * c2Contrib);
  float b = (c1.b() * c1Toc2) + (c2.b() * c2Contrib);
  float a = (c1.a() * c1Toc2) + (c2.a() * c2Contrib);

  return Color(r,g,b,a);
}

Color Color::linearComboMulti(std::vector<Color> colors, std::vector<float> contribs) {
  if (colors.size() != contribs.size()) {
    return COLOR_ERROR;
  }
  float r = 0;
  float g = 0;
  float b = 0;
  float a = 0;

  for (int i = 0; i < colors.size(); i++) {
    Color c = colors[i];
    float f = contribs[i];
    r += c.r() * f;
    g += c.g() * f;
    b += c.b() * f;
    a += c.a() * f;
  }
  
  clip(r); clip(g); clip(b); clip(a);
  return Color(r,g,b,a);
}

void Color::setR(float _r) {
  x = clip(_r);
}
void Color::setG(float _g) {
	y = clip(_g);
}
void Color::setB(float _b) {
	z = clip(_b);
}
void Color::setA(float _a) {
	w = clip(_a);
}

void Color::blendIn(Color newColor) {
  // Blend the new color into the old color according to this equation (simple alpha blending)
  // newColor = (1-alpha)existColor + (alpha)applyColor; (on a channel by channel basis).
  float alpha = newColor.a();
  float newR = ((1 - alpha) * this->r()) + ((alpha) * newColor.r());
  float newG = ((1 - alpha) * this->g()) + ((alpha) * newColor.g());
  float newB = ((1 - alpha) * this->b()) + ((alpha) * newColor.b());
  this->setR(newR);
  this->setG(newG);
  this->setB(newB);
}

unsigned int Color::ARGB() {
  unsigned int ret = ((int)(a() * 255) << 24);
  ret += ((int)(r() * 255) << 16);
  ret += ((int)(g() * 255) << 8);
  ret += ((int)(b() * 255) << 0);
  return ret;
}
unsigned int Color::RGBA() {
  unsigned int ret = ((int)(r() * 255) << 24);
  ret += ((int)(g() * 255) << 16);
  ret += ((int)(b() * 255) << 8);
  ret += ((int)(a() * 255) << 0);
  return ret;
}
unsigned int Color::XRGB() {
  unsigned int ret = ((int)(r() * 255) << 16);
  ret += ((int)(g() * 255) << 8);
  ret += ((int)(b() * 255) << 0);
  return ret;
}

bool Color::operator==(const Color& rhs) const
{
  bool ret = true;
  ret &= (int)(r() * 255) == (int)(rhs.r() * 255);
  ret &= (int)(g() * 255) == (int)(rhs.g() * 255);
  ret &= (int)(b() * 255) == (int)(rhs.b() * 255);
  ret &= (int)(a() * 255) == (int)(rhs.a() * 255);
  return ret;
}
bool Color::operator!=(const Color& rhs) const
{
  return !((*this) == rhs);
}

float Color::clip(float fl) {
  return std::max(0.0f, std::min(1.0f, fl));
}

