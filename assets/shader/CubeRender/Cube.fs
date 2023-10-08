#version 450 core

in vec3 Pos;
in vec3 Normal;
in vec2 TexCoords;

uniform samplerCube Tex;
uniform vec4 CubeColor;
uniform vec3 cameraPos;

out vec4 FragColor;

void main()
{
    FragColor = CubeColor;
}