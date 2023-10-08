#version 450 core

in vec3 Normal;
in vec3 Pos;
in vec2 TexCoords;

uniform sampler2D Tex;
uniform vec4 PlaneColor;
uniform vec3 cameraPos;

out vec4 FragColor;

void main()
{
    FragColor = PlaneColor;
}