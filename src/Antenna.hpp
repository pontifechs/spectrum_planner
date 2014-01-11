
#ifndef __GUARD_ANTENNA_HPP__
#define __GUARD_ANTENNA_HPP__

#include <math/Vec2.hpp>

#include <shaders/Program.hpp>
#include <shaders/UFloat.hpp>
#include <shaders/UMat3.hpp>
#include <shaders/AMesh.hpp>
#include <shaders/Framebuffer.hpp>

#include <GL/glew.h>

class Antenna 
{
public:

	static int antenna_count;
	static GLuint loss_array; 
	static Framebuffer fbo;


	Vec2 position;
	float azimuth;
	UFloat power;	

	Antenna(const Program& program, std::string name);

	// Recalculates the loss field
	void calculateLoss(AMesh screenQuad);
	
	void saveImage(std::string path);



private:
	void send();
	
	const Program& m_program;

	UMat3 m_orientation;
	unsigned int m_layer;

};

#endif
