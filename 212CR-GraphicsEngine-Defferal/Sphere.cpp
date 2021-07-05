#include "Sphere.h"
#include <fstream>
#include <GL/glew.h>
#include <GL/freeglut.h>

// Spheres information  * Stacks slices and the Radius
Sphere::Sphere()
{
	stacks = 10; //number of segments
	slices = 10; //number of segments
	radius = 6.0f;
	position = vec3(0);

	VAO = VBO = 0;

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

void Sphere::Setup()
{
	int verCount = 121;
	int triCount = 660;
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexWtihNormal) * verCount, sphereVerticesNor, GL_STATIC_DRAW);  ///please note the change
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * triCount, sphereIndices, GL_STATIC_DRAW); ///please note the change
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(sphereVerticesNor[0]), 0);  //layout(location=4) in vec4 fieldCoords;
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(sphereVerticesNor[0]), (GLvoid*)sizeof(sphereVerticesNor[0].normals));
	glEnableVertexAttribArray(3);
}

void Sphere::updateModelMatrix(unsigned int modelViewMatLoc, float CamPosX, float scale)
{
	// CamPosx Keeps the Ball on the same Axis when Rotation is apply by Arrow Keys to left/right.
	ModelMatrix = mat4(1.0);
	ModelMatrix = lookAt(vec3(0.0, 10.0, 15.0), vec3(0.0 + CamPosX, 10.0, 0.0), vec3(0.0, 1.0, 0.0));
	ModelMatrix = glm::scale(ModelMatrix, glm::vec3(scale, scale, scale)); // Apply Correct Scaling Of sphere
	ModelMatrix = glm::translate(ModelMatrix, glm::vec3(position.x, position.y, position.z)); //apply Sphere Position
	glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(ModelMatrix));  //send modelview matrix to the shader
}

void Sphere::Draw()
{
	int triCount = 660;
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLE_STRIP, triCount, GL_UNSIGNED_INT, sphereIndices);
}

void Sphere::Update(float deltaTime, glm::vec3 offset)
{
	//collider->Update(deltaTime, position, offset);
}

void Sphere::SetIDs(unsigned int vao, unsigned int vbo, unsigned int ibo)
{
	VAO = vao;
	VBO = vbo;
	IBO = ibo;
}

// Take in position and Set new To Position 
void Sphere::SetPosition(vec3 newPos)
{
	position = newPos;
}

vec3 Sphere::GetPosition(void)
{
	return position;
}