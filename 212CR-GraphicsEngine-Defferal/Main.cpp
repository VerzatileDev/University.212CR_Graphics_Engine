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


/* <https://codeyarns.com/tech/2015-09-14-how-to-check-error-in-glew.html>*/
// Error Checking For GLEW, if glewInit() does not return Glew_OK
#define GLEW_OK 0
#define GLEW_NO_ERROR 0
#define GLEW_ERROR_NO_GL_VERSION 1  /* GL version could not be found*/
#define GLEW_ERROR_GL_VERSION_10_ONLY 2  /* Specifies that a minimum of OpenGL 1.1 is required */
#define GLEW_ERROR_GLX_VERSION_11_ONLY 3  /*  Specifies that a minimum of GLX 1.2 is required*/

/* Global Variables*/

/* VAO and VBO ids*/

// declares object, with types of objects.
static enum object { FIELD, CUBE, SPHERE, SKY }; // VAO ids.
static enum buffer { FIELD_VERTICES, CUBE_VERTICES, SPHERE_VERTICES, SPHERE_INDICES, SKY_INDICES};

/* Sphere Vertices, Normals, Triangle indices */
static VertexWtihNormal* sphereVerticesNor = NULL;
static unsigned int* sphereIndices = NULL;
static Sphere Sphere1;

// Specifies a field to render  And its coordinates 
static Vertex fieldVertices[] =
{
	{glm::vec4(100.0, 0.0, 100.0, 1.0), glm::vec2(8.0, 0.0)},
	{glm::vec4(100.0, 0.0, -100.0, 1.0), glm::vec2(8.0, 8.0)},
	{glm::vec4(-100.0, 0.0, 100.0, 1.0), glm::vec2(0.0, 0.0)},
	{glm::vec4(-100.0, 0.0, -100.0, 1.0), glm::vec2(0.0, 8.0)}
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

/* Sphere Z position Coordinate */

static float SphereZ = 0;

/* CAMERA */
static float CamPosX = 0.0;
static float CamPosY = 0.0;


int size = 4.0f;
// TRIANGLE Verticiess Part of CUBE 
static const float vertices[] = {
	-size,-size,-size, // triangle 1 : begin
	-size,-size, size,
	-size, size, size, // triangle 1 : end
	size, size,-size, // triangle 2 : begin
	-size,-size,-size,
	-size, size,-size, // triangle 2 : end
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

//Program Location " Used to Set Shaders and Send Data for them.,
unsigned int programId, vertexShaderId, fragmentShaderId, modelViewMatLoc, projMatLoc, objectLoc;


/* VBO (BUFFER) VAO and textures */
unsigned int buffer[3];
unsigned int vao[3];
unsigned int texture[2];



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

	// Binding Field 
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
	glEnableVertexAttribArray(0);// Enables or disables a generic vertex attribute array Values of 1, 0  True or false.
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(fieldVertices[0]), (void*)(sizeof(fieldVertices[0].coords)));
	glEnableVertexAttribArray(1); // Cast to 1 === True 

	/* Sphere Vertex Data */
	int verCount, triCount;
	sphereVerticesNor = Sphere1.GetVerData(verCount);
	sphereIndices = Sphere1.GetTriData(triCount);

	/* CUBE */
	glBindVertexArray(vao[CUBE]);
	glBindBuffer(GL_ARRAY_BUFFER, buffer[CUBE_VERTICES]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	/* Sphere Binding */
	glBindVertexArray(vao[SPHERE]);
	glBindBuffer(GL_ARRAY_BUFFER, buffer[SPHERE_VERTICES]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexWtihNormal) * verCount, sphereVerticesNor, GL_STATIC_DRAW);  ///please note the change
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[SPHERE_INDICES]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * triCount, sphereIndices, GL_STATIC_DRAW); ///please note the change
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(sphereVerticesNor[0]), 0);  //layout(location=4) in vec4 fieldCoords;
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(sphereVerticesNor[0]), (GLvoid*)sizeof(sphereVerticesNor[0].normals));
	glEnableVertexAttribArray(3);



	////////////////////////////////////////////////////////////////////
	// Obtain projection matrix uniform location and set value.
	projMatLoc = glGetUniformLocation(programId, "projMat"); //uniform mat4 projMat;
	projMat = glm::frustum(-5.0, 5.0, -5.0, 5.0, 5.0, 100.0);
	// < https://glm.g-truc.net/0.9.4/api/a00151.html > Glm Lib Reference
	// Dictates frustum Parameters, LEft, Right, Bottom, TOp, near, Far
	
	glUniformMatrix4fv(projMatLoc, 1, GL_FALSE, glm::value_ptr(projMat)); //< https://www.khronos.org/registry/OpenGL-Refpages/es2.0/xhtml/glUniform.xml > 

	// Obtain modelview matrix uniform and object uniform locations.
	modelViewMatLoc = glGetUniformLocation(programId, "modelViewMat"); // ModelViewMatrix
	objectLoc = glGetUniformLocation(programId, "object");             // Uniform Object
}

/* Scene Drawables Routine*/
void drawScene(void)
{
	/* Indicates which Values are to be Cleared  " Takes one/ Several Values" */
	// Clear Colour Buffers at the start of the Frame 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// Calculate and update modelview matrix.
	modelViewMat = glm::mat4(1.0);
	modelViewMat = glm::lookAt(glm::vec3(0.0, 10.0, 15.0), glm::vec3(0.0, 10.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
	glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));


	/* FIELD Bind And Shader intilization for colour*/
	glUniform1ui(objectLoc, FIELD);  //if (object == FIELD)
	glBindVertexArray(vao[FIELD]);         // Bind
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4); // Render Array of triangles 


	/* SPHERE */
	int triCount;
	sphereIndices = Sphere1.GetTriData(triCount);

	/* Sphere Animation */

	modelViewMat = mat4(1.0);
	modelViewMat = lookAt(vec3(0.0, 10.0, 15.0), vec3(0.0 + CamPosX, 10.0 + CamPosY, 0.0), vec3(0.0, 1.0, 0.0)); // Apply modelview
	modelViewMat = translate(modelViewMat, Sphere1.GetPosition()); // Sets Sphere's Position and modifies modelview matrix
	glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat)); // Send to shader 
	
	/* SPHERE Binding */
	//glUniform1ui(objectLoc, SPHERE);  //if (object == SPHERE)
	//glBindVertexArray(vao[SPHERE]);
	//glDrawElements(GL_TRIANGLE_STRIP, triCount, GL_UNSIGNED_INT, sphereIndices);


	/* CUBE DRaws TRIANGLE ATM */
	glUniform1ui(objectLoc, CUBE); // If object == CUBE
	glBindVertexArray(vao[CUBE]);
	glDrawArrays(GL_TRIANGLES, 0, 12 * 3);


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
	//glutIdleFunc(animation);
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