#version 430

// Input
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;

// Uniform properties
uniform mat4 Model;
uniform float deltaTime;

out float vert_lifetime;
out float vert_iLifetime;

vec3 controls_0[5] = vec3[](vec3(0, 0,  0), vec3(0, 0, 0), vec3(0, 0, 0), vec3(0, 0, 0), vec3(0, 0, 0));
vec3 controls_1[5] = vec3[](vec3(0,  0.5,  1), vec3(0.2,  1.0,  0.5), vec3(0.4,  0,  1), vec3(0.25,  -1,  -7), vec3(1.5,  -1,  0.5));
vec3 controls_2[5] = vec3[](vec3(-0.3,  1,  0), vec3(-1.5,  2.0,  0.4), vec3(0,  0,  1.5), vec3(0.25,  -2.0,  0.5), vec3(1.5,  0,  0));
vec3 controls_3[5] = vec3[](vec3(-1.0,  2,  0), vec3(-0.75,  1,  0.7), vec3(-0.3,  2,  1), vec3(0.25,  -1,  1), vec3(1.5,  0,  1.0));

struct Particle
{
    int id;
    float speed;
    float delay;
    float iDelay;
    float lifetime;
    float iLifetime;
};


layout(std430, binding = 0) buffer particles {
    Particle data[];
};


float rand(vec2 co)
{
    return fract(sin(dot(co.xy, vec2(12.9898, 78.233))) * 43758.5453);
}


vec3 bezier(float t, int index)
{
    return  controls_0[index] * pow((1 - t), 3) +
            controls_1[index] * 3 * t * pow((1 - t), 2) +
            controls_2[index] * 3 * pow(t, 2) * (1 - t) +
            controls_3[index] * pow(t, 3);
}


void main()
{
    int id = data[gl_VertexID].id; 
    float speed = data[gl_VertexID].speed;
    float lifetime = data[gl_VertexID].lifetime;
    float delay = data[gl_VertexID].delay;

    lifetime -= deltaTime * speed;
    delay -= deltaTime;

    vec3 pos = bezier(1 - lifetime / data[gl_VertexID].iLifetime, id);

    if (lifetime < 0)
	{
		lifetime = data[gl_VertexID].iLifetime;
		delay = 0;
	}

    data[gl_VertexID].lifetime = lifetime;
    data[gl_VertexID].delay = delay;

    gl_Position = Model * vec4(pos, 1);
}
