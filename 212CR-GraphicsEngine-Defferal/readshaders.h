#ifndef SHADER_H // Checks to see If Token is already defined Previously or Included in file.
#define SHADER_H // Declares Header once if  #ifndef is true.

// fragmentShaderId = setShader("fragment", "fragmentShader.glsl"); // Example In Create Sphere
int setShader(const char* shaderType, const char* shaderFile);
char* readTextFile(const char* aTextFile); // Added to Read Information Data of Skybox. 

#endif