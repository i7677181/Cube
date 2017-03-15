#version 330 core

// Pointer to the current 2D texture object
uniform sampler2D text;

in vec2 vertUV;
out vec4 outColour;
void main ()
{
 // Set the fragment colour to the current texture
 outColour = texture(text,vertUV);
}
