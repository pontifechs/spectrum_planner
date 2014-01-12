
#include <algorithm>
#include <sstream>

#include <shaders/UImage.hpp>
#include <shaders/UImageArray.hpp>


UImageArray::UImageArray(const Program& prog, std::string name,
												 int width, int height, int layers)
	: IUniform(prog, name)
	, m_width(width)
	, m_height(height)
	, m_layers(layers)
{
	// Increment and get the current next available tex core (still hacky)
	m_texCore = UImage::total_loaded++;
	glActiveTexture(GL_TEXTURE0 + m_texCore);
	glGenTextures(1, &m_textureId);

	char* blank = new char[m_width * m_height * m_layers * 4];
	std::fill_n(blank, m_width * m_height * m_layers * 4, 0);
	

	if (m_height != 1)
	{

		glBindTexture(GL_TEXTURE_2D_ARRAY, m_textureId);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA, m_width, m_height, m_layers, 0,
								 GL_RGBA, GL_UNSIGNED_BYTE, blank);
	}
	else
	{
		glBindTexture(GL_TEXTURE_1D_ARRAY, m_textureId);
		glTexParameteri(GL_TEXTURE_1D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_1D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_1D_ARRAY, 0, GL_R32F, m_width, m_layers, 0,
								 GL_RED, GL_FLOAT, blank);
	}

	for (int i = 0; i < m_layers; ++i)
	{
		m_images.push_back(Image(m_width, m_height));
	}
	delete[] blank;
}

void UImageArray::setLayer(Image img, int layer)
{
	m_images[layer] = img;
	if (m_height != 1)
	{
		glBindTexture(GL_TEXTURE_2D_ARRAY, m_textureId);
		glTexSubImage3D(GL_TEXTURE_2D_ARRAY,
										0,
										0,
										0,
										layer,
										m_width,
										m_height,
										1,
										GL_BGRA,
										GL_UNSIGNED_BYTE,
										(GLvoid*)m_images[layer].get());	
	}
	else
	{
		glBindTexture(GL_TEXTURE_1D_ARRAY, m_textureId);
		glTexSubImage2D(GL_TEXTURE_1D_ARRAY,
										0,
										0,
										layer,
										m_width,
										1,
										GL_RED,
										GL_FLOAT,
										(GLvoid*)m_images[layer].get());

		GLenum huboError = glGetError();
		if(huboError){
			std::cout << gluErrorString(huboError) << std::endl;
			//std::cout<<"There was an error loading the texture for "<< path << std::endl;
		}


	}
}

const Image& UImageArray::getLayer(Image img, int layer) const
{
	return m_images.at(layer);
}

GLuint UImageArray::getId() const
{
	return m_textureId;
}

void UImageArray::loadFromTextureCore()
{
	glBindTexture(GL_TEXTURE_2D_ARRAY, m_textureId);
	
	// 4 is number of single byte channels (4 for RGBA
	char* rawPixels = new char[m_width * m_height * m_layers * 4];
	glGetTexImage(GL_TEXTURE_2D_ARRAY, 0, GL_BGRA, GL_UNSIGNED_BYTE, rawPixels);	

	for (int i = 0; i < m_layers; ++i)
	{
		m_images[i] = Image(rawPixels + (m_width * m_height * 4 * i), m_width, m_height);
	}
	
	delete[] rawPixels;
}

void UImageArray::saveAll(std::string path)
{
	for (int i = 0; i < m_layers; ++i)
	{
		std::stringstream ss;
		ss << path << i << ".png";
		m_images[i].save(ss.str());
	}

}

void UImageArray::send() const
{
	glUniform1i(m_id, m_texCore);
}
