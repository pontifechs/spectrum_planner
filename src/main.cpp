
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

enum ViewMode
{
	GainPattern,
	AlphaMap,
	SinglePathLoss,
    StaticSum,      // Test001 20140519
	// SummedNoise, // Test001 20140519
    MovingAnt,      // Test001 20140519
	Maximizer       // Test001 20140519
};

struct  nodePoint{  // Test001 20140519
    float   x;      // Test001 20140519
    float   y;      // Test001 20140519
    float   az;     // Test001 20140519
    int     state;  // Test002 20140519
};                  // Test001 20140519

ViewMode mode = GainPattern;
bool paused = false;

bool txRotate = false;
bool txTranslate = false;
bool rxRotate = false;


// Pointers to some critical things needed inside of the key callback
Framebuffer* fboPtr;    
UImageArray* viewAPtr;
UImageArray* viewBPtr;

UFloat* gainPatternLayerPtr;
UFloat* antennaLayerPtr;

static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}

float defaultAntenna;   // Test003 20140519
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	switch (key)
	{
	case GLFW_KEY_ESCAPE:
		glfwSetWindowShouldClose(window, GL_TRUE);
		break;

	case GLFW_KEY_SPACE:
		if (action == GLFW_PRESS)
		{
			paused = !paused;
		}
		break;

	case GLFW_KEY_1:
		if (action == GLFW_PRESS)
		{
			*gainPatternLayerPtr = (int)(gainPatternLayerPtr->val + 1) % 5;
			gainPatternLayerPtr->send();
		}
		mode = GainPattern;
		break;

	case GLFW_KEY_2:
		mode = AlphaMap;
		break;

	case GLFW_KEY_3:
		if (action == GLFW_PRESS)
		{
			*antennaLayerPtr = (int)(antennaLayerPtr->val + 1) % 4;
			antennaLayerPtr->send();
		}
		mode = SinglePathLoss;
		break;

	case GLFW_KEY_4:
		// mode = SummedNoise; // Test001 20140519
        mode = StaticSum;  // Test001 20140519
		break;

	case GLFW_KEY_5:
		// mode = Maximizer;    // Test001 20140519
        mode = MovingAnt;   // Test001 20140519
		// viewAPtr->clearAll(*fboPtr);  // Test001 20140519
		// viewBPtr->clearAll(*fboPtr);  // Test001 20140519
		break;
            
    case GLFW_KEY_6:
        mode = Maximizer;           // Test001 20140519
        viewAPtr->clearAll(*fboPtr);  // Test001 20140519
        viewBPtr->clearAll(*fboPtr);  // Test001 20140519
        break;

    case GLFW_KEY_J:
        defaultAntenna = 0.0;       // Test003 20140519
        break;
            
    case GLFW_KEY_K:
        defaultAntenna = 1.0;       // Test003 20140519
        break;
            
    case GLFW_KEY_L:
        defaultAntenna = 2.0;       // Test003 20140519
        break;
            
    case GLFW_KEY_M:
        defaultAntenna = 3.0;       // Test003 20140519
        break;
 
    case GLFW_KEY_N:
        defaultAntenna = 4.0;       // Test003 20140519
        break;
            
	case GLFW_KEY_T:
		if (action == GLFW_PRESS)
		{
			txRotate = !txRotate;
		}
		break;
	case GLFW_KEY_X:
		if (action == GLFW_PRESS)
		{
			txTranslate = !txTranslate;
		}
		break;
	case GLFW_KEY_R:
		if (action == GLFW_PRESS)
		{
			rxRotate = !rxRotate;
		}
		break;

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

// Set up GLEW
void initGLEW()
{
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

std::vector<Antenna> buildAntennas(Program powerfield,
                                   Framebuffer fbo,
									UImageArray loss_array,
									UImageArray gain_patterns,
									Vec2 resolution)
{

	AMesh screenFill = setupQuad(powerfield);
	std::vector<Antenna> antennas(4);

	antennas[0] = Antenna(powerfield, "antenna", fbo, loss_array, gain_patterns);
	antennas[0].position = Vec2(0.25, 0.3583) * resolution;
	antennas[0].azimuth = M_PI / 2.0;
	antennas[0].power = 3.0;
	antennas[0].gainPattern = 0.0;
	
	antennas[1] = Antenna(powerfield, "antenna", fbo, loss_array, gain_patterns);
	antennas[1].position = Vec2(0.821875, 0.65) * resolution;
	antennas[1].azimuth = -9.0 * M_PI / 16.0;
	antennas[1].power = 15.0;
	antennas[1].gainPattern = 1.0;

	antennas[2] = Antenna(powerfield, "antenna", fbo, loss_array, gain_patterns);
	antennas[2].position = Vec2(0.7125, 0.79583) * resolution;
	antennas[2].azimuth = - M_PI;
	antennas[2].power = 15.0;
	antennas[2].gainPattern = 2.0;

	antennas[3] = Antenna(powerfield, "antenna", fbo, loss_array, gain_patterns);
	antennas[3].position = Vec2(0.49375, 0.3583) * resolution;
	antennas[3].azimuth = - M_PI/2.0;
	antennas[3].power = 8.0;
	antennas[3].gainPattern = 4.0;
	
	antennas[0].calculateLoss(screenFill);
	antennas[1].calculateLoss(screenFill);     
	antennas[2].calculateLoss(screenFill);     
	antennas[3].calculateLoss(screenFill);     

	return antennas;
}
// Function is a test of mutiple Antenna reset 20140519
void setAntMult(std::vector<Antenna> antennas, Vec2 resolution, AMesh screenFill)
{
	antennas[0].position = Vec2(0.25, 0.3583) * resolution;
	antennas[0].azimuth = M_PI / 2.0;
	antennas[0].power = 3.0;
	antennas[0].gainPattern = 0.0;
	
	antennas[1].position = Vec2(0.821875, 0.65) * resolution;
	antennas[1].azimuth = -9.0 * M_PI / 16.0;
	antennas[1].power = 15.0;
	antennas[1].gainPattern = 1.0;
    
	antennas[2].position = Vec2(0.7125, 0.79583) * resolution;
	antennas[2].azimuth = - M_PI;
	antennas[2].power = 15.0;
	antennas[2].gainPattern = 1.0;
    
	antennas[3].position = Vec2(0.49375, 0.3583) * resolution;
	antennas[3].azimuth = - M_PI/2.0;
	antennas[3].power = 8.0;
	antennas[3].gainPattern = 1.0;
	
	antennas[0].calculateLoss(screenFill);
	antennas[1].calculateLoss(screenFill);
	antennas[2].calculateLoss(screenFill);
	antennas[3].calculateLoss(screenFill);

}

// Fuction is a test of ssingel antenna equivalent 20140519
// i.e. set all antennas ecept 1 to power of 0 & Omni-directional
void setAntSingle(std::vector<Antenna> antennas, Vec2 resolution, AMesh screenFill)
{
    antennas[0].position = Vec2(0.25, 0.3583) * resolution;
	antennas[0].azimuth = M_PI / 2.0;
	antennas[0].power = 0.0;
	antennas[0].gainPattern = 0.0;
	
	antennas[1].position = Vec2(0.821875, 0.65) * resolution;
	antennas[1].azimuth = -9.0 * M_PI / 16.0;
	antennas[1].power = 10.0;
	antennas[1].gainPattern = 2.0;
    
	antennas[2].position = Vec2(0.7125, 0.79583) * resolution;
	antennas[2].azimuth = - M_PI;
	antennas[2].power = 0.0;
	antennas[2].gainPattern = 0.0;
    
	antennas[3].position = Vec2(0.49375, 0.3583) * resolution;
	antennas[3].azimuth = - M_PI/2.0;
	antennas[3].power = 0.0;
	antennas[3].gainPattern = 0.0;
	
	antennas[0].calculateLoss(screenFill);
	antennas[1].calculateLoss(screenFill);
	antennas[2].calculateLoss(screenFill);
	antennas[3].calculateLoss(screenFill);
}

enum{           // Test001 20140519
    RIGHT,      // Test001 20140519
    TURN1,      // Test001 20140519
    TURN2,      // Test001 20140519
    LEFT        // Test001 20140519
};              // Test001 20140519
// New function  Test001 20140519
nodePoint getNewPointing( nodePoint curr, float delta){
    nodePoint   newPos;
    float   s, v, r, l, maxCircuit, maxTurn;
    float   dx, cl, cr, cy, num ;
    float   d_turn1, d_right, d_turn2, d_left;
    int     myCase;
    
    r   = 0.1;
    v   = 0.5;
    cl  = 0.65;
    cr  = 0.8;
    l   = cr - cl;
    cy  = 0.7;
    
    maxTurn     = 3.14159*r;
    maxCircuit  = 2*l + 2*maxTurn;
    num         = 0;
    
    s   = v*delta;          // Calculate the distance traveled
    // Make sure that this distance is less than a full circuit
    if (s > maxCircuit) {
        num = floor(s/maxCircuit);
    }
    s   = s - num * maxCircuit;
    
    switch (curr.state) {
        case RIGHT:
            d_turn1 = cr - curr.x;           // dist to enter turn 1
            d_left  = d_turn1 + maxTurn;    // dist to enter left section
            d_turn2 = d_left + l;           // dist to enter turn 1
            d_right = d_turn2 + maxTurn;    // dist to return to right section

            if (s > d_turn1) {
                if (s > d_left) {
                    if (s > d_turn2) {
                        if (s > d_right) {
                            dx      = s - d_right;
                            myCase  = RIGHT;
                        } else {
                            dx      = s - d_turn2;
                            myCase  = TURN2;
                        }
                    } else {
                        dx      = s - d_left;
                        myCase  = LEFT;
                    }
                } else {
                    dx      = s - d_turn1;
                    myCase  = TURN1;
                }
            } else {
                dx      = curr.x + s - cl;
                myCase  = RIGHT;
            }
            break;
            
        case TURN1:
            d_left  = r * (3.14159 - curr.az);
            d_turn2 = d_left + l;
            d_right = d_turn2 + maxTurn;
            d_turn1 = d_right + l;

            if (s > d_left) {
                if (s > d_turn2) {
                    if (s > d_right) {
                        if (s > d_turn1) {
                            dx      = s - d_turn1;
                            myCase  = TURN1;
                        } else {
                            dx      = s - d_right;
                            myCase  = RIGHT;
                        }
                    } else {
                        dx      = s - d_turn2;
                        myCase  = TURN2;
                    }
                } else {
                    dx      = s - d_left;
                    myCase  = LEFT;
                }
            } else {
                dx      = r * curr.az + s;
                myCase  = TURN1;
            }
            break;
            
        case LEFT:
            d_turn2 = curr.x - cl;
            d_right = d_turn2 + maxTurn;
            d_turn1 = d_right + l;
            d_left  = d_turn1 + maxTurn;

            if (s > d_turn2) {
                if (s > d_right) {
                    if (s > d_turn1) {
                        if (s > d_left) {
                            dx      = s - d_left;
                            myCase  = LEFT;
                        } else {
                            dx      = s - d_turn1;
                            myCase  = TURN1;
                        }
                    } else {
                        dx      = s - d_right;
                        myCase  = RIGHT;
                    }
                } else {
                    dx      = s - d_turn2;
                    myCase  = TURN2;
                }
            } else {
                dx      = cr - curr.x + s;
                myCase  = LEFT;
            }
            break;
            
        case TURN2:
            d_right = r * (2 * 3.14159 - curr.az);
            d_turn1 = d_right + l;
            d_left  = d_turn1 + maxTurn;
            d_turn2 = d_left + l;

            if (s > d_right) {
                if (s > d_turn1) {
                    if (s > d_left) {
                        if (s > d_turn2) {
                            dx      = s - d_turn2;
                            myCase  = TURN2;
                        } else {
                            dx      = s -d_left;
                            myCase  = LEFT;
                        }
                    } else {
                        dx      = s - d_turn1;
                        myCase  = TURN1;
                    }
                } else {
                    dx      = s - d_right;
                    myCase  = RIGHT;
                }
            } else {
                dx      = r * (curr.az - 3.14159) + s;
                myCase  = TURN2;
            }
            break;
            
        default:
            newPos.x    = 0.45;
            newPos.y    = 0.35;
            newPos.az   = 0;
            myCase      = -1;
            break;
    }
    
    switch (myCase) {
        case RIGHT:
            newPos.x    = cl + dx;
            newPos.y    = cy - r;
            newPos.az   = 0;
            newPos.state= RIGHT;
            break;
        
        case TURN1:
            newPos.az   = dx/r;
            newPos.x    = cr + r * sin(curr.az);
            newPos.y    = cy - r * cos(curr.az);
            newPos.state= TURN1;
            break;
        
        case LEFT:
            newPos.x    = cr - dx;
            newPos.y    = cy + r;
            newPos.az   = 3.14159;
            newPos.state= LEFT;
            break;
            
        case TURN2:
            newPos.az    = 3.14159 + dx/r;
            newPos.x    = cl + r * sin(curr.az);
            newPos.y    = cy - r * cos(curr.az);
            newPos.state= TURN2;
            break;
            
        default:
            newPos.x    = cl;
            newPos.y    = cy - r;
            newPos.az   = 0;
            newPos.state= RIGHT;
            break;
    }
    
    return newPos;
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
	Shader offsetAnglesFrag(res + "/shaders/offsetAngles.frag", Shader::FRAGMENT);
	Shader sh_viewAFrag(res + "/shaders/fr_maxA.frag", Shader::FRAGMENT);
	Shader sh_viewBFrag(res + "/shaders/fr_maxB.frag", Shader::FRAGMENT);
	Shader sh_viewMaxFrag(res + "/shaders/fr_viewMax.frag", Shader::FRAGMENT);
	Shader gainPatternKeyFrag(res + "/shaders/gainpattern_key.frag", Shader::FRAGMENT);
	Shader alphaMapFrag(res + "/shaders/texthrough.frag", Shader::FRAGMENT);
	Shader singlePathLossFrag(res + "/shaders/single_path_loss.frag", Shader::FRAGMENT);

	Program powerfield(passthroughVert, powerfieldFrag);
	Program summedNoise(passthroughVert, summedNoiseFrag);
	Program offsetAngles(passthroughVert, offsetAnglesFrag);  
	Program pr_viewA(passthroughVert, sh_viewAFrag);
	Program pr_viewB(passthroughVert, sh_viewBFrag);
	Program pr_viewMax(passthroughVert, sh_viewMaxFrag);
	Program gainPatternKey(passthroughVert, gainPatternKeyFrag);
	Program alphaMap(passthroughVert, alphaMapFrag);
	Program singlePathLoss(passthroughVert, singlePathLossFrag);
    

	// Set up FBO and Texture arrays
	Framebuffer fbo;
	fboPtr = &fbo;
	fbo.load();
	fbo.unload();

	UImageArray loss_array = UImageArray(summedNoise, "antennas", 640, 480, 8);
	UImageArray angleArray = UImageArray(summedNoise, "angles",   640, 480, 8);
	UImageArray im_viewA = UImageArray(pr_viewA, "viewA", 640, 480, 1);
	UImageArray im_viewB = UImageArray(pr_viewB, "viewB", 640, 480, 1);
	viewAPtr = &im_viewA;
	viewBPtr = &im_viewB;
    
	GLuint viewA_id = im_viewA.getId();
	GLuint viewB_id = im_viewB.getId();
    
	loss_array.sendTo(summedNoise);
	angleArray.sendTo(summedNoise);

	im_viewB.sendTo(pr_viewA);
	loss_array.sendTo(pr_viewA);
	angleArray.sendTo(pr_viewA);
    
	im_viewA.sendTo(pr_viewB);
	loss_array.sendTo(pr_viewB);
	angleArray.sendTo(pr_viewB);
    
	im_viewA.sendTo(pr_viewMax);

	loss_array.sendTo(singlePathLoss);
    
	// Meshes are needed to fill the screen for the fragment shader
	AMesh screenFill = setupQuad(powerfield);
    
	// Powerfield Uniforms
	Image omni(res + "/tex/antenna_omni.png");
	Image dir60_S(res + "/tex/antenna_60_simple.png");
	Image dir30_S(res + "/tex/antenna_30_simple.png");
	Image dir05_S(res + "/tex/antenna_05_simple.png");
    Image dirSin6x_S(res + "/tex/antenna_sinx.png");
 
	// Create the gain patterns for the powerField fragment shader
	UImageArray gain_patterns(powerfield, "gain_patterns", 512, 1, 5);
	gain_patterns.setLayer(omni, 0);        
	gain_patterns.setLayer(dir60_S, 1);     
	gain_patterns.setLayer(dir30_S, 2);
	gain_patterns.setLayer(dir05_S, 3);
    gain_patterns.setLayer(dirSin6x_S, 4);
    
	gain_patterns.sendTo(powerfield);
	gain_patterns.sendTo(summedNoise);
	gain_patterns.sendTo(gainPatternKey);

	UFloat gainPatternLayer(gainPatternKey, "gainPatternLayer", 0);
	gainPatternLayerPtr = &gainPatternLayer;

	UFloat antennaLayer(singlePathLoss, "antennaLayer", 0);
	antennaLayerPtr = &antennaLayer;

	UImage transfer(summedNoise, "transfer", res + "/tex/blue_yellow_dark_red_transfer.png", false);
	transfer.sendTo(summedNoise);
	transfer.sendTo(pr_viewMax);

 
	UImage alpha_map(powerfield, "global_alpha", res + "/tex/global-alpha.jpg", false);
	alpha_map.sendTo(powerfield);
	alpha_map.sendTo(alphaMap);

	UVec2 resolution(powerfield, "resolution", 640, 480);
	resolution.sendTo(powerfield);
    
	UVec2 mAntPosition(offsetAngles, "AntPosition", 0.5,0.0);
	mAntPosition.sendTo(offsetAngles);
    
	float pointAngle=0.5, rcvrGainPattern = 1.0;
	UVec2 rcvrPointing(summedNoise, "rcvr", pointAngle, rcvrGainPattern);
	rcvrPointing.sendTo(summedNoise);
    
	gain_patterns.sendTo(pr_viewA);
	rcvrPointing.sendTo(pr_viewA);
    
	gain_patterns.sendTo(pr_viewB);
	rcvrPointing.sendTo(pr_viewB);
    
	// establish a few antennas to work with
	std::vector<Antenna> antennas = buildAntennas(powerfield, fbo, loss_array,	gain_patterns, resolution);
    
	std::vector<Vec2> positions(4);
	for (int i = 0; i < 4; ++i)
	{
		positions[i] = antennas[i].position;
	}
    
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
		screenFill.draw();                              
		fbo.unbindTextureLayer();                   
		glfwSwapBuffers(window);                    
		glfwPollEvents();                           
	}
	fbo.unload();
    
	
	// Crude timing for rough FPS estimate
	time_t start_time = time(NULL);
	int global_time = 0;
	int iterations = 0;
	
	float txTime = 0;

    int mIndex;         // test of theory 20140509_01
    bool multAntCase = true; // Test001 20140519
    nodePoint antPt;        // Test001 20140519
    antPt.x = 0.45;         // Test001 20140519
    antPt.y = 0.35;         // Test001 20140519
    antPt.az = 0;           // Test001 20140519
    antPt.state = RIGHT;    // Test002 20141519
    defaultAntenna = 3.0;   // test003 20140519
	// Draw loop
	while (!glfwWindowShouldClose(window))
	{
		global_time++;
		iterations++;
		

		// Handle pausing.
		while (paused)
		{
			glfwPollEvents();
		}
        
		// Clear screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (txRotate)
		{
			// For giggles, have antenna 3 spin around
            // antennas[1].azimuth += 0.005;
            antennas[1].azimuth += 0.001;
			antennas[1].calculateLoss(screenFill);
		}

		if (txTranslate)
		{
			txTime += 0.005;
			Vec2 center = Vec2(0.821875, 0.65) * resolution;
			center.x += 50*cos(txTime);
			center.y += 50*sin(txTime);
			
			antennas[1].position = center;
			antennas[1].calculateLoss(screenFill);
            
            // Test of angleoffset theory 20140509_01
            fbo.load();
            offsetAngles.Load();
            mIndex = 1;
            fbo.bindTextureLayer(anglesTextureID, mIndex);
            mAntPosition.x = center.x;
            mAntPosition.y = center.y;
            mAntPosition.sendTo(offsetAngles);
            screenFill.draw();
            fbo.unbindTextureLayer();
            fbo.unload();
		}

		if (rxRotate)
		{
			rcvrPointing.x += 0.005;
		}
        
		switch (mode) {

		case GainPattern:                       // Key 1
			gainPatternKey.Load();
			screenFill.sendTo(gainPatternKey);
			break;

		case AlphaMap:                          // Key 2
			alphaMap.Load();
			screenFill.sendTo(gainPatternKey);
			break;

		case SinglePathLoss:                    // Key 3
			singlePathLoss.Load();
			screenFill.sendTo(singlePathLoss);
			break;

//		case SummedNoise:                   // Test001 20140519
//			summedNoise.Load();
//			rcvrPointing.sendTo(summedNoise);
//			screenFill.sendTo(summedNoise);
//			break;
                
        case StaticSum:                         // Key 4
            summedNoise.Load();         // Test001 20140519
            txRotate = false;           // Test001 20140519
            rxRotate = false;           // Test001 20140519
            if(!multAntCase){           // Test001 20140519
                setAntMult(antennas, resolution, screenFill);  // Test001
                multAntCase = true;         // Test001 20140519
                }
            rcvrPointing.sendTo(summedNoise);  // Test001 20140519
            screenFill.sendTo(summedNoise);    // Test001 20140519
            break;
            
        case MovingAnt:                         // Key 5
            summedNoise.Load();         // Test001 20140519
            txRotate = false;           // Test001 20140519
            rxRotate = false;           // Test001 20140519
            if(multAntCase){            // Test001 20140519
                setAntSingle(antennas, resolution, screenFill); // Test001
                multAntCase = false;        // Test001 20140519
                antPt.x    = 0.55;          // Test001 20140519
                antPt.y    = 0.35;          // Test001 20140519
                antPt.az   = 0;             // Test001 20140519
                antPt.state= RIGHT;         // Test002 20140519
                }
            // calculate the new position and antenna pointing
            antPt   = getNewPointing( antPt, 0.001);  // Test001 20140519
            antennas[1].position    = Vec2(antPt.x,antPt.y) * resolution;
            antennas[1].azimuth     = antPt.az;
            antennas[1].gainPattern = defaultAntenna;
            antennas[1].calculateLoss(screenFill);
                
                // Test001 20140519
                fbo.load();
                offsetAngles.Load();
                mIndex = 1;
                fbo.bindTextureLayer(anglesTextureID, mIndex);
                mAntPosition.x = antPt.x;
                mAntPosition.y = antPt.y;
                mAntPosition.sendTo(offsetAngles);
                screenFill.draw();
                fbo.unbindTextureLayer();
                fbo.unload();
            
            
            rcvrPointing.y  = 0;
            rcvrPointing.sendTo(summedNoise);  // Test001 20140519
            screenFill.sendTo(summedNoise);    // Test001 20140519
            break;
                
		case Maximizer:                         // Key 6
            if(!multAntCase){           // Test001 20140519
//                setAntMult(antennas, resolution, screenFill);  // Test001
                antennas[0].position = Vec2(0.25, 0.3583) * resolution;  // Test004 20140519
                antennas[0].azimuth = M_PI / 2.0;
                antennas[0].power = 3.0;
                antennas[0].gainPattern = 0.0;
                
                antennas[1].position = Vec2(0.821875, 0.65) * resolution;
                antennas[1].azimuth = -9.0 * M_PI / 16.0;
                antennas[1].power = 15.0;
                antennas[1].gainPattern = 1.0;
                
                antennas[2].position = Vec2(0.7125, 0.79583) * resolution;
                antennas[2].azimuth = - M_PI;
                antennas[2].power = 15.0;
                antennas[2].gainPattern = 1.0;
                
                antennas[3].position = Vec2(0.49375, 0.3583) * resolution;
                antennas[3].azimuth = - M_PI/2.0;
                antennas[3].power = 8.0;
                antennas[3].gainPattern = 1.0;
                
                antennas[0].calculateLoss(screenFill);
                antennas[1].calculateLoss(screenFill);
                antennas[2].calculateLoss(screenFill);
                antennas[3].calculateLoss(screenFill);

                multAntCase = true;         // Test001 20140519
                }
            // draw the B side (to get the A View)
			pr_viewB.Load();
			fbo.load();
			fbo.bindTextureLayer(viewA_id, 0);
			im_viewB.sendTo(pr_viewB);
			rcvrPointing.sendTo(pr_viewB);
			screenFill.sendTo(pr_viewB);
			fbo.unbindTextureLayer();
			fbo.unload();
                
			// Output the result to the screen
			pr_viewMax.Load();
			im_viewA.sendTo(pr_viewMax);
			transfer.sendTo(pr_viewMax);
			screenFill.sendTo(pr_viewMax);
                
			// draw the A side (to get the B view)
			pr_viewA.Load();
			fbo.load();
			fbo.bindTextureLayer(viewB_id, 0);
			im_viewA.sendTo(pr_viewA);
			rcvrPointing.sendTo(pr_viewA);
			screenFill.sendTo(pr_viewA);
			fbo.unbindTextureLayer();
			fbo.unload();
			break;
		}

		glfwSwapBuffers(window);
		glfwPollEvents();

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


