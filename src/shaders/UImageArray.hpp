
#ifndef __GUARD_UIMAGE_ARRAY_HPP__
#define __GUARD_UIMAGE_ARRAY_HPP__


#include <util/Image.hpp>
#include <shaders/IUniform.hpp>
#include <shaders/Program.hpp>

#include <vector>
#include <string>
#include "math/Color.hpp"

class UImageArray :  public IUniform
{
	
public:

	UImageArray();

	UImageArray(const Program& prog, std::string name,
							int width, int height, int layers);

	void Load(const Program& prog, std::string name,
						int width, int height, int layers);

	void setLayer(Image img, int layer);
	const Image& getLayer(Image img, int layer) const;

	GLuint getId() const;
    
	RGBQUAD getPixelColorValues(unsigned int mWidth, unsigned int mHeight, int mLayer);
	float  getPixelFloatValues(unsigned int mWidth, unsigned int mHeight, int mLayer);

	void loadFromTextureCore_BGRA();
	void loadFromTextureCore_32F();
	void saveAll(std::string path);
	GLuint getTexCoreID();
    
	virtual void send() const;

	
	

private:

	GLuint m_textureId;
	GLuint m_texCore;
	GLuint m_prog;

	int m_width;
	int m_height;
	int m_layers;

	std::vector<Image> m_images;
	
};


#endif
