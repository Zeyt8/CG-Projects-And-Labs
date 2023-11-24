#version 410

// Input
layout(location = 0) in vec2 texture_coord;

// Uniform properties
uniform sampler2D textureImage;
uniform ivec2 screenSize;
uniform int flipVertical;
uniform int outputMode = 2; // 0: original, 1: grayscale, 2: blur

// Output
layout(location = 0) out vec4 out_color;

// Local variables
vec2 textureCoord = vec2(texture_coord.x, (flipVertical != 0) ? 1 - texture_coord.y : texture_coord.y); // Flip texture
float grayscales[100];
vec4 colors[100];

vec4 grayscale()
{
    vec4 color = texture(textureImage, textureCoord);
    float gray = 0.21 * color.r + 0.71 * color.g + 0.07 * color.b; 
    return vec4(gray, gray, gray,  0);
}


vec4 blur(int blurRadius)
{
    vec2 texelSize = 1.0f / screenSize;
    vec4 sum = vec4(0);
    for(int i = -blurRadius; i <= blurRadius; i++)
    {
        for(int j = -blurRadius; j <= blurRadius; j++)
        {
            sum += texture(textureImage, textureCoord + vec2(i, j) * texelSize);
        }
    }
        
    float samples = pow((2 * blurRadius + 1), 2);
    return sum / samples;
}

vec4 median(int blurRadius)
{
    vec2 texelSize = 1.0f / screenSize;
    for (int i = -blurRadius; i <= blurRadius; i++)
	{
		for(int j = -blurRadius; j <= blurRadius; j++)
		{
            vec4 color = texture(textureImage, textureCoord + vec2(i, j) * texelSize);
            colors[i * blurRadius + j] = color;
			grayscales[i * blurRadius + j] = 0.21 * color.r + 0.71 * color.g + 0.07 * color.b;
		}
	}
    for (int i = 0; i < blurRadius * blurRadius; i++)
	{
		int min = i;
		for (int j = i + 1; j < blurRadius * blurRadius; j++)
		{
			if (grayscales[j] < grayscales[min])
			{
				min = j;
			}
		}
        // swap grayscales
		float temp = grayscales[i];
		grayscales[i] = grayscales[min];
		grayscales[min] = temp;
        // swap colors
        vec4 tempColor = colors[i];
        colors[i] = colors[min];
        colors[min] = tempColor;
	}
    return colors[blurRadius * blurRadius / 2];
}


void main()
{
    switch (outputMode)
    {
        case 1:
        {
            out_color = grayscale();
            break;
        }

        case 2:
        {
            out_color = blur(3);
            break;
        }

        case 3:
        {
			out_color = median(7);
			break;
		}

        default:
            out_color = texture(textureImage, textureCoord);
            break;
    }
}
