
#ifndef __GUARD_IMAGE_HPP__
#define __GUARD_IMAGE_HPP__


#include <FreeImage.h>
#include <string>
#include <GL/glew.h>


class Image
{

public:

	Image(std::string path);
	~Image();

	FREE_IMAGE_FORMAT format() const;
	const GLubyte* get() const;
	int width() const;
	int height() const;


protected:
private:

	FREE_IMAGE_FORMAT m_format;
	GLubyte* m_rawData;

	int m_width;
	int m_height;
};


#endif
