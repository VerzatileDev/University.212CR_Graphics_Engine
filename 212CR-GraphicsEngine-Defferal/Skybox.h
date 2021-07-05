#pragma once
#include <iostream>
#include <fstream>

#include <GL/glew.h>
#include <GL/freeglut.h>
#pragma comment(lib, "glew32.lib") 

/* INCLUDED LIB Soil. */
#include <filesystem>
#include "soil/SOIL.h"

/* GLM LIBRARIES */
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Skybox
{
private:
	unsigned int skyboxVAO, skyboxVBO; // VAO- VBO buffers
public:
	Skybox();
	~Skybox(); // Destructor 

	void InitialiseSkybox(unsigned int vao, unsigned int vbo); // Set Skybox Data Information Location/ Get and Activate Shader Layout  ( At Vertex Shader )
	void InitialiseCubeMap(unsigned int programId, unsigned int textureID); // Get Information of Pictures to be used for as The Skybox  ( Textures )

	void Draw(unsigned int programId); // Draws Skybox. 
	void SetViewMatrix(unsigned int modelViewMatLoc, glm::mat4 modelViewMat); // Initialize what we see at Our Sceneview.
};

