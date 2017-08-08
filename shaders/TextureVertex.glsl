///--------------------------------------------------------------------
/// @brief a simple texture shader
/// @author Tia-Louise Heeger
/// @version 1.0
/// @date 27/05/17
///--------------------------------------------------------------------
#version 330 core

uniform mat4 MVP;
/// @brief Vertex values from VAO
layout (location=0) in vec3 inVert;

/// @brief UV values from VAO
layout (location=1)in vec2 inUV;

//? @brief[out] Pass the UV values to the fragment shader
out vec2 vertUV;

void main(void)
{

/// calculates the vertex position
gl_Position = MVP*vec4(inVert, 1.0);
vertUV=inUV.st;
}
