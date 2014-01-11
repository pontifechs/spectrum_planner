
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <sstream>

#include <shaders/Shader.hpp>
#include <shaders/Program.hpp>
#include <util/Image.hpp>

#include <shaders/UVec2.hpp>
#include <shaders/UFloat.hpp>
#include <shaders/UMat3.hpp>
#include <shaders/UImage.hpp>
#include <shaders/AMesh.hpp>
#include <shaders/Framebuffer.hpp>

#include <math/Transform.hpp>

#include <Antenna.hpp>

#include <cmath> //trig
#include <ctime> // crude FPS counting

static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

GLFWwindow* initGLFW()
{
	GLFWwindow* window;
	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
		exit(EXIT_FAILURE);

	// Ask GLFW for a decent version of OpenGL :D
	// 2.1 was the default, and it didn't have sampler2DArray,
	// which from what I can tell is the only way to access TEXTURE_2D_ARRAY in GLSL.
	// I'm not sure what the minimum version that I do need is though. But 4.1 is safe
	// to assume for our laptops. Maybe not for more broad release, so something to
	// look into before we do that.
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	window = glfwCreateWindow(640, 480, "Spectrum Planner", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);

	// Turn off vsync for fps calculations
	glfwSwapInterval(0);

	return window;
}

void initGLEW()
{
	// Set up GLEW

	// Apparently this is necessary black magic. Don't ask me.
	glewExperimental = GL_TRUE;

	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		printf("Error during glew initialization\n");
		exit(1);
	}
}

AMesh setupQuad(const Program& prog)
{
	AMesh ret;

	float* verts = new float[18];
	verts[0] = -1.0; verts[1] = -1.0; verts[2] = 0.0; // bl
	verts[3] = -1.0; verts[4] =  1.0; verts[5] = 0.0; // ul
	verts[6] =  1.0; verts[7] =  1.0; verts[8] = 0.0; // ur

	verts[9] = -1.0; verts[10] = -1.0; verts[11] = 0.0; // bl
	verts[12] =  1.0; verts[13] = -1.0; verts[14] = 0.0; // br
	verts[15] =  1.0; verts[16] =  1.0; verts[17] = 0.0; // ur

	ret.attachAttribute(prog, "a_vertex", verts);

	delete[] verts;
	
	return ret;
}

int main(void)
{
	// Get the resource directory from cmake.
	#ifndef RESOURCE_DIR
	#error No Resource directory specified from cmake. Check the top-level CMakeLists.txt
	#endif
	std::string res(RESOURCE_DIR);
 
	// Initialize GLFW, GLEW, FreeImage
  GLFWwindow* window = initGLFW();
	initGLEW();
	FreeImage_Initialise();

	// Throw away any errors created by GLFW or GLEW.
	glGetError();

	// Grab the shaders
	Shader passthroughVert(res + "/shaders/passthrough.vert", Shader::VERTEX);
	Shader powerfieldFrag(res + "/shaders/powerfield.frag", Shader::FRAGMENT);
	Shader summedNoiseFrag(res + "/shaders/summednoise.frag", Shader::FRAGMENT);

	Program powerfield;
	powerfield.Build(passthroughVert,powerfieldFrag);
	powerfield.Load();
	
	Program summedNoise;
	summedNoise.Build(passthroughVert, summedNoiseFrag);
	summedNoise.Load();

	// Setup FBO
	Antenna::fbo.make();

	GLuint core = UImage::total_loaded;
	UImage::total_loaded++;
	glActiveTexture(GL_TEXTURE0 + core);
	glGenTextures(1, &Antenna::loss_array);
	glBindTexture(GL_TEXTURE_2D_ARRAY, Antenna::loss_array);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	GLint antennasLoc = glGetUniformLocation(summedNoise.GetId(), "antennas");
	glUniform1i(antennasLoc, core);

  // Set up a 50 layer 2D array 
	glTexImage3D(GL_TEXTURE_2D_ARRAY, 0,GL_RGB, 640, 480, 50, 0,GL_RGBA, GL_UNSIGNED_BYTE, 0);

	AMesh pfQuad = setupQuad(powerfield);
	AMesh snQuad = setupQuad(summedNoise);
	
	// Run the powerfield program
	powerfield.Load();

	// Powerfield Uniforms
	UImage single_linear(powerfield, "gain_map", res + "/tex/simplified-directional-float.png");
	UImage alpha_map(powerfield, "global_alpha", res + "/tex/global-alpha.jpg");

	UVec2 resolution(powerfield, "resolution",640, 480);

	Antenna antenna1(powerfield, "antenna");
	antenna1.position = Vec2(0.75, 0.25) * resolution;
	antenna1.azimuth = -M_PI;
	antenna1.power = 12.0;

	Antenna antenna2(powerfield, "antenna");
	antenna2.position = Vec2(0.25, 0.75) * resolution;
	antenna2.azimuth = 0.0;
	antenna2.power = 12.0;

	Antenna antenna3(powerfield, "antenna");
	antenna3.position = Vec2(0.75, 0.75) * resolution;
	antenna3.azimuth = -3 * M_PI / 4;
	antenna3.power = 12.0;

	single_linear.send();
	alpha_map.send();

	resolution.send();

	antenna1.calculateLoss(pfQuad);
	antenna2.calculateLoss(pfQuad);
	antenna3.calculateLoss(pfQuad);

	antenna1.saveImage("screen1.png");			
	antenna2.saveImage("screen2.png");
	antenna3.saveImage("screen3.png");

	// Crude timing for rough FPS estimate
	time_t start_time = time(NULL);
	unsigned int iterations = 0;

	// Draw loop
	while (!glfwWindowShouldClose(window))
	{
		// Clear all buffers
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		int width, height;
		glfwGetFramebufferSize(window, &width, &height);

        
		summedNoise.Load();

		snQuad.draw();

		glfwSwapBuffers(window);
		glfwPollEvents();

		iterations++;

		time_t now = time(NULL);
		if (now - start_time >= 5)
		{
			std::stringstream ss;
			ss << "Rough FPS: " << (iterations / 5.0);
			glfwSetWindowTitle(window, ss.str().c_str());

			iterations = 0;
			start_time = now;
		}
	}

	// Clean up glfw
	glfwDestroyWindow(window);
	glfwTerminate();

	// clean up freeimage
	FreeImage_DeInitialise();

	exit(EXIT_SUCCESS);
}


