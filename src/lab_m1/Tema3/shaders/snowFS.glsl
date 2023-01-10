#version 430

// Input
in vec2 texcoord;

// Uniform properties
uniform sampler2D texture_1;
uniform vec2 displacement;

// Output
layout(location = 0) out vec4 out_color;


void main()
{
    // Calculate the out_color using the texture2D() function.
    vec4 color;
    color = texture(texture_1, texcoord + displacement);
    out_color = color;
}
