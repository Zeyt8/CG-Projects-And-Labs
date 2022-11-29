#version 410

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec3 v_texcoord;
layout(location = 3) in vec3 v_color;
layout(location = 4) in float obj_position;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform float scale_factor;

out vec3 frag_color;

void main()
{
    frag_color = v_color;
	vec3 pos = v_position;
    gl_Position = Projection * View * Model * vec4(pos, 1.0);
}