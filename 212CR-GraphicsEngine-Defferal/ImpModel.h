#pragma once
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include "vertex.h"
#include "Object.h"



class ImpModel : public Object
{
private:
	VertexWithAll* VerticesData;  //Data with vertices, normal, texCoords

	unsigned int VAO;
	unsigned int VBO;
	unsigned int IBO;

	int NumVert; // Model Total Vertices
	glm::mat4 ModelMatrix;
	void CreateObject(const char*);
public:
	ImpModel(const char*);
	~ImpModel();

	void SetPosition(glm::vec3 newPos);
	glm::vec3 GetPosition(void);

	void SetIDs(unsigned int, unsigned int, unsigned int);
	void updateModelMatrix(unsigned int, float, float, float, float);

	void Setup();
	void Draw();
};

