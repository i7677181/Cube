///--------------------------------------------------------------------
/// @brief a simple phong shader
/// @author Tia-Louise Heeger
/// @version 1.0
/// @date 27/05/17
///--------------------------------------------------------------------
#version 330 core
/// @brief[in] The vertex data passed in
layout (location = 0) in vec3 inVert;

/// @brief[in] The normal data passed in
layout (location = 2) in vec3 inNormal;

/// @brief[in] The UV data passed in
layout (location = 1) in vec2 inUV;

/// @brief[in] The Colour data from program
in vec3 ColourPassedFromProgramToVertexShader;

/// @brief Flag to indicate if model has unit normals if not normalize
uniform bool Normalize;

/// @brief The viwer position of the camera
uniform vec3 viewerPos;

/// @brief[out] The current fragment normal for the vert being processed
out vec3 fragmentNormal;

/// @brief[out] The out Vertex colour
out vec3 vertColour;

/// @brief Uniform integer flag
 uniform int myflag;

/// @brief light structure
struct LightInfo
{
    //// light position
    vec4 position;
    /// light ambient
    vec4 ambient;
    /// light diffuse
    vec4 diffuse;
    /// light specular
    vec4 specular;
    /// light spot cos cut off
    float spotCosCutoff;
    /// light constant attenuation
    float constantAttenuation;
    /// light linear attenuation
    float linearAttenuation;
    /// light quadratic attenuation
    float quadraticAttenuation;

};

/// @brief Light information from structure to light 1
uniform LightInfo light1;

/// @brief Light information from structure to light 2
uniform LightInfo light2;

/// @brief Light information from structure to light 3
uniform LightInfo light3;

/// @brief Light information from structure to light 4
uniform LightInfo light4;

/// @brief[out] Light direction
out vec3 lightDir;

/// @brief[out] the blinn half vector
out vec3 halfVector;

/// @breif[out] the eye direction
out vec3 eyeDirection;

/// @brief Uniforms for shading calulations
uniform mat4 MV;
uniform mat4 MVP;
uniform mat3 normalMatrix;
uniform mat4 M;

/// @brief Uniform vec3 for flat diffuse sahder
uniform vec3 Colour;


void main()
{
    /// Calculates the fragment surface normal
    fragmentNormal = (normalMatrix*inNormal);

    /// Normalise the fragmnet surface
    if (Normalize == true)
    {
        fragmentNormal = normalize(fragmentNormal);
    }
    /// calculate the vertex position
    gl_Position = MVP*vec4(inVert,1.0);

    // Transform the vertex to eye co-ordinates for frag shader
    /// The vertex in eye co-ordinates  homogeneous
    vec4 eyeCord=MV*vec4(inVert,1);

    /// accurate light direction calculations with eye coordinates
    lightDir=vec3(light1.position.xyz-eyeCord.xyz);
    lightDir=vec3(light2.position.xyz-eyeCord.xyz);
    lightDir=vec3(light3.position.xyz-eyeCord.xyz);
    lightDir=vec3(light4.position.xyz-eyeCord.xyz);

    // vec4 for world position
    vec4 worldPosition = M * vec4(inVert, 1.0);

    /// normalising eye direction
    eyeDirection = normalize(viewerPos - worldPosition.xyz);

    /// Distance variable
    float dist;
    /// accurate light direction calculations with world positions
    lightDir=(light1.position.xyz - worldPosition.xyz);
    lightDir=(light2.position.xyz - worldPosition.xyz);
    lightDir=(light3.position.xyz - worldPosition.xyz);
    lightDir=(light4.position.xyz - worldPosition.xyz);

    dist = length(lightDir);
    lightDir/= dist;
    halfVector = normalize(eyeDirection + lightDir);


/// @brief if stament to switch between phong shader and flat diffuse colour shader
if(myflag==0)
    {
        /// enables phong shader calulations
        vertColour=ColourPassedFromProgramToVertexShader;
    }
    else
    {
        ///enables flat diffuse colour
        vertColour=Colour;
    }

}






