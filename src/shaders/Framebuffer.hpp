
#ifndef __GUARD_FRAMEBUFFER_HPP__
#define __GUARD_FRAMEBUFFER_HPP__

#include <GL/glew.h>

class Framebuffer
{
public:	
	
	Framebuffer();

	void make();

	void bindTextureLayer(GLuint texture, int layer);
	void unbindTextureLayer();
	
	void load();
	void unload();

	bool check() const;


private:
	
	GLuint m_fbo;
	bool m_loaded;

};



#endif
