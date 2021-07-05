#version 430 core

// THese have to be In order See Main.cpp |static enum object| and | static enum buffer|

#define TRACK 0
#define HOVER 1
#define SPHERE 2
#define SKYBOX 3


// https://learnopengl.com/Getting-started/Shaders
// Recived From Vertex Shader 
in vec2 texCoordsExport;
in vec3 normalExport;
in vec3 SkytexCoordsExport;

// Sphere Lighting 
struct Light
{
   vec4 ambCols;
   vec4 difCols;
   vec4 specCols;
   vec4 coords;
};

struct Material
{
   vec4 ambRefl;
   vec4 difRefl;
   vec4 specRefl;
   vec4 emitCols;
   float shininess;
};


// Sphere Lighting 
uniform Light light0;
uniform vec4 globAmb;
uniform Material sphereFandB;


//Final colour of the pixel
out vec4 colorsOut;

// Sphere Lighting 
vec3 normal, lightDirection;
vec4 fAndBDif;

/* Colour Def */
vec4 fieldTexColor, skyTexColor, specialTexColor, redTexColor;


// Pass to Vertex from Uniform
uniform sampler2D grassTex;    // GRASS TEXTURE
uniform sampler2D skyTex;      // SKY TEXTURE
uniform sampler2D cosmosTex;     // Wood Texture
uniform sampler2D redTex;
uniform samplerCube skyboxTexture;
uniform uint object;          // Object on Field

void main(void)
{
   //vec4(0.4,0.4,0.4,1.0); // Fixed Colour Gray

   /* Note Has to be Declared Under Vec4 Colour Def  And Passed to Vertex*/
   fieldTexColor = texture(grassTex, texCoordsExport);
   skyTexColor = texture(skyTex, texCoordsExport);
   specialTexColor = texture(cosmosTex, texCoordsExport);
   redTexColor = texture(redTex, texCoordsExport);


   if (object == TRACK) {
	colorsOut = specialTexColor; // Red
   }
   if (object == HOVER) {
    normal = normalize(normalExport);
    lightDirection = normalize(vec3(light0.coords));
    fAndBDif = max(dot(normal, lightDirection), 0.0f) * (light0.difCols * sphereFandB.difRefl); 
    colorsOut =  redTexColor*vec4(vec3(min(fAndBDif, vec4(1.0))), 1.0);
   }

    if (object == SKYBOX) {
    colorsOut = texture(skyboxTexture, SkytexCoordsExport);
    //colorsOut = vec4(1.0,0.0,0.0,1.0);
    }
     if (object == SPHERE) {
    normal = normalize(normalExport);
	lightDirection = normalize(vec3(light0.coords));
	fAndBDif = max(dot(normal, lightDirection), 0.0f) * (light0.difCols * sphereFandB.difRefl); 
    colorsOut =  vec4(vec3(min(fAndBDif, vec4(1.0))), 1.0);
   }

}