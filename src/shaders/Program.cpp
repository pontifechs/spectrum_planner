#include <stdio.h>

#include "shaders/Program.hpp"



Program::Program()
{
}

Program::Program(Shader vert, Shader frag)
{
	Build(vert, frag);
}


bool Program::IsBuilt() const
{
  return m_built;
}

void Program::Build(Shader vert, Shader frag)
{

  m_vert = vert;
  m_frag = frag;

	if (!(m_vert.IsCompiled() && m_frag.IsCompiled()))
	{
		// Both shaders must be compiled.
		throw 0;
	}

  m_id = glCreateProgram();
  
  glAttachShader(m_id, vert.GetId());
  glAttachShader(m_id, frag.GetId());
  glLinkProgram(m_id);

  // Check the Link.
  GLint link_ok;
  glGetProgramiv(m_id, GL_LINK_STATUS, &link_ok);

  if (GL_FALSE == link_ok)
  {
    // Link failed, get error message.
    char* err_buf = new char[200];
    int act_leng = 0;
    glGetProgramInfoLog(m_id, 200, &act_leng, err_buf);
    printf("Shader Link failure: %s\n", err_buf);
    return;
  }
    
  // Everything is ready to rock and roll.
  m_built = true;
}

void Program::Load() const
{
  glUseProgram(m_id);  
}

Shader Program::GetShader(Shader::Type type) const
{ 
  Shader ret;
  switch(type)
  {
  case Shader::VERTEX:
    ret = m_vert;
    break;
  case Shader::FRAGMENT:
    ret = m_frag;
    break;
  default:
    // Should never get here
    break;
  }
  return ret;
}


GLuint Program::GetId() const
{
  return m_id;
}
