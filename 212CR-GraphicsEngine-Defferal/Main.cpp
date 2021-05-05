#include <iostream>

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

double xx;

// Drawing routine.
void drawScene(void)
{
	/*Indicates which Values are to be Cleared  " Takes one or Several Values"*/
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	/* Defines the Color of the Element*/
	glColor3f(0.0, 0.0, 0.0);
	glLoadIdentity();

	// Modeling transformations.
	// Position the object On x,y,z Cordinates. aka "Starting Position."
	glTranslatef(xx, 0.0, -5.0);
	glTranslatef(0.0, 0.0, -5.0); // + Add on to the First glTranslatef"
	
	// Rotation of Angle within degrees of the vector (x,y,z)
	glRotatef(25, 1, 0, 0); //Higher relative to the Objects View.
	glRotatef(45, 0, 1, 0);

	// Body between the wheels.
	glPushMatrix(); //" To save or restore the Unrotated Coordinate System "With glPopMatrix."                 
	glScalef(2, .5, 1); // Change Diameters here for the Body Between wheels.
	glutSolidCube(.5); // Size of the CUBE   " Mid Body "
	glPopMatrix(); // Restores Unrotated Objects.
	glTranslatef(0, 0, .25); // Location Of the Body.

	/* Car Wheels */
	glPushMatrix();
	glTranslatef(-0.4, -0.2, 0);          // First Row Wheel Location
	glutSolidTorus(0.05, .1, 8, 8);       // wheel
	glTranslatef(0.8, 0, 0);              // Location

	/*Gldouble InnerRadius,-. OuterRadius, Glint nside, GLin Rings)*/ // About glutSolidTorus
	glutSolidTorus(0.05, 0.1, 8, 8);       // Second Wheel
	glPopMatrix();
	glTranslatef(0, 0, -0.5);

	/* Row 2 Wheels */
	glPushMatrix();
	glTranslatef(-0.4, -0.2, 0);
	glutSolidTorus(0.05, 0.1, 8, 8);       // wheel 1

	glTranslatef(0.8, 0, 0);
	glutSolidTorus(0.05, 0.1, 8, 8);       // wheel 2
	glPopMatrix();


	glFlush();  // Empties Buffers, " before it waits for User Input.. that depends on the generated image.
}

// Initialization routine.
void setup(void)
{
	// "Red, Green, Blue, Alpha " Specifies the values after colour Buffers are cleared, Default is 0"
	glClearColor(1.0, 1.0, 1.0, 0.0);
}

/* ANIMATION ROUTINE*/
void animate() {

	/* update state variables */
	xx += .001;

	/* refresh screen */
	glutPostRedisplay();
}

// OpenGL window reshape routine.
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

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:
		exit(0);
		break;
	default:
		break;
	}
}

// Main routine.
int main(int argc, char** argv)
{
	glutInit(&argc, argv);

	glutInitContextVersion(4, 3);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	//glutInitWindowSize(500, 500); // original Size of the Window that the USer Sees
	glutInitWindowSize(500, 500);
	//glutInitWindowPosition(100, 100); // original  Where Window Opens on Screen,
	glutInitWindowPosition(100, 100);
	glutCreateWindow("box.cpp");
	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);
	//glutIdleFunc(animate);   // ANimation
	glutKeyboardFunc(keyInput);

	glewExperimental = GL_TRUE;
	glewInit();

	setup();
	xx = 0.0;

	glutMainLoop();
}