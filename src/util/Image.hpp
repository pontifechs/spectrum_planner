
#ifndef __GUARD_IMAGE_HPP__
#define __GUARD_IMAGE_HPP__


#include <FreeImage.h>
#include <string>
#include <GL/glew.h>


class Image
{

public:
	Image();
	Image(std::string path);
	Image(int width, int height);
	Image(const char* rawPixels, int width, int height);
	
	// destructor
	~Image();
	// Copy constructor
	Image(const Image& original);
	// Assignment operator
	Image& operator= (const Image& rhs);



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
