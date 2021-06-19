#version 430 core

#define FIELD 0

//Recived from vertex shader.
in vec2 texCoordsExport;
in vec3 normalExport;

uniform uint object;

//Final colour of the pixel
out vec4 colorsOut;

vec4 fieldTexColor, skyTexColor;

void main(void)
{
   /* Colour Def */
   fieldTexColor = vec4(0.4,0.4,0.4,1.0);

   if (object == FIELD) colorsOut = fieldTexColor;

}