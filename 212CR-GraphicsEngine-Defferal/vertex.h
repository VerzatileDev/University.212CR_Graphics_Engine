#ifndef VERTEX_H // Checks to see If Token is already defined Previously or Included in file.
#define VERTEX_H // Declares Header once if  #ifndef is true.

#include <glm/glm.hpp> // MAT4

/* Structure for Combining Vertex and Colour Data*/
struct Vertex
{
	glm::vec4 coords;
	glm::vec2 texCoords;
};

/* Structure of Normalized form of vertex.*/
struct VertexWtihNormal
{
	glm::vec4 coords;
	glm::vec3 normals;
};

struct  VertexWithAll
{
	glm::vec3 position; // Object Position 
	glm::vec3 rotation; // In order to change the Angle of Spaceship
	glm::vec3 color;
	glm::vec2 textcoord;
	glm::vec3 normal;
};

#endif
