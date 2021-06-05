#version 430 core

//THIS IS FROM THE VERTEX SHADER
in vec4 colorsExport;

//FINAL COLOUR FOR THE PIXEL
out vec4 colorsOut;

void main(void)
{
   //A STRAIGHT PASS THROUGH FOR THE COLOUR
   //IN A FEW WEEKS TIME, THIS WILL BE USED FOR LIGHTING EFFECTS
   colorsOut = colorsExport;
}