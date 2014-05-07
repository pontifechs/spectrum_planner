#ifndef __GUARD_UFLOAT_HPP__
#define __GUARD_UFLOAT_HPP__


#include <shaders/IUniform.hpp>

class UFloat : public IUniform
{
public :
	float val;

	UFloat()
		: val(0)
		{}

	UFloat(const Program& program, const std::string name, float _val)
		: IUniform(program, name), val(_val)
		{}

	UFloat(const Program& program, const std::string name)
		: IUniform(program, name), val(0.0)
		{}

	// Assignment
	UFloat& operator=(float rhs)
		{
			val = rhs;
			return (*this);
		}

	// +=
	UFloat& operator+=(float rhs) 
		{
			val += rhs;
			return (*this);
		}


	virtual void send() const
		{
			glUniform1f(m_id, val);
		}
};


#endif
