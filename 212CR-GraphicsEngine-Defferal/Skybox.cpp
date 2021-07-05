#include "Skybox.h"

Skybox::Skybox()
{
}


Skybox::~Skybox()
{
}

// Intilialize/ Specifie boundries of the Skybox
// Set VAO and VBO buffers
// Bind buffers and send to shader.
void Skybox::InitialiseSkybox(unsigned int vao, unsigned int vbo)
{
	float skyboxVertices[] =
	{
		-300.0f,  300.0f, -300.0f,
		-300.0f, -300.0f, -300.0f,
		 300.0f, -300.0f, -300.0f,
		 300.0f, -300.0f, -300.0f,
		 300.0f,  300.0f, -300.0f,
		-300.0f,  300.0f, -300.0f,

		-300.0f, -300.0f,  300.0f,
		-300.0f, -300.0f, -300.0f,
		-300.0f,  300.0f, -300.0f,
		-300.0f,  300.0f, -300.0f,
		-300.0f,  300.0f,  300.0f,
		-300.0f, -300.0f,  300.0f,

		 300.0f, -300.0f, -300.0f,
		 300.0f, -300.0f,  300.0f,
		 300.0f,  300.0f,  300.0f,
		 300.0f,  300.0f,  300.0f,
		 300.0f,  300.0f, -300.0f,
		 300.0f, -300.0f, -300.0f,

		-300.0f, -300.0f,  300.0f,
		-300.0f,  300.0f,  300.0f,
		 300.0f,  300.0f,  300.0f,
		 300.0f,  300.0f,  300.0f,
		 300.0f, -300.0f,  300.0f,
		-300.0f, -300.0f,  300.0f,

		-300.0f,  300.0f, -300.0f,
		 300.0f,  300.0f, -300.0f,
		 300.0f,  300.0f,  300.0f,
		 300.0f,  300.0f,  300.0f,
		-300.0f,  300.0f,  300.0f,
		-300.0f,  300.0f, -300.0f,

		-300.0f, -300.0f, -300.0f,
		-300.0f, -300.0f,  300.0f,
		 300.0f, -300.0f, -300.0f,
		 300.0f, -300.0f, -300.0f,
		-300.0f, -300.0f,  300.0f,
		 300.0f, -300.0f,  300.0f
	};
	skyboxVAO = vao;
	skyboxVBO = vbo;
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	// Allocate Shader / Activate Shader 
	glEnableVertexAttribArray(0); // Enable Layout zero on Vertext Shader.  // layout(location=0) in vec3 skyCoords;
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
}

// Take Texture Folder Picture to be used in Skybox.
// Activate Texturess, 
// Set parameters of the cubemaps
// Generate Mip map
// Send to Shader.
void Skybox::InitialiseCubeMap(unsigned int programId, unsigned int textureID)
{
	std::string skyboxTextures[] =
	{
		"Textures/SkyboxRight.jpg",
		"Textures/SkyboxLeft.jpg",
		"Textures/SkyboxTop.jpg",
		"Textures/SkyboxBottom.jpg",
		"Textures/SkyboxFront.jpg",
		"Textures/SkyboxBack.jpg"
	};

	glActiveTexture(GL_TEXTURE0 + textureID - 1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height;
	for (unsigned int i = 0; i < 6; i++)
	{
		unsigned char* data = SOIL_load_image(skyboxTextures[i].c_str(), &width, &height, 0, SOIL_LOAD_RGBA);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		SOIL_free_image_data(data);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glGenerateMipmap(GL_TEXTURE_2D);

	unsigned int skyTexLoc = glGetUniformLocation(programId, "skyboxTexture");
	glUniform1i(skyTexLoc, textureID - 1); //send texture to shader
}


void Skybox::Draw(unsigned int programId)
{
	glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
	glBindVertexArray(skyboxVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36); //Draws 36 Triangles, each triangle has 3 Points, See at SkyboxVertices  " Type, Start, End "
	glBindVertexArray(0);  // Binds skyCoords in VertexShader 
	glDepthFunc(GL_LESS); // set depth function back to default
}

void Skybox::SetViewMatrix(unsigned int modelViewMatLoc, glm::mat4 modelViewMat)
{
	glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));
}