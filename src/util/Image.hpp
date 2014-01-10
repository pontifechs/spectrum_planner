
#ifndef __GUARD_IMAGE_HPP__
#define __GUARD_IMAGE_HPP__


#include <FreeImage.h>
#include <string>
#include <GL/glew.h>


class Image
{

public:
	Image(std::string path);
	Image(const char* rawPixels, int width, int height);
	~Image();

	const void* get() const;

	int width() const;
	int height() const;

	bool save(std::string path) const;


protected:
private:

	FIBITMAP* m_image;

	int m_width;
	int m_height;
};


#endif
