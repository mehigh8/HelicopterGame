#version 430
// Rosu Mihai Cosmin GMRV1A

// Input
in vec3 position;

// Uniform properties
uniform mat4 Model;
uniform sampler2D texture_1;
uniform sampler2D texture_2;
uniform int level;
uniform float treeHeight;
uniform int terrainSize;
uniform int treeID;

// Output
layout(location = 0) out vec4 out_color;
layout(location = 1) out vec4 out_info;


void main()
{
    vec4 world_position = Model * vec4(position, 1.0);
    vec4 color = vec4(1);

    // Use cylinder mapping.
    vec2 tex_coords = vec2(atan(world_position.z / world_position.x) / (3.1415 * 2.0), world_position.y / treeHeight);

    if (level < 4) {
        color = texture2D(texture_1, tex_coords);
    } else if (level < 5) {
        vec4 color1 = texture2D(texture_1, tex_coords);
        vec4 color2 = texture2D(texture_2, tex_coords);
        color = mix(color1, color2, 0.5);
    } else {
        color = texture2D(texture_2, tex_coords);
    }

    out_color = color;

    // For the second color texture, the value is based on the tree's position for r and b, and on the tree's ID for g.
    double treeIdScaled = treeID / 100.0;
    out_info = vec4((world_position.x + terrainSize / 2) / terrainSize, 0.2 + treeIdScaled, (world_position.z + terrainSize / 2) / terrainSize, 0);
}
