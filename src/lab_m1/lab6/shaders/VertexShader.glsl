#version 410

// Input
// Get vertex attributes from each location
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec3 v_texcoord;
layout(location = 3) in vec3 v_color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform float Time;

// Output
// Output values to fragment shader
out vec3 frag_position;
out vec3 frag_normal;
out vec3 frag_texcoord;
out vec3 frag_color;

void main()
{
    // Send output to fragment shader
    vec3 norm = v_normal;
    norm.x = norm.x + cos(Time);
    frag_color = norm;
    // Compute gl_Position
    vec3 pos = v_position;
    pos.x = pos.x + cos(Time);
    gl_Position = Projection * View * Model * vec4(pos, 1.0);
}
