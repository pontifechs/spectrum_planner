
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
	Shader offsetAnglesFrag(res + "/shaders/offsetAngles.frag", Shader::FRAGMENT);
	Shader sh_viewAFrag(res + "/shaders/fr_maxA.frag", Shader::FRAGMENT);       // test 01
	Shader sh_viewBFrag(res + "/shaders/fr_maxB.frag", Shader::FRAGMENT);       // test 01
	Shader sh_viewMaxFrag(res + "/shaders/fr_viewMax.frag", Shader::FRAGMENT);  // test 01

	Program powerfield;
	powerfield.Build(passthroughVert,powerfieldFrag);
	powerfield.Load();
	
	Program summedNoise;
	summedNoise.Build(passthroughVert, summedNoiseFrag);
	summedNoise.Load();
    
	// Added to create Receiver Angles
	Program offsetAngles;                                   
	offsetAngles.Build(passthroughVert, offsetAnglesFrag);  
	offsetAngles.Load();
    
	Program pr_viewA;                               // test 01
	pr_viewA.Build(passthroughVert, sh_viewAFrag);  // test 01
	pr_viewA.Load();                                // test 01
    
	Program pr_viewB;                               // test 01
	pr_viewB.Build(passthroughVert, sh_viewBFrag);  // test 01
	pr_viewB.Load();                                // test 01
    
	Program pr_viewMax;                             // test 01
	pr_viewMax.Build(passthroughVert, sh_viewMaxFrag); // test 01
	pr_viewMax.Load();                              // test 01
    
	// Set up FBO and Texture arrays
	Framebuffer fbo;
	fbo.load();
	fbo.unload();

	UImageArray loss_array = UImageArray(summedNoise, "antennas", 640, 480, 8);
	UImageArray angleArray = UImageArray(summedNoise, "angles",   640, 480, 8);
	UImageArray im_viewA = UImageArray(pr_viewA, "viewA", 640, 480, 1);     // test 01
	UImageArray im_viewB = UImageArray(pr_viewB, "viewB", 640, 480, 1);     // test 01
    
	GLuint viewA_id = im_viewA.getId();
	GLuint viewB_id = im_viewB.getId();
    
	loss_array.sendTo(summedNoise);
	angleArray.sendTo(summedNoise);

	im_viewB.sendTo(pr_viewA);      // test 01
	loss_array.sendTo(pr_viewA);    // test 01
	angleArray.sendTo(pr_viewA);    // test 01
    
	im_viewA.sendTo(pr_viewB);      // test 01
	loss_array.sendTo(pr_viewB);    // test 01
	angleArray.sendTo(pr_viewB);    // test 01
    
	im_viewA.sendTo(pr_viewMax);    // test 01
    
	// Meshes are needed to create vertex shader to the fragement shader
	AMesh pfQuad = setupQuad(powerfield);
	AMesh snQuad = setupQuad(summedNoise);
	AMesh oaQuad = setupQuad(offsetAngles);
	AMesh maQuad = setupQuad(pr_viewA);         // test 01
	AMesh mbQuad = setupQuad(pr_viewB);         // test 01
	AMesh mvQuad = setupQuad(pr_viewMax);       // test 01
    
	// Powerfield Uniforms
	Image omni(res + "/tex/antenna_omni.png");
	Image dir60_S(res + "/tex/antenna_60_simple.png");
	Image dir30_S(res + "/tex/antenna_30_simple.png");
	Image dir05_S(res + "/tex/antenna_05_simple.png");
 
	// Create the gain patterns for the powerField fragment shader
	UImageArray gain_patterns(powerfield, "gain_patterns", 512, 1, 4);
	gain_patterns.setLayer(omni, 0);        
	gain_patterns.setLayer(dir60_S, 1);     
	gain_patterns.setLayer(dir30_S, 2);
	gain_patterns.setLayer(dir05_S, 3);
    
	gain_patterns.sendTo(powerfield);
	gain_patterns.sendTo(summedNoise);

	UImage transfer(summedNoise, "transfer", res + "/tex/blue_yellow_dark_red_transfer.png", false);
	transfer.sendTo(summedNoise);
	transfer.sendTo(pr_viewMax);

 
	UImage alpha_map(powerfield, "global_alpha", res + "/tex/global-alpha.jpg", false);
	alpha_map.sendTo(powerfield);


	UVec2 resolution(powerfield, "resolution", 640, 480);
	resolution.sendTo(powerfield);
    
	UVec2 mAntPosition(offsetAngles, "AntPosition", 0.5,0.0);
	mAntPosition.sendTo(offsetAngles);
    
	float pointAngle=0.5, rcvrGainPattern = 1.0;
	UVec2 rcvrPointing(summedNoise, "rcvr", pointAngle, rcvrGainPattern);
	rcvrPointing.sendTo(summedNoise);
    
	gain_patterns.sendTo(pr_viewA);     // test 01
	rcvrPointing.sendTo(pr_viewA);      // test 01
    
	gain_patterns.sendTo(pr_viewB);     // test 01
	rcvrPointing.sendTo(pr_viewB);      // test 01
    
	// establish a few antennas to work with
	std::vector<Vec2> positions(8);               

	Antenna antenna0(powerfield, "antenna", fbo, loss_array, gain_patterns);
	antenna0.position = Vec2(0.25, 0.3583) * resolution;
	antenna0.azimuth = M_PI / 2.0;
	antenna0.power = 3.0;
	antenna0.gainPattern = 0.0;
	positions[0] = antenna0.position;
	
	Antenna antenna1(powerfield, "antenna", fbo, loss_array, gain_patterns);
	antenna1.position = Vec2(0.821875, 0.65) * resolution;
	antenna1.azimuth = -9.0 * M_PI / 16.0;
	antenna1.power = 15.0;
	antenna1.gainPattern = 1.0;
	positions[1] = antenna1.position;              

	Antenna antenna2(powerfield, "antenna", fbo, loss_array, gain_patterns);
	antenna2.position = Vec2(0.7125, 0.79583) * resolution;
	antenna2.azimuth = - M_PI;
	antenna2.power = 15.0;
	antenna2.gainPattern = 1.0;
	positions[2] = antenna2.position;              

	Antenna antenna3(powerfield, "antenna", fbo, loss_array, gain_patterns);
	antenna3.position = Vec2(0.49375, 0.3583) * resolution;
	antenna3.azimuth = - M_PI/2.0;
	antenna3.power = 8.0;
	antenna3.gainPattern = 1.0;
	positions[3] = antenna3.position;              
	
    
	antenna0.calculateLoss(pfQuad);
	antenna1.calculateLoss(pfQuad);     
	antenna2.calculateLoss(pfQuad);     
	antenna3.calculateLoss(pfQuad);     

    
	// Create the offset angles
	GLuint anglesTextureID= angleArray.getId();        
	fbo.load();                                     
	offsetAngles.Load();                            
	Vec2 myPos;                                     
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
	for (int mIndex = 0; mIndex<3; ++mIndex) {
		fbo.bindTextureLayer(anglesTextureID, mIndex);     
		myPos = positions[mIndex];                  
		mAntPosition.x = myPos.x ;                  
		mAntPosition.y = myPos.y ;
		mAntPosition.sendTo(offsetAngles);
		oaQuad.draw();                              
		fbo.unbindTextureLayer();                   
		glfwSwapBuffers(window);                    
		glfwPollEvents();                           
	}
	fbo.unload();
    
	antenna0.saveImage("Ant00.png");
	antenna1.saveImage("Ant01.png");
	antenna2.saveImage("Ant02.png");
	antenna3.saveImage("Ant03.png");

	int global_time = 0.0;

	// Crude timing for rough FPS estimate
	time_t start_time = time(NULL);
	unsigned int iterations = 0;
    
	// determine Whether to use summedNoise or the viewA/B cycle
	int viewCase = 0;
    
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
		// antenna1.azimuth = global_time * 0.005;
		// antenna1.calculateLoss(pfQuad);

		// Spin the receiving antenna
		double fractPart, intPart;
		fractPart = modf(global_time*0.0005, &intPart);
		rcvrPointing.x =fractPart;
        
		switch (viewCase) {
		case 1:
			// draw the B side (to get the A View)
			pr_viewB.Load();
			fbo.load();
			fbo.bindTextureLayer(viewA_id, 0);
			im_viewB.sendTo(pr_viewB);
			rcvrPointing.sendTo(pr_viewB);
			mbQuad.draw();
			fbo.unbindTextureLayer();
			fbo.unload();
                
			// Output the result to the screen
			pr_viewMax.Load();
			im_viewA.sendTo(pr_viewMax);
			transfer.sendTo(pr_viewMax);
			mvQuad.draw();
			glfwSwapBuffers(window);
			glfwPollEvents();
                
			// draw the A side (to get the B view)
			pr_viewA.Load();
			fbo.load();
			fbo.bindTextureLayer(viewB_id, 0);
			im_viewA.sendTo(pr_viewA);
			rcvrPointing.sendTo(pr_viewA);
			maQuad.draw();
			fbo.unbindTextureLayer();
			fbo.unload();
			break;
                
		default:
			summedNoise.Load();
			rcvrPointing.sendTo(summedNoise);
			snQuad.draw();
			glfwSwapBuffers(window);
			glfwPollEvents();
			break;
		}
        
		iterations++;
		global_time++;

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


