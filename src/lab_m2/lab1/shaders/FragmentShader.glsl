#version 430

// Input
layout(location = 0) in vec2 texture_coord;

// Uniform properties
uniform sampler2D texture_1;

// Output
layout(location = 0) out vec4 out_color;


void main()
{
    // Apply the texture
    out_color = texture(texture_1, texture_coord);

    // Discard when alpha component < 0.75
    if (out_color.a < 0.75)
		discard;
}
