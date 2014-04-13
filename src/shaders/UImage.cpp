
#include <shaders/UImage.hpp>

int UImage::total_loaded = 0;

UImage::UImage(const Program& program, 
							 const std::string name, 
							 const std::string path,
							 bool floatingPoint)
	: IUniform(program, name), Image(path)
{
	m_tex_core = total_loaded;
	glActiveTexture(GL_TEXTURE0 + m_tex_core);
	glGenTextures(1, &m_tex_id);
    uniformName = name;
		
	// Super hacky for the demo, but all 2D textures are GL_RGBA like normal
	// and all 1D textures are GL_R32F like the gain maps

		GLint internalFormat;
		GLenum format;
		GLenum type;

		if (floatingPoint)
		{
			internalFormat = GL_R32F;
			format = GL_RED;
			type = GL_FLOAT;
		}
		else
		{
			internalFormat = GL_RGBA;
			format = GL_BGRA;
			type = GL_UNSIGNED_BYTE;
		}

	if (height() != 1)
	{
		glBindTexture(GL_TEXTURE_2D, m_tex_id); 
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width(), height(), 0,
								 format, type, (GLvoid*)get());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}
	else 
	{
		glBindTexture(GL_TEXTURE_1D, m_tex_id); 
		glTexImage1D(GL_TEXTURE_1D, 0, internalFormat, width(), 0,
								 format, type, (GLvoid*)get());
		glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}



	GLenum huboError = glGetError();
	if(huboError){
		//std::cout << gluErrorString(huboError) << std::endl;
		std::cout<<"There was an error loading the texture for "<< path << std::endl;
	}
	
	total_loaded++;
}

void UImage::send() const
{
    std::cout << "UImage_Send " << uniformName << " Prog " << m_id << " Tex_Core " << m_tex_core << std::endl;
	glUniform1i(m_id, m_tex_core);
}
