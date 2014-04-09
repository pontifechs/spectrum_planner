
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
				std::cout << "IUniform sendTo error getting uniform location for " << m_name << std::endl;
			}
            //std::cout << "IUniform " << m_name << " to ProgramID " << m_id << std::endl;
			send();
		}

	virtual void send() const = 0;

	virtual ~IUniform() {}

protected:
	
	IUniform(const Program& program, const std::string name)
		: m_name(name)
        , m_id(glGetUniformLocation(program.GetId(), name.c_str()))
	{
		if (m_id == -1)
		{
			std::cout << "IUniform construction error getting uniform location for " << m_name << " " << program.GetId() << " " << m_id << std::endl;
		}
	}

    const std::string m_name;
    GLint m_id;     // location of uniform
	

};



#endif
