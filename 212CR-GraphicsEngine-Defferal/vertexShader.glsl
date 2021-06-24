#version 430 core

/* See Documentation <https://learnopengl.com/Getting-started/Shaders> */

/* Objects definitions */
// THese have to be In order See Main.cpp |static enum object| and | static enum buffer|
#define FIELD 0
#define CUBE 1
#define SPHERE 2
#define SKY 3


// Locations of Data Sent by glVertexAttribPointer And Activated by glEnableVertexAttribArray
layout(location=0) in vec4 Coords;
layout(location=1) in vec2 TexCoords;
layout(location=2) in vec4 sphereCoords;
layout(location=3) in vec3 sphereNormals;


// Uniform Pass by glUniformMatrix4fv
uniform mat4 modelViewMat;
uniform mat4 projMat;
uniform uint object;

// Out means it is Passed Out to NExt Shader  " Fragment Shader "
out vec2 texCoordsExport;
out vec3 normalExport;

vec4 coords;

void main(void)
{
   if (object == FIELD)
   {
      coords = Coords;
      texCoordsExport = TexCoords;
   }
   if (object == SKY)
   {
      coords = Coords;
      texCoordsExport = TexCoords;
   }
   if (object == SPHERE)
   {
      coords = sphereCoords;
      normalExport = sphereNormals;
   }
   if (object == CUBE)
   {
      coords = Coords;
   }


   
   gl_Position = projMat * modelViewMat * coords;
}