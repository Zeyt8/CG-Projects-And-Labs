#version 410

layout(location = 0) in vec3 v_position;
layout(location = 3) in vec3 v_color;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform vec3 car_position;

out vec3 frag_color;

void main()
{
    vec3 world_pos = (Model * vec4(v_position, 1)).xyz;
    vec3 pos = v_position;
    frag_color = v_color;
    pos.y -= pow(length(car_position - world_pos), 2) * 0.001f;
    gl_Position = Projection * View * Model * vec4(pos, 1.0);
}