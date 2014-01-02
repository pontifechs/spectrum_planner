
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
	window = glfwCreateWindow(640, 480, "Antenna GLSL", NULL, NULL);
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
		exit(1); // or handle the error in a nicer way
	}
}

int main(void)
{
	// Get the resource directory from cmake.
	#ifndef RESOURCE_DIR
	#error No Resource directory specified from cmake. Check the top-level CMakeLists.txt
	#endif

	// Initialize GLFW, GLEW
  GLFWwindow* window = initGLFW();
	initGLEW();
	FreeImage_Initialise();

	// Grab the shaders
	Shader vert(std::string(RESOURCE_DIR) + "/shaders/passthrough.vert", Shader::VERTEX);
	Shader frag(std::string(RESOURCE_DIR) + "/shaders/powerfield.frag", Shader::FRAGMENT);
	Program simple;
	simple.Build(vert,frag);
	simple.Load();


	//Image single_linear(std::string(RESOURCE_DIR) + "/tex/single-linear-float.png");
	Image single_linear(std::string(RESOURCE_DIR) + "/tex/simplified-directional-float.png");
	
	Image alpha_map(std::string(RESOURCE_DIR) + "/tex/global-alpha.jpg");

	UVec2 resolution(simple, "resolution",640, 480);


	Vec2 center1(0.75, 0.25);
	float azimuth1 = -M_PI;
	UMat3 orientation1 = UMat3(simple, "orientation1", 
														 Transform::RotateTranslate(-azimuth1, center1*resolution));

	Vec2 center2(0.25, 0.75);
	float azimuth2 = 0.0;
	UMat3 orientation2 = UMat3(simple, "orientation2", 
														 Transform::RotateTranslate(-azimuth2, center2*resolution));




	//Now send the image to OpenGL in texture core 0
	GLuint texId1;
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &texId1);
	glBindTexture(GL_TEXTURE_2D, texId1);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA, alpha_map.width(), alpha_map.height(), 0, 
							 GL_RGBA,GL_UNSIGNED_BYTE,(GLvoid*)alpha_map.get() );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);


	//Now send the image to OpenGL in texture core 1
	GLuint texId2;
	glActiveTexture(GL_TEXTURE1);
	glGenTextures(1, &texId2);
	glBindTexture(GL_TEXTURE_1D, texId2);
	glTexImage1D(GL_TEXTURE_1D,0,GL_R32F, single_linear.width(), 0, 
							 GL_RED,GL_FLOAT,(GLvoid*)single_linear.get() );
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);


	GLenum huboError = glGetError();
	if(huboError){
		std::cout<<"There was an error loading the texture"<<std::endl;
	}


	time_t start_time = time(NULL);
	unsigned int iterations = 0;
	
	// Draw loop
	while (!glfwWindowShouldClose(window))
	{
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);

		int nothing_location = glGetUniformLocation(simple.GetId(), "global_alpha");
		glUniform1i(nothing_location, 0);
		glBindTexture(GL_TEXTURE_2D, texId1);


		int single_location = glGetUniformLocation(simple.GetId(), "oned");
		glUniform1i(single_location, 1);
		glBindTexture(GL_TEXTURE_1D, texId2);

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


