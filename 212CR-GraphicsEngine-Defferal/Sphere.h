#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include "vertex.h"

using namespace glm;

/* See http://donw.io/post/sphere-indexing/ For Better sphere */
class Sphere
{
private:
	vec3 Position;
	VertexWtihNormal* sphereVerticesNor;  //Sphere Normal Data
	unsigned int* sphereIndices;          //Sphere triangle indices    

	 //nunber of segments
	int stacks;
	int slices; 
	float radius;

	void CreateSpherewithNormal();

public:
	Sphere();
	~Sphere(); // Destructor Called When Out of Scope

	void SetPosition(vec3 newPos);
	vec3 GetPosition(void);
	VertexWtihNormal* GetVerData(int&);
	unsigned int* GetTriData(int&);
};

