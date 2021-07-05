// Lib used.
#include <cmath>
#include <iostream> // Output 
#include <fstream> // Reading Files
#include <glm/glm.hpp>// MAT4
#include <glm/gtc/matrix_transform.hpp> // Lookat func
#include <glm/gtc/type_ptr.hpp>// MAt4 Value Pointer
#include <glm/gtc/matrix_inverse.hpp>

/* Libraries for Apple and Pc*/
#ifdef __APPLE__ // In order for this code to be used on different Operating systems
#  include <GL/glew.h>
#  include <GL/freeglut.h>
#  include <OpenGL/glext.h>
#else
#  include <GL/glew.h>
#  include <GL/freeglut.h>
#pragma comment(lib, "glew32.lib") 
#endif


/* Header Files to be Included */

#include "readshaders.h"
#include "vertex.h"   
#include "sphere.h"
#include "SOIL/SOIL.h" // Have it In Linker/Input  has to be before opengl links.
#include "ImpModel.h"  // Used to import Models. 
#include "Skybox.h"   // Sets Cube Around the play Area as Skybox. 

/* <https://codeyarns.com/tech/2015-09-14-how-to-check-error-in-glew.html>*/
// Error Checking For GLEW, if glewInit() does not return Glew_OK
#define GLEW_OK 0
#define GLEW_NO_ERROR 0
#define GLEW_ERROR_NO_GL_VERSION 1  /* GL version could not be found*/
#define GLEW_ERROR_GL_VERSION_10_ONLY 2  /* Specifies that a minimum of OpenGL 1.1 is required */
#define GLEW_ERROR_GLX_VERSION_11_ONLY 3  /*  Specifies that a minimum of GLX 1.2 is required*/


/* VAO and VBO ids*/

// declares object, with types of objects. ( Have to be In order WIth Vertex/Fragment In #Define
static enum object { TRACK,  HOVER, SPHERE, SKYBOX}; // VAO ids.
static enum buffer { TRACK_VERTICES , HOVER_VERTICES, SPHERE_VERTICES, SPHERE_INDICES, SKYBOX_VERTICES};



/* Lighting */
struct Material
{
	vec4 ambRefl;
	vec4 difRefl;
	vec4 specRefl;
	vec4 emitCols;
	float shininess;
};

struct Light
{
	vec4 ambCols;
	vec4 difCols;
	vec4 specCols;
	vec4 coords;
};

static const vec4 globAmb = vec4(0.2, 0.2, 0.2, 1.0);

/* Material properties of a Sphere */
static const Material sphereFandB =
{
	vec4(1.0, 1.0, 0.0, 1.0),
	vec4(1.0, 1.0, 0.0, 1.0),
	vec4(1.0, 1.0, 0.0, 1.0),
	vec4(0.0, 0.0, 0.0, 1.0),
	50.0f
};

static const Light light0 =
{
	vec4(0.0, 0.0, 0.0, 1.0),
	vec4(1.0, 1.0, 1.0, 1.0),
	vec4(1.0, 1.0, 1.0, 1.0),
	vec4(1.0, 1.0, 0.0, 0.0)
};


/* CUBE DATA */
int size = 4.0f; // Enables us to Set the Size of the Cube
static const float vertices[] = {
	-size,-size,-size, // triangle begin
	-size,-size, size,
	-size, size, size, // triangle end
	size, size,-size,
	-size,-size,-size,
	-size, size,-size,
	size,-size, size,
	-size,-size,-size,
	size,-size,-size,
	size, size,-size,
	size,-size,-size,
	-size,-size,-size,
	-size,-size,-size,
	-size, size, size,
	-size, size,-size,
	size,-size, size,
	-size,-size, size,
	-size,-size,-size,
	-size, size, size,
	-size,-size, size,
	size,-size, size,
	size, size, size,
	size,-size,-size,
	size, size,-size,
	size,-size,-size,
	size, size, size,
	size,-size, size,
	size, size, size,
	size, size,-size,
	-size, size,-size,
	size, size, size,
	-size, size,-size,
	-size, size, size,
	size, size, size,
	-size, size, size,
	size,-size, size
};


// Used in Definition of the camera. With ModelViewMat and ProjMat
static glm::mat4 modelViewMat(1.0f);
static glm::mat4 projMat(1.0f);
static glm::mat3 normalMat = glm::mat3(1.0); // Normalization of Camera

//Program Location " Used to Set Shaders and Send Data for them.
unsigned int programId, vertexShaderId, fragmentShaderId, modelViewMatLoc, projMatLoc, objectLoc, yValLoc, grassTexLoc, skyTexLoc, cosmosTexLoc, redTexLoc;


/* VBO (BUFFER) VAO and textures  ( INCRESE WITH NEW OBJECTS )*/
unsigned int buffer[3];
unsigned int vao[3];
unsigned int texture[4];

/* Sphere Vertices, Normals, Triangle indices */
static VertexWtihNormal* sphereVerticesNor = NULL;
static unsigned int* sphereIndices = NULL;
static Sphere sphere; // instance of a sphere..


// Specify Texture Location.
std::string TextureList[] = {
	"Textures/grass.bmp",
	"Textures/sky.bmp",
	 "Textures/cosmos.png",
	  "Textures/red.png",
};


/* GLOBAL VERIABLES */

/* Sphere Z position Coordinate for the Sphere */
static float SphereZ = 0;

/* CAMERA */
static float zVal = 0; // Z Co-ordinates of the ball.
static float xVal = 0; // X Co-ordinates of the hover.
static float yVal = 0; // Y Co-ordinates of the track.
static float CamPosX = 0.0; //Camera position
static float CamPosY = 7.0;

// YAW AND PITCH DEFINITIONS 
float cameraYaw = 90;
float cameraPitch;
glm::vec3 cameraPosition = glm::vec3(0, 0, 0);
glm::vec3 cameraForward = glm::vec3(0.0, 0.0, 15.0);



/* Object Imports*/
static ImpModel Track("racetrack.obj");
static ImpModel Hover("spaceship.obj");
Skybox skybox; // Make an instance of a skybox.

/* SPACESHIP ANIMATION */
static int turnCar = 0;
static int moveCar = 0;
static float angleCar = 0;

bool drawsphere = false;
float spheremove = 0;

float test = 0;
int state = 1;

/* PROJECT's Initializaiton ( Set Before the start of the game ) */
void setup(void)
{
	// "Red, Green, Blue, Alpha " Specifies the values after colour Buffers are cleared, Default is 0"
	glClearColor(1.0, 1.0, 1.0, 0.0);

	
	// If enabled, Clear buffer with GL_DEPTH_BUFFER_BIT
   // stores fragments their z-values in the depth buffer if they 
   // passed the depth test and discards fragments if they failed the depth test accordingly.
	glEnable(GL_DEPTH_TEST);
	// < https://stackoverflow.com/questions/28137027/why-do-i-need-glcleargl-depth-buffer-bit >
	glDepthFunc(GL_LESS);


	// Set from Shader.h  Create Shader Program Executable
	vertexShaderId = setShader("vertex", "vertexShader.glsl");
	fragmentShaderId = setShader("fragment", "fragmentShader.glsl");

	/* ReadShaders Fragment and Vertex */
	std::cout << " Establishing Shaders " << std::endl;
	programId = glCreateProgram();
	glAttachShader(programId, vertexShaderId);
	glAttachShader(programId, fragmentShaderId);
	glLinkProgram(programId);
	glUseProgram(programId);


	/* Sphere Lighting See Properties */
	glUniform4fv(glGetUniformLocation(programId, "sphereFandB.ambRefl"), 1, &sphereFandB.ambRefl[0]);
	glUniform4fv(glGetUniformLocation(programId, "sphereFandB.difRefl"), 1, &sphereFandB.difRefl[0]);
	glUniform4fv(glGetUniformLocation(programId, "sphereFandB.specRefl"), 1, &sphereFandB.specRefl[0]);
	glUniform4fv(glGetUniformLocation(programId, "sphereFandB.emitCols"), 1, &sphereFandB.emitCols[0]);
	glUniform1f(glGetUniformLocation(programId, "sphereFandB.shininess"), sphereFandB.shininess);

	glUniform4fv(glGetUniformLocation(programId, "globAmb"), 1, &globAmb[0]);
	glUniform4fv(glGetUniformLocation(programId, "light0.ambCols"), 1, &light0.ambCols[0]);
	glUniform4fv(glGetUniformLocation(programId, "light0.difCols"), 1, &light0.difCols[0]);
	glUniform4fv(glGetUniformLocation(programId, "light0.specCols"), 1, &light0.specCols[0]);
	glUniform4fv(glGetUniformLocation(programId, "light0.coords"), 1, &light0.coords[0]);

	/* VBO */
	std::cout << " Generating VAO and Buffers " << std::endl;

	// Generate VAO and VBO ( Buffer ) ID's
	glGenVertexArrays(3, vao);
	glGenBuffers(3, buffer);

	/* CUBE */
	//glBindVertexArray(vao[CUBE]);
	//glBindBuffer(GL_ARRAY_BUFFER, buffer[CUBE_VERTICES]);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, 3 * sizeof(float), 0);
	//glEnableVertexAttribArray(0);

	/* FIELD BINDING */
	std::cout << " Binding VAO and VBO" << std::endl;
	

	/*RACETRACK BINDING */ // Remember Adds another glGenVertexArrays 
	glGenBuffers(1, &buffer[TRACK_VERTICES]);
	Track.SetIDs(vao[TRACK], buffer[TRACK_VERTICES], 0);
	Track.Setup();

	//Binding Track VAO and VBO
	glGenBuffers(1, &buffer[HOVER_VERTICES]); ///generate one more id for VBO
	Hover.SetIDs(vao[HOVER], buffer[HOVER_VERTICES], 0);
	Hover.Setup();

	
	// Obtain projection matrix uniform location and set value.
	projMatLoc = glGetUniformLocation(programId, "projMat"); //uniform mat4 projMat;
	projMat = perspective(radians(60.0), 1.0, 0.1, 1000.0);
	glUniformMatrix4fv(projMatLoc, 1, GL_FALSE, glm::value_ptr(projMat)); //< https://www.khronos.org/registry/OpenGL-Refpages/es2.0/xhtml/glUniform.xml > 

	// Obtain modelview matrix uniform and object uniform locations.
	modelViewMatLoc = glGetUniformLocation(programId, "modelViewMat"); // uniform ModelViewMatrix
	objectLoc = glGetUniformLocation(programId, "object");             // Uniform uint Object

	/* Texture Names At Global*/
	// Checks textures inside TextureList[] takes size, and generates Textures.
	int texSize = sizeof(TextureList) / sizeof(TextureList[0]); // This Breaks Do Not Add !!
	glGenTextures(3, texture);


	/* https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glTexImage2D.xhtml Reference */
	/* Texture Data information */
	int width, height;
	unsigned char* data;

	/* Bind Grass Image */
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture[0]);

	// Load Image Data with SOIL Library
	data = SOIL_load_image(TextureList[0].c_str(), &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	SOIL_free_image_data(data);

	// Establish Texture Paremeters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glGenerateMipmap(GL_TEXTURE_2D);
	grassTexLoc = glGetUniformLocation(programId, "grassTex");
	glUniform1i(grassTexLoc, 0); // texture to shader


	/* BIND SKY TEXTURE */
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture[1]);  // See glGenTextures and TextureList

	data = SOIL_load_image(TextureList[1].c_str(), &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	SOIL_free_image_data(data);

	// Generate SKy Textures
	glGenerateMipmap(GL_TEXTURE_2D); // Generates map for the picture
	skyTexLoc = glGetUniformLocation(programId, "skyTex");
	glUniform1i(skyTexLoc, 1); //send texture to shader

	/* Cosmos Texture */
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, texture[2]);

	data = SOIL_load_image(TextureList[2].c_str(), &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	SOIL_free_image_data(data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	cosmosTexLoc = glGetUniformLocation(programId, "cosmosTex"); // cosmosTex To take in at Vertex
	glUniform1i(cosmosTexLoc, 2); //send texture to shader

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, texture[3]);

	data = SOIL_load_image(TextureList[3].c_str(), &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	SOIL_free_image_data(data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	redTexLoc = glGetUniformLocation(programId, "redTex");
	glUniform1i(redTexLoc, 3); //send texture to shader




    /* GENERATE SKYBOX Buffers vaos set textures */
	glGenBuffers(1, &buffer[SKYBOX_VERTICES]);
	skybox.InitialiseSkybox(vao[SKYBOX], buffer[SKYBOX_VERTICES]);
	skybox.InitialiseCubeMap(programId, texture[3]);

	//Binding VAO and VBO
	sphere.SetIDs(vao[SPHERE], buffer[SPHERE_VERTICES], buffer[SPHERE_INDICES]);
	sphere.Setup();


}

/* Scene Drawables Routine*/
void drawScene(void)
{
	/* Indicates which Values are to be Cleared  " Takes one/ Several Values" */
	// Clear Colour Buffers at the start of the Frame 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Calculate and update modelview matrix.  Of The Camera. 
	modelViewMat = glm::mat4(1.0);
	modelViewMat = glm::lookAt(glm::vec3(0.0, 10.0, 15.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
	glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));

	//Draw SkyBox
	glUniform1ui(objectLoc, SKYBOX);  //if (object == SKYBOX)
	skybox.SetViewMatrix(modelViewMatLoc, modelViewMat);
	skybox.Draw(programId);

	// CamPosx and CamPosY meaning that Elements Stay in their Relative Position On X and Z Axis Of the Screen.

	// Draw Track
	Track.updateModelMatrix(modelViewMatLoc, CamPosX, 0.2f, -200.0f, CamPosY); // X Position, size, Distance froM origin. Camera, height
	glUniform1ui(objectLoc, TRACK);  //if (object == TRACK)
	Track.Draw();

	// Draw Hover
	Hover.updateModelMatrix(modelViewMatLoc, CamPosX, 0.5f, -20.0f, CamPosY);
	glUniform1ui(objectLoc, HOVER);  //if (object == HOVER)
	Hover.Draw();

	// Gets Called When Ever SpaceBar is Pressed. 
	if (drawsphere == true)
	{
		sphere.updateModelMatrix(modelViewMatLoc, CamPosX, 0.2f); // D here keeps it on the Position so It does not move from the camera. 
		glUniform1ui(objectLoc, SPHERE);  //if (object == SPHERE)
		sphere.Draw();
	}



	// CUBE NOT DRAWABLE ANYMORE
	// Consider Adding Shaders in Fragment instead of usual fixed color And Position.
	/* CUBE Draws */
	//glUniform1ui(objectLoc, CUBE); // If object == CUBE
	//glBindVertexArray(vao[CUBE]);
	//glDrawArrays(GL_TRIANGLES, 0, 12 * 3);


	glutSwapBuffers(); // Change buffers when the current window is double buffered.
}

void timer(int) // Timer For Floating  Track..
{
	switch (state) {
	case 1:
		if (yVal < 8)
			yVal += 0.001;
		else
			state = -1;
		break;
	case -1:
		if (yVal > -10)
			yVal -= 0.001;
		else
			state = 1;
		break;
	}

}

/* ANIMATION ROUTINE*/
void animation() {

	// Update Object Position / Rotation .
	Hover.SetPosition(vec3(0 + turnCar, 0, 0 + moveCar), vec3(angleCar, 20, 0.0));// Movement Left/Right, up/down, Forward/Backwards.  ( Angle of SpaceShip) 
	Track.SetPosition(vec3(0, 0, -100), vec3(0.0, 1.0, 0.0)); // X, Y, Z Movement,  Angle of it.

	// If SpaceBar Is pressed Animated Sphere by position.
	// When Sphere reaches limit of -500 on Z axis Reset Position and Disable Spheremove.
	if (drawsphere == true) 	spheremove = spheremove - 0.2;
	if (spheremove <= -500)
	{
		drawsphere = false;
		spheremove = 0;
	}
	sphere.SetPosition(vec3(0, 0, spheremove)); // Initial Starting Position Of the Sphere.

	glutTimerFunc(0, timer, 0);

	 // Debug to See position of yVal 
	//std::cout << yVal << std::endl;

	//send  yVal to vertex shader For animation using Shaders
	yValLoc = glGetUniformLocation(programId, "yPos");  //uniform uint object;
	glUniform1f(yValLoc, yVal);

	/* refresh screen */
	glutPostRedisplay();
}
/*OpenGL window reshape routine.*/
void resize(int w, int h)
{
	glViewport(0, 0, w, h); // Specifies the part of the Window to which OpenGl, WIll draw in pixels
	// Conversts from normalized to pixels.
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//The initial display mode is used when creating top - level windows, subwindows, and overlays to determine the OpenGL display mode for the to - be - created window or overlay.
	//glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH);

	glOrtho(0.0, 100.0, 0.0, 100.0, -1.0, 1.0); // multiplies the current matrix by an orthographic matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity(); // set Current matrix into Identity Matrix.
}
/*  KEYBOARD PROCESSING ROUTINES, ACII keys*/
void keyInput(unsigned char key, int x, int y)
{
	switch (key)
	{
	/* SPACESHIP movement*/
	case 'a':
		std::cout << "durning left" << std::endl;
		turnCar -= 1;
		std::cout << turnCar << " Angle left " << std::endl;
		angleCar += 0.5; // Angle with Movement to left

		break;
	case 'd':
		std::cout << "durning right" << std::endl;
		turnCar += 1;
		std::cout << turnCar << " Angle right " << std::endl;
		angleCar -= 0.5; // Angle with Movement to Right


		break;
	case 'w':
		std::cout << "Moving Forward" << std::endl;
		moveCar -= 1;
		break;
	case 's':
		std::cout << "Going Back" << std::endl;
		moveCar += 1;
		break;
	case ' ':
		drawsphere = true;
		if (drawsphere == true && spheremove == 0) {
			std::cout << "Shooting" << std::endl;
		}
		break;
	case 'b':
		test += 0.1;
		break;
	case 27: // ESC button.
		std::cout << " Game has been terminated " << std::endl;
		exit(0);
		break;
	default:
		break;
	}
}
/* Keyboard Input for Non - ASCII keys*/
void specialKeyInput(int key, int x, int y)
{
	//https://www.opengl.org/resources/libraries/glut/spec3/node54.html

	// Move Camera Intiliazes CamPos with modelViewMat = lookat( ..  Under Sphere Animation
	if (key == GLUT_KEY_LEFT)
	{
		if (CamPosX > -50.0) CamPosX -= 0.5; // Change State By when True X Coord Camera
	}
	if (key == GLUT_KEY_RIGHT)
	{
		if (CamPosX < 15.0) CamPosX += 0.5;
	}
	if (key == GLUT_KEY_UP)
	{
		if (CamPosY < 15.0) CamPosY += 0.5; // Change State By when True Y Coord Camera
		
	}
	if (key == GLUT_KEY_DOWN)
	{
		if (CamPosY > -50.0) CamPosY -= 0.5;
	}
	glutPostRedisplay();
}


// Main routine.
int main(int argc, char** argv)
{
	glutInit(&argc, argv);  // Initializes GLUT library and negotiates a session with Window System

	/* Documentation from <http://freeglut.sourceforge.net/docs/api.php>*/
	glutInitContextVersion(4, 3); // Version 4.3
	// <https://sites.google.com/site/gsucomputergraphics/educational/initialization-tasks-in-an-opengl-program/opengl-context-and-profile>
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE); 

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(1080, 920); // original Size 500, 500 Display Window size
	glutInitWindowPosition(500, 50); // Specifies the location of the Window 500, 50 Centre Of screen.
	glutCreateWindow("Graphics Engine Alpha v0.1");
	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);
	glutIdleFunc(animation);
	glutKeyboardFunc(keyInput); // Process ACII keys
	glutSpecialFunc(specialKeyInput); // Process Non ACII Keys.

	glewExperimental = GL_TRUE;
	glewInit(); // Returns Error Value of a type GLenum " Checks For Error " <https://codeyarns.com/tech/2015-09-14-how-to-check-error-in-glew.html>

	// Tells Errors encountered by glewInit()

	const GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		std::cout << "GLEW Error: " << glewGetErrorString(err) << std::endl;
		exit(1);
	}

	setup(); // Intitalize setup proceedure 

	glutMainLoop(); // loop Until it is asked To stop Either by Esc or close application.
}