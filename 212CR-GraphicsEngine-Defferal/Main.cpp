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
#include "ImpModel.h"

/* <https://codeyarns.com/tech/2015-09-14-how-to-check-error-in-glew.html>*/
// Error Checking For GLEW, if glewInit() does not return Glew_OK
#define GLEW_OK 0
#define GLEW_NO_ERROR 0
#define GLEW_ERROR_NO_GL_VERSION 1  /* GL version could not be found*/
#define GLEW_ERROR_GL_VERSION_10_ONLY 2  /* Specifies that a minimum of OpenGL 1.1 is required */
#define GLEW_ERROR_GLX_VERSION_11_ONLY 3  /*  Specifies that a minimum of GLX 1.2 is required*/


/* VAO and VBO ids*/

// declares object, with types of objects. ( Have to be In order WIth Vertex/Fragment In #Define
static enum object { FIELD, SKY, SPHERE, TRACK, HOVER, CUBE }; // VAO ids.
static enum buffer { FIELD_VERTICES, SKY_VERTICES, SPHERE_VERTICES, SPHERE_INDICES, TRACK_VERTICES, HOVER_VERTICES, CUBE_VERTICES};


// FIELD Area Specification
static Vertex fieldVertices[] =
{
	{glm::vec4(100.0, 0.0, 100.0, 1.0), glm::vec2(8.0, 0.0)},
	{glm::vec4(100.0, 0.0, -100.0, 1.0), glm::vec2(8.0, 8.0)},
	{glm::vec4(-100.0, 0.0, 100.0, 1.0), glm::vec2(0.0, 0.0)},
	{glm::vec4(-100.0, 0.0, -100.0, 1.0), glm::vec2(0.0, 8.0)}
};

/* SKY Area Specification */
static Vertex skyVertices[] =
{
	// Fixed picture orientation by Changing Top left with bottom right, top right with bottom left. Vec4
	{vec4(-200.0, 120.0, -70.0, 1.0), vec2(1.0, 0.0)}, // Bottom right
	{vec4(-200.0, 0.0, -70.0, 1.0), vec2(1.0, 1.0)},// Top rigt corner
	{vec4(200.0, 120.0, -70.0, 1.0), vec2(0.0, 0.0)}, // bottom left
	{vec4(200.0, 0.0, -70.0, 1.0), vec2(0.0, 1.0)} // Top left
};

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
// Normalization of Camera
static glm::mat3 normalMat = glm::mat3(1.0);

//Program Location " Used to Set Shaders and Send Data for them.
unsigned int programId, vertexShaderId, fragmentShaderId, modelViewMatLoc, projMatLoc, objectLoc, grassTexLoc, skyTexLoc;


/* VBO (BUFFER) VAO and textures  ( INCRESE WITH NEW OBJECTS )*/
unsigned int buffer[6];
unsigned int vao[5];
unsigned int texture[2];
int texSize = 0; // Sets the Size of TextureList for Texture Generation.

// Specify Texture Location.
std::string TextureList[] = {
	"Textures/grass.bmp",
	"Textures/sky.bmp",
	"Textures/nightSky.bmp",
};


/* VERIABLES */

/* Sphere Z position Coordinate for the Sphere */
static float SphereZ = 0;

/* CAMERA */
static float CamPosX = 0.0;
static float CamPosY = 0.0;

/* Sphere Vertices, Normals, Triangle indices */
static VertexWtihNormal* sphereVerticesNor = NULL;
static unsigned int* sphereIndices = NULL;
static Sphere Sphere1;

// Keyboard
int costumnumber = 1; // Number used to select Texture  From Texture 1 or 2

/* Object Imports*/
static ImpModel Track("racetrack.obj");
static ImpModel Hover("spaceship.obj");

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
	glGenVertexArrays(5, vao);
	glGenBuffers(3, buffer);

	/* CUBE */
	glBindVertexArray(vao[CUBE]);
	glBindBuffer(GL_ARRAY_BUFFER, buffer[CUBE_VERTICES]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	/* FIELD BINDING */
	std::cout << " Binding VAO and VBO" << std::endl;
	glBindVertexArray(vao[FIELD]);
	glBindBuffer(GL_ARRAY_BUFFER, buffer[FIELD_VERTICES]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(fieldVertices), fieldVertices, GL_STATIC_DRAW); // Reserve Space

	// Sets Singular Array of generic vertex Attribute Data
	// Index - Specifies vertex Attribute to be Modified,
	// Size - Number of Components per generic Vertex Attribute ( Must be 1,2,3,4) Initial = 4
	// Type - of each component in Array
	// Normalized - GL_True When Fixed Point,  False when Accessed.
	// Stride - Specofies the byte offset Between Consecutive generic Vertex Attributes (Default 0)
	// Pointer - Specofies offset of first component. (Initial Value 0 ) GL_Array_buffer Target.
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(fieldVertices[0]), 0);

	// index - Specifies the index of the generic vertex attribute to be enabled or disabled.
	glEnableVertexAttribArray(0);  // layout(location=0) in vec4 Coords; ON VERTEXSHADER 
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(fieldVertices[0]), (void*)(sizeof(fieldVertices[0].coords)));
	glEnableVertexAttribArray(1); // layout(location=1) in vec2 TexCoords; ON VERTEXSHADER
	

	/* SKY BINDING */
	glBindVertexArray(vao[SKY]);
	glBindBuffer(GL_ARRAY_BUFFER, buffer[SKY_VERTICES]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyVertices), skyVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(skyVertices[0]), 0);
	glEnableVertexAttribArray(0);  // layout(location=0) in vec4 Coords;
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(skyVertices[0]), (void*)(sizeof(skyVertices[0].coords)));
	glEnableVertexAttribArray(1); // layout(location=1) in vec2 TexCoords;



	/*RACETRACK BINDING */ // Remember Adds another glGenVertexArrays 
	glGenBuffers(1, &buffer[TRACK_VERTICES]);
	Track.SetIDs(vao[TRACK], buffer[TRACK_VERTICES], 0);
	Track.Setup();

	//Binding Track VAO and VBO
	glGenBuffers(1, &buffer[HOVER_VERTICES]); ///generate one more id for VBO
	Hover.SetIDs(vao[HOVER], buffer[HOVER_VERTICES], 0);
	Hover.Setup();


	//Binding VAO and VBO
	Sphere1.SetIDs(vao[SPHERE], buffer[SPHERE_VERTICES], buffer[SPHERE_INDICES]);
	Sphere1.Setup();


	////////////////////////////////////////////////////////////////////
	// Obtain projection matrix uniform location and set value.
	projMatLoc = glGetUniformLocation(programId, "projMat"); //uniform mat4 projMat;
	projMat = glm::frustum(-5.0, 5.0, -5.0, 5.0, 5.0, 100.0);
	// < https://glm.g-truc.net/0.9.4/api/a00151.html > Glm Lib Reference
	// Dictates frustum Parameters, LEft, Right, Bottom, TOp, near, Far
	
	glUniformMatrix4fv(projMatLoc, 1, GL_FALSE, glm::value_ptr(projMat)); //< https://www.khronos.org/registry/OpenGL-Refpages/es2.0/xhtml/glUniform.xml > 

	// Obtain modelview matrix uniform and object uniform locations.
	modelViewMatLoc = glGetUniformLocation(programId, "modelViewMat"); // uniform ModelViewMatrix
	objectLoc = glGetUniformLocation(programId, "object");             // Uniform uint Object

	/* Texture Names At Global*/
	// Checks textures inside TextureList[] takes size, and generates Textures.
	texSize = sizeof(TextureList) / sizeof(TextureList[0]);
	glGenTextures(texSize, texture);


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
	glBindTexture(GL_TEXTURE_2D, texture[costumnumber]);  // See glGenTextures and TextureList

	//load image data using SOIL library
	data = SOIL_load_image(TextureList[costumnumber].c_str(), &width, &height, 0, SOIL_LOAD_RGBA);
	/* https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glTexImage2D.xhtml Reference */
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	SOIL_free_image_data(data);

	// Generate SKy Textures
	glGenerateMipmap(GL_TEXTURE_2D); // Generates map for the picture
	skyTexLoc = glGetUniformLocation(programId, "skyTex");
	glUniform1i(skyTexLoc, 1); //send texture to shader




}

/* Scene Drawables Routine*/
void drawScene(void)
{
	/* Indicates which Values are to be Cleared  " Takes one/ Several Values" */
	// Clear Colour Buffers at the start of the Frame 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// Calculate and update modelview matrix.
	modelViewMat = glm::mat4(1.0);
	modelViewMat = glm::lookAt(glm::vec3(0.0, 10.0, 15.0), glm::vec3(0.0 + CamPosX, 10.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
	glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));


	/* Initialize Colour And DRAW FIELD */
	glUniform1ui(objectLoc, FIELD);  //if (object == FIELD)
	glBindVertexArray(vao[FIELD]);         // Bind
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4); // Render Array of triangles 

	/* DRAW SKY */
	glUniform1ui(objectLoc, SKY);  //if (object == SKY)
	glBindVertexArray(vao[SKY]);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	// Draw sphere
	Sphere1.updateModelMatrix(modelViewMatLoc, CamPosX);
	glUniform1ui(objectLoc, SPHERE);  //if (object == SPHERE)
	Sphere1.Draw();

	// Draw Track
	Track.updateModelMatrix(modelViewMatLoc, CamPosX, 0.2f, -60.0f);
	glUniform1ui(objectLoc, TRACK);  //if (object == TRACK)
	Track.Draw();

	// Draw Hover
	Hover.updateModelMatrix(modelViewMatLoc, CamPosX, 1.5f, 0.0f);
	glUniform1ui(objectLoc, HOVER);  //if (object == HOVER)
	Hover.Draw();

	// CUBE NOT DRAWABLE ANYMORE
	// Consider Adding Shaders in Fragment instead of usual fixed color And Position.
	/* CUBE Draws */
	//glUniform1ui(objectLoc, CUBE); // If object == CUBE
	//glBindVertexArray(vao[CUBE]);
	//glDrawArrays(GL_TRIANGLES, 0, 12 * 3);


	glutSwapBuffers(); // Change buffers when the current window is double buffered.
}



/* ANIMATION ROUTINE*/
void animation() {

	/* update state variables */
	/* SPHERE */
	SphereZ = SphereZ - 0.2;
	if (SphereZ < -25.0) SphereZ = 0.0;
	Sphere1.SetPosition(vec3(0, 0, SphereZ));


	/* refresh screen */
	glutPostRedisplay();
}
/* OpenGL window reshape routine.*/
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
	case 101: // E Button on Keyboard.
	// Change texture image of the sky.
	std::cout << " Key E was pressed" << std::endl;
	costumnumber = costumnumber == 1 ? 2 : 1; // Change between Numbers.

	// https://www.w3schools.com/cpp/cpp_conditions_shorthand.asp Ternary operator
	setup(); // Rerun Setup and Replace Do not use Future Project Unnesecary usage,
	//As All textures have to be reapplied.

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
}


// Main routine.
int main(int argc, char** argv)
{
	glutInit(&argc, argv);  // Initializes GLUT library and negotiates a session with Window System

	/* Documentation from <http://freeglut.sourceforge.net/docs/api.php>*/
	glutInitContextVersion(4, 3); // Version 4.3
	// <https://sites.google.com/site/gsucomputergraphics/educational/initialization-tasks-in-an-opengl-program/opengl-context-and-profile>
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE); 

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
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