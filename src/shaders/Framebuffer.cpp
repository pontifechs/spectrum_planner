
#include <shaders/Framebuffer.hpp>
#include <stdio.h>

Framebuffer::Framebuffer()
{
	glGenFramebuffers(1, &m_fbo);
	load();
	GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
	glDrawBuffers(1, DrawBuffers);
}

void Framebuffer::bindTextureLayer(GLuint texture, int layer)
{
	bool wasLoaded = m_loaded;
	load();
	glFramebufferTextureLayer(GL_FRAMEBUFFER,
														GL_COLOR_ATTACHMENT0,
														texture,
														0,
														layer);
	if (!wasLoaded)
	{
		unload();
	}
}

void Framebuffer::unbindTextureLayer()
{
	bool wasLoaded = m_loaded;
	load();
	glFramebufferTextureLayer(GL_FRAMEBUFFER,
														GL_COLOR_ATTACHMENT0,
														0,
														0,
														0);
	if (!wasLoaded)
	{
		unload();
	}
}

void Framebuffer::load()
{
	if (!m_loaded)
	{
		m_loaded = true;
		glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
	}
}

void Framebuffer::unload()
{
	if (m_loaded)
	{
		m_loaded = false;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}

bool Framebuffer::check() const
{
	if (!m_loaded)
	{
		return false;
	}

	bool ret = false;

	// check if everything is OK
	GLenum e = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	switch (e) {	
		case GL_FRAMEBUFFER_UNDEFINED:
			printf("FBO Undefined\n");
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT :
			printf("FBO Incomplete Attachment\n");
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT :
			printf("FBO Missing Attachment\n");
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER :
			printf("FBO Incomplete Draw Buffer\n");
			break;
		case GL_FRAMEBUFFER_UNSUPPORTED :
			printf("FBO Unsupported\n");
			break;
		case GL_FRAMEBUFFER_COMPLETE:
			ret = true;
			break;
		default:
			printf("FBO Problem?\n");
	}

	return ret;
}
