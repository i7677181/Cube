///--------------------------------------------------------------------
/// @brief a simple phong shader
/// @author Tia-Louise Heeger
/// @version 1.0
/// @date 27/05/17
///--------------------------------------------------------------------
#version 330 core
/// @brief[out] Output fragment colour
layout (location =0) out vec4 fragColour;

/// @brief[in] The Fragment normal
in vec3 fragmentNormal;

/// @brief[in] The vertex colour
in vec3 vertColour;

/// @brief Uniform integer flag
uniform int myflag;

/// @brief Material structure
struct Materials
{
    /// material ambient
    vec4 ambient;
    /// material diffuse
    vec4 diffuse;
    /// material specular
    vec4 specular;
    /// material shininess
    float shininess;
};

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

/// @brief Material information from structure passed from our program
uniform Materials material;

/// @brief Light information from structure to light 1
uniform LightInfo light1;

/// @brief Light information from structure to light 2
uniform LightInfo light2;

/// @brief Light information from structure to light 3
uniform LightInfo light3;

/// @brief Light information from structure to light 4
uniform LightInfo light4;

/// @brief[in] Light direction
in vec3 lightDir;

/// @brief[in] the blinn half vector
in vec3 halfVector;

/// @breif[in] the eye direction
in vec3 eyeDirection;

/// @brief A function to calculate Point light values
/// @param[in] _normal, the normal vector
/// @param[inout] _ambient value of light1
/// @param[inout] _diffuse value of light1
/// @param[inout] _specular value of light1

void PointLight1(
                in vec3 _normal,
                inout vec4 _ambient,
                inout vec4 _diffuse,
                inout vec4 _specular
                )
{
    /// normal dot VP
    float nDotVP;

    /// normal dot half vector value
    float nDotHV;

    /// pf value
    float pf;

    /// calulations for complete phong model for light 1
    nDotVP= max(0.0,dot(_normal, normalize(light1.position.xyz)));
    nDotHV= max(0.0,dot(_normal,halfVector));

    pf=clamp(nDotVP,0.0,pow(nDotHV,material.shininess));
    _ambient+=light1.ambient*material.ambient;
    _diffuse+=light1.diffuse*material.diffuse*nDotVP;
    _specular+=light1.specular*material.specular*pf;


}
/// @brief A function to calculate Point light values
/// @param[in] _normal, the normal vector
/// @param[inout] _ambient value of light2
/// @param[inout] _diffuse value of light2
/// @param[inout] _specular value of light2
void PointLight2(
                in vec3 _normal,
                inout vec4 _ambient,
                inout vec4 _diffuse,
                inout vec4 _specular
                )
{
    /// normal dot VP
    float nDotVP;
    /// normal dot half vector value
    float nDotHV;
    /// pf value
    float pf;

    /// calulations for complete phong model for light 1
    nDotVP= max(0.0,dot(_normal, normalize(light2.position.xyz)));
    nDotHV= max(0.0,dot(_normal,halfVector));

    pf=clamp(nDotVP,0.0,pow(nDotHV,material.shininess));
    _ambient+=light2.ambient*material.ambient;
    _diffuse+=light2.diffuse*material.diffuse*nDotVP;
    _specular+=light2.specular*material.specular*pf;

}
/// @brief A function to calculate Point light values
/// @param[in] _normal, the normal vector
/// @param[inout] _ambient value of light3
/// @param[inout] _diffuse value of light3
/// @param[inout] _specular value of light3
void PointLight3(
                in vec3 _normal,
                inout vec4 _ambient,
                inout vec4 _diffuse,
                inout vec4 _specular
                )
{
    /// normal dot VP
    float nDotVP;
    /// normal dot half vector value
    float nDotHV;
    /// pf value
    float pf;

    /// calulations for complete phong model for light 1
    nDotVP= max(0.0,dot(_normal, normalize(light3.position.xyz)));
    nDotHV= max(0.0,dot(_normal,halfVector));

    pf=clamp(nDotVP,0.0,pow(nDotHV,material.shininess));
    _ambient+=light3.ambient*material.ambient;
    _diffuse+=light3.diffuse*material.diffuse*nDotVP;
    _specular+=light3.specular*material.specular*pf;

}

/// @brief A function to calculate Point light values
/// @param[in] _normal, the normal vector
/// @param[inout] _ambient value of light4
/// @param[inout] _diffuse value of light4
/// @param[inout] _specular value of light4
void PointLight4(
                in vec3 _normal,
                inout vec4 _ambient,
                inout vec4 _diffuse,
                inout vec4 _specular
                )
{
    /// normal dot VP
    float nDotVP;
    /// normal dot half vector value
    float nDotHV;
    /// pf value
    float pf;

    /// calulations for complete phong model for light 1
    nDotVP= max(0.0,dot(_normal, normalize(light4.position.xyz)));
    nDotHV= max(0.0,dot(_normal,halfVector));

    pf=clamp(nDotVP,0.0,pow(nDotHV,material.shininess));
    _ambient+=light4.ambient*material.ambient;
    _diffuse+=light4.diffuse*material.diffuse*nDotVP;
    _specular+=light4.specular*material.specular*pf;

}

void main ()
{
    /// Vec4 for complete ambient value
    vec4 ambient=vec4(0.0);

    /// Vec4 for complete diffuse value
    vec4 diffuse=vec4(0.0);

    /// Vec4 for complete specular value
    vec4 specular=vec4(0.0);

    /// complete phong calulation value for Point light 1
    PointLight1(fragmentNormal,ambient,diffuse,specular);

    /// complete phong calulation value for Point light 2
    PointLight2(fragmentNormal,ambient,diffuse,specular);

    /// complete phong calulation value for Point light 3
    PointLight3(fragmentNormal,ambient,diffuse,specular);

    /// complete phong calulation value for Point light 4
    PointLight4(fragmentNormal,ambient,diffuse,specular);

    /// fragmnet colour equals the complete phong model
    fragColour=ambient+diffuse+specular;

    ///If stament to swtich between phong shader and flat diffuse colour shader
    if(myflag==0)
    {
        /// Enaples Point light within the phong shader
        fragColour=ambient+diffuse+specular;
    }

    else
    {
        /// Enables flat diffuse shader
        fragColour= vec4(vertColour,1);
    }

}



