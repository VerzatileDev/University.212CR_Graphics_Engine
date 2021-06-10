// Lib used.
#include <iostream> // Output 
#include <fstream> // Reading Files
#include <glm/glm.hpp> // GLM Lib
#include <glm/gtc/type_ptr.hpp> // MAt4 Value Pointer
#include <vector> 

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


/* Global Variables*/

/* Define Vertex and Colour Data*/
struct Vertex
{
	float coords[4];
	float colours[4];
};

//  Includes colour and Vertex data Together
Vertex squareVertices[] =
{
	{ { 20.0, 20.0, 0.0, 1.0 },{ 0.0, 0.0, 0.0, 1.0 } },
	{ { 80.0, 20.0, 0.0, 1.0 },{ 0.0, 0.0, 0.0, 1.0 } },
	{ { 20.0, 80.0, 0.0, 1.0 },{ 0.0, 0.0, 0.0, 1.0 } },
	{ { 80.0, 80.0, 0.0, 1.0 },{ 0.0, 0.0, 0.0, 1.0 } }
};
Vertex triangleVertices[] =
{

	{ { 30.0, 30.0, 1.0, 1.0 },{ 1.0, 0.0, 1.0, 1.0 } },
	{ { 70.0, 30.0, 1.0, 1.0 },{ 1.0, 0.0, 1.0, 1.0 } },
	{ { 30.0, 70.0, 1.0, 1.0 },{ 1.0, 0.0, 1.0, 1.0 } }
};

// Used in Definition of the camera. With ModelViewMat and ProjMat
glm::mat4 modelViewMat(1.0f);
glm::mat4 projMat(1.0f);

//Program Location " Used to Set Shaders and Send Data for them.,
unsigned int programId, vertexShaderId, fragmentShaderId, modelViewMatLoc, projMatLoc;

/* BUFFERS and VAO */
unsigned int buffer[2]; // AMount of object currently nessecary for the buffer.
unsigned int vao[2]; // Will hold ( Square, Triangle )  Array of VAO IDS.  | Requires Binding |

////////////////////////////////////////////
/*  MOVE INTO SEPERATE FILE  Header */

/* Shader Document Loading function and Checking*/
char* readTextFile(const char* aTextFile)
{
	FILE* filePointer;
	fopen_s(&filePointer, aTextFile, "rb");
	char* content = NULL;
	long numVal = 0;

	fseek(filePointer, 0L, SEEK_END);
	numVal = ftell(filePointer);
	fseek(filePointer, 0L, SEEK_SET);
	content = (char*)malloc((numVal + 1) * sizeof(char));
	fread(content, 1, numVal, filePointer);
	content[numVal] = '\0';
	fclose(filePointer);
	return content;
}
void shaderCompileTest(GLuint shader)
{
	GLint result = GL_FALSE;
	int logLength; glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
	std::vector<GLchar> vertShaderError((logLength > 1) ? logLength : 1);
	glGetShaderInfoLog(shader, logLength, NULL, &vertShaderError[0]);
	std::cout << &vertShaderError[0] << std::endl;
}
//////////////////////////////////////////


/* PROJECT's SETUP */
void setup(void)
{
	// "Red, Green, Blue, Alpha " Specifies the values after colour Buffers are cleared, Default is 0"
	glClearColor(1.0, 1.0, 1.0, 0.0); // Reset Colours 

	// Establish shader program executable.  " Gets and Reads Fragment and Vertex Shader Files.
	char* vertexShader = readTextFile("vertexShader.glsl");
	vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShaderId, 1, (const char**)&vertexShader, NULL);
	glCompileShader(vertexShaderId);
	std::cout << "::: VERTEX SHADER :::" << std::endl;
	shaderCompileTest(vertexShaderId);

	char* fragmentShader = readTextFile("fragmentShader.glsl");
	fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderId, 1, (const char**)&fragmentShader, NULL);
	glCompileShader(fragmentShaderId);
	std::cout << "::: FRAGMENT SHADER :::" << std::endl;
	shaderCompileTest(fragmentShaderId);

	programId = glCreateProgram();
	glAttachShader(programId, vertexShaderId);
	glAttachShader(programId, fragmentShaderId);
	glLinkProgram(programId);
	glUseProgram(programId);


	/* VBO */
	std::cout << " Generating VAO and Buffers" << std::endl;

	// Generate 
	glGenVertexArrays(2, vao); // VAO ids.
	glGenBuffers(2, buffer); //buffer ids.

	// Bind First VAO and Buffer
	std::cout << " Binding Object one" << std::endl;
	glBindVertexArray(vao[0]); // Vao Buffer
	glBindBuffer(GL_ARRAY_BUFFER, buffer[0]); // Bind Vertex buffer

	// Reserve Space
	glBufferData(GL_ARRAY_BUFFER, sizeof(squareVertices), squareVertices, GL_STATIC_DRAW);

	// Sets Singular Array of generic vertex Attribute Data
	// Index - Specifies vertex Attribute to be Modified,
	// Size - Number of Components per generic Vertex Attribute ( Must be 1,2,3,4) Initial = 4
	// Type - of each component in Array
	// Normalized - GL_True When Fixed Point,  False when Accessed.
	// Stride - Specofies the byte offset Between Consecutive generic Vertex Attributes (Default 0)
	// Pointer - Specofies offset of first component. (Initial Value 0 ) GL_Array_buffer Target.
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(squareVertices[0]), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(squareVertices[0]), (GLvoid*)sizeof(squareVertices[0].coords));
	glEnableVertexAttribArray(1);


   // Bind Second VAO and Buffer
	std::cout << " Binding Object two" << std::endl;
	glBindVertexArray(vao[1]); // Increase with each object 
	glBindBuffer(GL_ARRAY_BUFFER, buffer[1]);  // Increase with each object 

	// Triangle
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(triangleVertices[0]), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(triangleVertices[0]), (GLvoid*)sizeof(triangleVertices[0].coords));
	glEnableVertexAttribArray(1);


	// Projection MAtrix Uniform  * Location and Set Value *
	glm::mat4 projMat =
	{
		0.02, 0.0,  0.0, -1.0,
		0.0,  0.02, 0.0, -1.0,
		0.0,  0.0, -1.0,  0.0,
		0.0,  0.0,  0.0,  1.0
	};
	/////////////////////////////////////////
	// Obtain ModelView matrix uniform Location and set value.
	projMatLoc = glGetUniformLocation(programId, "projMat");
	glUniformMatrix4fv(projMatLoc, 1, GL_TRUE, glm::value_ptr(projMat));
	glm::mat4 modelViewMat(1.0f);
	modelViewMatLoc = glGetUniformLocation(programId, "modelViewMat");
	glUniformMatrix4fv(modelViewMatLoc, 1, GL_TRUE, glm::value_ptr(modelViewMat));
	////////////////////////////////////////
}

/* Scene Drawables Routine*/
void drawScene(void)
{
	/*Indicates which Values are to be Cleared  " Takes one or Several Values"*/
	glClear(GL_COLOR_BUFFER_BIT);
	/* Defines the Color of the Element*/

	// Bind first VAO *object one*
	glBindVertexArray(vao[0]);

	// Debug Commenting.
	std::cout << " Drawing Strip Gl Triangle for VBO" << std::endl;

	//Draw Gl/Triangle strip  using 4 vertices in the VBO
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	// Bind Second VAO  *object two*
	glBindVertexArray(vao[1]);
	glDrawArrays(GL_TRIANGLES, 0, 3);


	// Force Execution of Gl commands that are in limited in size or extent (Finite) time
	glFlush();  // Empties Buffers, " before it waits for User Input.. that depends on the generated image.
}



/* ANIMATION ROUTINE*/
void animate() {

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

	/* set up depth-buffering */
	//glEnable(GL_DEPTH_TEST);
	/* turn on default lighting */
	//glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT0);
	//gluPerspective(40, 1, 4, 20);


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
	glutInitContextVersion(4, 3);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowSize(1080, 920); // original Size 500, 500 Display Window size
	glutInitWindowPosition(500, 50); // Specifies the location of the Window 500, 50 Centre Of screen.
	glutCreateWindow("Graphics Engine Alpha v0.1");
	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);
	//glutIdleFunc(animate);   // Animation Proccessing
	glutKeyboardFunc(keyInput); // Process ACII keys
	glutSpecialFunc(specialKeyInput); // Process Non ACII Keys.

	glewExperimental = GL_TRUE;
	glewInit(); // Returns Error Value of a type GLenum " Checks For Error " <https://codeyarns.com/tech/2015-09-14-how-to-check-error-in-glew.html>

	setup(); // Intitalize setup proceedure 

	glutMainLoop(); // loop Until it is asked To stop Either by Esc or close application.
}