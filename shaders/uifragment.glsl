#version 330 core

// Pointer to the current 2D texture object
uniform sampler2D text;

//passed through first attribute for vertex values of VAO
in vec2 vertUV;

//outputs the texture
out vec4 outColour;

void main ()
{
 // Set the fragment colour to the current texture
 outColour = texture(text,vertUV);

}
