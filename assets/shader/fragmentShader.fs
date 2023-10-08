#version 450 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

//uniform vec3 viewPos;
uniform sampler2D texture0;

out vec4 FragColor;

void main()
{
	FragColor = texture(texture0, TexCoords);
	//FragColor = vec4(Normal, 1.0f);
}