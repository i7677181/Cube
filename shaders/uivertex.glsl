#version 330 core

//First attribute is the vertex values from the VAO
layout (location=0) in vec3 inVert;

// Second attribute is the UV values of the VAO
layout (location=1)in vec2 inUV;
// Pass the UV values to the fragment shader

/// @brief position of VAO passed through
uniform vec3 pos;
/// @brief scale of VAO passed through
uniform vec3 scale;
// we use this to pass the uv values to the frag shader
out vec2 vertUV;

void main(void)
{
// calculates the vertex position
gl_Position = vec4(pos + (scale * inVert), 1.0);
//pass the uv values to the frag shader
vertUV=inUV.st;
}
