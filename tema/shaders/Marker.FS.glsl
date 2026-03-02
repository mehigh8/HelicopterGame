#version 430
// Rosu Mihai Cosmin GMRV1A

uniform vec3 marker_color;

// Output
layout(location = 0) out vec4 out_color;

void main()
{
    out_color = vec4(marker_color, 1);
}
