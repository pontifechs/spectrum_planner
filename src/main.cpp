
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

#include <math/Transform.hpp>

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
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		printf("Error during glew initialization\n");
		exit(1);
	}
}

int main(void)
{
	// Get the resource directory from cmake.
	#ifndef RESOURCE_DIR
	#error No Resource directory specified from cmake. Check the top-level CMakeLists.txt
	#endif
	std::string res(RESOURCE_DIR);
 

	// Initialize GLFW, GLEW
  GLFWwindow* window = initGLFW();
	initGLEW();
	FreeImage_Initialise();

	// Grab the shaders
	Shader passthroughVert(res + "/shaders/passthrough.vert", Shader::VERTEX);
	Shader powerfieldFrag(res + "/shaders/powerfield.frag", Shader::FRAGMENT);
	Shader logoFrag(res + "/shaders/logo.frag", Shader::FRAGMENT);

	Program logoProg;
	logoProg.Build(passthroughVert, logoFrag);
	
	Program powerfield;
	powerfield.Build(passthroughVert,powerfieldFrag);
	powerfield.Load();



	// Powerfield Uniforms
	UImage single_linear(powerfield, "gain_map", res + "/tex/simplified-directional-float.png");
	UImage alpha_map(powerfield, "global_alpha", res + "/tex/global-alpha.jpg");


	UVec2 resolution(powerfield, "resolution",640, 480);


	Vec2 center1(0.75, 0.25);
	float azimuth1 = -M_PI;
	UMat3 orientation1 = UMat3(powerfield, "orientation1", 
														 Transform::RotateTranslate(-azimuth1, center1*resolution));

	Vec2 center2(0.25, 0.75);
	float azimuth2 = 0.0;
	UMat3 orientation2 = UMat3(powerfield, "orientation2", 
														 Transform::RotateTranslate(-azimuth2, center2*resolution));

	
	// Logo uniforms
	UImage logo(logoProg, "logo", res + "/tex/combined-logo.png");
	UVec2 logo_resolution(logoProg, "resolution", logo.width(), logo.height());



	time_t start_time = time(NULL);
	unsigned int iterations = 0;
	
	// Draw loop
	while (!glfwWindowShouldClose(window))
	{
		// Clear all buffers
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		int width, height;
		glfwGetFramebufferSize(window, &width, &height);

		// Run the powerfield program

		powerfield.Load();

		single_linear.send();
		alpha_map.send();

		resolution.send();
		
		orientation1.send();
		orientation2.send();

		// Draw a single quad which everything else will be drawn on.
		glBegin(GL_QUADS);
		glVertex3f(-1.0f, -1.0f, 1.0f);
		glVertex3f( 1.0f, -1.0f, 1.0f);
		glVertex3f( 1.0f,  1.0f, 1.0f);
		glVertex3f(-1.0f,  1.0f, 1.0f);
		glEnd();
		

		// Run the logo overlay program

		logoProg.Load();
		logo.send();
		logo_resolution.send();
		
		glBegin(GL_QUADS);
		glVertex3f(-1.0f, -1.0f, 1.0f);
		glVertex3f( 1.0f, -1.0f, 1.0f);
		glVertex3f( 1.0f,  1.0f, 1.0f);
		glVertex3f(-1.0f,  1.0f, 1.0f);
		glEnd();


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


