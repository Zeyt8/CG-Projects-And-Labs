#version 330

// Input
in vec3 world_position;
in vec3 world_normal;

// Uniforms for light properties
uniform vec3 light_direction[2];
uniform vec3 light_position[2];
uniform vec3 eye_position;

uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;

uniform vec3 object_color;

uniform bool spot_light;
uniform float spot_angle;

// Output
layout(location = 0) out vec4 out_color;

float lightContrib(vec3 lightPos, vec3 lightDir)
{
    // Define diffuse and specular light components
	vec3 L = normalize(lightPos - world_position);
    vec3 V = normalize(eye_position - world_position);
    vec3 H = normalize(L + V);
	
    float diffuse_light = material_kd * max(dot(world_normal, L), 0);
    vec3 R = reflect(-L, world_normal);
    float specular_light = 0;
	
    // It's important to distinguish between "reflection model" and
    // "shading method". In this shader, we are experimenting with the Phong
    // (1975) and Blinn-Phong (1977) reflection models, and we are using the
    // Phong (1975) shading method. Don't mix them up!
    if (diffuse_light > 0)
    {
        specular_light = material_ks * pow(max(dot(V, R), 0), material_shininess);
    }

    // If (and only if) the light is a spotlight, we need to do
    // some additional things.
    float light_att_factor = 0;
	if (spot_light)
	{
	    float cut_off = radians(spot_angle);
        float spot_light = dot(-L, lightDir);
        float spot_light_limit = cos(cut_off);
        if (spot_light > cos(cut_off))
        {
	        float linear_att = (spot_light - spot_light_limit) / (1.0f - spot_light_limit);
            light_att_factor = pow(linear_att, 2);
        }
	}
    else
    {
        light_att_factor = 1;
    }

    // Compute the total light. You can just add the components
    // together, but if you're feeling extra fancy, you can add individual
    // colors to the light components. To do that, pick some vec3 colors that
    // you like, and multiply them with the respective light components.
    float lightColor = (1 / pow(length(lightPos - world_position), 2)) * light_att_factor * (diffuse_light + specular_light);

    return lightColor;
}

void main()
{
    float ambient_light = material_kd * 0.25;
	float lightColor = lightContrib(light_position[0], light_direction[0]);
	float lightColor2 = lightContrib(light_position[1], light_direction[1]);
    // Write pixel out color
    out_color = vec4((ambient_light + lightColor + lightColor2) * object_color, 1);
}
