#include "impModel.h"
#include "LoadObject.h"
#include <fstream>
#include <GL/glew.h>
#include <GL/freeglut.h>

// Initialize Data.
ImpModel::ImpModel(const char* name)
{
	position = glm::vec3(0);
	VAO = VBO = 0;
	VerticesData = NULL;
	NumVert = 0;
	CreateObject(name);
}

ImpModel::~ImpModel()
{
	free(VerticesData);
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

void ImpModel::SetPosition(glm::vec3 newPos)
{
	position = newPos;
}

glm::vec3 ImpModel::GetPosition(void)
{
	return position;
}

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

	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(VertexWithAll), (GLvoid*)offsetof(VertexWithAll, position));
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(VertexWithAll), (GLvoid*)offsetof(VertexWithAll, normal));
	glEnableVertexAttribArray(5);

	glVertexAttribPointer(6, 2, GL_FLOAT, GL_FALSE, sizeof(VertexWithAll), (GLvoid*)offsetof(VertexWithAll, textcoord));
	glEnableVertexAttribArray(6);

}

void ImpModel::updateModelMatrix(unsigned int modelViewMatLoc, float CamPosX, float scale, float ZPos)
{
	ModelMatrix = glm::mat4(1.0);
	ModelMatrix = glm::lookAt(glm::vec3(0.0, 10.0, 15.0), glm::vec3(0.0 + CamPosX, 10.0, 0.0), glm::vec3(0.0, 1.0, 0.0)); //camera matrix, apply first
	ModelMatrix = glm::scale(ModelMatrix, glm::vec3(scale, scale, scale));  //scale down the model
	ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.0f, 0.0f, ZPos));
	glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(ModelMatrix));  //send modelview matrix to the shader
}

void ImpModel::Draw()
{
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, NumVert);
}

void ImpModel::Update(float deltaTime, glm::vec3 offset)
{
	//collider->Update(deltaTime, position, offset);
}