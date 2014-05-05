
#include <shaders/AMesh.hpp>


AMesh::AMesh()
{
	glGenVertexArrays(1, &m_vao);
}


// For now, just assume all meshes are 6 vertices for 2 triangles
// When I write a generic mesh class, I'll fit this over top.
void AMesh::attachAttribute(const Program& program, std::string name, float* data)
{
	glBindVertexArray(m_vao);
	
	GLuint vbo;
	glGenBuffers(1, &vbo);
	m_vbos.push_back(vbo);
	m_names.push_back(name);


	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	// 6 is number of vertices, 3 is number of floats per vertex
	glBufferData(GL_ARRAY_BUFFER, 6 * 3 * sizeof(float), data, GL_STATIC_DRAW);	

	GLuint attribLoc = glGetAttribLocation(program.GetId(), name.c_str());

	// again, 3 is number of components per vertex, GL_FLOAT is the type,
	// GL_FALSE says don't normalize (0-1), 0 is stride, i.e. tightly packed, 
	// last 0 is the offset to the first.
	// So theoretically, you could pack all of the data in the same array,
	// and still be able to distinguish between the different components (color, pos, etc)
	glVertexAttribPointer(attribLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(attribLoc);

	glBindVertexArray(0);
}


void AMesh::draw() const
{

	glBindVertexArray(m_vao);
	// 6 is the number of vertices
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void AMesh::sendTo(const Program& program) const
{
	glBindVertexArray(m_vao);
	
	for (int i = 0; i < m_vbos.size(); ++i)
	{
		GLuint vbo = m_vbos[i];
		std::string name = m_names[i];

		// Bind the buffer for use in glVertexAttribPointer
		glBindBuffer(GL_ARRAY_BUFFER, vbo);


		GLuint attribLoc = glGetAttribLocation(program.GetId(), name.c_str());

		// again, 3 is number of components per vertex, GL_FLOAT is the type,
		// GL_FALSE says don't normalize (0-1), 0 is stride, i.e. tightly packed, 
		// last 0 is the offset to the first.
		// So theoretically, you could pack all of the data in the same array,
		// and still be able to distinguish between the different components (color, pos, etc)
		glVertexAttribPointer(attribLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(attribLoc);
	}

	
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);

}


