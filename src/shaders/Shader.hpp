#ifndef __GUARD_SHADER_HPP__
#define __GUARD_SHADER_HPP__

#include <GL/glew.h>

#include <string>

class Shader
{
public:

  enum Type
  {
    FRAGMENT = GL_FRAGMENT_SHADER,
    VERTEX = GL_VERTEX_SHADER
  };
  
  Shader();
  Shader(std::string source_file, Type type);
  
  Type GetType() const;
  
  GLuint GetId() const;

  bool IsCompiled() const;

protected:

private:

  Type m_type;
  std::string m_source;
  unsigned int m_id;
  bool m_compiled;

  void Initialize(std::string source, Type type);

};






#endif
