#version 430 core

//THESE ARE THE LOCATIONS WE SEND DATA TO VIA glVertexAttribPointer AND THEN ACTIVATE WITH glEnableVertexAttribArray
//NOTE HOW WE TOLD IT WE WERE PASSING 4 FOR EACH AND ARE NOW USING in vec4
//WE CAN CALL THE VARIABLES WHATEVER WE WANT, ITS JUST FOR USE IN THE SHADERS
//THESE CHANGE PER VERTEX
layout(location=0) in vec4 squareCoords;
layout(location=1) in vec4 squareColors;

//UNIFORMS ARE PASSED IN VIA glUniformMatrix4fv
//NOTE HOW THESE ARE mat4 LIKE THE PASS IN FUNCTION USED
uniform mat4 projMat;
uniform mat4 modelViewMat;

//THIS VARIABLE IS out MEANING IT IS PASSED OUT TO THE NEXT SHADER - IN THIS CASE THE FRAGMENT SHADER
out vec4 colorsExport;

//EACH SHADER NEEDS A MAIN WHICH WILL RUN THE CODE INSIDE IT - LIKE A NORMAL PROGRAM
void main(void)
{
   //WE MOVE THE VERTEX CO-ORDINATE FROM MODEL SPACE ALL THE WAY TO HOMOGENEOUS CLIP SPACE BY MULTIPLYING THE MATRICES (ORDER MATTERS!)
   //gl_Position is internal to the shader containing the position of the current vertex (which we work out for each vertex)
   gl_Position = projMat * modelViewMat * squareCoords;
   //WE PASS THE COLOUR STRAIGHT TO THE FRAGMENT SHADER
   colorsExport = squareColors;
}