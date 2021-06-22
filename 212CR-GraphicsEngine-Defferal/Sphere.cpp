#include "Sphere.h"
#include <fstream>
#include <GL/glew.h>
#include <GL/freeglut.h>

// Spheres information  * Stacks slices and the Radius
Sphere::Sphere()
{
	stacks = 10;
	slices = 10;
	radius = 6.0f;

	// Sphere Location
	Position = vec3(0);

	/* Set the Amount of Verticies by (stacks+1)*(slices +1)*/
	sphereVerticesNor = (VertexWtihNormal*)malloc(sizeof(VertexWtihNormal) * 121); //total number of vertices = (stacks+1)*(slices +1)
	sphereIndices = (unsigned int*)malloc(sizeof(unsigned int) * 660);
	CreateSpherewithNormal();
}

// Sphere Destructor when Out of Scope or bounds
Sphere::~Sphere()
{
	free(sphereVerticesNor);
	free(sphereIndices);
}

void Sphere::CreateSpherewithNormal(void)
{
	
	int count = 0; // Index Position for VerticesNormals

	for (int i = 0; i <= stacks; ++i) {

		GLfloat V = i / (float)stacks;
		GLfloat phi = V * glm::pi <float>();

		// Loop Through Slices
		for (int j = 0; j <= slices; ++j) {

			GLfloat U = j / (float)slices;
			GLfloat theta = U * (glm::pi <float>() * 2);

			// Calculate Vertex Positions
			GLfloat x = cosf(theta) * sinf(phi);
			GLfloat y = cosf(phi);
			GLfloat z = sinf(theta) * sinf(phi);

			sphereVerticesNor[count].coords = vec4(x * radius, y * radius + 6.0, z * radius, 1.0);
			sphereVerticesNor[count].normals = vec3(x, y, z);

			count++;
		}
	}

	count = 0; // Index Position for Indicies 

	// Calculate Index Positions
	for (int i = 0; i < slices * stacks + slices; ++i) {

		sphereIndices[count] = i;
		count++;
		sphereIndices[count] = i + slices + 1;
		count++;
		sphereIndices[count] = i + slices;
		count++;

		sphereIndices[count] = i + slices + 1;
		count++;
		sphereIndices[count] = i;
		count++;
		sphereIndices[count] = i + 1;
		count++;
	}

	count = 0;
}

// Take Int of the number of Verticies 
VertexWtihNormal* Sphere::GetVerData(int& verticiesNum)
{
	verticiesNum = 121;
	return sphereVerticesNor;
}

unsigned int* Sphere::GetTriData(int& triNum)
{
	triNum = 660;
	return sphereIndices;
}

// Take in position and Set new To Position 
void Sphere::SetPosition(vec3 newPos)
{
	Position = newPos;
}
// Retrives Position of the Sphere at that moment.
vec3 Sphere::GetPosition(void)
{
	return Position;
}