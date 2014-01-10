
#ifndef __GUARD_IUNIFORM_HPP__
#define __GUARD_IUNIFORM_HPP__

#include <string>
#include <iostream>
#include <GL/glew.h>


#include <shaders/Program.hpp>

class IUniform
{
public:

	void sendTo(const Program& program)
		{
			m_id = glGetUniformLocation(program.GetId(), m_name.c_str());
			if (m_id == -1)
			{
				std::cout << "error getting uniform location for " << m_name << std::endl;
			}
			send();
		}

	virtual void send() const = 0;

	virtual ~IUniform() {}

protected:
	
	IUniform(const Program& program, const std::string name)
		: m_name(name), 
			m_id(glGetUniformLocation(program.GetId(), name.c_str()))
	{
		if (m_id == -1)
		{
			std::cout << "error getting uniform location for " << m_name << std::endl;
		}
	}

	const std::string m_name;
  GLint m_id;
	

};



#endif
