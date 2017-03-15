#version 330 core


uniform mat4 MVP;
// Vertex values from VAO
layout (location=0) in vec3 inVert;
// UV values from VAO
layout (location=1)in vec2 inUV;
// Pass the UV values to the fragment shader
out vec2 vertUV;

void main(void)
{

// calculates the vertex position
gl_Position = MVP*vec4(inVert, 1.0);
vertUV=inUV.st;
}
