#version 450 core

in vec3 TexCoords;
in vec3 Normal;
in vec3 Pos;

out vec4 FragColor;

uniform samplerCube skybox;
uniform vec3 cameraPos;

void main()
{
     vec3 viewDir = normalize(Pos - cameraPos);
     vec3 reflDir = reflect(viewDir, normalize(Normal));
     FragColor = vec4(texture(skybox, reflDir).rgb, 1.0);
     //FragColor = texture(skybox, TexCoords);
}