#version 430 core

// THese have to be In order See Main.cpp |static enum object| and | static enum buffer|
#define FIELD 0
#define CUBE 1
#define SPHERE 2
#define SKY 3


// https://learnopengl.com/Getting-started/Shaders
// Recived From Vertex Shader 
in vec2 texCoordsExport;
in vec3 normalExport;

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

// Field  Object 
uniform uint object;

// Sphere Lighting 
uniform Light light0;
uniform vec4 globAmb;
uniform Material sphereFandB;


//Final colour of the pixel
out vec4 colorsOut;

// Sphere Lighting 
vec3 normal, lightDirection;
vec4 fAndBDif;

vec4 fieldTexColor, skyTexColor;

// Pass to Vertex from Uniform
uniform sampler2D grassTex;    // GRASS TEXTURE
uniform sampler2D skyTex;      // SKY TEXTURE 

void main(void)
{
   /* Colour Def */
   //fieldTexColor = vec4(0.4,0.4,0.4,1.0); // Fixed Colour Gray
   fieldTexColor = texture(grassTex, texCoordsExport);
   skyTexColor = texture(skyTex, texCoordsExport);

   if (object == FIELD) 
   {
   colorsOut = fieldTexColor;  // Used For Lighting Effects 
   }
   if (object == SPHERE) {
	 normal = normalize(normalExport);
	 lightDirection = normalize(vec3(light0.coords));
	 fAndBDif = max(dot(normal, lightDirection), 0.0f) * (light0.difCols * sphereFandB.difRefl); 
     colorsOut =  vec4(vec3(min(fAndBDif, vec4(1.0))), 1.0); 

    //colorsOut =  vec4(0.0,1.0,0.0, 1.0);  // Fixed Colour 
   }
   if (object == CUBE)
   {
	  colorsOut = vec4(1.0,0.0,0.0,1.0); // Fixed Colour 
   }
   if (object == SKY) 
   {
   colorsOut = skyTexColor;
   }

}