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
uniform vec3 helicopterPosition;
uniform int terrainSize;


void main()
{
    // Calculate noise value for the helicopter position.
    vec2 noiseCoords = vec2(float((helicopterPosition.x + float(terrainSize / 2)) / terrainSize),
                            float((helicopterPosition.z + float(terrainSize / 2)) / terrainSize));
    vec4 height = texture2D(noise, noiseCoords);
    mat4 noiseTranslation = mat4(1, 0, 0, 0,
                                 0, 1, 0, 0,
                                 0, 0, 1, 0,
                                 0, height.r * 10 + 1, 0, 1);
    
    // Apply noise.
    gl_Position = Projection * View * noiseTranslation * Model * vec4(v_position, 1.0);
}
