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

	// Element Data Buffers
	unsigned int VAO;
	unsigned int VBO;
	unsigned int IBO;

	int NumVert; // Model Total Vertices
	glm::mat4 ModelMatrix;
	void CreateObject(const char*);
public:
	ImpModel(const char*); // Imports the object through Object.h Setting Pos, Rot From Vertex.
	~ImpModel();          // Call If element gets our of Scope/Bound of the Screen. Automatical Call or Manual.

	void SetPosition(glm::vec3 newPos, glm::vec3 newrot); // Position/ Rotation.
	glm::vec3 GetPosition(void);                         // Update Current Position

	void SetIDs(unsigned int, unsigned int, unsigned int);// Specify Buffer Ids to be used for the element.
	void updateModelMatrix(unsigned int, float, float, float, float); //ModelViewMatLoc, CamPosx, Scale, ZPos, YPos

	void Setup();
	void Draw();
};

