#version 430

// Input
layout(location = 0) in vec3 world_position;
layout(location = 1) in vec3 world_normal;

// Uniform properties
uniform samplerCube texture_cubemap;

uniform vec3 camera_position;

// Output
layout(location = 0) out vec4 out_color;


vec3 myReflect()
{
    // Compute the reflection color value
    return texture(texture_cubemap, reflect(normalize(world_position - camera_position), world_normal)).rgb;
}


vec3 myRefract(float refractive_index)
{
    // Compute the refraction color value
    return texture(texture_cubemap, refract(normalize(world_position - camera_position), world_normal, 1.0 / refractive_index)).rgb;
}


void main()
{
    out_color = vec4(myReflect(), 0);
}
