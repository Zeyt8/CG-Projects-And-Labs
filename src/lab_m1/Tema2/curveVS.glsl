#version 410

layout(location = 0) in vec3 v_position;
layout(location = 3) in vec3 v_color;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform vec3 car_position;
uniform vec3 obj_position;
uniform vec3 obj_rotation;

out vec3 frag_color;

void main()
{
    frag_color = v_color;
	vec3 pos = v_position;
    pos.y -= pow(length(car_position - (obj_position + v_position)), 2) * 0.001f;
    gl_Position = Projection * View * Model * vec4(pos, 1.0);
}