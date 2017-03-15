
#version 330 core
//Both the vertex and normal need to have the same location or the shaders do not work correctly. Calculating the normal from the vertices values declared within the cube class.

//The vertex passed in
layout (location = 0) in vec3 inVert;

//The normal passed in
layout (location = 0) in vec3 inNormal;

//The in uv
layout (location = 1) in vec2 inUV;

//The colours from cube
in vec3 ColourPassedFromProgramToVertexShader;

//Flag to indicate if model has unit normals if not normalize
uniform bool Normalize;

//The eye position of the camera
uniform vec3 viewerPos;

//The current fragment normal for the vert being processed
out vec3 fragmentNormal;

//The out Vertex colour
out vec3 vertColour;

//Uniform integer flag
 uniform int myflag;

//---------------------------------------------------------------------------------

///Material structure
struct Materials
{
  vec4 ambient;
  vec4 diffuse;
  vec4 specular;
  float shininess;
};

//---------------------------------------------------------------------------------

// Light structure
struct Lights
{
  vec4 position;
  vec4 ambient;
  vec4 diffuse;
  vec4 specular;

};

//---------------------------------------------------------------------------------


uniform Materials material;// Our material

uniform Lights light;// Array of lights


out vec3 lightDir;// Direction of the lights used for shading

out vec3 halfVector;// out the blinn half vector
out vec3 eyeDirection;
out vec3 vPosition;

uniform mat4 MV;
uniform mat4 MVP;
uniform mat3 normalMatrix;
uniform mat4 M;
uniform vec3 Colour;


//---------------------------------------------------------------------------------

void main()
{

fragmentNormal = (normalMatrix*inNormal);// Calculate the fragments surface normal


if (Normalize == true)
{
 fragmentNormal = normalize(fragmentNormal);
}

gl_Position = MVP*vec4(inVert,1.0);// Calculate the vertex position
vec4 worldPosition = M * vec4(inVert, 1.0);
eyeDirection = normalize(viewerPos - worldPosition.xyz);

// Get vertex position in eye coordinates
// Transform the vertex to eye co-ordinates for frag shader
/// The vertex in eye co-ordinates homogeneous
vec4 eyeCord=MV*vec4(inVert,1);

vPosition = eyeCord.xyz / eyeCord.w;;

float dist;

lightDir=vec3(light.position.xyz-eyeCord.xyz);
dist = length(lightDir);
lightDir/= dist;
halfVector = normalize(eyeDirection + lightDir);


//if stament to switch between phong shader and flat diffuse colour shader
if(myflag==0)
{
    vertColour=ColourPassedFromProgramToVertexShader;//Enables phong shader calculations
}
else
{
    vertColour=Colour;//Flat diffuse colour
}

}


