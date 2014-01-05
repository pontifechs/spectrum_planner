
#include "util/Image.hpp"

#include <iostream>

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

Image::~Image()
{
	FreeImage_Unload(m_image);
}

FREE_IMAGE_FORMAT Image::format() const
{
	return m_format;
}

const void* Image::get() const
{
	return (void*)FreeImage_GetBits(m_image);
}

void Image::set(int x, int y, unsigned int value)
{
	unsigned int* row = (unsigned int*)FreeImage_GetScanLine(m_image, y);
	row[x] = value;
}
 
int Image::width() const
{
	return m_width;
}

int Image::height() const
{
	return m_height;
}
