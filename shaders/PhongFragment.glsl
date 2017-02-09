#version 330 core
/// @brief our output fragment colour
layout (location =0) out vec4 fragColour; //box colour from inVert
/// @brief[in] the vertex color in
in vec3 vertColour;

void main ()
{
//out color
fragColour= vec4(vertColour,1);

}


/*#version 330 core
//Output fragment colour
layout (location =0) out vec4 fragColour;

//The Fragment normal
in vec3 fragmentNormal;

//The vertex colour in
in vec3 vertColour;

//---------------------------------------------------------------------------------

///Material structure phong model
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
  float constantAttenuation;
  float spotCosCutoff;
  float quadraticAttenuation;
  float linearAttenuation;
};

//---------------------------------------------------------------------------------

// Material passed from our program
uniform Materials material;

uniform Lights light;
in vec3 lightDir;
// Out the blinn half vector
in vec3 halfVector;
in vec3 eyeDirection;
in vec3 vPosition;

//---------------------------------------------------------------------------------

//A function to calculate point light values
vec4 pointLight()
{
  vec3 N = normalize(fragmentNormal);
  vec3 halfV;
  float ndothv;
  float attenuation;
  vec3 E = normalize(eyeDirection);
  vec3 L = normalize(lightDir);
  float lambertTerm = dot(N,L);
  vec4 diffuse=vec4(0);
  vec4 ambient=vec4(0);
  vec4 specular=vec4(0);
  if (lambertTerm > 0.0)
  {
  float d;            // Distance from surface to light position
  vec3 VP;            // Direction from surface to light position


  VP = vec3 (light.position) - vPosition; // Calculate vector from surface to light position
  d = length (VP);// Calculate distance between surface and light position

  //Multiplications of material and light to get total diffuse, ambient and specular values.
  diffuse+=material.diffuse*light.diffuse*lambertTerm;
  ambient+=material.ambient*light.ambient;
  halfV = normalize(halfVector);
  ndothv = max(dot(N, halfV), 0.0);
  specular+=material.specular*light.specular*pow(ndothv, material.shininess);
  }
return ambient + diffuse + specular; //Complete phong shader
}

void main ()
{

fragColour=pointLight();
//fragColour= vec4(vertColour,1);//Rainbow shader for cube and sphere.

}*/
