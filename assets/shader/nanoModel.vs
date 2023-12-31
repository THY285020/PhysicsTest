#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Proj;

void main()
{
	TexCoords = aTexCoords;
	gl_Position = Proj * View * Model * vec4(aPos, 1.0f);
}