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


/* <https://codeyarns.com/tech/2015-09-14-how-to-check-error-in-glew.html>*/
// Error Checking For GLEW, if glewInit() does not return Glew_OK
#define GLEW_OK 0
#define GLEW_NO_ERROR 0
#define GLEW_ERROR_NO_GL_VERSION 1  /* GL version could not be found*/
#define GLEW_ERROR_GL_VERSION_10_ONLY 2  /* Specifies that a minimum of OpenGL 1.1 is required */
#define GLEW_ERROR_GLX_VERSION_11_ONLY 3  /*  Specifies that a minimum of GLX 1.2 is required*/

/* Global Variables*/

/* VAO and VBO ids*/
// Check for  warning C4091: Later,  
// declares object, with types of objects.
static enum object { FIELD }; // VAO ids.
static enum buffer { FIELD_VERTICES}; // VBO ids.

// Specifies a field to render  And its coordinates 
static Vertex fieldVertices[] =
{
	{glm::vec4(100.0, 0.0, 100.0, 1.0), glm::vec2(1.0, 0.0)},
	{glm::vec4(100.0, 0.0, -100.0, 1.0), glm::vec2(1.0, 8.0)},
	{glm::vec4(-100.0, 0.0, 100.0, 1.0), glm::vec2(1.0, 0.0)},
	{glm::vec4(-100.0, 0.0, -100.0, 1.0), glm::vec2(1.0, 8.0)}
};


// Used in Definition of the camera. With ModelViewMat and ProjMat
static glm::mat4 modelViewMat(1.0f);
static glm::mat4 projMat(1.0f);

// Normalization of Camera
static glm::mat3 normalMat = glm::mat3(1.0);

//Program Location " Used to Set Shaders and Send Data for them.,
unsigned int programId, vertexShaderId, fragmentShaderId, modelViewMatLoc, projMatLoc, objectLoc;

/* VBO (BUFFER) VAO and textures */
unsigned int buffer[3]; // objects nessecary for buffer.
unsigned int vao[3]; //  | Requires Binding |
unsigned int texture[2]; // Textures present




/* PROJECT's SETUP */
void setup(void)
{
	// "Red, Green, Blue, Alpha " Specifies the values after colour Buffers are cleared, Default is 0"
	glClearColor(1.0, 1.0, 1.0, 0.0); // Reset Colours 

	
	// If enabled, Clear buffer with GL_DEPTH_BUFFER_BIT
   // stores fragments their z-values in the depth buffer if they 
   // passed the depth test and discards fragments if they failed the depth test accordingly.
	glEnable(GL_DEPTH_TEST);

	// < https://stackoverflow.com/questions/28137027/why-do-i-need-glcleargl-depth-buffer-bit >
	glDepthFunc(GL_LESS);

	// Set from Shader.h  Create Shader Program Executable
	vertexShaderId = setShader("vertex", "vertexShader.glsl");
	fragmentShaderId = setShader("fragment", "fragmentShader.glsl");

	/* Part of readShaders */
	programId = glCreateProgram();
	glAttachShader(programId, vertexShaderId);
	glAttachShader(programId, fragmentShaderId);
	glLinkProgram(programId);
	glUseProgram(programId);


	/* VBO */
	std::cout << " Generating VAO and Buffers" << std::endl;

	// Generate 
	glGenVertexArrays(3, vao); // VAO ids.
	glGenBuffers(3, buffer); // VBO (BUffer) ids.

	// Bind First VAO and Buffer
	std::cout << " Binding Object one" << std::endl;
	glBindVertexArray(vao[FIELD]); // Vao Buffer
	glBindBuffer(GL_ARRAY_BUFFER, buffer[FIELD_VERTICES]); // Bind Vertex buffer

	// Reserve Space
	glBufferData(GL_ARRAY_BUFFER, sizeof(fieldVertices), fieldVertices, GL_STATIC_DRAW);

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


	
	// Obatains Projection MAtrix Uniform  * Location and Set Value *
	projMatLoc = glGetUniformLocation(programId, "projMat");

	// < https://glm.g-truc.net/0.9.4/api/a00151.html > Glm Lib Reference
    // Dictates frustum Parameters, LEft, Right, Bottom, TOp, near, Far

	projMat = glm::frustum(-5.0, 5.0, -5.0, 5.0, 5.0, 100.0);

	//< https://www.khronos.org/registry/OpenGL-Refpages/es2.0/xhtml/glUniform.xml > 
	glUniformMatrix4fv(projMatLoc, 1, GL_FALSE, glm::value_ptr(projMat));

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


	// Debug Commenting.
	std::cout << " Intializing Field" << std::endl;
	glUniform1ui(objectLoc, FIELD);  //if (object == FIELD)
	glBindVertexArray(vao[FIELD]);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);


	glutSwapBuffers(); // Change buffers when the current window is double buffered.
}



/* ANIMATION ROUTINE*/
void animation() {

	/* update state variables */
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