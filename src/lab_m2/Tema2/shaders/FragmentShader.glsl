#version 410

// Input
layout(location = 0) in vec2 texture_coord;

// Uniform properties
uniform sampler2D textureImage;
uniform ivec2 screenSize;

// Output
layout(location = 0) out vec4 out_color;


vec4 grayscale(vec4 c)
{
    vec4 color = c;
    float gray = 0.21 * color.r + 0.71 * color.g + 0.07 * color.b; 
    return vec4(gray, gray, gray,  0);
}

float sobel(sampler2D image)
{
	vec2 texelSize = 1.0f / screenSize;
    float I11 = grayscale(texture(image, texture_coord + vec2(1, 1) * texelSize)).r;
    float I10 = grayscale(texture(image, texture_coord + vec2(1, 0) * texelSize)).r;
    float I1_1 = grayscale(texture(image, texture_coord + vec2(1, -1) * texelSize)).r;
    float I_11 = grayscale(texture(image, texture_coord + vec2(-1, 1) * texelSize)).r;
    float I_10 = grayscale(texture(image, texture_coord + vec2(-1, 0) * texelSize)).r;
    float I_1_1 = grayscale(texture(image, texture_coord + vec2(-1, -1) * texelSize)).r;
    float I01 = grayscale(texture(image, texture_coord + vec2(0, 1) * texelSize)).r;
    float I0_1 = grayscale(texture(image, texture_coord + vec2(0, -1) * texelSize)).r;
    float dx = I11 + 2 * I10 + I1_1 - I_11 - 2 * I_10 - I_1_1;
    float dy = I11 + 2 * I01 + I_11 - I1_1 - 2 * I0_1 - I_1_1;
    return sqrt(dx * dx + dy * dy);
}


void main()
{
    float edge_value = sobel(textureImage);
    out_color = edge_value > 0.9 ? vec4(1, 1, 1, 1) : vec4(0, 0, 0, 1);
}
