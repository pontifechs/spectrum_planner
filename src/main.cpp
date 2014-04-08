
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
#include <shaders/UImageArray.hpp>
#include <shaders/AMesh.hpp>
#include <shaders/Framebuffer.hpp>

#include <math/Transform.hpp>

#include <Antenna.hpp>

#include <cmath> //trig
#include <ctime> // crude FPS counting
#include <unistd.h> // for the sleep function

static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}

bool stopTest = FALSE;
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
    if (key==GLFW_KEY_LEFT){
        stopTest= TRUE;}
    if (key==GLFW_KEY_RIGHT) {
        stopTest=FALSE;
    }
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
 
    std::cout << res << std::endl;
    
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
    Shader offsetAnglesFrag(res + "/shaders/offsetAngles.frag", Shader::FRAGMENT); // New 01

	Program powerfield;
	powerfield.Build(passthroughVert,powerfieldFrag);
	powerfield.Load();
	
	Program summedNoise;
	summedNoise.Build(passthroughVert, summedNoiseFrag);
	summedNoise.Load();
    
    // Added to create Receiver Angles
    Program offsetAngles;                                   // test 01 Good
    offsetAngles.Build(passthroughVert, offsetAnglesFrag);  // test 01 Good
    offsetAngles.Load();                                    // test 01 Good
    
	// Set up FBO and Texture arrays
    Framebuffer fbo;
    UImageArray loss_array = UImageArray(summedNoise, "antennas", 640, 480, 8);
    UImageArray angleArray = UImageArray(summedNoise, "angles",   640, 480, 8);  // test 17

    
	AMesh pfQuad = setupQuad(powerfield);
	AMesh snQuad = setupQuad(summedNoise);
    AMesh oaQuad = setupQuad(offsetAngles);         // test 01 Good
    
	// Run the powerfield program
	powerfield.Load();

	// Powerfield Uniforms
//	Image single_linear(res + "/tex/simplified-directional-float.png");
//	Image sixty_degree(res + "/tex/60-degree.png");
    Image omni(res + "/tex/antenna_omni.png");
    Image dir60_S(res + "/tex/antenna_60_simple.png");
    Image dir30_S(res + "/tex/antenna_30_simple.png");
    Image dir05_S(res + "/tex/antenna_05_simple.png");
 
    // Create the gain patterns for the powerField fragment shader
	UImageArray gain_patterns(powerfield, "gain_patterns", 512, 1, 4);
//	gain_patterns.setLayer(single_linear, 0);
//	gain_patterns.setLayer(sixty_degree, 1);
    gain_patterns.setLayer(omni, 0);        // test 25
    gain_patterns.setLayer(dir60_S, 1);     // test 25
    gain_patterns.setLayer(dir30_S, 2);     // test 25
    gain_patterns.setLayer(dir05_S, 3);     // test 25
    
    // Create and send the gain atterns for the summedNoise fragment shader
    summedNoise.Load();
    UImageArray gain_Rcvr(summedNoise, "s_gain_patterns", 512, 1, 4); // test 17
//	gain_Rcvr.setLayer(single_linear, 0);                           // test 17
//	gain_Rcvr.setLayer(sixty_degree, 1);                            // test 17
    gain_patterns.setLayer(omni, 0);        // test 25
    gain_patterns.setLayer(dir60_S, 1);     // test 25
    gain_patterns.setLayer(dir30_S, 2);     // test 25
    gain_patterns.setLayer(dir05_S, 3);     // test 25
    gain_Rcvr.send();                                               // test 17

	UImage alpha_map(powerfield, "global_alpha", res + "/tex/global-alpha.jpg");

	UVec2 resolution(powerfield, "resolution",640, 480);
    
    UVec2 mAntPosition(offsetAngles, "AntPosition", 0.5,0.5);           // test 07
    
    float pointAngle=0.0, rcvrGain = 6;                                 // test 20
    UVec2 rcvrPointing(summedNoise, "rcvr", pointAngle, rcvrGain);    // test 22

    
    std::vector<Vec2> positions(8);               // test 14  Good

    Antenna antenna0(powerfield, "antenna", fbo, loss_array, gain_patterns);
	antenna0.position = Vec2(0.49375, 0.3583) * resolution;
	antenna0.azimuth = M_PI/6.0;
	antenna0.power = 8.0;
	antenna0.gainPattern = 0.0;
    positions[0] = antenna0.position;              // test 14  Good
	
    Antenna antenna1(powerfield, "antenna", fbo, loss_array, gain_patterns);
	antenna1.position = Vec2(0.821875, 0.65) * resolution;
	antenna1.azimuth = M_PI/6.0;
	antenna1.power = 7.0;
	antenna1.gainPattern = 1.0;
    positions[1] = antenna1.position;              // test 14 Good

	Antenna antenna2(powerfield, "antenna", fbo, loss_array, gain_patterns);
	antenna2.position = Vec2(0.7125, 0.79583) * resolution;
	antenna2.azimuth = M_PI/6.0;
	antenna2.power = 8.0;
	antenna2.gainPattern = 2.0;
    positions[2] = antenna2.position;              // test 14 Good

	Antenna antenna3(powerfield, "antenna", fbo, loss_array, gain_patterns);
	antenna3.position = Vec2(0.603125, 0.65) * resolution;
	antenna3.azimuth = M_PI/6.0;
	antenna3.power = 8.0;
	antenna3.gainPattern = 3.0;
    positions[3] = antenna3.position;              // test 14 Good
	
	Antenna antenna4(powerfield, "antenna", fbo, loss_array, gain_patterns);
	antenna4.position = Vec2(0.7125, 0.50417) * resolution;
	antenna4.azimuth = M_PI/6.0;
	antenna4.power = 8.0;
	antenna4.gainPattern = 0.0;
    positions[4] = antenna4.position;              // test 14 Good
    
    Antenna antenna5(powerfield, "antenna", fbo, loss_array, gain_patterns);
	antenna5.position = Vec2(0.7125, 0.65) * resolution;
	antenna5.azimuth = M_PI/4;
	antenna5.power = 5.0;
	antenna5.gainPattern = 1.0;
    positions[5] = antenna5.position;              // test 14 Good
    
	Antenna antenna6(powerfield, "antenna", fbo, loss_array, gain_patterns);
	antenna6.position = Vec2(0.603125, 0.504167) * resolution;
	antenna6.azimuth = M_PI/4;
	antenna6.power = 5.0;
	antenna6.gainPattern = 2.0;
    positions[6] = antenna6.position;              // test 14 Good
    
	Antenna antenna7(powerfield, "antenna", fbo, loss_array, gain_patterns);
	antenna7.position = Vec2(0.49375, 0.3583) * resolution;
	antenna7.azimuth = M_PI/4;
	antenna7.power = 5.0;
	antenna7.gainPattern = 3.0;
    positions[7] = antenna7.position;              // test 14 Good
    
    alpha_map.send();
    
	resolution.send();
    rcvrPointing.send();                            // test 22
    
    antenna0.calculateLoss(pfQuad);     // test 14 Good
	antenna1.calculateLoss(pfQuad);     // test 14 Good
	antenna2.calculateLoss(pfQuad);     // test 14 Good
	antenna3.calculateLoss(pfQuad);     // test 14 Good
	antenna4.calculateLoss(pfQuad);     // test 14 Good
    antenna5.calculateLoss(pfQuad);     // test 14 Good
	antenna6.calculateLoss(pfQuad);     // test 14 Good
	antenna7.calculateLoss(pfQuad);     // test 14 Good
    
    GLuint anglesTextureID= angleArray.getId();        // test 18
    glActiveTexture(GL_TEXTURE0 + angleArray.getTexCoreID());  // test 26
//    std::cout << "AngleArrayID " << anglesTextureID << std::endl; // test 18
    fbo.load();                                     // test 17
    offsetAngles.Load();                            // test 15
    Vec2 myPos;                                     // Test 15
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    for (int mIndex = 0; mIndex<8; ++mIndex) {      // test 15
        fbo.bindTextureLayer(anglesTextureID, mIndex);     // test 17
        myPos = positions[mIndex];                  // test 15
        mAntPosition.x = myPos.x ;                  // test 14
        mAntPosition.y = myPos.y ;                  // test 15
//        std::cout << mAntPosition.x << " " << mAntPosition.y << std::endl;
        mAntPosition.send();                        // test 15
        offsetAngles.Load();                        // test 15, 16 (Don't need to re-load)
        oaQuad.draw();                              // test 15
        fbo.unbindTextureLayer();                   // test 17
        glfwSwapBuffers(window);                    // test 15
		glfwPollEvents();                           // test 15
    }
    fbo.unload();
    glActiveTexture(GL_TEXTURE0 + loss_array.getTexCoreID());
    
    antenna0.saveImage("Ant00.png");
	antenna1.saveImage("Ant01.png");
//	antenna2.saveImage("Ant02.png");
//	antenna3.saveImage("Ant03.png");
//	antenna4.saveImage("Ant04.png");
//  antenna5.saveImage("Ant05.png");
//	antenna6.saveImage("Ant06.png");
//	antenna7.saveImage("Ant07.png");


	int global_time = 0.0;

	// Crude timing for rough FPS estimate
	time_t start_time = time(NULL);
	unsigned int iterations = 0;
    
	// Draw loop
	while (!glfwWindowShouldClose(window))
	{
        //
        if (stopTest) {
            do {
                sleep(1);
                glfwPollEvents();
            } while (stopTest);
        }
        
		// Clear screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// // For giggles, have antenna 3 spin around
        antenna1.azimuth = global_time * 0.005;
		antenna1.calculateLoss(pfQuad);
//        antenna2.azimuth = global_time * 0.005;
//		antenna2.calculateLoss(pfQuad);
//		antenna3.azimuth = global_time * 0.005;
//		antenna3.calculateLoss(pfQuad);
//        antenna4.azimuth = global_time * 0.005;
//		antenna4.calculateLoss(pfQuad);
//        antenna5.azimuth = global_time * 0.005;
//		antenna5.calculateLoss(pfQuad);
//		antenna6.azimuth = global_time * 0.005;
//		antenna6.calculateLoss(pfQuad);
//        antenna7.azimuth = global_time * 0.005+M_PI/4;
//		antenna7.calculateLoss(pfQuad);
        
//        double fractPart, intPart;                                              // test 22
//        fractPart = modf(global_time*0.005, &intPart);                        // test 22
//        rcvrPointing.x =fractPart;                                              // test 22
//        std::cout << rcvrPointing.x << " " << rcvrPointing.y << std::endl;      // test 20
//        rcvrPointing.send();                                                    // test 22
        
		summedNoise.Load();

		snQuad.draw();
        

		glfwSwapBuffers(window);
		glfwPollEvents();

		iterations++;
		global_time++;
//        sleep(1);           // test 21

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


