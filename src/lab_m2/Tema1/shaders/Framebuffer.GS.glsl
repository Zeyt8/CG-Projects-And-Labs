#version 430

layout(triangles) in;
layout(triangle_strip, max_vertices = 100) out;

uniform mat4 Projection;
uniform mat4 viewMatrices[6];

in vec3 geom_position[3];
in vec2 geom_texture_coord[3];
in vec3 geom_normal[3];

out vec3 frag_position;
out vec2 frag_texture_coord;

void duplicate(int layer)
{
    for (int i = 0; i < gl_in.length(); i++) {
        frag_position = geom_position[i];
        frag_texture_coord = geom_texture_coord[i];
	    gl_Position = Projection * viewMatrices[layer] * gl_in[i].gl_Position;
        EmitVertex();
    }
    EndPrimitive();
}

void main()
{
    // Update the code to compute the position from each camera view 
    // in order to render a cubemap in one pass using gl_Layer. Use the "viewMatrices"
    // attribute according to the corresponding layer.
    for (int layer = 0; layer < 6; layer++) {
		gl_Layer = layer;
        duplicate(layer);
    }
}
