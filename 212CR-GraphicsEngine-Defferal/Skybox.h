#pragma once
#include <iostream>
#include <fstream>

#include <GL/glew.h>
#include <GL/freeglut.h>
#pragma comment(lib, "glew32.lib") 

#include <filesystem>
#include "soil/SOIL.h"

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

	void InitialiseSkybox(unsigned int vao, unsigned int vbo);
	void InitialiseCubeMap(unsigned int programId, unsigned int textureID);

	void Draw(unsigned int programId);
	void SetViewMatrix(unsigned int modelViewMatLoc, glm::mat4 modelViewMat);
};

