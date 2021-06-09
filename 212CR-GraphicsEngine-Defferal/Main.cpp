#include <iostream> // Output 
#include <fstream>

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

 // Make a square  4 corners of square
float vertices[] =
{
	20.0, 20.0, 0.0,
	80.0, 20.0, 0.0,
	20.0, 80.0, 0.0,
	80.0, 80.0, 0.0
};
// Blood Red 
float colours[] =
{
	4.0, 0.0, 0.0,
	5.0, 0.0, 0.0,
	7.0, 0.0, 0.0,
	8.0, 0.0, 0.0
};

/* Triangle Information */
float vertices1[] =
{
	30.0, 30.0, 1.0,
	70.0, 30.0, 1.0,
	30.0, 70.0, 1.0

};
// White from Coroner Black
float colours1[] =
{
	5.0, 5.0, 5.0,
	5.0, 5.0, 5.0,
	5.0, 5.0, 5.0
};


unsigned int buffer[2]; // AMount of object currently nessecary for the buffer.
unsigned int vao[2]; // Will hold ( Square, Triangle )  Array of VAO IDS.  | Requires Binding |


/* PROJECT's SETUP */
void setup(void)
{
	// "Red, Green, Blue, Alpha " Specifies the values after colour Buffers are cleared, Default is 0"
	glClearColor(1.0, 1.0, 1.0, 0.0); // Reset Colours 

	/* VBO */
	std::cout << " Generating VAO and Buffers" << std::endl;

	// Generate 
	glGenVertexArrays(2, vao); // VAO ids.
	glGenBuffers(2, buffer); //buffer ids.

	// Bind First VAO and Buffer
	std::cout << " Binding Object one" << std::endl;
	glBindVertexArray(vao[0]); // Vao Buffer
	glBindBuffer(GL_ARRAY_BUFFER, buffer[0]); // Bind Vertex buffer

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(colours), NULL, GL_STATIC_DRAW); // Reserver Space
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Part buffer for verticies
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(colours), colours); // Copy vertex colour data into the second half of vertex buffer.
	
	glVertexPointer(3, GL_FLOAT, 0, 0); // Specifies  vertex and colour pointerss, to start respective data
	glColorPointer(3, GL_FLOAT, 0, (GLvoid*)(sizeof(vertices))); // Specify the amount of thing that make up vertex colour and starting position.

	// Tell OPENGL that 2 Capabilities are used ( Vertex Array and Colour Array
	glEnableClientState(GL_VERTEX_ARRAY); // Coordinates
	glEnableClientState(GL_COLOR_ARRAY); // Colour.


   // Bind Second VAO and Buffer
	std::cout << " Binding Object two" << std::endl;
	glBindVertexArray(vao[1]); // Increase to second object
	glBindBuffer(GL_ARRAY_BUFFER, buffer[1]);  // Increase to second object

	// Get Vertices 1 and Colours1
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1) + sizeof(colours1), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices1), vertices1);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices1), sizeof(colours1), colours1);


	glVertexPointer(3, GL_FLOAT, 0, 0);
	glColorPointer(3, GL_FLOAT, 0, (GLvoid*)(sizeof(vertices)));

	// Tell OPENGL that 2 Capabilities are used ( Vertex Array and Colour Array
	glEnableClientState(GL_VERTEX_ARRAY); // Coordinates
	glEnableClientState(GL_COLOR_ARRAY); // Colour.
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
	case 27:
		std::cout << " Game has been terminated By Esc" << std::endl;
		exit(0);
		break;
	default:
		break;
	}
}
/* Keyboard Input for Non - ASCII keys*/
void specialKeyInput(int key, int x, int y)
{

}



// Main routine.
int main(int argc, char** argv)
{
	glutInit(&argc, argv);

	glutInitContextVersion(4, 3);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	//glutInitWindowSize(500, 500); // original Size of the Window that the USer Sees
	glutInitWindowSize(1080, 920);
	glutInitWindowPosition(500, 50); // Specifies the location of the Window
	glutCreateWindow("Graphics Engine Alpha v0.1");
	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);
	//glutIdleFunc(animate);   // ANimation Proccessing
	glutKeyboardFunc(keyInput); // Process ACII keys

	glewExperimental = GL_TRUE;
	glewInit();

	setup(); // Intitalize setup proceedure 

	glutMainLoop();
}