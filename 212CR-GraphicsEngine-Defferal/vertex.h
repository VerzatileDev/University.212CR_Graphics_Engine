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

#endif
