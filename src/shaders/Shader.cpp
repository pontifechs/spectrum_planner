#include "shaders/Shader.hpp"

#include <fstream>


Shader::Shader()
{
}

Shader::Shader(std::string source_file, Shader::Type type)
{
  Initialize(source_file, type);
}

GLuint Shader::GetId() const
{
  return m_id;
}

void Shader::Initialize(std::string source_file, Shader::Type type)
{
  m_compiled = false;
  m_type = type;
  
  // Get source
  std::string line;
  std::string source;
  std::ifstream objfile(source_file.c_str());

  if (objfile.is_open())
  {
    while (objfile.good())
    {
      // Grab the line
      getline(objfile,line);
      source = source + line + "\n";
    }
    objfile.close();
  }
  else
  {
    printf("Couldn't open %s.", source_file.c_str());
  }

  // Register the shader with OpenGL
  m_id = glCreateShader(m_type);
  
  // Pass in the source
  const char* source_c_str = source.c_str();
  int source_length = source.length();
  glShaderSource(m_id, 1, &source_c_str, &source_length);
  glCompileShader(m_id);

  // Error Checking
  GLint comp_ok;
  glGetShaderiv(m_id, GL_COMPILE_STATUS, &comp_ok);

  if (GL_FALSE == comp_ok)
  {
    // Compilation failed, get error message.
    char* err_buf = new char[200];
    int act_leng = 0;
    glGetShaderInfoLog(m_id, 200, &act_leng, err_buf);
    printf("Shader Compilation failure in %s: %s\n", source_file.c_str(),  err_buf);
		// For now, will replace with real exceptions at some point.
		throw 0;
  }
  else
  {
    m_compiled = true;
  }

}

Shader::Type Shader::GetType() const
{
  return m_type;
}

bool Shader::IsCompiled() const
{
  return m_compiled;
}
