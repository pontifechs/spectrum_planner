
#ifndef __GUARD_AMESH_HPP__
#define __GUARD_AMESH_HPP__


#include <vector>

#include <shaders/Program.hpp>

class AMesh
{
public:

	AMesh();
	
	void attachAttribute(const Program& program, std::string name, float* data);

	void draw() const;

	void sendTo(const Program& program) const;
	
	
private:

	GLuint m_vao;
	std::vector<GLuint> m_vbos;
	std::vector<std::string> m_names;
};

#endif
