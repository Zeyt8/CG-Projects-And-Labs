#version 430

// Input
in vec2 texcoord;
in vec3 world_position;
in vec3 world_normal;

// Uniform properties
uniform sampler2D texture_1;
uniform vec2 displacement;
uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;
uniform vec3 eye_position;

// Output
layout(location = 0) out vec4 out_color;

struct Light
{
    int type; // 1 - Directional, 2 - Spot, 3 - Point
    vec3 position;
    vec3 direction;
	vec3 color;
    float angle;
    float intensity;
};

uniform Light lights[100];

vec3 lightContrib(in Light light)
{
    vec3 L = vec3(0);
    if (light.type == 1)
	{
		L = normalize(-light.direction);
	}
	else
	{
		L = normalize(light.position - world_position);
	}
    vec3 V = normalize(eye_position - world_position);
    vec3 H = normalize(L + V);
	
    float diffuse_light = material_kd * light.intensity * max(dot(world_normal, L), 0);
    vec3 R = reflect(-L, world_normal);
    float specular_light = 0;
	
    if (diffuse_light > 0)
    {
        specular_light = material_ks * light.intensity * pow(max(dot(V, R), 0), material_shininess);
    }

    float light_att_factor = 0;
	if (light.type == 2)
	{
	    float cut_off = radians(light.angle);
        float spot_light = dot(-L, light.direction);
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

    float attenuation = 1;
    if (light.type != 1)
    {
        attenuation = (1 / pow(length(light.position - world_position), 2)) * light_att_factor;
    }

    vec3 lightColor = attenuation * (diffuse_light + specular_light) * light.color;

    return lightColor;
}

void main()
{
    vec3 ambient_light = vec3(material_kd * 0.25);
	vec3 lightColor = vec3(0);
    for (int i = 0; i < 100; i++)
    {
        if (lights[i].type == 0)
		{
			break;
		}
		lightColor += lightContrib(lights[i]);
    }
    vec4 color = texture(texture_1, texcoord + displacement);
    out_color = vec4((ambient_light + lightColor) * vec3(color), color.a);
}
