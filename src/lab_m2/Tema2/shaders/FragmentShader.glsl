#version 410

// Input
layout(location = 0) in vec2 texture_coord;

// Uniform properties
uniform sampler2D textureImage;

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
    float I11 = grayscale(texelFetch(image, ivec2(gl_FragCoord.xy) + ivec2(1, 1), 0)).r;
    float I10 = grayscale(texelFetch(image, ivec2(gl_FragCoord.xy) + ivec2(1, 0), 0)).r;
    float I1_1 = grayscale(texelFetch(image, ivec2(gl_FragCoord.xy) + ivec2(1, -1), 0)).r;
    float I_11 = grayscale(texelFetch(image, ivec2(gl_FragCoord.xy) + ivec2(-1, 1), 0)).r;
    float I_10 = grayscale(texelFetch(image, ivec2(gl_FragCoord.xy) + ivec2(-1, 0), 0)).r;
    float I_1_1 = grayscale(texelFetch(image, ivec2(gl_FragCoord.xy) + ivec2(-1, -1), 0)).r;
    float I01 = grayscale(texelFetch(image, ivec2(gl_FragCoord.xy) + ivec2(0, 1), 0)).r;
    float I0_1 = grayscale(texelFetch(image, ivec2(gl_FragCoord.xy) + ivec2(0, -1), 0)).r;
    float dx = I11 + 2 * I10 + I1_1 - I_11 - 2 * I_10 - I_1_1;
    float dy = I11 + 2 * I01 + I_11 - I1_1 - 2 * I0_1 - I_1_1;
    return sqrt(dx * dx + dy * dy);
}


void main()
{
    float edge_value = sobel(textureImage);
    out_color = edge_value > 0.9 ? vec4(1, 1, 1, 1) : vec4(0, 0, 0, 1);
}
