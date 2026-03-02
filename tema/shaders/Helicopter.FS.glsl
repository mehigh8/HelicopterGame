#version 430
// Rosu Mihai Cosmin GMRV1A

// Uniform properties
uniform sampler2D texture_1;
uniform sampler2D texture_2;
uniform bool helicopterDrawBody;
uniform vec3 helicopterBodyColor;
uniform vec3 helicopterPropellerColor;

// Output
layout(location = 0) out vec4 out_color;

void main()
{
    if (helicopterDrawBody) 
        out_color = vec4(helicopterBodyColor, 1);
    else
        out_color = vec4(helicopterPropellerColor, 1);
}
