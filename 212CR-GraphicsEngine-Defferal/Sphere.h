#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include "vertex.h"
#include "Object.h"

using namespace glm;

/* See http://donw.io/post/sphere-indexing/ For Better sphere */
class Sphere : public Object
{
private:

	VertexWtihNormal* sphereVerticesNor;  //Sphere Normal Data
	unsigned int* sphereIndices;          //Sphere triangle indices    

	 //nunber of segments
	int stacks;
	int slices; 
	float radius;

	unsigned int VAO;
	unsigned int VBO;
	unsigned int IBO; //for triangle indices buffer

	mat4 ModelMatrix;

	void CreateSpherewithNormal();

public:
	Sphere();
	~Sphere(); // Destructor Called When Out of Scope

	void SetPosition(vec3 newPos);
	vec3 GetPosition(void);
	void SetIDs(unsigned int, unsigned int, unsigned int);
	void updateModelMatrix(unsigned int, float);

	void Setup();
	void Draw();
	void Update(float, glm::vec3 offset);
};

