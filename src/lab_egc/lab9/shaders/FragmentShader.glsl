#version 330

// Input
in vec2 texcoord;

// Uniform properties
uniform sampler2D texture_1;
uniform sampler2D texture_2;

// Output
layout(location = 0) out vec4 out_color;


void main()
{
    // TODO(student): calculate the out_color using the texture2D() function

    out_color = vec4(1);
}
