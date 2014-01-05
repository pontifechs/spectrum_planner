
#ifndef __GUARD_IUNIFORM_HPP__
#define __GUARD_IUNIFORM_HPP__

#include <string>
#include <iostream>
#include <GL/glew.h>


#include <shaders/Program.hpp>

class IUniform
{
public:

	virtual void send() const = 0;

protected:
	
	IUniform(const Program& program, const std::string name)
		:m_program(program), 
		 m_name(name), 
		 m_id(glGetUniformLocation(program.GetId(), name.c_str()))
	{
		if (m_id == -1)
		{
			std::cout << "error getting uniform location for " << name << std::endl;;
		}
	}

	const Program& m_program;
	const std::string m_name;
	const GLuint m_id;
	

};



#endif
