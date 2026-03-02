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

// Output
out vec2 texture_coord;
out vec3 modified_position;


void main()
{
    texture_coord = v_texture_coord;

    vec4 height = texture2D(noise, v_texture_coord);
    // Apply noise
    modified_position = v_position + vec3(0, height.r * 10, 0);
    // Apply curve
    modified_position.y = modified_position.y - pow(length(helicopterPosition - modified_position), 2) * 0.02;

    gl_Position = Projection * View * Model * vec4(modified_position, 1.0);
}
