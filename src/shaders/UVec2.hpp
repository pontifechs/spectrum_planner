#ifndef __GUARD_UVEC2_HPP__
#define __GUARD_UVEC2_HPP__

#include <math/Vec2.hpp>
#include <shaders/IUniform.hpp>


class UVec2 : public Vec2, public IUniform
{
public:
	UVec2(const Program& program, const std::string name, float x, float y)
		: IUniform(program, name), Vec2(x,y)
		{}
	
	UVec2(const Program& program, const std::string name)
		: IUniform(program, name)
		{}

	virtual void send() const
		{
			glUniform2f(m_id, m_x, m_y);
		}

};


#endif
