
#ifndef __GUARD_UVEC3_HPP__
#define __GUARD_UVEC3_HPP__

#include <GL/glew.h>

#include <math/Vec3.hpp>
#include <shaders/IUniform.hpp>

class UVec3 : public IUniform, public Vec3
{
public:
	UVec3(const Program& program, const std::string name, float x, float y, float z)
		: IUniform(program, name), Vec3(x,y,z)
		{}

	UVec3(const Program& program, const std::string name)
		: IUniform(program, name), Vec3()
		{}

	virtual void send() const
		{
			glUniform3f(m_id, m_x, m_y, m_z);
		}
};


#endif
