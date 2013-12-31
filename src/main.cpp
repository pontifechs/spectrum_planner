// Skeleton taken from GLFW's quick start


#include <GL/glew.h>
#include <GLFW/glfw3.h>


#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#include <shaders/Shader.hpp>
#include <shaders/Program.hpp>
#include <util/Image.hpp>

#include <shaders/UVec2.hpp>
#include <shaders/UFloat.hpp>
#include <shaders/UMat3.hpp>
#include <math/Transform.hpp>

#include <cmath> //trig


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
	Shader vert(std::string(RESOURCE_DIR) + "/shaders/simple.vert", Shader::VERTEX);
	Shader frag(std::string(RESOURCE_DIR) + "/shaders/simple.frag", Shader::FRAGMENT);
	Program simple;
	simple.Build(vert,frag);
	simple.Load();


	//Image single_linear(std::string(RESOURCE_DIR) + "/tex/single-linear.jpg");
	Image single_linear(std::string(RESOURCE_DIR) + "/tex/simplified-directional.jpg");

	Vec2 resolution(640, 480);

	Vec2 center1(0.75, 0.25);
	float azimuth1 = -M_PI / 2.0;
	UMat3 orientation1 = UMat3(simple, "orientation1", 
														 Transform::RotateTranslate(-azimuth1, center1*resolution));

	Vec2 center2(0.25, 0.75);
	float azimuth2 = M_PI;
	UMat3 orientation2 = UMat3(simple, "orientation2", 
														 Transform::RotateTranslate(-azimuth2, center2*resolution));




	//Now send the image to OpenGL in texture core 0
	GLuint texId2;
	glActiveTexture(GL_TEXTURE1);
	glGenTextures(1, &texId2);
	glBindTexture(GL_TEXTURE_1D, texId2);
	glTexImage1D(GL_TEXTURE_1D,0,GL_RGBA, single_linear.width(), 0, 
							 GL_RGBA,GL_UNSIGNED_BYTE,(GLvoid*)single_linear.get() );
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);


	GLenum huboError = glGetError();
	if(huboError){
		std::cout<<"There was an error loading the texture"<<std::endl;
	}


	// std::cout << atan2(0.0, 1.0) << std::endl;
	// std::cout << atan2(1.0, 0.0) << std::endl;
	// std::cout << atan2(0.0, -1.0) << std::endl;
	// std::cout << atan2(-1.0, 0.0) << std::endl;


	// exit(0);



	// Draw loop
	while (!glfwWindowShouldClose(window))
	{
		// int width, height;
		// glfwGetFramebufferSize(window, &width, &height);

		

		int single_location = glGetUniformLocation(simple.GetId(), "oned");
		glUniform1i(single_location, 1);
		glBindTexture(GL_TEXTURE_1D, texId2);

		
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
	}



	// Clean up glfw
	glfwDestroyWindow(window);
	glfwTerminate();

	// clean up freeimage
	FreeImage_DeInitialise();

	exit(EXIT_SUCCESS);
}


