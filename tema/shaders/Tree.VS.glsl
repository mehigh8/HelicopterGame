#version 430
// Rosu Mihai Cosmin GMRV1A

// Input
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;
layout(location = 3) in vec3 v_color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform sampler2D noise;
uniform int terrainSize;
uniform vec3 helicopterPosition;

// Output
out vec3 position;


void main()
{
    position = v_position;

    // Calculate noise value for the vertex position in world space.
    vec4 world_position = Model * vec4(v_position, 1.0);
    vec2 noiseCoords = vec2((world_position.x + terrainSize / 2) / terrainSize, (world_position.z + terrainSize / 2) / terrainSize);

    vec4 height = texture2D(noise, noiseCoords);
    mat4 noiseTranslation = mat4(1, 0, 0, 0,
                                 0, 1, 0, 0,
                                 0, 0, 1, 0,
                                 0, height.r * 10 - 0.75, 0, 1);
    
    // Apply curve
    world_position.y = world_position.y - pow(length(helicopterPosition - vec3(world_position)), 2) * 0.02;

    // Apply noise
    gl_Position = Projection * View * noiseTranslation * world_position;
}
