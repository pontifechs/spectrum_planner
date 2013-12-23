
#ifndef __GUARD_PROGRAM_HPP__
#define __GUARD_PROGRAM_HPP__

#include <GL/glew.h>

#include "shaders/Shader.hpp"

#include <string>

class Program
{
public:
  Program();

  void Build(Shader vert, Shader frag);

  void Load();

  Shader GetShader(Shader::Type type) const;
  void SetShader(Shader::Type type, Shader shader);
  
  bool IsBuilt() const;

  GLuint GetId() const;
  
private:
  Shader m_vert;
  Shader m_frag;

  GLuint m_id;
  
  bool m_built;

};


#endif
