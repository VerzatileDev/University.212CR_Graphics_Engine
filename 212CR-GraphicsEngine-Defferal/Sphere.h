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

	// Sphere Data
	int stacks;
	int slices; 
	float radius;

	/* BUFFER IDS */
	unsigned int VAO;
	unsigned int VBO;
	unsigned int IBO;

	mat4 ModelMatrix;

	void CreateSpherewithNormal();

public:
	Sphere();
	~Sphere(); // Destructor Called When Out of Scope

	void SetPosition(vec3 newPos); // Get Input of x,y,z For an initial set of position within the scene.
	vec3 GetPosition(void); // Retrives Position of the Sphere at that moment.
	void SetIDs(unsigned int, unsigned int, unsigned int); // Locate Buffer Values and allocate to Sphere
	void updateModelMatrix(unsigned int, float, float);    // Update, Location, Scale, Update Shader.

	void Setup(); // Adds buffers, getting setID and allocating VAO,VBO, IBO buffers / Initialize sphere to be used At Draw.
	void Draw();  // Draws the Sphere if Setup Is previously Called  See Information on Main.
	void Update(float, glm::vec3 offset); 
};

