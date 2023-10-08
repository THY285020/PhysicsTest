#version 450 core

in vec3 Normal;
in vec2 TexCoords;
in vec3 Pos;

uniform sampler2D Tex;
uniform vec4 SphereColor;
uniform vec3 cameraPos;

out vec4 FragColor;

void main()
{
    FragColor = SphereColor;
}