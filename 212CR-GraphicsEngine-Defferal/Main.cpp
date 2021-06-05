#include <iostream>

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

using namespace std;

/* Global Variables*/
double xx;


// Initialization routine.
void setup(void)
{
	// "Red, Green, Blue, Alpha " Specifies the values after colour Buffers are cleared, Default is 0"
	glClearColor(1.0, 1.0, 1.0, 0.0);
}

/* Scene Drawables Routine*/
void drawScene(void)
{
	/*Indicates which Values are to be Cleared  " Takes one or Several Values"*/
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	/* Defines the Color of the Element*/
	glColor3f(0.0, 0.0, 0.0);
	glLoadIdentity();



	glFlush();  // Empties Buffers, " before it waits for User Input.. that depends on the generated image.
}

/* ANIMATION ROUTINE*/
void animate() {

	/* update state variables */
	xx += .001;

	/* refresh screen */
	glutPostRedisplay();
}

/* OpenGL window reshape routine.*/
void resize(int w, int h)
{
	glViewport(0, 0, w, h); // Specifies the part of the Window to which OpenGl, WIll draw in pixels
	// Conversts from normalized to pixels.

	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH);
	/* set up depth-buffering */
	glEnable(GL_DEPTH_TEST);

	/* turn on default lighting */
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40, 1, 4, 20);

	glMatrixMode(GL_MODELVIEW);
}


/*  KEYBOARD PROCESSING ROUTINES, ACII keys*/
void keyInput(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:
		cout << " Game has been terminated By Esc" << endl;
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
	glutCreateWindow("Brian's Graphics Engine Alpha v1.0");
	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);
	//glutIdleFunc(animate);   // ANimation of the sphere.
	glutKeyboardFunc(keyInput);

	glewExperimental = GL_TRUE;
	glewInit();

	setup();
	xx = 0.0;

	glutMainLoop();
}