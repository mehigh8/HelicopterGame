#version 430
// Rosu Mihai Cosmin GMRV1A

// Input
in vec2 texture_coord;
in vec3 modified_position;

// Uniform properties
uniform sampler2D texture_1;
uniform sampler2D texture_2;
uniform sampler2D noise;
uniform int terrainSize;

// Output
layout(location = 0) out vec4 out_color;
layout(location = 1) out vec4 out_info;

float inverseLerp(float a, float b, float v)
{
    return (v - a) / (b - a);
}

void main()
{
    vec4 ground = texture2D(texture_1, texture_coord);
    vec4 snow = texture2D(texture_2, texture_coord);

    vec4 height = texture2D(noise, texture_coord);
    vec4 color;

    // Use textures based on height.
    if (height.r < 0.6) {
        color = ground;
    } else if (height.r < 0.7) {
        float t = inverseLerp(0.6, 0.7, height.r);
        color = mix(ground, snow, t);
    } else {
        color = snow;
    }

    vec4 heightGradient = vec4(height.r, height.r, height.r, 1.0);
    color = mix(color, heightGradient, 0.5);

    out_color = color;

    // For the second color texture, the value is based on the world space position. It is scaled as the color only accepts values
    // between 0 and 1.
    out_info = vec4((modified_position.x + terrainSize / 2) / terrainSize, 0, (modified_position.z + terrainSize / 2) / terrainSize, 0);
}
