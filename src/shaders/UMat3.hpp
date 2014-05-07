#ifndef __GUARD_UMAT3_HPP__
#define __GUARD_UMAT3_HPP__

#include <math/Mat3.hpp>
#include <shaders/IUniform.hpp>

class UMat3 : public IUniform, public Mat3
{
public:

	UMat3()
		: Mat3()
		{}

	UMat3(const Program& program, const std::string name, const Mat3& mat)
		: IUniform(program, name), Mat3(mat)
		{}
	
	UMat3(const Program& program, const std::string name)
		: IUniform(program, name), Mat3()
		{}

	UMat3& operator=(const Mat3& rhs)
		{
			Mat3::operator=(rhs);
			return (*this);
		}
	
	virtual void send() const
		{
			glUniformMatrix3fv(m_id,1, GL_FALSE, this->GetData()); 
		}
	
};

#endif
