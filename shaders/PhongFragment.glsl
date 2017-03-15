
#version 330 core
//Output fragment colour
layout (location =0) out vec4 fragColour;

//The Fragment normal
in vec3 fragmentNormal;

//The vertex colour in
in vec3 vertColour;

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

// Material passed from our program
uniform Materials material;

uniform Lights light;
in vec3 lightDir;

// Out the blinn half vector
in vec3 halfVector;
in vec3 eyeDirection;
in vec3 vPosition;

//---------------------------------------------------------------------------------

//A function to calculate Directional light values
vec4 DirectionalLight()
{
  vec3 N = normalize(fragmentNormal);
  vec3 halfV;
  float ndothv;
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


//If stament to swtich between phong shader and flat diffuse colour shader
    if(myflag==0)
    {
        fragColour=DirectionalLight();//Enables directional light within the phong shader
    }

    else
    {
     fragColour= vec4(vertColour,1);//Enables flat diffuse colour shader
    }

}


