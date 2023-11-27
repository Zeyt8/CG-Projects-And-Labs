#version 430

layout(triangles) in;
layout(triangle_strip, max_vertices = 100) out;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform mat4 viewMatrices[6];
uniform int type;

in vec3 geom_position[3];
in vec2 geom_texture_coord[3];
in vec3 geom_normal[3];

out vec3 frag_position;
out vec2 frag_texture_coord;

vec3 cameraDirection;

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

void edge(vec3 p1, vec3 p2, vec3 p3, float[3] o, int layer)
{
    vec3 p12 = p1 + (p2 - p1) * (abs(o[0]) / (abs(o[0]) + abs(o[1])));
    vec3 p23 = p3 + (p2 - p3) * (abs(o[2]) / (abs(o[2]) + abs(o[1])));
    frag_position = p12;
    gl_Position = Projection * viewMatrices[layer] * Model * vec4(p12, 1);
    EmitVertex();
    frag_position = p23;
    gl_Position = Projection * viewMatrices[layer] * Model * vec4(p23, 1);
    EmitVertex();
    if (o[1] < 0) {
		frag_position = p2;
		gl_Position = Projection * viewMatrices[layer] * Model * vec4(p2, 1);
		EmitVertex();
	} else {
		frag_position = p1;
		gl_Position = Projection * viewMatrices[layer] * Model * vec4(p1, 1);
		EmitVertex();
		frag_position = p3;
		gl_Position = Projection * viewMatrices[layer] * Model * vec4(p3, 1);
		EmitVertex();
	}
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

void setCameraDirection(int layer)
{
    if (layer == 0) {
        cameraDirection = vec3(1, 0, 0);
    }
    if (layer == 1) {
		cameraDirection = vec3(-1, 0, 0);
	}
    if (layer == 2) {
		cameraDirection = vec3(0, 1, 0);
	}
    if (layer == 3) {
		cameraDirection = vec3(0, -1, 0);
	}
    if (layer == 4) {
		cameraDirection = vec3(0, 0, 1);
	}
    if (layer == 5) {
		cameraDirection = vec3(0, 0, -1);
	}
}

void main()
{
    // Update the code to compute the position from each camera view 
    // in order to render a cubemap in one pass using gl_Layer. Use the "viewMatrices"
    // attribute according to the corresponding layer.
    for (int layer = 0; layer < 6; layer++) {
		gl_Layer = layer;
        setCameraDirection(layer);
        if (type == 0 || type == 2) {
            duplicate(layer);
        } else if (type == 1) {
			wireframe(layer);
		}
    }
}
