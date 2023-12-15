#version 430

layout(triangles) in;
layout(line_strip, max_vertices = 100) out;

uniform mat4 Model;
uniform mat4 Projection;
uniform mat4 viewMatrices[6];
uniform vec3 cameraDirection;

in vec3 geom_position[3];
in vec2 geom_texture_coord[3];
in vec3 geom_normal[3];

out vec3 frag_position;
out vec2 frag_texture_coord;

void edge(vec3 p1, vec3 p2, vec3 p3, float[3] o, int layer)
{
    vec3 p12 = p1 + (p2 - p1) * (abs(o[0]) / (abs(o[0]) + abs(o[1])));
    vec3 p32 = p3 + (p2 - p3) * (abs(o[2]) / (abs(o[2]) + abs(o[1])));
    frag_position = p12;
    gl_Position = Projection * viewMatrices[layer] * Model * vec4(p12, 1);
    EmitVertex();
    frag_position = p32;
    gl_Position = Projection * viewMatrices[layer] * Model * vec4(p32, 1);
    EmitVertex();
    EndPrimitive();
}

void wireframe(int layer)
{
    float vert_orientation[3];
    int vert_dir[3];
    for (int i = 0; i < gl_in.length(); i++) {
        float d = dot(cameraDirection, geom_normal[i]);
        vert_orientation[i] = d;
        vert_dir[i] = d > 0 ? 1 : (d < 0 ? -1 : 0);
    }
    if (vert_dir[0] == vert_dir[1] && vert_dir[1] == vert_dir[2]) return;
    if (vert_dir[0] == vert_dir[1]) {
        // 0 and 1 have the same orientation, 2 is different
        edge(geom_position[0], geom_position[2], geom_position[1], vert_orientation, layer);
    } else {
        if (vert_dir[1] == vert_dir[2]) {
            // 1 and 2 have the same orientation, 0 is different
            edge(geom_position[1], geom_position[0], geom_position[2], vert_orientation, layer);
        } else {
            // 0 and 2 have the same orientation, 1 is different
            edge(geom_position[0], geom_position[1], geom_position[2], vert_orientation, layer);
        }
    }
}

void main()
{
    // Update the code to compute the position from each camera view 
    // in order to render a cubemap in one pass using gl_Layer. Use the "viewMatrices"
    // attribute according to the corresponding layer.
    for (int layer = 0; layer < 6; layer++) {
		gl_Layer = layer;
	    wireframe(layer);
    }
}
