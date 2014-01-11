


#include <Antenna.hpp>

#include <math/Transform.hpp>
#include <util/Image.hpp>


Antenna::Antenna(const Program& program, std::string name)
	: power(program, name + ".power")
	, m_program(program)
	, m_orientation(program, name + ".orientation")
	, m_layer(antenna_count)
{
	antenna_count++;
}

void Antenna::calculateLoss(AMesh screenQuad)
{
	// Bind the right layer of the loss textures to color attach 0 for rendering
	fbo.load();
	fbo.bindTextureLayer(loss_array, m_layer);

	// Load the right program
	m_program.Load();

	// Send the uniform information
	send();

	// Draw the quad
	screenQuad.draw();

	fbo.unbindTextureLayer();
	fbo.unload();
}

void Antenna::saveImage(std::string path)
{
	fbo.load();
	// Bind the appropriate layer to color attach 0
	fbo.bindTextureLayer(loss_array, m_layer);

	// Read from color attach 0
	glReadBuffer(GL_COLOR_ATTACHMENT0);

	// Need to work on getting the width and height infor here, but for now.
	char* rawPixels = new char[640*480*4];

	// GL_BGRA because FreeImage is stupid.
	glReadPixels(0, 0, 640, 480, GL_BGRA, GL_UNSIGNED_BYTE, rawPixels);

	Image i(rawPixels, 640, 480);
	i.save(path);
	
	fbo.unbindTextureLayer();
	fbo.unload();

	delete[] rawPixels;
}

void Antenna::send()
{
	// Recalculate the matrix and send
	m_orientation = Transform::RotateTranslate(-azimuth, position);
	m_orientation.send();

	// User changes the power value directly, so just send it along.
	power.send();
}


int Antenna::antenna_count = 0;
GLuint Antenna::loss_array = 0;
Framebuffer Antenna::fbo;
