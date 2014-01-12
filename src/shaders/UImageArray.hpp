
#ifndef __GUARD_UIMAGE_ARRAY_HPP__
#define __GUARD_UIMAGE_ARRAY_HPP__


#include <util/Image.hpp>
#include <shaders/IUniform.hpp>
#include <shaders/Program.hpp>

#include <vector>
#include <string>

class UImageArray :  public IUniform
{
	
public:

	UImageArray(const Program& prog, std::string name,
							int width, int height, int layers);

	void setLayer(Image img, int layer);
	const Image& getLayer(Image img, int layer) const;

	GLuint getId() const;

	void loadFromTextureCore();
	void saveAll(std::string path);

	virtual void send() const;

	
	

private:

	GLuint m_textureId;
	GLuint m_texCore;

	int m_width;
	int m_height;
	int m_layers;
	std::vector<Image> m_images;
	
};


#endif
