
#ifndef __GUARD_IMAGE_HPP__
#define __GUARD_IMAGE_HPP__


#include <FreeImage.h>
#include <string>
#include <GL/glew.h>


class Image
{

public:
	// Images are always a 32-bit per pixel format
	Image(std::string path);
	Image(int width, int height);
	~Image();

	FREE_IMAGE_FORMAT format() const;
	const void* get() const;
	void set(int x, int y, unsigned int value);
	int width() const;
	int height() const;


protected:
private:

	FREE_IMAGE_FORMAT m_format;
	FIBITMAP* m_image;

	int m_width;
	int m_height;
};


#endif
