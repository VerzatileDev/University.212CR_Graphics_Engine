#version 430 core

/* See Documentation <https://learnopengl.com/Getting-started/Shaders> */

/* Objects definitions */
#define FIELD 0


//Locations of Data Sent Via glVertexAttribPointer And Activated by glEnableVertexAttribArray
layout(location=0) in vec4 Coords;
layout(location=1) in vec2 TexCoords;

// Uniform Pass by glUniformMatrix4fv
uniform mat4 modelViewMat;
uniform mat4 projMat;
uniform uint object;

//Pass to fragmentShader 
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
   
   gl_Position = projMat * modelViewMat * coords;
}