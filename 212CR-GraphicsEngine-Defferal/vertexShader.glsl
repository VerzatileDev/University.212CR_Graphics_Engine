#version 430 core

/* See Documentation <https://learnopengl.com/Getting-started/Shaders> */

/* Objects definitions */
// THese have to be In order See Main.cpp |static enum object| and | static enum buffer|
#define TRACK 0
#define HOVER 1
#define SPHERE 2
#define SKYBOX 3


// Locations of Data Sent by glVertexAttribPointer And Activated by glEnableVertexAttribArray
layout(location=0) in vec3 skyCoords; // SKY coordinates 
layout(location=1) in vec3 skyNormals;

layout(location=2) in vec4 sphereCoords;
layout(location=3) in vec3 sphereNormals;

// OBJECT DATA LOCATION, NORMALS, TEXCOORDS
layout(location=4) in vec3 objCoords;
layout(location=5) in vec3 objNormals;
layout(location=6) in vec2 objTexCoords;



// Declared at Main Under Static Globals area 
// Uniform Pass by glUniformMatrix4fv
uniform mat4 modelViewMat;
uniform mat4 projMat;
uniform uint object;
uniform float yPos;

// Out means it is Passed Out to NExt Shader  " Fragment Shader "
out vec2 texCoordsExport;
out vec3 SkytexCoordsExport;
out vec3 normalExport;

vec4 coords;

void main(void)
{


   if (object == TRACK)
   {
    coords = vec4(objCoords, 1.0f);
    //coords.y = coords.y+yPos;
    normalExport = objNormals;
    texCoordsExport = objTexCoords;
   }
    if (object == HOVER)
   {
     coords = vec4(objCoords, 1.0f);
     normalExport = objNormals;
     texCoordsExport = objTexCoords;
   }
    if (object == SKYBOX)
    {
        SkytexCoordsExport = skyCoords;
        coords = vec4(skyCoords, 1.0);
        //coords = skyCoords;
    }
    if (object == SPHERE)
    {
      coords = sphereCoords;       // Location of The Object 
      normalExport = sphereNormals; // Sphere Normals 
    }

   
   gl_Position = projMat * modelViewMat * coords;
}