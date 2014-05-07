


#include <Antenna.hpp>

#include <math/Transform.hpp>
#include <util/Image.hpp>


Antenna::Antenna()
{
}

Antenna::Antenna(const Program& program, std::string name, 
								 Framebuffer& fbo, UImageArray& loss_array, UImageArray& gain_patterns)
	: power(program, name + ".power")
	, gainPattern(program, name + ".gainPattern")
	, m_program(program)
	, m_orientation(program, name + ".orientation")
	, m_layer(antenna_count)
	, m_fbo(fbo)
	, m_loss_array(loss_array)
	, m_gain_patterns(gain_patterns)
		
{
	antenna_count++;
}

void Antenna::calculateLoss(AMesh screenQuad)
{
	// Bind the right layer of the loss textures to color attach 0 for rendering
	m_fbo.load();

	m_fbo.bindTextureLayer(m_loss_array.getId(), m_layer);

	// Load the right program
	m_program.Load();

	// Send the uniform information
	send();

	// Draw the quad
	screenQuad.draw();

	m_fbo.unbindTextureLayer();
	m_fbo.unload();
}

void Antenna::saveImage(std::string path)
{
	m_fbo.load();
	// Bind the appropriate layer to color attach 0
	m_fbo.bindTextureLayer(m_loss_array.getId(), m_layer);

	// Read from color attach 0
	glReadBuffer(GL_COLOR_ATTACHMENT0);

	// Need to work on getting the width and height infor here, but for now.
	char* rawPixels = new char[640*480*4];

	// GL_BGRA because FreeImage is stupid.
	glReadPixels(0, 0, 640, 480, GL_BGRA, GL_UNSIGNED_BYTE, rawPixels);

	Image i(rawPixels, 640, 480);
	i.save(path);
	
	m_fbo.unbindTextureLayer();
	m_fbo.unload();

	delete[] rawPixels;
}

void Antenna::send()
{
	// Recalculate the matrix and send
	m_orientation = Transform::RotateTranslate(-azimuth, position);
	m_orientation.send();

	// User changes the power value directly, so just send it along.
	power.send();
	gainPattern.send();

	m_gain_patterns.send();
}


int Antenna::antenna_count = 0;

