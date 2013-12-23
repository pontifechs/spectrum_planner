
#include "util/Image.hpp"


Image::Image(std::string path)
{
	// Load a texture
	FREE_IMAGE_FORMAT format = FreeImage_GetFileType(path.c_str(),0);
	FIBITMAP* image = FreeImage_Load(format, path.c_str());
 
	FIBITMAP* temp = image;
	image = FreeImage_ConvertTo32Bits(image);
	FreeImage_Unload(temp);
 
	m_width = FreeImage_GetWidth(image);
	m_height = FreeImage_GetHeight(image);
 
	m_rawData = new GLubyte[4*m_width*m_height];
	char* pixels = (char*)FreeImage_GetBits(image);
	//FreeImage loads in BGR format, so you need to swap some bytes(Or use GL_BGR).
 
	for(int j= 0; j<m_width*m_height; j++){
		m_rawData[j*4+0]= pixels[j*4+2];
		m_rawData[j*4+1]= pixels[j*4+1];
		m_rawData[j*4+2]= pixels[j*4+0];
		m_rawData[j*4+3]= pixels[j*4+3];
	}
	//FreeImage_Unload(pixels);
}

Image::~Image()
{
	delete[] m_rawData;
}

FREE_IMAGE_FORMAT Image::format() const
{
	return m_format;
}

const GLubyte* Image::get() const
{
	return m_rawData;
}
 
int Image::width() const
{
	return m_width;
}

int Image::height() const
{
	return m_height;
}
