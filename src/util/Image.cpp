
#include "util/Image.hpp"

#include <iostream>

Image::Image()
{
	m_image = FreeImage_Allocate(640, 480, 32);
}

Image::Image(std::string path)
{
	// Load a texture
	FREE_IMAGE_FORMAT format = FreeImage_GetFileType(path.c_str(),0);
	m_image = FreeImage_ConvertTo32Bits(FreeImage_Load(format, path.c_str()));

	if (m_image == NULL)
	{
		std::cout << "Error loading " << path << std::endl;
		throw 0;
	}
 
	m_width = FreeImage_GetWidth(m_image);
	m_height = FreeImage_GetHeight(m_image);

	//  For now, I'm just using GL_BGR when going back and forth between OpenGL
	//  and FreeImage
	// //FreeImage loads in BGR format, so you need to swap some bytes(Or use GL_BGR).
	// for (int j = 0; j < m_height; ++j)
	// {
	// 	char* row = (char*) FreeImage_GetScanLine(m_image, j);

	// 	for (int i = 0; i < m_width; ++i)
	// 	{
	// 		char tmp = row[i*4 + 0];
	// 		row[i*4 + 0] = row[i*4 + 2];
	// 		row[i*4 + 2] = tmp;
	// 	}
	// }

}

Image::Image(int width, int height)
	: m_width(width)
	, m_height(height)
{
	m_image = FreeImage_Allocate(width, height, 32);
}

Image::Image(const char* rawPixels, int width, int height)
	: m_width(width)
	, m_height(height)
{
	// Always assume 32 bit images for now. Some day if I need fancy formats,
	// I'll probably rewrite this entire class with templates/enums ,etc.
	m_image = FreeImage_ConvertFromRawBits((BYTE*)rawPixels, width, height, width * 4, 32, 0, 0, 0);
}

Image::~Image()
{
	FreeImage_Unload(m_image);
}

Image::Image(const Image& original)
	: m_width(original.m_width)
	, m_height(original.m_height)
{
	m_image = FreeImage_Clone(original.m_image);
}

Image& Image::operator=(const Image& rhs)
	
{
	FreeImage_Unload(m_image);
	m_image = FreeImage_Clone(rhs.m_image);
	m_width = rhs.m_width;
	m_height = rhs.m_height;
	return (*this);
}


const void* Image::get() const
{
	return (void*)FreeImage_GetBits(m_image);
}
 
int Image::width() const
{
	return m_width;
}

int Image::height() const
{
	return m_height;
}

bool Image::save(std::string path) const
{
	return FreeImage_Save(FIF_PNG, m_image, path.c_str(), 0);
}
