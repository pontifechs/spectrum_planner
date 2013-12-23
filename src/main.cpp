// Skeleton taken from GLFW's quick start


#include <GL/glew.h>
#include <GLFW/glfw3.h>


#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#include <shaders/Shader.hpp>
#include <shaders/Program.hpp>
#include <util/Image.hpp>


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
	// Initialize GLFW, GLEW
  GLFWwindow* window = initGLFW();
	initGLEW();
	FreeImage_Initialise();

	// Grab the shaders
	Shader vert("../shaders/simple.vert", Shader::VERTEX);
	Shader frag("../shaders/simple.frag", Shader::FRAGMENT);
	Program simple;
	simple.Build(vert,frag);
	simple.Load();

	Image nothing("../nothing-to-do-here.jpeg");
	

	//Now send the image to OpenGL in texture core 0
	GLuint texId;
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_2D, texId);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA, nothing.width(), nothing.height(), 0, 
							 GL_RGBA,GL_UNSIGNED_BYTE,(GLvoid*)nothing.get() );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

 
	GLenum huboError = glGetError();
	if(huboError){
		std::cout<<"There was an error loading the texture"<<std::endl;
	}
 



	// Draw loop
	while (!glfwWindowShouldClose(window))
	{
		// float ratio;
		// int width, height;
		// glfwGetFramebufferSize(window, &width, &height);
		// ratio = width / (float) height;

    glActiveTexture(GL_TEXTURE0);
    int texture_location = glGetUniformLocation(simple.GetId(), "tex");
    glUniform1i(texture_location, 0);
    glBindTexture(GL_TEXTURE_2D, texId);

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


