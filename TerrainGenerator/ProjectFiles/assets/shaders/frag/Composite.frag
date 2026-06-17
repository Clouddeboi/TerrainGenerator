#version 330 core

in vec2 vTexCoord;

uniform sampler2D u_ScreenTexture;

//Optional effects
uniform bool u_EnableScanlines = true;
uniform bool u_EnableVignette = true;

out vec4 FragColor;

void main()
{
    vec4 colour = texture(u_ScreenTexture, vTexCoord);

    FragColor = colour;
}