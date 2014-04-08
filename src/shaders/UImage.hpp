
#ifndef __GUARD_UIMAGE_HPP__
#define __GUARD_UIMAGE_HPP__


#include <GL/glew.h>

#include <util/Image.hpp>
#include <shaders/IUniform.hpp>



class UImage : public IUniform, public Image
{
public:

	// This is a hack. But it will do for this demo
	static int total_loaded;

	GLuint m_tex_id;
	int m_tex_core;
    std::string uniformName;

	UImage(const Program& program,
				 const std::string name, 
				 const std::string path);

	virtual void send() const;
};


#endif
