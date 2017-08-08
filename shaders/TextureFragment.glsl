///--------------------------------------------------------------------
/// @brief a simple texture shader
/// @author Tia-Louise Heeger
/// @version 1.0
/// @date 27/05/17
///--------------------------------------------------------------------
#version 330 core

/// @brief Pointer to the current 2D texture object
uniform sampler2D text;

/// @brief[in] vertext UVS
in vec2 vertUV;

/// @brief[out] fragment colour
out vec4 outColour;

void main ()
{
 // /Set the fragment colour to the current texture
 outColour = texture(text,vertUV);
}
