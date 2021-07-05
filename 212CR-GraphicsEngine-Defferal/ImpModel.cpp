#include "impModel.h"
#include "LoadObject.h"
#include <fstream>
#include <GL/glew.h>
#include <GL/freeglut.h>

// Initialize Data.
ImpModel::ImpModel(const char* name)
{
	position = glm::vec3(0);
	rotation = glm::vec3(0);
	VAO = VBO = 0;
	VerticesData = NULL;
	NumVert = 0;
	CreateObject(name);
}

ImpModel::~ImpModel()
{
	free(VerticesData); // If object Out of Bound or Scope, free Resource
}

void ImpModel::CreateObject(const char* name)
{
	std::vector<VertexWithAll> mesh = loadOBJ(name); //Import OBJ file data into a vector array
	NumVert = mesh.size();

	this->VerticesData = new VertexWithAll[NumVert];
	for (size_t i = 0; i < NumVert; i++)
	{
		this->VerticesData[i] = mesh[i];
	}
}

// Check For position on x,y,z
void ImpModel::SetPosition(glm::vec3 newPos, glm::vec3 newrot)
{
	position = newPos;
	rotation = newrot;
}

// Recheck Position
glm::vec3 ImpModel::GetPosition(void)
{
	return position;
}
// Set Buffer IDs To be Used on Shader through Main declaration see Buffer/ Object at the Global Area
void ImpModel::SetIDs(unsigned int vao, unsigned int vbo, unsigned int ibo)
{
	VAO = vao;
	VBO = vbo;
	IBO = ibo;
}

void ImpModel::Setup()
{
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexWithAll) * NumVert, VerticesData, GL_STATIC_DRAW);  ///please note the change

	// GlVertex Attribute Pointer 
	// index - Specifies the index of the generic vertex attribute to be enabled or disabled.
	// Size - Number of Components per generic Vertex Attribute ( Must be 1,2,3,4) Initial = 4
	// Type - of each component in Array
	// Normalized - GL_True When Fixed Point,  False when Accessed.
	// Stride - Specofies the byte offset Between Consecutive generic Vertex Attributes (Default 0)
	// Pointer - Specofies offset of first component. (Initial Value 0 ) GL_Array_buffer Target.

	// Activate Layout Object Coordinates At Vertex Shader.
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(VertexWithAll), (GLvoid*)offsetof(VertexWithAll, position));
	glEnableVertexAttribArray(4);
	// Activate Layout objNormals
	glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(VertexWithAll), (GLvoid*)offsetof(VertexWithAll, normal));
	glEnableVertexAttribArray(5);
	// Activate Layout objTexCoords
	glVertexAttribPointer(6, 2, GL_FLOAT, GL_FALSE, sizeof(VertexWithAll), (GLvoid*)offsetof(VertexWithAll, textcoord));
	glEnableVertexAttribArray(6);

}

void ImpModel::updateModelMatrix(unsigned int modelViewMatLoc, float CamPosX, float scale, float ZPos, float yPos)
{
	ModelMatrix = glm::mat4(1.0);
	ModelMatrix = glm::lookAt(glm::vec3(0.0, 10.0, 15), glm::vec3(0.0 + CamPosX, 0.0 + yPos, 0.0+ ZPos), glm::vec3(rotation.x, rotation.y, rotation.z));// To keep It in the Position Of the Scene.
	ModelMatrix = glm::scale(ModelMatrix, glm::vec3(scale, scale, scale));  //scale down the model
	ModelMatrix = glm::translate(ModelMatrix, glm::vec3(position.x, position.y, position.z)); // Object Position Updates.
	ModelMatrix = glm::translate(ModelMatrix, GetPosition()); // Recheck Theoretically not Nessecary.
	glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(ModelMatrix));  //send modelview matrix to the shader
}

void ImpModel::Draw()
{
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, NumVert);
}