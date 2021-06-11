/* Libraries */
#include "readshaders.h"

#include <cstdlib>
#include <iostream> // Output
#include <fstream>// Read Write on file.

/* See if It requires the use of MacOs or Windows.*/
#ifdef __APPLE__ 
#  include <GL/glew.h>
#  include <GL/freeglut.h>
#  include <OpenGL/glext.h>
#else
#  include <GL/glew.h>
#  include <GL/freeglut.h>
#pragma comment(lib, "glew32.lib") 
#endif


/* Read Shader Files and return the content */
char* readTextFile(const char* aTextFile)
{
	FILE* filePointer;
	fopen_s(&filePointer, aTextFile, "rb");
	char* content = NULL;
	long numVal = 0;

	fseek(filePointer, 0L, SEEK_END);
	numVal = ftell(filePointer);
	fseek(filePointer, 0L, SEEK_SET);
	content = (char*)malloc((numVal + 1) * sizeof(char));
	fread(content, 1, numVal, filePointer);
	content[numVal] = '\0';
	fclose(filePointer);
	return content;
}

// Function to initialize shaders.
// Get the Type of shader and the file of the shader
// Depending on the type of shader, create/Load a shader accordingly. 
int setShader(const char* shaderType, const char* shaderFile)
{
	int shaderId;
	char* shader = readTextFile(shaderFile);

	if (shaderType == "vertex") shaderId = glCreateShader(GL_VERTEX_SHADER);
	if (shaderType == "tessControl") shaderId = glCreateShader(GL_TESS_CONTROL_SHADER);
	if (shaderType == "tessEvaluation") shaderId = glCreateShader(GL_TESS_EVALUATION_SHADER);
	if (shaderType == "geometry") shaderId = glCreateShader(GL_GEOMETRY_SHADER);
	if (shaderType == "fragment") shaderId = glCreateShader(GL_FRAGMENT_SHADER);

	// Shader, Count- Number of elements in a string or lenght of array
	// String Points to string Containing the requires source code in order to load shader for it.
	// Length- Which is set to NULL and specifies an array of Strings lengths.
	glShaderSource(shaderId, 1, (const char**)&shader, NULL);
	// Specifies which shader has to be compiled.
	glCompileShader(shaderId);

	return shaderId;
}