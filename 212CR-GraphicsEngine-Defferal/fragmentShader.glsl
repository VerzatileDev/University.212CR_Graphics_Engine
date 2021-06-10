#version 430 core

//Recived from vertex shader.
in vec4 colorsExport;

//Final colour of the pixel
out vec4 colorsOut;

void main(void)
{
   //Pass through the colour

   //Lighiting Effects.
   colorsOut = colorsExport;
}