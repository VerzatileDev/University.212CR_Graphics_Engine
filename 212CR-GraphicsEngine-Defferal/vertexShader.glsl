#version 430 core

/* Objects definitions */


/* See Documentation <https://learnopengl.com/Getting-started/Shaders> */

//Locations of Data Sent Via glVertexAttribPointer And Activated by glEnableVertexAttribArray
layout(location=0) in vec4 squareCoords;
layout(location=1) in vec4 squareColors;

// Uniform Pass by glUniformMatrix4fv
uniform mat4 projMat;
uniform mat4 modelViewMat;

//Pass to fragmentShader 
out vec4 colorsExport;

vec4 coords;

void main(void)
{
   //gl_Position is internal to the shader containing the position of the current vertex
   gl_Position = projMat * modelViewMat * squareCoords;

   // Send Colour to FragmentShader.glsl
   colorsExport = squareColors;
}